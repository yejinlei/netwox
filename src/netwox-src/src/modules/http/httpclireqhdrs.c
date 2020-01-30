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
netwib_err netwox_httpclireqhdrs_init(netwox_httpclictx *pctx,
                                      netwox_httphdr_method method,
                                      netwib_constbuf *puri,
                                      netwib_io **ppio)
{
  netwib_io *pio;
  netwox_url_scheme scheme;
  netwib_buf authority, *ppath, query, *purl, *ptmp;
  netwib_buf *pserveruser, *pserverpassword, *pserverhost;
  netwib_ip serverip;
  netwib_port serverport;
  netwib_err ret=NETWIB_ERR_OK;

  /* initialize local variables */
  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &ppath));
  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &purl));
  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &ptmp));
  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pserveruser));
  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pserverpassword));
  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pserverhost));

  /* decode url */
  netwib_eg(netwox_url_decode(puri, &scheme, &authority, ppath, &query,
                              NULL));
  netwib_eg(netwox_url_authority_decode(scheme, &authority, pserveruser,
                                        pserverpassword, pserverhost,
                                        &serverport));

  /* connect to server or proxy */
  if (pctx->useproxy) {
    netwib_eg(netwib_io_init_sock_tcp_cli_easy(&pctx->proxy_ip,
                                               pctx->proxy_port, &pio));
  } else {
    if (scheme != NETWOX_URL_SCHEME_HTTP) {
      netwib_eg(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_eg(netwox_dnscache_ip(&pctx->dnscache, pserverhost, &serverip));
    netwib_eg(netwib_io_init_sock_tcp_cli_easy(&serverip, serverport, &pio));
  }
  netwib_eg(netwib_io_init_data(NETWIB_IO_INIT_DATA_TYPE_LINE,
                                NETWIB_IO_INIT_DATA_TYPE_TRANSPARENT, ppio));
  netwib_eg(netwib_io_plug_rdwr(*ppio, pio));

  /* construct url to use in protocol */
  if (pctx->useproxy) {
    netwib_eg(netwox_url_authority_encode(scheme, NULL, NULL, pserverhost,
                                          serverport, ptmp));
    netwib_eg(netwox_url_encode(scheme, ptmp, ppath, &query, NULL, purl));
  } else {
    netwib_eg(netwox_url_pqf_encode(ppath, &query, NULL, purl));
  }

  /* construct headers */
  netwib__buf_reinit(ptmp);
  netwib_eg(netwox_httphdr_methoduri(method, purl, ptmp));
  netwib_eg(netwox_httphdr_host(pserverhost, serverport, ptmp));
  if (netwib__buf_ref_data_size(pserveruser)
      || netwib__buf_ref_data_size(pserverpassword)) {
    netwib_eg(netwox_httphdr_authbasic(pserveruser, pserverpassword, ptmp));
  }
  if (pctx->useproxy) {
    if (netwib__buf_ref_data_size(&pctx->proxy_login)
        || netwib__buf_ref_data_size(&pctx->proxy_password)) {
      netwib_eg(netwox_httphdr_proxyauthbasic(&pctx->proxy_login,
                                              &pctx->proxy_password, ptmp));
    }
  }
  if (netwib__buf_ref_data_size(&pctx->useragent)) {
    netwib_eg(netwox_httphdr_useragent(&pctx->useragent, ptmp));
  }

  /* send headers */
  netwib_eg(netwib_io_write(*ppio, ptmp));

 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &ppath));
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &purl));
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &ptmp));
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pserveruser));
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pserverpassword));
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pserverhost));
  return(ret);
}
