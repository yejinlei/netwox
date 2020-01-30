/*
                                  NETWIB
                             Network library
                Copyright(c) 1999-2012 Laurent CONSTANTIN
                      http://ntwib.sourceforge.net/
                        laurentconstantin@free.fr
                                  -----
  This file is part of Netwib.

  Netwib is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3
  as published by the Free Software Foundation.

  Netwib is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details (http://www.gnu.org/).

------------------------------------------------------------------------
*/

#include <netwib/inc/maininc.h>

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
static netwib_err netwib_priv_dirname_secure(netwib_conststring dirnamestr,
                                             netwib_bool *pyes)
{
  struct stat st;
  netwib_bool yes;
  int reti;

  reti = lstat(dirnamestr, &st);
  if (reti == -1) {
    if (errno == ENOENT || errno == ENOTDIR) {
      netwib_er(netwib_priv_errmsg_string("file not found: "));
      netwib_er(netwib_priv_errmsg_append_string(dirnamestr));
      return(NETWIB_ERR_NOTFOUND);
    }
    return(NETWIB_ERR_FULSTAT);
  }

  if (S_ISDIR(st.st_mode)) {
    netwib_er(netwib_priv_right_user_trust(st.st_uid, &yes));
    if (!yes) {
      *pyes = NETWIB_FALSE;
      return(NETWIB_ERR_OK);
    }
    if (st.st_mode & S_IWOTH) {
      /* note : do not allow S_ISVTX because of tmp cleaners
         such as tmpwatch which migth remove our files */
      *pyes = NETWIB_FALSE;
      return(NETWIB_ERR_OK);
    }
    if (st.st_mode & S_IWGRP) {
      /* check if we are alone in this group */
      netwib_er(netwib_priv_right_group_trust(st.st_gid, pyes));
      return(NETWIB_ERR_OK);
    }
    *pyes = NETWIB_TRUE;
  } else if (S_ISLNK(st.st_mode)) {
    netwib_er(netwib_priv_right_user_trust(st.st_uid, pyes));
  } else {
    *pyes = NETWIB_FALSE;
  }

  return(NETWIB_ERR_OK);
}
#elif defined NETWIBDEF_SYSNAME_Windows
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_dirname_secure(netwib_constbuf *pdirname,
                                 netwib_bool *pyes)
#if defined NETWIBDEF_SYSNAME_Unix
{
  netwib_byte dirnameabsarray[512], cwdarray[512];
  netwib_buf dirnameabs, cwd;
  netwib_string dn, pc;
  netwib_bool yes;
  netwib_err ret;

  /* init buffers */
  ret = NETWIB_ERR_OK;
  netwib_er(netwib_buf_init_ext_storagearraysizeof(cwdarray, &cwd));
  netwib_er(netwib_buf_init_ext_storagearraysizeof(dirnameabsarray,
                                                   &dirnameabs));

  /* obtain absolute path */
  netwib_eg(netwib_dirname_cwd(&cwd));
  netwib_eg(netwib_path_init_abs(&cwd, pdirname, &dirnameabs));
  netwib_eg(netwib_buf_ref_string(&dirnameabs, &dn));

  pc = dn;
  yes = NETWIB_FALSE;
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    pc = netwib_c_strchr(pc + 1, '/');
    if (pc == NULL) {
      netwib_eg(netwib_priv_dirname_secure(dn, &yes));
      break;
    } else {
      *pc = '\0';
      netwib_eg(netwib_priv_dirname_secure(dn, &yes));
      if (!yes) break;
      *pc = '/';
    }
  }
  if (pyes != NULL) *pyes = yes;

  /* close */
 netwib_gotolabel:
  netwib_er(netwib_buf_close(&cwd));
  netwib_er(netwib_buf_close(&dirnameabs));
  return(ret);
}
#elif defined NETWIBDEF_SYSNAME_Windows
{
  if (pyes != NULL) *pyes = NETWIB_TRUE;
  pdirname = pdirname; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_dirname_cwd(netwib_buf *pdirname)
{
  netwib_byte cwdarray[512];
  netwib_buf cwd;
  netwib_data data;
  netwib_string pc;
  netwib_uint32 pathsize;
  netwib_err ret;

  /* init buffers */
  ret = NETWIB_ERR_OK;
  netwib_er(netwib_buf_init_ext_storagearraysizeof(cwdarray, &cwd));

  /* obtain current directory */
#if NETWIBDEF_HAVEFUNC_PATHCONF==1
  pathsize = pathconf("/", _PC_PATH_MAX);
#elif NETWIBDEF_HAVEVAR_PATH_MAX==1
  pathsize = PATH_MAX;
#else
  pathsize = 255;
#endif
  netwib_eg(netwib_buf_wantspace(&cwd, pathsize, &data));
  while (NETWIB_TRUE) {
    pc = getcwd((netwib_string)data, pathsize);
    if (pc != NULL) {
      break;
    }
    netwib_eg(netwib_buf_wantspace(&cwd, pathsize, &data));
  }
  cwd.endoffset += netwib_c_strlen((netwib_string)data);

  /* canon */
  netwib_eg(netwib_path_canon(&cwd, pdirname));

  /* close */
 netwib_gotolabel:
  netwib_er(netwib_buf_close(&cwd));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_dirname_exists(netwib_constbuf *pdirname,
                                 netwib_bool *pyes)
{
  netwib_pathstat pathstat;
  netwib_bool exists;
  netwib_err ret;

  exists = NETWIB_FALSE;
  ret = netwib_priv_stat_init_pathname(pdirname, &pathstat);
  if (ret == NETWIB_ERR_OK) {
    if (pathstat.type == NETWIB_PATHSTAT_TYPE_DIR) {
      exists = NETWIB_TRUE;
    } else {
      exists = NETWIB_FALSE;
    }
  } else {
    exists = NETWIB_FALSE;
  }

  if (pyes != NULL) {
    *pyes = exists;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_dirname_create(netwib_constbuf *pdirname)
{
  netwib_er(netwib_priv_dir_create_parents(pdirname));
  netwib_er(netwib_priv_dir_create(pdirname));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_dirname_remove(netwib_constbuf *pdirname)
{
  netwib_string dirnamestr;
  int reti;

  netwib__constbuf_ref_string(pdirname, dirnamestr, bufstorage,
                              netwib_dirname_remove(&bufstorage));

  reti = netwib_c_rmdir(dirnamestr);
  if (reti == -1) {
    netwib_er(netwib_priv_errmsg_string("cannot remove this dir: "));
    netwib_er(netwib_priv_errmsg_append_buf(pdirname));
    if (errno == ENOENT) {
      return(NETWIB_ERR_NOTFOUND);
    }
    return(NETWIB_ERR_FURMDIR);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_dirname_rename(netwib_constbuf *polddirname,
                                 netwib_constbuf *pnewdirname)
{
  netwib_string olddirname, newdirname;
  int reti, errnocopy;

  netwib__constbuf_ref_string(polddirname, olddirname, bufstorage,
                              netwib_dirname_rename(&bufstorage, pnewdirname));
  netwib__constbuf_ref_string(pnewdirname, newdirname, bufstorage,
                              netwib_dirname_rename(polddirname, &bufstorage));

  netwib_er(netwib_priv_dir_create_parents(pnewdirname));
  reti = rename(olddirname, newdirname);
  if (reti == -1) {
    errnocopy = errno;
#if defined NETWIBDEF_SYSNAME_Windows
    if (errnocopy == EACCES || errnocopy == EEXIST) {
      /* means newdirname exists : try to delete it */
      reti = netwib_c_rmdir(newdirname);
      if (reti == 0) {
        reti = rename(olddirname, newdirname);
        if (reti == 0) {
          return(NETWIB_ERR_OK);
        }
      }
    }
#endif
    netwib_er(netwib_priv_errmsg_string("cannot rename this dir: "));
    netwib_er(netwib_priv_errmsg_append_buf(polddirname));
    if (errnocopy == ENOENT) {
      return(NETWIB_ERR_NOTFOUND);
    }
    return(NETWIB_ERR_FURENAME);
  }

  return(NETWIB_ERR_OK);
}
