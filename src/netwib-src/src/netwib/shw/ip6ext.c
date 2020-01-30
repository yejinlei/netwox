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
static netwib_err netwib_ip6ext_show_hopopts(netwib_constip6ext *pip6ext,
                                             netwib_encodetype encodetype,
                                             netwib_buf *pbuf)
{
  netwib_buf buf;
  netwib_uint32 hdrextlen;

  switch(encodetype) {
    case NETWIB_ENCODETYPE_SYNTH :
      netwib_er(netwib_buf_append_string("hopopts", pbuf));
      break;
    case NETWIB_ENCODETYPE_ARRAY :
      netwib_er(netwib_show_array_head("IP6 Hop-by-Hop options", pbuf));
      netwib_er(netwib_show_array_line_begin(pbuf));
      netwib_er(netwib_show_array_text8("next header", pbuf));
      netwib_er(netwib_show_array_text8("hdr ext len", pbuf));
      netwib_er(netwib_show_array_text16("", pbuf));
      netwib_er(netwib_show_array_line_end(pbuf));
      netwib_er(netwib_show_array_line_begin(pbuf));
      netwib_er(netwib_show_array_num8(pip6ext->nextproto,
                                       NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                       pbuf));
      hdrextlen = 4 + netwib__buf_ref_data_size(&pip6ext->ext.hopopts.options);
      hdrextlen = (hdrextlen+7)/8;
      netwib_er(netwib_show_array_num8(hdrextlen,
                                       NETWIB_SHOW_ARRAY_NUMTYPE_DEC, pbuf));
      netwib_er(netwib_show_array_text16("", pbuf));
      netwib_er(netwib_show_array_line_end(pbuf));
      netwib_er(netwib_show_array_data(" options",
                                       &pip6ext->ext.hopopts.options,
                                       NETWIB_ENCODETYPE_HEXA0, ' ',
                                       pbuf));
      netwib_er(netwib_show_array_tail(pbuf));
      break;
    default :
      netwib_er(netwib_buf_init_mallocdefault(&buf));
      netwib_er(netwib_pkt_append_ip6ext(pip6ext, &buf));
      netwib_er(netwib_buf_encode(&buf, encodetype, pbuf));
      netwib_er(netwib_buf_close(&buf));
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ip6ext_show_dstopts(netwib_constip6ext *pip6ext,
                                             netwib_encodetype encodetype,
                                             netwib_buf *pbuf)
{
  netwib_buf buf;
  netwib_uint32 hdrextlen;

  switch(encodetype) {
    case NETWIB_ENCODETYPE_SYNTH :
      netwib_er(netwib_buf_append_string("dstopts", pbuf));
      break;
    case NETWIB_ENCODETYPE_ARRAY :
      netwib_er(netwib_show_array_head("IP6 Destination options", pbuf));
      netwib_er(netwib_show_array_line_begin(pbuf));
      netwib_er(netwib_show_array_text8("next header", pbuf));
      netwib_er(netwib_show_array_text8("hdr ext len", pbuf));
      netwib_er(netwib_show_array_text16("", pbuf));
      netwib_er(netwib_show_array_line_end(pbuf));
      netwib_er(netwib_show_array_line_begin(pbuf));
      netwib_er(netwib_show_array_num8(pip6ext->nextproto,
                                       NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                       pbuf));
      hdrextlen = 4 + netwib__buf_ref_data_size(&pip6ext->ext.dstopts.options);
      hdrextlen = (hdrextlen+7)/8;
      netwib_er(netwib_show_array_num8(hdrextlen,
                                       NETWIB_SHOW_ARRAY_NUMTYPE_DEC, pbuf));
      netwib_er(netwib_show_array_text16("", pbuf));
      netwib_er(netwib_show_array_line_end(pbuf));
      netwib_er(netwib_show_array_data(" options",
                                       &pip6ext->ext.dstopts.options,
                                       NETWIB_ENCODETYPE_HEXA0, ' ',
                                       pbuf));
      netwib_er(netwib_show_array_tail(pbuf));
      break;
    default :
      netwib_er(netwib_buf_init_mallocdefault(&buf));
      netwib_er(netwib_pkt_append_ip6ext(pip6ext, &buf));
      netwib_er(netwib_buf_encode(&buf, encodetype, pbuf));
      netwib_er(netwib_buf_close(&buf));
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ip6ext_show_routing(netwib_constip6ext *pip6ext,
                                             netwib_encodetype encodetype,
                                             netwib_buf *pbuf)
{
  netwib_buf buf;
  netwib_uint32 hdrextlen;

  switch(encodetype) {
    case NETWIB_ENCODETYPE_SYNTH :
      netwib_er(netwib_buf_append_string("routing", pbuf));
      break;
    case NETWIB_ENCODETYPE_ARRAY :
      netwib_er(netwib_show_array_head("IP6 Routing", pbuf));
      netwib_er(netwib_show_array_line_begin(pbuf));
      netwib_er(netwib_show_array_text8("next header", pbuf));
      netwib_er(netwib_show_array_text8("hdr ext len", pbuf));
      netwib_er(netwib_show_array_text8("routing type", pbuf));
      netwib_er(netwib_show_array_text8("segments left", pbuf));
      netwib_er(netwib_show_array_line_end(pbuf));
      netwib_er(netwib_show_array_line_begin(pbuf));
      netwib_er(netwib_show_array_num8(pip6ext->nextproto,
                                       NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                       pbuf));
      hdrextlen = (4+netwib__buf_ref_data_size(&pip6ext->ext.routing.data))/8;
      netwib_er(netwib_show_array_num8(hdrextlen,
                                       NETWIB_SHOW_ARRAY_NUMTYPE_DEC, pbuf));
      netwib_er(netwib_show_array_num8(pip6ext->ext.routing.routingtype,
                                       NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                       pbuf));
      netwib_er(netwib_show_array_num8(pip6ext->ext.routing.segmentsleft,
                                       NETWIB_SHOW_ARRAY_NUMTYPE_DEC, pbuf));
      netwib_er(netwib_show_array_line_end(pbuf));
      netwib_er(netwib_show_array_data(" data",
                                       &pip6ext->ext.routing.data,
                                       NETWIB_ENCODETYPE_HEXA0, ' ',
                                       pbuf));
      netwib_er(netwib_show_array_tail(pbuf));
      break;
    default :
      netwib_er(netwib_buf_init_mallocdefault(&buf));
      netwib_er(netwib_pkt_append_ip6ext(pip6ext, &buf));
      netwib_er(netwib_buf_encode(&buf, encodetype, pbuf));
      netwib_er(netwib_buf_close(&buf));
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ip6ext_show_fragment(netwib_constip6ext *pip6ext,
                                              netwib_encodetype encodetype,
                                              netwib_buf *pbuf)
{
  netwib_buf buf;

  switch(encodetype) {
    case NETWIB_ENCODETYPE_SYNTH :
      netwib_er(netwib_buf_append_string("fragment", pbuf));
      break;
    case NETWIB_ENCODETYPE_ARRAY :
      netwib_er(netwib_show_array_head("IP6 Fragment", pbuf));
      /**/
      netwib_er(netwib_show_array_line_begin(pbuf));
      netwib_er(netwib_show_array_text8("next header", pbuf));
      netwib_er(netwib_show_array_text8("reserved", pbuf));
      netwib_er(netwib_show_array_text(13, "fragment offset", pbuf));
      netwib_er(netwib_show_array_text1("r", pbuf));
      netwib_er(netwib_show_array_text1("r", pbuf));
      netwib_er(netwib_show_array_text1("M", pbuf));
      netwib_er(netwib_show_array_line_end(pbuf));
      netwib_er(netwib_show_array_line_begin(pbuf));
      netwib_er(netwib_show_array_num8(pip6ext->nextproto,
                                       NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                       pbuf));
      netwib_er(netwib_show_array_num8(0, NETWIB_SHOW_ARRAY_NUMTYPE_DEC,
                                       pbuf));
      netwib_er(netwib_show_array_num(13, pip6ext->ext.fragment.fragmentoffset,
                                      NETWIB_SHOW_ARRAY_NUMTYPE_DEC, pbuf));
      netwib_er(netwib_show_array_num1(pip6ext->ext.fragment.reservedb1,
                                       pbuf));
      netwib_er(netwib_show_array_num1(pip6ext->ext.fragment.reservedb2,
                                       pbuf));
      netwib_er(netwib_show_array_num1(pip6ext->ext.fragment.morefrag, pbuf));
      netwib_er(netwib_show_array_line_end(pbuf));
      /**/
      netwib_er(netwib_show_array_text32("identification", pbuf));
      netwib_er(netwib_show_array_num32(pip6ext->ext.fragment.id,
                                        NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                        pbuf));
      break;
    default :
      netwib_er(netwib_buf_init_mallocdefault(&buf));
      netwib_er(netwib_pkt_append_ip6ext(pip6ext, &buf));
      netwib_er(netwib_buf_encode(&buf, encodetype, pbuf));
      netwib_er(netwib_buf_close(&buf));
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ip6ext_show_ah(netwib_constip6ext *pip6ext,
                                        netwib_encodetype encodetype,
                                        netwib_buf *pbuf)
{
  netwib_buf buf;
  netwib_uint32 payloadlen;

  switch(encodetype) {
    case NETWIB_ENCODETYPE_SYNTH :
      netwib_er(netwib_buf_append_string("ah", pbuf));
      break;
    case NETWIB_ENCODETYPE_ARRAY :
      netwib_er(netwib_show_array_head("IP6 Authentication Header", pbuf));
      netwib_er(netwib_show_array_line_begin(pbuf));
      netwib_er(netwib_show_array_text8("next header", pbuf));
      netwib_er(netwib_show_array_text8("payload len", pbuf));
      netwib_er(netwib_show_array_text16("reserved", pbuf));
      netwib_er(netwib_show_array_line_end(pbuf));
      netwib_er(netwib_show_array_line_begin(pbuf));
      netwib_er(netwib_show_array_num8(pip6ext->nextproto,
                                       NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                       pbuf));
      payloadlen = (12+netwib__buf_ref_data_size(&pip6ext->ext.ah.data))/4 - 2;
      netwib_er(netwib_show_array_num8(payloadlen,
                                       NETWIB_SHOW_ARRAY_NUMTYPE_DEC, pbuf));
      netwib_er(netwib_show_array_num16(pip6ext->ext.ah.reserved,
                                        NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                        pbuf));
      netwib_er(netwib_show_array_line_end(pbuf));
      netwib_er(netwib_show_array_text32("security parameters index", pbuf));
      netwib_er(netwib_show_array_num32(pip6ext->ext.ah.spi,
                                        NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                        pbuf));
      netwib_er(netwib_show_array_text32("sequence number", pbuf));
      netwib_er(netwib_show_array_num32(pip6ext->ext.ah.seqnum,
                                        NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                        pbuf));
      netwib_er(netwib_show_array_data(" data", &pip6ext->ext.ah.data,
                                       NETWIB_ENCODETYPE_HEXA0, ' ', pbuf));
      netwib_er(netwib_show_array_tail(pbuf));
      break;
    default :
      netwib_er(netwib_buf_init_mallocdefault(&buf));
      netwib_er(netwib_pkt_append_ip6ext(pip6ext, &buf));
      netwib_er(netwib_buf_encode(&buf, encodetype, pbuf));
      netwib_er(netwib_buf_close(&buf));
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ip6ext_show(netwib_constip6ext *pip6ext,
                              netwib_encodetype encodetype,
                              netwib_buf *pbuf)
{

  switch(pip6ext->proto) {
  case NETWIB_IPPROTO_HOPOPTS :
    netwib_er(netwib_ip6ext_show_hopopts(pip6ext, encodetype, pbuf));
    break;
  case NETWIB_IPPROTO_DSTOPTS :
    netwib_er(netwib_ip6ext_show_dstopts(pip6ext, encodetype, pbuf));
    break;
  case NETWIB_IPPROTO_ROUTING :
    netwib_er(netwib_ip6ext_show_routing(pip6ext, encodetype, pbuf));
    break;
  case NETWIB_IPPROTO_FRAGMENT :
    netwib_er(netwib_ip6ext_show_fragment(pip6ext, encodetype, pbuf));
    break;
  case NETWIB_IPPROTO_AH :
    netwib_er(netwib_ip6ext_show_ah(pip6ext, encodetype, pbuf));
    break;
  default :
    return(NETWIB_ERR_LONOTIMPLEMENTED);
  }

  return(NETWIB_ERR_OK);
}
