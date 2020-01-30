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
static netwib_err netwib_priv_icmp4_show(netwib_consticmp4 *picmp4,
                                         netwib_buf *pbuf)
{

  switch(picmp4->type) {
  case NETWIB_ICMP4TYPE_ECHOREP :
  case NETWIB_ICMP4TYPE_ECHOREQ :
    netwib_er(netwib_show_array_line_begin(pbuf));
    netwib_er(netwib_show_array_text16("id", pbuf));
    netwib_er(netwib_show_array_text16("seqnum", pbuf));
    netwib_er(netwib_show_array_line_end(pbuf));
    netwib_er(netwib_show_array_line_begin(pbuf));
    netwib_er(netwib_show_array_num16(picmp4->msg.echo.id,
                                      NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                      pbuf));
    netwib_er(netwib_show_array_num16(picmp4->msg.echo.seqnum,
                                      NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                      pbuf));
    netwib_er(netwib_show_array_line_end(pbuf));
    netwib_er(netwib_show_array_data(" data",
                                     &picmp4->msg.echo.data,
                                     NETWIB_ENCODETYPE_HEXA0, ' ',
                                     pbuf));
    netwib_er(netwib_show_array_tail(pbuf));
    break;
  case NETWIB_ICMP4TYPE_DSTUNREACH :
    netwib_er(netwib_show_array_text32("reserved", pbuf));
    netwib_er(netwib_show_array_num32(picmp4->msg.dstunreach.reserved,
                                      NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                      pbuf));
    netwib_er(netwib_show_array_text32("bad IP packet :", pbuf));
    netwib_er(netwib_pkt_ip_show(&picmp4->msg.dstunreach.badippacket,
                                 NULL, NETWIB_ENCODETYPE_ARRAY,
                                 NETWIB_ENCODETYPE_DUMP, pbuf));
    break;
  case NETWIB_ICMP4TYPE_SRCQUENCH :
    netwib_er(netwib_show_array_text32("reserved", pbuf));
    netwib_er(netwib_show_array_num32(picmp4->msg.srcquench.reserved,
                                      NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                      pbuf));
    netwib_er(netwib_show_array_text32("bad IP packet :", pbuf));
    netwib_er(netwib_pkt_ip_show(&picmp4->msg.srcquench.badippacket,
                                 NULL, NETWIB_ENCODETYPE_ARRAY,
                                 NETWIB_ENCODETYPE_DUMP, pbuf));
    break;
  case NETWIB_ICMP4TYPE_REDIRECT :
    netwib_er(netwib_show_array_text32("gateway", pbuf));
    netwib_er(netwib_show_array_fmt(32, NETWIB_SHOW_ARRAY_ALIGN_CENTER, '_',
                                    pbuf, "%{ip}",
                                    &picmp4->msg.redirect.gw));
    netwib_er(netwib_show_array_text32("bad IP packet :", pbuf));
    netwib_er(netwib_pkt_ip_show(&picmp4->msg.redirect.badippacket,
                                 NULL, NETWIB_ENCODETYPE_ARRAY,
                                 NETWIB_ENCODETYPE_DUMP, pbuf));
    break;
  case NETWIB_ICMP4TYPE_TIMEEXCEED :
    netwib_er(netwib_show_array_text32("reserved", pbuf));
    netwib_er(netwib_show_array_num32(picmp4->msg.timeexceed.reserved,
                                      NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                      pbuf));
    netwib_er(netwib_show_array_text32("bad IP packet :", pbuf));
    netwib_er(netwib_pkt_ip_show(&picmp4->msg.timeexceed.badippacket,
                                 NULL, NETWIB_ENCODETYPE_ARRAY,
                                 NETWIB_ENCODETYPE_DUMP, pbuf));
    break;
  case NETWIB_ICMP4TYPE_PARAPROB :
    netwib_er(netwib_show_array_line_begin(pbuf));
    netwib_er(netwib_show_array_text8("pointer", pbuf));
    netwib_er(netwib_show_array_text(24, "reserved", pbuf));
    netwib_er(netwib_show_array_line_end(pbuf));
    netwib_er(netwib_show_array_line_begin(pbuf));
    netwib_er(netwib_show_array_num8(picmp4->msg.paraprob.pointer,
                                     NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                     pbuf));
    netwib_er(netwib_show_array_num(24, picmp4->msg.paraprob.reserved,
                                    NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                    pbuf));
    netwib_er(netwib_show_array_line_end(pbuf));
    netwib_er(netwib_show_array_text32("bad IP packet :", pbuf));
    netwib_er(netwib_pkt_ip_show(&picmp4->msg.paraprob.badippacket,
                                 NULL, NETWIB_ENCODETYPE_ARRAY,
                                 NETWIB_ENCODETYPE_DUMP, pbuf));
    break;
  case NETWIB_ICMP4TYPE_TIMESTAMPREQ :
  case NETWIB_ICMP4TYPE_TIMESTAMPREP :
    netwib_er(netwib_show_array_line_begin(pbuf));
    netwib_er(netwib_show_array_text16("id", pbuf));
    netwib_er(netwib_show_array_text16("seqnum", pbuf));
    netwib_er(netwib_show_array_line_end(pbuf));
    netwib_er(netwib_show_array_line_begin(pbuf));
    netwib_er(netwib_show_array_num16(picmp4->msg.timestamp.id,
                                      NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                      pbuf));
    netwib_er(netwib_show_array_num16(picmp4->msg.timestamp.seqnum,
                                      NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                      pbuf));
    netwib_er(netwib_show_array_line_end(pbuf));
    netwib_er(netwib_show_array_text32("originate timestamp", pbuf));
    netwib_er(netwib_show_array_num32(picmp4->msg.timestamp.originatetimestamp,
                                      NETWIB_SHOW_ARRAY_NUMTYPE_DEC, pbuf));
    netwib_er(netwib_show_array_text32("receive timestamp", pbuf));
    netwib_er(netwib_show_array_num32(picmp4->msg.timestamp.receivetimestamp,
                                      NETWIB_SHOW_ARRAY_NUMTYPE_DEC, pbuf));
    netwib_er(netwib_show_array_text32("transmit timestamp", pbuf));
    netwib_er(netwib_show_array_num32(picmp4->msg.timestamp.transmittimestamp,
                                      NETWIB_SHOW_ARRAY_NUMTYPE_DEC, pbuf));
    break;
  case NETWIB_ICMP4TYPE_INFOREQ :
  case NETWIB_ICMP4TYPE_INFOREP :
    netwib_er(netwib_show_array_line_begin(pbuf));
    netwib_er(netwib_show_array_text16("id", pbuf));
    netwib_er(netwib_show_array_text16("seqnum", pbuf));
    netwib_er(netwib_show_array_line_end(pbuf));
    netwib_er(netwib_show_array_line_begin(pbuf));
    netwib_er(netwib_show_array_num16(picmp4->msg.info.id,
                                      NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                      pbuf));
    netwib_er(netwib_show_array_num16(picmp4->msg.info.seqnum,
                                      NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                      pbuf));
    netwib_er(netwib_show_array_line_end(pbuf));
    netwib_er(netwib_show_array_data(" data",
                                     &picmp4->msg.info.data,
                                     NETWIB_ENCODETYPE_HEXA0, ' ',
                                     pbuf));
    netwib_er(netwib_show_array_tail(pbuf));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_icmp4_show(netwib_consticmp4 *picmp4,
                             netwib_encodetype encodetype,
                             netwib_buf *pbuf)
{
  netwib_buf buf;
  netwib_byte array[80];
  netwib_string str;

  switch(encodetype) {
    case NETWIB_ENCODETYPE_SYNTH :
      netwib_er(netwib_buf_append_fmt(pbuf, "icmp4:%{uint32}:%{uint32}",
                                      picmp4->type, picmp4->code));
      break;
    case NETWIB_ENCODETYPE_ARRAY :
      netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));
      netwib_er(netwib_buf_append_string("ICMP4_", &buf));
      netwib_er(netwib_buf_append_icmp4type(picmp4->type, &buf));
      netwib_er(netwib_buf_append_string("_", &buf));
      netwib_er(netwib_buf_append_icmp4code(picmp4->type, picmp4->code,
                                            &buf));
      netwib_er(netwib_buf_ref_string(&buf, &str));
      netwib_er(netwib_show_array_head(str, pbuf));
      netwib_er(netwib_show_array_line_begin(pbuf));
      netwib_er(netwib_show_array_text8("type", pbuf));
      netwib_er(netwib_show_array_text8("code", pbuf));
      netwib_er(netwib_show_array_text16("checksum", pbuf));
      netwib_er(netwib_show_array_line_end(pbuf));
      netwib_er(netwib_show_array_line_begin(pbuf));
      netwib_er(netwib_show_array_num8((netwib_uint8)picmp4->type,
                                       NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                       pbuf));
      netwib_er(netwib_show_array_num8((netwib_uint8)picmp4->code,
                                       NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                       pbuf));
      netwib_er(netwib_show_array_num16(picmp4->check,
                                        NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                        pbuf));
      netwib_er(netwib_show_array_line_end(pbuf));
      netwib_er(netwib_priv_icmp4_show(picmp4, pbuf));
      break;
    default :
      netwib_er(netwib_buf_init_mallocdefault(&buf));
      netwib_er(netwib_pkt_append_icmp4(picmp4, &buf));
      netwib_er(netwib_buf_encode(&buf, encodetype, pbuf));
      netwib_er(netwib_buf_close(&buf));
      break;
  }

  return(NETWIB_ERR_OK);
}
