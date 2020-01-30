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
netwib_err netwib_iphdr_initdefault(netwib_iptype iptype,
                                    netwib_iphdr *piphdr)
{
  netwib_uint32 id;

  piphdr->iptype = iptype;
  switch(iptype) {
  case NETWIB_IPTYPE_IP4 :
    piphdr->header.ip4.ihl = 5;
    piphdr->header.ip4.tos = 0;
    piphdr->header.ip4.totlen = 20;
    netwib_er(netwib_uint32_init_rand(0, 0xFFFF, &id));
    piphdr->header.ip4.id = (netwib_uint16)id;
    piphdr->header.ip4.reserved = NETWIB_FALSE;
    piphdr->header.ip4.dontfrag = NETWIB_FALSE;
    piphdr->header.ip4.morefrag = NETWIB_FALSE;
    piphdr->header.ip4.offsetfrag = 0;
    piphdr->ttl = 128;
    piphdr->protocol = NETWIB_IPPROTO_IP;
    piphdr->header.ip4.check = 0;
    netwib_er(netwib_ip_init_ip4(0, &piphdr->src));
    netwib_er(netwib_ip_init_ip4(0, &piphdr->dst));
    netwib_er(netwib_buf_init_ext_empty(&piphdr->header.ip4.opts));
    break;
  case NETWIB_IPTYPE_IP6 :
    piphdr->header.ip6.trafficclass = 0;
    piphdr->header.ip6.flowlabel = 0;
    piphdr->header.ip6.payloadlength = 0;
    piphdr->protocol = NETWIB_IPPROTO_NONE;
    piphdr->ttl = 128;
    netwib_er(netwib_ip_init_ip6_fields(0,0,0,0, &piphdr->src));
    netwib_er(netwib_ip_init_ip6_fields(0,0,0,0, &piphdr->dst));
    netwib_er(netwib_buf_init_ext_empty(&piphdr->header.ip6.exts));
    break;
  default :
    return(NETWIB_ERR_PAIPTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_ip4hdr(netwib_constiphdr *piphdr,
                                           netwib_buf *ppkt)
{
  netwib_data data;
  netwib_uint32 flagoff, ip4optssize;

  netwib_er(netwib_buf_wantspace(ppkt, NETWIB_PRIV_IP4HDR_MINLEN, &data));

  if (piphdr->header.ip4.ihl > 0xF)
    return(NETWIB_ERR_PATOOBIGFORHDR);
  if (piphdr->header.ip4.offsetfrag > 0x1FFF)
    return(NETWIB_ERR_PATOOBIGFORHDR);
  if (piphdr->src.iptype != NETWIB_IPTYPE_IP4)
    return(NETWIB_ERR_PAIPTYPENOT4);
  if (piphdr->dst.iptype != NETWIB_IPTYPE_IP4)
    return(NETWIB_ERR_PAIPTYPENOT4);
  ip4optssize = netwib__buf_ref_data_size(&piphdr->header.ip4.opts);
  if (ip4optssize) {
    if (ip4optssize % 4) {
      return(NETWIB_ERR_PAIP4OPTSNOTX4);
    }
    if (ip4optssize > 40) {
      return(NETWIB_ERR_PAIP4OPTSMAX10);
    }
  }

  flagoff = piphdr->header.ip4.offsetfrag;
  if (piphdr->header.ip4.reserved) flagoff |= 0x8000;
  if (piphdr->header.ip4.dontfrag) flagoff |= 0x4000;
  if (piphdr->header.ip4.morefrag) flagoff |= 0x2000;

  netwib__data_append_uint8(data, (4 << 4) | piphdr->header.ip4.ihl);
  netwib__data_append_uint8(data, piphdr->header.ip4.tos);
  netwib__data_append_uint16(data, piphdr->header.ip4.totlen);
  netwib__data_append_uint16(data, piphdr->header.ip4.id);
  netwib__data_append_uint16(data, flagoff);
  netwib__data_append_uint8(data, piphdr->ttl);
  netwib__data_append_uint8(data, piphdr->protocol);
  netwib__data_append_uint16(data, piphdr->header.ip4.check);
  netwib__data_append_uint32(data, piphdr->src.ipvalue.ip4);
  netwib__data_append_uint32(data, piphdr->dst.ipvalue.ip4);

  ppkt->endoffset += NETWIB_PRIV_IP4HDR_MINLEN;

  if (netwib__buf_ref_data_size(&piphdr->header.ip4.opts)) {
    netwib_er(netwib_buf_append_buf(&piphdr->header.ip4.opts, ppkt));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_ip6hdr(netwib_constiphdr *piphdr,
                                           netwib_buf *ppkt)
{
  netwib_data data;
  netwib_uint32 vertrfl, ip6extsize;

  netwib_er(netwib_buf_wantspace(ppkt, NETWIB_PRIV_IP6HDR_MINLEN, &data));

  if (piphdr->header.ip6.flowlabel > 0xFFFFF)
    return(NETWIB_ERR_PATOOBIGFORHDR);
  if (piphdr->src.iptype != NETWIB_IPTYPE_IP6)
    return(NETWIB_ERR_PAIPTYPENOT6);
  if (piphdr->dst.iptype != NETWIB_IPTYPE_IP6)
    return(NETWIB_ERR_PAIPTYPENOT6);
  ip6extsize = netwib__buf_ref_data_size(&piphdr->header.ip6.exts);
  if (ip6extsize % 4) {
    return(NETWIB_ERR_PAIP6EXTSNOTX4);
  }

  vertrfl = (6<<28) | (piphdr->header.ip6.trafficclass<<20) |
    piphdr->header.ip6.flowlabel;
  netwib__data_append_uint32(data, vertrfl);
  netwib__data_append_uint16(data, piphdr->header.ip6.payloadlength);
  netwib__data_append_uint8(data, piphdr->protocol);
  netwib__data_append_uint8(data, piphdr->ttl);
  netwib_c_memcpy(data, piphdr->src.ipvalue.ip6.b, NETWIB_IP6_LEN);
  data += NETWIB_IP6_LEN;
  netwib_c_memcpy(data, piphdr->dst.ipvalue.ip6.b, NETWIB_IP6_LEN);

  ppkt->endoffset += NETWIB_PRIV_IP6HDR_MINLEN;

  if (ip6extsize) {
    netwib_er(netwib_buf_append_buf(&piphdr->header.ip6.exts, ppkt));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_iphdr(netwib_constiphdr *piphdr,
                                   netwib_buf *ppkt)
{

  switch(piphdr->iptype) {
  case NETWIB_IPTYPE_IP4 :
    netwib_er(netwib_pkt_append_ip4hdr(piphdr, ppkt));
    break;
  case NETWIB_IPTYPE_IP6 :
    netwib_er(netwib_pkt_append_ip6hdr(piphdr, ppkt));
    break;
  default :
    return(NETWIB_ERR_PAIPTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_prepend_iphdr(netwib_constiphdr *piphdr,
                                    netwib_buf *ppkt)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));
  ret = netwib_pkt_append_iphdr(piphdr, &buf);
  if (ret == NETWIB_ERR_OK) {
    ret = netwib_buf_prepend_buf(&buf, ppkt);
  }
  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_ip4hdr(netwib_constbuf *ppkt,
                                           netwib_iphdr *piphdr,
                                           netwib_uint32 *pskipsize)
{
  netwib_data data;
  netwib_uint32 datasize, flagoff;
  netwib_byte versionihl;

  datasize = netwib__buf_ref_data_size(ppkt);
  if (datasize < NETWIB_PRIV_IP4HDR_MINLEN) {
    return(NETWIB_ERR_DATAMISSING);
  }

  data = netwib__buf_ref_data_ptr(ppkt);
  netwib__data_decode_uint8(data, versionihl);
  if ((versionihl>>4) != 4) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  piphdr->header.ip4.ihl = (netwib_uint8)(versionihl & 0xF);
  if (datasize < (netwib_uint32)(4*piphdr->header.ip4.ihl)) {
    return(NETWIB_ERR_DATAMISSING);
  }

  netwib__data_decode_uint8(data, piphdr->header.ip4.tos);
  netwib__data_decode_uint16(data, piphdr->header.ip4.totlen);
  netwib__data_decode_uint16(data, piphdr->header.ip4.id);
  netwib__data_decode_uint16(data, flagoff);
  piphdr->header.ip4.reserved = (flagoff&0x8000)?NETWIB_TRUE:NETWIB_FALSE;
  piphdr->header.ip4.dontfrag = (flagoff&0x4000)?NETWIB_TRUE:NETWIB_FALSE;
  piphdr->header.ip4.morefrag = (flagoff&0x2000)?NETWIB_TRUE:NETWIB_FALSE;
  piphdr->header.ip4.offsetfrag = (netwib_uint16)(flagoff & 0x1FFF);
  netwib__data_decode_uint8(data, piphdr->ttl);
  netwib__data_decode_uint8t(data, piphdr->protocol, netwib_ipproto);
  netwib__data_decode_uint16(data, piphdr->header.ip4.check);
  netwib__data_decode_uint32(data, piphdr->src.ipvalue.ip4);
  piphdr->src.iptype = NETWIB_IPTYPE_IP4;
  netwib__data_decode_uint32(data, piphdr->dst.ipvalue.ip4);
  piphdr->dst.iptype = NETWIB_IPTYPE_IP4;

  if (piphdr->header.ip4.ihl > NETWIB_PRIV_IP4HDR_MINLEN/4) {
    netwib_er(netwib_buf_init_ext_arrayfilled(data,
                     4*piphdr->header.ip4.ihl - NETWIB_PRIV_IP4HDR_MINLEN,
                                              &piphdr->header.ip4.opts));
  } else {
    netwib_er(netwib_buf_init_ext_empty(&piphdr->header.ip4.opts));
  }

  if (pskipsize != NULL) {
    if (piphdr->header.ip4.ihl < NETWIB_PRIV_IP4HDR_MINLEN/4) {
      /* invalid, but more logical */
      *pskipsize = NETWIB_PRIV_IP4HDR_MINLEN;
    } else {
      *pskipsize = 4*piphdr->header.ip4.ihl;
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_ip6hdr(netwib_constbuf *ppkt,
                                           netwib_iphdr *piphdr,
                                           netwib_uint32 *pskipsize)
{
  netwib_data data;
  netwib_uint32 datasize, vertrfl, skipsize;
  netwib_buf pkt;

  datasize = netwib__buf_ref_data_size(ppkt);
  if (datasize < NETWIB_PRIV_IP6HDR_MINLEN) {
    return(NETWIB_ERR_DATAMISSING);
  }

  data = netwib__buf_ref_data_ptr(ppkt);
  netwib__data_decode_uint32(data, vertrfl);
  if ((vertrfl>>28) != 6) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  piphdr->header.ip6.trafficclass = (netwib_uint8)((vertrfl>>20)&0xFF);
  piphdr->header.ip6.flowlabel = vertrfl & 0xFFFFF;
  netwib__data_decode_uint16(data, piphdr->header.ip6.payloadlength);
  netwib__data_decode_uint8t(data, piphdr->protocol, netwib_ipproto);
  netwib__data_decode_uint8(data, piphdr->ttl);
  piphdr->src.iptype = NETWIB_IPTYPE_IP6;
  netwib_c_memcpy(piphdr->src.ipvalue.ip6.b, data, NETWIB_IP6_LEN);
  data += NETWIB_IP6_LEN;
  piphdr->dst.iptype = NETWIB_IPTYPE_IP6;
  netwib_c_memcpy(piphdr->dst.ipvalue.ip6.b, data, NETWIB_IP6_LEN);

  pkt = *ppkt;
  pkt.beginoffset += NETWIB_PRIV_IP6HDR_MINLEN;
  netwib_er(netwib_pkt_decode_ip6exts(piphdr->protocol, &pkt, NULL, NULL,
                                      &skipsize));

  if (skipsize) {
    data = netwib__buf_ref_data_ptr(&pkt);
    netwib_er(netwib_buf_init_ext_arrayfilled(data, skipsize,
                                              &piphdr->header.ip6.exts));
  } else {
    netwib_er(netwib_buf_init_ext_empty(&piphdr->header.ip6.exts));
  }

  if (pskipsize != NULL) *pskipsize = NETWIB_PRIV_IP6HDR_MINLEN + skipsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_iphdr(netwib_constbuf *ppkt,
                                   netwib_iphdr *piphdr,
                                   netwib_uint32 *pskipsize)
{

  netwib_er(netwib_priv_ippkt_decode_iptype(ppkt, &piphdr->iptype));

  switch(piphdr->iptype) {
  case NETWIB_IPTYPE_IP4 :
    netwib_er(netwib_pkt_decode_ip4hdr(ppkt, piphdr, pskipsize));
    break;
  case NETWIB_IPTYPE_IP6 :
    netwib_er(netwib_pkt_decode_ip6hdr(ppkt, piphdr, pskipsize));
    break;
  default :
    return(NETWIB_ERR_NOTCONVERTED);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_iphdr_set_proto(netwib_iphdr *piphdr,
                                  netwib_ipproto ipproto)
{
  netwib_data data;
  netwib_uint32 lastprotoffset;

  switch(piphdr->iptype) {
  case NETWIB_IPTYPE_IP4 :
    piphdr->protocol = ipproto;
    break;
  case NETWIB_IPTYPE_IP6 :
    if (netwib__buf_ref_data_size(&piphdr->header.ip6.exts)) {
      netwib_er(netwib_pkt_decode_ip6exts(piphdr->protocol,
                                          &piphdr->header.ip6.exts,
                                          NULL, &lastprotoffset, NULL));
      data = netwib__buf_ref_data_ptr(&piphdr->header.ip6.exts);
      data[lastprotoffset] = (netwib_byte)ipproto;
    } else {
      piphdr->protocol = ipproto;
    }
    break;
  default :
    return(NETWIB_ERR_NOTCONVERTED);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_iphdr_get_proto(netwib_constiphdr *piphdr,
                                  netwib_ipproto *pipproto)
{
  netwib_data data;
  netwib_uint32 lastprotoffset;

  switch(piphdr->iptype) {
  case NETWIB_IPTYPE_IP4 :
    if (pipproto != NULL) *pipproto = piphdr->protocol;
    break;
  case NETWIB_IPTYPE_IP6 :
    if (netwib__buf_ref_data_size(&piphdr->header.ip6.exts)) {
      netwib_er(netwib_pkt_decode_ip6exts(piphdr->protocol,
                                          &piphdr->header.ip6.exts,
                                          NULL, &lastprotoffset, NULL));
      data = netwib__buf_ref_data_ptr(&piphdr->header.ip6.exts);
      if (pipproto != NULL) *pipproto = data[lastprotoffset];
    } else {
      if (pipproto != NULL) *pipproto = piphdr->protocol;
    }
    break;
  default :
    return(NETWIB_ERR_NOTCONVERTED);
  }

  return(NETWIB_ERR_OK);
}

