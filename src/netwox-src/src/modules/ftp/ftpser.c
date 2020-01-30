/*
                                  NETWOX
                             Network toolbox
                Copyright(c) 1999-2012 Laurent CONSTANTIN
                      http://ntwox.sourceforge.net/
                        laurentconstantin@free.fr
                                  -----

  This file is part of Netwox.

  Netwox is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3
  as published by the Free Software Foundation.

  Netwox is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details (http://www.gnu.org/).

------------------------------------------------------------------------
*/

/*-------------------------------------------------------------*/
#include "../../netwox.h"

/*-------------------------------------------------------------*/
/* Be strict in decoding:
     CMD <SP> para
     CMD
   Note: we are strict because para can be a filename starting
         with a space.
 */
netwib_err netwox_ftpser_decode_line(netwib_constbuf *pbuf,
                                     netwib_bufext *pcmd,
                                     netwib_bufext *ppara)
{
  netwib_buf buf;
  netwib_data data, pc;
  netwib_uint32 datasize;

  datasize = netwib__buf_ref_data_size(pbuf);
  if (datasize == 0) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  data = netwib__buf_ref_data_ptr(pbuf);

  pc = netwib_c_memchr(data, ' ', datasize);
  if (pc != NULL) {
    buf = *pbuf;
    buf.endoffset = buf.beginoffset + pc - data;
    netwib_er(netwib_buf_init_ext_buf(&buf, pcmd));
    buf = *pbuf;
    buf.beginoffset += pc - data + 1;
    netwib_er(netwib_buf_init_ext_buf(&buf, ppara));
  } else {
    netwib_er(netwib_buf_init_ext_buf(pbuf, pcmd));
    netwib_er(netwib_buf_init_ext_empty(ppara));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_path(netwib_bufpool *pbufpool,
                              netwib_constbuf *prootdir,
                              netwib_constbuf *pvirtcwd, /* absolute */
                              netwib_constbuf *pvirtwantedpath, /* abs or
                                                            relative to cwd */
                              netwib_buf *pvirtabspath,
                              netwib_buf *prealpath)
{
  netwib_buf *pvirtwantedpath2, *pvirtabspath2;
  netwib_pathtype pathtype;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pvirtwantedpath2));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pvirtabspath2));

  /* canonize pvirtwantedpath, to ensure check for absolute is correct */
  netwib_eg(netwib_path_canon(pvirtwantedpath, pvirtwantedpath2));

  /* set absolute virtual path */
  netwib_eg(netwib_pathtype_init(pvirtwantedpath2, &pathtype));
  if (pathtype & NETWIB_PATHTYPE_ABSOLUTE) {
    netwib_eg(netwib_buf_append_buf(pvirtwantedpath2, pvirtabspath2));
  } else {
    /* pvirtcwd is absolute, so if pvirtwantedpath contains too
       many ../, an error occurs here */
    netwib_eg(netwib_path_init_concat(pvirtcwd, pvirtwantedpath2,
                                      pvirtabspath2));
  }
  netwib_eg(netwib_buf_append_buf(pvirtabspath2, pvirtabspath));

  /* obtain real path (path of wantedpath on disk) */
  if (netwib__buf_ref_data_size(pvirtabspath2) == 1) {
    /* there is only the '/' */
    netwib_eg(netwib_buf_append_buf(prootdir, prealpath));
  } else {
    pvirtabspath2->beginoffset++;
    netwib_eg(netwib_path_init_concat(prootdir, pvirtabspath2, prealpath));
  }

 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pvirtabspath2));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pvirtwantedpath2));
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_ftpser_dir_quoted(netwib_constbuf *pdir,
                                           netwib_conststring msg,
                                           netwib_buf *pbuf)
{
  netwib_data data;
  netwib_uint32 datasize;

  data = netwib__buf_ref_data_ptr(pdir);
  datasize = netwib__buf_ref_data_size(pdir);

  netwib_er(netwib_buf_append_string("\"", pbuf));

  while (datasize--) {
    if (*data == '"') {
      netwib_er(netwib_buf_append_byte('"', pbuf));
    }
    netwib_er(netwib_buf_append_byte(*data, pbuf));
    data++;
  }

  netwib_er(netwib_buf_append_string("\" ", pbuf));
  netwib_er(netwib_buf_append_string(msg, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_user(netwib_io *pctlio,
                              netwib_constbuf *pwanteduser,
                              netwib_bool passwordok,
                              netwib_constbuf *puser,
                              netwib_bool *puserok)
{
  netwib_bool userok;
  netwib_cmp cmp;

  userok = NETWIB_FALSE;
  if (netwib__buf_ref_data_size(pwanteduser) == 0) {
    userok = NETWIB_TRUE;
  }
  if (!userok) {
    netwib_er(netwib_buf_cmp(puser, pwanteduser, &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      userok = NETWIB_TRUE;
    }
  }
  if (puserok != NULL) *puserok = userok;

  if (userok && passwordok) {
    netwib_er(netwox_txtproto_write_reply_text(pctlio, NETWOX_FTPREPLY_230_AUTHOK,
                                               "User logged in, proceed."));
  } else {
    netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                               NETWOX_FTPREPLY_331_AUTHUSEROK,
                                              "User name ok, need password."));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_pass(netwib_io *pctlio,
                              netwib_bool userok,
                              netwib_constbuf *pwantedpassword,
                              netwib_constbuf *ppassword,
                              netwib_bool *ppasswordok)
{
  netwib_bool passwordok;
  netwib_cmp cmp;

  passwordok = NETWIB_FALSE;
  if (netwib__buf_ref_data_size(pwantedpassword) == 0) {
    passwordok = NETWIB_TRUE;
  }
  if (!passwordok) {
    netwib_er(netwib_buf_cmp(ppassword, pwantedpassword, &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      passwordok = NETWIB_TRUE;
    }
  }
  if (ppasswordok != NULL) *ppasswordok = passwordok;

  if (userok && passwordok) {
    netwib_er(netwox_txtproto_write_reply_text(pctlio, NETWOX_FTPREPLY_230_AUTHOK,
                                               "User logged in, proceed."));
  } else {
    netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                               NETWOX_FTPREPLY_530_AUTHBAD,
                                               "Not logged in."));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_quit(netwib_io *pctlio)
{
  netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                             NETWOX_FTPREPLY_221_SVCCLOSING,
                                       "Service closing control connection."));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_syst(netwib_io *pctlio)
{
  netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                             NETWOX_FTPREPLY_215_SYST,
                                             "UNIX Type: L8"));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_cwd(netwib_bufpool *pbufpool,
                             netwib_constbuf *prootdir,
                             netwib_buf *pcwd,
                             netwib_io *pctlio,
                             netwib_constbuf *pwanteddir)
{
  netwib_buf *pvirtdir, *prealdir, *pmsg;
  netwib_bool exists;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pvirtdir));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &prealdir));
  ret = netwox_ftpser_path(pbufpool, prootdir, pcwd, pwanteddir,
                           pvirtdir, prealdir);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                          NETWOX_FTPREPLY_550_ACTIONNOTTAKEN,
                                               "Bad path."));
  } else {
    netwib_er(netwib_dirname_exists(prealdir, &exists));
    if (exists) {
      netwib__buf_reinit(pcwd);
      netwib_er(netwib_buf_append_buf(pvirtdir, pcwd));
      netwib_er(netwib_bufpool_buf_init(pbufpool, &pmsg));
      netwib_er(netwib_buf_append_string("Current directory is now ", pmsg));
      netwib_er(netwib_buf_append_buf(pvirtdir, pmsg));
      netwib_er(netwox_txtproto_write_reply(pctlio,
                                            NETWOX_FTPREPLY_250_FILEACTIONDONE,
                                            pmsg));
      netwib_er(netwib_bufpool_buf_close(pbufpool, &pmsg));
    } else {
      netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                            NETWOX_FTPREPLY_550_ACTIONNOTTAKEN,
                                                 "Path is not a directory."));
    }
  }
  netwib_er(netwib_bufpool_buf_close(pbufpool, &prealdir));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pvirtdir));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_pwd(netwib_bufpool *pbufpool,
                             netwib_constbuf *prootdir,
                             netwib_buf *pcwd,
                             netwib_io *pctlio)
{
  netwib_buf *pvirtdir, *prealdir, *pmsg, dir;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pvirtdir));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &prealdir));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pmsg));
  netwib_er(netwib_buf_init_ext_string(".", &dir));
  ret = netwox_ftpser_path(pbufpool, prootdir, pcwd, &dir, pvirtdir, prealdir);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                           NETWOX_FTPREPLY_550_ACTIONNOTTAKEN,
                                               "Bad path."));
  } else {
    netwib_er(netwox_ftpser_dir_quoted(pvirtdir, "is current directory",
                                       pmsg));
    netwib_er(netwox_txtproto_write_reply(pctlio, NETWOX_FTPREPLY_257_FILECREATED,
                                          pmsg));
  }
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pmsg));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &prealdir));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pvirtdir));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_cdup(netwib_bufpool *pbufpool,
                              netwib_constbuf *prootdir,
                              netwib_buf *pcwd,
                              netwib_io *pctlio)
{
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string("..", &buf));
  ret = netwox_ftpser_cwd(pbufpool, prootdir, pcwd, pctlio, &buf);

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_type(netwib_io *pctlio,
                              netwib_constbuf *ptype,
                              netwib_bool *pascii)
{
  netwib_data data;
  netwib_byte type;

  /* rfc 959 chapter 3.1.1.5 describes all types.
     This program only supports (page 28):
       A (parameter N, T or C ignored)
       I
  */
  type = '\0';
  if (netwib__buf_ref_data_size(ptype)) {
    data = netwib__buf_ref_data_ptr(ptype);
    type = *data;
  }

  if (type == 'A' || type == 'a') {
    netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                               NETWOX_FTPREPLY_200_CMDOK,
                                               "Type is ascii."));
    if (pascii != NULL) *pascii = NETWIB_TRUE;
  } else if (type == 'I' || type == 'i') {
    netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                               NETWOX_FTPREPLY_200_CMDOK,
                                               "Type is binary."));
    if (pascii != NULL) *pascii = NETWIB_FALSE;
  } else {
    netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                        NETWOX_FTPREPLY_504_CMDOKBADPARAMETER,
                                               "Unsupported type."));
    /* pascii not changed */
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_stru(netwib_io *pctlio,
                              netwib_constbuf *pstru)
{
  netwib_data data;
  netwib_byte stru;

  stru = '\0';
  if (netwib__buf_ref_data_size(pstru)) {
    data = netwib__buf_ref_data_ptr(pstru);
    stru = *data;
  }

  if (stru == 'F' || stru == 'f') {
    netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                               NETWOX_FTPREPLY_200_CMDOK,
                                               "Structure is file."));
  } else {
    netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                        NETWOX_FTPREPLY_504_CMDOKBADPARAMETER,
                                               "Unsupported structure."));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_mode(netwib_io *pctlio,
                              netwib_constbuf *pmode)
{
  netwib_data data;
  netwib_byte mode;

  mode = '\0';
  if (netwib__buf_ref_data_size(pmode)) {
    data = netwib__buf_ref_data_ptr(pmode);
    mode = *data;
  }

  if (mode == 'S' || mode == 's') {
    netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                               NETWOX_FTPREPLY_200_CMDOK,
                                               "Mode is stream."));
  } else {
    netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                        NETWOX_FTPREPLY_504_CMDOKBADPARAMETER,
                                               "Unsupported mode."));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_rnfr(netwib_bufpool *pbufpool,
                              netwib_constbuf *prootdir,
                              netwib_buf *pcwd,
                              netwib_buf *prnfr,
                              netwib_io *pctlio,
                              netwib_constbuf *ppara)
{
  netwib_buf *pvirtfile, *prealfile;
  netwib_bool exists;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pvirtfile));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &prealfile));
  ret = netwox_ftpser_path(pbufpool, prootdir, pcwd, ppara,
                           pvirtfile, prealfile);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                       NETWOX_FTPREPLY_550_ACTIONNOTTAKEN,
                                               "Bad path."));
  } else {
    netwib_er(netwib_filename_exists(prealfile, &exists));
    if (!exists) {
      netwib_er(netwib_dirname_exists(prealfile, &exists));
    }
    if (exists) {
      netwib__buf_reinit(prnfr);
      netwib_er(netwib_buf_append_buf(prealfile, prnfr));
      netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                         NETWOX_FTPREPLY_350_FILEACTIONPENDING,
                                                "Now, enter a RNTO command."));
    } else {
      netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                       NETWOX_FTPREPLY_550_ACTIONNOTTAKEN,
                                            "Path is not a file/directory."));
    }
  }
  netwib_er(netwib_bufpool_buf_close(pbufpool, &prealfile));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pvirtfile));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_rnto(netwib_bufpool *pbufpool,
                              netwib_constbuf *prootdir,
                              netwib_buf *pcwd,
                              netwib_buf *prnfr,
                              netwib_io *pctlio,
                              netwib_constbuf *prnto)
{
  netwib_buf *pvirtfile, *prealfile;
  netwib_pathstat pathstat;
  netwib_err ret;

  if (netwib__buf_ref_data_size(prnfr) == 0) {
    netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                        NETWOX_FTPREPLY_503_CMDBADSEQUENCE,
                                               "Please, use RNFR before."));
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pvirtfile));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &prealfile));
  ret = netwox_ftpser_path(pbufpool, prootdir, pcwd, prnto,
                           pvirtfile, prealfile);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                        NETWOX_FTPREPLY_553_ACTIONNOTTAKEN,
                                               "Bad path."));
  } else {
    netwib_er(netwib_pathstat_init(prnfr, &pathstat));
    switch(pathstat.type) {
    case NETWIB_PATHSTAT_TYPE_REG :
      ret = netwib_filename_rename(prnfr, prealfile);
      break;
    case NETWIB_PATHSTAT_TYPE_DIR :
      ret = netwib_dirname_rename(prnfr, prealfile);
      break;
    default :
      ret = NETWOX_ERR_NOTIMPLEMENTED;
    }
    netwib__buf_reinit(prnfr);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                           NETWOX_FTPREPLY_553_ACTIONNOTTAKEN,
                                             "File/directory not renamed."));
    } else {
      netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                          NETWOX_FTPREPLY_250_FILEACTIONDONE,
                                                 "File/directory renamed."));
    }
  }
  netwib_er(netwib_bufpool_buf_close(pbufpool, &prealfile));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pvirtfile));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_dele(netwib_bufpool *pbufpool,
                              netwib_constbuf *prootdir,
                              netwib_buf *pcwd,
                              netwib_io *pctlio,
                              netwib_constbuf *pfile)
{
  netwib_buf *pvirtfile, *prealfile;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pvirtfile));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &prealfile));
  ret = netwox_ftpser_path(pbufpool, prootdir, pcwd, pfile,
                           pvirtfile, prealfile);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                           NETWOX_FTPREPLY_550_ACTIONNOTTAKEN,
                                               "Bad path."));
  } else {
    ret = netwib_filename_remove(prealfile);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                           NETWOX_FTPREPLY_550_ACTIONNOTTAKEN,
                                                "File not deleted."));
    } else {
      netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                          NETWOX_FTPREPLY_250_FILEACTIONDONE,
                                                 "File deleted."));
    }
  }
  netwib_er(netwib_bufpool_buf_close(pbufpool, &prealfile));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pvirtfile));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_size(netwib_bufpool *pbufpool,
                              netwib_constbuf *prootdir,
                              netwib_buf *pcwd,
                              netwib_io *pctlio,
                              netwib_constbuf *pfile)
{
  netwib_buf *pvirtfile, *prealfile, *pmsg;
  netwib_bool exists;
  netwib_uint32 size;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pvirtfile));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &prealfile));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pmsg));
  ret = netwox_ftpser_path(pbufpool, prootdir, pcwd, pfile,
                           pvirtfile, prealfile);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                           NETWOX_FTPREPLY_550_ACTIONNOTTAKEN,
                                               "Bad path."));
  } else {
    netwib_er(netwib_filename_exists(prealfile, &exists));
    if (!exists) {
      netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                           NETWOX_FTPREPLY_550_ACTIONNOTTAKEN,
                                                "File not found."));
    } else {
      ret = netwib_filename_size(prealfile, &size);
      if (ret != NETWIB_ERR_OK) {
        netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                           NETWOX_FTPREPLY_550_ACTIONNOTTAKEN,
                                                   "File size not obtained."));
      } else {
        netwib_er(netwib_buf_append_fmt(pmsg, "%{uint32}", size));
        netwib_er(netwox_txtproto_write_reply(pctlio,
                                              NETWOX_FTPREPLY_213_FILESTATUS,
                                              pmsg));
      }
    }
  }
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pmsg));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &prealfile));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pvirtfile));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_rmd(netwib_bufpool *pbufpool,
                             netwib_constbuf *prootdir,
                             netwib_buf *pcwd,
                             netwib_io *pctlio,
                             netwib_constbuf *pdir)
{
  netwib_buf *pvirtdir, *prealdir;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pvirtdir));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &prealdir));
  ret = netwox_ftpser_path(pbufpool, prootdir, pcwd, pdir, pvirtdir, prealdir);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                           NETWOX_FTPREPLY_550_ACTIONNOTTAKEN,
                                               "Bad path."));
  } else {
    ret = netwib_dirname_remove(prealdir);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                           NETWOX_FTPREPLY_550_ACTIONNOTTAKEN,
                                                "Directory not deleted."));
    } else {
      netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                          NETWOX_FTPREPLY_250_FILEACTIONDONE,
                                                 "Directory deleted."));
    }
  }
  netwib_er(netwib_bufpool_buf_close(pbufpool, &prealdir));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pvirtdir));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_mkd(netwib_bufpool *pbufpool,
                             netwib_constbuf *prootdir,
                             netwib_buf *pcwd,
                             netwib_io *pctlio,
                             netwib_constbuf *pdir)
{
  netwib_buf *pvirtdir, *prealdir, *pmsg;
  netwib_bool exists;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pvirtdir));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &prealdir));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pmsg));
  ret = netwox_ftpser_path(pbufpool, prootdir, pcwd, pdir, pvirtdir, prealdir);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                           NETWOX_FTPREPLY_550_ACTIONNOTTAKEN,
                                               "Bad path."));
  } else {
    netwib_er(netwib_dirname_exists(prealdir, &exists));
    if (exists) {
      netwib_er(netwox_ftpser_dir_quoted(pvirtdir, "directory already exists",
                                         pmsg));
      netwib_er(netwox_txtproto_write_reply(pctlio,
                                            NETWOX_FTPREPLY_521_DIREXISTS,
                                            pmsg));
    } else {
      ret = netwib_dirname_create(prealdir);
      if (ret != NETWIB_ERR_OK) {
        netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                           NETWOX_FTPREPLY_550_ACTIONNOTTAKEN,
                                                "Directory not created."));
      } else {
      netwib_er(netwox_ftpser_dir_quoted(pvirtdir, "directory created", pmsg));
        netwib_er(netwox_txtproto_write_reply(pctlio,
                                              NETWOX_FTPREPLY_257_FILECREATED,
                                              pmsg));
      }
    }
  }
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pmsg));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &prealdir));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pvirtdir));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_noop(netwib_io *pctlio)
{

  netwib_er(netwox_txtproto_write_reply_text(pctlio, NETWOX_FTPREPLY_200_CMDOK,
                                             "No operation done."));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_ftpser_active(netwox_sockinfo *pmainsockinfo,
                                            netwib_ips *pallowedclients,
                                            netwib_constip *premoteip,
                                            netwib_port remoteport,
                                            netwib_io *pctlio,
                                            netwib_io **ppdataio)
{
  netwib_bool allowed;
  netwox_sockinfo newsockinfo;
  netwib_err ret;

  /* check allowed clients */
  if (pallowedclients != NULL) {
    netwib_er(netwib_ips_contains_ip(pallowedclients, premoteip, &allowed));
    if (!allowed) {
      netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                           NETWOX_FTPREPLY_501_PARAMETERERROR,
                                           "This IP address is forbidden."));
      return(NETWIB_ERR_OK);
    }
  }

  /* try to connect */
  netwib_er(netwox_sockinfo_init_connect(pmainsockinfo, pctlio,
                                         remoteport, &newsockinfo));
  newsockinfo.remoteip = *premoteip;
  if (premoteip->iptype != newsockinfo.localip.iptype) {
    newsockinfo.plocalip = NULL;
  }
  /* port is generally 20, but it corresponds to 21-1 */
  newsockinfo.localport = pmainsockinfo->localport-1;
  ret = netwox_sock_init(&newsockinfo, ppdataio);
  netwib_er(netwox_sockinfo_close(&newsockinfo));

  /* return message */
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                           NETWOX_FTPREPLY_501_PARAMETERERROR,
                                               "Could not connect."));
  } else {
    netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                               NETWOX_FTPREPLY_200_CMDOK,
                                               "Connected."));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_port(netwox_sockinfo *pmainsockinfo,
                              netwib_ips *pallowedclients,
                              netwib_constbuf *phostport,
                              netwib_io *pctlio,
                              netwib_io **ppdataio)
{
  netwib_uint32 a, b, c, d, e, f;
  netwib_ip remoteip;
  netwib_port remoteport;
  netwib_err ret;

  /* decode h,h,h,h,p,p */
  ret = netwib_buf_decode_fmt(phostport, "%{uint32},%{uint32},%{uint32},%{uint32},%{uint32},%{uint32}%$", &a, &b, &c, &d, &e, &f);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                           NETWOX_FTPREPLY_501_PARAMETERERROR,
                                               "Host,port not recognized."));
    return(NETWIB_ERR_OK);
  }
  netwib_er(netwib_ip_init_ip4(netwib_c2_uint32_4(a, b, c, d), &remoteip));
  remoteport = netwib_c2_uint16_2(e, f);

  /* connect on client */
  netwib_er(netwox_priv_ftpser_active(pmainsockinfo, pallowedclients,
                                      &remoteip, remoteport, pctlio,
                                      ppdataio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_eprt(netwox_sockinfo *pmainsockinfo,
                              netwib_ips *pallowedclients,
                              netwib_constbuf *phostport,
                              netwib_io *pctlio,
                              netwib_io **ppdataio)
{
  netwib_uint32 type;
  netwib_ip remoteip;
  netwib_port remoteport;
  netwib_err ret;

  /* decode host and port */
  ret = netwib_buf_decode_fmt(phostport, "|%{uint32}|%{ip}|%{port}|%$",
                              &type, &remoteip, &remoteport);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                           NETWOX_FTPREPLY_501_PARAMETERERROR,
                                           "Host and port not recognized."));
    return(NETWIB_ERR_OK);
  }
  switch(type) {
  case 1 :
    if (remoteip.iptype != NETWIB_IPTYPE_IP4) {
      netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                           NETWOX_FTPREPLY_501_PARAMETERERROR,
                                           "It is not an IPv4 address."));
      return(NETWIB_ERR_OK);
    }
    break;
  case 2 :
    if (remoteip.iptype != NETWIB_IPTYPE_IP6) {
      netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                           NETWOX_FTPREPLY_501_PARAMETERERROR,
                                           "It is not an IPv6 address."));
      return(NETWIB_ERR_OK);
    }
    break;
  default :
    netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                   NETWOX_FTPREPLY_501_PARAMETERERROR,
                                   "This type of address is not supported."));
    return(NETWIB_ERR_OK);
    break;
  }

  /* connect on client */
  netwib_er(netwox_priv_ftpser_active(pmainsockinfo, pallowedclients,
                                      &remoteip, remoteport, pctlio,
                                      ppdataio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_ftpser_passive(netwox_sockinfo *pmainsockinfo,
                                             netwib_io *pctlio,
                                             netwib_ip *plocalip,
                                             netwib_port *plocalport,
                                             netwib_io **ppdataio)
{
  netwox_sockinfo newsockinfo;
  netwib_err ret;

  netwib_er(netwox_sockinfo_init_listen(pmainsockinfo, pctlio,
                                        0, &newsockinfo));
  while(NETWIB_TRUE) {
    netwib_er(netwib_uint32_init_rand(1024, 0xFFFF, &newsockinfo.localport));
    ret = netwox_sock_init(&newsockinfo, ppdataio);
    if (ret == NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwox_sockinfo_close(&newsockinfo));

  if (plocalip) *plocalip = newsockinfo.localip;
  if (plocalport) *plocalport = newsockinfo.localport;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_pasv(netwib_bufpool *pbufpool,
                              netwox_sockinfo *pmainsockinfo,
                              netwib_io *pctlio,
                              netwib_io **ppdataio)
{
  netwib_buf *pmsg;
  netwib_ip localip;
  netwib_ip4 localip4;
  netwib_port localport;
  netwib_err ret;

  /* listen on port */
  netwib_er(netwox_priv_ftpser_passive(pmainsockinfo, pctlio,
                                       &localip, &localport, ppdataio));
  if (localip.iptype != NETWIB_IPTYPE_IP4) {
    /* if we listen on an IPv6 address such as "::", an IPv4 client
       can connect. In this case, ctl_get_local_ip returns an IPv6
       address like ::ffff:1.2.3.4, which has to be converted to
       1.2.3.4. */
    ret = netwib_ip4_init_ip(&localip, &localip4);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                   NETWOX_FTPREPLY_550_ACTIONNOTTAKEN,
                                   "PASV command only supports IPv4."));
      netwib_er(netwib_io_close(ppdataio));
      return(NETWIB_ERR_OK);
    }
    netwib_er(netwib_ip_init_ip4(localip4, &localip));
  }

  /* tell to client */
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pmsg));
  netwib_er(netwib_buf_append_fmt(pmsg, "Entering Passive Mode (%{uint32},%{uint32},%{uint32},%{uint32},%{uint32},%{uint32})", netwib_c2_uint32_0(localip.ipvalue.ip4), netwib_c2_uint32_1(localip.ipvalue.ip4), netwib_c2_uint32_2(localip.ipvalue.ip4), netwib_c2_uint32_3(localip.ipvalue.ip4), netwib_c2_uint16_0(localport), netwib_c2_uint16_1(localport)));
  netwib_er(netwox_txtproto_write_reply(pctlio,
                                       NETWOX_FTPREPLY_227_ENTERINGPASSIVEMODE,
                                        pmsg));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pmsg));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_epsv(netwib_bufpool *pbufpool,
                              netwox_sockinfo *pmainsockinfo,
                              netwib_io *pctlio,
                              netwib_io **ppdataio)
{
  netwib_buf *pmsg;
  netwib_ip localip;
  netwib_port localport;

  /* listen on port */
  netwib_er(netwox_priv_ftpser_passive(pmainsockinfo, pctlio,
                                       &localip, &localport, ppdataio));

  /* tell to client */
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pmsg));
  netwib_er(netwib_buf_append_fmt(pmsg, "Entering Extended Passive Mode (|||%{port}|)", localport));
  netwib_er(netwox_txtproto_write_reply(pctlio,
                                   NETWOX_FTPREPLY_229_ENTERINGPASSIVEMODEEXT,
                                        pmsg));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pmsg));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_ftpser_list_file(netwib_bufpool *pbufpool,
                                               netwib_constbuf *pfilename,
                                               netwib_io *pdataio,
                                               netwib_bool longdisplay,
                                               netwib_uint32 currentyear)
{
  netwib_buf *pline, *pendpath;
  netwib_pathstat pathstat;
  netwib_localtime lt;
  netwib_char c;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pline));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pendpath));

  netwib_er(netwib_path_decode_child(pfilename, pendpath));

  if (longdisplay) {
    netwib_eg(netwib_pathstat_init(pfilename, &pathstat));
    switch(pathstat.type) {
    case NETWIB_PATHSTAT_TYPE_REG : c = '-'; break;
    case NETWIB_PATHSTAT_TYPE_DIR : c = 'd'; break;
    case NETWIB_PATHSTAT_TYPE_LINK : c = 'l'; break;
    case NETWIB_PATHSTAT_TYPE_SOCK : c = 's'; break;
    case NETWIB_PATHSTAT_TYPE_BLOCK : c = 'b'; break;
    case NETWIB_PATHSTAT_TYPE_CHAR : c = 'c'; break;
    case NETWIB_PATHSTAT_TYPE_FIFO : c = 'f'; break;
    default : c = '-'; break;
    }
    netwib_er(netwib_buf_append_byte(c, pline));
    /* fake values, but I don't think it's really important for now */
    netwib_er(netwib_buf_append_string("rwxrwxrwx 1 ftp ftp ", pline));
    netwib_er(netwib_buf_append_fmt(pline, "%{r 9;uint32}", pathstat.size));
    netwib_er(netwib_time_decode_localtime(&pathstat.mtime, &lt));
    netwib_er(netwib_buf_append_string(" ", pline));
    netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_MON_SHORT,
                                          pline));
    netwib_er(netwib_buf_append_string(" ", pline));
    netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_MDAY_SPACE,
                                          pline));
    netwib_er(netwib_buf_append_string(" ", pline));
    if (lt.year == currentyear) {
      netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_HOUR_ZERO,
                                            pline));
      netwib_er(netwib_buf_append_string(":", pline));
      netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_MIN_ZERO,
                                            pline));
    } else {
      netwib_er(netwib_buf_append_string(" ", pline));
      netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_YEAR_LONG,
                                            pline));
    }
    netwib_er(netwib_buf_append_string(" ", pline));
    netwib_er(netwib_buf_append_buf(pendpath, pline));
    if (pathstat.type == NETWIB_PATHSTAT_TYPE_LINK) {
      netwib_er(netwib_buf_append_string(" -> ", pline));
      netwib_eg(netwib_unix_readlink(pfilename, pline));
    }
  } else {
    netwib_er(netwib_buf_append_buf(pendpath, pline));
  }
  netwib_er(netwib_buf_append_string("\r\n", pline));

  netwib_er(netwib_io_write(pdataio, pline));

 netwib_gotolabel:
  /* on error, simply do not display line */
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pendpath));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pline));
  ret = ret; /* Ignore. */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_ftpser_list_dir(netwib_bufpool *pbufpool,
                                              netwib_constbuf *pdirname,
                                              netwib_io *pdataio,
                                              netwib_bool longdisplay,
                                              netwib_uint32 currentyear)
{
  netwib_buf *pfilename, *prealfilename;
  netwib_dir *pdir;
  netwib_err ret;

  ret = netwib_dir_init(pdirname, &pdir);
  if (ret != NETWIB_ERR_OK) {
    /* display nothing */
    return(NETWIB_ERR_OK);
  }
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pfilename));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &prealfilename));

  while (NETWIB_TRUE) {
    netwib__buf_reinit(pfilename);
    ret = netwib_dir_next(pdir, pfilename);
    if (ret == NETWIB_ERR_DATAEND) {
      break;
    }
    netwib__buf_reinit(prealfilename);
    netwib_er(netwib_path_init_concat(pdirname, pfilename, prealfilename));
    netwib_er(netwox_priv_ftpser_list_file(pbufpool, prealfilename, pdataio,
                                           longdisplay, currentyear));
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &prealfilename));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pfilename));
  netwib_er(netwib_dir_close(&pdir));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_list(netwib_bufpool *pbufpool,
                              netwib_constbuf *prootdir,
                              netwib_buf *pcwd,
                              netwib_io *pctlio,
                              netwib_io **ppdataio,
                              netwib_constbuf *pfile,
                              netwib_bool longdisplay)
{
  netwib_buf *pvirtfile, *prealfile, file;
  netwib_constbuf *pfile2;
  netwib_bool exists;
  netwib_time t;
  netwib_localtime lt;
  netwib_err ret;

  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_decode_localtime(&t, &lt));

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pvirtfile));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &prealfile));

  pfile2 = pfile;
  if (netwib__buf_ref_data_size(pfile) == 0) {
    netwib_er(netwib_buf_init_ext_string(".", &file));
    pfile2 = &file;
  }
  ret = netwox_ftpser_path(pbufpool, prootdir, pcwd, pfile2,
                           pvirtfile, prealfile);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                        NETWOX_FTPREPLY_450_FILEACTIONNOTTAKEN,
                                               "Bad path."));
  } else {
    netwib_er(netwib_pathname_exists(prealfile, &exists));
    if (exists) {
      netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                              NETWOX_FTPREPLY_150_WILLOPENDATA,
                                                 "Start of file list."));
      netwib_er(netwib_dirname_exists(prealfile, &exists));
      if (exists) {
        netwib_er(netwox_priv_ftpser_list_dir(pbufpool, prealfile, *ppdataio,
                                              longdisplay, lt.year));
      } else {
        netwib_er(netwox_priv_ftpser_list_file(pbufpool, prealfile, *ppdataio,
                                               longdisplay, lt.year));
      }
      netwib_er(netwib_io_close(ppdataio));
      netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                                 NETWOX_FTPREPLY_226_DATAEND,
                                                 "Transfer complete."));
    } else {
      netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                        NETWOX_FTPREPLY_450_FILEACTIONNOTTAKEN,
                                        "This path does not exits."));
    }
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &prealfile));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pvirtfile));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_ftpser_data(netwib_bufpool *pbufpool,
                                          netwib_io *pinio,
                                          netwib_io *poutio,
                                          netwib_bool ascii,
                                          netwib_bool outnetwork)
{
  netwib_io *pinio2=NULL, *poutio2=NULL;
  netwib_buf *pbuf;
  netwib_err ret=NETWIB_ERR_OK;

  /* init */
  if (ascii) {
    netwib_er(netwib_io_init_data_line(&pinio2));
    netwib_er(netwib_io_plug_read(pinio2, pinio));
    netwib_er(netwib_io_init_data_line(&poutio2));
    if (outnetwork) {
      netwib_er(netwib_io_ctl_set_data_line_msdos(poutio2, NETWIB_TRUE));
    } else {
#if defined NETWIBDEF_SYSNAME_Unix
      netwib_er(netwib_io_ctl_set_data_line_msdos(poutio2, NETWIB_FALSE));
#elif defined NETWIBDEF_SYSNAME_Windows
      netwib_er(netwib_io_ctl_set_data_line_msdos(poutio2, NETWIB_TRUE));
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif
    }
    netwib_er(netwib_io_plug_write(poutio2, poutio));
  } else {
    pinio2 = pinio;
    poutio2 = poutio;
  }
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));

  /* read data */
  /* Note: in this loop, we could also read commands on pctlio, and
     deal with ABORT. This is not yet implemented. */
  while(NETWIB_TRUE) {
    netwib__buf_reinit(pbuf);
    ret = netwib_io_read(pinio2, pbuf);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    ret = netwib_io_write(poutio2, pbuf);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }

  /* close */
  if (ascii) {
    netwib_er(netwib_io_unplug_next_read(pinio2, NULL));
    netwib_er(netwib_io_close(&pinio2));
    netwib_er(netwib_io_unplug_next_write(poutio2, NULL));
    netwib_er(netwib_io_close(&poutio2));
  }
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_retr(netwib_bufpool *pbufpool,
                              netwib_constbuf *prootdir,
                              netwib_buf *pcwd,
                              netwib_io *pctlio,
                              netwib_io **ppdataio,
                              netwib_constbuf *pfile,
                              netwib_bool ascii)
{
  netwib_buf *pvirtfile, *prealfile;
  netwib_io *pfileio;
  netwib_bool exists;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pvirtfile));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &prealfile));

  ret = netwox_ftpser_path(pbufpool, prootdir, pcwd, pfile,
                           pvirtfile, prealfile);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                        NETWOX_FTPREPLY_450_FILEACTIONNOTTAKEN,
                                               "Bad path."));
  } else {
    netwib_er(netwib_filename_exists(prealfile, &exists));
    if (!exists) {
      netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                        NETWOX_FTPREPLY_450_FILEACTIONNOTTAKEN,
                                        "This path does not exits."));
    } else {
      ret = netwib_io_init_file_read(prealfile, &pfileio);
      if (ret != NETWIB_ERR_OK) {
        netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                     NETWOX_FTPREPLY_450_FILEACTIONNOTTAKEN,
                                     "Could not open this file for reading."));
      } else {
        netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                              NETWOX_FTPREPLY_150_WILLOPENDATA,
                                                   "Start of transfer."));
        ret = netwox_priv_ftpser_data(pbufpool, pfileio, *ppdataio, ascii,
                                      NETWIB_TRUE);
        netwib_er(netwib_io_close(ppdataio));
        if (ret != NETWIB_ERR_OK) {
          netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                          NETWOX_FTPREPLY_451_ACTIONABORTED,
                                                     "An error occurred."));
        } else {
          netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                                   NETWOX_FTPREPLY_226_DATAEND,
                                                     "Transfer complete."));
        }
        netwib_er(netwib_io_close(&pfileio));
      }
    }
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &prealfile));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pvirtfile));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_stor(netwib_bufpool *pbufpool,
                              netwib_constbuf *prootdir,
                              netwib_buf *pcwd,
                              netwib_io *pctlio,
                              netwib_io **ppdataio,
                              netwib_constbuf *pfile,
                              netwib_bool ascii)
{
  netwib_buf *pvirtfile, *prealfile;
  netwib_io *pfileio;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pvirtfile));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &prealfile));

  ret = netwox_ftpser_path(pbufpool, prootdir, pcwd, pfile,
                           pvirtfile, prealfile);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                        NETWOX_FTPREPLY_450_FILEACTIONNOTTAKEN,
                                               "Bad path."));
  } else {
    ret = netwib_io_init_file_write(prealfile, &pfileio);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                     NETWOX_FTPREPLY_450_FILEACTIONNOTTAKEN,
                                     "Could not open this file for writing."));
    } else {
      netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                         NETWOX_FTPREPLY_150_WILLOPENDATA,
                                                 "Start of transfer."));
      ret = netwox_priv_ftpser_data(pbufpool, *ppdataio, pfileio, ascii,
                                    NETWIB_FALSE);
      netwib_er(netwib_io_close(ppdataio));
      if (ret != NETWIB_ERR_OK) {
        netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                           NETWOX_FTPREPLY_451_ACTIONABORTED,
                                                   "An error occurred."));
      } else {
        netwib_er(netwox_txtproto_write_reply_text(pctlio,
                                                   NETWOX_FTPREPLY_226_DATAEND,
                                                   "Transfer complete."));
      }
      netwib_er(netwib_io_close(&pfileio));
    }
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &prealfile));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pvirtfile));

  return(NETWIB_ERR_OK);
}

