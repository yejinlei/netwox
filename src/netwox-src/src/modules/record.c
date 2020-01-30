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
#include "../netwox.h"

/*-------------------------------------------------------------*/
typedef struct {
  netwib_io *pnextio; /* suppose we'll not unplug and replug it */
  netwib_device_dlttype dlttype;
} netwox_priv_io_record_sniff;

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_io_record_sniff_init(netwib_constbuf *pfilename,
                                                 netwib_device_dlttype dlttype,
                                                 netwib_bool *preadinitialized,
                                                 netwib_bool *pwriteinitialized,
                                                 netwox_priv_io_record_sniff *ptr)
{

  ptr->dlttype = dlttype;
  netwib_er(netwib_io_init_record_read(pfilename, &ptr->pnextio));

  *preadinitialized = NETWIB_TRUE;
  *pwriteinitialized = NETWIB_FALSE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_io_record_sniff_close(netwox_priv_io_record_sniff *ptr)
{
  netwib_er(netwib_io_close(&ptr->pnextio));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_io_record_sniff_read(netwib_io *pio,
                                                   netwib_buf *pbuf)
{
  netwox_priv_io_record_sniff *ptr=(netwox_priv_io_record_sniff *)pio->pcommon;

  netwib_er(netwib_io_read(ptr->pnextio, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_io_record_sniff_ctl_set(netwib_io *pio,
                                                      netwib_io_waytype way,
                                                      netwib_io_ctltype type,
                                                      netwib_ptr p,
                                                      netwib_uint32 ui)
{
  switch(type) {
    case NETWIB_IO_CTLTYPE_SNIFF_FILTER:
      /* ignore */
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_IO_CTLTYPE_SNIFF_DLT:
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
    default:
      return(NETWIB_ERR_PLEASETRYNEXT);
  }

  pio = pio; /* for compiler warning */
  way = way; /* for compiler warning */
  p = p; /* for compiler warning */
  ui = ui; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_io_record_sniff_ctl_get(netwib_io *pio,
                                                      netwib_io_waytype way,
                                                      netwib_io_ctltype type,
                                                      netwib_ptr p,
                                                      netwib_uint32 *pui)
{
  netwox_priv_io_record_sniff *ptr=(netwox_priv_io_record_sniff *)pio->pcommon;

  switch(type) {
    case NETWIB_IO_CTLTYPE_SNIFF_FILTER:
      if (p != NULL) {
        netwib_er(netwib_buf_append_string("", (netwib_buf*)&p));
      }
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_IO_CTLTYPE_SNIFF_DLT:
      if (pui != NULL) *pui = ptr->dlttype;
      return(NETWIB_ERR_OK);
    default:
      return(NETWIB_ERR_PLEASETRYNEXT);
  }

  way = way; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_io_record_sniff_fclose(netwib_io *pio)
{
  netwox_priv_io_record_sniff *ptr=(netwox_priv_io_record_sniff *)pio->pcommon;
  netwib_er(netwox_priv_io_record_sniff_close(ptr));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_io_init_record_sniff(netwib_constbuf *pfilename,
                                       netwib_device_dlttype dlttype,
                                       netwib_io **ppio)
{
  netwib_bool rdsup, wrsup;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwox_priv_io_record_sniff), &pcommon));
  ret = netwox_priv_io_record_sniff_init(pfilename, dlttype, &rdsup, &wrsup,
                                 (netwox_priv_io_record_sniff *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdsup, wrsup,
                           pcommon,
                           &netwox_priv_io_record_sniff_read,
                           NULL, NULL, NULL,
                           &netwox_priv_io_record_sniff_ctl_set,
                           &netwox_priv_io_record_sniff_ctl_get,
                           &netwox_priv_io_record_sniff_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}
