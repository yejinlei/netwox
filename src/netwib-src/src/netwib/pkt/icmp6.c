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
netwib_err netwib_buf_append_icmp6type(netwib_icmp6type type,
                                       netwib_buf *pbuf)
{
  netwib_conststring pc = NULL;

  switch(type) {
    case NETWIB_ICMP6TYPE_DSTUNREACH :
      pc = "destination unreachable";
      break;
    case NETWIB_ICMP6TYPE_PKTTOOBIG :
      pc = "packet to big";
      break;
    case NETWIB_ICMP6TYPE_TIMEEXCEED :
      pc = "time exceeded";
      break;
    case NETWIB_ICMP6TYPE_PARAPROB :
      pc = "parameter problem";
      break;
    case NETWIB_ICMP6TYPE_ECHOREQ :
      pc = "echo request";
      break;
    case NETWIB_ICMP6TYPE_ECHOREP :
      pc = "echo reply";
      break;
    case NETWIB_ICMP6TYPE_ROUTERSOLICIT :
      pc = "router solicitation";
      break;
    case NETWIB_ICMP6TYPE_ROUTERADVERT :
      pc = "router advertisement";
      break;
    case NETWIB_ICMP6TYPE_NEIGHBORSOLICIT :
      pc = "neighbor solicitation";
      break;
    case NETWIB_ICMP6TYPE_NEIGHBORADVERT :
      pc = "neighbor advertisement";
      break;
    case NETWIB_ICMP6TYPE_REDIRECT :
      pc = "redirect";
      break;
    default :
      pc = "unknown";
      break;
  }

  netwib_er(netwib_buf_append_string(pc, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_icmp6code(netwib_icmp6type type,
                                       netwib_icmp6code code,
                                       netwib_buf *pbuf)
{
  netwib_conststring pc = NULL;

  switch(type) {
  case NETWIB_ICMP6TYPE_DSTUNREACH :
    switch(code) {
    case NETWIB_ICMP6CODE_DSTUNREACH_NOROUTE :
      pc = "no route";
      break;
    case NETWIB_ICMP6CODE_DSTUNREACH_PROHI :
      pc = "prohibited";
      break;
    case NETWIB_ICMP6CODE_DSTUNREACH_HOST :
      pc = "host";
      break;
    case NETWIB_ICMP6CODE_DSTUNREACH_PORT :
      pc = "port";
      break;
    default :
      pc = "unknown";
      break;
    }
    break;
  case NETWIB_ICMP6TYPE_PKTTOOBIG :
    switch(code) {
    case NETWIB_ICMP6CODE_PKTTOOBIG :
      pc = "";
      break;
    default :
      pc = "unknown";
      break;
    }
    break;
  case NETWIB_ICMP6TYPE_TIMEEXCEED :
    switch(code) {
    case NETWIB_ICMP6CODE_TIMEEXCEED_TTL :
      pc = "ttl";
      break;
    case NETWIB_ICMP6CODE_TIMEEXCEED_FRAG :
      pc = "fragment reassembly";
      break;
    default :
      pc = "unknown";
      break;
    }
    break;
  case NETWIB_ICMP6TYPE_PARAPROB :
    switch(code) {
    case NETWIB_ICMP6CODE_PARAPROB_HDR :
      pc = "header";
      break;
    case NETWIB_ICMP6CODE_PARAPROB_NEXTHDR :
      pc = "unrecognized Next Header";
      break;
    case NETWIB_ICMP6CODE_PARAPROB_OPTION :
      pc = "unrecognized IPv6 option";
      break;
    default :
      pc = "unknown";
      break;
    }
    break;
  case NETWIB_ICMP6TYPE_ECHOREQ :
    switch(code) {
    case NETWIB_ICMP6CODE_ECHOREQ :
      pc = "";
      break;
    default :
      pc = "unknown";
      break;
    }
    break;
  case NETWIB_ICMP6TYPE_ECHOREP :
    switch(code) {
    case NETWIB_ICMP6CODE_ECHOREP :
      pc = "";
      break;
    default :
      pc = "unknown";
      break;
    }
    break;
  case NETWIB_ICMP6TYPE_ROUTERSOLICIT :
    switch(code) {
    case NETWIB_ICMP6CODE_ROUTERSOLICIT :
      pc = "";
      break;
    default :
      pc = "unknown";
      break;
    }
    break;
  case NETWIB_ICMP6TYPE_ROUTERADVERT :
    switch(code) {
    case NETWIB_ICMP6CODE_ROUTERADVERT :
      pc = "";
      break;
    default :
      pc = "unknown";
      break;
    }
    break;
  case NETWIB_ICMP6TYPE_NEIGHBORSOLICIT :
    switch(code) {
    case NETWIB_ICMP6CODE_NEIGHBORSOLICIT :
      pc = "";
      break;
    default :
      pc = "unknown";
      break;
    }
    break;
  case NETWIB_ICMP6TYPE_NEIGHBORADVERT :
    switch(code) {
    case NETWIB_ICMP6CODE_NEIGHBORADVERT :
      pc = "";
      break;
    default :
      pc = "unknown";
      break;
    }
    break;
  case NETWIB_ICMP6TYPE_REDIRECT :
    switch(code) {
    case NETWIB_ICMP6CODE_REDIRECT :
      pc = "";
      break;
    default :
      pc = "unknown";
      break;
    }
    break;
  default :
    pc = "unknown";
    break;
  }

  netwib_er(netwib_buf_append_string(pc, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_icmp6_size(netwib_consticmp6 *picmp6,
                                  netwib_uint32 *psize)
{
  netwib_uint32 size=0;

  switch(picmp6->type) {
  case NETWIB_ICMP6TYPE_DSTUNREACH :
    size = 8 + netwib__buf_ref_data_size(&picmp6->msg.dstunreach.badippacket);
    break;
  case NETWIB_ICMP6TYPE_PKTTOOBIG :
    size = 8 + netwib__buf_ref_data_size(&picmp6->msg.pkttoobig.badippacket);
    break;
  case NETWIB_ICMP6TYPE_TIMEEXCEED :
    size = 8 + netwib__buf_ref_data_size(&picmp6->msg.timeexceed.badippacket);
    break;
  case NETWIB_ICMP6TYPE_PARAPROB :
    size = 8 + netwib__buf_ref_data_size(&picmp6->msg.paraprob.badippacket);
    break;
  case NETWIB_ICMP6TYPE_ECHOREQ :
  case NETWIB_ICMP6TYPE_ECHOREP :
    size = 8 + netwib__buf_ref_data_size(&picmp6->msg.echo.data);
    break;
  case NETWIB_ICMP6TYPE_ROUTERSOLICIT :
    size = 8 + netwib__buf_ref_data_size(&picmp6->msg.routersolicit.options);
    break;
  case NETWIB_ICMP6TYPE_ROUTERADVERT :
    size = 16 + netwib__buf_ref_data_size(&picmp6->msg.routeradvert.options);
    break;
  case NETWIB_ICMP6TYPE_NEIGHBORSOLICIT :
    size = 24 +netwib__buf_ref_data_size(&picmp6->msg.neighborsolicit.options);
    break;
  case NETWIB_ICMP6TYPE_NEIGHBORADVERT :
    size = 24 + netwib__buf_ref_data_size(&picmp6->msg.neighboradvert.options);
    break;
  case NETWIB_ICMP6TYPE_REDIRECT :
    size = 40 + netwib__buf_ref_data_size(&picmp6->msg.redirect.options);
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  }

  if (psize != NULL) *psize = size;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_icmp6_initdefault(netwib_icmp6type type,
                                    netwib_icmp6 *picmp6)
{
  netwib_uint32 ui;

  picmp6->type = type;
  picmp6->check = 0;
  switch(type) {
  case NETWIB_ICMP6TYPE_DSTUNREACH :
    picmp6->code = NETWIB_ICMP6CODE_DSTUNREACH_HOST;
    picmp6->msg.dstunreach.reserved = 0;
    netwib_er(netwib_buf_init_ext_empty(&picmp6->msg.dstunreach.badippacket));
    break;
  case NETWIB_ICMP6TYPE_PKTTOOBIG :
    picmp6->code = NETWIB_ICMP6CODE_PKTTOOBIG;
    picmp6->msg.pkttoobig.mtu = 0;
    netwib_er(netwib_buf_init_ext_empty(&picmp6->msg.pkttoobig.badippacket));
    break;
  case NETWIB_ICMP6TYPE_TIMEEXCEED :
    picmp6->code = NETWIB_ICMP6CODE_TIMEEXCEED_TTL;
    picmp6->msg.timeexceed.reserved = 0;
    netwib_er(netwib_buf_init_ext_empty(&picmp6->msg.timeexceed.badippacket));
    break;
  case NETWIB_ICMP6TYPE_PARAPROB :
    picmp6->code = NETWIB_ICMP6CODE_PARAPROB_HDR;
    picmp6->msg.paraprob.pointer = 0;
    netwib_er(netwib_buf_init_ext_empty(&picmp6->msg.paraprob.badippacket));
    break;
  case NETWIB_ICMP6TYPE_ECHOREQ :
    picmp6->code = NETWIB_ICMP6CODE_ECHOREQ;
    netwib_er(netwib_uint32_init_rand(1, 0xFFFF, &ui));
    picmp6->msg.echo.id = (netwib_uint16)ui;
    netwib_er(netwib_uint32_init_rand(1, 0xFFFF, &ui));
    picmp6->msg.echo.seqnum = (netwib_uint16)ui;
    netwib_er(netwib_buf_init_ext_empty(&picmp6->msg.echo.data));
    break;
  case NETWIB_ICMP6TYPE_ECHOREP :
    picmp6->code = NETWIB_ICMP6CODE_ECHOREP;
    picmp6->msg.echo.id = 0;
    picmp6->msg.echo.seqnum = 0;
    netwib_er(netwib_buf_init_ext_empty(&picmp6->msg.echo.data));
    break;
  case NETWIB_ICMP6TYPE_ROUTERSOLICIT :
    picmp6->code = NETWIB_ICMP6CODE_ROUTERSOLICIT;
    picmp6->msg.routersolicit.reserved = 0;
    netwib_er(netwib_buf_init_ext_empty(&picmp6->msg.routersolicit.options));
    break;
  case NETWIB_ICMP6TYPE_ROUTERADVERT :
    picmp6->code = NETWIB_ICMP6CODE_ROUTERADVERT;
    picmp6->msg.routeradvert.curhoplimit = 0;
    picmp6->msg.routeradvert.managedaddress = NETWIB_FALSE;
    picmp6->msg.routeradvert.otherstateful = NETWIB_FALSE;
    picmp6->msg.routeradvert.reserved = 0;
    picmp6->msg.routeradvert.routerlifetime = 0;
    picmp6->msg.routeradvert.reachabletime = 0;
    picmp6->msg.routeradvert.retranstimer = 0;
    netwib_er(netwib_buf_init_ext_empty(&picmp6->msg.routeradvert.options));
    break;
  case NETWIB_ICMP6TYPE_NEIGHBORSOLICIT :
    picmp6->code = NETWIB_ICMP6CODE_NEIGHBORSOLICIT;
    picmp6->msg.neighborsolicit.reserved = 0;
    netwib_er(netwib_ip_init_ip6_fields(0,0,0,0,
                                        &picmp6->msg.neighborsolicit.target));
    netwib_er(netwib_buf_init_ext_empty(&picmp6->msg.neighborsolicit.options));
    break;
  case NETWIB_ICMP6TYPE_NEIGHBORADVERT :
    picmp6->code = NETWIB_ICMP6CODE_NEIGHBORADVERT;
    picmp6->msg.neighboradvert.router = NETWIB_FALSE;
    picmp6->msg.neighboradvert.solicited = NETWIB_FALSE;
    picmp6->msg.neighboradvert.override = NETWIB_FALSE;
    picmp6->msg.neighboradvert.reserved = 0;
    netwib_er(netwib_ip_init_ip6_fields(0,0,0,0,
                                        &picmp6->msg.neighboradvert.target));
    netwib_er(netwib_buf_init_ext_empty(&picmp6->msg.neighboradvert.options));
    break;
  case NETWIB_ICMP6TYPE_REDIRECT :
    picmp6->code = NETWIB_ICMP6CODE_REDIRECT;
    picmp6->msg.redirect.reserved = 0;
    netwib_er(netwib_ip_init_ip6_fields(0,0,0,0,
                                        &picmp6->msg.redirect.target));
    netwib_er(netwib_ip_init_ip6_fields(0,0,0,0,
                                        &picmp6->msg.redirect.dst));
    netwib_er(netwib_buf_init_ext_empty(&picmp6->msg.redirect.options));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_icmp6(netwib_consticmp6 *picmp6,
                                   netwib_buf *ppkt)
{
  netwib_buf badippkt;
  netwib_data data;
  netwib_uint32 ui;

  if (picmp6->type > 0xFF) {
    return(NETWIB_ERR_PATOOBIGFORHDR);
  }
  if (picmp6->code > 0xFF) {
    return(NETWIB_ERR_PATOOBIGFORHDR);
  }

  netwib_er(netwib_buf_wantspace(ppkt, NETWIB_ICMP6_MINLEN, &data));
  netwib__data_append_uint8(data, picmp6->type);
  netwib__data_append_uint8(data, picmp6->code);
  netwib__data_append_uint16(data, picmp6->check);
  ppkt->endoffset += NETWIB_ICMP6_MINLEN;

  switch(picmp6->type) {
  case NETWIB_ICMP6TYPE_DSTUNREACH :
    netwib__data_append_uint32(data, picmp6->msg.dstunreach.reserved);
    ppkt->endoffset += 4;
    netwib_er(netwib_ip64bits_init_ippkt(&picmp6->msg.dstunreach.badippacket,
                                         &badippkt));
    netwib_er(netwib_buf_append_buf(&badippkt, ppkt));
    break;
  case NETWIB_ICMP6TYPE_PKTTOOBIG :
    netwib__data_append_uint32(data, picmp6->msg.pkttoobig.mtu);
    ppkt->endoffset += 4;
    netwib_er(netwib_ip64bits_init_ippkt(&picmp6->msg.pkttoobig.badippacket,
                                         &badippkt));
    netwib_er(netwib_buf_append_buf(&badippkt, ppkt));
    break;
  case NETWIB_ICMP6TYPE_TIMEEXCEED :
    netwib__data_append_uint32(data, picmp6->msg.timeexceed.reserved);
    ppkt->endoffset += 4;
    netwib_er(netwib_ip64bits_init_ippkt(&picmp6->msg.timeexceed.badippacket,
                                         &badippkt));
    netwib_er(netwib_buf_append_buf(&badippkt, ppkt));
    break;
  case NETWIB_ICMP6TYPE_PARAPROB :
    netwib__data_append_uint32(data, picmp6->msg.paraprob.pointer);
    ppkt->endoffset += 4;
    netwib_er(netwib_ip64bits_init_ippkt(&picmp6->msg.paraprob.badippacket,
                                         &badippkt));
    netwib_er(netwib_buf_append_buf(&badippkt, ppkt));
    break;
  case NETWIB_ICMP6TYPE_ECHOREQ :
  case NETWIB_ICMP6TYPE_ECHOREP :
    netwib__data_append_uint16(data, picmp6->msg.echo.id);
    netwib__data_append_uint16(data, picmp6->msg.echo.seqnum);
    ppkt->endoffset += 4;
    netwib_er(netwib_buf_append_buf(&picmp6->msg.echo.data, ppkt));
    break;
  case NETWIB_ICMP6TYPE_ROUTERSOLICIT :
    netwib__data_append_uint32(data, picmp6->msg.routersolicit.reserved);
    ppkt->endoffset += 4;
    netwib_er(netwib_buf_append_buf(&picmp6->msg.routersolicit.options, ppkt));
    break;
  case NETWIB_ICMP6TYPE_ROUTERADVERT :
    netwib__data_append_uint8(data, picmp6->msg.routeradvert.curhoplimit);
    ui = picmp6->msg.routeradvert.reserved;
    if (picmp6->msg.routeradvert.managedaddress) {
      ui |= (1 << 7);
    }
    if (picmp6->msg.routeradvert.otherstateful) {
      ui |= (1 << 6);
    }
    netwib__data_append_uint8(data, ui);
    netwib__data_append_uint16(data, picmp6->msg.routeradvert.routerlifetime);
    netwib__data_append_uint32(data, picmp6->msg.routeradvert.reachabletime);
    netwib__data_append_uint32(data, picmp6->msg.routeradvert.retranstimer);
    ppkt->endoffset += 12;
    netwib_er(netwib_buf_append_buf(&picmp6->msg.routeradvert.options, ppkt));
    break;
  case NETWIB_ICMP6TYPE_NEIGHBORSOLICIT :
    netwib__data_append_uint32(data, picmp6->msg.neighborsolicit.reserved);
    if (picmp6->msg.neighborsolicit.target.iptype != NETWIB_IPTYPE_IP6) {
      return(NETWIB_ERR_PAIPTYPENOT6);
    }
    netwib_c_memcpy(data, picmp6->msg.neighborsolicit.target.ipvalue.ip6.b,
                    NETWIB_IP6_LEN);
    ppkt->endoffset += 20;
    netwib_er(netwib_buf_append_buf(&picmp6->msg.neighborsolicit.options,
                                    ppkt));
    break;
  case NETWIB_ICMP6TYPE_NEIGHBORADVERT :
    ui = picmp6->msg.neighboradvert.reserved;
    if (picmp6->msg.neighboradvert.router) {
      ui |= (1 << 31);
    }
    if (picmp6->msg.neighboradvert.solicited) {
      ui |= (1 << 30);
    }
    if (picmp6->msg.neighboradvert.override) {
      ui |= (1 << 29);
    }
    netwib__data_append_uint32(data, ui);
    if (picmp6->msg.neighboradvert.target.iptype != NETWIB_IPTYPE_IP6) {
      return(NETWIB_ERR_PAIPTYPENOT6);
    }
    netwib_c_memcpy(data, picmp6->msg.neighboradvert.target.ipvalue.ip6.b,
                    NETWIB_IP6_LEN);
    ppkt->endoffset += 20;
    netwib_er(netwib_buf_append_buf(&picmp6->msg.neighboradvert.options,
                                    ppkt));
    break;
  case NETWIB_ICMP6TYPE_REDIRECT :
    netwib__data_append_uint32(data, picmp6->msg.redirect.reserved);
    if (picmp6->msg.redirect.target.iptype != NETWIB_IPTYPE_IP6) {
      return(NETWIB_ERR_PAIPTYPENOT6);
    }
    netwib_c_memcpy(data, picmp6->msg.redirect.target.ipvalue.ip6.b,
                    NETWIB_IP6_LEN);
    if (picmp6->msg.redirect.dst.iptype != NETWIB_IPTYPE_IP6) {
      return(NETWIB_ERR_PAIPTYPENOT6);
    }
    data += NETWIB_IP6_LEN;
    netwib_c_memcpy(data, picmp6->msg.redirect.dst.ipvalue.ip6.b,
                    NETWIB_IP6_LEN);
    ppkt->endoffset += 36;
    netwib_er(netwib_buf_append_buf(&picmp6->msg.redirect.options,
                                    ppkt));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_icmp6(netwib_constbuf *ppkt,
                                   netwib_icmp6 *picmp6,
                                   netwib_uint32 *pskipsize)
{
  netwib_data data;
  netwib_uint32 datasize, ui;

  datasize = netwib__buf_ref_data_size(ppkt);
  if (pskipsize != NULL) *pskipsize = datasize;
  if (datasize < NETWIB_ICMP6_MINLEN) {
    return(NETWIB_ERR_DATAMISSING);
  }
  data = netwib__buf_ref_data_ptr(ppkt);

  netwib__data_decode_uint8t(data, picmp6->type, netwib_icmp6type);
  netwib__data_decode_uint8(data, picmp6->code);
  netwib__data_decode_uint16(data, picmp6->check);
  datasize -= 4;

  switch(picmp6->type) {
  case NETWIB_ICMP6TYPE_DSTUNREACH :
    if (datasize < 4) {
      return(NETWIB_ERR_DATAMISSING);
    }
    netwib__data_decode_uint32(data, picmp6->msg.dstunreach.reserved);
    datasize -= 4;
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, &picmp6->msg.dstunreach.badippacket));
    break;
  case NETWIB_ICMP6TYPE_PKTTOOBIG :
    if (datasize < 4) {
      return(NETWIB_ERR_DATAMISSING);
    }
    netwib__data_decode_uint32(data, picmp6->msg.pkttoobig.mtu);
    datasize -= 4;
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, &picmp6->msg.pkttoobig.badippacket));
    break;
  case NETWIB_ICMP6TYPE_TIMEEXCEED :
    if (datasize < 4) {
      return(NETWIB_ERR_DATAMISSING);
    }
    netwib__data_decode_uint32(data, picmp6->msg.timeexceed.reserved);
    datasize -= 4;
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, &picmp6->msg.timeexceed.badippacket));
    break;
  case NETWIB_ICMP6TYPE_PARAPROB :
    if (datasize < 4) {
      return(NETWIB_ERR_DATAMISSING);
    }
    netwib__data_decode_uint32(data, picmp6->msg.paraprob.pointer);
    datasize -= 4;
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, &picmp6->msg.paraprob.badippacket));
    break;
  case NETWIB_ICMP6TYPE_ECHOREQ :
  case NETWIB_ICMP6TYPE_ECHOREP :
    if (datasize < 4) {
      return(NETWIB_ERR_DATAMISSING);
    }
    netwib__data_decode_uint16(data, picmp6->msg.echo.id);
    netwib__data_decode_uint16(data, picmp6->msg.echo.seqnum);
    datasize -= 4;
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize,
                                              &picmp6->msg.echo.data));
    break;
  case NETWIB_ICMP6TYPE_ROUTERSOLICIT :
    if (datasize < 4) {
      return(NETWIB_ERR_DATAMISSING);
    }
    netwib__data_decode_uint32(data, picmp6->msg.routersolicit.reserved);
    datasize -= 4;
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, &picmp6->msg.routersolicit.options));
    break;
  case NETWIB_ICMP6TYPE_ROUTERADVERT :
    if (datasize < 12) {
      return(NETWIB_ERR_DATAMISSING);
    }
    netwib__data_decode_uint8(data, picmp6->msg.routeradvert.curhoplimit);
    netwib__data_decode_uint8(data, ui);
    picmp6->msg.routeradvert.managedaddress = (ui & (1<<7))?NETWIB_TRUE:NETWIB_FALSE;
    picmp6->msg.routeradvert.otherstateful = (ui & (1<<6))?NETWIB_TRUE:NETWIB_FALSE;
    picmp6->msg.routeradvert.reserved = (netwib_byte)(ui & 0x3F);
    netwib__data_decode_uint16(data, picmp6->msg.routeradvert.routerlifetime);
    netwib__data_decode_uint32(data, picmp6->msg.routeradvert.reachabletime);
    netwib__data_decode_uint32(data, picmp6->msg.routeradvert.retranstimer);
    datasize -= 12;
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, &picmp6->msg.routeradvert.options));
    break;
  case NETWIB_ICMP6TYPE_NEIGHBORSOLICIT :
    if (datasize < 20) {
      return(NETWIB_ERR_DATAMISSING);
    }
    netwib__data_decode_uint32(data, picmp6->msg.neighborsolicit.reserved);
    picmp6->msg.neighborsolicit.target.iptype = NETWIB_IPTYPE_IP6;
    netwib_c_memcpy(picmp6->msg.neighborsolicit.target.ipvalue.ip6.b, data,
                    NETWIB_IP6_LEN);
    data += NETWIB_IP6_LEN;
    datasize -= 20;
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, &picmp6->msg.neighborsolicit.options));
    break;
  case NETWIB_ICMP6TYPE_NEIGHBORADVERT :
    if (datasize < 20) {
      return(NETWIB_ERR_DATAMISSING);
    }
    netwib__data_decode_uint32(data, ui);
    picmp6->msg.neighboradvert.router = (ui & (1<<31))?NETWIB_TRUE:NETWIB_FALSE;
    picmp6->msg.neighboradvert.solicited = (ui & (1<<30))?NETWIB_TRUE:NETWIB_FALSE;
    picmp6->msg.neighboradvert.override = (ui & (1<<29))?NETWIB_TRUE:NETWIB_FALSE;
    picmp6->msg.neighboradvert.reserved = ui & 0x1FFFFFFF;
    picmp6->msg.neighboradvert.target.iptype = NETWIB_IPTYPE_IP6;
    netwib_c_memcpy(picmp6->msg.neighboradvert.target.ipvalue.ip6.b, data,
                    NETWIB_IP6_LEN);
    data += NETWIB_IP6_LEN;
    datasize -= 20;
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize,
                                       &picmp6->msg.neighboradvert.options));
    break;
  case NETWIB_ICMP6TYPE_REDIRECT :
    if (datasize < 36) {
      return(NETWIB_ERR_DATAMISSING);
    }
    netwib__data_decode_uint32(data, picmp6->msg.redirect.reserved);
    picmp6->msg.redirect.target.iptype = NETWIB_IPTYPE_IP6;
    netwib_c_memcpy(picmp6->msg.redirect.target.ipvalue.ip6.b, data,
                    NETWIB_IP6_LEN);
    data += NETWIB_IP6_LEN;
    picmp6->msg.redirect.dst.iptype = NETWIB_IPTYPE_IP6;
    netwib_c_memcpy(picmp6->msg.redirect.dst.ipvalue.ip6.b, data,
                    NETWIB_IP6_LEN);
    data += NETWIB_IP6_LEN;
    datasize -= 36;
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize,
                                              &picmp6->msg.redirect.options));
    break;
  default :
    return(NETWIB_ERR_LONOTIMPLEMENTED);
    break;
  }

  return(NETWIB_ERR_OK);
}
