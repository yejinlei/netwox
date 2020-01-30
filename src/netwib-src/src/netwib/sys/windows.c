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

#if defined NETWIBDEF_SYSNAME_Unix
#elif defined NETWIBDEF_SYSNAME_Windows

/*-------------------------------------------------------------*/
typedef struct {
  NETWIBHANDLE h;
  netwib_handletype handletype;
  netwib_bool closeatend;
} netwib_priv_io_handle;

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_handle_init(NETWIBHANDLE h,
                                             netwib_bool closeatend,
                                             netwib_bool *preadinitialized,
                                             netwib_bool *pwriteinitialized,
                                             netwib_priv_io_handle *ptr)
{

  netwib_er(netwib_priv_handle_type(h, &ptr->handletype));
  ptr->h = h;
  ptr->closeatend = closeatend;
  *preadinitialized = NETWIB_TRUE;
  *pwriteinitialized = NETWIB_TRUE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_handle_close(netwib_priv_io_handle *ptr)
{
  BOOL err;

  /* system close */
  if (ptr->closeatend) {
    err = CloseHandle(ptr->h);
    if (!err) return(NETWIB_ERR_FUCLOSEHANDLE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_handle_read(netwib_io *pio,
                                             netwib_buf *pbuf)
{
  netwib_priv_io_handle *ptr = (netwib_priv_io_handle *)pio->pcommon;
  return(netwib_priv_handle_read(ptr->h, pbuf));
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_handle_write(netwib_io *pio,
                                              netwib_constbuf *pbuf)
{
  netwib_priv_io_handle *ptr = (netwib_priv_io_handle *)pio->pcommon;
  return(netwib_priv_handle_write(ptr->h, pbuf));
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_handle_wait(netwib_io *pio,
                                             netwib_io_waytype way,
                                             netwib_consttime *pabstime,
                                             netwib_bool *pevent)
{
  netwib_priv_io_handle *ptr = (netwib_priv_io_handle *)pio->pcommon;

  if (way == NETWIB_IO_WAYTYPE_SUPPORTED) {
    return(NETWIB_ERR_PLEASECONSTRUCT);
  } else {
    netwib_er(netwib_priv_handle_wait(ptr->h, ptr->handletype, way,
                                      pabstime, pevent));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_handle_ctl_set(netwib_io *pio,
                                                netwib_io_waytype way,
                                                netwib_io_ctltype type,
                                                netwib_ptr p,
                                                netwib_uint32 ui)
{

  switch(type) {
    case NETWIB_IO_CTLTYPE_RES:
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
    case NETWIB_IO_CTLTYPE_END:
      if (way != NETWIB_IO_WAYTYPE_WRITE) return(NETWIB_ERR_PAINVALIDTYPE);
      pio->wr.supported = NETWIB_FALSE;
      return(NETWIB_ERR_OK);
      break;
    default:
      return(NETWIB_ERR_PLEASETRYNEXT);
  }

  way = way; /* for compiler warning */
  p = p; /* for compiler warning */
  ui = ui; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_handle_ctl_get(netwib_io *pio,
                                                netwib_io_waytype way,
                                                netwib_io_ctltype type,
                                                netwib_ptr p,
                                                netwib_uint32 *pui)
{

  switch(type) {
    case NETWIB_IO_CTLTYPE_RES:
      if (pui != NULL) *pui = NETWIB_IO_RESTYPE_HANDLE;
      return(NETWIB_ERR_OK);
    case NETWIB_IO_CTLTYPE_END:
      return(NETWIB_ERR_PAINVALIDTYPE);
    default:
      return(NETWIB_ERR_PLEASETRYNEXT);
  }

  pio = pio; /* for compiler warning */
  way = way; /* for compiler warning */
  p = p; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_handle_fclose(netwib_io *pio)
{
  netwib_priv_io_handle *ptr = (netwib_priv_io_handle *)pio->pcommon;

  netwib_er(netwib_priv_io_handle_close(ptr));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_handle(NETWIBHANDLE h,
                                 netwib_bool closeatend,
                                 netwib_io **ppio)
#if defined NETWIBDEF_SYSNAME_Unix
{
  h = h; /* for compiler warning */
  closeatend = closeatend; /* for compiler warning */
  ppio = ppio; /* for compiler warning */
  return(NETWIB_ERR_LONOTSUPPORTED);
}
#elif defined NETWIBDEF_SYSNAME_Windows
{
  netwib_bool rdinit, wrinit;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_handle), &pcommon));
  ret = netwib_priv_io_handle_init(h, closeatend, &rdinit, &wrinit,
                                   (netwib_priv_io_handle *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdinit, wrinit,
                           pcommon,
                           &netwib_priv_io_handle_read,
                           &netwib_priv_io_handle_write,
                           &netwib_priv_io_handle_wait,
                           NULL, /* an io_handle is not buffered,
                                    so unread is not supported */
                           &netwib_priv_io_handle_ctl_set,
                           &netwib_priv_io_handle_ctl_get,
                           &netwib_priv_io_handle_fclose,
                           (netwib_io **)ppio));

  return(NETWIB_ERR_OK);
}
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_windowstype_init(netwib_windowstype *ptype)
#if defined NETWIBDEF_SYSNAME_Unix
{
  ptype = ptype; /* for compiler warning */
  return(NETWIB_ERR_LONOTSUPPORTED);
}
#elif defined NETWIBDEF_SYSNAME_Windows
{
  OSVERSIONINFO verInfo;
  netwib_windowstype type;

  /* obtain the version */
  verInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  if (!GetVersionEx(&verInfo)) {
    if (ptype != NULL)
      *ptype = NETWIB_WINDOWSTYPE_UNKNOWN;
    return(NETWIB_ERR_OK);
  }

  /* set version */
  type = NETWIB_WINDOWSTYPE_UNKNOWN;

  /* update these fields */
  switch (verInfo.dwPlatformId) {
    case VER_PLATFORM_WIN32s:
      type = NETWIB_WINDOWSTYPE_31;
      break;

    case VER_PLATFORM_WIN32_WINDOWS:
      if (verInfo.dwMajorVersion == 4 && verInfo.dwMinorVersion == 0) {
        type = NETWIB_WINDOWSTYPE_95;
      } else if (verInfo.dwMajorVersion == 4 && verInfo.dwMinorVersion == 10) {
        type = NETWIB_WINDOWSTYPE_98;
      } else if (verInfo.dwMajorVersion == 4 && verInfo.dwMinorVersion == 90) {
        type = NETWIB_WINDOWSTYPE_ME;
      }
      break;

    case VER_PLATFORM_WIN32_NT:
      if (verInfo.dwMajorVersion == 3 && verInfo.dwMinorVersion == 50) {
        type = NETWIB_WINDOWSTYPE_NT350;
      } else if (verInfo.dwMajorVersion == 3 && verInfo.dwMinorVersion == 51) {
        type = NETWIB_WINDOWSTYPE_NT351;
      } else if (verInfo.dwMajorVersion == 4) {
        type = NETWIB_WINDOWSTYPE_NT4;
      } else if (verInfo.dwMajorVersion == 5 && verInfo.dwMinorVersion == 0) {
        type = NETWIB_WINDOWSTYPE_2000;
      } else if (verInfo.dwMajorVersion == 5 && verInfo.dwMinorVersion == 1) {
        type = NETWIB_WINDOWSTYPE_XP;
      } else if (verInfo.dwMajorVersion == 5 && verInfo.dwMinorVersion == 2) {
        type = NETWIB_WINDOWSTYPE_2003;
      } else if (verInfo.dwMajorVersion == 6 && verInfo.dwMinorVersion == 0) {
        type = NETWIB_WINDOWSTYPE_2008;
      } else if (verInfo.dwMajorVersion == 6 && verInfo.dwMinorVersion == 1) {
        type = NETWIB_WINDOWSTYPE_7;
      }
      break;
  }

  if (ptype != NULL)
    *ptype = type;

  return(NETWIB_ERR_OK);
}
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif
