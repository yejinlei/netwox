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
netwib_err netwib_buf_append_icmp4type(netwib_icmp4type type,
                                       netwib_buf *pbuf)
{
  netwib_conststring pc = NULL;

  switch(type) {
    case NETWIB_ICMP4TYPE_ECHOREP :
      pc = "echo reply";
      break;
    case NETWIB_ICMP4TYPE_DSTUNREACH :
      pc = "destination unreachable";
      break;
    case NETWIB_ICMP4TYPE_SRCQUENCH :
      pc = "source quench";
      break;
    case NETWIB_ICMP4TYPE_REDIRECT :
      pc = "redirect";
      break;
    case NETWIB_ICMP4TYPE_ALTHOSTAD :
      pc = "alternate host address";
      break;
    case NETWIB_ICMP4TYPE_ECHOREQ :
      pc = "echo request";
      break;
    case NETWIB_ICMP4TYPE_ROUTERADVERT :
      pc = "router advertisement";
      break;
    case NETWIB_ICMP4TYPE_ROUTERSELECT :
      pc = "router selection";
      break;
    case NETWIB_ICMP4TYPE_TIMEEXCEED :
      pc = "time exceeded";
      break;
    case NETWIB_ICMP4TYPE_PARAPROB :
      pc = "parameter problem";
      break;
    case NETWIB_ICMP4TYPE_TIMESTAMPREQ :
      pc = "timestamp request";
      break;
    case NETWIB_ICMP4TYPE_TIMESTAMPREP :
      pc = "timestamp reply";
      break;
    case NETWIB_ICMP4TYPE_INFOREQ :
      pc = "information request";
      break;
    case NETWIB_ICMP4TYPE_INFOREP :
      pc = "information reply";
      break;
    case NETWIB_ICMP4TYPE_ADMASKREQ :
      pc = "address mask request";
      break;
    case NETWIB_ICMP4TYPE_ADMASKREP :
      pc = "address mask reply";
      break;
    case NETWIB_ICMP4TYPE_TRACEROUTE :
      pc = "traceroute";
      break;
    case NETWIB_ICMP4TYPE_DATACONVERR :
      pc = "datagram conversion error";
      break;
    case NETWIB_ICMP4TYPE_MOBHOSTREDIR :
      pc = "mobile host redirect";
      break;
    case NETWIB_ICMP4TYPE_IPV6WHEREYOU :
      pc = "IPv6 where are you";
      break;
    case NETWIB_ICMP4TYPE_IPV6IAMHERE :
      pc = "IPv6 I'am here";
      break;
    case NETWIB_ICMP4TYPE_MOBREGREQ :
      pc = "mobile registration request";
      break;
    case NETWIB_ICMP4TYPE_MOBREGREP :
      pc = "mobile registration reply";
      break;
    case NETWIB_ICMP4TYPE_SKIP :
      pc = "skip";
      break;
    case NETWIB_ICMP4TYPE_PHOTURIS :
      pc = "photuris";
      break;
    default :
      pc = "unknown";
      break;
  }

  netwib_er(netwib_buf_append_string(pc, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_icmp4code(netwib_icmp4type type,
                                       netwib_icmp4code code,
                                       netwib_buf *pbuf)
{
  netwib_conststring pc = NULL;

  switch(type) {
  case NETWIB_ICMP4TYPE_ECHOREP :
    switch(code) {
    case NETWIB_ICMP4CODE_ECHOREP :
      pc = "";
      break;
    default :
      pc = "unknown";
      break;
    }
    break;
  case NETWIB_ICMP4TYPE_DSTUNREACH :
    switch(code) {
    case NETWIB_ICMP4CODE_DSTUNREACH_NET :
      pc = "network";
      break;
    case NETWIB_ICMP4CODE_DSTUNREACH_HOST :
      pc = "host";
      break;
    case NETWIB_ICMP4CODE_DSTUNREACH_PROTO :
      pc = "protocol";
      break;
    case NETWIB_ICMP4CODE_DSTUNREACH_PORT :
      pc = "port";
      break;
    case NETWIB_ICMP4CODE_DSTUNREACH_FRAG :
      pc = "fragmentation needed";
      break;
    case NETWIB_ICMP4CODE_DSTUNREACH_SR :
      pc = "source route failed";
      break;
    case NETWIB_ICMP4CODE_DSTUNREACH_NETUNKNOWN :
      pc = "network unknown";
      break;
    case NETWIB_ICMP4CODE_DSTUNREACH_HOSTUNKNOWN :
      pc = "host unknown";
      break;
    case NETWIB_ICMP4CODE_DSTUNREACH_HOSTISOL :
      pc = "source host isolated";
      break;
    case NETWIB_ICMP4CODE_DSTUNREACH_NETPROHI :
      pc = "network prohibited";
      break;
    case NETWIB_ICMP4CODE_DSTUNREACH_HOSTPROHI :
      pc = "host prohibited";
      break;
    case NETWIB_ICMP4CODE_DSTUNREACH_NETTOS :
      pc = "network TOS";
      break;
    case NETWIB_ICMP4CODE_DSTUNREACH_HOSTTOS :
      pc = "host TOS";
      break;
    case NETWIB_ICMP4CODE_DSTUNREACH_PROHI :
      pc = "admin prohibited";
      break;
    case NETWIB_ICMP4CODE_DSTUNREACH_PRECVIOL :
      pc = "precedence violation";
      break;
    case NETWIB_ICMP4CODE_DSTUNREACH_PRECCUTOFF :
      pc = "precedence cutoff";
      break;
    default :
      pc = "unknown";
      break;
    }
    break;
  case NETWIB_ICMP4TYPE_SRCQUENCH :
    switch(code) {
    case NETWIB_ICMP4CODE_SRCQUENCH :
      pc = "";
      break;
    default :
      pc = "unknown";
      break;
    }
    break;
  case NETWIB_ICMP4TYPE_REDIRECT :
    switch(code) {
    case NETWIB_ICMP4CODE_REDIRECT_NET :
      pc = "network";
      break;
    case NETWIB_ICMP4CODE_REDIRECT_HOST :
      pc = "host";
      break;
    case NETWIB_ICMP4CODE_REDIRECT_NETTOS :
      pc = "network and tos";
      break;
    case NETWIB_ICMP4CODE_REDIRECT_HOSTTOS :
      pc = "host and tos";
      break;
    default :
      pc = "unknown";
      break;
    }
    break;
  case NETWIB_ICMP4TYPE_ECHOREQ :
    switch(code) {
    case NETWIB_ICMP4CODE_ECHOREQ :
      pc = "";
      break;
    default :
      pc = "unknown";
      break;
    }
    break;
  case NETWIB_ICMP4TYPE_TIMEEXCEED :
    switch(code) {
    case NETWIB_ICMP4CODE_TIMEEXCEED_TTL :
      pc = "ttl";
      break;
    case NETWIB_ICMP4CODE_TIMEEXCEED_FRAG :
      pc = "fragment reassembly";
      break;
    default :
      pc = "unknown";
      break;
    }
    break;
  case NETWIB_ICMP4TYPE_PARAPROB :
    switch(code) {
    case NETWIB_ICMP4CODE_PARAPROB_POINTER :
      pc = "pointer indicates error";
      break;
    case NETWIB_ICMP4CODE_PARAPROB_MISSOPT :
      pc = "missing option";
      break;
    case NETWIB_ICMP4CODE_PARAPROB_BADLEN :
      pc = "bad length";
      break;
    default :
      pc = "unknown";
      break;
    }
    break;
  case NETWIB_ICMP4TYPE_TIMESTAMPREQ :
    switch(code) {
    case NETWIB_ICMP4CODE_TIMESTAMPREQ :
      pc = "";
      break;
    default :
      pc = "unknown";
      break;
    }
    break;
  case NETWIB_ICMP4TYPE_TIMESTAMPREP :
    switch(code) {
    case NETWIB_ICMP4CODE_TIMESTAMPREP :
      pc = "";
      break;
    default :
      pc = "unknown";
      break;
    }
    break;
  case NETWIB_ICMP4TYPE_INFOREQ :
    switch(code) {
    case NETWIB_ICMP4CODE_INFOREQ :
      pc = "";
      break;
    default :
      pc = "unknown";
      break;
    }
    break;
  case NETWIB_ICMP4TYPE_INFOREP :
    switch(code) {
    case NETWIB_ICMP4CODE_INFOREP :
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
netwib_err netwib_priv_icmp4_size(netwib_consticmp4 *picmp4,
                                  netwib_uint32 *psize)
{
  netwib_uint32 size=0;

  switch(picmp4->type) {
  case NETWIB_ICMP4TYPE_ECHOREP :
  case NETWIB_ICMP4TYPE_ECHOREQ :
    size = 8 + netwib__buf_ref_data_size(&picmp4->msg.echo.data);
    break;
  case NETWIB_ICMP4TYPE_DSTUNREACH :
    size = 8 + netwib__buf_ref_data_size(&picmp4->msg.dstunreach.badippacket);
    break;
  case NETWIB_ICMP4TYPE_SRCQUENCH :
    size = 8 + netwib__buf_ref_data_size(&picmp4->msg.srcquench.badippacket);
    break;
  case NETWIB_ICMP4TYPE_REDIRECT :
    size = 8 + netwib__buf_ref_data_size(&picmp4->msg.redirect.badippacket);
    break;
  case NETWIB_ICMP4TYPE_TIMEEXCEED :
    size = 8 + netwib__buf_ref_data_size(&picmp4->msg.timeexceed.badippacket);
    break;
  case NETWIB_ICMP4TYPE_PARAPROB :
    size = 8 + netwib__buf_ref_data_size(&picmp4->msg.paraprob.badippacket);
    break;
  case NETWIB_ICMP4TYPE_TIMESTAMPREQ :
  case NETWIB_ICMP4TYPE_TIMESTAMPREP :
    size = 20;
    break;
  case NETWIB_ICMP4TYPE_INFOREQ :
  case NETWIB_ICMP4TYPE_INFOREP :
    size = 8 + netwib__buf_ref_data_size(&picmp4->msg.info.data);
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  }

  if (psize != NULL) *psize = size;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_icmp4_initdefault(netwib_icmp4type type,
                                    netwib_icmp4 *picmp4)
{
  netwib_uint32 ui;

  picmp4->type = type;
  picmp4->check = 0;
  switch(type) {
  case NETWIB_ICMP4TYPE_ECHOREP :
    picmp4->code = NETWIB_ICMP4CODE_ECHOREP;
    picmp4->msg.echo.id = 0;
    picmp4->msg.echo.seqnum = 0;
    netwib_er(netwib_buf_init_ext_empty(&picmp4->msg.echo.data));
    break;
  case NETWIB_ICMP4TYPE_DSTUNREACH :
    picmp4->code = NETWIB_ICMP4CODE_DSTUNREACH_HOST;
    picmp4->msg.dstunreach.reserved = 0;
    netwib_er(netwib_buf_init_ext_empty(&picmp4->msg.dstunreach.badippacket));
    break;
  case NETWIB_ICMP4TYPE_SRCQUENCH :
    picmp4->code = NETWIB_ICMP4CODE_SRCQUENCH;
    picmp4->msg.srcquench.reserved = 0;
    netwib_er(netwib_buf_init_ext_empty(&picmp4->msg.srcquench.badippacket));
    break;
  case NETWIB_ICMP4TYPE_REDIRECT :
    picmp4->code = NETWIB_ICMP4CODE_REDIRECT_HOST;
    netwib_er(netwib_ip_init_ip4(0, &picmp4->msg.redirect.gw));
    netwib_er(netwib_buf_init_ext_empty(&picmp4->msg.redirect.badippacket));
    break;
  case NETWIB_ICMP4TYPE_ECHOREQ :
    picmp4->code = NETWIB_ICMP4CODE_ECHOREQ;
    netwib_er(netwib_uint32_init_rand(1, 0xFFFF, &ui));
    picmp4->msg.echo.id = (netwib_uint16)ui;
    netwib_er(netwib_uint32_init_rand(1, 0xFFFF, &ui));
    picmp4->msg.echo.seqnum = (netwib_uint16)ui;
    netwib_er(netwib_buf_init_ext_empty(&picmp4->msg.echo.data));
    break;
  case NETWIB_ICMP4TYPE_TIMEEXCEED :
    picmp4->code = NETWIB_ICMP4CODE_TIMEEXCEED_TTL;
    picmp4->msg.timeexceed.reserved = 0;
    netwib_er(netwib_buf_init_ext_empty(&picmp4->msg.timeexceed.badippacket));
    break;
  case NETWIB_ICMP4TYPE_PARAPROB :
    picmp4->code = NETWIB_ICMP4CODE_PARAPROB_POINTER;
    picmp4->msg.paraprob.pointer = 0;
    picmp4->msg.paraprob.reserved = 0;
    netwib_er(netwib_buf_init_ext_empty(&picmp4->msg.paraprob.badippacket));
    break;
  case NETWIB_ICMP4TYPE_TIMESTAMPREQ :
    picmp4->code = NETWIB_ICMP4CODE_TIMESTAMPREQ;
    netwib_er(netwib_uint32_init_rand(1, 0xFFFF, &ui));
    picmp4->msg.timestamp.id = (netwib_uint16)ui;
    netwib_er(netwib_uint32_init_rand(1, 0xFFFF, &ui));
    picmp4->msg.timestamp.seqnum = (netwib_uint16)ui;
    picmp4->msg.timestamp.originatetimestamp = 0;
    picmp4->msg.timestamp.receivetimestamp = 0;
    picmp4->msg.timestamp.transmittimestamp = 0;
    break;
  case NETWIB_ICMP4TYPE_TIMESTAMPREP :
    picmp4->code = NETWIB_ICMP4CODE_TIMESTAMPREP;
    picmp4->msg.timestamp.id = 0;
    picmp4->msg.timestamp.seqnum = 0;
    picmp4->msg.timestamp.originatetimestamp = 0;
    picmp4->msg.timestamp.receivetimestamp = 0;
    picmp4->msg.timestamp.transmittimestamp = 0;
    break;
  case NETWIB_ICMP4TYPE_INFOREQ :
    picmp4->code = NETWIB_ICMP4CODE_INFOREQ;
    netwib_er(netwib_uint32_init_rand(1, 0xFFFF, &ui));
    picmp4->msg.info.id = (netwib_uint16)ui;
    netwib_er(netwib_uint32_init_rand(1, 0xFFFF, &ui));
    picmp4->msg.info.seqnum = (netwib_uint16)ui;
    netwib_er(netwib_buf_init_ext_empty(&picmp4->msg.info.data));
    break;
  case NETWIB_ICMP4TYPE_INFOREP :
    picmp4->code = NETWIB_ICMP4CODE_INFOREP;
    picmp4->msg.info.id = 0;
    picmp4->msg.info.seqnum = 0;
    netwib_er(netwib_buf_init_ext_empty(&picmp4->msg.info.data));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_icmp4(netwib_consticmp4 *picmp4,
                                   netwib_buf *ppkt)
{
  netwib_buf badippkt;
  netwib_data data;

  if (picmp4->type > 0xFF) {
    return(NETWIB_ERR_PATOOBIGFORHDR);
  }
  if (picmp4->code > 0xFF) {
    return(NETWIB_ERR_PATOOBIGFORHDR);
  }

  netwib_er(netwib_buf_wantspace(ppkt, NETWIB_ICMP4_MINLEN, &data));
  netwib__data_append_uint8(data, picmp4->type);
  netwib__data_append_uint8(data, picmp4->code);
  netwib__data_append_uint16(data, picmp4->check);
  ppkt->endoffset += NETWIB_ICMP4_MINLEN;

  switch(picmp4->type) {
  case NETWIB_ICMP4TYPE_ECHOREP :
  case NETWIB_ICMP4TYPE_ECHOREQ :
    netwib__data_append_uint16(data, picmp4->msg.echo.id);
    netwib__data_append_uint16(data, picmp4->msg.echo.seqnum);
    ppkt->endoffset += 4;
    netwib_er(netwib_buf_append_buf(&picmp4->msg.echo.data, ppkt));
    break;
  case NETWIB_ICMP4TYPE_DSTUNREACH :
    netwib__data_append_uint32(data, picmp4->msg.dstunreach.reserved);
    ppkt->endoffset += 4;
    netwib_er(netwib_ip64bits_init_ippkt(&picmp4->msg.dstunreach.badippacket,
                                         &badippkt));
    netwib_er(netwib_buf_append_buf(&badippkt, ppkt));
    break;
  case NETWIB_ICMP4TYPE_SRCQUENCH :
    netwib__data_append_uint32(data, picmp4->msg.srcquench.reserved);
    ppkt->endoffset += 4;
    netwib_er(netwib_ip64bits_init_ippkt(&picmp4->msg.srcquench.badippacket,
                                         &badippkt));
    netwib_er(netwib_buf_append_buf(&badippkt, ppkt));
    break;
  case NETWIB_ICMP4TYPE_REDIRECT :
    if (picmp4->msg.redirect.gw.iptype != NETWIB_IPTYPE_IP4) {
      return(NETWIB_ERR_PAIPTYPENOT4);
    }
    netwib__data_append_uint32(data, picmp4->msg.redirect.gw.ipvalue.ip4);
    ppkt->endoffset += 4;
    netwib_er(netwib_ip64bits_init_ippkt(&picmp4->msg.redirect.badippacket,
                                         &badippkt));
    netwib_er(netwib_buf_append_buf(&badippkt, ppkt));
    break;
  case NETWIB_ICMP4TYPE_TIMEEXCEED :
    netwib__data_append_uint32(data, picmp4->msg.timeexceed.reserved);
    ppkt->endoffset += 4;
    netwib_er(netwib_ip64bits_init_ippkt(&picmp4->msg.timeexceed.badippacket,
                                         &badippkt));
    netwib_er(netwib_buf_append_buf(&badippkt, ppkt));
    break;
  case NETWIB_ICMP4TYPE_PARAPROB :
    netwib__data_append_uint8(data, picmp4->msg.paraprob.pointer);
    if (picmp4->msg.paraprob.reserved > 0xFFFFFF) {
      return(NETWIB_ERR_PATOOBIGFORHDR);
    }
    netwib__data_append_uint8(data,
                            netwib_c2_uint32_1(picmp4->msg.paraprob.reserved));
    netwib__data_append_uint8(data,
                            netwib_c2_uint32_2(picmp4->msg.paraprob.reserved));
    netwib__data_append_uint8(data,
                            netwib_c2_uint32_3(picmp4->msg.paraprob.reserved));
    ppkt->endoffset += 4;
    netwib_er(netwib_ip64bits_init_ippkt(&picmp4->msg.paraprob.badippacket,
                                         &badippkt));
    netwib_er(netwib_buf_append_buf(&badippkt, ppkt));
    break;
  case NETWIB_ICMP4TYPE_TIMESTAMPREQ :
  case NETWIB_ICMP4TYPE_TIMESTAMPREP :
    netwib__data_append_uint16(data, picmp4->msg.timestamp.id);
    netwib__data_append_uint16(data, picmp4->msg.timestamp.seqnum);
    netwib__data_append_uint32(data, picmp4->msg.timestamp.originatetimestamp);
    netwib__data_append_uint32(data, picmp4->msg.timestamp.receivetimestamp);
    netwib__data_append_uint32(data, picmp4->msg.timestamp.transmittimestamp);
    ppkt->endoffset += 16;
    break;
  case NETWIB_ICMP4TYPE_INFOREQ :
  case NETWIB_ICMP4TYPE_INFOREP :
    netwib__data_append_uint16(data, picmp4->msg.info.id);
    netwib__data_append_uint16(data, picmp4->msg.info.seqnum);
    ppkt->endoffset += 4;
    netwib_er(netwib_buf_append_buf(&picmp4->msg.info.data, ppkt));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_icmp4(netwib_constbuf *ppkt,
                                   netwib_icmp4 *picmp4,
                                   netwib_uint32 *pskipsize)
{
  netwib_data data;
  netwib_uint32 datasize;

  datasize = netwib__buf_ref_data_size(ppkt);
  if (datasize < NETWIB_ICMP4_MINLEN) {
    return(NETWIB_ERR_DATAMISSING);
  }
  if (pskipsize != NULL) *pskipsize = datasize;
  data = netwib__buf_ref_data_ptr(ppkt);

  netwib__data_decode_uint8t(data, picmp4->type, netwib_icmp4type);
  netwib__data_decode_uint8(data, picmp4->code);
  netwib__data_decode_uint16(data, picmp4->check);
  datasize -= 4;

  switch(picmp4->type) {
  case NETWIB_ICMP4TYPE_ECHOREP :
  case NETWIB_ICMP4TYPE_ECHOREQ :
    if (datasize < 4) {
      return(NETWIB_ERR_DATAMISSING);
    }
    netwib__data_decode_uint16(data, picmp4->msg.echo.id);
    netwib__data_decode_uint16(data, picmp4->msg.echo.seqnum);
    datasize -= 4;
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize,
                                              &picmp4->msg.echo.data));
    break;
  case NETWIB_ICMP4TYPE_DSTUNREACH :
    if (datasize < 4) {
      return(NETWIB_ERR_DATAMISSING);
    }
    netwib__data_decode_uint32(data, picmp4->msg.dstunreach.reserved);
    datasize -= 4;
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, &picmp4->msg.dstunreach.badippacket));
    break;
  case NETWIB_ICMP4TYPE_SRCQUENCH :
    if (datasize < 4) {
      return(NETWIB_ERR_DATAMISSING);
    }
    netwib__data_decode_uint32(data, picmp4->msg.srcquench.reserved);
    datasize -= 4;
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, &picmp4->msg.srcquench.badippacket));
    break;
  case NETWIB_ICMP4TYPE_REDIRECT :
    if (datasize < 4) {
      return(NETWIB_ERR_DATAMISSING);
    }
    picmp4->msg.redirect.gw.iptype = NETWIB_IPTYPE_IP4;
    netwib__data_decode_uint32(data, picmp4->msg.redirect.gw.ipvalue.ip4);
    datasize -= 4;
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, &picmp4->msg.redirect.badippacket));
    break;
  case NETWIB_ICMP4TYPE_TIMEEXCEED :
    if (datasize < 4) {
      return(NETWIB_ERR_DATAMISSING);
    }
    netwib__data_decode_uint32(data, picmp4->msg.timeexceed.reserved);
    datasize -= 4;
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, &picmp4->msg.timeexceed.badippacket));
    break;
  case NETWIB_ICMP4TYPE_PARAPROB :
    if (datasize < 4) {
      return(NETWIB_ERR_DATAMISSING);
    }
    netwib__data_decode_uint8(data, picmp4->msg.paraprob.pointer);
    picmp4->msg.paraprob.reserved = netwib_c2_uint32_4(0, data[0],
                                                       data[1], data[2]);
    data += 3; /* the first decode_uint8 skipped one byte */
    datasize -= 4;
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, &picmp4->msg.paraprob.badippacket));
    break;
  case NETWIB_ICMP4TYPE_TIMESTAMPREQ :
  case NETWIB_ICMP4TYPE_TIMESTAMPREP :
    if (datasize < 16) {
      return(NETWIB_ERR_DATAMISSING);
    }
    if (datasize > 16) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16(data, picmp4->msg.timestamp.id);
    netwib__data_decode_uint16(data, picmp4->msg.timestamp.seqnum);
    netwib__data_decode_uint32(data, picmp4->msg.timestamp.originatetimestamp);
    netwib__data_decode_uint32(data, picmp4->msg.timestamp.receivetimestamp);
    netwib__data_decode_uint32(data, picmp4->msg.timestamp.transmittimestamp);
    break;
  case NETWIB_ICMP4TYPE_INFOREQ :
  case NETWIB_ICMP4TYPE_INFOREP :
    if (datasize < 4) {
      return(NETWIB_ERR_DATAMISSING);
    }
    netwib__data_decode_uint16(data, picmp4->msg.info.id);
    netwib__data_decode_uint16(data, picmp4->msg.info.seqnum);
    datasize -= 4;
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize,
                                              &picmp4->msg.info.data));
    break;
  default :
    return(NETWIB_ERR_LONOTIMPLEMENTED);
    break;
  }

  return(NETWIB_ERR_OK);
}
