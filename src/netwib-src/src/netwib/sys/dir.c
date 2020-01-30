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
#define NETWIBDEF__USE_READDIR_R 0
#if defined NETWIBDEF_SYSNAME_Unix
 #if NETWIBDEF_HAVEFUNC_READDIR_R == 1
  #if NETWIBDEF_HAVEFUNC_FPATHCONF==1
   #if NETWIBDEF_HAVEFUNC_DIRFD==1
    #undef NETWIBDEF__USE_READDIR_R
    #define NETWIBDEF__USE_READDIR_R 1
   #endif
  #endif
 #endif
#endif

/*-------------------------------------------------------------*/
struct netwib_dir {
 #if defined NETWIBDEF_SYSNAME_Unix
  DIR *pdirstream;
  #if NETWIBDEF__USE_READDIR_R==1
   netwib_ptr ptr;
  #endif
 #elif defined NETWIBDEF_SYSNAME_Windows
  netwib_buf dirnamebuf;
  netwib_bool dirhandlevalid;
  long dirhandle;
 #else
  #error "Unknown value for NETWIBDEF_SYSNAME"
 #endif
};

/*-------------------------------------------------------------*/
netwib_err netwib_dir_init(netwib_constbuf *pdirname,
                           netwib_dir **ppdir)
{
  netwib_string dirnamestr;
  netwib_dir *pdir;

  /* parameter verification */
  if (ppdir == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  netwib__constbuf_ref_string(pdirname, dirnamestr, bufstorage,
                              netwib_dir_init(&bufstorage, ppdir));

  /* allocate needed memory to store pdir */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_dir), (netwib_ptr*)&pdir));

  /* set parameters */
#if defined NETWIBDEF_SYSNAME_Unix
  pdir->pdirstream = opendir(dirnamestr);
  if (pdir->pdirstream == NULL) {
    netwib_er(netwib_ptr_free((netwib_ptr*)&pdir));
    return(NETWIB_ERR_FUOPENDIR);
  }
  #if NETWIBDEF__USE_READDIR_R==1
  {
    int namemax, fd;
    pdir->ptr = NULL; /* on error, ptr will stay NULL */
    fd = dirfd(pdir->pdirstream);
    if (fd != -1) {
      namemax = fpathconf(fd, _PC_NAME_MAX);
      if (namemax != -1) { /* in fact, in this case, -1 means infinite, but
                              I treat it as an error because I don't know
                              which size to allocate */
        netwib_er(netwib_ptr_malloc(sizeof(struct dirent) + namemax + 1,
                                    &pdir->ptr));
      }
    }
  }
  #endif
#elif defined NETWIBDEF_SYSNAME_Windows
  netwib_er(netwib_buf_init_mallocdefault(&pdir->dirnamebuf));
  netwib_er(netwib_buf_append_buf(pdirname, &pdir->dirnamebuf));
  if (dirnamestr[netwib_c_strlen(dirnamestr) - 1] == '/') {
    netwib_er(netwib_buf_append_byte('*', &pdir->dirnamebuf));
  } else {
    netwib_er(netwib_buf_append_string("/*", &pdir->dirnamebuf));
  }
  pdir->dirhandlevalid = NETWIB_FALSE;
  pdir->dirhandle = 0;
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_add_dir(pdir));
#endif

  *ppdir = pdir;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_dir_close(netwib_dir **ppdir)
{
  netwib_dir *pdir;
  int reti;

  /* parameter verification */
  if (ppdir == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  pdir = *ppdir;

  /* close */
#if defined NETWIBDEF_SYSNAME_Unix
  #if NETWIBDEF__USE_READDIR_R==1
  {
    if (pdir->ptr != NULL) {
      netwib_er(netwib_ptr_free(&pdir->ptr));
    }
  }
  #endif
  reti = closedir(pdir->pdirstream);
  if (reti == -1) {
    return(NETWIB_ERR_FUCLOSEDIR);
  }
#elif defined NETWIBDEF_SYSNAME_Windows
  netwib_er(netwib_buf_close(&pdir->dirnamebuf));
  if (pdir->dirhandlevalid) {
    reti = _findclose(pdir->dirhandle);
    if (reti == -1) {
      return(NETWIB_ERR_FUFINDCLOSE);
    }
    pdir->dirhandlevalid = NETWIB_FALSE;
  }
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  /* free memory */
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_del_dir(pdir));
#endif
  netwib_er(netwib_ptr_free((netwib_ptr*)&pdir));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_dir_ctl_set(netwib_dir *pdir,
                              netwib_dir_ctltype type,
                              netwib_ptr p,
                              netwib_uint32 ui)
{

  /* simple cases */
  if (pdir == NULL) {
    /* we have no sufficient info, but we suppose it will be ok if
       a correct parameter is given*/
    return(NETWIB_ERR_OK);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_dir(pdir));
#endif

  switch(type) {
    case NETWIB_DIR_CTLTYPE_REWIND :
#if defined NETWIBDEF_SYSNAME_Unix
      rewinddir(pdir->pdirstream);
#elif defined NETWIBDEF_SYSNAME_Windows
      {
        int reti;

        if (pdir->dirhandlevalid) {
          reti = _findclose(pdir->dirhandle);
          if (reti == -1) {
            return(NETWIB_ERR_FUFINDCLOSE);
          }
          pdir->dirhandlevalid = NETWIB_FALSE;
        }
      }
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif
      return(NETWIB_ERR_OK);
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
      /* perhaps used in those cases */
      p = p;
      ui = ui;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_dir_ctl_get(netwib_dir *pdir,
                              netwib_dir_ctltype type,
                              netwib_ptr p,
                              netwib_uint32 *pui)
{

  /* simple cases */
  if (pdir == NULL) {
    /* it's not possible to obtain info if this pointer is NULL */
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_dir(pdir));
#endif

  switch(type) {
    case NETWIB_DIR_CTLTYPE_REWIND :
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
      /* perhaps used in those cases */
      p = p;
      pui = pui;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* those defines are here because I have a script checking if
 * netwib contains MT-unsafe functions. Like this, they are ignored. */
#define netwib_readdir readdir

/*-------------------------------------------------------------*/
#define netwib_rejected_entries(s) (!netwib_c_strcmp(s, ".") || !netwib_c_strcmp(s, ".."))

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
static netwib_err netwib_priv_dir_next(netwib_dir *pdir,
                                       netwib_buf *pbuffilename)
{
  struct dirent *presult;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_priv_glovars_other_wrlock());
  presult = netwib_readdir(pdir->pdirstream);
  if (presult == NULL) {
    ret = NETWIB_ERR_DATAEND;
  } else {
    if (netwib_rejected_entries(presult->d_name)) {
      ret = NETWIB_ERR_DATANOTAVAIL;
    } else {
      ret = netwib_buf_append_string(presult->d_name, pbuffilename);
    }
  }
  netwib_er(netwib_priv_glovars_other_wrunlock());

  return(ret);
}
#elif defined NETWIBDEF_SYSNAME_Windows
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
#if NETWIBDEF__USE_READDIR_R==1
static netwib_err netwib_priv_dir_next_r(netwib_dir *pdir,
                                         netwib_buf *pbuffilename)
{
  struct dirent *pentry;
  int reti;

 #if NETWIBDEF_LIBPTHREADFROMDCE == 1
  {
    pentry = (struct dirent *)pdir->ptr;
    reti = readdir_r(pdir->pdirstream, pentry);
    if (reti) {
      return(NETWIB_ERR_DATAEND);
    }
  }
 #else
  {
    struct dirent *presult;
    pentry = (struct dirent *)pdir->ptr;
    reti = readdir_r(pdir->pdirstream, pentry, &presult);
    if (reti) {
      if (errno == ENOENT) {
        /* Solaris 2.5.1 set errno instead of presult */
        return(NETWIB_ERR_DATAEND);
      } else {
        return(NETWIB_ERR_FUREADDIRR);
      }
    }
    if (presult == NULL) {
      return(NETWIB_ERR_DATAEND);
    }
  }
 #endif

  if (netwib_rejected_entries(pentry->d_name)) {
    return(NETWIB_ERR_DATANOTAVAIL);
  } else {
    netwib_er(netwib_buf_append_string(pentry->d_name, pbuffilename));
  }

  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_dir_next(netwib_dir *pdir,
                           netwib_buf *pbuffilename)
{
  netwib_err ret=NETWIB_ERR_OK;

  /* parameter verification */
  if (pdir == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_dir(pdir));
#endif

  /* obtain next */
#if defined NETWIBDEF_SYSNAME_Unix
  while (NETWIB_TRUE) {
   #if NETWIBDEF__USE_READDIR_R==1
    if (pdir->ptr != NULL) {
      ret = netwib_priv_dir_next_r(pdir, pbuffilename);
    } else {
      ret = netwib_priv_dir_next(pdir, pbuffilename);
    }
   #else
    ret = netwib_priv_dir_next(pdir, pbuffilename);
   #endif
    if (ret != NETWIB_ERR_DATANOTAVAIL) {
      break;
    }
  }
#elif defined NETWIBDEF_SYSNAME_Windows
  {
    netwib_string dirnamestr;
    struct _finddata_t finddata;
    int reti;

    while (NETWIB_TRUE) {
      if (pdir->dirhandlevalid) {
        reti = _findnext(pdir->dirhandle, &finddata);
        if (reti == -1) {
          return(NETWIB_ERR_DATAEND);
        }
      } else {
        netwib_er(netwib_buf_ref_string(&pdir->dirnamebuf, &dirnamestr));
        pdir->dirhandle = _findfirst(dirnamestr, &finddata);
        if (pdir->dirhandle == (long)-1) {
          return(NETWIB_ERR_DATAEND);
        }
        pdir->dirhandlevalid = NETWIB_TRUE;
      }
      if (!netwib_rejected_entries(finddata.name)) {
        netwib_er(netwib_buf_append_string(finddata.name, pbuffilename));
        break;
      }
    }
  }
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  return(ret);
}
