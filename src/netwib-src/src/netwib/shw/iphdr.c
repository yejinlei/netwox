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
static netwib_err netwib_show_ip4hdr(netwib_constiphdr *piphdr,
                                     netwib_buf *pbuf)
{

  netwib_er(netwib_show_array_head("IP", pbuf));
  /**/
  netwib_er(netwib_show_array_line_begin(pbuf));
  netwib_er(netwib_show_array_text4("version", pbuf));
  netwib_er(netwib_show_array_text4("ihl", pbuf));
  netwib_er(netwib_show_array_text8("tos", pbuf));
  netwib_er(netwib_show_array_text16("totlen", pbuf));
  netwib_er(netwib_show_array_line_end(pbuf));
  netwib_er(netwib_show_array_line_begin(pbuf));
  netwib_er(netwib_show_array_num4(4, NETWIB_SHOW_ARRAY_NUMTYPE_DEC, pbuf));
  netwib_er(netwib_show_array_num4(piphdr->header.ip4.ihl,
                                   NETWIB_SHOW_ARRAY_NUMTYPE_DEC, pbuf));
  netwib_er(netwib_show_array_num8(piphdr->header.ip4.tos,
                                   NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC, pbuf));
  netwib_er(netwib_show_array_num16(piphdr->header.ip4.totlen,
                                    NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                    pbuf));
  netwib_er(netwib_show_array_line_end(pbuf));
  /**/
  netwib_er(netwib_show_array_line_begin(pbuf));
  netwib_er(netwib_show_array_text16("id", pbuf));
  netwib_er(netwib_show_array_text1("r", pbuf));
  netwib_er(netwib_show_array_text1("D", pbuf));
  netwib_er(netwib_show_array_text1("M", pbuf));
  netwib_er(netwib_show_array_text(13, "offsetfrag", pbuf));
  netwib_er(netwib_show_array_line_end(pbuf));
  netwib_er(netwib_show_array_line_begin(pbuf));
  netwib_er(netwib_show_array_num16(piphdr->header.ip4.id,
                                    NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                    pbuf));
  netwib_er(netwib_show_array_num1(piphdr->header.ip4.reserved, pbuf));
  netwib_er(netwib_show_array_num1(piphdr->header.ip4.dontfrag, pbuf));
  netwib_er(netwib_show_array_num1(piphdr->header.ip4.morefrag, pbuf));
  netwib_er(netwib_show_array_num(13, piphdr->header.ip4.offsetfrag,
                                  NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                  pbuf));
  netwib_er(netwib_show_array_line_end(pbuf));
  /**/
  netwib_er(netwib_show_array_line_begin(pbuf));
  netwib_er(netwib_show_array_text8("ttl", pbuf));
  netwib_er(netwib_show_array_text8("protocol", pbuf));
  netwib_er(netwib_show_array_text16("checksum", pbuf));
  netwib_er(netwib_show_array_line_end(pbuf));
  netwib_er(netwib_show_array_line_begin(pbuf));
  netwib_er(netwib_show_array_num8(piphdr->ttl,
                                   NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                   pbuf));
  netwib_er(netwib_show_array_num8((netwib_uint8)piphdr->protocol,
                                   NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                   pbuf));
  netwib_er(netwib_show_array_num16(piphdr->header.ip4.check,
                                    NETWIB_SHOW_ARRAY_NUMTYPE_HEXA, pbuf));
  netwib_er(netwib_show_array_line_end(pbuf));
  /**/
  netwib_er(netwib_show_array_text32("source", pbuf));
  netwib_er(netwib_show_array_fmt(32, NETWIB_SHOW_ARRAY_ALIGN_CENTER, '_',
                                  pbuf, "%{ip}", &piphdr->src));
  /**/
  netwib_er(netwib_show_array_text32("destination", pbuf));
  netwib_er(netwib_show_array_fmt(32, NETWIB_SHOW_ARRAY_ALIGN_CENTER, '_',
                                  pbuf, "%{ip}", &piphdr->dst));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_show_ip6hdr(netwib_constiphdr *piphdr,
                                     netwib_buf *pbuf)
{

  netwib_er(netwib_show_array_head("IP", pbuf));
  /**/
  netwib_er(netwib_show_array_line_begin(pbuf));
  netwib_er(netwib_show_array_text4("version", pbuf));
  netwib_er(netwib_show_array_text8("traffic class", pbuf));
  netwib_er(netwib_show_array_text(20, "flow label", pbuf));
  netwib_er(netwib_show_array_line_end(pbuf));
  netwib_er(netwib_show_array_line_begin(pbuf));
  netwib_er(netwib_show_array_num4(6, NETWIB_SHOW_ARRAY_NUMTYPE_DEC, pbuf));
  netwib_er(netwib_show_array_num8(piphdr->header.ip6.trafficclass,
                                   NETWIB_SHOW_ARRAY_NUMTYPE_DEC, pbuf));
  netwib_er(netwib_show_array_num(20, piphdr->header.ip6.flowlabel,
                                  NETWIB_SHOW_ARRAY_NUMTYPE_DEC, pbuf));
  netwib_er(netwib_show_array_line_end(pbuf));
  /**/
  netwib_er(netwib_show_array_line_begin(pbuf));
  netwib_er(netwib_show_array_text16("payload length", pbuf));
  netwib_er(netwib_show_array_text8("next header", pbuf));
  netwib_er(netwib_show_array_text8("hop limit", pbuf));
  netwib_er(netwib_show_array_line_end(pbuf));
  netwib_er(netwib_show_array_line_begin(pbuf));
  netwib_er(netwib_show_array_num16(piphdr->header.ip6.payloadlength,
                                    NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                    pbuf));
  netwib_er(netwib_show_array_num8(piphdr->protocol,
                                   NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                   pbuf));
  netwib_er(netwib_show_array_num8(piphdr->ttl,
                                   NETWIB_SHOW_ARRAY_NUMTYPE_DEC, pbuf));
  netwib_er(netwib_show_array_line_end(pbuf));
  /**/
  netwib_er(netwib_show_array_text32("source", pbuf));
  netwib_er(netwib_show_array_fmt(32, NETWIB_SHOW_ARRAY_ALIGN_CENTER, '_',
                                  pbuf, "%{ip}", &piphdr->src));
  /**/
  netwib_er(netwib_show_array_text32("destination", pbuf));
  netwib_er(netwib_show_array_fmt(32, NETWIB_SHOW_ARRAY_ALIGN_CENTER, '_',
                                  pbuf, "%{ip}", &piphdr->dst));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_iphdr_show(netwib_constiphdr *piphdr,
                             netwib_encodetype encodetype,
                             netwib_buf *pbuf)
{
  netwib_encodetype_context context;
  netwib_buf buf;

  switch(encodetype) {
  case NETWIB_ENCODETYPE_SYNTH :
    netwib_er(netwib_buf_append_fmt(pbuf, "%{ip}->%{ip}",
                                    &piphdr->src, &piphdr->dst));
    break;
  case NETWIB_ENCODETYPE_ARRAY :
    switch(piphdr->iptype) {
    case NETWIB_IPTYPE_IP4 :
      netwib_er(netwib_show_ip4hdr(piphdr, pbuf));
      break;
    case NETWIB_IPTYPE_IP6 :
      netwib_er(netwib_show_ip6hdr(piphdr, pbuf));
      break;
    default :
      return(NETWIB_ERR_PAIPTYPE);
    }
    break;
  default :
    netwib_er(netwib_buf_init_mallocdefault(&buf));
    netwib_er(netwib_pkt_append_iphdr(piphdr, &buf));
    netwib_er(netwib_buf_encode(&buf, encodetype, pbuf));
    netwib_er(netwib_buf_close(&buf));
    break;
  }

  switch(piphdr->iptype) {
  case NETWIB_IPTYPE_IP4 :
    if (netwib__buf_ref_data_size(&piphdr->header.ip4.opts)) {
      netwib_er(netwib_buf_encode_transition_init(&context));
      netwib_er(netwib_buf_encode_transition(&context, encodetype, pbuf));
      netwib_er(netwib_ip4opts_show(&piphdr->header.ip4.opts, encodetype,
                                    pbuf));
    }
    break;
  case NETWIB_IPTYPE_IP6 :
    if (netwib__buf_ref_data_size(&piphdr->header.ip6.exts)) {
      netwib_er(netwib_buf_encode_transition_init(&context));
      netwib_er(netwib_buf_encode_transition(&context, encodetype, pbuf));
      netwib_er(netwib_buf_encode_transition(&context, encodetype, pbuf));
      netwib_er(netwib_ip6exts_show(piphdr->protocol,
                                    &piphdr->header.ip6.exts, encodetype,
                                    pbuf));
    }
    break;
  default :
    return(NETWIB_ERR_PAIPTYPE);
  }

  return(NETWIB_ERR_OK);
}
