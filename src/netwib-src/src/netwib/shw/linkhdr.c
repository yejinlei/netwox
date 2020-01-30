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
static netwib_err netwib_show_etherhdr(netwib_constetherhdr *petherhdr,
                                       netwib_encodetype encodetype,
                                       netwib_buf *pbuf)
{
  switch(encodetype) {
    case NETWIB_ENCODETYPE_SYNTH :
      netwib_er(netwib_buf_append_fmt(pbuf, "%{eth}->%{eth}",
                                      &petherhdr->src, &petherhdr->dst));
      break;
    case NETWIB_ENCODETYPE_ARRAY :
      netwib_er(netwib_show_array_head("Ethernet", pbuf));
      netwib_er(netwib_show_array_fmt32(pbuf,
                                        " %{eth}->%{eth} type:%{uint32:#04X}",
                                        &petherhdr->src, &petherhdr->dst,
                                        petherhdr->type));
      netwib_er(netwib_show_array_tail(pbuf));
      break;
    default :
      return(NETWIB_ERR_LOINTERNALERROR);
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_show_nullhdr(netwib_constnullhdr *pnullhdr,
                                      netwib_encodetype encodetype,
                                      netwib_buf *pbuf)
{
  switch(encodetype) {
    case NETWIB_ENCODETYPE_SYNTH :
      netwib_er(netwib_buf_append_string("null", pbuf));
      break;
    case NETWIB_ENCODETYPE_ARRAY :
      netwib_er(netwib_show_array_head("Null", pbuf));
      netwib_er(netwib_show_array_fmt32(pbuf, " type:%{uint32:#08X}",
                                        pnullhdr->type));
      netwib_er(netwib_show_array_tail(pbuf));
      break;
    default :
      return(NETWIB_ERR_LOINTERNALERROR);
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_show_loophdr(netwib_constloophdr *ploophdr,
                                      netwib_encodetype encodetype,
                                      netwib_buf *pbuf)
{
  switch(encodetype) {
    case NETWIB_ENCODETYPE_SYNTH :
      netwib_er(netwib_buf_append_string("loop", pbuf));
      break;
    case NETWIB_ENCODETYPE_ARRAY :
      netwib_er(netwib_show_array_head("Loop", pbuf));
      netwib_er(netwib_show_array_fmt32(pbuf, " type:%{uint32:#08X}",
                                        ploophdr->type));
      netwib_er(netwib_show_array_tail(pbuf));
      break;
    default :
      return(NETWIB_ERR_LOINTERNALERROR);
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_show_ppphdr(netwib_constppphdr *pppphdr,
                                     netwib_encodetype encodetype,
                                     netwib_buf *pbuf)
{
  switch(encodetype) {
    case NETWIB_ENCODETYPE_SYNTH :
      netwib_er(netwib_buf_append_string("ppp", pbuf));
      break;
    case NETWIB_ENCODETYPE_ARRAY :
      netwib_er(netwib_show_array_head("Ppp", pbuf));
      netwib_er(netwib_show_array_fmt32(pbuf, " address:%{uint32:#02X} control:%{uint32:#02X} protocol:%{uint32:#04X}", pppphdr->address, pppphdr->control, pppphdr->protocol));
      netwib_er(netwib_show_array_tail(pbuf));
      break;
    default :
      return(NETWIB_ERR_LOINTERNALERROR);
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_show_linuxsllhdr(netwib_constlinuxsllhdr *plinuxsllhdr,
                                          netwib_encodetype encodetype,
                                          netwib_buf *pbuf)
{
  netwib_uint32 i, halen;
  netwib_byte array[80];
  netwib_buf buf;

  switch(encodetype) {
    case NETWIB_ENCODETYPE_SYNTH :
      netwib_er(netwib_buf_append_string("linuxsll", pbuf));
      break;
    case NETWIB_ENCODETYPE_ARRAY :
      netwib_er(netwib_show_array_head("Linux_SLL", pbuf));
      netwib_er(netwib_buf_init_ext_arrayempty(array, sizeof(array), &buf));
      netwib_er(netwib_buf_append_fmt(&buf,
                    "pkttype:%{uint32:#04X} hatype:%{uint32:#04X} srcaddr:",
                                      plinuxsllhdr->pkttype,
                                      plinuxsllhdr->hatype));
      halen = plinuxsllhdr->halen;
      if (halen > 8) halen = 8;
      for (i = 0; i < halen; i++) {
        netwib_er(netwib_buf_append_fmt(&buf, "%{uint32:02X}",
                                        plinuxsllhdr->srcaddr[i]));
      }
      netwib_er(netwib_show_array_fmt32(pbuf, " %{buf}", &buf));
      netwib_er(netwib_show_array_fmt32(pbuf, " protocol:%{uint32:#04X}",
                                        plinuxsllhdr->protocol));
      netwib_er(netwib_show_array_tail(pbuf));
      break;
    default :
      return(NETWIB_ERR_LOINTERNALERROR);
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_linkhdr_show(netwib_constlinkhdr *plinkhdr,
                               netwib_encodetype encodetype,
                               netwib_buf *pbuf)
{
  netwib_buf buf;

  switch(encodetype) {
    case NETWIB_ENCODETYPE_SYNTH :
    case NETWIB_ENCODETYPE_ARRAY :
      break;
    default:
      netwib_er(netwib_buf_init_mallocdefault(&buf));
      netwib_er(netwib_pkt_append_linkhdr(plinkhdr, &buf));
      netwib_er(netwib_buf_encode(&buf, encodetype, pbuf));
      netwib_er(netwib_buf_close(&buf));
      return(NETWIB_ERR_OK);
  }

  switch(plinkhdr->type) {
    case NETWIB_DEVICE_DLTTYPE_EN10MB :
      netwib_er(netwib_show_etherhdr(&plinkhdr->hdr.ether, encodetype,
                                           pbuf));
      break;
    case NETWIB_DEVICE_DLTTYPE_NULL :
      netwib_er(netwib_show_nullhdr(&plinkhdr->hdr.null, encodetype,
                                          pbuf));
      break;
    case NETWIB_DEVICE_DLTTYPE_LOOP :
      netwib_er(netwib_show_loophdr(&plinkhdr->hdr.loop, encodetype,
                                          pbuf));
      break;
    case NETWIB_DEVICE_DLTTYPE_RAW :
    case NETWIB_DEVICE_DLTTYPE_RAW4 :
    case NETWIB_DEVICE_DLTTYPE_RAW6 :
      /* append nothing */
      break;
    case NETWIB_DEVICE_DLTTYPE_PPP :
      netwib_er(netwib_show_ppphdr(&plinkhdr->hdr.ppp, encodetype,
                                         pbuf));
      break;
    case NETWIB_DEVICE_DLTTYPE_LINUX_SLL :
      netwib_er(netwib_show_linuxsllhdr(&plinkhdr->hdr.linuxsll,
                                              encodetype, pbuf));
      break;
    default :
      return(NETWIB_ERR_LONOTIMPLEMENTED);
      break;
  }
  return(NETWIB_ERR_OK);
}
