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
netwib_err netwox_httpserreqhdrs_init(netwox_httpserreqhdrs *phdrs)
{
  netwib_er(netwib_buf_init_mallocdefault(&phdrs->uri));

  phdrs->hostset = NETWIB_FALSE;
  netwib_er(netwib_buf_init_ext_storage(&phdrs->hosthost));

  phdrs->authbasicset = NETWIB_FALSE;
  netwib_er(netwib_buf_init_ext_storage(&phdrs->authbasiclogin));
  netwib_er(netwib_buf_init_ext_storage(&phdrs->authbasicpassword));

  phdrs->proxyauthbasicset = NETWIB_FALSE;
  netwib_er(netwib_buf_init_ext_storage(&phdrs->proxyauthbasiclogin));
  netwib_er(netwib_buf_init_ext_storage(&phdrs->proxyauthbasicpassword));

  phdrs->useragentset = NETWIB_FALSE;
  netwib_er(netwib_buf_init_ext_storage(&phdrs->useragent));

  phdrs->contentlengthset = NETWIB_FALSE;

  phdrs->saveallheaders = NETWIB_FALSE;
  netwib_er(netwib_buf_init_ext_storage(&phdrs->allheaders));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_httpserreqhdrs_close(netwox_httpserreqhdrs *phdrs)
{
  netwib_er(netwib_buf_close(&phdrs->uri));
  netwib_er(netwib_buf_close(&phdrs->hosthost));
  netwib_er(netwib_buf_close(&phdrs->authbasiclogin));
  netwib_er(netwib_buf_close(&phdrs->authbasicpassword));
  netwib_er(netwib_buf_close(&phdrs->proxyauthbasiclogin));
  netwib_er(netwib_buf_close(&phdrs->proxyauthbasicpassword));
  netwib_er(netwib_buf_close(&phdrs->useragent));
  netwib_er(netwib_buf_close(&phdrs->allheaders));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_httpreq_decode_line(netwib_bufpool *pbufpool,
                                             netwox_httpserreqhdrs *phdrs,
                                             netwib_constbuf *pline,
                                             netwib_uint32 numcalls)
{
  netwib_buf *pbuf1, *pbuf2, bufext;
  netwib_string method;
  netwib_bool decoded;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf1));

  /* first call must be "method url HTTP/1.0 */
  if (numcalls == 1) {
    ret = netwib_buf_decode_fmt(pline, "%{buf} %{buf} HTTP/%{*;buf}%$",
                                pbuf1, &phdrs->uri);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf1));
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwib_buf_ref_string(pbuf1, &method));
    if (!netwib_c_strcmp(method, "GET")) {
      phdrs->method = NETWOX_HTTPHDR_METHOD_GET;
    } else if (!netwib_c_strcmp(method, "HEAD")) {
      phdrs->method = NETWOX_HTTPHDR_METHOD_HEAD;
    } else if (!netwib_c_strcmp(method, "POST")) {
      phdrs->method = NETWOX_HTTPHDR_METHOD_POST;
    } else if (!netwib_c_strcmp(method, "PUT")) {
      phdrs->method = NETWOX_HTTPHDR_METHOD_PUT;
    } else if (!netwib_c_strcmp(method, "DELETE")) {
      phdrs->method = NETWOX_HTTPHDR_METHOD_DELETE;
    } else if (!netwib_c_strcmp(method, "TRACE")) {
      phdrs->method = NETWOX_HTTPHDR_METHOD_TRACE;
    } else if (!netwib_c_strcmp(method, "OPTIONS")) {
      phdrs->method = NETWOX_HTTPHDR_METHOD_OPTIONS;
    } else {
      netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf1));
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf1));
    return(NETWIB_ERR_OK);
  }

  /* empty line means end of headers */
  if (netwib__buf_ref_data_size(pline) == 0) {
    netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf1));
    return(NETWIB_ERR_DATAEND);
  }

  /* Host */
  if ( ! phdrs->hostset) {
    netwib_er(netwox_txthdr_decode_hostport(pline, "Host", &decoded, &bufext,
                                            &phdrs->hostport));
    if (decoded) {
      netwib_er(netwib_buf_append_buf(&bufext, &phdrs->hosthost));
      netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf1));
      phdrs->hostset = NETWIB_TRUE;
      return(NETWIB_ERR_OK);
    }
  }

  /* Content-Length */
  if ( ! phdrs->contentlengthset) {
    netwib_er(netwox_txthdr_decode_uint32(pline, "Content-Length", &decoded,
                                          &phdrs->contentlength));
    if (decoded) {
      netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf1));
      phdrs->contentlengthset = NETWIB_TRUE;
      return(NETWIB_ERR_OK);
    }
  }
  if ( ! phdrs->contentlengthset) {
    netwib_er(netwox_txthdr_decode_uint32(pline, "Content-length", &decoded,
                                          &phdrs->contentlength));
    if (decoded) {
      netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf1));
      phdrs->contentlengthset = NETWIB_TRUE;
      return(NETWIB_ERR_OK);
    }
  }

  /* User-Agent */
  if ( ! phdrs->useragentset) {
    netwib_er(netwox_txthdr_decode(pline, "User-Agent", &decoded, &bufext));
    if (decoded) {
      netwib_er(netwib_buf_append_buf(&bufext, &phdrs->useragent));
      netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf1));
      phdrs->useragentset = NETWIB_TRUE;
      return(NETWIB_ERR_OK);
    }
  }

  /* now headers needing one more buffer */
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf2));

  /* Authorization */
  if ( ! phdrs->authbasicset) {
    netwib_er(netwox_txthdr_decode_authbasic(pbufpool, pline, "Authorization",
                                             &decoded, pbuf1, pbuf2));
    if (decoded) {
      netwib_er(netwib_buf_append_buf(pbuf1, &phdrs->authbasiclogin));
      netwib_er(netwib_buf_append_buf(pbuf2, &phdrs->authbasicpassword));
      netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf1));
      netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf2));
      phdrs->authbasicset = NETWIB_TRUE;
      return(NETWIB_ERR_OK);
    }
  }

  /* Proxy-Authorization */
  if ( ! phdrs->proxyauthbasicset) {
    netwib_er(netwox_txthdr_decode_authbasic(pbufpool, pline,
                                             "Proxy-Authorization", &decoded,
                                             pbuf1, pbuf2));
    if (decoded) {
      netwib_er(netwib_buf_append_buf(pbuf1, &phdrs->proxyauthbasiclogin));
      netwib_er(netwib_buf_append_buf(pbuf2, &phdrs->proxyauthbasicpassword));
      netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf1));
      netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf2));
      phdrs->proxyauthbasicset = NETWIB_TRUE;
      return(NETWIB_ERR_OK);
    }
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf1));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf2));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_httpserreqhdrs_decode(netwox_httpserctx *pctx,
                                        netwib_io *pio,
                                        netwox_httpserreqhdrs *phdrs)
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
    ret = netwox_httpreq_decode_line(pctx->pbufpool, phdrs, pbuf, numcalls++);
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
