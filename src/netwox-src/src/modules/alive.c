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
netwib_err netwox_alive_ether(netwib_io *pspoofio,
                              netwib_consteth *peth,
                              netwib_constip *pip,
                              netwib_constbuf *ppkt,
                              netwib_bool answerarp,
                              netwib_bool answericmp,
                              netwib_bool answertcp,
                              netwib_bool *panswered)
{
  netwib_linkhdr linkheader;
  netwib_arphdr arpheader;
  netwib_iphdr ipheader;
  netwib_tcphdr tcpheader;
  netwib_icmp4 icmp4;
  netwib_icmp6 icmp6;
  netwib_buf pkt;
  netwib_ipproto ipproto;
  netwib_err ret;

  pkt = *ppkt;
  if (panswered != NULL) *panswered = NETWIB_FALSE;

  ret = netwib_pkt_decode_layer_link(NETWIB_DEVICE_DLTTYPE_ETHER,
                                     &pkt, &linkheader);
  if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);

  switch(linkheader.hdr.ether.type) {
  case NETWIB_ETHERHDRTYPE_IP4 :
    if (!answericmp && !answertcp) return(NETWIB_ERR_OK);
    /* follow */
  case NETWIB_ETHERHDRTYPE_IP6 :
    ret = netwib_pkt_decode_layer_ip(&pkt, &ipheader);
    if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
    netwib_er(netwib_iphdr_get_proto(&ipheader, &ipproto));
    switch(ipproto) {
    case NETWIB_IPPROTO_TCP :
      if (answertcp) {
        ret = netwib_pkt_decode_layer_tcp(&pkt, &tcpheader);
        if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
        netwib_er(netwox_alive_linkiptcpdata(pspoofio, peth, pip,
                                             &linkheader, &ipheader,
                                             &tcpheader, &pkt, panswered));
      }
      break;
    case NETWIB_IPPROTO_ICMP4 :
      if (answericmp) {
        ret = netwib_pkt_decode_layer_icmp4(&pkt, &icmp4);
        if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
        netwib_er(netwox_alive_linkipicmp4ping(pspoofio, peth, pip,
                                               &linkheader, &ipheader,
                                               &icmp4, panswered));
      }
      break;
    case NETWIB_IPPROTO_ICMP6 :
      if (answericmp || answerarp) {
        ret = netwib_pkt_decode_layer_icmp6(&pkt, &icmp6);
        if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
        if (answerarp) {
          netwib_er(netwox_alive_linkipicmp6nd(pspoofio, peth, pip,
                                               &linkheader, &ipheader,
                                               &icmp6, panswered));
        }
        if (answericmp) {
          netwib_er(netwox_alive_linkipicmp6ping(pspoofio, peth, pip,
                                                 &linkheader, &ipheader,
                                                 &icmp6, panswered));
        }
      }
      break;
    default :
      break;
    }
    break;
  case NETWIB_ETHERHDRTYPE_ARP :
    if (answerarp) {
      ret = netwib_pkt_decode_layer_arp(&pkt, &arpheader);
      if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
      netwib_er(netwox_alive_linkarp(pspoofio, peth, pip,
                                     &linkheader, &arpheader, panswered));
    }
    break;
  default :
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_alive_linkarp(netwib_io *pspoofio,
                                netwib_consteth *peth,
                                netwib_constip *pip,
                                netwib_constlinkhdr *plinkhdr,
                                netwib_constarphdr *parphdr,
                                netwib_bool *panswered)
{
  netwib_linkhdr linkheader;
  netwib_arphdr arpheader;
  netwib_buf pkt;
  netwib_cmp cmp;

  if (pip->iptype != NETWIB_IPTYPE_IP4) return(NETWIB_ERR_OK);

  /* check it's for us */
  if (plinkhdr->type != NETWIB_DEVICE_DLTTYPE_ETHER) return(NETWIB_ERR_OK);
  if (parphdr->op != NETWIB_ARPHDROP_ARPREQ) return(NETWIB_ERR_OK);
  netwib_er(netwib_ip_cmp(&parphdr->ipdst, pip, &cmp));
  if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);

  /* answer */
  netwib_er(netwib_linkhdr_initdefault(NETWIB_DEVICE_DLTTYPE_ETHER,
                                       &linkheader));
  linkheader.hdr.ether.src = *peth;
  linkheader.hdr.ether.dst = parphdr->ethsrc;
  linkheader.hdr.ether.type = NETWIB_ETHERHDRTYPE_ARP;
  netwib_er(netwib_arphdr_initdefault(&arpheader));
  arpheader.op = NETWIB_ARPHDROP_ARPREP;
  arpheader.ethsrc = *peth;
  arpheader.ipsrc = *pip;
  arpheader.ethdst = parphdr->ethsrc;
  arpheader.ipdst = parphdr->ipsrc;
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_pkt_append_linkarp(&linkheader, &arpheader, &pkt));
  netwib_er(netwib_io_write(pspoofio, &pkt));
  netwib_er(netwib_buf_close(&pkt));
  if (panswered != NULL) *panswered = NETWIB_TRUE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_alive_linkipicmp6nd(netwib_io *pspoofio, /* link level */
                                      netwib_consteth *peth,
                                      netwib_constip *pip,
                                      netwib_constlinkhdr *plinkhdr,
                                      netwib_constiphdr *piphdr,
                                      netwib_consticmp6 *picmp6,
                                      netwib_bool *panswered)
{
  netwib_buf pkt;
  netwib_bool pktset;

  netwib_er(netwib_buf_init_mallocdefault(&pkt));

  netwib_er(netwox_icmp6nd_alive(peth, pip, plinkhdr, piphdr, picmp6,
                                 &pktset, &pkt));

  if (pktset) {
    netwib_er(netwib_io_write(pspoofio, &pkt));
  }

  netwib_er(netwib_buf_close(&pkt));
  if (panswered != NULL) *panswered = pktset;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_alive_linkipicmp4ping(netwib_io *pspoofio,
                                        netwib_consteth *peth,
                                        netwib_constip *pip,
                                        netwib_constlinkhdr *plinkhdr,
                                        netwib_constiphdr *piphdr,
                                        netwib_consticmp4 *picmp4,
                                        netwib_bool *panswered)
{
  netwib_linkhdr linkheader;
  netwib_iphdr ipheader;
  netwib_icmp4 icmp4;
  netwib_buf pkt;
  netwib_cmp cmp;

  if (pip->iptype != NETWIB_IPTYPE_IP4) return(NETWIB_ERR_OK);

  /* check it's for us */
  if (plinkhdr->type != NETWIB_DEVICE_DLTTYPE_ETHER) return(NETWIB_ERR_OK);
  netwib_er(netwib_eth_cmp(&plinkhdr->hdr.ether.dst, peth, &cmp));
  if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
  netwib_er(netwib_ip_cmp(&piphdr->dst, pip, &cmp));
  if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
  if (picmp4->type != NETWIB_ICMP4TYPE_ECHOREQ) return(NETWIB_ERR_OK);
  if (picmp4->code != NETWIB_ICMP4CODE_ECHOREQ) return(NETWIB_ERR_OK);

  /* answer */
  netwib_er(netwib_linkhdr_initdefault(NETWIB_DEVICE_DLTTYPE_ETHER,
                                       &linkheader));
  linkheader.hdr.ether.src = *peth;
  linkheader.hdr.ether.dst = plinkhdr->hdr.ether.src;
  netwib_er(netwib_iphdr_initdefault(piphdr->iptype, &ipheader));
  ipheader.src = *pip;
  ipheader.dst = piphdr->src;
  icmp4 = *picmp4;
  icmp4.type = NETWIB_ICMP4TYPE_ECHOREP;
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_pkt_append_linkipicmp4(&linkheader, &ipheader, &icmp4,
                                          &pkt));
  netwib_er(netwib_io_write(pspoofio, &pkt));
  netwib_er(netwib_buf_close(&pkt));
  if (panswered != NULL) *panswered = NETWIB_TRUE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_alive_linkipicmp6ping(netwib_io *pspoofio,
                                        netwib_consteth *peth,
                                        netwib_constip *pip,
                                        netwib_constlinkhdr *plinkhdr,
                                        netwib_constiphdr *piphdr,
                                        netwib_consticmp6 *picmp6,
                                        netwib_bool *panswered)
{
  netwib_linkhdr linkheader;
  netwib_iphdr ipheader;
  netwib_icmp6 icmp6;
  netwib_buf pkt;
  netwib_cmp cmp;

  if (pip->iptype != NETWIB_IPTYPE_IP6) return(NETWIB_ERR_OK);

  /* check it's for us */
  if (plinkhdr->type != NETWIB_DEVICE_DLTTYPE_ETHER) return(NETWIB_ERR_OK);
  netwib_er(netwib_eth_cmp(&plinkhdr->hdr.ether.dst, peth, &cmp));
  if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
  netwib_er(netwib_ip_cmp(&piphdr->dst, pip, &cmp));
  if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
  if (picmp6->type != NETWIB_ICMP6TYPE_ECHOREQ) return(NETWIB_ERR_OK);
  if (picmp6->code != NETWIB_ICMP6CODE_ECHOREQ) return(NETWIB_ERR_OK);

  /* answer */
  netwib_er(netwib_linkhdr_initdefault(NETWIB_DEVICE_DLTTYPE_ETHER,
                                       &linkheader));
  linkheader.hdr.ether.src = *peth;
  linkheader.hdr.ether.dst = plinkhdr->hdr.ether.src;
  netwib_er(netwib_iphdr_initdefault(piphdr->iptype, &ipheader));
  ipheader.src = *pip;
  ipheader.dst = piphdr->src;
  icmp6 = *picmp6;
  icmp6.type = NETWIB_ICMP6TYPE_ECHOREP;
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_pkt_append_linkipicmp6(&linkheader, &ipheader, &icmp6,
                                          &pkt));
  netwib_er(netwib_io_write(pspoofio, &pkt));
  netwib_er(netwib_buf_close(&pkt));
  if (panswered != NULL) *panswered = NETWIB_TRUE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_alive_linkiptcpdata(netwib_io *pspoofio,
                                      netwib_consteth *peth,
                                      netwib_constip *pip,
                                      netwib_constlinkhdr *plinkhdr,
                                      netwib_constiphdr *piphdr,
                                      netwib_consttcphdr *ptcphdr,
                                      netwib_constbuf *ptcpdata,
                                      netwib_bool *panswered)
{
  netwib_linkhdr linkheader;
  netwib_iphdr ipheader;
  netwib_tcphdr tcpheader;
  netwib_buf pkt;
  netwib_cmp cmp;

  /* check it's for us */
  if (plinkhdr->type != NETWIB_DEVICE_DLTTYPE_ETHER) return(NETWIB_ERR_OK);
  netwib_er(netwib_eth_cmp(&plinkhdr->hdr.ether.dst, peth, &cmp));
  if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
  netwib_er(netwib_ip_cmp(&piphdr->dst, pip, &cmp));
  if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
  if (!ptcphdr->syn || !ptcphdr->ack) return(NETWIB_ERR_OK);
  if (netwib__buf_ref_data_sizenull(ptcpdata)) return(NETWIB_ERR_OK);

  /* answer */
  netwib_er(netwib_linkhdr_initdefault(NETWIB_DEVICE_DLTTYPE_ETHER,
                                       &linkheader));
  linkheader.hdr.ether.src = *peth;
  linkheader.hdr.ether.dst = plinkhdr->hdr.ether.src;
  netwib_er(netwib_iphdr_initdefault(piphdr->iptype, &ipheader));
  ipheader.src = *pip;
  ipheader.dst = piphdr->src;
  netwib_er(netwib_tcphdr_initdefault(&tcpheader));
  tcpheader.src = ptcphdr->dst;
  tcpheader.dst = ptcphdr->src;
  tcpheader.rst = NETWIB_TRUE;
  tcpheader.ack = NETWIB_TRUE;
  tcpheader.window = 0;
  tcpheader.seqnum = ptcphdr->acknum;
  tcpheader.acknum = ptcphdr->seqnum + 1;
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_pkt_append_linkiptcpdata(&linkheader, &ipheader,
                                            &tcpheader, NULL, &pkt));
  netwib_er(netwib_io_write(pspoofio, &pkt));
  netwib_er(netwib_buf_close(&pkt));
  if (panswered != NULL) *panswered = NETWIB_TRUE;

  return(NETWIB_ERR_OK);
}
