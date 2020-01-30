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
static netwib_err netwib_priv_icmp6_show(netwib_consticmp6 *picmp6,
                                         netwib_buf *pbuf)
{

  switch(picmp6->type) {
  case NETWIB_ICMP6TYPE_DSTUNREACH :
    netwib_er(netwib_show_array_text32("reserved", pbuf));
    netwib_er(netwib_show_array_num32(picmp6->msg.dstunreach.reserved,
                                      NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                      pbuf));
    netwib_er(netwib_show_array_text32("bad IP packet :", pbuf));
    netwib_er(netwib_pkt_ip_show(&picmp6->msg.dstunreach.badippacket,
                                 NULL, NETWIB_ENCODETYPE_ARRAY,
                                 NETWIB_ENCODETYPE_DUMP, pbuf));
    break;
  case NETWIB_ICMP6TYPE_PKTTOOBIG :
    netwib_er(netwib_show_array_text32("mtu", pbuf));
    netwib_er(netwib_show_array_num32(picmp6->msg.pkttoobig.mtu,
                                      NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                      pbuf));
    netwib_er(netwib_show_array_text32("bad IP packet :", pbuf));
    netwib_er(netwib_pkt_ip_show(&picmp6->msg.pkttoobig.badippacket,
                                 NULL, NETWIB_ENCODETYPE_ARRAY,
                                 NETWIB_ENCODETYPE_DUMP, pbuf));
    break;
  case NETWIB_ICMP6TYPE_TIMEEXCEED :
    netwib_er(netwib_show_array_text32("reserved", pbuf));
    netwib_er(netwib_show_array_num32(picmp6->msg.timeexceed.reserved,
                                      NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                      pbuf));
    netwib_er(netwib_show_array_text32("bad IP packet :", pbuf));
    netwib_er(netwib_pkt_ip_show(&picmp6->msg.timeexceed.badippacket,
                                 NULL, NETWIB_ENCODETYPE_ARRAY,
                                 NETWIB_ENCODETYPE_DUMP, pbuf));
    break;
  case NETWIB_ICMP6TYPE_PARAPROB :
    netwib_er(netwib_show_array_text32("pointer", pbuf));
    netwib_er(netwib_show_array_num32(picmp6->msg.paraprob.pointer,
                                      NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                      pbuf));
    netwib_er(netwib_show_array_text32("bad IP packet :", pbuf));
    netwib_er(netwib_pkt_ip_show(&picmp6->msg.paraprob.badippacket,
                                 NULL, NETWIB_ENCODETYPE_ARRAY,
                                 NETWIB_ENCODETYPE_DUMP, pbuf));
    break;
  case NETWIB_ICMP6TYPE_ECHOREQ :
  case NETWIB_ICMP6TYPE_ECHOREP :
    netwib_er(netwib_show_array_line_begin(pbuf));
    netwib_er(netwib_show_array_text16("id", pbuf));
    netwib_er(netwib_show_array_text16("seqnum", pbuf));
    netwib_er(netwib_show_array_line_end(pbuf));
    netwib_er(netwib_show_array_line_begin(pbuf));
    netwib_er(netwib_show_array_num16(picmp6->msg.echo.id,
                                      NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                      pbuf));
    netwib_er(netwib_show_array_num16(picmp6->msg.echo.seqnum,
                                      NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                      pbuf));
    netwib_er(netwib_show_array_line_end(pbuf));
    netwib_er(netwib_show_array_data(" data",
                                     &picmp6->msg.echo.data,
                                     NETWIB_ENCODETYPE_HEXA0, ' ',
                                     pbuf));
    netwib_er(netwib_show_array_tail(pbuf));
    break;
  case NETWIB_ICMP6TYPE_ROUTERSOLICIT :
    netwib_er(netwib_show_array_text32("reserved", pbuf));
    netwib_er(netwib_show_array_num32(picmp6->msg.routersolicit.reserved,
                                      NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                      pbuf));
    if (netwib__buf_ref_data_size(&picmp6->msg.routersolicit.options)) {
      netwib_er(netwib_show_array_text32("options :", pbuf));
      netwib_er(netwib_icmp6nds_show(&picmp6->msg.routersolicit.options,
                                     NETWIB_ENCODETYPE_ARRAY, pbuf));
    }
    break;
  case NETWIB_ICMP6TYPE_ROUTERADVERT :
    netwib_er(netwib_show_array_line_begin(pbuf));
    netwib_er(netwib_show_array_text8("cur hop limit", pbuf));
    netwib_er(netwib_show_array_text1("M", pbuf));
    netwib_er(netwib_show_array_text1("O", pbuf));
    netwib_er(netwib_show_array_text(6, "reserved", pbuf));
    netwib_er(netwib_show_array_text16("router lifetime", pbuf));
    netwib_er(netwib_show_array_line_end(pbuf));
    netwib_er(netwib_show_array_line_begin(pbuf));
    netwib_er(netwib_show_array_num8(picmp6->msg.routeradvert.curhoplimit,
                                     NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                     pbuf));
    netwib_er(netwib_show_array_num1(picmp6->msg.routeradvert.managedaddress,
                                     pbuf));
    netwib_er(netwib_show_array_num1(picmp6->msg.routeradvert.otherstateful,
                                     pbuf));
    netwib_er(netwib_show_array_num(6, picmp6->msg.routeradvert.reserved,
                                    NETWIB_SHOW_ARRAY_NUMTYPE_DEC,
                                    pbuf));
    netwib_er(netwib_show_array_num16(picmp6->msg.routeradvert.routerlifetime,
                                      NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                      pbuf));
    netwib_er(netwib_show_array_line_end(pbuf));
    netwib_er(netwib_show_array_text32("reachable time", pbuf));
    netwib_er(netwib_show_array_num32(picmp6->msg.routeradvert.reachabletime,
                                      NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                      pbuf));
    netwib_er(netwib_show_array_text32("retrans timer", pbuf));
    netwib_er(netwib_show_array_num32(picmp6->msg.routeradvert.retranstimer,
                                      NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                      pbuf));
    if (netwib__buf_ref_data_size(&picmp6->msg.routeradvert.options)) {
      netwib_er(netwib_show_array_text32("options :", pbuf));
      netwib_er(netwib_icmp6nds_show(&picmp6->msg.routeradvert.options,
                                     NETWIB_ENCODETYPE_ARRAY, pbuf));
    }
    break;
  case NETWIB_ICMP6TYPE_NEIGHBORSOLICIT :
    netwib_er(netwib_show_array_text32("reserved", pbuf));
    netwib_er(netwib_show_array_num32(picmp6->msg.neighborsolicit.reserved,
                                      NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                      pbuf));
    netwib_er(netwib_show_array_text32("target", pbuf));
    netwib_er(netwib_show_array_fmt(32, NETWIB_SHOW_ARRAY_ALIGN_CENTER, '_',
                                    pbuf, "%{ip}",
                                    &picmp6->msg.neighborsolicit.target));
    if (netwib__buf_ref_data_size(&picmp6->msg.neighborsolicit.options)) {
      netwib_er(netwib_show_array_text32("options :", pbuf));
      netwib_er(netwib_icmp6nds_show(&picmp6->msg.neighborsolicit.options,
                                     NETWIB_ENCODETYPE_ARRAY, pbuf));
    }
    break;
  case NETWIB_ICMP6TYPE_NEIGHBORADVERT :
    netwib_er(netwib_show_array_line_begin(pbuf));
    netwib_er(netwib_show_array_text1("R", pbuf));
    netwib_er(netwib_show_array_text1("S", pbuf));
    netwib_er(netwib_show_array_text1("O", pbuf));
    netwib_er(netwib_show_array_text(29, "reserved", pbuf));
    netwib_er(netwib_show_array_line_end(pbuf));
    netwib_er(netwib_show_array_line_begin(pbuf));
    netwib_er(netwib_show_array_num1(picmp6->msg.neighboradvert.router,
                                     pbuf));
    netwib_er(netwib_show_array_num1(picmp6->msg.neighboradvert.solicited,
                                     pbuf));
    netwib_er(netwib_show_array_num1(picmp6->msg.neighboradvert.override,
                                     pbuf));
    netwib_er(netwib_show_array_num(29, picmp6->msg.neighboradvert.reserved,
                                    NETWIB_SHOW_ARRAY_NUMTYPE_DEC,
                                    pbuf));
    netwib_er(netwib_show_array_line_end(pbuf));
    netwib_er(netwib_show_array_text32("target", pbuf));
    netwib_er(netwib_show_array_fmt(32, NETWIB_SHOW_ARRAY_ALIGN_CENTER, '_',
                                    pbuf, "%{ip}",
                                    &picmp6->msg.neighboradvert.target));
    if (netwib__buf_ref_data_size(&picmp6->msg.neighboradvert.options)) {
      netwib_er(netwib_show_array_text32("options :", pbuf));
      netwib_er(netwib_icmp6nds_show(&picmp6->msg.neighboradvert.options,
                                     NETWIB_ENCODETYPE_ARRAY, pbuf));
    }
    break;
  case NETWIB_ICMP6TYPE_REDIRECT :
    netwib_er(netwib_show_array_text32("reserved", pbuf));
    netwib_er(netwib_show_array_num32(picmp6->msg.redirect.reserved,
                                      NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                      pbuf));
    netwib_er(netwib_show_array_text32("target", pbuf));
    netwib_er(netwib_show_array_fmt(32, NETWIB_SHOW_ARRAY_ALIGN_CENTER, '_',
                                    pbuf, "%{ip}",
                                    &picmp6->msg.redirect.target));
    netwib_er(netwib_show_array_text32("destination", pbuf));
    netwib_er(netwib_show_array_fmt(32, NETWIB_SHOW_ARRAY_ALIGN_CENTER, '_',
                                    pbuf, "%{ip}",
                                    &picmp6->msg.redirect.dst));
    if (netwib__buf_ref_data_size(&picmp6->msg.redirect.options)) {
      netwib_er(netwib_show_array_text32("options :", pbuf));
      netwib_er(netwib_icmp6nds_show(&picmp6->msg.redirect.options,
                                     NETWIB_ENCODETYPE_ARRAY, pbuf));
    }
    break;
  default :
    return(NETWIB_ERR_LONOTIMPLEMENTED);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_icmp6_show(netwib_consticmp6 *picmp6,
                             netwib_encodetype encodetype,
                             netwib_buf *pbuf)
{
  netwib_buf buf;
  netwib_byte array[80];
  netwib_string str;

  switch(encodetype) {
    case NETWIB_ENCODETYPE_SYNTH :
      netwib_er(netwib_buf_append_fmt(pbuf, "icmp6:%{uint32}:%{uint32}",
                                      picmp6->type, picmp6->code));
      break;
    case NETWIB_ENCODETYPE_ARRAY :
      netwib_er(netwib_buf_init_ext_arrayempty(array, sizeof(array), &buf));
      netwib_er(netwib_buf_append_string("ICMP6_", &buf));
      netwib_er(netwib_buf_append_icmp6type(picmp6->type, &buf));
      netwib_er(netwib_buf_append_string("_", &buf));
      netwib_er(netwib_buf_append_icmp6code(picmp6->type, picmp6->code,
                                            &buf));
      netwib_er(netwib_buf_ref_string(&buf, &str));
      netwib_er(netwib_show_array_head(str, pbuf));
      netwib_er(netwib_show_array_line_begin(pbuf));
      netwib_er(netwib_show_array_text8("type", pbuf));
      netwib_er(netwib_show_array_text8("code", pbuf));
      netwib_er(netwib_show_array_text16("checksum", pbuf));
      netwib_er(netwib_show_array_line_end(pbuf));
      netwib_er(netwib_show_array_line_begin(pbuf));
      netwib_er(netwib_show_array_num8((netwib_uint8)picmp6->type,
                                       NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                       pbuf));
      netwib_er(netwib_show_array_num8((netwib_uint8)picmp6->code,
                                       NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                       pbuf));
      netwib_er(netwib_show_array_num16(picmp6->check,
                                        NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                        pbuf));
      netwib_er(netwib_show_array_line_end(pbuf));
      netwib_er(netwib_priv_icmp6_show(picmp6, pbuf));
      break;
    default :
      netwib_er(netwib_buf_init_mallocdefault(&buf));
      netwib_er(netwib_pkt_append_icmp6(picmp6, &buf));
      netwib_er(netwib_buf_encode(&buf, encodetype, pbuf));
      netwib_er(netwib_buf_close(&buf));
      break;
  }

  return(NETWIB_ERR_OK);
}
