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
static netwib_err netwib_priv_io_screen_write(netwib_io *pio,
                                              netwib_constbuf *pbuf)
{
  netwib_er(netwib_buf_display(pbuf, NETWIB_ENCODETYPE_DATA));
  pio = pio; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_screen_wait(netwib_io *pio,
                                             netwib_io_waytype way,
                                             netwib_consttime *pabstime,
                                             netwib_bool *pevent)
{
  /* suppose it's always possible to write to screen
     (note : we do not have to bother with way because
     piocommon->rd.initialized==NETWIB_FALSE) */
  if (pevent != NULL) *pevent = NETWIB_TRUE;

  pio = pio; /* for compiler warning */
  way = way; /* for compiler warning */
  pabstime = pabstime; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_screen_ctl_set(netwib_io *pio,
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

  p = p; /* for compiler warning */
  ui = ui; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_screen_ctl_get(netwib_io *pio,
                                                netwib_io_waytype way,
                                                netwib_io_ctltype type,
                                                netwib_ptr p,
                                                netwib_uint32 *pui)
{
  switch(type) {
    case NETWIB_IO_CTLTYPE_RES:
      if (pui != NULL) *pui = NETWIB_IO_RESTYPE_SCREEN;
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
static netwib_err netwib_priv_io_screen_close(netwib_io *pio)
{
  pio = pio; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_screen(netwib_io **ppio)
{
  netwib_er(netwib_io_init(NETWIB_FALSE, NETWIB_TRUE,
                           NULL,
                           NULL,
                           &netwib_priv_io_screen_write,
                           &netwib_priv_io_screen_wait,
                           NULL,
                           &netwib_priv_io_screen_ctl_set,
                           &netwib_priv_io_screen_ctl_get,
                           &netwib_priv_io_screen_close,
                           ppio));

  return(NETWIB_ERR_OK);
}


