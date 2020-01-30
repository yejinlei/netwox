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
netwib_err netwox_pkt_append_dnshdr(netwox_constdnshdr *pdnshdr,
                                    netwib_buf *ppkt)
{
  netwib_data data;
  netwib_uint32 ui;

  netwib_er(netwib_buf_wantspace(ppkt, 12, &data));

  netwib__data_append_uint16(data, pdnshdr->id);
  ui = 0;
  if (pdnshdr->isaresponse) ui |= 0x80;
  ui |= pdnshdr->opcode << 3;
  if (pdnshdr->authanswer) ui |= 0x4;
  if (pdnshdr->truncation) ui |= 0x2;
  if (pdnshdr->recurdesired) ui |= 0x1;
  netwib__data_append_uint8(data, ui);
  ui = 0;
  if (pdnshdr->recuravail) ui |= 0x80;
  ui |= pdnshdr->rcode;
  netwib__data_append_uint8(data, ui);
  netwib__data_append_uint16(data, pdnshdr->count_question);
  netwib__data_append_uint16(data, pdnshdr->count_answer);
  netwib__data_append_uint16(data, pdnshdr->count_authority);
  netwib__data_append_uint16(data, pdnshdr->count_additional);

  ppkt->endoffset += 12;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_decode_dnshdr(netwib_buf *ppkt,
                                    netwox_dnshdr *pdnshdr)
{
  netwib_data data;
  netwib_uint32 datasize, ui;

  datasize = netwib__buf_ref_data_size(ppkt);
  if (datasize < 12) {
    return(NETWIB_ERR_DATAMISSING);
  }
  data = netwib__buf_ref_data_ptr(ppkt);

  netwib__data_decode_uint16(data, pdnshdr->id);
  ui = *data++;
  pdnshdr->isaresponse = (ui & 0x80)?NETWIB_TRUE:NETWIB_FALSE;
  pdnshdr->opcode = (ui >> 3) & 0xF;
  pdnshdr->authanswer = (ui & 0x4)?NETWIB_TRUE:NETWIB_FALSE;
  pdnshdr->truncation = (ui & 0x2)?NETWIB_TRUE:NETWIB_FALSE;
  pdnshdr->recurdesired = (ui & 0x1)?NETWIB_TRUE:NETWIB_FALSE;
  ui = *data++;
  pdnshdr->recuravail = (ui & 0x80)?NETWIB_TRUE:NETWIB_FALSE;
  pdnshdr->rcode = ui & 0xF;
  netwib__data_decode_uint16(data, pdnshdr->count_question);
  netwib__data_decode_uint16(data, pdnshdr->count_answer);
  netwib__data_decode_uint16(data, pdnshdr->count_authority);
  netwib__data_decode_uint16(data, pdnshdr->count_additional);

  ppkt->beginoffset += 12;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_dnsrr(netwib_bufpool *pbufpool,
                                   netwib_constbuf *pfullpkt,
                                   netwox_constdnsrr *pdnsrr,
                                   netwib_buf *ppkt)
{
  netwib_buf *pbuf;
  netwib_data data;
  netwib_uint32 size;

  netwib_er(netwox_dnsfield_host_initcomp(pfullpkt, &pdnsrr->name, ppkt));

  netwib_er(netwib_buf_wantspace(ppkt, 4, &data));
  netwib__data_append_uint16(data, pdnsrr->type);
  netwib__data_append_uint16(data, pdnsrr->rclass);
  ppkt->endoffset += 4;

  if (pdnsrr->kind == NETWOX_DNSRR_KIND_QUESTION) {
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_buf_wantspace(ppkt, 4, &data));
  netwib__data_append_uint32(data, pdnsrr->ttl);
  ppkt->endoffset += 4;

  /* if pfullpkt is NULL, user does not want compression */
  if (pfullpkt == NULL) {
    size = netwib__buf_ref_data_size(&pdnsrr->rdata);
    netwib_er(netwib_buf_wantspace(ppkt, 2, &data));
    netwib__data_append_uint16(data, size);
    ppkt->endoffset += 2;
    netwib_er(netwib_buf_append_buf(&pdnsrr->rdata, ppkt));
  } else {
    netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
    netwib_er(netwox_dnsrdata_decode_pkt(pbufpool, pdnsrr->type, pfullpkt,
                                         &pdnsrr->rdata, pbuf));
    size = netwib__buf_ref_data_size(pbuf);
    netwib_er(netwib_buf_wantspace(ppkt, 2, &data));
    netwib__data_append_uint16(data, size);
    ppkt->endoffset += 2;
    netwib_er(netwib_buf_append_buf(pbuf, ppkt));
    netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_decode_dnsrr(netwib_bufpool *pbufpool,
                                   netwib_constbuf *pfullpkt,
                                   netwib_buf *ppkt,
                                   netwox_dnsrr *pdnsrr)
{
  netwib_buf rdata;
  netwib_data data;
  netwib_uint32 datasize, size;

  netwib_er(netwox_dnsfield_host_uncdecode(pfullpkt, ppkt, &pdnsrr->name));

  datasize = netwib__buf_ref_data_size(ppkt);
  if (datasize < 4) {
    return(NETWIB_ERR_DATAMISSING);
  }
  data = netwib__buf_ref_data_ptr(ppkt);
  netwib__data_decode_uint16(data, pdnsrr->type);
  netwib__data_decode_uint16(data, pdnsrr->rclass);
  datasize -= 4;

  if (pdnsrr->kind == NETWOX_DNSRR_KIND_QUESTION) {
    ppkt->beginoffset += 4;
    return(NETWIB_ERR_OK);
  }

  if (datasize < 6) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwib__data_decode_uint32(data, pdnsrr->ttl);
  netwib__data_decode_uint16(data, size);
  datasize -= 6;

  if (size > datasize) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  ppkt->beginoffset += 10 + size;

  netwib_er(netwib_buf_init_ext_arrayfilled(data, size, &rdata));

  /* if pfullpkt is NULL, user does not want compression */
  if (pfullpkt == NULL) {
    netwib_er(netwib_buf_append_buf(&rdata, &pdnsrr->rdata));
  } else {
    netwib_er(netwox_dnsrdata_init_pkt(pbufpool, pdnsrr->type, pfullpkt,
                                       &rdata, &pdnsrr->rdata));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_pkt_append_dns2(netwib_bufpool *pbufpool,
                                         netwib_ring *prrring,
                                         netwib_constbuf *pfullpkt,
                                         netwox_dnsrr_kind kind,
                                         netwib_buf *ppkt)
{
  netwib_ring_index *pringindex;
  netwib_ptr pitem;
  netwox_dnsrr *pdnsrr;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_ring_index_init(prrring, &pringindex));
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, &pitem);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    pdnsrr = (netwox_dnsrr *)pitem;
    if (pdnsrr->kind != kind) {
      continue;
    }
    ret = netwox_pkt_append_dnsrr(pbufpool, pfullpkt, pdnsrr, ppkt);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_dns(netwib_bufpool *pbufpool,
                                 netwox_constdnshdr *pdnshdr,
                                 netwib_ring *prrring,
                                 netwib_buf *ppkt)
{
  netwox_dnshdr dnsheader;
  netwib_ring_index *pringindex;
  netwib_ptr pitem;
  netwox_dnsrr *pdnsrr;
  netwib_buf fullpkt;
  netwib_uint32 fullpktstartsize;
  netwib_err ret=NETWIB_ERR_OK;

  fullpktstartsize = netwib__buf_ref_data_size(ppkt);

  /* update counts in header */
  dnsheader = *pdnshdr;
  dnsheader.count_question = 0;
  dnsheader.count_answer = 0;
  dnsheader.count_authority = 0;
  dnsheader.count_additional = 0;
  netwib_er(netwib_ring_index_init(prrring, &pringindex));
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, &pitem);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    pdnsrr = (netwox_dnsrr *)pitem;
    switch(pdnsrr->kind) {
    case NETWOX_DNSRR_KIND_QUESTION : dnsheader.count_question++; break;
    case NETWOX_DNSRR_KIND_ANSWER : dnsheader.count_answer++; break;
    case NETWOX_DNSRR_KIND_AUTHORITY : dnsheader.count_authority++; break;
    case NETWOX_DNSRR_KIND_ADDITIONAL : dnsheader.count_additional++; break;
    default : break;
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  netwib_er(ret);

  /* write header */
  netwib_er(netwox_pkt_append_dnshdr(&dnsheader, ppkt));

  /* successively write rrs */
  fullpkt = *ppkt;
  fullpkt.beginoffset += fullpktstartsize;
  netwib_er(netwox_pkt_append_dns2(pbufpool, prrring, &fullpkt,
                                   NETWOX_DNSRR_KIND_QUESTION, ppkt));
  fullpkt = *ppkt;
  fullpkt.beginoffset += fullpktstartsize;
  netwib_er(netwox_pkt_append_dns2(pbufpool, prrring, &fullpkt,
                                   NETWOX_DNSRR_KIND_ANSWER, ppkt));
  fullpkt = *ppkt;
  fullpkt.beginoffset += fullpktstartsize;
  netwib_er(netwox_pkt_append_dns2(pbufpool, prrring, &fullpkt,
                                   NETWOX_DNSRR_KIND_AUTHORITY, ppkt));
  fullpkt = *ppkt;
  fullpkt.beginoffset += fullpktstartsize;
  netwib_er(netwox_pkt_append_dns2(pbufpool, prrring, &fullpkt,
                                   NETWOX_DNSRR_KIND_ADDITIONAL, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_decode_dns(netwib_bufpool *pbufpool,
                                 netwib_constbuf *ppkt,
                                 netwox_dnshdr *pdnshdr,
                                 netwib_ring *prrring)
{
  netwib_ptr pitem;
  netwox_dnsrr *pdnsrr;
  netwib_buf fullpkt, pkt;
  netwib_uint32 i, totalcount, limit2, limit3;
  netwib_err ret=NETWIB_ERR_OK;

  /* decode header */
  fullpkt = *ppkt;
  pkt = *ppkt;
  netwib_er(netwox_pkt_decode_dnshdr(&pkt, pdnshdr));

  /* decode rrs */
  limit2 = pdnshdr->count_question + pdnshdr->count_answer;
  limit3 = limit2 + pdnshdr->count_authority;
  totalcount = limit3 + pdnshdr->count_additional;
  for (i = 0; i < totalcount; i++) {
    netwib_er(netwox_dnsrrring_item_create(&pitem));
    pdnsrr = (netwox_dnsrr *)pitem;
    if (i < pdnshdr->count_question) {
      pdnsrr->kind = NETWOX_DNSRR_KIND_QUESTION;
    } else if (i < limit2) {
      pdnsrr->kind = NETWOX_DNSRR_KIND_ANSWER;
    } else if (i < limit3) {
      pdnsrr->kind = NETWOX_DNSRR_KIND_AUTHORITY;
    } else {
      pdnsrr->kind = NETWOX_DNSRR_KIND_ADDITIONAL;
    }
    ret = netwox_pkt_decode_dnsrr(pbufpool, &fullpkt, &pkt, pdnsrr);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwox_dnsrrring_item_erase(pitem));
      break;
    }
    netwib_er(netwib_ring_add_last(prrring, pitem));
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_dns_question(netwib_bufpool *pbufpool,
                                          netwox_constdnshdr *pdnshdr,
                                          netwib_constbuf *pname,
                                          netwox_dnsrr_type type,
                                          netwox_dnsrr_class rclass,
                                          netwib_buf *ppkt)
{
  netwox_dnshdr dnsheader;
  netwox_dnsrr dnsrr;
  netwib_buf fullpkt;
  netwib_uint32 fullpktstartsize;

  fullpktstartsize = netwib__buf_ref_data_size(ppkt);

  /* update counts in header */
  dnsheader = *pdnshdr;
  dnsheader.count_question = 1;
  dnsheader.count_answer = 0;
  dnsheader.count_authority = 0;
  dnsheader.count_additional = 0;

  /* write header */
  netwib_er(netwox_pkt_append_dnshdr(&dnsheader, ppkt));

  /* write rr */
  fullpkt = *ppkt;
  fullpkt.beginoffset += fullpktstartsize;
  netwib_er(netwox_dnsrr_init(&dnsrr));
  netwib_er(netwox_dnsrr_set_question(&dnsrr, pname, type, rclass));
  netwib_er(netwox_pkt_append_dnsrr(pbufpool, &fullpkt, &dnsrr, ppkt));
  netwib_er(netwox_dnsrr_close(&dnsrr));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_decode_dns_question(netwib_bufpool *pbufpool,
                                          netwib_constbuf *ppkt,
                                          netwox_dnshdr *pdnshdr,
                                          netwib_buf *pname,
                                          netwox_dnsrr_type *ptype,
                                          netwox_dnsrr_class *pclass)
{
  netwox_dnsrr dnsrr;
  netwib_buf fullpkt, pkt;
  netwib_err ret=NETWIB_ERR_OK;

  /* decode header */
  fullpkt = *ppkt;
  pkt = *ppkt;
  netwib_er(netwox_pkt_decode_dnshdr(&pkt, pdnshdr));
  if (pdnshdr->isaresponse) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  /* decode question rr (the first one) */
  if (pdnshdr->count_question == 0) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  netwib_er(netwox_dnsrr_init(&dnsrr));
  dnsrr.kind = NETWOX_DNSRR_KIND_QUESTION;
  ret = netwox_pkt_decode_dnsrr(pbufpool, &fullpkt, &pkt, &dnsrr);
  if (ret == NETWIB_ERR_OK) {
    ret = netwib_buf_append_buf(&dnsrr.name, pname);
    if (ptype != NULL) *ptype = dnsrr.type;
    if (pclass != NULL) *pclass = dnsrr.rclass;
  }
  netwib_er(netwox_dnsrr_close(&dnsrr));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_tcpdns_prepare(netwib_buf *ppkt,
                                            netwib_uint32 *pprevioussize)
{

  *pprevioussize = netwib__buf_ref_data_size(ppkt);

  netwib_er(netwib_buf_wantspace(ppkt, 2, NULL));
  ppkt->endoffset += 2;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_tcpdns_finish(netwib_buf *ppkt,
                                           netwib_uint32 previoussize,
                                           netwib_err errcode)
{
  netwib_data data;
  netwib_uint32 size;

  if (errcode != NETWIB_ERR_OK) {
    ppkt->endoffset = ppkt->beginoffset + previoussize;
    return(errcode);
  }

  size = netwib__buf_ref_data_size(ppkt) - previoussize - 2;
  data = netwib__buf_ref_data_ptr(ppkt) + previoussize;
  netwib__data_decode_uint16(data, size);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_decode_tcpdns(netwib_constbuf *ppkt,
                                    netwib_bufext *pdnspkt,
                                    netwib_uint32 *pskipsize)
{
  netwib_data data;
  netwib_uint32 datasize, dnssize;

  datasize = netwib__buf_ref_data_size(ppkt);
  if (datasize < 2) {
    return(NETWIB_ERR_DATAMISSING);
  }

  data = netwib__buf_ref_data_ptr(ppkt);
  netwib__data_decode_uint16(data, dnssize);
  datasize -= 2;
  if (dnssize > datasize) {
    return(NETWIB_ERR_DATAMISSING);
  }

  if (pskipsize != NULL) *pskipsize = dnssize + 2;

  netwib_er(netwib_buf_init_ext_arrayfilled(data, dnssize, pdnspkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_decode_dnsid(netwib_constbuf *ppkt,
                                   netwib_uint16 *pdnsid)
{
  netwib_data data;

  if (netwib__buf_ref_data_size(ppkt) < 2) {
    return(NETWIB_ERR_DATAMISSING);
  }
  data = netwib__buf_ref_data_ptr(ppkt);

  if (pdnsid != NULL) {
    netwib__data_decode_uint16(data, *pdnsid);
  }

  return(NETWIB_ERR_OK);
}

