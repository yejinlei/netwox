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
netwib_err netwib_udphdr_show(netwib_constudphdr *pudphdr,
                              netwib_encodetype encodetype,
                              netwib_buf *pbuf)
{
  netwib_buf buf;

  switch(encodetype) {
    case NETWIB_ENCODETYPE_SYNTH :
      netwib_er(netwib_buf_append_fmt(pbuf, "udp%{port}->%{port}",
                                      pudphdr->src, pudphdr->dst));
      break;
    case NETWIB_ENCODETYPE_ARRAY :
      netwib_er(netwib_show_array_head("UDP", pbuf));
      netwib_er(netwib_show_array_line_begin(pbuf));
      netwib_er(netwib_show_array_text16("source port", pbuf));
      netwib_er(netwib_show_array_text16("destination port", pbuf));
      netwib_er(netwib_show_array_line_end(pbuf));
      netwib_er(netwib_show_array_line_begin(pbuf));
      netwib_er(netwib_show_array_num16((netwib_uint16)pudphdr->src,
                                        NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                        pbuf));
      netwib_er(netwib_show_array_num16((netwib_uint16)pudphdr->dst,
                                        NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                        pbuf));
      netwib_er(netwib_show_array_line_end(pbuf));
      netwib_er(netwib_show_array_line_begin(pbuf));
      netwib_er(netwib_show_array_text16("length", pbuf));
      netwib_er(netwib_show_array_text16("checksum", pbuf));
      netwib_er(netwib_show_array_line_end(pbuf));
      netwib_er(netwib_show_array_line_begin(pbuf));
      netwib_er(netwib_show_array_num16(pudphdr->len,
                                        NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                        pbuf));
      netwib_er(netwib_show_array_num16(pudphdr->check,
                                        NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                        pbuf));
      netwib_er(netwib_show_array_line_end(pbuf));
      break;
    default :
      netwib_er(netwib_buf_init_mallocdefault(&buf));
      netwib_er(netwib_pkt_append_udphdr(pudphdr, &buf));
      netwib_er(netwib_buf_encode(&buf, encodetype, pbuf));
      netwib_er(netwib_buf_close(&buf));
      break;
  }

  return(NETWIB_ERR_OK);
}
