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
netwib_err netwox_tracert_addinfo_initdefault(netwox_tracert_addinfo *paddinfo)
{
  netwib_buf_init_ext_empty(&paddinfo->device);
  paddinfo->srcethset = NETWIB_FALSE;
  paddinfo->dstethset = NETWIB_FALSE;
  paddinfo->srcipset = NETWIB_FALSE;
  paddinfo->dstport = NETWOX_SERVICES_HTTP;
  paddinfo->ipproto = NETWIB_IPPROTO_ICMP4;
  netwib_buf_init_ext_empty(&paddinfo->ipdata);
  paddinfo->maxttl = 1;
  paddinfo->maxttl = 30;
  paddinfo->maxwaitms = 1000;
  paddinfo->resolve = NETWIB_FALSE;

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
typedef struct {
  netwox_tracert_method method;
  netwib_constip *pdstip;
  netwox_tracert_addinfo *paddinfo;
  netwib_io *pio;
  netwib_uint32 ttl;
  netwib_uint32 iduint;
  netwib_uint32 idport;
  netwib_buf pkt;
  netwib_buf pkt1;
  netwib_buf pkt2;
} netwox_priv_tracert_method;

/*-------------------------------------------------------------*/
static netwib_err netwox_tracert_method_init(netwox_tracert_method method,
                                             netwib_constip *pdstip,
                                             netwox_tracert_addinfo *paddinfo,
                                             netwox_priv_tracert_method *ptracertmethod)
{
  netwib_buf systemdevice, filter;
  netwib_bool supportip6;
  netwib_err ret;

  /* set info */
  ptracertmethod->method = method;
  ptracertmethod->pdstip = pdstip;
  ptracertmethod->paddinfo = paddinfo;

  /* parameter verification */
  switch(method) {
  case NETWOX_TRACERT_METHOD_PING_ETHIPSPOOF :
  case NETWOX_TRACERT_METHOD_TCP_ETHIPSPOOF :
  case NETWOX_TRACERT_METHOD_UDP_ETHIPSPOOF :
  case NETWOX_TRACERT_METHOD_PROTO_ETHIPSPOOF :
    if (netwib__buf_ref_data_size(&paddinfo->device) == 0) {
      netwib_er(netwib_fmt_display("device must be set\n"));
      return(NETWOX_ERR_TOOLARG_INVALID);
    }
    if (!paddinfo->srcethset) {
      netwib_er(netwib_fmt_display("src-eth must be set\n"));
      return(NETWOX_ERR_TOOLARG_INVALID);
    }
    if (!paddinfo->dstethset) {
      netwib_er(netwib_fmt_display("dst-eth must be set\n"));
      return(NETWOX_ERR_TOOLARG_INVALID);
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
  ret = NETWIB_ERR_OK;
  switch(method) {
  case NETWOX_TRACERT_METHOD_PING :
  case NETWOX_TRACERT_METHOD_TCP :
  case NETWOX_TRACERT_METHOD_UDP :
  case NETWOX_TRACERT_METHOD_PROTO :
    /* obtain legitimate route */
    netwib_eg(netwib_conf_routes_reach_ip(pdstip, &systemdevice,
                                          &paddinfo->srcip));
    /* initialize sniff/spoof io */
    netwib_eg(netwox_pcapfilt_supports_ip6(&systemdevice, &supportip6));
    if (supportip6 || paddinfo->srcip.iptype == NETWIB_IPTYPE_IP4) {
      netwib_eg(netwib_buf_append_fmt(&filter, "host %{ip}",
                                      &paddinfo->srcip));
    }
    netwib_eg(netwox_snispo_init_ip(&systemdevice, &filter,
                                    NETWIB_FALSE, NETWIB_FALSE,
                                    paddinfo->spoofipinittype,
                                    &ptracertmethod->pio));
    break;
  case NETWOX_TRACERT_METHOD_PING_ETHIPSPOOF :
  case NETWOX_TRACERT_METHOD_TCP_ETHIPSPOOF :
  case NETWOX_TRACERT_METHOD_UDP_ETHIPSPOOF :
  case NETWOX_TRACERT_METHOD_PROTO_ETHIPSPOOF :
    /* initialize sniff/spoof io */
    netwib_eg(netwox_pcapfilt_supports_ip6(&paddinfo->device, &supportip6));
    if (paddinfo->srcip.iptype == NETWIB_IPTYPE_IP4) {
      netwib_eg(netwib_buf_append_fmt(&filter, "arp or host %{ip}",
                                      &paddinfo->srcip));
    } else {
      if (supportip6) {
        netwib_eg(netwib_buf_append_fmt(&filter, "icmp6 or host %{ip}",
                                        &paddinfo->srcip));
      }
    }
    netwib_eg(netwox_snispo_init_eth(&paddinfo->device, &filter,
                                     &ptracertmethod->pio));
    break;
  }
 netwib_gotolabel:
  netwib_er(netwib_buf_close(&filter));
  netwib_er(netwib_buf_close(&systemdevice));
  netwib_er(ret);

  /* initialize temporary buffer */
  netwib_er(netwib_buf_init_mallocdefault(&ptracertmethod->pkt));
  netwib_er(netwib_buf_init_mallocdefault(&ptracertmethod->pkt1));
  netwib_er(netwib_buf_init_mallocdefault(&ptracertmethod->pkt2));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_tracert_method_close(netwox_priv_tracert_method *ptracertmethod)
{

  netwib_er(netwib_buf_close(&ptracertmethod->pkt2));
  netwib_er(netwib_buf_close(&ptracertmethod->pkt1));
  netwib_er(netwib_buf_close(&ptracertmethod->pkt));
  netwib_er(netwib_io_close(&ptracertmethod->pio));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_tracert_method_request(netwox_priv_tracert_method *ptracertmethod)
{
  netwib_linkhdr linkheader;
  netwib_iphdr ipheader;
  netwib_udphdr udpheader;
  netwib_tcphdr tcpheader;
  netwib_icmp4 icmp4;
  netwib_icmp6 icmp6;

  netwib__buf_reinit(&ptracertmethod->pkt);
  netwib__buf_reinit(&ptracertmethod->pkt1);
  netwib__buf_reinit(&ptracertmethod->pkt2);

  netwib_er(netwib_uint32_init_rand(0, 0xFFFFFFFFu, &ptracertmethod->iduint));
  netwib_er(netwib_uint32_init_rand(1024, 0xFFFFu, &ptracertmethod->idport));

  /* in those cases, first add an Eth header */
  switch(ptracertmethod->method) {
  case NETWOX_TRACERT_METHOD_PING_ETHIPSPOOF :
  case NETWOX_TRACERT_METHOD_TCP_ETHIPSPOOF :
  case NETWOX_TRACERT_METHOD_UDP_ETHIPSPOOF :
  case NETWOX_TRACERT_METHOD_PROTO_ETHIPSPOOF :
    netwib_er(netwib_linkhdr_initdefault(NETWIB_DEVICE_DLTTYPE_ETHER,
                                         &linkheader));
    linkheader.hdr.ether.src = ptracertmethod->paddinfo->srceth;
    linkheader.hdr.ether.dst = ptracertmethod->paddinfo->dsteth;
    if (ptracertmethod->pdstip->iptype == NETWIB_IPTYPE_IP4) {
      linkheader.hdr.ether.type = NETWIB_ETHERHDRTYPE_IP4;
    } else {
      linkheader.hdr.ether.type = NETWIB_ETHERHDRTYPE_IP6;
    }
    netwib_er(netwib_pkt_append_linkhdr(&linkheader, &ptracertmethod->pkt));
    break;
  default :
    break;
  }

  /* construct packet */
  switch(ptracertmethod->method) {
  case NETWOX_TRACERT_METHOD_PING :
  case NETWOX_TRACERT_METHOD_PING_ETHIPSPOOF :
    netwib_er(netwib_iphdr_initdefault(ptracertmethod->pdstip->iptype,
                                       &ipheader));
    ipheader.src = ptracertmethod->paddinfo->srcip;
    ipheader.dst = *ptracertmethod->pdstip;
    ipheader.ttl = (netwib_uint8)ptracertmethod->ttl;
    if (ptracertmethod->pdstip->iptype == NETWIB_IPTYPE_IP4) {
      netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_ECHOREQ, &icmp4));
      icmp4.msg.echo.id = (netwib_uint16)(ptracertmethod->iduint & 0xFF);
      icmp4.msg.echo.seqnum = (netwib_uint16)(ptracertmethod->iduint >> 16);
      netwib_er(netwib_buf_init_ext_string("abcdefghijkm",
                                         &icmp4.msg.echo.data));
      netwib_er(netwib_pkt_append_ipicmp4(&ipheader, &icmp4,
                                          &ptracertmethod->pkt));
    } else {
      netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_ECHOREQ, &icmp6));
      icmp6.msg.echo.id = (netwib_uint16)(ptracertmethod->iduint & 0xFF);
      icmp6.msg.echo.seqnum = (netwib_uint16)(ptracertmethod->iduint >> 16);
      netwib_er(netwib_buf_init_ext_string("abcdefghijkm",
                                         &icmp6.msg.echo.data));
      netwib_er(netwib_pkt_append_ipicmp6(&ipheader, &icmp6,
                                          &ptracertmethod->pkt));
    }
    break;
  case NETWOX_TRACERT_METHOD_TCP :
  case NETWOX_TRACERT_METHOD_TCP_ETHIPSPOOF :
    netwib_er(netwib_iphdr_initdefault(ptracertmethod->pdstip->iptype,
                                       &ipheader));
    ipheader.src = ptracertmethod->paddinfo->srcip;
    ipheader.dst = *ptracertmethod->pdstip;
    ipheader.ttl = (netwib_uint8)ptracertmethod->ttl;
    netwib_er(netwib_tcphdr_initdefault(&tcpheader));
    tcpheader.src = ptracertmethod->idport;
    tcpheader.dst = ptracertmethod->paddinfo->dstport;
    tcpheader.syn = NETWIB_TRUE;
    tcpheader.seqnum = ptracertmethod->iduint;
    netwib_er(netwib_pkt_append_iptcpdata(&ipheader, &tcpheader, NULL,
                                          &ptracertmethod->pkt));
    break;
  case NETWOX_TRACERT_METHOD_UDP :
  case NETWOX_TRACERT_METHOD_UDP_ETHIPSPOOF :
    netwib_er(netwib_iphdr_initdefault(ptracertmethod->pdstip->iptype,
                                       &ipheader));
    ipheader.src = ptracertmethod->paddinfo->srcip;
    ipheader.dst = *ptracertmethod->pdstip;
    ipheader.ttl = (netwib_uint8)ptracertmethod->ttl;
    netwib_er(netwib_udphdr_initdefault(&udpheader));
    udpheader.src = ptracertmethod->idport;
    udpheader.dst = ptracertmethod->paddinfo->dstport;
    netwib_er(netwib_pkt_append_ipudpdata(&ipheader, &udpheader, NULL,
                                           &ptracertmethod->pkt));
    break;
  case NETWOX_TRACERT_METHOD_PROTO :
  case NETWOX_TRACERT_METHOD_PROTO_ETHIPSPOOF :
    netwib_er(netwib_iphdr_initdefault(ptracertmethod->pdstip->iptype,
                                       &ipheader));
    ipheader.src = ptracertmethod->paddinfo->srcip;
    ipheader.dst = *ptracertmethod->pdstip;
    ipheader.ttl = (netwib_uint8)ptracertmethod->ttl;
    ipheader.protocol = ptracertmethod->paddinfo->ipproto;
    netwib_er(netwib_pkt_append_ipdata(&ipheader,
                                       &ptracertmethod->paddinfo->ipdata,
                                       &ptracertmethod->pkt));
    break;
  }

  /* send it */
  netwib_er(netwib_io_write(ptracertmethod->pio, &ptracertmethod->pkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_tracert_method_reply(netwox_priv_tracert_method *ptracertmethod,
                                              netwib_bool *pisrouter,
                                              netwib_bool *pisdst,
                                              netwib_ip *phopip,
                                              netwib_bool *pportisopen)
{
  netwib_linkhdr linkheader;
  netwib_iphdr ipheader;
  netwib_tcphdr tcpheader;
  netwib_icmp4 icmp4;
  netwib_icmp6 icmp6;
  netwib_ipproto ipproto;
  netwib_port udpsrc, udpdst;
  netwib_cmp cmp;
  netwib_bool answered, routerreached;
  netwib_err ret;

  *pisrouter = NETWIB_FALSE;
  *pisdst = NETWIB_FALSE;

  /* for compiler warning */
  icmp4.type = 0;
  icmp4.code = 0;

  /* in those cases, first decode an Eth header */
  switch(ptracertmethod->method) {
  case NETWOX_TRACERT_METHOD_PING_ETHIPSPOOF :
  case NETWOX_TRACERT_METHOD_TCP_ETHIPSPOOF :
  case NETWOX_TRACERT_METHOD_UDP_ETHIPSPOOF :
  case NETWOX_TRACERT_METHOD_PROTO_ETHIPSPOOF :
    netwib_er(netwox_alive_ether(ptracertmethod->pio,
                                 &ptracertmethod->paddinfo->srceth,
                                 &ptracertmethod->paddinfo->srcip,
                                 &ptracertmethod->pkt, NETWIB_TRUE,
                                 NETWIB_FALSE, NETWIB_FALSE, &answered));
    if (answered) return(NETWIB_ERR_OK);
    netwib_er(netwib_pkt_decode_layer_link(NETWIB_DEVICE_DLTTYPE_ETHER,
                                           &ptracertmethod->pkt, &linkheader));
    if (linkheader.hdr.ether.type != NETWIB_ETHERHDRTYPE_IP4 &&
        linkheader.hdr.ether.type != NETWIB_ETHERHDRTYPE_IP6) {
      return(NETWIB_ERR_OK);
    }
    netwib_er(netwib_eth_cmp(&linkheader.hdr.ether.dst,
                             &ptracertmethod->paddinfo->srceth, &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    break;
  default :
    break;
  }

  /* now, we must have an IP header */
  netwib_er(netwib_pkt_decode_layer_ip(&ptracertmethod->pkt, &ipheader));
  netwib_er(netwib_ip_cmp(&ipheader.dst, &ptracertmethod->paddinfo->srcip,
                          &cmp));
  if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
  netwib_er(netwib_iphdr_get_proto(&ipheader, &ipproto));

  /* check if a router was reached */
  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_ECHOREQ, &icmp4));
  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_ECHOREQ, &icmp6));
  routerreached = NETWIB_FALSE;
  if (ipproto == NETWIB_IPPROTO_ICMP4) {
    ret = netwib_pkt_decode_layer_icmp4(&ptracertmethod->pkt, &icmp4);
    if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
    if (icmp4.type == NETWIB_ICMP4TYPE_TIMEEXCEED &&
        icmp4.code == NETWIB_ICMP4CODE_TIMEEXCEED_TTL) {
      *phopip = ipheader.src;
      ret = netwib_ip64bits_decode_ipdata(&icmp4.msg.timeexceed.badippacket,
                                          &ipheader, NULL);
      if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
      routerreached = NETWIB_TRUE;
    }
  } else if (ipproto == NETWIB_IPPROTO_ICMP6) {
    ret = netwib_pkt_decode_layer_icmp6(&ptracertmethod->pkt, &icmp6);
    if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
    if (icmp6.type == NETWIB_ICMP6TYPE_TIMEEXCEED &&
        icmp6.code == NETWIB_ICMP6CODE_TIMEEXCEED_TTL) {
      *phopip = ipheader.src;
      ret = netwib_ip64bits_decode_ipdata(&icmp6.msg.timeexceed.badippacket,
                                          &ipheader, NULL);
      if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
      routerreached = NETWIB_TRUE;
    }
  }
  if (routerreached) {
    netwib_er(netwib_ip_cmp(&ipheader.src, &ptracertmethod->paddinfo->srcip,
                            &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    netwib_er(netwib_ip_cmp(&ipheader.dst, ptracertmethod->pdstip, &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    *pisrouter = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  /* check if dst was reached */
  switch(ptracertmethod->method) {
  case NETWOX_TRACERT_METHOD_PING :
  case NETWOX_TRACERT_METHOD_PING_ETHIPSPOOF :
    if (ipproto == NETWIB_IPPROTO_ICMP4) {
      netwib_er(netwib_ip_cmp(&ipheader.src, ptracertmethod->pdstip, &cmp));
      if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
      if (icmp4.type != NETWIB_ICMP4TYPE_ECHOREP) return(NETWIB_ERR_OK);
      if (icmp4.code != NETWIB_ICMP4CODE_ECHOREP) return(NETWIB_ERR_OK);
      if (icmp4.msg.echo.id != (ptracertmethod->iduint & 0xFF)) {
        return(NETWIB_ERR_OK);
      }
      if (icmp4.msg.echo.seqnum != (ptracertmethod->iduint >> 16)) {
        return(NETWIB_ERR_OK);
      }
    } else if (ipproto == NETWIB_IPPROTO_ICMP6) {
      netwib_er(netwib_ip_cmp(&ipheader.src, ptracertmethod->pdstip, &cmp));
      if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
      if (icmp6.type != NETWIB_ICMP6TYPE_ECHOREP) return(NETWIB_ERR_OK);
      if (icmp6.code != NETWIB_ICMP6CODE_ECHOREP) return(NETWIB_ERR_OK);
      if (icmp6.msg.echo.id != (ptracertmethod->iduint & 0xFF)) {
        return(NETWIB_ERR_OK);
      }
      if (icmp6.msg.echo.seqnum != (ptracertmethod->iduint >> 16)) {
        return(NETWIB_ERR_OK);
      }
    } else {
      return(NETWIB_ERR_OK);
    }
    /* do not check icmp data, suppose it's ok */
    *pisdst = NETWIB_TRUE;
    *phopip = ipheader.src;
    *pportisopen = NETWIB_TRUE; /* not really needed */
    break;
  case NETWOX_TRACERT_METHOD_TCP :
  case NETWOX_TRACERT_METHOD_TCP_ETHIPSPOOF :
    if (ipproto != NETWIB_IPPROTO_TCP) return(NETWIB_ERR_OK);
    ret = netwib_pkt_decode_layer_tcp(&ptracertmethod->pkt, &tcpheader);
    if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
    netwib_er(netwib_ip_cmp(&ipheader.src, ptracertmethod->pdstip, &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    if (tcpheader.src != ptracertmethod->paddinfo->dstport) {
      return(NETWIB_ERR_OK);
    }
    if (tcpheader.dst != ptracertmethod->idport) return(NETWIB_ERR_OK);
    if (tcpheader.acknum != ptracertmethod->iduint+1) return(NETWIB_ERR_OK);
    if (tcpheader.syn && tcpheader.ack) {
      /* open */
      *pisdst = NETWIB_TRUE;
      *phopip = ipheader.src;
      *pportisopen = NETWIB_TRUE;
      if (ptracertmethod->method ==  NETWOX_TRACERT_METHOD_TCP_ETHIPSPOOF) {
        /* now, send a RST, otherwise, syn flood the host */
        netwib_er(netwox_alive_linkiptcpdata(ptracertmethod->pio,
                                             &ptracertmethod->paddinfo->srceth,
                                             &ptracertmethod->paddinfo->srcip,
                                             &linkheader, &ipheader,
                                             &tcpheader, &ptracertmethod->pkt,
                                             NULL));
      }
    } else if (tcpheader.rst && tcpheader.ack) {
      /* closed, but accept it */
      *pisdst = NETWIB_TRUE;
      *phopip = ipheader.src;
      *pportisopen = NETWIB_FALSE;
    }
    break;
  case NETWOX_TRACERT_METHOD_UDP :
  case NETWOX_TRACERT_METHOD_UDP_ETHIPSPOOF :
    if (ipproto == NETWIB_IPPROTO_ICMP4) {
      if (icmp4.type != NETWIB_ICMP4TYPE_DSTUNREACH) {
        return(NETWIB_ERR_OK);
      }
      if (icmp4.code != NETWIB_ICMP4CODE_DSTUNREACH_PORT) {
        return(NETWIB_ERR_OK);
      }
      ret = netwib_ip64bits_decode_ipudp(&icmp4.msg.dstunreach.badippacket,
                                         &ipheader, &udpsrc, &udpdst);
      if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
    } else if (ipproto == NETWIB_IPPROTO_ICMP6) {
      if (icmp6.type != NETWIB_ICMP6TYPE_DSTUNREACH) {
        return(NETWIB_ERR_OK);
      }
      if (icmp6.code != NETWIB_ICMP6CODE_DSTUNREACH_PORT) {
        return(NETWIB_ERR_OK);
      }
      ret = netwib_ip64bits_decode_ipudp(&icmp6.msg.dstunreach.badippacket,
                                         &ipheader, &udpsrc, &udpdst);
      if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
    } else {
      return(NETWIB_ERR_OK);
    }
    netwib_er(netwib_ip_cmp(&ipheader.src, &ptracertmethod->paddinfo->srcip,
                            &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    netwib_er(netwib_ip_cmp(&ipheader.dst, ptracertmethod->pdstip, &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    if (udpsrc != ptracertmethod->idport) return(NETWIB_ERR_OK);
    if (udpdst != ptracertmethod->paddinfo->dstport) return(NETWIB_ERR_OK);
    *pisdst = NETWIB_TRUE;
    *phopip = ipheader.dst;
    *pportisopen = NETWIB_FALSE; /* if answer, it's closed */
    break;
  case NETWOX_TRACERT_METHOD_PROTO :
  case NETWOX_TRACERT_METHOD_PROTO_ETHIPSPOOF :
    /* we have nothing to check dst, because we do not know
       how the protocol is implemented. We only report error */
    if (ipproto == NETWIB_IPPROTO_ICMP4) {
      if (icmp4.type != NETWIB_ICMP4TYPE_DSTUNREACH) {
        return(NETWIB_ERR_OK);
      }
      *phopip = ipheader.src;
      ret = netwib_ip64bits_decode_ipdata(&icmp4.msg.dstunreach.badippacket,
                                          &ipheader, NULL);
      if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
    } else if (ipproto == NETWIB_IPPROTO_ICMP6) {
      if (icmp6.type != NETWIB_ICMP6TYPE_DSTUNREACH &&
          icmp6.type != NETWIB_ICMP6TYPE_PARAPROB) {
        return(NETWIB_ERR_OK);
      }
      *phopip = ipheader.src;
      ret = netwib_ip64bits_decode_ipdata(&icmp6.msg.dstunreach.badippacket,
                                          &ipheader, NULL);
      if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
    } else {
      return(NETWIB_ERR_OK);
    }
    netwib_er(netwib_ip_cmp(&ipheader.src, &ptracertmethod->paddinfo->srcip,
                            &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    netwib_er(netwib_ip_cmp(&ipheader.dst, ptracertmethod->pdstip, &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    netwib_er(netwib_fmt_display("Note : %{ip} sent us an error : stop here\n",
                                 phopip));
    *pisdst = NETWIB_TRUE;
    *pportisopen = NETWIB_FALSE; /* don't know but safe*/
    break;
  }

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwox_tracert_loop(netwox_tracert_method method,
                               netwib_constip *pdstip,
                               netwox_tracert_addinfo *paddinfo,
                               netwib_ring *presult)
{
  netwox_priv_tracert_method tracertmethod;
  netwib_bool event, isrouter, isdst, portisopen;
  netwib_ip hopip;
  netwib_buf hn;
  netwib_time t;
  netwib_err ret;

  /* initialize depending on method */
  netwib_er(netwox_tracert_method_init(method, pdstip, paddinfo,
                                       &tracertmethod));

  /* main loop */
  isdst = NETWIB_FALSE;
  event = NETWIB_FALSE;
  portisopen=NETWIB_FALSE;
  tracertmethod.ttl = paddinfo->minttl;
  while (NETWIB_TRUE) {
    /* send a request */
    netwib_er(netwox_tracert_method_request(&tracertmethod));
    /* sniff and expect a reply */
    netwib_er(netwib_time_init_now(&t));
    netwib_er(netwib_time_plus_msec(&t, paddinfo->maxwaitms));
    while (NETWIB_TRUE) {
      /* sniff */
      netwib_er(netwib_io_wait_read(tracertmethod.pio, &t, &event));
      if (!event) break;
      netwib__buf_reinit(&tracertmethod.pkt);
      ret = netwib_io_read(tracertmethod.pio, &tracertmethod.pkt);
      if (ret == NETWIB_ERR_DATANOTAVAIL) {
        ret = NETWIB_ERR_OK;
        continue;
      }
      netwib_er(ret);
      /* check it corresponds */
      netwib_er(netwox_tracert_method_reply(&tracertmethod, &isrouter, &isdst,
                                            &hopip, &portisopen));
      if (isrouter || isdst) {
        netwib_er(netwib_fmt_display("%{uint32} : %{ip}", tracertmethod.ttl,
                                     &hopip));
        if (presult != NULL) {
          netwib_er(netwox_tracertdisc_hoplist_ring_add(presult,
                                                        tracertmethod.ttl,
                                                        &hopip, isdst,
                                                        portisopen,
                                                        NETWIB_FALSE));
        }
        if (paddinfo->resolve) {
          netwib_er(netwib_buf_init_mallocdefault(&hn));
          ret = netwib_buf_append_ip(&hopip, NETWIB_IP_ENCODETYPE_HNS, &hn);
          if (ret != NETWIB_ERR_OK) {
            netwib__buf_reinit(&hn);
            netwib_er(netwib_buf_append_string("unresolved", &hn));
          }
          netwib_er(netwib_fmt_display(" (%{buf})", &hn));
          netwib_er(netwib_buf_close(&hn));
        }
        netwib_er(netwib_fmt_display("\n"));
        break;
      }
    }
    tracertmethod.ttl++;
    if (isdst) {
      break;
    }
    if (tracertmethod.ttl > paddinfo->maxttl) {
      if (presult != NULL) {
        netwib_er(netwox_tracertdisc_hoplist_ring_add(presult,
                                                      tracertmethod.ttl,
                                                      pdstip, NETWIB_FALSE,
                                                      NETWIB_FALSE,
                                                      NETWIB_TRUE));
      }
      break;
    }
  }

  /* close */
  netwib_er(netwox_tracert_method_close(&tracertmethod));

  return(NETWIB_ERR_OK);
}

