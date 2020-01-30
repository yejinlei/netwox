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
  netwib_priv_kbd kbd;
} netwib_priv_io_kbd;

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_kbd_read(netwib_io *pio,
                                          netwib_buf *pbuf)
{
  netwib_priv_io_kbd *ptr = (netwib_priv_io_kbd *)pio->pcommon;

  netwib_er(netwib_priv_kbd_read(&ptr->kbd, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_kbd_wait(netwib_io *pio,
                                          netwib_io_waytype way,
                                          netwib_consttime *pabstime,
                                          netwib_bool *pevent)
{
  netwib_priv_io_kbd *ptr = (netwib_priv_io_kbd *)pio->pcommon;

  if (way == NETWIB_IO_WAYTYPE_SUPPORTED) {
    return(NETWIB_ERR_PLEASECONSTRUCT);
  } else if (way == NETWIB_IO_WAYTYPE_WRITE) {
    return(NETWIB_ERR_PAINVALIDTYPE);
  } else {
    netwib_er(netwib_priv_kbd_wait(&ptr->kbd, pabstime, pevent));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_kbd_ctl_set(netwib_io *pio,
                                             netwib_io_waytype way,
                                             netwib_io_ctltype type,
                                             netwib_ptr p,
                                             netwib_uint32 ui)
{
  netwib_priv_io_kbd *ptr = (netwib_priv_io_kbd *)pio->pcommon;

  switch(type) {
    case NETWIB_IO_CTLTYPE_RES:
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
    case NETWIB_IO_CTLTYPE_END:
      if (way != NETWIB_IO_WAYTYPE_WRITE) return(NETWIB_ERR_PAINVALIDTYPE);
      netwib_er(netwib_io_ctl_set_support(pio, way, NETWIB_FALSE));
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_IO_CTLTYPE_KBD_ECHO:
      netwib_er(netwib_priv_kbd_ctl_set_echoline(&ptr->kbd, (netwib_bool)ui,
                                                 ptr->kbd.consolereadbyline));
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_IO_CTLTYPE_KBD_LINE:
      netwib_er(netwib_priv_kbd_ctl_set_echoline(&ptr->kbd,
                                                 ptr->kbd.consoleecho,
                                                 (netwib_bool)ui));
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_IO_CTLTYPE_KBD_PURGE:
      netwib_er(netwib_priv_kbd_ctl_set_purge(&ptr->kbd));
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
static netwib_err netwib_priv_io_kbd_ctl_get(netwib_io *pio,
                                             netwib_io_waytype way,
                                             netwib_io_ctltype type,
                                             netwib_ptr p,
                                             netwib_uint32 *pui)
{
  netwib_priv_io_kbd *ptr = (netwib_priv_io_kbd *)pio->pcommon;

  switch(type) {
    case NETWIB_IO_CTLTYPE_RES:
      if (pui != NULL) *pui = NETWIB_IO_RESTYPE_KBD;
      return(NETWIB_ERR_OK);
    case NETWIB_IO_CTLTYPE_END:
      return(NETWIB_ERR_PAINVALIDTYPE);
    case NETWIB_IO_CTLTYPE_KBD_ECHO:
      if (pui != NULL) *pui = ptr->kbd.consoleecho;
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_IO_CTLTYPE_KBD_LINE:
      if (pui != NULL) *pui = ptr->kbd.consolereadbyline;
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_IO_CTLTYPE_KBD_PURGE:
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
    default:
      return(NETWIB_ERR_PLEASETRYNEXT);
  }

  way = way; /* for compiler warning */
  p = p; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_kbd_fclose(netwib_io *pio)
{
  netwib_priv_io_kbd *ptr = (netwib_priv_io_kbd *)pio->pcommon;

  netwib_er(netwib_priv_kbd_close(&ptr->kbd));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_kbddefault(netwib_io **ppio)
{
  netwib_ptr pcommon;
  netwib_priv_io_kbd *ptr;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_kbd), &pcommon));
  ptr = (netwib_priv_io_kbd *)pcommon;
  ret = netwib_priv_kbd_initdefault(&ptr->kbd);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(NETWIB_TRUE, NETWIB_FALSE,
                           pcommon,
                           &netwib_priv_io_kbd_read,
                           NULL, /* no write */
                           &netwib_priv_io_kbd_wait,
                           NULL, /* an io_kbd is not buffered,
                                    so unread is not supported */
                           &netwib_priv_io_kbd_ctl_set,
                           &netwib_priv_io_kbd_ctl_get,
                           &netwib_priv_io_kbd_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_kbd_fd(int fd,
                                 netwib_io **ppio)
{
  netwib_ptr pcommon;
  netwib_priv_io_kbd *ptr;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_kbd), &pcommon));
  ptr = (netwib_priv_io_kbd *)pcommon;
  ret = netwib_priv_kbd_init_fd(fd, &ptr->kbd);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(NETWIB_TRUE, NETWIB_FALSE,
                           pcommon,
                           &netwib_priv_io_kbd_read,
                           NULL, /* no write */
                           &netwib_priv_io_kbd_wait,
                           NULL, /* an io_kbd is not buffered,
                                    so unread is not supported */
                           &netwib_priv_io_kbd_ctl_set,
                           &netwib_priv_io_kbd_ctl_get,
                           &netwib_priv_io_kbd_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_kbd_handle(NETWIBHANDLE h,
                                     netwib_io **ppio)
{
  netwib_ptr pcommon;
  netwib_priv_io_kbd *ptr;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_kbd), &pcommon));
  ptr = (netwib_priv_io_kbd *)pcommon;
  ret = netwib_priv_kbd_init_handle(h, &ptr->kbd);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(NETWIB_TRUE, NETWIB_FALSE,
                           pcommon,
                           &netwib_priv_io_kbd_read,
                           NULL, /* no write */
                           &netwib_priv_io_kbd_wait,
                           NULL, /* an io_kbd is not buffered,
                                    so unread is not supported */
                           &netwib_priv_io_kbd_ctl_set,
                           &netwib_priv_io_kbd_ctl_get,
                           &netwib_priv_io_kbd_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

