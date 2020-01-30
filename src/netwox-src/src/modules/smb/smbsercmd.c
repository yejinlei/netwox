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
static netwib_err netwox_smbsercmd__rename_E(netwox_smbser *psmbser,
                                             netwib_constbuf *prootdir,
                                             netwox_smbcmdcmn_fileattr32 searchattributes,
                                             netwib_constbuf *poldname,
                                             netwib_constbuf *pnewname)
{
  netwox_smbserglob_item item;
  netwib_pathstat pathstat;
  netwib_bool foundone;
  netwib_buf *prealoldname, *prealnewname;
  netwib_err ret;

  /* init */
  ret = NETWIB_ERR_OK;
  netwib_er(netwox_smbserglob_item_init(psmbser->pcommon->pbufpool, &item));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &prealoldname));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &prealnewname));

  /* obtain names */
  netwib_eg(netwox_smbserjailglob_E(psmbser, prootdir, searchattributes,
                                    poldname, &item));
  netwib_eg(netwox_smbserjail_E(psmbser, prootdir,
                                NETWIB_PATHSTAT_TYPE_UNKNOWN, pnewname,
                                prealnewname));

  /* rename */
  foundone = NETWIB_FALSE;
  while(NETWIB_TRUE) {
    netwib__buf_reinit(prealoldname);
    ret = netwox_smbserglob_item_next(&psmbser->smbser83, &item, prealoldname);
    if (ret == NETWIB_ERR_DATAEND) {
      ret = NETWIB_ERR_OK;
      break;
    }
    if (ret == NETWIB_ERR_OK) {
      if (psmbser->pcommon->verbose) {
        netwib_eg(netwib_fmt_display("Real old pathname: %{buf}\n",
                                     prealoldname));
      }
      ret = netwib_pathstat_init(prealoldname, &pathstat);
      if (ret == NETWIB_ERR_OK) {
        if (foundone) {
          /* ignore several renames to same target (I don't know if this
             is ever used) */
        }
        foundone = NETWIB_TRUE;
        if (pathstat.type == NETWIB_PATHSTAT_TYPE_DIR) {
          ret = netwib_dirname_rename(prealoldname, prealnewname);
        } else {
          ret = netwib_filename_rename(prealoldname, prealnewname);
        }
      }
    }
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  if (!foundone || ret != NETWIB_ERR_OK) {
    psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
    netwib_eg(ret);
  }

  /* end */
 netwib_gotolabel:
  netwib_er(netwox_smbserglob_item_close(&item));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &prealnewname));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &prealoldname));
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbsercmd__open_E(netwox_smbser *psmbser,
                                           netwox_smbserfid_item *pitem,
                                           netwib_constbuf *prealpathname,
                                           netwib_bool deleteonclose,
                                           netwox_smbserfid_fidtype fidtype,
                                          netwox_smbserfid_regiotype regiotype,
                                           netwib_bool ifexists_replace,
                                           netwib_bool ifexists_open,
                                           netwib_bool ifexists_error,
                                           netwib_bool ifnoexists_create,
                                           netwib_bool ifnoexists_error,
                                           netwox_smbcmdcmn_createaction *pcreateaction)
{
  netwib_pathstat pathstat;
  netwib_err ret;

  /* init */
  ret = NETWIB_ERR_OK;
  *pcreateaction = NETWOX_SMBCMDCMN_CREATEACTION_ACTION & NETWOX_SMBCMDCMN_CREATEACTION_ACTION_NONE;

  /* prepare resource */
  netwib_eg(netwib_buf_append_buf(prealpathname, &pitem->realname));
  pitem->deleteonclose = deleteonclose;

  /* manage creationaction */
  ret = netwib_pathstat_init(prealpathname, &pathstat);
  if (ret == NETWIB_ERR_OK) {
    /* case error if exists */
    if (ifexists_error) {
      psmbser->smberrtosend = NETWOX_SMBERR_FILEEXISTS;
      netwib_goto(NETWIB_ERR_OK);
    }
    /* check or select its type */
    if (fidtype == NETWOX_SMBSERFID_FIDTYPE_UNKNOWN) {
      if (pathstat.type == NETWIB_PATHSTAT_TYPE_DIR) {
        fidtype = NETWOX_SMBSERFID_FIDTYPE_DIR;
      } else {
        fidtype = NETWOX_SMBSERFID_FIDTYPE_REG;
      }
    } else if (fidtype == NETWOX_SMBSERFID_FIDTYPE_REG) {
      if (pathstat.type == NETWIB_PATHSTAT_TYPE_DIR) {
        psmbser->smberrtosend = NETWOX_SMBERR_NOTAFILE;
        netwib_goto(NETWIB_ERR_OK);
      }
    } else if (fidtype == NETWOX_SMBSERFID_FIDTYPE_DIR) {
      if (pathstat.type != NETWIB_PATHSTAT_TYPE_DIR) {
        psmbser->smberrtosend = NETWOX_SMBERR_NOTADIR;
        netwib_goto(NETWIB_ERR_OK);
      }
    }
    /* case replace if exists */
    if (ifexists_replace) {
      if (fidtype == NETWOX_SMBSERFID_FIDTYPE_DIR) {
        netwib_er(netwox_smbser_notimplemented(psmbser));
        psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
        netwib_goto(NETWIB_ERR_OK);
      } else {
        ret = netwib_filename_create(prealpathname);
        if (ret != NETWIB_ERR_OK) {
          psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
          netwib_goto(NETWIB_ERR_OK);
        }
      }
      *pcreateaction = NETWOX_SMBCMDCMN_CREATEACTION_ACTION & NETWOX_SMBCMDCMN_CREATEACTION_ACTION_EXISTTRUNCATED;
    }
    /* case open if exist : nothing to do, just indicate what open will do */
    if (ifexists_open) {
      if (regiotype != NETWOX_SMBSERFID_REGIOTYPE_NOTOPEN) {
        *pcreateaction = NETWOX_SMBCMDCMN_CREATEACTION_ACTION & NETWOX_SMBCMDCMN_CREATEACTION_ACTION_EXISTOPENED;
      }
    }
  } else {
    ret = NETWIB_ERR_OK;
    /* if unknown, guess user required a file */
    if (fidtype == NETWOX_SMBSERFID_FIDTYPE_UNKNOWN) {
      fidtype = NETWOX_SMBSERFID_FIDTYPE_REG;
    }
    /* case error if no exists */
    if (ifnoexists_error) {
      psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
      netwib_goto(NETWIB_ERR_OK);
    }
    /* case create if no exists */
    if (ifnoexists_create) {
      if (fidtype == NETWOX_SMBSERFID_FIDTYPE_DIR) {
        ret = netwib_dirname_create(prealpathname);
        if (ret != NETWIB_ERR_OK) {
          psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
          netwib_goto(NETWIB_ERR_OK);
        }
      } else {
        ret = netwib_filename_create(prealpathname);
        if (ret != NETWIB_ERR_OK) {
          psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
          netwib_goto(NETWIB_ERR_OK);
        }
      }
      *pcreateaction = NETWOX_SMBCMDCMN_CREATEACTION_ACTION & NETWOX_SMBCMDCMN_CREATEACTION_ACTION_NOEXISTCREATED;
    }
    /* indicate what open will do */
    if (fidtype == NETWOX_SMBSERFID_FIDTYPE_REG) {
      if (regiotype != NETWOX_SMBSERFID_REGIOTYPE_NOTOPEN) {
        *pcreateaction = NETWOX_SMBCMDCMN_CREATEACTION_ACTION & NETWOX_SMBCMDCMN_CREATEACTION_ACTION_NOEXISTCREATED;
      }
    }
  }

  /* set value (currently, this is only used for reg) */
  netwib_eg(netwox_smbserfid_item_value_setdefault(pitem, fidtype));

  /* open file (nothing to do with directories) */
  if (fidtype == NETWOX_SMBSERFID_FIDTYPE_REG) {
    if (regiotype != NETWOX_SMBSERFID_REGIOTYPE_NOTOPEN) {
      ret = netwib_io_init_file(prealpathname, regiotype,
                                NETWIB_FALSE, &pitem->value.itemreg.pio);
      if (ret != NETWIB_ERR_OK) {
        psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
        netwib_goto(NETWIB_ERR_OK);
      }
      pitem->value.itemreg.regiotype = regiotype;
    }
  }

 netwib_gotolabel:
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbsercmd__close_E(netwox_smbser *psmbser,
                                            netwib_uint16 fileid)
{
  netwox_smbserfid_item *pitem;
  netwib_pathstat pathstat;
  netwib_err ret;

  /* init */
  ret = NETWIB_ERR_OK;

  /* obtain real pathname from fileid (of file or directory) */
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Fileid: %{uint16}\n", fileid));
  }
  ret = netwox_smbserfid_search(&psmbser->smbserfid, fileid, &pitem);
  if (ret != NETWIB_ERR_OK) {
    psmbser->smberrtosend = NETWOX_SMBERR_BADFILEID;
    netwib_goto(NETWIB_ERR_OK);
  }
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Real pathname: %{buf}\n", &pitem->realname));
  }

  /* optionally remove the file */
  if (pitem->deleteonclose) {
    pathstat.type = NETWIB_PATHSTAT_TYPE_UNKNOWN;
    if (pitem->fidtype == NETWOX_SMBSERFID_FIDTYPE_REG) {
      pathstat.type = NETWIB_PATHSTAT_TYPE_REG;
    } else if (pitem->fidtype == NETWOX_SMBSERFID_FIDTYPE_DIR) {
      pathstat.type = NETWIB_PATHSTAT_TYPE_DIR;
    } else {
      netwib_eg(netwib_pathstat_init(&pitem->realname, &pathstat));
    }
    if (pathstat.type == NETWIB_PATHSTAT_TYPE_REG) {
      ret = netwib_filename_remove(&pitem->realname);
    } else if (pathstat.type == NETWIB_PATHSTAT_TYPE_DIR) {
      ret = netwib_dirname_remove(&pitem->realname);
    } else {
      netwib_er(netwox_smbser_notimplemented(psmbser));
      psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
      netwib_goto(NETWIB_ERR_OK);
    }
    if (ret != NETWIB_ERR_OK) {
      psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
      netwib_goto(NETWIB_ERR_OK);
    }
  }

  /* close the resource */
  netwib_eg(netwox_smbserfid_del(&psmbser->smbserfid, fileid));

  /* end */
 netwib_gotolabel:
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_createdirectory(netwox_smbser *psmbser,
                                            netwox_constsmbmsg *psmbmsgq,
                                            netwox_constsmbcmd *psmbcmdq,
                                            netwox_smbmsg *psmbmsgr,
                                            netwox_smbcmd *psmbcmdr,
                                            netwib_constbuf *prootdir)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_buf *pdirname, *prealdirname;
  netwib_bool exists;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_createdirectory\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pdirname));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &prealdirname));

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_CREATEDIRECTORY_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }

  /* obtain dirname */
  netwib_eg(netwox_smbcmd_decode_buf_path(psmbmsgq, &psmbcmdq->value.createdirectory_q.dirname, pdirname));
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Directory: %{buf}\n", pdirname));
  }
  netwib_eg(netwox_smbserjail_E(psmbser, prootdir, NETWIB_PATHSTAT_TYPE_DIR, pdirname, prealdirname));

  /* check if exits or create */
  netwib_eg(netwib_dirname_exists(prealdirname, &exists));
  if (exists) {
    psmbser->smberrtosend = NETWOX_SMBERR_FILEEXISTS;
    netwib_goto(NETWIB_ERR_OK);
  }
  ret = netwib_dirname_create(prealdirname);
  if (ret != NETWIB_ERR_OK) {
    psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
    netwib_goto(NETWIB_ERR_OK);
  }

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_CREATEDIRECTORY_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));

  /* end */
 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &prealdirname));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pdirname));
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_deletedirectory(netwox_smbser *psmbser,
                                             netwox_constsmbmsg *psmbmsgq,
                                             netwox_constsmbcmd *psmbcmdq,
                                             netwox_smbmsg *psmbmsgr,
                                             netwox_smbcmd *psmbcmdr,
                                             netwib_constbuf *prootdir)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_buf *pdirname, *prealdirname;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_deletedirectory\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pdirname));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &prealdirname));

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_DELETEDIRECTORY_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }

  /* obtain dirname */
  netwib_eg(netwox_smbcmd_decode_buf_path(psmbmsgq, &psmbcmdq->value.deletedirectory_q.dirname, pdirname));
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Directory: %{buf}\n", pdirname));
  }
  netwib_eg(netwox_smbserjail_E(psmbser, prootdir, NETWIB_PATHSTAT_TYPE_DIR, pdirname, prealdirname));
  ret = netwib_dirname_remove(prealdirname);
  if (ret != NETWIB_ERR_OK) {
    psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
    netwib_goto(NETWIB_ERR_OK);
  }

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_DELETEDIRECTORY_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));

  /* end */
 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &prealdirname));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pdirname));
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_create(netwox_smbser *psmbser,
                                   netwox_constsmbmsg *psmbmsgq,
                                   netwox_constsmbcmd *psmbcmdq,
                                   netwox_smbmsg *psmbmsgr,
                                   netwox_smbcmd *psmbcmdr,
                                   netwib_constbuf *prootdir)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_buf *pfilename, *prealfilename;
  netwox_smbserfid_item *pitem=NULL;
  netwox_smbcmdcmn_createaction createaction;
  netwib_bool normalend;
  netwib_uint16 fileid=0;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_create\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;
  normalend = NETWIB_FALSE;
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pfilename));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &prealfilename));
  netwib_er(netwox_smbserfid_new(&psmbser->smbserfid, &fileid, &pitem));

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_CREATE_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  /* ignore psmbcmdq->value.create_q.fileattr and creationtime */
  /* obtain filename */
  netwib_eg(netwox_smbcmd_decode_buf_path(psmbmsgq, &psmbcmdq->value.create_q.filename, pfilename));
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Filename: %{buf}\n", pfilename));
  }
  netwib_eg(netwox_smbserjail_E(psmbser, prootdir, NETWIB_PATHSTAT_TYPE_REG, pfilename, prealfilename));

  /* create */
  netwib_eg(netwox_smbsercmd__open_E(psmbser/*psmbser*/,
                                     pitem/*pitem*/,
                                     prealfilename/*prealpathname*/,
                                     NETWIB_FALSE/*deleteonclose*/,
                                     NETWOX_SMBSERFID_FIDTYPE_REG/*fidtype*/,
                                  NETWOX_SMBSERFID_REGIOTYPE_RDWR/*regiotype*/,
                                     NETWIB_TRUE/*ifexists_replace*/,
                                     NETWIB_TRUE/*ifexists_open*/,
                                     NETWIB_FALSE/*ifexists_error*/,
                                     NETWIB_TRUE/*ifnoexists_create*/,
                                     NETWIB_FALSE/*ifnoexists_error*/,
                                     &createaction/*pcreateaction*/));
  if (psmbser->smberrtosend != NETWOX_SMBERR_OK) {
    netwib_goto(NETWIB_ERR_OK);
  }

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr, NETWOX_SMBCMD_TYPE_CREATE_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));
  psmbcmdr->value.create_r.fileid = fileid;

  /* end */
  normalend = NETWIB_TRUE;
 netwib_gotolabel:
  if (!normalend) {
    netwib_er(netwox_smbserfid_del(&psmbser->smbserfid, fileid));
  }
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &prealfilename));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pfilename));
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_open(netwox_smbser *psmbser,
                                 netwox_constsmbmsg *psmbmsgq,
                                 netwox_constsmbcmd *psmbcmdq,
                                 netwox_smbmsg *psmbmsgr,
                                 netwox_smbcmd *psmbcmdr,
                                 netwib_constbuf *prootdir)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_buf *pfilename, *prealfilename;
  netwox_smbserfid_regiotype regiotype;
  netwox_smbserfid_item *pitem=NULL;
  netwox_smbcmdcmn_createaction createaction;
  netwib_bool normalend;
  netwib_pathstat pathstat;
  netwib_uint16 fileid=0;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_open\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;
  normalend = NETWIB_FALSE;
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pfilename));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &prealfilename));
  netwib_er(netwox_smbserfid_new(&psmbser->smbserfid, &fileid, &pitem));

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_OPEN_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  /* check ACCESS desiredaccess, but ignore WRITETHROUGH, CACHE, LOCALITY
     and SHARING */
  regiotype = NETWOX_SMBSERFID_REGIOTYPE_NOTOPEN;
  switch(psmbcmdq->value.open_q.desiredaccess & NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS) {
  case NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS_R:
  case NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS_X:
    regiotype = NETWOX_SMBSERFID_REGIOTYPE_READ;
    break;
  case NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS_W:
    regiotype = NETWOX_SMBSERFID_REGIOTYPE_WRITE;
    if (!psmbser->pcommon->allowput) {
      psmbser->smberrtosend = NETWOX_SMBERR_DEVICEWRITEPROTECTED;
      netwib_goto(NETWIB_ERR_OK);
    }
    break;
  case NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS_RW:
    regiotype = NETWOX_SMBSERFID_REGIOTYPE_RDWR;
    if (!psmbser->pcommon->allowput) {
      psmbser->smberrtosend = NETWOX_SMBERR_DEVICEWRITEPROTECTED;
      netwib_goto(NETWIB_ERR_OK);
    }
    break;
  default :
    netwib_er(netwox_smbser_notimplemented(psmbser));
    psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
    netwib_goto(NETWIB_ERR_OK);
    break;
  }
  /* ignore psmbcmdq->value.open_q.searchattributes */
  /* obtain filename */
  netwib_eg(netwox_smbcmd_decode_buf_path(psmbmsgq, &psmbcmdq->value.open_q.filename, pfilename));
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Filename: %{buf}\n", pfilename));
  }
  netwib_eg(netwox_smbserjail_E(psmbser, prootdir, NETWIB_PATHSTAT_TYPE_REG, pfilename, prealfilename));

  /* open */
  netwib_eg(netwox_smbsercmd__open_E(psmbser/*psmbser*/,
                                     pitem/*pitem*/,
                                     prealfilename/*prealpathname*/,
                                     NETWIB_FALSE/*deleteonclose*/,
                                     NETWOX_SMBSERFID_FIDTYPE_REG/*fidtype*/,
                                     regiotype/*regiotype*/,
                                     NETWIB_FALSE/*ifexists_replace*/,
                                     NETWIB_TRUE/*ifexists_open*/,
                                     NETWIB_FALSE/*ifexists_error*/,
                                     NETWIB_FALSE/*ifnoexists_create*/,
                                     NETWIB_FALSE/*ifnoexists_error*/,
                                     &createaction/*pcreateaction*/));
  if (psmbser->smberrtosend != NETWOX_SMBERR_OK) {
    netwib_goto(NETWIB_ERR_OK);
  }

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr, NETWOX_SMBCMD_TYPE_OPEN_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));
  psmbcmdr->value.open_r.fileid = fileid;
  if (!psmbser->pcommon->allowput) {
    psmbcmdr->value.open_r.fileattributes = NETWOX_SMBCMDCMN_FILEATTR16_READONLY;
  }
  psmbcmdr->value.open_r.grantedaccess = NETWOX_SMBCMDCMN_ACCESSMODE_WRITETHROUGH & NETWOX_SMBCMDCMN_ACCESSMODE_WRITETHROUGH_NO;
  psmbcmdr->value.open_r.grantedaccess |= NETWOX_SMBCMDCMN_ACCESSMODE_CACHE & NETWOX_SMBCMDCMN_ACCESSMODE_CACHE_NONE;
  psmbcmdr->value.open_r.grantedaccess |= NETWOX_SMBCMDCMN_ACCESSMODE_LOCALITY & NETWOX_SMBCMDCMN_ACCESSMODE_LOCALITY_UNKNOWN;
  psmbcmdr->value.open_r.grantedaccess |= NETWOX_SMBCMDCMN_ACCESSMODE_SHARING & NETWOX_SMBCMDCMN_ACCESSMODE_SHARING_COMPATIBILITY;
  if (pitem->value.itemreg.regiotype == NETWOX_SMBSERFID_REGIOTYPE_WRITE) {
    psmbcmdr->value.open_r.grantedaccess |= NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS & NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS_W;
  } else if (pitem->value.itemreg.regiotype == NETWOX_SMBSERFID_REGIOTYPE_RDWR) {
    psmbcmdr->value.open_r.grantedaccess |= NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS & NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS_RW;
  } else if (pitem->value.itemreg.regiotype == NETWOX_SMBSERFID_REGIOTYPE_READ) {
    psmbcmdr->value.open_r.grantedaccess |= NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS & NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS_R;
  }
  netwib_eg(netwib_pathstat_init(prealfilename, &pathstat));
  netwib_eg(netwox_time1970l_init_time(&pathstat.mtime, &psmbcmdr->value.open_r.lastwritetime));
  psmbcmdr->value.open_r.filesize = pathstat.size;

  /* end */
  normalend = NETWIB_TRUE;
 netwib_gotolabel:
  if (!normalend) {
    netwib_er(netwox_smbserfid_del(&psmbser->smbserfid, fileid));
  }
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &prealfilename));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pfilename));
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_close(netwox_smbser *psmbser,
                                   netwox_constsmbmsg *psmbmsgq,
                                   netwox_constsmbcmd *psmbcmdq,
                                   netwox_smbmsg *psmbmsgr,
                                   netwox_smbcmd *psmbcmdr)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_close\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_CLOSE_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  /* ignore psmbcmdq->value.close_q.lastwritetime */

  /* close */
  netwib_eg(netwox_smbsercmd__close_E(psmbser, psmbcmdq->value.close_q.fileid));
  if (psmbser->smberrtosend != NETWOX_SMBERR_OK) {
    netwib_goto(NETWIB_ERR_OK);
  }

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_CLOSE_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));

  /* end */
 netwib_gotolabel:
  psmbmsgq = psmbmsgq; /* for compiler warning */
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_flush(netwox_smbser *psmbser,
                                   netwox_constsmbmsg *psmbmsgq,
                                   netwox_constsmbcmd *psmbcmdq,
                                   netwox_smbmsg *psmbmsgr,
                                   netwox_smbcmd *psmbcmdr)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwox_smbserfid_item *pitem;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_flush\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_FLUSH_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  /* obtain real filename from fileid */
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Fileid: %{uint16}\n", psmbcmdq->value.flush_q.fileid));
  }
  if (psmbcmdq->value.flush_q.fileid == 0xFFFFu) {
    netwib_eg(netwib_fmt_display("All files\n"));
  } else {
    ret = netwox_smbserfid_search(&psmbser->smbserfid, psmbcmdq->value.flush_q.fileid, &pitem);
    if (ret != NETWIB_ERR_OK || pitem->fidtype != NETWOX_SMBSERFID_FIDTYPE_REG) {
      psmbser->smberrtosend = NETWOX_SMBERR_BADFILEID;
      netwib_goto(NETWIB_ERR_OK);
    }
    if (psmbser->pcommon->verbose) {
      netwib_eg(netwib_fmt_display("Real filename: %{buf}\n", &pitem->realname));
    }
  }

  /* flush the file is not done */

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_FLUSH_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));

  /* end */
 netwib_gotolabel:
  psmbmsgq = psmbmsgq; /* for compiler warning */
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_delete(netwox_smbser *psmbser,
                                    netwox_constsmbmsg *psmbmsgq,
                                    netwox_constsmbcmd *psmbcmdq,
                                    netwox_smbmsg *psmbmsgr,
                                    netwox_smbcmd *psmbcmdr,
                                    netwib_constbuf *prootdir)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwox_smbserglob_item item;
  netwib_pathstat pathstat;
  netwib_buf *pfilename, *prealpathname;
  netwib_bool foundone;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_delete\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pfilename));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &prealpathname));
  netwib_er(netwox_smbserglob_item_init(psmbser->pcommon->pbufpool, &item));

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_DELETE_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }

  /* obtain filename */
  netwib_eg(netwox_smbcmd_decode_buf_path(psmbmsgq, &psmbcmdq->value.delete_q.filename, pfilename));
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Filename: %{buf}\n", pfilename));
  }
  netwib_eg(netwox_smbserjailglob_E(psmbser, prootdir, psmbcmdq->value.delete_q.searchattributes, pfilename, &item));

  /* delete */
  foundone = NETWIB_FALSE;
  while(NETWIB_TRUE) {
    netwib__buf_reinit(prealpathname);
    ret = netwox_smbserglob_item_next(&psmbser->smbser83, &item, prealpathname);
    if (ret == NETWIB_ERR_DATAEND) {
      ret = NETWIB_ERR_OK;
      break;
    }
    if (ret == NETWIB_ERR_OK) {
      ret = netwib_pathstat_init(prealpathname, &pathstat);
      if (ret == NETWIB_ERR_OK) {
        foundone = NETWIB_TRUE;
        if (pathstat.type == NETWIB_PATHSTAT_TYPE_DIR) {
          ret = netwib_dirname_remove(prealpathname);
        } else {
          ret = netwib_filename_remove(prealpathname);
        }
      }
    }
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  if (!foundone || ret != NETWIB_ERR_OK) {
    psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
    netwib_eg(ret);
  }

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_DELETE_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));

  /* end */
 netwib_gotolabel:
  netwib_er(netwox_smbserglob_item_close(&item));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &prealpathname));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pfilename));
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_rename(netwox_smbser *psmbser,
                                    netwox_constsmbmsg *psmbmsgq,
                                    netwox_constsmbcmd *psmbcmdq,
                                    netwox_smbmsg *psmbmsgr,
                                    netwox_smbcmd *psmbcmdr,
                                    netwib_constbuf *prootdir)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_buf *poldname, *pnewname;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_rename\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &poldname));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pnewname));

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_RENAME_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }

  /* obtain names */
  netwib_eg(netwox_smbcmd_decode_buf_path(psmbmsgq, &psmbcmdq->value.rename_q.oldname, poldname));
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Old name: %{buf}\n", poldname));
  }
  netwib_eg(netwox_smbcmd_decode_buf_path(psmbmsgq, &psmbcmdq->value.rename_q.newname, pnewname));
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("New name: %{buf}\n", pnewname));
  }

  /* rename */
  netwib_eg(netwox_smbsercmd__rename_E(psmbser, prootdir, psmbcmdq->value.rename_q.searchattributes, poldname, pnewname));
  if (psmbser->smberrtosend != NETWOX_SMBERR_OK) {
    netwib_goto(NETWIB_ERR_OK);
  }

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_RENAME_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));

  /* end */
 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pnewname));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &poldname));
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_queryinformation(netwox_smbser *psmbser,
                                              netwox_constsmbmsg *psmbmsgq,
                                              netwox_constsmbcmd *psmbcmdq,
                                              netwox_smbmsg *psmbmsgr,
                                              netwox_smbcmd *psmbcmdr,
                                              netwib_constbuf *prootdir)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_buf *pfilename, *prealfilename;
  netwib_pathstat pathstat;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_queryinformation\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pfilename));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &prealfilename));

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_QUERYINFORMATION_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }

  /* obtain filename */
  netwib_eg(netwox_smbcmd_decode_buf_path(psmbmsgq, &psmbcmdq->value.queryinformation_q.filename, pfilename));
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Filename: %{buf}\n", pfilename));
  }
  netwib_eg(netwox_smbserjail_E(psmbser, prootdir, NETWIB_PATHSTAT_TYPE_UNKNOWN, pfilename, prealfilename));
  ret = netwib_pathstat_init(prealfilename, &pathstat);
  if (ret != NETWIB_ERR_OK) {
    psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
    netwib_goto(NETWIB_ERR_OK);
  }

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_QUERYINFORMATION_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));
  if (!psmbser->pcommon->allowput) {
    psmbcmdr->value.queryinformation_r.fileattributes |= NETWOX_SMBCMDCMN_FILEATTR16_READONLY;
  }
  netwib_eg(netwox_time1970l_init_time(&pathstat.mtime, &psmbcmdr->value.queryinformation_r.lastwritetime));
  if (pathstat.type == NETWIB_PATHSTAT_TYPE_DIR) {
    psmbcmdr->value.queryinformation_r.fileattributes |= NETWOX_SMBCMDCMN_FILEATTR16_DIR;
    psmbcmdr->value.queryinformation_r.filesize = 0;
  } else {
    psmbcmdr->value.queryinformation_r.filesize = pathstat.size;
  }

  /* end */
 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &prealfilename));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pfilename));
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_setinformation(netwox_smbser *psmbser,
                                           netwox_constsmbmsg *psmbmsgq,
                                           netwox_constsmbcmd *psmbcmdq,
                                           netwox_smbmsg *psmbmsgr,
                                           netwox_smbcmd *psmbcmdr,
                                           netwib_constbuf *prootdir)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_buf *pfilename, *prealfilename;
  netwib_pathstat pathstat;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_setinformation\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pfilename));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &prealfilename));

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_SETINFORMATION_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }

  /* obtain filename */
  netwib_eg(netwox_smbcmd_decode_buf_path(psmbmsgq, &psmbcmdq->value.setinformation_q.filename, pfilename));
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Filename: %{buf}\n", pfilename));
  }
  netwib_eg(netwox_smbserjail_E(psmbser, prootdir, NETWIB_PATHSTAT_TYPE_UNKNOWN, pfilename, prealfilename));
  ret = netwib_pathstat_init(prealfilename, &pathstat);
  if (ret != NETWIB_ERR_OK) {
    psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
    netwib_goto(NETWIB_ERR_OK);
  }

  /* send reply, without doing requested changes */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_SETINFORMATION_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));

  /* end */
 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &prealfilename));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pfilename));
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_read(netwox_smbser *psmbser,
                                 netwox_constsmbmsg *psmbmsgq,
                                 netwox_constsmbcmd *psmbcmdq,
                                 netwox_smbmsg *psmbmsgr,
                                 netwox_smbcmd *psmbcmdr)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwox_smbserfid_item *pitem;
  netwib_bool needtoread;
  netwib_uint32 sizetocopy;
  netwib_uint64 sizetocopy64, offset64;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_read\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_READ_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  /* obtain real filename from fileid */
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Fileid: %{uint16}\n", psmbcmdq->value.read_q.fileid));
  }
  ret = netwox_smbserfid_search(&psmbser->smbserfid, psmbcmdq->value.read_q.fileid, &pitem);
  if (ret != NETWIB_ERR_OK || pitem->fidtype != NETWOX_SMBSERFID_FIDTYPE_REG) {
    psmbser->smberrtosend = NETWOX_SMBERR_BADFILEID;
    netwib_goto(NETWIB_ERR_OK);
  }
  if (pitem->value.itemreg.regiotype != NETWOX_SMBSERFID_REGIOTYPE_READ &&
      pitem->value.itemreg.regiotype != NETWOX_SMBSERFID_REGIOTYPE_RDWR) {
    psmbser->smberrtosend = NETWOX_SMBERR_BADFILEID;
    netwib_goto(NETWIB_ERR_OK);
  }
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Real filename: %{buf}\n", &pitem->realname));
  }
  /* will use psmbcmdq->value.read_q.maxsize and offset below */
  /* ignore psmbcmdq->value.read_q.remaining */

  /* ensure readbuf contains sufficient data */
  needtoread = NETWIB_FALSE;
  netwib__uint64_init_uint32(psmbcmdq->value.read_q.offset, offset64);
  if (netwib__uint64_cmp_eq(offset64, pitem->value.itemreg.offset)) {
    if (netwib__buf_ref_data_size(&pitem->value.itemreg.readbuf) < psmbcmdq->value.read_q.size) {
      needtoread = NETWIB_TRUE;
    }
  } else {
    /* read new data */
    needtoread = NETWIB_TRUE;
    netwib__buf_reinit(&pitem->value.itemreg.readbuf);
    netwib__uint64_init_uint32(psmbcmdq->value.read_q.offset, pitem->value.itemreg.offset);
    ret = netwib_file_ctl_set_seek64_begin(pitem->value.itemreg.pio, &pitem->value.itemreg.offset);
    if (ret != NETWIB_ERR_OK) {
      /* perhaps a bad offset, so simulate end */
      pitem->value.itemreg.readeofreached = NETWIB_TRUE;
      ret = NETWIB_ERR_OK;
    } else {
      pitem->value.itemreg.readeofreached = NETWIB_FALSE;
    }
  }
  if (needtoread && !pitem->value.itemreg.readeofreached) {
    ret = netwib_io_read(pitem->value.itemreg.pio, &pitem->value.itemreg.readbuf);
    if (ret == NETWIB_ERR_OK) {
      if (netwib__buf_ref_data_size(&pitem->value.itemreg.readbuf) == 0) {
        pitem->value.itemreg.readeofreached = NETWIB_TRUE;
      }
    } else if (ret == NETWIB_ERR_DATAEND) {
      pitem->value.itemreg.readeofreached = NETWIB_TRUE;
      ret = NETWIB_ERR_OK;
    } else {
      netwib_goto(ret);
    }
  }

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_READ_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));
  if (pitem->value.itemreg.readeofreached &&
      netwib__buf_ref_data_size(&pitem->value.itemreg.readbuf) == 0) {
  } else {
    sizetocopy = netwib__buf_ref_data_size(&pitem->value.itemreg.readbuf);
    if (sizetocopy > psmbcmdq->value.read_q.size) {
      sizetocopy = psmbcmdq->value.read_q.size;
    }
    netwib_eg(netwib_buf_append_data(netwib__buf_ref_data_ptr(&pitem->value.itemreg.readbuf), sizetocopy, &psmbcmdr->value.read_r.data));
    pitem->value.itemreg.readbuf.beginoffset += sizetocopy;
    netwib__uint64_init_uint32(sizetocopy, sizetocopy64);
    netwib__uint64_add(sizetocopy64, pitem->value.itemreg.offset, pitem->value.itemreg.offset);
  }

  /* end */
 netwib_gotolabel:
  psmbmsgq = psmbmsgq; /* for compiler warning */
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_write(netwox_smbser *psmbser,
                                  netwox_constsmbmsg *psmbmsgq,
                                  netwox_constsmbcmd *psmbcmdq,
                                  netwox_smbmsg *psmbmsgr,
                                  netwox_smbcmd *psmbcmdr)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwox_smbserfid_item *pitem;
  netwib_uint32 writtensize;
  netwib_uint64 writtensize64, offset64;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_write\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_WRITE_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  /* obtain real filename from fileid */
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Fileid: %{uint16}\n", psmbcmdq->value.write_q.fileid));
  }
  ret = netwox_smbserfid_search(&psmbser->smbserfid, psmbcmdq->value.write_q.fileid, &pitem);
  if (ret != NETWIB_ERR_OK || pitem->fidtype != NETWOX_SMBSERFID_FIDTYPE_REG) {
    psmbser->smberrtosend = NETWOX_SMBERR_BADFILEID;
    netwib_goto(NETWIB_ERR_OK);
  }
  if (pitem->value.itemreg.regiotype != NETWOX_SMBSERFID_REGIOTYPE_WRITE &&
      pitem->value.itemreg.regiotype != NETWOX_SMBSERFID_REGIOTYPE_RDWR) {
    psmbser->smberrtosend = NETWOX_SMBERR_BADFILEID;
    netwib_goto(NETWIB_ERR_OK);
  }
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Real filename: %{buf}\n", &pitem->realname));
  }
  /* will use psmbcmdq->value.write_q.offset and data below */
  /* ignore psmbcmdq->value.write_q.remaining */

  /* check current offset */
  netwib__uint64_init_uint32(psmbcmdq->value.write_q.offset, offset64);
  if (!netwib__uint64_cmp_eq(offset64, pitem->value.itemreg.offset)) {
    ret = netwib_file_ctl_set_seek64_begin(pitem->value.itemreg.pio, &offset64);
    if (ret != NETWIB_ERR_OK) {
      psmbser->smberrtosend = NETWOX_SMBERR_BADPARAMETER;
      netwib_goto(NETWIB_ERR_OK);
    }
    pitem->value.itemreg.offset = offset64;
  }
  /* reset for next read */
  netwib__buf_reinit(&pitem->value.itemreg.readbuf);
  pitem->value.itemreg.readeofreached = NETWIB_FALSE;

  /* write data */
  if (netwib__buf_ref_data_size(&psmbcmdq->value.write_q.data) == 0) {
    /* truncate */
    ret = netwib_file_ctl_set_truncate64(pitem->value.itemreg.pio, &offset64);
    if (ret != NETWIB_ERR_OK) {
      psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
      netwib_goto(NETWIB_ERR_OK);
    }
  } else {
    /* write */
    ret = netwib_io_write(pitem->value.itemreg.pio, &psmbcmdq->value.write_q.data);
    if (ret != NETWIB_ERR_OK) {
      psmbser->smberrtosend = NETWOX_SMBERR_QUOTAEXCEEDED;
      netwib_goto(NETWIB_ERR_OK);
    }
  }
  writtensize = netwib__buf_ref_data_size(&psmbcmdq->value.write_q.data);

  /* update offset */
  netwib__uint64_init_uint32(writtensize, writtensize64);
  netwib__uint64_add(writtensize64, pitem->value.itemreg.offset, pitem->value.itemreg.offset);

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_WRITE_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));
  psmbcmdr->value.write_r.writtensize = (netwib_uint16)writtensize;

  /* end */
 netwib_gotolabel:
  psmbmsgq = psmbmsgq; /* for compiler warning */
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_createtemporary(netwox_smbser *psmbser,
                                             netwox_constsmbmsg *psmbmsgq,
                                             netwox_constsmbcmd *psmbcmdq,
                                             netwox_smbmsg *psmbmsgr,
                                             netwox_smbcmd *psmbcmdr,
                                             netwib_constbuf *prootdir)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_buf *pdirname, *prealdirname;
  netwox_smbserfid_item *pitem;
  netwib_bool isdir, normalend;
  netwib_data data;
  netwib_uint32 datasize;
  netwib_uint16 fileid;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_createtemporary\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;
  normalend = NETWIB_FALSE;
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pdirname));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &prealdirname));
  netwib_er(netwox_smbserfid_new(&psmbser->smbserfid, &fileid, &pitem));
  netwib_er(netwox_smbserfid_item_value_setdefault(pitem, NETWOX_SMBSERFID_FIDTYPE_REG));

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_CREATETEMPORARY_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  /* ignore psmbcmdq->value.createtemporary_q.creationtime */

  /* obtain dirname */
  netwib_eg(netwox_smbcmd_decode_buf_path(psmbmsgq, &psmbcmdq->value.createtemporary_q.dirname, pdirname));
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Directory: %{buf}\n", pdirname));
  }
  netwib_eg(netwox_smbserjail_E(psmbser, prootdir, NETWIB_PATHSTAT_TYPE_DIR, pdirname, prealdirname));
  ret = netwib_dirname_exists(prealdirname, &isdir);
  if (ret != NETWIB_ERR_OK) {
    psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
    netwib_goto(NETWIB_ERR_OK);
  }
  if (!isdir) {
    psmbser->smberrtosend = NETWOX_SMBERR_NOTADIR;
    netwib_goto(NETWIB_ERR_OK);
  }

  /* create temporary file */
  netwib_eg(netwib_buf_append_buf(prealdirname, &pitem->realname));
  data = netwib__buf_ref_data_ptr(&pitem->realname);
  datasize = netwib__buf_ref_data_size(&pitem->realname);
  if (data[datasize-1] != '/') {
    netwib_eg(netwib_buf_append_byte('/', &pitem->realname));
    datasize++;
  }
  ret = netwib_io_init_filetemp(NETWIB_FALSE, &pitem->realname, &pitem->value.itemreg.pio);
  if (ret != NETWIB_ERR_OK) {
    psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
    netwib_goto(NETWIB_ERR_OK);
  }
  pitem->value.itemreg.regiotype = NETWOX_SMBSERFID_REGIOTYPE_RDWR;

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_CREATETEMPORARY_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));
  psmbcmdr->value.createtemporary_r.fileid = fileid;
  netwib_eg(netwox_smbserfid_item_svcname(prootdir, pitem, &psmbcmdr->value.createtemporary_r.filename));
  psmbcmdr->value.createtemporary_r.filename.beginoffset += datasize;

  /* end */
  normalend = NETWIB_TRUE;
 netwib_gotolabel:
  if (!normalend) {
    netwib_er(netwox_smbserfid_del(&psmbser->smbserfid, fileid));
  }
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &prealdirname));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pdirname));
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_createnew(netwox_smbser *psmbser,
                                      netwox_constsmbmsg *psmbmsgq,
                                      netwox_constsmbcmd *psmbcmdq,
                                      netwox_smbmsg *psmbmsgr,
                                      netwox_smbcmd *psmbcmdr,
                                      netwib_constbuf *prootdir)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_buf *pfilename, *prealfilename;
  netwox_smbserfid_item *pitem;
  netwib_bool normalend;
  netwib_uint16 fileid;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_createnew\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;
  normalend = NETWIB_FALSE;
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pfilename));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &prealfilename));
  netwib_er(netwox_smbserfid_new(&psmbser->smbserfid, &fileid, &pitem));
  netwib_er(netwox_smbserfid_item_value_setdefault(pitem, NETWOX_SMBSERFID_FIDTYPE_REG));

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_CREATENEW_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  /* ignore psmbcmdq->value.createnew_q.fileattributes */

  /* obtain filename */
  netwib_eg(netwox_smbcmd_decode_buf_path(psmbmsgq, &psmbcmdq->value.createnew_q.filename, pfilename));
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Filename: %{buf}\n", pfilename));
  }
  netwib_eg(netwox_smbserjail_E(psmbser, prootdir, NETWIB_PATHSTAT_TYPE_REG, pfilename, prealfilename));
  ret = netwib_filename_create(prealfilename);
  if (ret != NETWIB_ERR_OK) {
    psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
    netwib_goto(NETWIB_ERR_OK);
  }

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_CREATENEW_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));
  psmbcmdr->value.createnew_r.fileid = fileid;

  /* end */
  normalend = NETWIB_TRUE;
 netwib_gotolabel:
  if (!normalend) {
    netwib_er(netwox_smbserfid_del(&psmbser->smbserfid, fileid));
  }
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &prealfilename));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pfilename));
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_checkdirpath(netwox_smbser *psmbser,
                                          netwox_constsmbmsg *psmbmsgq,
                                          netwox_constsmbcmd *psmbcmdq,
                                          netwox_smbmsg *psmbmsgr,
                                          netwox_smbcmd *psmbcmdr,
                                          netwib_constbuf *prootdir)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_buf *pdirname, *prealdirname;
  netwib_bool isdir;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_checkdirpath\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pdirname));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &prealdirname));

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_CHECKDIRPATH_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }

  /* obtain dirname */
  netwib_eg(netwox_smbcmd_decode_buf_path(psmbmsgq, &psmbcmdq->value.checkdirpath_q.dirname, pdirname));
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Directory: %{buf}\n", pdirname));
  }
  netwib_eg(netwox_smbserjail_E(psmbser, prootdir, NETWIB_PATHSTAT_TYPE_DIR, pdirname, prealdirname));
  ret = netwib_dirname_exists(prealdirname, &isdir);
  if (ret != NETWIB_ERR_OK) {
    psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
    netwib_goto(NETWIB_ERR_OK);
  }
  if (!isdir) {
    psmbser->smberrtosend = NETWOX_SMBERR_NOTADIR;
    netwib_goto(NETWIB_ERR_OK);
  }

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_CHECKDIRPATH_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));

  /* end */
 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &prealdirname));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pdirname));
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_seek(netwox_smbser *psmbser,
                                   netwox_constsmbmsg *psmbmsgq,
                                   netwox_constsmbcmd *psmbcmdq,
                                   netwox_smbmsg *psmbmsgr,
                                   netwox_smbcmd *psmbcmdr)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwox_smbserfid_item *pitem;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_seek\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_SEEK_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  /* obtain real filename from fileid */
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Fileid: %{uint16}\n", psmbcmdq->value.seek_q.fileid));
  }
  ret = netwox_smbserfid_search(&psmbser->smbserfid, psmbcmdq->value.seek_q.fileid, &pitem);
  if (ret != NETWIB_ERR_OK || pitem->fidtype != NETWOX_SMBSERFID_FIDTYPE_REG) {
    psmbser->smberrtosend = NETWOX_SMBERR_BADFILEID;
    netwib_goto(NETWIB_ERR_OK);
  }
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Real filename: %{buf}\n", &pitem->realname));
  }

  /* seek the file */
  if (pitem->value.itemreg.regiotype == NETWOX_SMBSERFID_REGIOTYPE_NOTOPEN) {
    psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
    netwib_goto(NETWIB_ERR_OK);
  }
  switch(psmbcmdq->value.seek_q.seektype) {
  case NETWOX_SMBCMDCMN_SEEKTYPE_BEGIN :
    ret = netwib_file_ctl_set_seek_begin(pitem->value.itemreg.pio, psmbcmdq->value.seek_q.offset);
    break;
  case NETWOX_SMBCMDCMN_SEEKTYPE_CURRENT :
    ret = netwib_file_ctl_set_seek_current(pitem->value.itemreg.pio, psmbcmdq->value.seek_q.offset);
    break;
  case NETWOX_SMBCMDCMN_SEEKTYPE_END :
    ret = netwib_file_ctl_set_seek_end(pitem->value.itemreg.pio, psmbcmdq->value.seek_q.offset);
    break;
  default :
    psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
    netwib_goto(NETWIB_ERR_OK);
  }
  if (ret != NETWIB_ERR_OK) {
    psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
    netwib_goto(NETWIB_ERR_OK);
  }

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_SEEK_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));
  ret = netwib_file_ctl_get_tell(pitem->value.itemreg.pio, &psmbcmdr->value.seek_r.offset);
  if (ret != NETWIB_ERR_OK) {
    psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
    netwib_goto(NETWIB_ERR_OK);
  }

  /* reset offset for next read */
  netwib__uint64_init_uint32(psmbcmdr->value.seek_r.offset, pitem->value.itemreg.offset);
  netwib__buf_reinit(&pitem->value.itemreg.readbuf);
  pitem->value.itemreg.readeofreached = NETWIB_FALSE;

  /* end */
 netwib_gotolabel:
  psmbmsgq = psmbmsgq; /* for compiler warning */
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_setinformation2(netwox_smbser *psmbser,
                                             netwox_constsmbmsg *psmbmsgq,
                                             netwox_constsmbcmd *psmbcmdq,
                                             netwox_smbmsg *psmbmsgr,
                                             netwox_smbcmd *psmbcmdr)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwox_smbserfid_item *pitem;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_setinformation2\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_SETINFORMATION2_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Fileid: %{uint16}\n", psmbcmdq->value.setinformation2_q.fileid));
  }
  ret = netwox_smbserfid_search(&psmbser->smbserfid, psmbcmdq->value.setinformation2_q.fileid, &pitem);
  if (ret != NETWIB_ERR_OK || pitem->fidtype != NETWOX_SMBSERFID_FIDTYPE_REG) {
    psmbser->smberrtosend = NETWOX_SMBERR_BADFILEID;
    netwib_goto(NETWIB_ERR_OK);
  }

  /* send reply without setting requested time */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_SETINFORMATION2_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));

  /* end */
 netwib_gotolabel:
  psmbmsgq = psmbmsgq; /* for compiler warning */
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_queryinformation2(netwox_smbser *psmbser,
                                               netwox_constsmbmsg *psmbmsgq,
                                               netwox_constsmbcmd *psmbcmdq,
                                               netwox_smbmsg *psmbmsgr,
                                               netwox_smbcmd *psmbcmdr)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwox_smbserfid_item *pitem;
  netwib_pathstat pathstat;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_queryinformation2\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_QUERYINFORMATION2_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }

  /* obtain filename */
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Fileid: %{uint16}\n", psmbcmdq->value.queryinformation2_q.fileid));
  }
  ret = netwox_smbserfid_search(&psmbser->smbserfid, psmbcmdq->value.queryinformation2_q.fileid, &pitem);
  if (ret != NETWIB_ERR_OK || pitem->fidtype != NETWOX_SMBSERFID_FIDTYPE_REG) {
    psmbser->smberrtosend = NETWOX_SMBERR_BADFILEID;
    netwib_goto(NETWIB_ERR_OK);
  }
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Real filename: %{buf}\n", &pitem->realname));
  }

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_QUERYINFORMATION2_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));
  ret = netwib_pathstat_init(&pitem->realname, &pathstat);
  if (ret != NETWIB_ERR_OK) {
    psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
    netwib_goto(NETWIB_ERR_OK);
  }
  /* fake values for creation, lastaccess and lastwrite but should be ok */
  netwib_eg(netwox_timemsdos_init_time(&pathstat.mtime, &psmbcmdr->value.queryinformation2_r.creationtime));
  netwib_eg(netwox_timemsdos_init_time(&pathstat.mtime, &psmbcmdr->value.queryinformation2_r.lastaccesstime));
  netwib_eg(netwox_timemsdos_init_time(&pathstat.mtime, &psmbcmdr->value.queryinformation2_r.lastwritetime));
  if (!psmbser->pcommon->allowput) {
    psmbcmdr->value.queryinformation2_r.fileattributes |= NETWOX_SMBCMDCMN_FILEATTR16_READONLY;
  }
  if (pathstat.type == NETWIB_PATHSTAT_TYPE_DIR) {
    psmbcmdr->value.queryinformation2_r.allocationsize = 0;
    psmbcmdr->value.queryinformation2_r.filesize = 0;
    psmbcmdr->value.queryinformation2_r.fileattributes |= NETWOX_SMBCMDCMN_FILEATTR16_DIR;
  } else {
    psmbcmdr->value.queryinformation2_r.allocationsize = pathstat.size;
    psmbcmdr->value.queryinformation2_r.filesize = pathstat.size;
  }

  /* end */
 netwib_gotolabel:
  psmbmsgq = psmbmsgq; /* for compiler warning */
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_lockingandx(netwox_smbser *psmbser,
                                               netwox_constsmbmsg *psmbmsgq,
                                               netwox_constsmbcmd *psmbcmdq,
                                               netwox_smbmsg *psmbmsgr,
                                               netwox_smbcmd *psmbcmdr)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwox_smbserfid_item *pitem;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_lockingandx\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_LOCKINGANDX_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }

  /* obtain filename */
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Fileid: %{uint16}\n", psmbcmdq->value.lockingandx_q.fileid));
  }
  ret = netwox_smbserfid_search(&psmbser->smbserfid, psmbcmdq->value.lockingandx_q.fileid, &pitem);
  if (ret != NETWIB_ERR_OK || pitem->fidtype != NETWOX_SMBSERFID_FIDTYPE_REG) {
    psmbser->smberrtosend = NETWOX_SMBERR_BADFILEID;
    netwib_goto(NETWIB_ERR_OK);
  }
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Real filename: %{buf}\n", &pitem->realname));
  }

  /* do nothing, and check nothing more, because locking is not implemented */

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_LOCKINGANDX_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));

  /* end */
 netwib_gotolabel:
  psmbmsgq = psmbmsgq; /* for compiler warning */
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_echo(netwox_smbser *psmbser,
                                  netwox_constsmbmsg *psmbmsgq,
                                  netwox_constsmbcmd *psmbcmdq,
                                  netwib_bool *psendreply)
{
  netwox_smbmsg smbmsgr;
  netwox_smbcmd smbcmdr;
  netwib_uint32 i;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_echo\n"));
  }
  /* no check for psmbser->currentsession nor psession->state because
     this kind of command might be sent out of a session (userid=0,
     treeid=0xFFFF) */

  /* init */
  ret = NETWIB_ERR_OK;

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_ECHO_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (psmbcmdq->value.echo_q.count > 16) {
    /* do not allow denial of service */
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }

  /* send replies */
  for (i = 1 ; i <= psmbcmdq->value.echo_q.count; i++) {
    if (psmbser->pcommon->verbose) {
      netwib_eg(netwib_fmt_display("Reply number %{uint32}\n", i));
    }
    netwib_eg(netwox_smbser_send_begin(psmbser, psmbmsgq, &smbmsgr, &smbcmdr,
                                       NETWOX_SMBCMD_TYPE_ECHO_R));
    smbcmdr.value.echo_r.number = (netwib_uint16)i;
    netwib_eg(netwib_buf_init_ext_buf(&psmbcmdq->value.echo_q.data,
                                      &smbcmdr.value.echo_r.data));
    netwib_eg(netwox_smbser_send_end(psmbser, &smbmsgr, &smbcmdr));
  }
  *psendreply = NETWIB_FALSE;

  /* end */
 netwib_gotolabel:
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_openandx(netwox_smbser *psmbser,
                                     netwox_constsmbmsg *psmbmsgq,
                                     netwox_constsmbcmd *psmbcmdq,
                                     netwox_smbmsg *psmbmsgr,
                                     netwox_smbcmd *psmbcmdr,
                                     netwib_constbuf *prootdir)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_buf *pfilename, *prealfilename;
  netwox_smbserfid_item *pitem=NULL;
  netwib_bool normalend;
  netwib_bool ifexists_replace, ifexists_open, ifexists_error;
  netwib_bool ifnoexists_create, ifnoexists_error;
  netwox_smbserfid_fidtype fidtype;
  netwox_smbserfid_regiotype regiotype;
  netwox_smbcmdcmn_createaction createaction;
  netwib_pathstat pathstat;
  netwib_uint16 fileid;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_openandx\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;
  normalend = NETWIB_FALSE;
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pfilename));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &prealfilename));
  netwib_er(netwox_smbserfid_new(&psmbser->smbserfid, &fileid, &pitem));

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_OPENANDX_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  /* check openflags */
  fidtype = NETWOX_SMBSERFID_FIDTYPE_REG;
  if (psmbcmdq->value.openandx_q.openflags & (NETWOX_SMBCMDCMN_OPENFLAGS16_ADDITIONALINFO|NETWOX_SMBCMDCMN_OPENFLAGS16_EXTENDEDRESPONSE)) {
    /* don't know what to do, and ignoring seems to work */
  }
  if (psmbcmdq->value.openandx_q.openflags & (NETWOX_SMBCMDCMN_OPENFLAGS16_EXCLUSIVEOPLOCK|NETWOX_SMBCMDCMN_OPENFLAGS16_BATCHOPLOCK)) {
    /* below in this function will say that oplock is not granted */
  }
  if (psmbcmdq->value.openandx_q.openflags & NETWOX_SMBCMDCMN_OPENFLAGS16_TARGETISADIR) {
    fidtype = NETWOX_SMBSERFID_FIDTYPE_DIR;
  }
  /* check ACCESS desiredaccess, but ignore WRITETHROUGH, CACHE, LOCALITY
     and SHARING */
  regiotype = NETWOX_SMBSERFID_REGIOTYPE_NOTOPEN;
  switch(psmbcmdq->value.openandx_q.desiredaccess & NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS) {
  case NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS_R:
  case NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS_X:
    regiotype = NETWOX_SMBSERFID_REGIOTYPE_READ;
    break;
  case NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS_W:
    regiotype = NETWOX_SMBSERFID_REGIOTYPE_WRITE;
    if (!psmbser->pcommon->allowput) {
      psmbser->smberrtosend = NETWOX_SMBERR_DEVICEWRITEPROTECTED;
      netwib_goto(NETWIB_ERR_OK);
    }
    break;
  case NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS_RW:
    regiotype = NETWOX_SMBSERFID_REGIOTYPE_RDWR;
    if (!psmbser->pcommon->allowput) {
      psmbser->smberrtosend = NETWOX_SMBERR_DEVICEWRITEPROTECTED;
      netwib_goto(NETWIB_ERR_OK);
    }
    break;
  default :
    netwib_er(netwox_smbser_notimplemented(psmbser));
    psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
    netwib_goto(NETWIB_ERR_OK);
    break;
  }
  /* ignore psmbcmdq->value.openandx_q.searchattributes, fileattributes
     and creationtime */
  /* check openfunction */
  ifexists_replace = NETWIB_FALSE;
  ifexists_open = NETWIB_FALSE;
  ifexists_error = NETWIB_FALSE;
  ifnoexists_create = NETWIB_FALSE;
  ifnoexists_error = NETWIB_FALSE;
  switch(psmbcmdq->value.openandx_q.openfunction & NETWOX_SMBCMDCMN_OPENFUNCTION_CREATEFILENOEXIST) {
  case NETWOX_SMBCMDCMN_OPENFUNCTION_CREATEFILENOEXIST_ERROR:
    ifnoexists_error = NETWIB_TRUE;
    break;
  case NETWOX_SMBCMDCMN_OPENFUNCTION_CREATEFILENOEXIST_OVERWRITE:
    ifnoexists_create = NETWIB_TRUE;
    break;
  }
  switch(psmbcmdq->value.openandx_q.openfunction & NETWOX_SMBCMDCMN_OPENFUNCTION_OPENFILEEXIST) {
  case NETWOX_SMBCMDCMN_OPENFUNCTION_OPENFILEEXIST_ERROR:
    ifexists_error = NETWIB_TRUE;
    break;
  case NETWOX_SMBCMDCMN_OPENFUNCTION_OPENFILEEXIST_OPEN:
    ifexists_open = NETWIB_TRUE;
    break;
  case NETWOX_SMBCMDCMN_OPENFUNCTION_OPENFILEEXIST_TRUNCATE:
    ifexists_replace = NETWIB_TRUE;
    break;
  }
  /* ignore psmbcmdq->value.openandx_q.allocationsize */
  /* obtain filename */
  netwib_eg(netwox_smbcmd_decode_buf_path(psmbmsgq, &psmbcmdq->value.openandx_q.filename, pfilename));
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Filename: %{buf}\n", pfilename));
  }
  netwib_eg(netwox_smbserjail_E(psmbser, prootdir, NETWIB_PATHSTAT_TYPE_UNKNOWN, pfilename, prealfilename));

  /* open */
  netwib_eg(netwox_smbsercmd__open_E(psmbser/*psmbser*/,
                                     pitem/*pitem*/,
                                     prealfilename/*prealpathname*/,
                                     NETWIB_FALSE/*deleteonclose*/,
                                     fidtype/*fidtype*/,
                                     regiotype/*regiotype*/,
                                     ifexists_replace/*ifexists_replace*/,
                                     ifexists_open/*ifexists_open*/,
                                     ifexists_error/*ifexists_error*/,
                                     ifnoexists_create/*ifnoexists_create*/,
                                     ifnoexists_error/*ifnoexists_error*/,
                                     &createaction/*pcreateaction*/));

  if (psmbser->smberrtosend != NETWOX_SMBERR_OK) {
    netwib_goto(NETWIB_ERR_OK);
  }

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_OPENANDX_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));
  psmbcmdr->value.openandx_r.fileid = fileid;
  if (!psmbser->pcommon->allowput) {
    psmbcmdr->value.openandx_r.fileattributes = NETWOX_SMBCMDCMN_FILEATTR16_READONLY;
  }
  psmbcmdr->value.openandx_r.grantedaccess = NETWOX_SMBCMDCMN_ACCESSMODE_WRITETHROUGH & NETWOX_SMBCMDCMN_ACCESSMODE_WRITETHROUGH_NO;
  psmbcmdr->value.openandx_r.grantedaccess |= NETWOX_SMBCMDCMN_ACCESSMODE_CACHE & NETWOX_SMBCMDCMN_ACCESSMODE_CACHE_NONE;
  psmbcmdr->value.openandx_r.grantedaccess |= NETWOX_SMBCMDCMN_ACCESSMODE_LOCALITY & NETWOX_SMBCMDCMN_ACCESSMODE_LOCALITY_UNKNOWN;
  psmbcmdr->value.openandx_r.grantedaccess |= NETWOX_SMBCMDCMN_ACCESSMODE_SHARING & NETWOX_SMBCMDCMN_ACCESSMODE_SHARING_COMPATIBILITY;
  if (pitem->value.itemreg.regiotype == NETWOX_SMBSERFID_REGIOTYPE_WRITE) {
    psmbcmdr->value.openandx_r.grantedaccess |= NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS & NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS_W;
  } else if (pitem->value.itemreg.regiotype == NETWOX_SMBSERFID_REGIOTYPE_RDWR) {
    psmbcmdr->value.openandx_r.grantedaccess |= NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS & NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS_RW;
  } else if (pitem->value.itemreg.regiotype == NETWOX_SMBSERFID_REGIOTYPE_READ) {
    psmbcmdr->value.openandx_r.grantedaccess |= NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS & NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS_R;
  }
  netwib_eg(netwib_pathstat_init(prealfilename, &pathstat));
  netwib_eg(netwox_time1970l_init_time(&pathstat.mtime, &psmbcmdr->value.openandx_r.lastwritetime));
  if (pathstat.type == NETWIB_PATHSTAT_TYPE_DIR) {
    psmbcmdr->value.openandx_r.filesize = 0;
  } else {
    psmbcmdr->value.openandx_r.filesize = pathstat.size;
  }
  psmbcmdr->value.openandx_r.filetype = NETWOX_SMBCMDCMN_FILETYPE_FILEDIR;
  psmbcmdr->value.openandx_r.action = (NETWOX_SMBCMDCMN_CREATEACTION_OPLOCK & NETWOX_SMBCMDCMN_CREATEACTION_OPLOCK_NO) | createaction;

  /* end */
  normalend = NETWIB_TRUE;
 netwib_gotolabel:
  if (!normalend) {
    netwib_er(netwox_smbserfid_del(&psmbser->smbserfid, fileid));
  }
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &prealfilename));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pfilename));
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_readandx(netwox_smbser *psmbser,
                                      netwox_constsmbmsg *psmbmsgq,
                                      netwox_constsmbcmd *psmbcmdq,
                                      netwox_smbmsg *psmbmsgr,
                                      netwox_smbcmd *psmbcmdr)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwox_smbserfid_item *pitem;
  netwib_bool needtoread;
  netwib_uint32 sizetocopy;
  netwib_uint64 sizetocopy64;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_readandx\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_READANDX_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  /* obtain real filename from fileid */
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Fileid: %{uint16}\n", psmbcmdq->value.readandx_q.fileid));
  }
  ret = netwox_smbserfid_search(&psmbser->smbserfid, psmbcmdq->value.readandx_q.fileid, &pitem);
  if (ret != NETWIB_ERR_OK || pitem->fidtype != NETWOX_SMBSERFID_FIDTYPE_REG) {
    psmbser->smberrtosend = NETWOX_SMBERR_BADFILEID;
    netwib_goto(NETWIB_ERR_OK);
  }
  if (pitem->value.itemreg.regiotype != NETWOX_SMBSERFID_REGIOTYPE_READ &&
      pitem->value.itemreg.regiotype != NETWOX_SMBSERFID_REGIOTYPE_RDWR) {
    psmbser->smberrtosend = NETWOX_SMBERR_BADFILEID;
    netwib_goto(NETWIB_ERR_OK);
  }
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Real filename: %{buf}\n", &pitem->realname));
  }
  /* will use psmbcmdq->value.readandx_q.maxsize and offset below */
  /* ignore psmbcmdq->value.readandx_q.minsize and remaining */

  /* ensure readbuf contains sufficient data */
  needtoread = NETWIB_FALSE;
  if (netwib__uint64_cmp_eq(psmbcmdq->value.readandx_q.offset, pitem->value.itemreg.offset)) {
    if (netwib__buf_ref_data_size(&pitem->value.itemreg.readbuf) < psmbcmdq->value.readandx_q.maxsize) {
      needtoread = NETWIB_TRUE;
    }
  } else {
    /* read new data */
    needtoread = NETWIB_TRUE;
    netwib__buf_reinit(&pitem->value.itemreg.readbuf);
    pitem->value.itemreg.offset = psmbcmdq->value.readandx_q.offset;
    ret = netwib_file_ctl_set_seek64_begin(pitem->value.itemreg.pio, &pitem->value.itemreg.offset);
    if (ret != NETWIB_ERR_OK) {
      /* perhaps a bad offset, so simulate end */
      pitem->value.itemreg.readeofreached = NETWIB_TRUE;
      ret = NETWIB_ERR_OK;
    } else {
      pitem->value.itemreg.readeofreached = NETWIB_FALSE;
    }
  }
  if (needtoread && !pitem->value.itemreg.readeofreached) {
    ret = netwib_io_read(pitem->value.itemreg.pio, &pitem->value.itemreg.readbuf);
    if (ret == NETWIB_ERR_OK) {
      if (netwib__buf_ref_data_size(&pitem->value.itemreg.readbuf) == 0) {
        pitem->value.itemreg.readeofreached = NETWIB_TRUE;
      }
    } else if (ret == NETWIB_ERR_DATAEND) {
      pitem->value.itemreg.readeofreached = NETWIB_TRUE;
      ret = NETWIB_ERR_OK;
    } else {
      netwib_goto(ret);
    }
  }

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_READANDX_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));
  if (pitem->value.itemreg.readeofreached &&
      netwib__buf_ref_data_size(&pitem->value.itemreg.readbuf) == 0) {
    psmbcmdr->value.readandx_r.remaining = 0;
  } else {
    psmbcmdr->value.readandx_r.remaining = NETWOX_SMBCMD_READANDX_R_REMAINING_SAFE;
    sizetocopy = netwib__buf_ref_data_size(&pitem->value.itemreg.readbuf);
    if (sizetocopy > psmbcmdq->value.readandx_q.maxsize) {
      sizetocopy = psmbcmdq->value.readandx_q.maxsize;
    }
    netwib_eg(netwib_buf_append_data(netwib__buf_ref_data_ptr(&pitem->value.itemreg.readbuf), sizetocopy, &psmbcmdr->value.readandx_r.data));
    pitem->value.itemreg.readbuf.beginoffset += sizetocopy;
    netwib__uint64_init_uint32(sizetocopy, sizetocopy64);
    netwib__uint64_add(sizetocopy64, pitem->value.itemreg.offset, pitem->value.itemreg.offset);
  }

  /* end */
 netwib_gotolabel:
  psmbmsgq = psmbmsgq; /* for compiler warning */
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_writeandx(netwox_smbser *psmbser,
                                       netwox_constsmbmsg *psmbmsgq,
                                       netwox_constsmbcmd *psmbcmdq,
                                       netwox_smbmsg *psmbmsgr,
                                       netwox_smbcmd *psmbcmdr)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwox_smbserfid_item *pitem;
  netwib_uint32 writtensize;
  netwib_uint64 writtensize64, offset64;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_writeandx\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_WRITEANDX_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  /* obtain real filename from fileid */
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Fileid: %{uint16}\n", psmbcmdq->value.writeandx_q.fileid));
  }
  ret = netwox_smbserfid_search(&psmbser->smbserfid, psmbcmdq->value.writeandx_q.fileid, &pitem);
  if (ret != NETWIB_ERR_OK || pitem->fidtype != NETWOX_SMBSERFID_FIDTYPE_REG) {
    psmbser->smberrtosend = NETWOX_SMBERR_BADFILEID;
    netwib_goto(NETWIB_ERR_OK);
  }
  if (pitem->value.itemreg.regiotype != NETWOX_SMBSERFID_REGIOTYPE_WRITE &&
      pitem->value.itemreg.regiotype != NETWOX_SMBSERFID_REGIOTYPE_RDWR) {
    psmbser->smberrtosend = NETWOX_SMBERR_BADFILEID;
    netwib_goto(NETWIB_ERR_OK);
  }
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Real filename: %{buf}\n", &pitem->realname));
  }
  /* will use psmbcmdq->value.writeandx_q.offset and data below */
  /* ignore psmbcmdq->value.writeandx_q.writemode and remaining */

  /* check current offset */
  if (!netwib__uint64_cmp_eq(psmbcmdq->value.writeandx_q.offset, pitem->value.itemreg.offset)) {
    offset64 = psmbcmdq->value.writeandx_q.offset;
    ret = netwib_file_ctl_set_seek64_begin(pitem->value.itemreg.pio, &offset64);
    if (ret != NETWIB_ERR_OK) {
      psmbser->smberrtosend = NETWOX_SMBERR_BADPARAMETER;
      netwib_goto(NETWIB_ERR_OK);
    }
    pitem->value.itemreg.offset = psmbcmdq->value.writeandx_q.offset;
  }
  /* reset for next read */
  netwib__buf_reinit(&pitem->value.itemreg.readbuf);
  pitem->value.itemreg.readeofreached = NETWIB_FALSE;

  /* write data */
  ret = netwib_io_write(pitem->value.itemreg.pio, &psmbcmdq->value.writeandx_q.data);
  if (ret != NETWIB_ERR_OK) {
    psmbser->smberrtosend = NETWOX_SMBERR_QUOTAEXCEEDED;
    netwib_goto(NETWIB_ERR_OK);
  }
  writtensize = netwib__buf_ref_data_size(&psmbcmdq->value.writeandx_q.data);

  /* update offset */
  netwib__uint64_init_uint32(writtensize, writtensize64);
  netwib__uint64_add(writtensize64, pitem->value.itemreg.offset, pitem->value.itemreg.offset);

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_WRITEANDX_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));
  psmbcmdr->value.writeandx_r.writtensize = (netwib_uint16)writtensize;
  psmbcmdr->value.writeandx_r.remaining = NETWOX_SMBCMD_WRITEANDX_R_REMAINING_SAFE;

  /* end */
 netwib_gotolabel:
  psmbmsgq = psmbmsgq; /* for compiler warning */
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_closetreedisconnect(netwox_smbser *psmbser,
                                                 netwox_constsmbmsg *psmbmsgq,
                                                 netwox_constsmbcmd *psmbcmdq,
                                                 netwox_smbmsg *psmbmsgr,
                                                 netwox_smbcmd *psmbcmdr)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_closetreedisconnect\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_CLOSETREEDISCONNECT_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  /* ignore psmbcmdq->value.closetreedisconnect_q.lastwritetime */

  /* close */
  netwib_eg(netwox_smbsercmd__close_E(psmbser, psmbcmdq->value.closetreedisconnect_q.fileid));
  if (psmbser->smberrtosend != NETWOX_SMBERR_OK) {
    netwib_goto(NETWIB_ERR_OK);
  }

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_CLOSETREEDISCONNECT_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));

  /* end */
  ptree->state = NETWOX_SMBSER_SESSIONTREE_STATE_CLOSED;
  netwib_eg(netwox_smbser_sessiontree_display(psmbser));
 netwib_gotolabel:
  psmbmsgq = psmbmsgq; /* for compiler warning */
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_findclose2(netwox_smbser *psmbser,
                                        netwox_constsmbmsg *psmbmsgq,
                                        netwox_constsmbcmd *psmbcmdq,
                                        netwox_smbmsg *psmbmsgr,
                                        netwox_smbcmd *psmbcmdr)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_findclose2\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_FINDCLOSE2_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  /* obtain real filename from fileid */
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Search id: %{uint16}\n", psmbcmdq->value.findclose2_q.sid));
  }

  /* close the resource */
  ret = netwox_smbserglob_del(&psmbser->smbserglob,
                              psmbcmdq->value.findclose2_q.sid);
  if (ret != NETWIB_ERR_OK) {
    psmbser->smberrtosend = NETWOX_SMBERR_BADFILEID;
    netwib_goto(NETWIB_ERR_OK);
  }

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_FINDCLOSE2_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));

  /* end */
 netwib_gotolabel:
  psmbmsgq = psmbmsgq; /* for compiler warning */
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_treedisconnect(netwox_smbser *psmbser,
                                            netwox_constsmbmsg *psmbmsgq,
                                            netwox_constsmbcmd *psmbcmdq,
                                            netwox_smbmsg *psmbmsgr,
                                            netwox_smbcmd *psmbcmdr)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_treedisconnect\n"));
  }
  netwib_er(netwox_smbser_sessiontree_get(psmbser, &psession, &ptree));
  /* allow CLOSED state because sometimes Windows 2000 sends
     a logout before a tree disconnect */
  if (psession->state != NETWOX_SMBSER_SESSIONTREE_STATE_INUSE &&
      psession->state != NETWOX_SMBSER_SESSIONTREE_STATE_CLOSED) {
    return(NETWOX_ERR_PROTOCOL);
  }
  if (ptree->state != NETWOX_SMBSER_SESSIONTREE_STATE_INUSE &&
      ptree->state != NETWOX_SMBSER_SESSIONTREE_STATE_CLOSED) {
    return(NETWOX_ERR_PROTOCOL);
  }

  /* init */
  ret = NETWIB_ERR_OK;

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_TREEDISCONNECT_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_TREEDISCONNECT_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));

  /* end */
  ptree->state = NETWOX_SMBSER_SESSIONTREE_STATE_CLOSED;
  netwib_eg(netwox_smbser_sessiontree_display(psmbser));
 netwib_gotolabel:
  psmbmsgq = psmbmsgq; /* for compiler warning */
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_negotiate(netwox_smbser *psmbser,
                                       netwox_constsmbmsg *psmbmsgq,
                                       netwox_constsmbcmd *psmbcmdq,
                                       netwox_smbmsg *psmbmsgr,
                                       netwox_smbcmd *psmbcmdr,
                                       netwib_constbuf *pdomain)
{
  netwib_time t;
  netwib_localtime lt;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_negotiate\n"));
  }
  if (psmbser->negotiated) {
    return(NETWOX_ERR_PROTOCOL);
  }

  /* init */
  ret = NETWIB_ERR_OK;

  /* check query */
  if (psmbcmdq->value.negotiate_q.ntlm012position == 0) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_NEGOTIATE_R0));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));
  psmbcmdr->value.negotiate_r0.dialectindex = psmbcmdq->value.negotiate_q.ntlm012position - 1;
  psmbcmdr->value.negotiate_r0.secumode = psmbser->cmn_secumode;
  psmbcmdr->value.negotiate_r0.maxmultiplex = 1;
  psmbcmdr->value.negotiate_r0.maxvc = 1;
  psmbcmdr->value.negotiate_r0.maxmsgsize = psmbser->cmn_maxmsgsize;
  psmbcmdr->value.negotiate_r0.vcid = psmbser->cmn_vcid;
  /* if NETWOX_SMBCMDCMN_NEGOCAPA_NTSMBS is unset, Windows 2000 client
     sends unicode without setting flags2. So, we have to set it to make
     it work. */
  psmbcmdr->value.negotiate_r0.capabilities = NETWOX_SMBCMDCMN_NEGOCAPA_UNICODE | NETWOX_SMBCMDCMN_NEGOCAPA_NTERROR | NETWOX_SMBCMDCMN_NEGOCAPA_LARGEFILES | NETWOX_SMBCMDCMN_NEGOCAPA_LARGEREADANDX | NETWOX_SMBCMDCMN_NEGOCAPA_LARGEWRITEANDX | NETWOX_SMBCMDCMN_NEGOCAPA_NTSMBS;
  netwib_eg(netwib_time_init_now(&t));
  netwib_eg(netwox_time1601_init_time(&t, &psmbcmdr->value.negotiate_r0.systemtime));
  netwib_eg(netwib_time_decode_localtime(&t, &lt));
  psmbcmdr->value.negotiate_r0.timezoneoffset = (netwib_uint16)(lt.zoneoffset/60);
  netwib_eg(netwox_winauth_lm_chal_gene(psmbser->pcmn_challenge));
  netwib_eg(netwib_buf_append_buf(psmbser->pcmn_challenge,
                                  &psmbcmdr->value.negotiate_r0.challenge));
  /* domain is always stored in unicode (NETWOX_SMBCMDCMN_DATATYPE_STRING
     ONLY_UNINOPAD), even if unicode flag in flags2 is unset.
  */
  netwib_eg(netwox_buf_decode_ucs2le(pdomain,
                                     &psmbcmdr->value.negotiate_r0.domain));

  /* end */
  psmbser->negotiated = NETWIB_TRUE;
 netwib_gotolabel:
  psmbmsgq = psmbmsgq; /* for compiler warning */
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_sessionsetupandx(netwox_smbser *psmbser,
                                             netwox_constsmbmsg *psmbmsgq,
                                             netwox_constsmbcmd *psmbcmdq,
                                             netwox_smbmsg *psmbmsgr,
                                             netwox_smbcmd *psmbcmdr,
                                             netwib_constbuf *pdomain,
                                             netwib_constbuf *puser,
                                             netwib_constbuf *ppassword)
{
  netwib_buf *phash, *pwantedpassword, *pwantedunicodepassword, blob, buf;
  netwox_smbser_session *psession;
  netwib_cmp cmp;
  netwib_bool authok;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_sessionsetupandx\n"));
  }
  if (!psmbser->negotiated) {
    return(NETWOX_ERR_PROTOCOL);
  }

  /* init */
  ret = NETWIB_ERR_OK;
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &phash));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pwantedpassword));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pwantedunicodepassword));

  /* add a session */
  netwib_eg(netwox_smbser_session_add(psmbser, &psession));

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_Q0) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  /* ignore psmbcmdq->value.sessionsetupandx_q0.vcnumber (sometimes 0,
     sometimes 1, perhaps other values) */
  if (psmbcmdq->value.sessionsetupandx_q0.vcid != psmbser->cmn_vcid) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }

  /* save negotiated values */
  if (psmbcmdq->value.sessionsetupandx_q0.maxmsgsize < psmbser->cmn_maxmsgsize) {
    psmbser->cmn_maxmsgsize = psmbcmdq->value.sessionsetupandx_q0.maxmsgsize;
    /* check now, because this value is used in tsc and search commands */
    if (psmbser->cmn_maxmsgsize <= NETWOX_SMBSER_SUFFICIENT_FOR_HEADERS) {
      netwib_er(netwox_smbser_internalerror(psmbser));
      netwib_eg(NETWOX_ERR_INTERNALERROR);
    }
  }
  /* save capabilitites we support */
  psession->capabilities = 0;
  if (psmbcmdq->value.sessionsetupandx_q0.capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_UNICODE) {
    psession->capabilities |= NETWOX_SMBCMDCMN_NEGOCAPA_UNICODE;
  }
  if (psmbcmdq->value.sessionsetupandx_q0.capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_LARGEFILES) {
    psession->capabilities |= NETWOX_SMBCMDCMN_NEGOCAPA_LARGEFILES;
  }
  if (psmbcmdq->value.sessionsetupandx_q0.capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_NTERROR) {
    psession->capabilities |= NETWOX_SMBCMDCMN_NEGOCAPA_NTERROR;
  }
  if (psmbcmdq->value.sessionsetupandx_q0.capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_LARGEREADANDX) {
    psession->capabilities |= NETWOX_SMBCMDCMN_NEGOCAPA_LARGEREADANDX;
  }
  if (psmbcmdq->value.sessionsetupandx_q0.capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_LARGEWRITEANDX) {
    psession->capabilities |= NETWOX_SMBCMDCMN_NEGOCAPA_LARGEWRITEANDX;
  }

  /* check authentication */
  authok = NETWIB_FALSE;
  if (netwib__buf_ref_data_size(ppassword) == 0) {
    authok = NETWIB_TRUE;
    if (psmbser->pcommon->verbose) {
      netwib_er(netwib_fmt_display("Auth OK with no password.\n"));
    }
  }
  /* LM */
  if (!authok) {
    netwib_eg(netwox_winauth_lm_hash(ppassword, phash));
    netwib_eg(netwox_winauth_lm_chal_answer(psmbser->pcmn_challenge, phash, pwantedpassword));
    netwib_eg(netwib_buf_cmp(pwantedpassword, &psmbcmdq->value.sessionsetupandx_q0.password, &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      authok = NETWIB_TRUE;
      if (psmbser->pcommon->verbose) {
        netwib_er(netwib_fmt_display("Auth OK with LM.\n"));
      }
    }
  }
  /* NTLMv1 */
  if (!authok) {
    netwib__buf_reinit(phash);
    netwib__buf_reinit(pwantedpassword);
    netwib_eg(netwox_winauth_ntlmv1_hash(ppassword, phash));
    netwib_eg(netwox_winauth_ntlmv1_chal_answer(psmbser->pcmn_challenge, phash, pwantedpassword));
    netwib_eg(netwib_buf_cmp(pwantedpassword, &psmbcmdq->value.sessionsetupandx_q0.password, &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      authok = NETWIB_TRUE;
      if (psmbser->pcommon->verbose) {
        netwib_er(netwib_fmt_display("Auth OK with NTLMv1 ASCII password.\n"));
      }
    } else {
      netwib_eg(netwib_buf_cmp(pwantedpassword, &psmbcmdq->value.sessionsetupandx_q0.unicodepassword, &cmp));
      if (cmp == NETWIB_CMP_EQ) {
        authok = NETWIB_TRUE;
        if (psmbser->pcommon->verbose) {
          netwib_er(netwib_fmt_display("Auth OK with NTLMv1 Unicode password.\n"));
        }
      }
    }
  }
  /* NTLMv2 */
  if (!authok) {
    netwib__buf_reinit(pwantedpassword);
    netwib__buf_reinit(pwantedunicodepassword);
    ret = netwox_winauth_ntlmv2_blob_get(&psmbcmdq->value.sessionsetupandx_q0.unicodepassword, &blob);
    if (ret == NETWIB_ERR_OK) {
      netwib_eg(netwox_winauth_ntlmv2_chal_answer(psmbser->pcmn_challenge, puser, ppassword, pdomain, &blob, pwantedunicodepassword, pwantedpassword));
      netwib_eg(netwib_buf_cmp(pwantedpassword, &psmbcmdq->value.sessionsetupandx_q0.password, &cmp));
      if (cmp == NETWIB_CMP_EQ) {
        authok = NETWIB_TRUE;
        if (psmbser->pcommon->verbose) {
          netwib_er(netwib_fmt_display("Auth OK with NTLMv2 ASCII password.\n"));
        }
      } else {
        netwib_eg(netwib_buf_cmp(pwantedunicodepassword, &psmbcmdq->value.sessionsetupandx_q0.unicodepassword, &cmp));
        if (cmp == NETWIB_CMP_EQ) {
          authok = NETWIB_TRUE;
          if (psmbser->pcommon->verbose) {
            netwib_er(netwib_fmt_display("Auth OK with NTLMv2 Unicode password.\n"));
          }
        }
      }
    }
  }
  /* bad authentication */
  if (!authok) {
    if (psmbser->pcommon->verbose) {
      netwib_er(netwib_fmt_display("Auth not OK.\n"));
    }
    psmbser->smberrtosend = NETWOX_SMBERR_BADPASSWORD;
    netwib_goto(NETWIB_ERR_OK);
  }

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_R0));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));
  psmbcmdr->value.sessionsetupandx_r0.i1set = NETWIB_TRUE;
  netwib_eg(netwib_buf_init_ext_string("Netwox", &buf));
  netwib_eg(netwox_smbcmd_append_buf(psmbmsgr, &buf, &psmbcmdr->value.sessionsetupandx_r0.i1.nativeos));
  netwib_eg(netwox_smbcmd_append_buf(psmbmsgr, &buf, &psmbcmdr->value.sessionsetupandx_r0.i1.nativelanman));
  psmbcmdr->value.sessionsetupandx_r0.i1.i2set = NETWIB_TRUE;
  netwib_eg(netwox_smbcmd_append_buf(psmbmsgr, pdomain, &psmbcmdr->value.sessionsetupandx_r0.i1.i2.domain));

  /* end */
  psession->state = NETWOX_SMBSER_SESSIONTREE_STATE_INUSE;
  psmbmsgr->hdr.userid = psession->userid;
  netwib_eg(netwox_smbser_sessiontree_display(psmbser));
 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pwantedunicodepassword));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pwantedpassword));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &phash));
  psmbmsgq = psmbmsgq; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_logoffandx(netwox_smbser *psmbser,
                                       netwox_constsmbmsg *psmbmsgq,
                                       netwox_constsmbcmd *psmbcmdq,
                                       netwox_smbmsg *psmbmsgr,
                                       netwox_smbcmd *psmbcmdr)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_uint32 j;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_logoffandx\n"));
  }
  /* do not check treeid because it can be zero to end all trees of same
     userid */
  netwib_er(netwox_smbser_sessiontree_get(psmbser, &psession, NULL));
  if (psession->state != NETWOX_SMBSER_SESSIONTREE_STATE_INUSE) {
    /* the only case were it is valid, is when NT bug occurred in
       netwox_smbser_smbmsg_recv(), and session does not point
       to the good value. So check this case. */
    if (psmbser->currentsession_beforentbug != NETWOX_SMBSER_CURRENTSESSION_NOTFOUND) {
      psession = (netwox_smbser_session *)psmbser->sessions.p[psmbser->currentsession_beforentbug];
      /* check the second time, with the original session */
      if (psession->state != NETWOX_SMBSER_SESSIONTREE_STATE_INUSE) {
        return(NETWOX_ERR_PROTOCOL);
      }
    } else {
      return(NETWOX_ERR_PROTOCOL);
    }
  }

  /* init */
  ret = NETWIB_ERR_OK;

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_LOGOFFANDX_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_LOGOFFANDX_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));

  /* change states */
  psession->state = NETWOX_SMBSER_SESSIONTREE_STATE_CLOSED;
  for (j = 0; j < psession->trees.size; j++) {
    ptree = (netwox_smbser_tree *)psession->trees.p[j];
    if (psmbmsgq->hdr.treeid == 0 || psmbmsgq->hdr.treeid == ptree->treeid) {
      ptree->state = NETWOX_SMBSER_SESSIONTREE_STATE_CLOSED;
    }
  }
  netwib_eg(netwox_smbser_sessiontree_display(psmbser));
 netwib_gotolabel:
  psmbmsgq = psmbmsgq; /* for compiler warning */
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_treeconnectandx(netwox_smbser *psmbser,
                                             netwox_constsmbmsg *psmbmsgq,
                                             netwox_constsmbcmd *psmbcmdq,
                                             netwox_smbmsg *psmbmsgr,
                                             netwox_smbcmd *psmbcmdr,
                                             netwib_constbuf *pshare)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_buf *pfullshare, buf;
  netwib_cmp cmp;
  netwib_data data, pc;
  netwib_uint32 datasize;
  netwib_bool any;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_treeconnectandx\n"));
  }
  if (psmbser->currentsession == NETWOX_SMBSER_CURRENTSESSION_NOTFOUND) {
    /* client tried an attack, or to use share access which is not
       supported */
    psmbser->smberrtosend = NETWOX_SMBERR_BADPASSWORD;
    netwib_goto(NETWIB_ERR_OK);
  }
  netwib_er(netwox_smbser_sessiontree_get(psmbser, &psession, NULL));
  if (psession->state != NETWOX_SMBSER_SESSIONTREE_STATE_INUSE) {
    /* Windows ME: if we disconnect a share, then reconnect it, the session
       is still in netwox, but no more in use or not even authenticated */
    psmbser->smberrtosend = NETWOX_SMBERR_BADPASSWORD;
    netwib_goto(NETWIB_ERR_OK);
  }
  /* Windows 98 keeps the treeid if server process is stopped and restarted
  if (psmbmsgq->hdr.treeid != 0) [
    //return(NETWOX_ERR_PROTOCOL);
  ]
  */
  netwib_er(netwox_smbser_tree_add(psmbser, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pfullshare));

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_TREECONNECTANDX_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  netwib_eg(netwib_buf_cmp_string(&psmbcmdq->value.treeconnectandx_q.service, NETWOX_SMBCMD_TREECONNECTANDX_SERVICE_SHARE, &cmp));
  any = NETWIB_FALSE;
  if (cmp == NETWIB_CMP_EQ) {
    ptree->isipc = NETWIB_FALSE;
  } else {
    netwib_eg(netwib_buf_cmp_string(&psmbcmdq->value.treeconnectandx_q.service, NETWOX_SMBCMD_TREECONNECTANDX_SERVICE_IPC, &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      ptree->isipc = NETWIB_TRUE;
    } else {
      netwib_eg(netwib_buf_cmp_string(&psmbcmdq->value.treeconnectandx_q.service, NETWOX_SMBCMD_TREECONNECTANDX_SERVICE_ANY, &cmp));
      if (cmp == NETWIB_CMP_EQ) {
        any = NETWIB_TRUE;
      } else {
        netwib_er(netwox_smbser_notimplemented(psmbser));
        psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
        netwib_goto(NETWIB_ERR_OK);
      }
    }
  }
  /* do not check password because share access is not supported */

  /* check share (like "\\host\share", but ignore host) */
  netwib_eg(netwox_smbcmd_decode_buf(psmbmsgq, &psmbcmdq->value.treeconnectandx_q.path, pfullshare));
  data = netwib__buf_ref_data_ptr(pfullshare);
  datasize = netwib__buf_ref_data_size(pfullshare);
  if (datasize < 2 || data[0] != '\\' || data[1] != '\\') {
    psmbser->smberrtosend = NETWOX_SMBERR_BADSHARENAME;
    netwib_goto(NETWIB_ERR_OK);
  }
  pc = netwib_c_memchr(data+2, '\\', datasize-2);
  if (pc == NULL) {
    psmbser->smberrtosend = NETWOX_SMBERR_BADSHARENAME;
    netwib_goto(NETWIB_ERR_OK);
  }
  pfullshare->beginoffset += (pc - data + 1);
  if (ptree->isipc || any) {
    netwib_eg(netwib_buf_casecmp_string(pfullshare, "IPC$", &cmp));
    if (any) {
      if (cmp == NETWIB_CMP_EQ) {
        ptree->isipc = NETWIB_TRUE;
      } else {
        ptree->isipc = NETWIB_FALSE;
      }
    } else {
      if (cmp != NETWIB_CMP_EQ) {
        psmbser->smberrtosend = NETWOX_SMBERR_BADSHARENAME;
        netwib_goto(NETWIB_ERR_OK);
      }
    }
  }
  if (!ptree->isipc) {
    if (netwib__buf_ref_data_size(pshare) != 0) {
      netwib_eg(netwib_buf_casecmp(pshare, pfullshare, &cmp));
      if (cmp != NETWIB_CMP_EQ) {
        psmbser->smberrtosend = NETWOX_SMBERR_BADSHARENAME;
        netwib_goto(NETWIB_ERR_OK);
      }
    }
  }

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_TREECONNECTANDX_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));
  if (ptree->isipc) {
    netwib_eg(netwib_buf_append_string(NETWOX_SMBCMD_TREECONNECTANDX_SERVICE_IPC, &psmbcmdr->value.treeconnectandx_r.service));
  } else {
    netwib_eg(netwib_buf_append_string(NETWOX_SMBCMD_TREECONNECTANDX_SERVICE_SHARE, &psmbcmdr->value.treeconnectandx_r.service));
    psmbcmdr->value.treeconnectandx_r.i1set = NETWIB_TRUE;
    netwib_eg(netwib_buf_init_ext_string(NETWOX_SMBSER_FAKE_FS_TYPE, &buf));
    netwib_eg(netwox_smbcmd_append_buf(psmbmsgr, &buf, &psmbcmdr->value.treeconnectandx_r.i1.nativefs));
  }

  /* end */
  ptree->state = NETWOX_SMBSER_SESSIONTREE_STATE_INUSE;
  psmbmsgr->hdr.treeid = ptree->treeid;
  netwib_eg(netwox_smbser_sessiontree_display(psmbser));
 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pfullshare));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_queryinformationdisk(netwox_smbser *psmbser,
                                                  netwox_constsmbmsg *psmbmsgq,
                                                  netwox_constsmbcmd *psmbcmdq,
                                                  netwox_smbmsg *psmbmsgr,
                                                  netwox_smbcmd *psmbcmdr)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_queryinformationdisk\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_QUERYINFORMATIONDISK_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_QUERYINFORMATIONDISK_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));
  psmbcmdr->value.queryinformationdisk_r.totalunits = NETWOX_SMBSER_FAKE_FS_TOTALUNITS;
  psmbcmdr->value.queryinformationdisk_r.sectorsperunit = NETWOX_SMBSER_FAKE_FS_SECTORSPERUNIT;
  psmbcmdr->value.queryinformationdisk_r.bytespersector = NETWOX_SMBSER_FAKE_FS_BYTESPERSECTOR;
  psmbcmdr->value.queryinformationdisk_r.freeunits = NETWOX_SMBSER_FAKE_FS_FREEUNITS;

  /* end */
 netwib_gotolabel:
  psmbmsgq = psmbmsgq; /* for compiler warning */
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_ntcreateandx(netwox_smbser *psmbser,
                                          netwox_constsmbmsg *psmbmsgq,
                                          netwox_constsmbcmd *psmbcmdq,
                                          netwox_smbmsg *psmbmsgr,
                                          netwox_smbcmd *psmbcmdr,
                                          netwib_constbuf *prootdir)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_buf *pfilename, *prealfilename;
  netwox_smbserfid_item *pitem=NULL;
  netwib_bool normalend, deleteonclose;
  netwib_bool ifexists_replace, ifexists_open, ifexists_error;
  netwib_bool ifnoexists_create, ifnoexists_error;
  netwox_smbcmdcmn_createaction createaction;
  netwox_smbserfid_fidtype fidtype;
  netwox_smbserfid_regiotype regiotype;
  netwib_pathstat pathstat;
  netwib_uint16 fileid;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_ntcreateandx\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;
  normalend = NETWIB_FALSE;
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pfilename));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &prealfilename));
  netwib_er(netwox_smbserfid_new(&psmbser->smbserfid, &fileid, &pitem));

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_NTCREATEANDX_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  /* check openflags */
  fidtype = NETWOX_SMBSERFID_FIDTYPE_UNKNOWN;
  if (psmbcmdq->value.ntcreateandx_q.openflags & (NETWOX_SMBCMDCMN_OPENFLAGS32_ADDITIONALINFO|NETWOX_SMBCMDCMN_OPENFLAGS32_EXTENDEDRESPONSE)) {
    /* don't know what to do, and ignoring seems to work */
  }
  if (psmbcmdq->value.ntcreateandx_q.openflags & (NETWOX_SMBCMDCMN_OPENFLAGS32_EXCLUSIVEOPLOCK|NETWOX_SMBCMDCMN_OPENFLAGS32_BATCHOPLOCK)) {
    /* below in this function will say that oplock is not granted */
  }
  if (psmbcmdq->value.ntcreateandx_q.openflags & NETWOX_SMBCMDCMN_OPENFLAGS32_TARGETISADIR) {
    fidtype = NETWOX_SMBSERFID_FIDTYPE_DIR;
  }
  /* check accessmask */
  regiotype = NETWOX_SMBSERFID_REGIOTYPE_NOTOPEN;
  if ((psmbcmdq->value.ntcreateandx_q.accessmask & NETWOX_SMBCMDCMN_ACCESSMASK_READ) && (psmbcmdq->value.ntcreateandx_q.accessmask & NETWOX_SMBCMDCMN_ACCESSMASK_WRITE)) {
    regiotype = NETWOX_SMBSERFID_REGIOTYPE_RDWR;
  } else if (psmbcmdq->value.ntcreateandx_q.accessmask & (NETWOX_SMBCMDCMN_ACCESSMASK_READ|NETWOX_SMBCMDCMN_ACCESSMASK_EXECUTE)) {
    regiotype = NETWOX_SMBSERFID_REGIOTYPE_READ;
  } else if (psmbcmdq->value.ntcreateandx_q.accessmask & NETWOX_SMBCMDCMN_ACCESSMASK_WRITE) {
    regiotype = NETWOX_SMBSERFID_REGIOTYPE_WRITE;
  } else if (psmbcmdq->value.ntcreateandx_q.accessmask & NETWOX_SMBCMDCMN_ACCESSMASK_APPEND) {
    regiotype = NETWOX_SMBSERFID_REGIOTYPE_APPEND;
  }
  if (regiotype == NETWOX_SMBSERFID_REGIOTYPE_RDWR ||
      regiotype == NETWOX_SMBSERFID_REGIOTYPE_WRITE ||
      regiotype == NETWOX_SMBSERFID_REGIOTYPE_APPEND) {
    if (!psmbser->pcommon->allowput) {
      psmbser->smberrtosend = NETWOX_SMBERR_DEVICEWRITEPROTECTED;
      netwib_goto(NETWIB_ERR_OK);
    }
  }
  /* ignore psmbcmdq->value.ntcreateandx_q.allocationsize, fileattributes,
     shareaccess */
  /* check createfunction */
  ifexists_replace = NETWIB_FALSE;
  ifexists_open = NETWIB_FALSE;
  ifexists_error = NETWIB_FALSE;
  ifnoexists_create = NETWIB_FALSE;
  ifnoexists_error = NETWIB_FALSE;
  switch(psmbcmdq->value.ntcreateandx_q.createfunction) {
  case NETWOX_SMBCMDCMN_CREATEFUNCTION_SUPERSEDE :
    ifexists_replace = NETWIB_TRUE;
    ifnoexists_create = NETWIB_TRUE;
    break;
  case NETWOX_SMBCMDCMN_CREATEFUNCTION_OPEN :
    ifexists_open = NETWIB_TRUE;
    ifnoexists_error = NETWIB_TRUE;
    break;
  case NETWOX_SMBCMDCMN_CREATEFUNCTION_CREATE :
    ifexists_error = NETWIB_TRUE;
    ifnoexists_create = NETWIB_TRUE;
    break;
  case NETWOX_SMBCMDCMN_CREATEFUNCTION_OPENIF :
    ifexists_open = NETWIB_TRUE;
    ifnoexists_create = NETWIB_TRUE;
    break;
  case NETWOX_SMBCMDCMN_CREATEFUNCTION_OVERWRITE :
    ifexists_replace = NETWIB_TRUE;
    ifnoexists_error = NETWIB_TRUE;
    break;
  case NETWOX_SMBCMDCMN_CREATEFUNCTION_OVERWRITEIF :
    ifexists_replace = NETWIB_TRUE;
    ifnoexists_create = NETWIB_TRUE;
    break;
  default :
    netwib_er(netwox_smbser_notimplemented(psmbser));
    psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
    netwib_goto(NETWIB_ERR_OK);
    break;
  }
  /* check createoptions */
  if (psmbcmdq->value.ntcreateandx_q.createoptions & NETWOX_SMBCMDCMN_CREATEOPT_DIRECTORY) {
    fidtype = NETWOX_SMBSERFID_FIDTYPE_DIR;
  }
  if (psmbcmdq->value.ntcreateandx_q.createoptions & NETWOX_SMBCMDCMN_CREATEOPT_NONDIRECTORY) {
    fidtype = NETWOX_SMBSERFID_FIDTYPE_REG;
  }
  deleteonclose = NETWIB_FALSE;
  if (psmbcmdq->value.ntcreateandx_q.createoptions & NETWOX_SMBCMDCMN_CREATEOPT_DELETEONCLOSE) {
    deleteonclose = NETWIB_TRUE;
  }
  if (psmbcmdq->value.ntcreateandx_q.createoptions & (NETWOX_SMBCMDCMN_CREATEOPT_WRITETHROUGH|NETWOX_SMBCMDCMN_CREATEOPT_SEQUENTIALONLY|NETWOX_SMBCMDCMN_CREATEOPT_SYNCIOALERT|NETWOX_SMBCMDCMN_CREATEOPT_SYNCIONONALERT|NETWOX_SMBCMDCMN_CREATEOPT_NOEAKNOWLEDGE|NETWOX_SMBCMDCMN_CREATEOPT_83ONLY|NETWOX_SMBCMDCMN_CREATEOPT_RANDOMACCESS)) {
    /* see CreateFile() function in MSDN ; ignoring them works */
  }
  if (psmbcmdq->value.ntcreateandx_q.createoptions & NETWOX_SMBCMDCMN_CREATEOPT_OPENBYFILEID) {
    /* don't know if I have to implement this */
    netwib_er(netwox_smbser_notimplemented(psmbser));
  }
  /* ignore psmbcmdq->value.ntcreateandx_q.impersonation and secuflags */
  /* obtain filename */
  netwib_eg(netwox_smbcmd_decode_buf_path(psmbmsgq, &psmbcmdq->value.ntcreateandx_q.filename, pfilename));
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Root file id: %{uint32}\n", psmbcmdq->value.ntcreateandx_q.rootfileid));
    netwib_eg(netwib_fmt_display("Filename: %{buf}\n", pfilename));
  }
  netwib_eg(netwox_smbserjail_rootfileid_E(psmbser, prootdir, psmbcmdq->value.ntcreateandx_q.rootfileid, NETWIB_PATHSTAT_TYPE_UNKNOWN, pfilename, prealfilename));

  /* open */
  netwib_eg(netwox_smbsercmd__open_E(psmbser/*psmbser*/,
                                     pitem/*pitem*/,
                                     prealfilename/*prealpathname*/,
                                     deleteonclose/*deleteonclose*/,
                                     fidtype/*fidtype*/,
                                     regiotype/*regiotype*/,
                                     ifexists_replace/*ifexists_replace*/,
                                     ifexists_open/*ifexists_open*/,
                                     ifexists_error/*ifexists_error*/,
                                     ifnoexists_create/*ifnoexists_create*/,
                                     ifnoexists_error/*ifnoexists_error*/,
                                     &createaction/*pcreateaction*/));
  if (psmbser->smberrtosend != NETWOX_SMBERR_OK) {
    netwib_goto(NETWIB_ERR_OK);
  }

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_NTCREATEANDX_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));
  psmbcmdr->value.ntcreateandx_r.fileid = fileid;
  psmbcmdr->value.ntcreateandx_r.oplocklevel = NETWOX_SMBCMDCMN_OPLOCKLEVEL_NONE;
  psmbcmdr->value.ntcreateandx_r.createaction = (NETWOX_SMBCMDCMN_CREATEACTION_OPLOCK & NETWOX_SMBCMDCMN_CREATEACTION_OPLOCK_NO) | createaction;
  ret = netwib_pathstat_init(prealfilename, &pathstat);
  if (ret != NETWIB_ERR_OK) {
    psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
    netwib_goto(NETWIB_ERR_OK);
  }
  netwib_eg(netwox_time1601_init_time(&pathstat.mtime, &psmbcmdr->value.ntcreateandx_r.creationtime));
  netwib_eg(netwox_time1601_init_time(&pathstat.mtime, &psmbcmdr->value.ntcreateandx_r.lastaccesstime));
  netwib_eg(netwox_time1601_init_time(&pathstat.mtime, &psmbcmdr->value.ntcreateandx_r.lastwritetime));
  netwib_eg(netwox_time1601_init_time(&pathstat.mtime, &psmbcmdr->value.ntcreateandx_r.changetime));
  if (!psmbser->pcommon->allowput) {
    psmbcmdr->value.ntcreateandx_r.fileattributes |= NETWOX_SMBCMDCMN_FILEATTR32_READONLY;
  }
  if (pathstat.type == NETWIB_PATHSTAT_TYPE_DIR) {
    psmbcmdr->value.ntcreateandx_r.fileattributes |= NETWOX_SMBCMDCMN_FILEATTR32_DIR;
    netwib__uint64_init_uint32(0, psmbcmdr->value.ntcreateandx_r.allocationsize);
    netwib__uint64_init_uint32(0, psmbcmdr->value.ntcreateandx_r.filesize);
    psmbcmdr->value.ntcreateandx_r.isdirectory = NETWIB_TRUE;
  } else {
    psmbcmdr->value.ntcreateandx_r.allocationsize = pathstat.size64;
    psmbcmdr->value.ntcreateandx_r.filesize = pathstat.size64;
  }
  psmbcmdr->value.ntcreateandx_r.filetype = NETWOX_SMBCMDCMN_FILETYPE_FILEDIR;

  /* end */
  normalend = NETWIB_TRUE;
 netwib_gotolabel:
  if (!normalend) {
    netwib_er(netwox_smbserfid_del(&psmbser->smbserfid, fileid));
  }
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &prealfilename));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pfilename));
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_ntcancel(netwox_smbser *psmbser,
                                      netwox_constsmbmsg *psmbmsgq,
                                      netwox_constsmbcmd *psmbcmdq,
                                      netwox_smbmsg *psmbmsgr,
                                      netwox_smbcmd *psmbcmdr)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_ntcancel\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_NTCANCEL_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }

  /* ntcancel do nothing in our implementation */

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_NTCANCEL_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));

  /* end */
 netwib_gotolabel:
  psmbmsgq = psmbmsgq; /* for compiler warning */
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_ntrename(netwox_smbser *psmbser,
                                      netwox_constsmbmsg *psmbmsgq,
                                      netwox_constsmbcmd *psmbcmdq,
                                      netwox_smbmsg *psmbmsgr,
                                      netwox_smbcmd *psmbcmdr,
                                      netwib_constbuf *prootdir)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_buf *poldname, *pnewname;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbser_ntrename\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &poldname));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pnewname));

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_NTRENAME_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  /* ignore psmbcmdq->value.ntrename_q.informationlevel and clustercount */

  /* obtain names */
  netwib_eg(netwox_smbcmd_decode_buf_path(psmbmsgq, &psmbcmdq->value.ntrename_q.oldname, poldname));
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Old name: %{buf}\n", poldname));
  }
  netwib_eg(netwox_smbcmd_decode_buf_path(psmbmsgq, &psmbcmdq->value.ntrename_q.newname, pnewname));
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("New name: %{buf}\n", pnewname));
  }

  /* rename */
  netwib_eg(netwox_smbsercmd__rename_E(psmbser, prootdir, psmbcmdq->value.rename_q.searchattributes, poldname, pnewname));
  if (psmbser->smberrtosend != NETWOX_SMBERR_OK) {
    netwib_goto(NETWIB_ERR_OK);
  }

  /* send reply */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr,
                                     NETWOX_SMBCMD_TYPE_NTRENAME_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));

  /* end */
 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pnewname));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &poldname));
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}
