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
  NETWIBFILE *pfile;
  netwib_bool closeatend;
} netwib_priv_io_stream;

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_stream_init(NETWIBFILE *pfile,
                                             netwib_bool closefileatend,
                                             netwib_bool *preadinitialized,
                                             netwib_bool *pwriteinitialized,
                                             netwib_priv_io_stream *ptr)
{
  ptr->pfile = pfile;
  ptr->closeatend = closefileatend;
  *preadinitialized = NETWIB_TRUE;
  *pwriteinitialized = NETWIB_TRUE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_stream_close(netwib_priv_io_stream *ptr)
{
  int reti;

  /* system close */
  if (ptr->closeatend) {
    reti = fclose((FILE*)ptr->pfile);
    if (reti == -1) {
      return(NETWIB_ERR_FUFCLOSE);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_stream_read(netwib_io *pio,
                                             netwib_buf *pbuf)
{
  netwib_priv_io_stream *ptr = (netwib_priv_io_stream *)pio->pcommon;
  return(netwib_priv_stream_read(ptr->pfile, pbuf));
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_stream_write(netwib_io *pio,
                                              netwib_constbuf *pbuf)
{
  netwib_priv_io_stream *ptr = (netwib_priv_io_stream *)pio->pcommon;
  return(netwib_priv_stream_write(ptr->pfile, pbuf));
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_stream_wait(netwib_io *pio,
                                             netwib_io_waytype way,
                                             netwib_consttime *pabstime,
                                             netwib_bool *pevent)
{
  netwib_priv_io_stream *ptr = (netwib_priv_io_stream *)pio->pcommon;

  if (way == NETWIB_IO_WAYTYPE_SUPPORTED) {
    return(NETWIB_ERR_PLEASECONSTRUCT);
  } else {
    netwib_er(netwib_priv_stream_wait(ptr->pfile, way, pabstime, pevent));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_stream_ctl_set(netwib_io *pio,
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
static netwib_err netwib_priv_io_stream_ctl_get(netwib_io *pio,
                                                netwib_io_waytype way,
                                                netwib_io_ctltype type,
                                                netwib_ptr p,
                                                netwib_uint32 *pui)
{

  switch(type) {
    case NETWIB_IO_CTLTYPE_RES:
      if (pui != NULL) *pui = NETWIB_IO_RESTYPE_STREAM;
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
static netwib_err netwib_priv_io_stream_fclose(netwib_io *pio)
{
  netwib_priv_io_stream *ptr = (netwib_priv_io_stream *)pio->pcommon;

  netwib_er(netwib_priv_io_stream_close(ptr));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_stream(NETWIBFILE *pfile,
                                 netwib_bool closefileatend,
                                 netwib_io **ppio)
{
  netwib_bool rdinit, wrinit;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_stream), &pcommon));
  ret = netwib_priv_io_stream_init(pfile, closefileatend,
                                   &rdinit, &wrinit,
                                   (netwib_priv_io_stream *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdinit, wrinit,
                           pcommon,
                           &netwib_priv_io_stream_read,
                           &netwib_priv_io_stream_write,
                           &netwib_priv_io_stream_wait,
                           NULL, /* an io_stream is not buffered,
                                    so unread is not supported */
                           &netwib_priv_io_stream_ctl_set,
                           &netwib_priv_io_stream_ctl_get,
                           &netwib_priv_io_stream_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

