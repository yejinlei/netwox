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
netwib_err netwox_httpcliresphdrs_init(netwox_httpcliresphdrs *phdrs)
{
  netwib_er(netwib_buf_init_mallocdefault(&phdrs->statusmessage));

  phdrs->contentlengthset = NETWIB_FALSE;

  phdrs->locationset = NETWIB_FALSE;
  netwib_er(netwib_buf_init_ext_storage(&phdrs->location));

  phdrs->contenttypeset = NETWIB_FALSE;
  netwib_er(netwib_buf_init_ext_storage(&phdrs->contenttype));

  phdrs->saveallheaders = NETWIB_FALSE;
  netwib_er(netwib_buf_init_ext_storage(&phdrs->allheaders));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_httpcliresphdrs_close(netwox_httpcliresphdrs *phdrs)
{
  netwib_er(netwib_buf_close(&phdrs->statusmessage));
  netwib_er(netwib_buf_close(&phdrs->location));
  netwib_er(netwib_buf_close(&phdrs->contenttype));
  netwib_er(netwib_buf_close(&phdrs->allheaders));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_httpresp_decode_line(netwox_httpcliresphdrs *phdrs,
                                              netwib_constbuf *pline,
                                              netwib_uint32 numcalls)
{
  netwib_buf bufext;
  netwib_bool decoded;
  netwib_err ret;

  /* first call must be "HTTP/1.0 status msg" */
  if (numcalls == 1) {
    ret = netwib_buf_decode_fmt(pline, "HTTP/%{*;buf} %{uint32} %{buf:glob}%$",
                                &phdrs->statuscode, &phdrs->statusmessage);
    if (ret != NETWIB_ERR_OK) {
      /* for example, dmoz.org creates 404 errors without message */
      netwib__buf_reinit(&phdrs->statusmessage);
      ret = netwib_buf_decode_fmt(pline, "HTTP/%{*;buf} %{uint32}%$",
                                  &phdrs->statuscode);
      if (ret != NETWIB_ERR_OK) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
    }
    return(NETWIB_ERR_OK);
  }

  /* empty line means end of headers */
  if (netwib__buf_ref_data_size(pline) == 0) {
    return(NETWIB_ERR_DATAEND);
  }

  /* Content-Length */
  if ( ! phdrs->contentlengthset) {
    netwib_er(netwox_txthdr_decode_uint32(pline, "Content-Length", &decoded,
                                          &phdrs->contentlength));
    if (decoded) {
      phdrs->contentlengthset = NETWIB_TRUE;
      return(NETWIB_ERR_OK);
    }
  }

  /* Location */
  if ( ! phdrs->locationset) {
    netwib_er(netwox_txthdr_decode(pline, "Location", &decoded, &bufext));
    if (decoded) {
      netwib_er(netwib_buf_append_buf(&bufext, &phdrs->location));
      phdrs->locationset = NETWIB_TRUE;
      return(NETWIB_ERR_OK);
    }
  }

  /* Content-Type */
  if ( ! phdrs->contenttypeset) {
    netwib_er(netwox_txthdr_decode(pline, "Content-Type", &decoded, &bufext));
    if (decoded) {
      netwib_er(netwib_buf_append_buf(&bufext, &phdrs->contenttype));
      phdrs->contenttypeset = NETWIB_TRUE;
      return(NETWIB_ERR_OK);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_httpcliresphdrs_decode(netwox_httpclictx *pctx,
                                         netwib_io *pio,
                                         netwox_httpcliresphdrs *phdrs)
{
  netwib_buf *pbuf;
  netwib_bool event;
  netwib_uint32 numcalls;
  netwib_time t;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_plus_msec(&t, pctx->timeoutms));

  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pbuf));
  numcalls = 1;
  while(NETWIB_TRUE) {
    /* read */
    netwib__buf_reinit(pbuf);
    netwib_er(netwib_io_wait_read(pio, &t, &event));
    if (!event) {
      ret = NETWOX_ERR_TIMEOUT;
      break;
    }
    ret = netwib_io_read(pio, pbuf);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* analyze line */
    ret = netwox_httpresp_decode_line(phdrs, pbuf, numcalls++);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    /* eventually save all headers */
    if (phdrs->saveallheaders) {
      netwib_er(netwib_buf_append_buf(pbuf, &phdrs->allheaders));
      netwib_er(netwib_buf_append_string("\r\n", &phdrs->allheaders));
    }
  }
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pbuf));

  return(ret);
}
