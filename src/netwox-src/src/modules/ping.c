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
netwib_err netwox_ping_addinfo_initdefault(netwox_ping_addinfo *paddinfo)
{
  netwib_buf_init_ext_empty(&paddinfo->device);
  paddinfo->srcethset = NETWIB_FALSE;
  paddinfo->dstethset = NETWIB_FALSE;
  paddinfo->srcipset = NETWIB_FALSE;
  paddinfo->dstport = NETWOX_SERVICES_HTTP;
  paddinfo->maxcount = 0xFFFFFFFFu;
  paddinfo->beep = NETWIB_FALSE;
  paddinfo->display01 = NETWIB_FALSE;
  paddinfo->maxwaitms = 1000;

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
typedef struct {
  netwox_ping_method method;
  netwib_constip *pdstip;
  netwox_ping_addinfo *paddinfo;
  netwib_io *pio;
  netwib_uint32 id;
  netwib_uint32 idport;
  netwib_buf pkt;
  netwib_buf pkt1;
  netwib_buf pkt2;
} netwox_priv_ping_method;

/*-------------------------------------------------------------*/
static netwib_err netwox_ping_method_init(netwox_ping_method method,
                                          netwib_constip *pdstip,
                                          netwox_ping_addinfo *paddinfo,
                                          netwox_priv_ping_method *ppingmethod)
{
  netwib_buf systemdevice, filter, *pdevice=NULL;
  netwib_bool supportip6;

  /* set info */
  ppingmethod->method = method;
  ppingmethod->pdstip = pdstip;
  ppingmethod->paddinfo = paddinfo;

  /* parameter verification */
  switch(method) {
  case NETWOX_PING_METHOD_PING_ETHIPSPOOF :
  case NETWOX_PING_METHOD_TCP_ETHIPSPOOF :
  case NETWOX_PING_METHOD_UDP_ETHIPSPOOF :
  case NETWOX_PING_METHOD_ARP_ETHIPSPOOF :
  case NETWOX_PING_METHOD_ICMP6ND_ETHIPSPOOF :
    if (netwib__buf_ref_data_size(&paddinfo->device) == 0) {
      netwib_er(netwib_fmt_display("device must be set\n"));
      return(NETWOX_ERR_TOOLARG_INVALID);
    }
    if (!paddinfo->srcethset) {
      netwib_er(netwib_fmt_display("src-eth must be set\n"));
      return(NETWOX_ERR_TOOLARG_INVALID);
    }
    if (method != NETWOX_PING_METHOD_ARP_ETHIPSPOOF &&
        method != NETWOX_PING_METHOD_ICMP6ND_ETHIPSPOOF) {
      if (!paddinfo->dstethset) {
        netwib_er(netwib_fmt_display("dst-eth must be set\n"));
        return(NETWOX_ERR_TOOLARG_INVALID);
      }
    }
    if (!paddinfo->srcipset) {
      netwib_er(netwib_fmt_display("src-ip must be set\n"));
      return(NETWOX_ERR_TOOLARG_INVALID);
    }
    break;
  default :
    break;
  }

  /* initialize spoof/sniff io */
  netwib_er(netwib_buf_init_mallocdefault(&systemdevice));
  netwib_er(netwib_buf_init_mallocdefault(&filter));
  switch(method) {
  case NETWOX_PING_METHOD_PING :
  case NETWOX_PING_METHOD_TCP :
  case NETWOX_PING_METHOD_UDP :
    /* obtain legitimate route */
    netwib_er(netwib_conf_routes_reach_ip(pdstip, &systemdevice,
                                          &paddinfo->srcip));
    /* initialize sniff/spoof io */
    netwib_er(netwox_pcapfilt_supports_ip6(&systemdevice, &supportip6));
    if (supportip6 || pdstip->iptype == NETWIB_IPTYPE_IP4) {
      netwib_er(netwib_buf_append_fmt(&filter, "host %{ip}", pdstip));
    }
    netwib_er(netwox_snispo_init_ip(&systemdevice, &filter,
                                    NETWIB_FALSE, NETWIB_FALSE,
                                    paddinfo->spoofipinittype,
                                    &ppingmethod->pio));
    break;
  case NETWOX_PING_METHOD_PING_ETHIPSPOOF :
  case NETWOX_PING_METHOD_TCP_ETHIPSPOOF :
  case NETWOX_PING_METHOD_UDP_ETHIPSPOOF :
    /* initialize sniff/spoof io */
    netwib_er(netwox_pcapfilt_supports_ip6(&paddinfo->device, &supportip6));
    if (pdstip->iptype == NETWIB_IPTYPE_IP4) {
      netwib_er(netwib_buf_append_fmt(&filter, "arp or host %{ip}", pdstip));
    } else {
      if (supportip6) {
        netwib_er(netwib_buf_append_fmt(&filter, "icmp6 or host %{ip}",
                                        pdstip));
      }
    }
    netwib_er(netwox_snispo_init_eth(&paddinfo->device, &filter,
                                     &ppingmethod->pio));
    break;
  case NETWOX_PING_METHOD_ARP :
  case NETWOX_PING_METHOD_ARP_ETHIPSPOOF :
    if (pdstip->iptype != NETWIB_IPTYPE_IP4) {
      netwib_er(netwib_fmt_display("ARP method is not valid for IPv6\n"));
      return(NETWOX_ERR_TOOLARG_INVALID);
    }
    pdevice = &paddinfo->device;
    if (method == NETWOX_PING_METHOD_ARP) {
      /* obtain legitimate route */
      netwib_er(netwib_conf_routes_reach_ip(pdstip, &systemdevice,
                                            &paddinfo->srcip));
      netwib_er(netwib_eth_init_ip(&paddinfo->srcip, &paddinfo->srceth));
      pdevice = &systemdevice;
    }
    /* initialize io */
    netwib_er(netwib_buf_append_fmt(&filter, "arp"));
    netwib_er(netwox_snispo_init_eth(pdevice, &filter, &ppingmethod->pio));
    break;
  case NETWOX_PING_METHOD_ICMP6ND :
  case NETWOX_PING_METHOD_ICMP6ND_ETHIPSPOOF :
    if (pdstip->iptype != NETWIB_IPTYPE_IP6) {
      netwib_er(netwib_fmt_display("ICMP6ND method is not valid for IPv4\n"));
      return(NETWOX_ERR_TOOLARG_INVALID);
    }
    pdevice = &paddinfo->device;
    if (method == NETWOX_PING_METHOD_ICMP6ND) {
      /* obtain legitimate route */
      netwib_er(netwib_conf_routes_reach_ip(pdstip, &systemdevice,
                                            &paddinfo->srcip));
      netwib_er(netwib_eth_init_ip(&paddinfo->srcip, &paddinfo->srceth));
      pdevice = &systemdevice;
    }
    /* initialize io */
    netwib_er(netwox_pcapfilt_supports_ip6(pdevice, &supportip6));
    if (supportip6) {
      netwib_er(netwib_buf_append_fmt(&filter, "icmp6"));
    }
    netwib_er(netwox_snispo_init_eth(pdevice, &filter, &ppingmethod->pio));
    break;
  }
  netwib_er(netwib_buf_close(&filter));
  netwib_er(netwib_buf_close(&systemdevice));

  /* initialize temporary buffer */
  netwib_er(netwib_buf_init_mallocdefault(&ppingmethod->pkt));
  netwib_er(netwib_buf_init_mallocdefault(&ppingmethod->pkt1));
  netwib_er(netwib_buf_init_mallocdefault(&ppingmethod->pkt2));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_ping_method_close(netwox_priv_ping_method *ppingmethod)
{

  netwib_er(netwib_buf_close(&ppingmethod->pkt2));
  netwib_er(netwib_buf_close(&ppingmethod->pkt1));
  netwib_er(netwib_buf_close(&ppingmethod->pkt));
  netwib_er(netwib_io_close(&ppingmethod->pio));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_ping_method_request(netwox_priv_ping_method *ppingmethod)
{
  netwib_linkhdr linkheader;
  netwib_iphdr ipheader;
  netwib_udphdr udpheader;
  netwib_tcphdr tcpheader;
  netwib_icmp4 icmp4;
  netwib_icmp6 icmp6;
  netwib_arphdr arpheader;

  netwib__buf_reinit(&ppingmethod->pkt);
  netwib__buf_reinit(&ppingmethod->pkt1);
  netwib__buf_reinit(&ppingmethod->pkt2);

  netwib_er(netwib_uint32_init_rand(0, 0xFFFFFFFFu, &ppingmethod->id));
  netwib_er(netwib_uint32_init_rand(1024, 0xFFFFu, &ppingmethod->idport));

  /* in those cases, first add an Eth header */
  switch(ppingmethod->method) {
  case NETWOX_PING_METHOD_PING_ETHIPSPOOF :
  case NETWOX_PING_METHOD_TCP_ETHIPSPOOF :
  case NETWOX_PING_METHOD_UDP_ETHIPSPOOF :
    netwib_er(netwib_linkhdr_initdefault(NETWIB_DEVICE_DLTTYPE_ETHER,
                                         &linkheader));
    linkheader.hdr.ether.src = ppingmethod->paddinfo->srceth;
    linkheader.hdr.ether.dst = ppingmethod->paddinfo->dsteth;
    if (ppingmethod->pdstip->iptype == NETWIB_IPTYPE_IP4) {
      linkheader.hdr.ether.type = NETWIB_ETHERHDRTYPE_IP4;
    } else {
      linkheader.hdr.ether.type = NETWIB_ETHERHDRTYPE_IP6;
    }
    netwib_er(netwib_pkt_append_linkhdr(&linkheader, &ppingmethod->pkt));
    break;
  default :
    break;
  }

  /* construct packet */
  switch(ppingmethod->method) {
  case NETWOX_PING_METHOD_PING :
  case NETWOX_PING_METHOD_PING_ETHIPSPOOF :
    netwib_er(netwib_iphdr_initdefault(ppingmethod->pdstip->iptype,&ipheader));
    ipheader.src = ppingmethod->paddinfo->srcip;
    ipheader.dst = *ppingmethod->pdstip;
    if (ppingmethod->pdstip->iptype == NETWIB_IPTYPE_IP4) {
      netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_ECHOREQ, &icmp4));
      icmp4.msg.echo.id = (netwib_uint16)(ppingmethod->id & 0xFF);
      icmp4.msg.echo.seqnum = (netwib_uint16)(ppingmethod->id >> 16);
      netwib_er(netwib_buf_init_ext_string("abcdefghijkm",
                                         &icmp4.msg.echo.data));
      netwib_er(netwib_pkt_append_ipicmp4(&ipheader, &icmp4,
                                          &ppingmethod->pkt));
    } else {
      netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_ECHOREQ, &icmp6));
      icmp6.msg.echo.id = (netwib_uint16)(ppingmethod->id & 0xFF);
      icmp6.msg.echo.seqnum = (netwib_uint16)(ppingmethod->id >> 16);
      netwib_er(netwib_buf_init_ext_string("abcdefghijkm",
                                         &icmp6.msg.echo.data));
      netwib_er(netwib_pkt_append_ipicmp6(&ipheader, &icmp6,
                                          &ppingmethod->pkt));
    }
    break;
  case NETWOX_PING_METHOD_TCP :
  case NETWOX_PING_METHOD_TCP_ETHIPSPOOF :
    netwib_er(netwib_iphdr_initdefault(ppingmethod->pdstip->iptype,&ipheader));
    ipheader.src = ppingmethod->paddinfo->srcip;
    ipheader.dst = *ppingmethod->pdstip;
    netwib_er(netwib_tcphdr_initdefault(&tcpheader));
    tcpheader.src = ppingmethod->idport;
    tcpheader.dst = ppingmethod->paddinfo->dstport;
    tcpheader.syn = NETWIB_TRUE;
    tcpheader.seqnum = ppingmethod->id;
    netwib_er(netwib_pkt_append_iptcpdata(&ipheader, &tcpheader, NULL,
                                          &ppingmethod->pkt));
    break;
  case NETWOX_PING_METHOD_UDP :
  case NETWOX_PING_METHOD_UDP_ETHIPSPOOF :
    netwib_er(netwib_iphdr_initdefault(ppingmethod->pdstip->iptype,&ipheader));
    ipheader.src = ppingmethod->paddinfo->srcip;
    ipheader.dst = *ppingmethod->pdstip;
    netwib_er(netwib_udphdr_initdefault(&udpheader));
    udpheader.src = ppingmethod->idport;
    udpheader.dst = ppingmethod->paddinfo->dstport;
    netwib_er(netwib_pkt_append_ipudpdata(&ipheader, &udpheader, NULL,
                                          &ppingmethod->pkt));
    break;
  case NETWOX_PING_METHOD_ARP :
  case NETWOX_PING_METHOD_ARP_ETHIPSPOOF :
    linkheader.type = NETWIB_DEVICE_DLTTYPE_ETHER;
    linkheader.hdr.ether.src = ppingmethod->paddinfo->srceth;
    netwib_er(netwib_eth_init_fields(0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                                   &linkheader.hdr.ether.dst));
    linkheader.hdr.ether.type = NETWIB_ETHERHDRTYPE_ARP;
    netwib_er(netwib_arphdr_initdefault(&arpheader));
    arpheader.op = NETWIB_ARPHDROP_ARPREQ;
    arpheader.ethsrc = ppingmethod->paddinfo->srceth;
    arpheader.ipsrc = ppingmethod->paddinfo->srcip;
    arpheader.ipdst = *ppingmethod->pdstip;
    netwib_er(netwib_pkt_append_linkarp(&linkheader, &arpheader,
                                        &ppingmethod->pkt));
    break;
  case NETWOX_PING_METHOD_ICMP6ND :
  case NETWOX_PING_METHOD_ICMP6ND_ETHIPSPOOF :
    netwib_er(netwox_icmp6nd_ns(&ppingmethod->paddinfo->srceth,
                                &ppingmethod->paddinfo->srcip,
                                ppingmethod->pdstip, &ppingmethod->pkt));
    break;
  }

  /* send it */
  netwib_er(netwib_io_write(ppingmethod->pio, &ppingmethod->pkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_ping_method_reply(netwox_priv_ping_method *ppingmethod,
                                           netwib_bool *pmatched)
{
  netwib_linkhdr linkheader;
  netwib_arphdr arpheader;
  netwib_iphdr ipheader;
  netwib_tcphdr tcpheader;
  netwib_icmp4 icmp4;
  netwib_icmp6 icmp6;
  netwib_buf databuf;
  netwib_port udpsrc, udpdst;
  netwib_cmp cmp;
  netwib_bool answered;
  netwib_err ret;

  *pmatched = NETWIB_FALSE;

  /* in those cases, first decode an Eth header */
  switch(ppingmethod->method) {
  case NETWOX_PING_METHOD_PING_ETHIPSPOOF :
  case NETWOX_PING_METHOD_TCP_ETHIPSPOOF :
  case NETWOX_PING_METHOD_UDP_ETHIPSPOOF :
    netwib_er(netwox_alive_ether(ppingmethod->pio,
                                 &ppingmethod->paddinfo->srceth,
                                 &ppingmethod->paddinfo->srcip,
                                 &ppingmethod->pkt, NETWIB_TRUE,
                                 NETWIB_FALSE, NETWIB_FALSE, &answered));
    if (answered) return(NETWIB_ERR_OK);
    netwib_er(netwib_pkt_decode_layer_link(NETWIB_DEVICE_DLTTYPE_ETHER,
                                           &ppingmethod->pkt, &linkheader));
    if (linkheader.hdr.ether.type != NETWIB_ETHERHDRTYPE_IP4 &&
        linkheader.hdr.ether.type != NETWIB_ETHERHDRTYPE_IP6) {
      return(NETWIB_ERR_OK);
    }
    netwib_er(netwib_eth_cmp(&linkheader.hdr.ether.dst,
                             &ppingmethod->paddinfo->srceth, &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    break;
  default :
    break;
  }

  /* decode packet */
  switch(ppingmethod->method) {
  case NETWOX_PING_METHOD_PING :
  case NETWOX_PING_METHOD_PING_ETHIPSPOOF :
    if (ppingmethod->pdstip->iptype == NETWIB_IPTYPE_IP4) {
      ret = netwib_pkt_decode_ipicmp4(&ppingmethod->pkt, &ipheader, &icmp4);
      if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
      netwib_er(netwib_ip_cmp(&ipheader.src, ppingmethod->pdstip, &cmp));
      if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
      netwib_er(netwib_ip_cmp(&ipheader.dst, &ppingmethod->paddinfo->srcip,
                              &cmp));
      if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
      if (icmp4.type != NETWIB_ICMP4TYPE_ECHOREP) return(NETWIB_ERR_OK);
      if (icmp4.code != NETWIB_ICMP4CODE_ECHOREP) return(NETWIB_ERR_OK);
      if (icmp4.msg.echo.id != (ppingmethod->id & 0xFF)) return(NETWIB_ERR_OK);
      if (icmp4.msg.echo.seqnum != (ppingmethod->id >> 16))
        return(NETWIB_ERR_OK);
    } else {
      ret = netwib_pkt_decode_ipicmp6(&ppingmethod->pkt, &ipheader, &icmp6);
      if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
      netwib_er(netwib_ip_cmp(&ipheader.src, ppingmethod->pdstip, &cmp));
      if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
      netwib_er(netwib_ip_cmp(&ipheader.dst, &ppingmethod->paddinfo->srcip,
                              &cmp));
      if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
      if (icmp6.type != NETWIB_ICMP6TYPE_ECHOREP) return(NETWIB_ERR_OK);
      if (icmp6.code != NETWIB_ICMP6CODE_ECHOREP) return(NETWIB_ERR_OK);
      if (icmp6.msg.echo.id != (ppingmethod->id & 0xFF)) return(NETWIB_ERR_OK);
      if (icmp6.msg.echo.seqnum != (ppingmethod->id >> 16))
        return(NETWIB_ERR_OK);
    }
    /* do not check icmp data, suppose it's ok */
    *pmatched = NETWIB_TRUE;
    break;
  case NETWOX_PING_METHOD_TCP :
  case NETWOX_PING_METHOD_TCP_ETHIPSPOOF :
    ret = netwib_pkt_decode_iptcpdata(&ppingmethod->pkt, &ipheader,
                                      &tcpheader, &databuf);
    if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
    netwib_er(netwib_ip_cmp(&ipheader.src, ppingmethod->pdstip, &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    netwib_er(netwib_ip_cmp(&ipheader.dst, &ppingmethod->paddinfo->srcip,
                            &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    if (tcpheader.src != ppingmethod->paddinfo->dstport) return(NETWIB_ERR_OK);
    if (tcpheader.dst != ppingmethod->idport) return(NETWIB_ERR_OK);
    if (tcpheader.acknum != ppingmethod->id+1) return(NETWIB_ERR_OK);
    if (tcpheader.syn && tcpheader.ack) {
      /* open */
      *pmatched = NETWIB_TRUE;
      if (ppingmethod->method ==  NETWOX_PING_METHOD_TCP_ETHIPSPOOF) {
        /* now, send a RST, otherwise, syn flood the host */
        netwib_er(netwox_alive_linkiptcpdata(ppingmethod->pio,
                                             &ppingmethod->paddinfo->srceth,
                                             &ppingmethod->paddinfo->srcip,
                                             &linkheader, &ipheader,
                                             &tcpheader, &databuf, NULL));
      }
    } else if (tcpheader.rst && tcpheader.ack) {
      /* closed, but accept it */
      *pmatched = NETWIB_TRUE;
    }
    break;
  case NETWOX_PING_METHOD_UDP :
  case NETWOX_PING_METHOD_UDP_ETHIPSPOOF :
    if (ppingmethod->pdstip->iptype == NETWIB_IPTYPE_IP4) {
      ret = netwib_pkt_decode_ipicmp4(&ppingmethod->pkt, &ipheader, &icmp4);
      if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
      netwib_er(netwib_ip_cmp(&ipheader.dst, &ppingmethod->paddinfo->srcip,
                              &cmp));
      if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
      if (icmp4.type != NETWIB_ICMP4TYPE_DSTUNREACH) return(NETWIB_ERR_OK);
      if (icmp4.code != NETWIB_ICMP4CODE_DSTUNREACH_PORT)
        return(NETWIB_ERR_OK);
      ret = netwib_ip64bits_decode_ipudp(&icmp4.msg.dstunreach.badippacket,
                                         &ipheader, &udpsrc, &udpdst);
    } else {
      ret = netwib_pkt_decode_ipicmp6(&ppingmethod->pkt, &ipheader, &icmp6);
      if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
      netwib_er(netwib_ip_cmp(&ipheader.dst, &ppingmethod->paddinfo->srcip,
                              &cmp));
      if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
      if (icmp6.type != NETWIB_ICMP6TYPE_DSTUNREACH) return(NETWIB_ERR_OK);
      if (icmp6.code != NETWIB_ICMP6CODE_DSTUNREACH_PORT)
        return(NETWIB_ERR_OK);
      ret = netwib_ip64bits_decode_ipudp(&icmp6.msg.dstunreach.badippacket,
                                         &ipheader, &udpsrc, &udpdst);
    }
    if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
    netwib_er(netwib_ip_cmp(&ipheader.src, &ppingmethod->paddinfo->srcip,
                            &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    netwib_er(netwib_ip_cmp(&ipheader.dst, ppingmethod->pdstip, &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    if (udpsrc != ppingmethod->idport) return(NETWIB_ERR_OK);
    if (udpdst != ppingmethod->paddinfo->dstport) return(NETWIB_ERR_OK);
    *pmatched = NETWIB_TRUE;
    break;
  case NETWOX_PING_METHOD_ARP :
  case NETWOX_PING_METHOD_ARP_ETHIPSPOOF :
    ret = netwib_pkt_decode_linkarp(NETWIB_DEVICE_DLTTYPE_ETHER,
                                   &ppingmethod->pkt, &linkheader, &arpheader);
    if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
    netwib_er(netwib_eth_cmp(&linkheader.hdr.ether.dst,
                             &ppingmethod->paddinfo->srceth, &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    if (arpheader.op != NETWIB_ARPHDROP_ARPREP) return(NETWIB_ERR_OK);
    netwib_er(netwib_ip_cmp(&arpheader.ipdst, &ppingmethod->paddinfo->srcip,
                            &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    netwib_er(netwib_eth_cmp(&arpheader.ethdst, &ppingmethod->paddinfo->srceth,
                             &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    netwib_er(netwib_ip_cmp(&arpheader.ipsrc, ppingmethod->pdstip, &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    *pmatched = NETWIB_TRUE;
    break;
  case NETWOX_PING_METHOD_ICMP6ND :
  case NETWOX_PING_METHOD_ICMP6ND_ETHIPSPOOF :
    ret = netwib_pkt_decode_linkipicmp6(NETWIB_DEVICE_DLTTYPE_ETHER,
                                        &ppingmethod->pkt, &linkheader,
                                        &ipheader, &icmp6);
    if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
    netwib_er(netwox_icmp6nd_na(ppingmethod->pdstip, &linkheader,
                                &ipheader, &icmp6, pmatched, NULL));
    break;
  }

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwox_ping_loop(netwox_ping_method method,
                            netwib_constip *pdstip,
                            netwox_ping_addinfo *paddinfo)
{
  netwox_priv_ping_method pingmethod;
  netwib_bool matched, event, reached;
  netwib_time t;
  netwib_err ret;

  /* adapt parameters */
  if (paddinfo->display01) {
    paddinfo->maxcount = 1;
  }

  /* initialize depending on method */
  netwib_er(netwox_ping_method_init(method, pdstip, paddinfo, &pingmethod));

  /* main loop */
  reached = NETWIB_FALSE;
  event = NETWIB_FALSE;
  while (NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();
    /* send a request */
    netwib_er(netwox_ping_method_request(&pingmethod));
    /* sniff and expect a reply */
    netwib_er(netwib_time_init_now(&t));
    netwib_er(netwib_time_plus_msec(&t, paddinfo->maxwaitms));
    while (NETWIB_TRUE) {
      netwib__debug_ctrlc_pressed_break();
      /* sniff */
      netwib_er(netwib_io_wait_read(pingmethod.pio, &t, &event));
      if (!event) break;
      netwib__buf_reinit(&pingmethod.pkt);
      ret = netwib_io_read(pingmethod.pio, &pingmethod.pkt);
      if (ret == NETWIB_ERR_DATANOTAVAIL) {
        ret = NETWIB_ERR_OK;
        continue;
      }
      netwib_er(ret);
      /* check it corresponds */
      netwib_er(netwox_ping_method_reply(&pingmethod, &matched));
      if (matched) {
        reached = NETWIB_TRUE;
        if (pingmethod.paddinfo->beep) {
          netwib_er(netwib_beep());
        }
        if (!pingmethod.paddinfo->display01) {
          netwib_er(netwib_fmt_display("Ok\n"));
        }
        break;
      }
    }
    paddinfo->maxcount--;
    if (!paddinfo->maxcount) {
      break;
    }
    if (event) {
      /* now, sleep the remaining time */
      netwib_er(netwib_time_wait_time(&t));
    }
  }

  if (pingmethod.paddinfo->display01) {
    netwib_er(netwib_fmt_display("%c\n", reached?'1':'0'));
  }

  /* close */
  netwib_er(netwox_ping_method_close(&pingmethod));

  return(NETWIB_ERR_OK);
}

