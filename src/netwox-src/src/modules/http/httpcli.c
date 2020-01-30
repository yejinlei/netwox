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
netwib_err netwox_httpcli_method(netwox_httphdr_method method,
                                 netwox_httpclictx *pctx,
                                 netwib_constbuf *puri,
                                 netwox_httpcliresphdrs *presphdrs,
                                 netwox_httphdr_statuscode *pstatuscode,
                                 netwib_io *phdrio,
                                 netwib_io *pbodyio)
{
  netwib_io *psockio;
  netwib_buf *pbuf;
  netwox_httpcliresphdrs resphdrs, *pworkresphdrs;
  netwib_err ret;

  /* connect and send basic headers */
  netwib_er(netwox_httpclireqhdrs_init(pctx, method, puri, &psockio));

  /* send other headers */
  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pbuf));
  netwib_er(netwox_httphdr_endheader(pbuf));
  ret = netwib_io_write(psockio, pbuf);
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pbuf));
  if (ret != NETWIB_ERR_OK) {
   netwib_er(netwib_io_close(&psockio));
   return(ret);
  }

  /* read headers */
  pworkresphdrs = presphdrs;
  if (presphdrs == NULL) {
    pworkresphdrs = &resphdrs;
    netwib_er(netwox_httpcliresphdrs_init(&resphdrs));
  }
  if (phdrio != NULL) {
    pworkresphdrs->saveallheaders = NETWIB_TRUE;
  }
  ret = netwox_httpcliresphdrs_decode(pctx, psockio, pworkresphdrs);
  if (ret != NETWIB_ERR_OK) {
    if (presphdrs == NULL) {
      netwib_er(netwox_httpcliresphdrs_close(&resphdrs));
    }
    netwib_er(netwib_io_close(&psockio));
    return(ret);
  }
  if (pstatuscode != NULL) *pstatuscode = pworkresphdrs->statuscode;

  /* write headers */
  if (phdrio != NULL) {
    netwib_er(netwib_io_write(phdrio, &resphdrs.allheaders));
  }
  if (presphdrs == NULL) {
    netwib_er(netwox_httpcliresphdrs_close(&resphdrs));
  }

  /* read body */
  if (pbodyio != NULL) {
    ret = netwox_httpbody_read_io(pctx->pbufpool, pctx->timeoutms,
                                  pworkresphdrs->contentlengthset,
                                  pworkresphdrs->contentlength,
                                  psockio, pbodyio);
  }

  /* close */
  netwib_er(netwib_io_close(&psockio));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_httpcli_post(netwox_httpclictx *pctx,
                               netwib_constbuf *puri,
                               netwib_constbuf *pparameters,
                               netwox_httphdr_statuscode *pstatuscode,
                               netwib_io *phdrio,
                               netwib_io *pbodyio)
{
  netwib_io *psockio;
  netwib_buf *pbuf;
  netwox_httpcliresphdrs resphdrs;
  netwib_err ret;

  /* connect and send basic headers */
  netwib_er(netwox_httpclireqhdrs_init(pctx, NETWOX_HTTPHDR_METHOD_POST, puri,
                                       &psockio));

  /* send other headers */
  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pbuf));
  netwib_er(netwox_httphdr_contenttype("application/x-www-form-urlencoded",
                                       pbuf));
  netwib_er(netwox_httphdr_contentlength(netwib__buf_ref_data_sizenull(pparameters), pbuf));
  netwib_er(netwox_httphdr_endheader(pbuf));
  netwib_er(netwib_buf_append_buf(pparameters, pbuf));
  ret = netwib_io_write(psockio, pbuf);
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pbuf));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  /* read headers */
  netwib_er(netwox_httpcliresphdrs_init(&resphdrs));
  if (phdrio != NULL) {
    resphdrs.saveallheaders = NETWIB_TRUE;
  }
  ret = netwox_httpcliresphdrs_decode(pctx, psockio, &resphdrs);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_httpcliresphdrs_close(&resphdrs));
    return(ret);
  }
  if (pstatuscode != NULL) *pstatuscode = resphdrs.statuscode;

  /* write headers */
  if (phdrio != NULL) {
    netwib_er(netwib_io_write(phdrio, &resphdrs.allheaders));
  }
  netwib_er(netwox_httpcliresphdrs_close(&resphdrs));

  /* read body */
  if (pbodyio != NULL) {
    ret = netwox_httpbody_read_io(pctx->pbufpool, pctx->timeoutms,
                                  resphdrs.contentlengthset,
                                  resphdrs.contentlength, psockio, pbodyio);
  }

  /* close */
  netwib_er(netwib_io_close(&psockio));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_httpcli_put(netwox_httpclictx *pctx,
                              netwib_constbuf *puri,
                              netwib_bool contentlengthset,
                              netwib_uint32 contentlength,
                              netwib_io *pbodyio,
                              netwox_httphdr_statuscode *pstatuscode,
                              netwib_io *phdrio)
{
  netwib_io *psockio;
  netwib_buf *pbuf;
  netwox_httpcliresphdrs resphdrs;
  netwib_err ret;

  /* connect and send basic headers */
  netwib_er(netwox_httpclireqhdrs_init(pctx, NETWOX_HTTPHDR_METHOD_PUT, puri,
                                       &psockio));

  /* send other headers */
  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pbuf));
  if (contentlengthset) {
    netwib_er(netwox_httphdr_contentlength(contentlength, pbuf));
  }
  netwib_er(netwox_httphdr_endheader(pbuf));
  ret = netwib_io_write(psockio, pbuf);
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pbuf));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  /* send body */
  netwib_er(netwox_httpbody_write_io_close(pctx->pbufpool, pbodyio, psockio));

  /* read headers */
  netwib_er(netwox_httpcliresphdrs_init(&resphdrs));
  if (phdrio != NULL) {
    resphdrs.saveallheaders = NETWIB_TRUE;
  }
  ret = netwox_httpcliresphdrs_decode(pctx, psockio, &resphdrs);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_httpcliresphdrs_close(&resphdrs));
    return(ret);
  }
  if (pstatuscode != NULL) *pstatuscode = resphdrs.statuscode;

  /* write headers */
  if (phdrio != NULL) {
    netwib_er(netwib_io_write(phdrio, &resphdrs.allheaders));
  }
  netwib_er(netwox_httpcliresphdrs_close(&resphdrs));

  /* close */
  netwib_er(netwib_io_close(&psockio));
  return(ret);
}
