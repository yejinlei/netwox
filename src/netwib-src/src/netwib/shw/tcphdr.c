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
netwib_err netwib_tcphdr_show(netwib_consttcphdr *ptcphdr,
                              netwib_encodetype encodetype,
                              netwib_buf *pbuf)
{
  netwib_char flags[9], *pc;
  netwib_buf buf;
  netwib_encodetype_context context;

  switch(encodetype) {
    case NETWIB_ENCODETYPE_SYNTH :
      pc = flags;
      if (ptcphdr->syn) {
        *pc++ = 's';
      }
      if (ptcphdr->ack) {
        *pc++ = 'a';
      }
      if (ptcphdr->fin) {
        *pc++ = 'f';
      }
      if (ptcphdr->rst) {
        *pc++ = 'r';
      }
      if (ptcphdr->psh) {
        *pc++ = 'p';
      }
      if (ptcphdr->urg) {
        *pc++ = 'u';
      }
      if (ptcphdr->cwr) {
        *pc++ = 'c';
      }
      if (ptcphdr->ece) {
        *pc++ = 'e';
      }
      *pc = '\0';
      netwib_er(netwib_buf_append_fmt(pbuf, "tcp%{port}->%{port}:%s",
                                    ptcphdr->src, ptcphdr->dst, flags));
      break;
    case NETWIB_ENCODETYPE_ARRAY :
      netwib_er(netwib_show_array_head("TCP", pbuf));
      netwib_er(netwib_show_array_line_begin(pbuf));
      netwib_er(netwib_show_array_text16("source port", pbuf));
      netwib_er(netwib_show_array_text16("destination port", pbuf));
      netwib_er(netwib_show_array_line_end(pbuf));
      netwib_er(netwib_show_array_line_begin(pbuf));
      netwib_er(netwib_show_array_num16((netwib_uint16)ptcphdr->src,
                                        NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                        pbuf));
      netwib_er(netwib_show_array_num16((netwib_uint16)ptcphdr->dst,
                                        NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                        pbuf));
      netwib_er(netwib_show_array_line_end(pbuf));
      netwib_er(netwib_show_array_text32("seqnum", pbuf));
      netwib_er(netwib_show_array_num32(ptcphdr->seqnum,
                                        NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                        pbuf));
      netwib_er(netwib_show_array_text32("acknum", pbuf));
      netwib_er(netwib_show_array_num32(ptcphdr->acknum,
                                        NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                        pbuf));
      netwib_er(netwib_show_array_line_begin(pbuf));
      netwib_er(netwib_show_array_text4("doff", pbuf));
      netwib_er(netwib_show_array_text1("r", pbuf));
      netwib_er(netwib_show_array_text1("r", pbuf));
      netwib_er(netwib_show_array_text1("r", pbuf));
      netwib_er(netwib_show_array_text1("r", pbuf));
      netwib_er(netwib_show_array_text1("C", pbuf));
      netwib_er(netwib_show_array_text1("E", pbuf));
      netwib_er(netwib_show_array_text1("U", pbuf));
      netwib_er(netwib_show_array_text1("A", pbuf));
      netwib_er(netwib_show_array_text1("P", pbuf));
      netwib_er(netwib_show_array_text1("R", pbuf));
      netwib_er(netwib_show_array_text1("S", pbuf));
      netwib_er(netwib_show_array_text1("F", pbuf));
      netwib_er(netwib_show_array_text16("window", pbuf));
      netwib_er(netwib_show_array_line_end(pbuf));
      netwib_er(netwib_show_array_line_begin(pbuf));
      netwib_er(netwib_show_array_num4(ptcphdr->doff,
                                       NETWIB_SHOW_ARRAY_NUMTYPE_DEC, pbuf));
      netwib_er(netwib_show_array_num1(ptcphdr->reserved1, pbuf));
      netwib_er(netwib_show_array_num1(ptcphdr->reserved2, pbuf));
      netwib_er(netwib_show_array_num1(ptcphdr->reserved3, pbuf));
      netwib_er(netwib_show_array_num1(ptcphdr->reserved4, pbuf));
      netwib_er(netwib_show_array_num1(ptcphdr->cwr, pbuf));
      netwib_er(netwib_show_array_num1(ptcphdr->ece, pbuf));
      netwib_er(netwib_show_array_num1(ptcphdr->urg, pbuf));
      netwib_er(netwib_show_array_num1(ptcphdr->ack, pbuf));
      netwib_er(netwib_show_array_num1(ptcphdr->psh, pbuf));
      netwib_er(netwib_show_array_num1(ptcphdr->rst, pbuf));
      netwib_er(netwib_show_array_num1(ptcphdr->syn, pbuf));
      netwib_er(netwib_show_array_num1(ptcphdr->fin, pbuf));
      netwib_er(netwib_show_array_num16(ptcphdr->window,
                                        NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                        pbuf));
      netwib_er(netwib_show_array_line_end(pbuf));
      netwib_er(netwib_show_array_line_begin(pbuf));
      netwib_er(netwib_show_array_text16("checksum", pbuf));
      netwib_er(netwib_show_array_text16("urgptr", pbuf));
      netwib_er(netwib_show_array_line_end(pbuf));
      netwib_er(netwib_show_array_line_begin(pbuf));
      netwib_er(netwib_show_array_num16(ptcphdr->check,
                                        NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                        pbuf));
      netwib_er(netwib_show_array_num16(ptcphdr->urgptr,
                                        NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                        pbuf));
      netwib_er(netwib_show_array_line_end(pbuf));
      break;
    default :
      netwib_er(netwib_buf_init_mallocdefault(&buf));
      netwib_er(netwib_pkt_append_tcphdr(ptcphdr, &buf));
      netwib_er(netwib_buf_encode(&buf, encodetype, pbuf));
      netwib_er(netwib_buf_close(&buf));
      break;
  }

  if (netwib__buf_ref_data_size(&ptcphdr->opts)) {
    netwib_er(netwib_buf_encode_transition_init(&context));
    netwib_er(netwib_buf_encode_transition(&context, encodetype, pbuf));
    netwib_er(netwib_buf_encode_transition(&context, encodetype, pbuf));
    netwib_er(netwib_tcpopts_show(&ptcphdr->opts, encodetype, pbuf));
  }

  return(NETWIB_ERR_OK);
}
