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
netwib_err netwib_arphdr_show(netwib_constarphdr *parphdr,
                              netwib_encodetype encodetype,
                              netwib_buf *pbuf)
{
  netwib_buf buf;

  switch(encodetype) {
    case NETWIB_ENCODETYPE_SYNTH :
      switch(parphdr->op) {
        case NETWIB_ARPHDROP_ARPREQ :
          netwib_er(netwib_buf_append_fmt(pbuf, "arpQ_%{ip}_asks_%{ip}",
                                          &parphdr->ipsrc, &parphdr->ipdst));
          break;
        case NETWIB_ARPHDROP_ARPREP :
          netwib_er(netwib_buf_append_fmt(pbuf, "arpR_%{ip}_at_%{eth}",
                                          &parphdr->ipsrc, &parphdr->ethsrc));
          break;
        case NETWIB_ARPHDROP_RARPREQ :
          netwib_er(netwib_buf_append_fmt(pbuf, "rarpQ"));
          break;
        case NETWIB_ARPHDROP_RARPREP :
          netwib_er(netwib_buf_append_fmt(pbuf, "rarpR"));
          break;
        default :
          netwib_er(netwib_buf_append_string("arp??", pbuf));
          break;
      }
      break;
    case NETWIB_ENCODETYPE_ARRAY :
      switch(parphdr->op) {
        case NETWIB_ARPHDROP_ARPREQ :
          netwib_er(netwib_show_array_head("ARP Request", pbuf));
          netwib_er(netwib_show_array_fmt32(pbuf,
                                            " this address : %{eth} %{ip}",
                                            &parphdr->ethsrc,
                                            &parphdr->ipsrc));
          netwib_er(netwib_show_array_fmt32(pbuf,
                                            " asks         : %{eth} %{ip}",
                                            &parphdr->ethdst,
                                            &parphdr->ipdst));
          break;
        case NETWIB_ARPHDROP_ARPREP :
          netwib_er(netwib_show_array_head("ARP Reply", pbuf));
          netwib_er(netwib_show_array_fmt32(pbuf,
                                            " this answer : %{eth} %{ip}",
                                            &parphdr->ethsrc,
                                            &parphdr->ipsrc));
          netwib_er(netwib_show_array_fmt32(pbuf,
                                            " is for      : %{eth} %{ip}",
                                            &parphdr->ethdst,
                                            &parphdr->ipdst));
          break;
        case NETWIB_ARPHDROP_RARPREQ :
          netwib_er(netwib_show_array_head("RARP Request", pbuf));
          netwib_er(netwib_show_array_fmt32(pbuf,
                                            " this address : %{eth} %{ip}",
                                            &parphdr->ethsrc,
                                            &parphdr->ipsrc));
          netwib_er(netwib_show_array_fmt32(pbuf,
                                            " asks         : %{eth} %{ip}",
                                            &parphdr->ethdst,
                                            &parphdr->ipdst));
          break;
        case NETWIB_ARPHDROP_RARPREP :
          netwib_er(netwib_show_array_head("RARP Reply", pbuf));
          netwib_er(netwib_show_array_fmt32(pbuf,
                                            " this address : %{eth} %{ip}",
                                            &parphdr->ethsrc,
                                            &parphdr->ipsrc));
          netwib_er(netwib_show_array_fmt32(pbuf,
                                            " answered     : %{eth} %{ip}",
                                            &parphdr->ethdst,
                                            &parphdr->ipdst));
          break;
        default :
          netwib_er(netwib_show_array_head("ARP??", pbuf));
          netwib_er(netwib_show_array_fmt32(pbuf,
                                            " src : %{eth} %{ip}",
                                            &parphdr->ethsrc,
                                            &parphdr->ipsrc));
          netwib_er(netwib_show_array_fmt32(pbuf,
                                            " dst : %{eth} %{ip}",
                                            &parphdr->ethdst,
                                            &parphdr->ipdst));
          break;
      }
      netwib_er(netwib_show_array_tail(pbuf));
      break;
    default :
      netwib_er(netwib_buf_init_mallocdefault(&buf));
      netwib_er(netwib_pkt_append_arphdr(parphdr, &buf));
      netwib_er(netwib_buf_encode(&buf, encodetype, pbuf));
      netwib_er(netwib_buf_close(&buf));
      break;
  }

  return(NETWIB_ERR_OK);
}
