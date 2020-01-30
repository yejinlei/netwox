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
netwib_err netwox_pkt_ntp_show(netwib_constbuf *ppkt,
                               netwib_encodetype_context *pctx,
                               netwib_encodetype ntpencodetype,
                               netwib_encodetype dataencodetype,
                               netwib_buf *pbuf)
{
  netwox_ntphdr ntpheader;
  netwib_encodetype_context *pctxtouse, ctx;
  netwib_err ret;

  pctxtouse = pctx;
  if (pctx == NULL) {
    netwib_er(netwib_buf_encode_transition_init(&ctx));
    pctxtouse = &ctx;
  }

  ret = netwox_pkt_decode_ntphdr(ppkt, &ntpheader);
  if (ret == NETWIB_ERR_NOTCONVERTED || ret == NETWIB_ERR_DATAMISSING
      || ret == NETWIB_ERR_LONOTIMPLEMENTED) {
    netwib_er(netwib_buf_encode_transition(pctxtouse, dataencodetype, pbuf));
    netwib_er(netwib_pkt_data_show(ppkt, pctxtouse, dataencodetype, pbuf));
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  switch(ntpencodetype) {
  case NETWIB_ENCODETYPE_SYNTH :
  case NETWIB_ENCODETYPE_ARRAY :
    netwib_er(netwib_buf_encode_transition(pctxtouse, ntpencodetype, pbuf));
    netwib_er(netwox_ntphdr_show(&ntpheader, ntpencodetype, pbuf));
    break;
  default :
    netwib_er(netwib_buf_encode_transition(pctxtouse, ntpencodetype, pbuf));
    netwib_er(netwib_pkt_data_show(ppkt, pctxtouse, ntpencodetype, pbuf));
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_ntp_display(netwib_bufpool *pbufpool,
                                  netwib_constbuf *ppkt,
                                  netwib_encodetype_context *pctx,
                                  netwib_encodetype ntpencodetype,
                                  netwib_encodetype dataencodetype)
{
  netwib_buf *pbuf;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
  ret = netwox_pkt_ntp_show(ppkt, pctx, ntpencodetype, dataencodetype, pbuf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_display(pbuf, NETWIB_ENCODETYPE_DATA));
  }
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(ret);
}
