/*
                                  NETWOX
                             Network toolbox
                Copyright(c) 1999-2012 Laurent CONSTANTIN
                      http://ntwox.sourceforge.net/
                        laurentconstantin@free.fr
                                  -----

  This file is part of Netwox.

  Netwox is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3
  as published by the Free Software Foundation.

  Netwox is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details (http://www.gnu.org/).

------------------------------------------------------------------------
*/

/*-------------------------------------------------------------*/
#include "../netwox.h"

/*-------------------------------------------------------------*/
static netwib_err netwox_icmperr_send(netwox_icmperr_info *pinfo,
                                      netwib_bufpool *ppool,
                                      netwib_constbuf *ppkt,
                                      netwib_constip *psrcip,
                                      netwib_constip *pdstip)
{
  netwib_iphdr ipheader;
  netwib_icmp4 icmp4;
  netwib_icmp6 icmp6;
  netwib_buf *ppacket, ip64bits;

  netwib_er(netwib_bufpool_buf_init(ppool, &ppacket));

  netwib_er(netwib_ip64bits_init_ippkt(ppkt, &ip64bits));

  /* IP header */
  netwib_er(netwib_iphdr_initdefault(pinfo->iptype, &ipheader));
  ipheader.src = pinfo->srcip;
  ipheader.dst = *psrcip;
  ipheader.ttl = 255;

  /* ICMP header */
  switch(pinfo->iptype) {
  case NETWIB_IPTYPE_IP4 :
    netwib_er(netwib_icmp4_initdefault(pinfo->type4, &icmp4));
    switch(pinfo->type4) {
    case NETWIB_ICMP4TYPE_DSTUNREACH :
      icmp4.msg.dstunreach.badippacket = ip64bits;
      break;
    case NETWIB_ICMP4TYPE_TIMEEXCEED :
      icmp4.msg.timeexceed.badippacket = ip64bits;
      break;
    case NETWIB_ICMP4TYPE_PARAPROB :
      icmp4.msg.paraprob.pointer = (netwib_byte)pinfo->pointer;
      icmp4.msg.paraprob.badippacket = ip64bits;
      break;
    case NETWIB_ICMP4TYPE_SRCQUENCH :
      icmp4.msg.srcquench.badippacket = ip64bits;
      break;
    case NETWIB_ICMP4TYPE_REDIRECT :
      icmp4.msg.redirect.gw = pinfo->gw;
      icmp4.msg.redirect.badippacket = ip64bits;
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
    }
    netwib_er(netwib_pkt_append_ipicmp4(&ipheader, &icmp4, ppacket));
    break;
  case NETWIB_IPTYPE_IP6 :
    netwib_er(netwib_icmp6_initdefault(pinfo->type6, &icmp6));
    switch(pinfo->type6) {
    case NETWIB_ICMP6TYPE_DSTUNREACH :
      icmp6.msg.dstunreach.badippacket = ip64bits;
      break;
    case NETWIB_ICMP6TYPE_TIMEEXCEED :
      icmp6.msg.timeexceed.badippacket = ip64bits;
      break;
    case NETWIB_ICMP6TYPE_PARAPROB :
      icmp6.msg.paraprob.pointer = pinfo->pointer;
      icmp6.msg.paraprob.badippacket = ip64bits;
      break;
    case NETWIB_ICMP6TYPE_REDIRECT :
      icmp6.msg.redirect.target = *pdstip;
      icmp6.msg.redirect.dst = pinfo->gw;
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
    }
    netwib_er(netwib_pkt_append_ipicmp6(&ipheader, &icmp6, ppacket));
    break;
  default :
    return(NETWIB_ERR_PAIPTYPE);
  }

  /* send */
  /*
  netwib_er(netwib_pkt_ip_display(ppacket, NULL, NETWIB_ENCODETYPE_ARRAY,
                                  NETWIB_ENCODETYPE_DUMP));
  */
  netwib_er(netwib_io_write(pinfo->pspoofio, ppacket));

  netwib_er(netwib_bufpool_buf_close(ppool, &ppacket));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_icmperr_loop(netwox_icmperr_info *pinfo)
{
  netwib_bufpool *ppool;
  netwib_iphdr ipheader;
  netwib_ipproto ipproto;
  netwib_icmp4 icmp4;
  netwib_icmp6 icmp6;
  netwib_buf pkt, pkt2;
  netwib_err ret;

  if (pinfo->iptype != pinfo->srcip.iptype) {
    return(NETWIB_ERR_PAIPTYPE);
  }

  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_bufpool_initdefault(&ppool));

  /* main loop */
  while (NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();
    /* sniff */
    netwib__buf_reinit(&pkt);
    ret = netwib_io_read(pinfo->psniffio, &pkt);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    }
    netwib_er(ret);
    /* obtain IP layer */
    pkt2 = pkt;
    ret = netwib_pkt_decode_layer_ip(&pkt2, &ipheader);
    if (ret != NETWIB_ERR_OK) continue;
    if (ipheader.src.iptype != pinfo->iptype) continue;
    ret = netwib_iphdr_get_proto(&ipheader, &ipproto);
    if (ret != NETWIB_ERR_OK) continue;
    /* check if it's not the same packet we want to send */
    if (ipproto == NETWIB_IPPROTO_ICMP4) {
      ret = netwib_pkt_decode_layer_icmp4(&pkt2, &icmp4);
      if (ret == NETWIB_ERR_OK) {
        if (icmp4.type == pinfo->type4) {
          continue;
        }
      }
    } else if (ipproto == NETWIB_IPPROTO_ICMP6) {
      ret = netwib_pkt_decode_layer_icmp6(&pkt2, &icmp6);
      if (ret == NETWIB_ERR_OK) {
        if (icmp6.type == pinfo->type6) {
          continue;
        }
      }
    }
    /* send a fake ICMP */
    netwib_er(netwox_icmperr_send(pinfo, ppool, &pkt, &ipheader.src,
                                  &ipheader.dst));
  }

  netwib_er(netwib_bufpool_close(&ppool));
  netwib_er(netwib_buf_close(&pkt));

  return(NETWIB_ERR_OK);
}
