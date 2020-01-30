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
  netwib_bool closeatend;
} netwib_priv_io_fd;

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_fd_init(int fd,
                                         netwib_bool closefdatend,
                                         netwib_bool *preadinitialized,
                                         netwib_bool *pwriteinitialized,
                                         netwib_priv_io_fd *ptr)
{

  ptr->fd = fd;
  ptr->closeatend = closefdatend;
  *preadinitialized = NETWIB_TRUE;
  *pwriteinitialized = NETWIB_TRUE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_fd_close(netwib_priv_io_fd *ptr)
{
  int reti;

  /* system close */
  if (ptr->closeatend) {
    reti = netwib_c_close(ptr->fd);
    if (reti == -1) {
      return(NETWIB_ERR_FUCLOSE);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_fd_read(netwib_io *pio,
                                         netwib_buf *pbuf)
{
  netwib_priv_io_fd *ptr = (netwib_priv_io_fd *)pio->pcommon;

  return(netwib_priv_fd_read(ptr->fd, pbuf));
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_fd_write(netwib_io *pio,
                                          netwib_constbuf *pbuf)
{
  netwib_priv_io_fd *ptr = (netwib_priv_io_fd *)pio->pcommon;

  return(netwib_priv_fd_write(ptr->fd, pbuf));
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_fd_wait(netwib_io *pio,
                                         netwib_io_waytype way,
                                         netwib_consttime *pabstime,
                                         netwib_bool *pevent)
{
#if defined NETWIBDEF_SYSNAME_Unix
  netwib_priv_io_fd *ptr = (netwib_priv_io_fd *)pio->pcommon;

  if (way == NETWIB_IO_WAYTYPE_SUPPORTED) {
    return(NETWIB_ERR_PLEASECONSTRUCT);
  } else {
    netwib_er(netwib_priv_fd_wait(ptr->fd, way, pabstime, pevent));
  }
#elif defined NETWIBDEF_SYSNAME_Windows
  /* suppose it's a filehandle for a disk file, for which there is
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
static netwib_err netwib_priv_io_fd_ctl_set(netwib_io *pio,
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
      netwib_er(netwib_io_ctl_set_support(pio, way, NETWIB_FALSE));
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
static netwib_err netwib_priv_io_fd_ctl_get(netwib_io *pio,
                                            netwib_io_waytype way,
                                            netwib_io_ctltype type,
                                            netwib_ptr p,
                                            netwib_uint32 *pui)
{

  switch(type) {
    case NETWIB_IO_CTLTYPE_RES:
      if (pui != NULL) *pui = NETWIB_IO_RESTYPE_FD;
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
static netwib_err netwib_priv_io_fd_fclose(netwib_io *pio)
{
  netwib_priv_io_fd *ptr = (netwib_priv_io_fd *)pio->pcommon;

  netwib_er(netwib_priv_io_fd_close(ptr));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_fd(int fd,
                             netwib_bool closefdatend,
                             netwib_io **ppio)
{
  netwib_bool rdinit, wrinit;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_fd), &pcommon));
  ret = netwib_priv_io_fd_init(fd, closefdatend, &rdinit, &wrinit,
                               (netwib_priv_io_fd *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdinit, wrinit,
                           pcommon,
                           &netwib_priv_io_fd_read,
                           &netwib_priv_io_fd_write,
                           &netwib_priv_io_fd_wait,
                           NULL, /* an io_fd is not buffered,
                                    so unread is not supported */
                           &netwib_priv_io_fd_ctl_set,
                           &netwib_priv_io_fd_ctl_get,
                           &netwib_priv_io_fd_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}




