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
static netwib_err netwox_smbsercase_update2(netwib_bufpool *pbufpool,
                                            netwib_buf *prealpathname,
                                            netwib_pathstat_type pathtype,
                                            netwib_bool *pimproved)
{
  netwib_dir *pdir;
  netwib_buf *pparent, *pchild, *ptmp, *pfile;
  netwib_bool exists, diropened;
  netwib_pathstat pathstat;
  netwib_cmp cmp;
  netwib_err ret;

  /* init */
  ret = NETWIB_ERR_OK;
  diropened = NETWIB_FALSE;
  *pimproved = NETWIB_FALSE;
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pparent));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pchild));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &ptmp));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pfile));

  /* obtain both parts */
  netwib_eg(netwib_path_decode_parent(prealpathname, pparent));
  netwib_eg(netwib_path_decode_child(prealpathname, pchild));

  /* ensure parent directory is ok */
  netwib_eg(netwib_dirname_exists(pparent, &exists));
  if (!exists) {
    ret = netwox_smbsercase_update2(pbufpool, pparent, NETWIB_PATHSTAT_TYPE_DIR, pimproved);
    if (*pimproved) {
      /* pparent was improved, so update prealpathname.
         use ptmp to ensure prealpathname is not corrupted on error. */
      netwib_eg(netwib_path_init_concat(pparent, pchild, ptmp));
      netwib__buf_reinit(prealpathname);
      netwib_eg(netwib_buf_append_buf(ptmp, prealpathname));
    }
    if (ret != NETWIB_ERR_OK) {
      /* pparent is not valid. If it is was improved, we've updated, there is
         nothing more to do (*pimproved is already set to true) */
      netwib_goto(ret);
    }
    if (!*pimproved) {
      /* pparent is valid and was not improved : this should not happen */
      netwib_goto(NETWOX_ERR_INTERNALERROR);
    }
    /* now, pparent contains an improved dir, so pparent+child is perhaps
       valid */
    if (pathtype == NETWIB_PATHSTAT_TYPE_DIR) {
      netwib_eg(netwib_dirname_exists(prealpathname, &exists));
    } else if (pathtype == NETWIB_PATHSTAT_TYPE_REG) {
      netwib_eg(netwib_filename_exists(prealpathname, &exists));
    } else {
      netwib_eg(netwib_pathname_exists(prealpathname, &exists));
    }
    if (exists) {
      /* there is no need to continue, because a valid pathname was found.
         *pimproved is already set to true */
      netwib_goto(NETWIB_ERR_OK);
    }
  }

  /* now loop through the directory to find a valid child
     We could use glob, but it should be in netwib to ensure there is no
     portability problem.
  */
  netwib_eg(netwib_dir_init(pparent, &pdir));
  diropened = NETWIB_TRUE;
  while(NETWIB_TRUE) {
    /* find next entry */
    netwib__buf_reinit(pfile);
    ret = netwib_dir_next(pdir, pfile);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* check if matches our name criteria */
    netwib_eg(netwib_buf_casecmp(pchild, pfile, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      continue;
    }
    /* construct the candidate */
    netwib__buf_reinit(ptmp);
    netwib_eg(netwib_path_init_concat(pparent, pfile, ptmp));
    /* check if matches our type criteria */
    if (pathtype != NETWIB_PATHSTAT_TYPE_UNKNOWN) {
      netwib_eg(netwib_pathstat_init(ptmp, &pathstat));
      if (pathstat.type != pathtype) {
        continue;
      }
    }
    /* if we are here, the file is the good one */
    netwib__buf_reinit(prealpathname);
    netwib_eg(netwib_buf_append_buf(ptmp, prealpathname));
    *pimproved = NETWIB_TRUE;
    netwib_goto(NETWIB_ERR_OK);
  }
  /* we did not found the good entry, however prealpathname was perhaps
     improved, so don't change *pimproved */
  ret = NETWIB_ERR_NOTFOUND;

 netwib_gotolabel:
  if (diropened) {
    netwib_er(netwib_dir_close(&pdir));
  }
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pfile));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &ptmp));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pchild));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pparent));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercase_update(netwib_bufpool *pbufpool,
                                    netwib_pathstat_type pathtype,
                                    netwib_buf *prealpathname)
{
  netwib_bool exists, improved;
  netwib_err ret;

  /* first, check if pathname is already ok */
  netwib_er(netwib_pathname_exists(prealpathname, &exists));
  if (exists) {
    return(NETWIB_ERR_OK);
  }

  /* convert */
  ret = netwox_smbsercase_update2(pbufpool, prealpathname, pathtype,
                                  &improved);
  if (ret != NETWIB_ERR_OK) {
    if (improved) {
      /* an error occurred, but prealpathname contains the best found value,
         so ignore error */
      ret = NETWIB_ERR_OK;
    } else {
      /* an error occurred, but prealpathname was not corrupted,
         so ignore error */
      ret = NETWIB_ERR_OK;
    }
  }
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbsercase_test2(netwib_bufpool *pbufpool,
                                          netwib_pathstat_type pathtype,
                                          netwib_conststring ppathname,
                                          netwib_conststring wantedmixed)
{
  netwib_buf *pbuf;
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
  netwib_er(netwib_buf_append_string(ppathname, pbuf));
  netwib_er(netwib_fmt_display(" %{buf}\n", pbuf));
  netwib_er(netwox_smbsercase_update(pbufpool, pathtype, pbuf));
  netwib_er(netwox_buf_check_strmixed(pbuf, wantedmixed));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));
  return(NETWIB_ERR_OK);
}
netwib_err netwox_smbsercase_test(void)
{
  netwib_bufpool *pbufpool;
  netwib_buf pathname;
  netwib_bool exists;

  netwib_er(netwib_bufpool_initdefault(&pbufpool));

  netwib_er(netwib_buf_init_ext_string("/etc/X11", &pathname));
  netwib_er(netwib_dirname_exists(&pathname, &exists));
  if (exists) {
    netwib_er(netwox_smbsercase_test2(pbufpool, NETWIB_PATHSTAT_TYPE_DIR,
                                      "/etc/X11", "'/etc/X11'"));
    netwib_er(netwox_smbsercase_test2(pbufpool, NETWIB_PATHSTAT_TYPE_REG,
                                       "/etc/X11", "'/etc/X11'"));
    netwib_er(netwox_smbsercase_test2(pbufpool, NETWIB_PATHSTAT_TYPE_UNKNOWN,
                                     "/etc/X11", "'/etc/X11'"));
    netwib_er(netwox_smbsercase_test2(pbufpool, NETWIB_PATHSTAT_TYPE_DIR,
                                    "/etc/x11", "'/etc/X11'"));
    netwib_er(netwox_smbsercase_test2(pbufpool, NETWIB_PATHSTAT_TYPE_REG,
                                      "/etc/x11", "'/etc/x11'"));
    netwib_er(netwox_smbsercase_test2(pbufpool, NETWIB_PATHSTAT_TYPE_UNKNOWN,
                                      "/etc/x11", "'/etc/X11'"));
    netwib_er(netwox_smbsercase_test2(pbufpool, NETWIB_PATHSTAT_TYPE_DIR,
                                      "/eTc/x11", "'/etc/X11'"));
    netwib_er(netwox_smbsercase_test2(pbufpool, NETWIB_PATHSTAT_TYPE_REG,
                                      "/eTc/x11", "'/etc/x11'"));
    netwib_er(netwox_smbsercase_test2(pbufpool, NETWIB_PATHSTAT_TYPE_UNKNOWN,
                                      "/eTc/x11", "'/etc/X11'"));
  }

  /* we don't use /etc/host, because under Solaris Saprc, it is a link
     to /etc/inet/hosts */
  netwib_er(netwib_buf_init_ext_string("/etc/passwd", &pathname));
  netwib_er(netwib_filename_exists(&pathname, &exists));
  if (exists) {
    netwib_er(netwox_smbsercase_test2(pbufpool, NETWIB_PATHSTAT_TYPE_DIR,
                                      "/etc/passwd", "'/etc/passwd'"));
    netwib_er(netwox_smbsercase_test2(pbufpool, NETWIB_PATHSTAT_TYPE_REG,
                                      "/etc/passwd", "'/etc/passwd'"));
    netwib_er(netwox_smbsercase_test2(pbufpool, NETWIB_PATHSTAT_TYPE_UNKNOWN,
                                      "/etc/passwd", "'/etc/passwd'"));
    netwib_er(netwox_smbsercase_test2(pbufpool, NETWIB_PATHSTAT_TYPE_DIR,
                                      "/etc/pAsswd", "'/etc/pAsswd'"));
    netwib_er(netwox_smbsercase_test2(pbufpool, NETWIB_PATHSTAT_TYPE_REG,
                                      "/etc/pAsswd", "'/etc/passwd'"));
    netwib_er(netwox_smbsercase_test2(pbufpool, NETWIB_PATHSTAT_TYPE_UNKNOWN,
                                      "/etc/pAsswd", "'/etc/passwd'"));
    netwib_er(netwox_smbsercase_test2(pbufpool, NETWIB_PATHSTAT_TYPE_DIR,
                                      "/eTc/pAsswd", "'/etc/pAsswd'"));
    netwib_er(netwox_smbsercase_test2(pbufpool, NETWIB_PATHSTAT_TYPE_REG,
                                      "/eTc/pAsswd", "'/etc/passwd'"));
    netwib_er(netwox_smbsercase_test2(pbufpool, NETWIB_PATHSTAT_TYPE_UNKNOWN,
                                      "/eTc/pAsswd", "'/etc/passwd'"));
  }

  netwib_er(netwib_buf_init_ext_string("/etc/X11/rgb.txt", &pathname));
  netwib_er(netwib_filename_exists(&pathname, &exists));
  if (exists) {
    netwib_er(netwox_smbsercase_test2(pbufpool, NETWIB_PATHSTAT_TYPE_DIR,
                              "/etc/X11/rgb.txt", "'/etc/X11/rgb.txt'"));
    netwib_er(netwox_smbsercase_test2(pbufpool, NETWIB_PATHSTAT_TYPE_REG,
                              "/etc/X11/rgb.txt", "'/etc/X11/rgb.txt'"));
    netwib_er(netwox_smbsercase_test2(pbufpool, NETWIB_PATHSTAT_TYPE_UNKNOWN,
                              "/etc/X11/rgb.txt", "'/etc/X11/rgb.txt'"));
    netwib_er(netwox_smbsercase_test2(pbufpool, NETWIB_PATHSTAT_TYPE_DIR,
                              "/etc/x11/rGb.txt", "'/etc/X11/rGb.txt'"));
    netwib_er(netwox_smbsercase_test2(pbufpool, NETWIB_PATHSTAT_TYPE_REG,
                              "/etc/x11/rGb.txt", "'/etc/X11/rgb.txt'"));
    netwib_er(netwox_smbsercase_test2(pbufpool, NETWIB_PATHSTAT_TYPE_UNKNOWN,
                              "/etc/x11/rGb.txt", "'/etc/X11/rgb.txt'"));
  }

  netwib_er(netwib_bufpool_close(&pbufpool));
  return(NETWIB_ERR_OK);
}
