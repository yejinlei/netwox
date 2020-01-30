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
typedef struct {
  int fd;
} netwib_priv_io_file;

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_file_init(netwib_constbuf *pfilename,
                                           netwib_file_inittype type,
                                           netwib_bool textmode,
                                           netwib_bool *preadinitialized,
                                           netwib_bool *pwriteinitialized,
                                           netwib_priv_io_file *ptr)
{
  netwib_string filename;
  netwib_uint32 bintextmode;
  int errnocopy;

  netwib__constbuf_ref_string(pfilename, filename, bufstorage,
                              netwib_priv_io_file_init(&bufstorage, type, textmode, preadinitialized, pwriteinitialized, ptr));

#if defined NETWIBDEF_SYSNAME_Unix
  bintextmode = 0;
#elif defined NETWIBDEF_SYSNAME_Windows
  bintextmode = O_BINARY;
  if (textmode) bintextmode = O_TEXT;
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  switch(type) {
    case NETWIB_FILE_INITTYPE_READ:
      ptr->fd = netwib_c_open(filename, O_RDONLY | bintextmode);
      if (ptr->fd == -1) {
        errnocopy = errno;
        netwib_er(netwib_priv_errmsg_string("cannot open this file for read: "));
        netwib_er(netwib_priv_errmsg_append_buf(pfilename));
        if (errnocopy == ENOENT) {
          return(NETWIB_ERR_NOTFOUND);
        }
        return(NETWIB_ERR_FUOPEN);
      }
      *preadinitialized = NETWIB_TRUE;
      *pwriteinitialized = NETWIB_FALSE;
      break;
    case NETWIB_FILE_INITTYPE_WRITE:
      netwib_er(netwib_priv_dir_create_parents(pfilename));
      ptr->fd = netwib_c_open3(filename,
                               O_WRONLY | O_TRUNC | bintextmode | O_CREAT,
                               0600);
      if (ptr->fd == -1) {
        errnocopy = errno;
        netwib_er(netwib_priv_errmsg_string("cannot open this file for write: "));
        netwib_er(netwib_priv_errmsg_append_buf(pfilename));
#if defined NETWIBDEF_SYSNAME_Unix
#elif defined NETWIBDEF_SYSNAME_Windows
#define NETWIB_ERRMSG_WIN240 " (origin of this problem is perhaps Windows' limitation of 240 chars for total pathname length)"
        if (errnocopy == ENOENT) {
          netwib_er(netwib_priv_errmsg_append_string(NETWIB_ERRMSG_WIN240));
        }
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif
        return(NETWIB_ERR_FUOPEN);
      }
      *preadinitialized = NETWIB_FALSE;
      *pwriteinitialized = NETWIB_TRUE;
      break;
    case NETWIB_FILE_INITTYPE_APPEND:
      netwib_er(netwib_priv_dir_create_parents(pfilename));
      ptr->fd = netwib_c_open3(filename,
                               O_WRONLY | O_APPEND | bintextmode | O_CREAT,
                               0600);
      if (ptr->fd == -1) {
        errnocopy = errno;
        netwib_er(netwib_priv_errmsg_string("cannot open this file for appending: "));
        netwib_er(netwib_priv_errmsg_append_buf(pfilename));
#if defined NETWIBDEF_SYSNAME_Unix
#elif defined NETWIBDEF_SYSNAME_Windows
        if (errnocopy == ENOENT) {
          netwib_er(netwib_priv_errmsg_append_string(NETWIB_ERRMSG_WIN240));
        }
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif
        return(NETWIB_ERR_FUOPEN);
      }
      *preadinitialized = NETWIB_FALSE;
      *pwriteinitialized = NETWIB_TRUE;
      break;
    case NETWIB_FILE_INITTYPE_RDWR:
      netwib_er(netwib_priv_dir_create_parents(pfilename));
      ptr->fd = netwib_c_open3(filename,
                               O_RDWR | bintextmode | O_CREAT,
                               0600);
      if (ptr->fd == -1) {
        errnocopy = errno;
        netwib_er(netwib_priv_errmsg_string("cannot open this file for read/write: "));
        netwib_er(netwib_priv_errmsg_append_buf(pfilename));
#if defined NETWIBDEF_SYSNAME_Unix
#elif defined NETWIBDEF_SYSNAME_Windows
        if (errnocopy == ENOENT) {
          netwib_er(netwib_priv_errmsg_append_string(NETWIB_ERRMSG_WIN240));
        }
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif
        return(NETWIB_ERR_FUOPEN);
      }
      *preadinitialized = NETWIB_TRUE;
      *pwriteinitialized = NETWIB_TRUE;
      break;
    default:
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  textmode = textmode; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_file_close(netwib_priv_io_file *ptr)
{
  int reti;

  /* system close */
  reti = netwib_c_close(ptr->fd);
  if (reti == -1) {
    return(NETWIB_ERR_FUCLOSE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_file_read(netwib_io *pio,
                                           netwib_buf *pbuf)
{
  netwib_priv_io_file *ptr = (netwib_priv_io_file *)pio->pcommon;

  netwib_er(netwib_priv_fd_read(ptr->fd, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_file_write(netwib_io *pio,
                                            netwib_constbuf *pbuf)
{
  netwib_priv_io_file *ptr = (netwib_priv_io_file *)pio->pcommon;

  netwib_er(netwib_priv_fd_write(ptr->fd, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_file_wait(netwib_io *pio,
                                           netwib_io_waytype way,
                                           netwib_consttime *pabstime,
                                           netwib_bool *pevent)
{
#if defined NETWIBDEF_SYSNAME_Unix
  netwib_priv_io_file *ptr = (netwib_priv_io_file *)pio->pcommon;

  if (way == NETWIB_IO_WAYTYPE_SUPPORTED) {
    return(NETWIB_ERR_PLEASECONSTRUCT);
  } else {
    netwib_er(netwib_priv_fd_wait(ptr->fd, way, pabstime, pevent));
  }
#elif defined NETWIBDEF_SYSNAME_Windows
  /* it's a filehandle for a disk file, for which there is
     only data, or endoffile. So there is no need to wait. */
  if (pevent != NULL) {
    *pevent = NETWIB_TRUE;
  }
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  pio = pio; /* for compiler warning */
  way = way; /* for compiler warning */
  pabstime = pabstime; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_file_ctl_set(netwib_io *pio,
                                              netwib_io_waytype way,
                                              netwib_io_ctltype type,
                                              netwib_ptr p,
                                              netwib_uint32 ui)
{
  netwib_priv_io_file *ptr = (netwib_priv_io_file *)pio->pcommon;
  netwib_intmax imax;
#if defined NETWIBDEF_SYSNAME_Windows
  __int64 offset = 0;
  BOOL err;
#elif NETWIBDEF_HAVETYPE_OFF_T == 1
  off_t offset=0;
#else
  int offset=0;
#endif
  int origin, reti;

  switch(type) {
  case NETWIB_IO_CTLTYPE_RES:
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  case NETWIB_IO_CTLTYPE_END:
    if (way != NETWIB_IO_WAYTYPE_WRITE) return(NETWIB_ERR_PAINVALIDTYPE);
    netwib_er(netwib_io_ctl_set_support(pio, way, NETWIB_FALSE));
    return(NETWIB_ERR_OK);
    break;
  case NETWIB_IO_CTLTYPE_FILE_SEEK_BEGIN:
  case NETWIB_IO_CTLTYPE_FILE_SEEK_CURRENT:
  case NETWIB_IO_CTLTYPE_FILE_SEEK_END:
    if (p != NULL) {
      netwib__intmax_init_int64(*(netwib_int64*)p, imax);
#if defined NETWIBDEF_SYSNAME_Windows
      offset = (__int64)imax;
#elif NETWIBDEF_HAVETYPE_OFF_T == 1
      offset = (off_t)imax;
#else
      offset = (int)imax;
#endif
    } else {
#if defined NETWIBDEF_SYSNAME_Windows
      offset = (__int64)(netwib_int32)ui;
#elif NETWIBDEF_HAVETYPE_OFF_T == 1
      offset = (off_t)(netwib_int32)ui;
#else
      offset = (int)(netwib_int32)ui;
#endif
    }
    origin = 0;
    switch(type) {
    case NETWIB_IO_CTLTYPE_FILE_SEEK_BEGIN: origin = SEEK_SET; break;
    case NETWIB_IO_CTLTYPE_FILE_SEEK_CURRENT: origin = SEEK_CUR; break;
    case NETWIB_IO_CTLTYPE_FILE_SEEK_END: origin = SEEK_END; break;
    default : break;
    }
#if defined NETWIBDEF_SYSNAME_Unix
    offset = lseek(ptr->fd, offset, origin);
#elif defined NETWIBDEF_SYSNAME_Windows
    offset = _lseeki64(ptr->fd, offset, origin);
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif
    if (offset == -1) return(NETWIB_ERR_FULSEEK);
    return(NETWIB_ERR_OK);
    break;
  case NETWIB_IO_CTLTYPE_FILE_TRUNCATE:
    if (p != NULL) {
      netwib__intmax_init_int64(*(netwib_int64*)p, imax);
#if defined NETWIBDEF_SYSNAME_Windows
      offset = (__int64)imax;
#elif NETWIBDEF_HAVETYPE_OFF_T == 1
      offset = (off_t)imax;
#else
      offset = (int)imax;
#endif
    } else {
#if defined NETWIBDEF_SYSNAME_Windows
      offset = (__int64)(netwib_int32)ui;
#elif NETWIBDEF_HAVETYPE_OFF_T == 1
      offset = (off_t)(netwib_int32)ui;
#else
      offset = (int)(netwib_int32)ui;
#endif
    }
#if defined NETWIBDEF_SYSNAME_Unix
    reti = ftruncate(ptr->fd, offset);
    if (reti == -1) return(NETWIB_ERR_FUFTRUNCATE);
#elif defined NETWIBDEF_SYSNAME_Windows
    if (offset < 0x7FFFFFFF) {
      reti = _chsize(ptr->fd, (long)offset);
      if (reti == -1) return(NETWIB_ERR_FUFTRUNCATE);
    } else {
      offset = _lseeki64(ptr->fd, offset, SEEK_SET);
      if (offset == -1) return(NETWIB_ERR_FULSEEK);
      err = SetEndOfFile((HANDLE)_get_osfhandle(ptr->fd));
      if (!err) return(NETWIB_ERR_FUFTRUNCATE);
    }
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif
    return(NETWIB_ERR_OK);
    break;
  default:
    return(NETWIB_ERR_PLEASETRYNEXT);
  }

  way = way; /* for compiler warning */
  p = p; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_file_ctl_get(netwib_io *pio,
                                              netwib_io_waytype way,
                                              netwib_io_ctltype type,
                                              netwib_ptr p,
                                              netwib_uint32 *pui)
{
  netwib_priv_io_file *ptr = (netwib_priv_io_file *)pio->pcommon;
  netwib_intmax offset;

  switch(type) {
  case NETWIB_IO_CTLTYPE_RES:
    if (pui != NULL) *pui = NETWIB_IO_RESTYPE_FILE;
    return(NETWIB_ERR_OK);
  case NETWIB_IO_CTLTYPE_END:
    return(NETWIB_ERR_PAINVALIDTYPE);
  case NETWIB_IO_CTLTYPE_FILE_TELL:
#if defined NETWIBDEF_SYSNAME_Unix
    offset = lseek(ptr->fd, 0, SEEK_CUR);
#elif defined NETWIBDEF_SYSNAME_Windows
    offset = _lseeki64(ptr->fd, 0, SEEK_CUR);
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif
    if (offset == -1) return(NETWIB_ERR_FULSEEK);
    if (p != NULL) {
      netwib__uint64_init_uintmax((netwib_uintmax)offset, *(netwib_uint64*)p);
    } else {
      if (pui != NULL) *pui = (netwib_uint32)offset;
    }
    return(NETWIB_ERR_OK);
    break;
  default:
    return(NETWIB_ERR_PLEASETRYNEXT);
  }

  pio = pio; /* for compiler warning */
  way = way; /* for compiler warning */
  p = p; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_file_fclose(netwib_io *pio)
{
  netwib_priv_io_file *ptr = (netwib_priv_io_file *)pio->pcommon;

  netwib_er(netwib_priv_io_file_close(ptr));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_file(netwib_constbuf *pfilename,
                               netwib_file_inittype type,
                               netwib_bool textmode,
                               netwib_io **ppio)
{
  netwib_bool rdinit, wrinit;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_file), &pcommon));
  ret = netwib_priv_io_file_init(pfilename, type, textmode,
                                 &rdinit, &wrinit,
                                 (netwib_priv_io_file *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdinit, wrinit,
                           pcommon,
                           &netwib_priv_io_file_read,
                           &netwib_priv_io_file_write,
                           &netwib_priv_io_file_wait,
                           NULL, /* an io_file is not buffered,
                                    so unread is not supported */
                           &netwib_priv_io_file_ctl_set,
                           &netwib_priv_io_file_ctl_get,
                           &netwib_priv_io_file_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_init_filetemp_root(netwib_buf *pfilename)
{

  /* set root directory */
#if defined NETWIBDEF_SYSNAME_Unix
  netwib_er(netwib_buf_append_string("/tmp/netw", pfilename));
#elif defined NETWIBDEF_SYSNAME_Windows
  netwib_buf dirname;
  netwib_data data;
  netwib_err ret;
  int err;

  err = GetTempPath(0, NULL);
  if (err != 0) {
    netwib_er(netwib_buf_init_mallocdefault(&dirname));
    netwib_er(netwib_buf_wantspace(&dirname, err, &data));
    err = GetTempPath(err, (netwib_string)data);
    if (err != 0) {
      dirname.endoffset += err;
      netwib_er(netwib_buf_append_string("/netw", &dirname));
      ret = netwib_path_canon(&dirname, pfilename);
      if (ret != NETWIB_ERR_OK) {
        err = 0;
      }
    }
    netwib_er(netwib_buf_close(&dirname));
  }
  if (err == 0) {
    netwib__buf_reinit(pfilename);
    /* choose ./netw because might not have access else where (ie C:) */
    netwib_er(netwib_buf_append_string("netw", pfilename));
  }
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_filetemp(netwib_bool textmode,
                                   netwib_buf *pfilename,
                                   netwib_io **ppio)
{
  netwib_uint32 bintextmode, endoffset;
  netwib_ptr pcommon;
  netwib_string filenamestr;
  netwib_priv_io_file *pif;
  int fd=0;

  if (netwib__buf_ref_data_size(pfilename) == 0) {
    netwib_er(netwib_priv_io_init_filetemp_root(pfilename));
  } else {
    netwib_er(netwib_priv_dir_create_parents(pfilename));
  }

#if defined NETWIBDEF_SYSNAME_Unix
  bintextmode = 0;
#elif defined NETWIBDEF_SYSNAME_Windows
  bintextmode = O_BINARY;
  if (textmode) bintextmode = O_TEXT;
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  endoffset = pfilename->endoffset;
  while(NETWIB_TRUE) {
    /* generate a rand name */
    pfilename->endoffset = endoffset;
    netwib_er(netwib_buf_append_rand(6, 'a', 'z', pfilename));
    netwib_er(netwib_buf_ref_string(pfilename, &filenamestr));
    /* open it */
    fd = netwib_c_open3(filenamestr,
                        O_RDWR | bintextmode | O_CREAT | O_EXCL, 0600);
    if (fd >= 0) {
      break;
    }
  }

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_file), &pcommon));
  pif = (netwib_priv_io_file*)pcommon;
  pif->fd = fd;
  netwib_er(netwib_io_init(NETWIB_TRUE, NETWIB_TRUE, pcommon,
                           &netwib_priv_io_file_read,
                           &netwib_priv_io_file_write,
                           &netwib_priv_io_file_wait,
                           NULL,
                           &netwib_priv_io_file_ctl_set,
                           &netwib_priv_io_file_ctl_get,
                           &netwib_priv_io_file_fclose,
                           ppio));

  textmode = textmode; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
