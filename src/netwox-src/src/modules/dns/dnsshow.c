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
netwib_err netwox_dnshdr_show(netwib_bufpool *pbufpool,
                              netwox_constdnshdr *pdnshdr,
                              netwib_encodetype encodetype,
                              netwib_buf *pbuf)
{
  netwib_byte array[80];
  netwib_buf *plocalbuf, bufarray;

  switch(encodetype) {
  case NETWIB_ENCODETYPE_SYNTH :
    netwib_er(netwib_buf_append_fmt(pbuf, "dns%c%{uint16}",
                                    pdnshdr->isaresponse?'A':'Q',
                                    pdnshdr->id));
    break;
  case NETWIB_ENCODETYPE_ARRAY :
    netwib_er(netwib_bufpool_buf_init(pbufpool, &plocalbuf));
    netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &bufarray));
    if (pdnshdr->isaresponse) {
      netwib_er(netwib_show_array_head("DNS_answer", pbuf));
    } else {
      netwib_er(netwib_show_array_head("DNS_question", pbuf));
    }
    netwib_er(netwox_buf_append_dnshdr_rcode(pdnshdr->rcode, &bufarray));
    netwib_er(netwib_buf_append_fmt(plocalbuf,
                                    "id=%{l 5;uint32}  rcode=%{l 13;buf}  ",
                                    pdnshdr->id, &bufarray));
    netwib__buf_reinit(&bufarray);
    netwib_er(netwox_buf_append_dnshdr_opcode(pdnshdr->opcode, &bufarray));
    netwib_er(netwib_buf_append_fmt(plocalbuf, "opcode=%{l 7;buf}",
                                    &bufarray));
    netwib_er(netwib_show_array_fmt32(pbuf, " %{buf}", plocalbuf));
    netwib__buf_reinit(plocalbuf);
    netwib_er(netwib_buf_append_fmt(plocalbuf, "aa=%{bool:0} tr=%{bool:0} rd=%{bool:0} ra=%{bool:0}  ", pdnshdr->authanswer, pdnshdr->truncation, pdnshdr->recurdesired, pdnshdr->recuravail));
    netwib_er(netwib_buf_append_fmt(plocalbuf, "quest=%{l 2;uint16} answer=%{l 2;uint16} auth=%{l 2;uint16} add=%{l 2;uint16}", pdnshdr->count_question, pdnshdr->count_answer, pdnshdr->count_authority, pdnshdr->count_additional));
    netwib_er(netwib_show_array_fmt32(pbuf, " %{buf}", plocalbuf));
    netwib_er(netwib_bufpool_buf_close(pbufpool, &plocalbuf));
    break;
  default :
    return(NETWOX_ERR_NOTIMPLEMENTED);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrr_show(netwib_bufpool *pbufpool,
                             netwox_constdnsrr *pdnsrr,
                             netwib_encodetype encodetype,
                             netwib_buf *pbuf)
{
  netwib_buf *plocalbuf;

  switch(encodetype) {
  case NETWIB_ENCODETYPE_SYNTH :
    if (pdnsrr->kind == NETWOX_DNSRR_KIND_QUESTION) {
      netwib_er(netwib_buf_append_string("_", pbuf));
      netwib_er(netwox_buf_append_dnsrr_type(pdnsrr->type, pbuf));
      netwib_er(netwib_buf_append_string("_", pbuf));
      netwib_er(netwib_buf_append_buf(&pdnsrr->name, pbuf));
    }
    break;
  case NETWIB_ENCODETYPE_ARRAY :
    netwib_er(netwib_bufpool_buf_init(pbufpool, &plocalbuf));
    netwib_er(netwib_buf_append_buf(&pdnsrr->name, plocalbuf));
    netwib_er(netwib_buf_append_string(" ", plocalbuf));
    netwib_er(netwox_buf_append_dnsrr_type(pdnsrr->type, plocalbuf));
    if (pdnsrr->type == NETWOX_DNSRR_TYPE_OPT) {
      /* rfc 2671 */
      netwib_er(netwib_buf_append_fmt(plocalbuf, " UDPpl=%{uint32} errcode=%{uint32} v=%{uint32} ...", pdnsrr->rclass, netwib_c2_uint32_0(pdnsrr->ttl), netwib_c2_uint32_1(pdnsrr->ttl)));
    } else {
      if (pdnsrr->rclass != NETWOX_DNSRR_CLASS_IN ) {
        netwib_er(netwib_buf_append_string(" ", plocalbuf));
        netwib_er(netwox_buf_append_dnsrr_class(pdnsrr->rclass, plocalbuf));
      }
      if (pdnsrr->kind != NETWOX_DNSRR_KIND_QUESTION) {
        netwib_er(netwib_buf_append_fmt(plocalbuf, " %{uint32} ",
                                        pdnsrr->ttl));
        netwib_er(netwox_buf_append_dnsrdata(pbufpool, pdnsrr->type,
                                             &pdnsrr->rdata, plocalbuf));
      }
    }
    netwib_er(netwib_show_array_fmt32(pbuf, " %{buf}", plocalbuf));
    netwib_er(netwib_bufpool_buf_close(pbufpool, &plocalbuf));
    break;
  default :
    return(NETWOX_ERR_NOTIMPLEMENTED);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dns_show(netwib_bufpool *pbufpool,
                           netwox_constdnshdr *pdnshdr,
                           netwib_ring *prrring,
                           netwib_encodetype encodetype,
                           netwib_buf *pbuf)
{
  netwib_ring_index *pringindex;
  netwib_ptr pitem;
  netwox_dnsrr *pdnsrr;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwox_dnshdr_show(pbufpool, pdnshdr, encodetype, pbuf));

  netwib_er(netwib_ring_index_init(prrring, &pringindex));
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, &pitem);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    pdnsrr = (netwox_dnsrr *)pitem;
    ret = netwox_dnsrr_show(pbufpool, pdnsrr, encodetype, pbuf);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));

  if (encodetype == NETWIB_ENCODETYPE_ARRAY) {
    netwib_er(netwib_show_array_tail(pbuf));
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_dns_show(netwib_bufpool *pbufpool,
                               netwib_constbuf *ppkt,
                               netwib_encodetype_context *pctx,
                               netwib_encodetype dnsencodetype,
                               netwib_encodetype dataencodetype,
                               netwib_buf *pbuf)
{
  netwox_dnshdr dnsheader;
  netwib_ring *prrring;
  netwib_buf pkt;
  netwib_encodetype_context *pctxtouse, ctx;
  netwib_err ret;

  pctxtouse = pctx;
  if (pctx == NULL) {
    netwib_er(netwib_buf_encode_transition_init(&ctx));
    pctxtouse = &ctx;
  }

  pkt = *ppkt;
  netwib_er(netwox_dnsrrring_init(&prrring));
  ret = netwox_pkt_decode_dns(pbufpool, &pkt, &dnsheader, prrring);
  if (ret == NETWIB_ERR_NOTCONVERTED || ret == NETWIB_ERR_DATAMISSING
      || ret == NETWIB_ERR_LONOTIMPLEMENTED) {
    netwib_er(netwib_buf_encode_transition(pctxtouse, dataencodetype, pbuf));
    netwib_er(netwib_pkt_data_show(&pkt, pctxtouse, dataencodetype, pbuf));
    netwib_er(netwox_dnsrrring_close(&prrring));
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_dnsrrring_close(&prrring));
    return(ret);
  }

  switch(dnsencodetype) {
  case NETWIB_ENCODETYPE_SYNTH :
  case NETWIB_ENCODETYPE_ARRAY :
    netwib_er(netwib_buf_encode_transition(pctxtouse, dnsencodetype, pbuf));
    netwib_er(netwox_dns_show(pbufpool, &dnsheader, prrring, dnsencodetype,
                              pbuf));
    break;
  default :
    netwib_er(netwib_buf_encode_transition(pctxtouse, dnsencodetype, pbuf));
    netwib_er(netwib_pkt_data_show(&pkt, pctxtouse, dnsencodetype, pbuf));
    break;
  }

  netwib_er(netwox_dnsrrring_close(&prrring));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_dns_display(netwib_bufpool *pbufpool,
                                  netwib_constbuf *ppkt,
                                  netwib_encodetype_context *pctx,
                                  netwib_encodetype hdrencodetype,
                                  netwib_encodetype dataencodetype)
{
  netwib_buf *pbuf;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
  ret = netwox_pkt_dns_show(pbufpool, ppkt, pctx, hdrencodetype,
                            dataencodetype, pbuf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_display(pbuf, NETWIB_ENCODETYPE_DATA));
  }
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(ret);
}
