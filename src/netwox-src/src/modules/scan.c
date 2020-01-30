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
netwib_err netwox_scan_info_initdefault(netwox_scan_info *pinfo)
{
  netwib_buf_init_ext_empty(&pinfo->device);
  pinfo->srcethset = NETWIB_FALSE;
  pinfo->srcipset = NETWIB_FALSE;
  pinfo->mindelayms = 0;
  pinfo->maxwaitms = 0;
  pinfo->onlyuseful = NETWIB_TRUE;
  pinfo->numtargets = 20;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bool inuse;
  netwib_time expireat;
  netwib_eth dsteth;
  netwib_ip dstip;
  netwib_port dstport;
  netwib_uint32 iduint;
  netwib_uint32 idport;
} netwox_scan_target;

#define NETWOX_SCAN_TARGET_MAX 200
typedef struct {
  netwox_scan_method method;
  netwox_scan_info *pinfo;
  netwib_io *pio;
  netwib_buf pkt;
  netwib_buf pkt1;
  netwib_buf pkt2;
  netwib_ips_index *pipsindex;
  netwib_ports_index *pportsindex;
  netwib_bool lasttargettried;
  netwib_uint32 numtargets;
  netwox_scan_target targets[NETWOX_SCAN_TARGET_MAX];
  netwib_time donotsendnextpacketbefore;
} netwox_scan;

/*-------------------------------------------------------------*/
static netwib_err netwox_scan_init(netwox_scan_method method,
                                   netwox_scan_info *pinfo,
                                   netwox_scan *pscan)
{
  netwib_buf systemdevice, filter, *pdevice=NULL;
  netwib_ip dstip;
  netwib_uint32 i;
  netwib_bool supportip6;

  /* set info */
  pscan->method = method;
  pscan->pinfo = pinfo;
  pscan->numtargets = pinfo->numtargets;

  /* parameter verification */
  switch(method) {
  case NETWOX_SCAN_METHOD_PING_ETHIPSPOOF :
  case NETWOX_SCAN_METHOD_TCP_ETHIPSPOOF :
  case NETWOX_SCAN_METHOD_UDP_ETHIPSPOOF :
  case NETWOX_SCAN_METHOD_ARP_ETHIPSPOOF :
  case NETWOX_SCAN_METHOD_ICMP6ND_ETHIPSPOOF :
    if (netwib__buf_ref_data_size(&pinfo->device) == 0) {
      netwib_er(netwib_fmt_display("device must be set\n"));
      return(NETWOX_ERR_TOOLARG_INVALID);
    }
    if (!pinfo->srcethset) {
      netwib_er(netwib_fmt_display("src-eth must be set\n"));
      return(NETWOX_ERR_TOOLARG_INVALID);
    }
    if (!pinfo->srcipset) {
      netwib_er(netwib_fmt_display("src-ip must be set\n"));
      return(NETWOX_ERR_TOOLARG_INVALID);
    }
    break;
  default :
    break;
  }

  /* initialize ips index (also obtain first destination) */
  netwib_er(netwib_ips_index_init(pinfo->pdstips, &pscan->pipsindex));
  netwib_er(netwib_ips_index_next_ip(pscan->pipsindex, &dstip));
  netwib_er(netwib_ips_index_ctl_set_rewind(pscan->pipsindex));

  /* initialize spoof/sniff io */
  netwib_er(netwib_buf_init_mallocdefault(&systemdevice));
  netwib_er(netwib_buf_init_mallocdefault(&filter));
  switch(method) {
  case NETWOX_SCAN_METHOD_PING :
  case NETWOX_SCAN_METHOD_TCP :
  case NETWOX_SCAN_METHOD_UDP :
    /* obtain legitimate srcip (suppose it will not change) */
    netwib_er(netwib_conf_routes_reach_ip(&dstip, &systemdevice,
                                          &pinfo->srcip));
    /* initialize sniff/spoof io */
    netwib_er(netwox_pcapfilt_supports_ip6(&systemdevice, &supportip6));
    if (supportip6 || pinfo->srcip.iptype == NETWIB_IPTYPE_IP4) {
      netwib_er(netwib_buf_append_fmt(&filter, "host %{ip}", &pinfo->srcip));
    }
    netwib_er(netwox_snispo_init_ip(&systemdevice, &filter,
                                    NETWIB_FALSE, NETWIB_FALSE,
                                    pinfo->spoofipinittype, &pscan->pio));
    break;
  case NETWOX_SCAN_METHOD_PING_ETHIPSPOOF :
  case NETWOX_SCAN_METHOD_TCP_ETHIPSPOOF :
  case NETWOX_SCAN_METHOD_UDP_ETHIPSPOOF :
    /* initialize sniff/spoof io */
    netwib_er(netwox_pcapfilt_supports_ip6(&pinfo->device, &supportip6));
    if (pinfo->srcip.iptype == NETWIB_IPTYPE_IP4) {
      netwib_er(netwib_buf_append_fmt(&filter, "arp or host %{ip}",
                                      &pinfo->srcip));
    } else {
      if (supportip6) {
        netwib_er(netwib_buf_append_fmt(&filter, "icmp6 or host %{ip}",
                                        &pinfo->srcip));
      }
    }
    netwib_er(netwox_snispo_init_eth(&pinfo->device, &filter, &pscan->pio));
    break;
  case NETWOX_SCAN_METHOD_ARP :
  case NETWOX_SCAN_METHOD_ARP_ETHIPSPOOF :
    if (dstip.iptype != NETWIB_IPTYPE_IP4) {
      netwib_er(netwib_fmt_display("ARP method is not valid for IPv6\n"));
      return(NETWOX_ERR_TOOLARG_INVALID);
    }
    pdevice = &pinfo->device;
    if (method == NETWOX_SCAN_METHOD_ARP) {
      /* obtain legitimate route */
      netwib_er(netwib_conf_routes_reach_ip(&dstip, &systemdevice,
                                            &pinfo->srcip));
      netwib_er(netwib_eth_init_ip(&pinfo->srcip, &pinfo->srceth));
      pdevice = &systemdevice;
    }
    /* initialize io */
    netwib_er(netwib_buf_append_fmt(&filter, "arp"));
    netwib_er(netwox_snispo_init_eth(pdevice, &filter, &pscan->pio));
    break;
  case NETWOX_SCAN_METHOD_ICMP6ND :
  case NETWOX_SCAN_METHOD_ICMP6ND_ETHIPSPOOF :
    if (dstip.iptype != NETWIB_IPTYPE_IP6) {
      netwib_er(netwib_fmt_display("ARP method is not valid for IPv4\n"));
      return(NETWOX_ERR_TOOLARG_INVALID);
    }
    pdevice = &pinfo->device;
    if (method == NETWOX_SCAN_METHOD_ICMP6ND) {
      /* obtain legitimate route */
      netwib_er(netwib_conf_routes_reach_ip(&dstip, &systemdevice,
                                            &pinfo->srcip));
      netwib_er(netwib_eth_init_ip(&pinfo->srcip, &pinfo->srceth));
      pdevice = &systemdevice;
    }
    /* initialize io */
    netwib_er(netwox_pcapfilt_supports_ip6(pdevice, &supportip6));
    if (supportip6) {
      netwib_er(netwib_buf_append_fmt(&filter, "icmp6"));
    }
    netwib_er(netwox_snispo_init_eth(pdevice, &filter, &pscan->pio));
    break;
  }
  netwib_er(netwib_buf_close(&filter));
  netwib_er(netwib_buf_close(&systemdevice));

  /* initialize temporary buffer */
  netwib_er(netwib_buf_init_mallocdefault(&pscan->pkt));
  netwib_er(netwib_buf_init_mallocdefault(&pscan->pkt1));
  netwib_er(netwib_buf_init_mallocdefault(&pscan->pkt2));

  /* initialize for loop */
  pscan->lasttargettried = NETWIB_FALSE;
  for (i = 0; i < pscan->numtargets; i++) {
    pscan->targets[i].inuse = NETWIB_FALSE;
  }
  netwib_er(netwib_time_init_sec(0, &pscan->donotsendnextpacketbefore));

  /* initialize port index, and point to first ip (needed later) */
  switch(method) {
    case NETWOX_SCAN_METHOD_TCP :
    case NETWOX_SCAN_METHOD_UDP :
    case NETWOX_SCAN_METHOD_TCP_ETHIPSPOOF :
    case NETWOX_SCAN_METHOD_UDP_ETHIPSPOOF :
      netwib_er(netwib_ports_index_init(pinfo->pdstports,
                                        &pscan->pportsindex));
      netwib_er(netwib_ips_index_next_ip(pscan->pipsindex, NULL));
      break;
    default :
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_scan_close(netwox_scan *pscan)
{

  netwib_er(netwib_ips_index_close(&pscan->pipsindex));
  switch(pscan->method) {
    case NETWOX_SCAN_METHOD_TCP :
    case NETWOX_SCAN_METHOD_UDP :
    case NETWOX_SCAN_METHOD_TCP_ETHIPSPOOF :
    case NETWOX_SCAN_METHOD_UDP_ETHIPSPOOF :
      netwib_er(netwib_ports_index_close(&pscan->pportsindex));
      break;
    default :
      break;
  }

  netwib_er(netwib_buf_close(&pscan->pkt2));
  netwib_er(netwib_buf_close(&pscan->pkt1));
  netwib_er(netwib_buf_close(&pscan->pkt));
  netwib_er(netwib_io_close(&pscan->pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_SCAN_TARGET_DISPLAY_OPEN = 1,
  NETWIB_SCAN_TARGET_DISPLAY_CLOSED,
  NETWIB_SCAN_TARGET_DISPLAY_TIMEOUT
} netwib_scan_target_display;
static netwib_err netwox_scan_target_display(netwox_scan *pscan,
                                             netwox_scan_target *ptarget,
                                             netwib_scan_target_display disp,
                                             netwib_consteth *peth)
{
  switch(pscan->method) {
    case NETWOX_SCAN_METHOD_TCP :
    case NETWOX_SCAN_METHOD_TCP_ETHIPSPOOF :
      switch(disp) {
        case NETWIB_SCAN_TARGET_DISPLAY_OPEN :
          netwib_er(netwib_fmt_display("%{ip} - %{port} : open\n",
                                       &ptarget->dstip, ptarget->dstport));
          break;
        case NETWIB_SCAN_TARGET_DISPLAY_CLOSED :
          if (!pscan->pinfo->onlyuseful) {
            netwib_er(netwib_fmt_display("%{ip} - %{port} : closed\n",
                                         &ptarget->dstip, ptarget->dstport));
          }
          break;
        case NETWIB_SCAN_TARGET_DISPLAY_TIMEOUT :
          if (!pscan->pinfo->onlyuseful) {
            netwib_er(netwib_fmt_display("%{ip} - %{port} : timeout\n",
                                         &ptarget->dstip, ptarget->dstport));
          }
          break;
      }
      break;
    case NETWOX_SCAN_METHOD_UDP :
    case NETWOX_SCAN_METHOD_UDP_ETHIPSPOOF :
      switch(disp) {
        case NETWIB_SCAN_TARGET_DISPLAY_OPEN :
          return(NETWIB_ERR_PAINVALIDTYPE);
          break;
        case NETWIB_SCAN_TARGET_DISPLAY_CLOSED :
          if (!pscan->pinfo->onlyuseful) {
            netwib_er(netwib_fmt_display("%{ip} - %{port} : closed\n",
                                         &ptarget->dstip, ptarget->dstport));
          }
          break;
        case NETWIB_SCAN_TARGET_DISPLAY_TIMEOUT :
          netwib_er(netwib_fmt_display("%{ip} - %{port} : timeout (perhaps open)\n", &ptarget->dstip, ptarget->dstport));
          break;
      }
      break;
    case NETWOX_SCAN_METHOD_PING :
    case NETWOX_SCAN_METHOD_PING_ETHIPSPOOF :
      switch(disp) {
        case NETWIB_SCAN_TARGET_DISPLAY_OPEN :
          netwib_er(netwib_fmt_display("%{ip} : reached\n", &ptarget->dstip));
          break;
        case NETWIB_SCAN_TARGET_DISPLAY_CLOSED :
          return(NETWIB_ERR_PAINVALIDTYPE);
          break;
        case NETWIB_SCAN_TARGET_DISPLAY_TIMEOUT :
          if (!pscan->pinfo->onlyuseful) {
            netwib_er(netwib_fmt_display("%{ip} : unreached\n",
                                         &ptarget->dstip));
          }
          break;
      }
      break;
    case NETWOX_SCAN_METHOD_ARP :
    case NETWOX_SCAN_METHOD_ARP_ETHIPSPOOF :
    case NETWOX_SCAN_METHOD_ICMP6ND :
    case NETWOX_SCAN_METHOD_ICMP6ND_ETHIPSPOOF :
      switch(disp) {
        case NETWIB_SCAN_TARGET_DISPLAY_OPEN :
          netwib_er(netwib_fmt_display("%{ip} : %{eth}\n", &ptarget->dstip,
                                       peth));
          break;
        case NETWIB_SCAN_TARGET_DISPLAY_CLOSED :
          return(NETWIB_ERR_PAINVALIDTYPE);
          break;
        case NETWIB_SCAN_TARGET_DISPLAY_TIMEOUT :
          if (!pscan->pinfo->onlyuseful) {
            netwib_er(netwib_fmt_display("%{ip} : unreached\n",
                                         &ptarget->dstip));
          }
          break;
      }
      break;
  }


  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_scan_request(netwox_scan *pscan,
                                      netwox_scan_target *ptarget)
{
  netwib_linkhdr linkheader;
  netwib_iphdr ipheader;
  netwib_udphdr udpheader;
  netwib_tcphdr tcpheader;
  netwib_icmp4 icmp4;
  netwib_icmp6 icmp6;
  netwib_arphdr arpheader;
  netwib_bool alsoport;
  netwib_err ret;

  /* first, obtain next target */
  alsoport = NETWIB_TRUE;
 try_next:
  switch(pscan->method) {
    case NETWOX_SCAN_METHOD_TCP :
    case NETWOX_SCAN_METHOD_UDP :
    case NETWOX_SCAN_METHOD_TCP_ETHIPSPOOF :
    case NETWOX_SCAN_METHOD_UDP_ETHIPSPOOF :
      ret = NETWIB_ERR_DATAEND;
      if (alsoport) {
        ret = netwib_ports_index_next_port(pscan->pportsindex,
                                           &ptarget->dstport);
      }
      if (ret == NETWIB_ERR_OK) {
        netwib_er(netwib_ips_index_this_ip(pscan->pipsindex, &ptarget->dstip));
      } else if (ret == NETWIB_ERR_DATAEND) {
        ret = netwib_ips_index_next_ip(pscan->pipsindex, &ptarget->dstip);
        if (ret == NETWIB_ERR_OK) {
          netwib_er(netwib_ports_index_ctl_set_rewind(pscan->pportsindex));
          netwib_er(netwib_ports_index_next_port(pscan->pportsindex,
                                                 &ptarget->dstport));
        } else if (ret == NETWIB_ERR_DATAEND) {
          pscan->lasttargettried = NETWIB_TRUE;
          return(NETWIB_ERR_OK);
        } else {
          return(ret);
        }
      } else {
        return(ret);
      }
      break;
    case NETWOX_SCAN_METHOD_PING :
    case NETWOX_SCAN_METHOD_ARP :
    case NETWOX_SCAN_METHOD_ICMP6ND :
    case NETWOX_SCAN_METHOD_PING_ETHIPSPOOF :
    case NETWOX_SCAN_METHOD_ARP_ETHIPSPOOF :
    case NETWOX_SCAN_METHOD_ICMP6ND_ETHIPSPOOF :
      ret = netwib_ips_index_next_ip(pscan->pipsindex, &ptarget->dstip);
      if (ret == NETWIB_ERR_DATAEND) {
        pscan->lasttargettried = NETWIB_TRUE;
        return(NETWIB_ERR_OK);
      } else if (ret != NETWIB_ERR_OK) {
        return(ret);
      }
      break;
  }

  /* in those cases, we also need dsteth */
  switch(pscan->method) {
    case NETWOX_SCAN_METHOD_TCP_ETHIPSPOOF :
    case NETWOX_SCAN_METHOD_UDP_ETHIPSPOOF :
    case NETWOX_SCAN_METHOD_PING_ETHIPSPOOF :
    case NETWOX_SCAN_METHOD_ARP_ETHIPSPOOF :
    case NETWOX_SCAN_METHOD_ICMP6ND_ETHIPSPOOF :
      ret = netwib_conf_routes_reach_eth(&ptarget->dstip, NULL, NULL,
                                         &ptarget->dsteth, NULL);
      if (ret != NETWIB_ERR_OK) {
        netwib_er(netwib_fmt_display("Warning : Eth address needed to reach %{ip} is unknown\n", &ptarget->dstip));
        /* a bit tricky, but nicer than an ugly loop */
        alsoport = NETWIB_FALSE;
        goto try_next;
      }
      break;
    default :
      break;
  }

  /* also set additional info in the structure */
  ptarget->inuse = NETWIB_TRUE;
  netwib_er(netwib_uint32_init_rand(0, 0xFFFFFFFFu, &ptarget->iduint));
  netwib_er(netwib_uint32_init_rand(1024, 0xFFFFu, &ptarget->idport));
  if (pscan->pinfo->mindelayms) {
    netwib_er(netwib_time_init_now(&pscan->donotsendnextpacketbefore));
    netwib_er(netwib_time_plus_msec(&pscan->donotsendnextpacketbefore,
                                    pscan->pinfo->mindelayms));
  }

  /* reinit temporary buffers */
  netwib__buf_reinit(&pscan->pkt);
  netwib__buf_reinit(&pscan->pkt1);
  netwib__buf_reinit(&pscan->pkt2);

  /* in those cases, first add an Eth header */
  switch(pscan->method) {
  case NETWOX_SCAN_METHOD_PING_ETHIPSPOOF :
  case NETWOX_SCAN_METHOD_TCP_ETHIPSPOOF :
  case NETWOX_SCAN_METHOD_UDP_ETHIPSPOOF :
    netwib_er(netwib_linkhdr_initdefault(NETWIB_DEVICE_DLTTYPE_ETHER,
                                         &linkheader));
    linkheader.hdr.ether.src = pscan->pinfo->srceth;
    linkheader.hdr.ether.dst = ptarget->dsteth;
    if (ptarget->dstip.iptype == NETWIB_IPTYPE_IP4) {
      linkheader.hdr.ether.type = NETWIB_ETHERHDRTYPE_IP4;
    } else {
      linkheader.hdr.ether.type = NETWIB_ETHERHDRTYPE_IP6;
    }
    netwib_er(netwib_pkt_append_linkhdr(&linkheader, &pscan->pkt));
    break;
  default :
    break;
  }

  /* construct packet */
  switch(pscan->method) {
  case NETWOX_SCAN_METHOD_PING :
  case NETWOX_SCAN_METHOD_PING_ETHIPSPOOF :
    netwib_er(netwib_iphdr_initdefault(ptarget->dstip.iptype, &ipheader));
    ipheader.src = pscan->pinfo->srcip;
    ipheader.dst = ptarget->dstip;
    if (ptarget->dstip.iptype == NETWIB_IPTYPE_IP4) {
      netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_ECHOREQ, &icmp4));
      icmp4.msg.echo.id = (netwib_uint16)(ptarget->iduint & 0xFF);
      icmp4.msg.echo.seqnum = (netwib_uint16)(ptarget->iduint >> 16);
      netwib_er(netwib_buf_init_ext_string("abcdefghijkm",
                                         &icmp4.msg.echo.data));
      netwib_er(netwib_pkt_append_ipicmp4(&ipheader, &icmp4, &pscan->pkt));
    } else {
      netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_ECHOREQ, &icmp6));
      icmp6.msg.echo.id = (netwib_uint16)(ptarget->iduint & 0xFF);
      icmp6.msg.echo.seqnum = (netwib_uint16)(ptarget->iduint >> 16);
      netwib_er(netwib_buf_init_ext_string("abcdefghijkm",
                                         &icmp6.msg.echo.data));
      netwib_er(netwib_pkt_append_ipicmp6(&ipheader, &icmp6, &pscan->pkt));
    }
    break;
  case NETWOX_SCAN_METHOD_TCP :
  case NETWOX_SCAN_METHOD_TCP_ETHIPSPOOF :
    netwib_er(netwib_iphdr_initdefault(ptarget->dstip.iptype, &ipheader));
    ipheader.src = pscan->pinfo->srcip;
    ipheader.dst = ptarget->dstip;
    netwib_er(netwib_tcphdr_initdefault(&tcpheader));
    tcpheader.src = ptarget->idport;
    tcpheader.dst = ptarget->dstport;
    tcpheader.syn = NETWIB_TRUE;
    tcpheader.seqnum = ptarget->iduint;
    netwib_er(netwib_pkt_append_iptcpdata(&ipheader, &tcpheader, NULL,
                                          &pscan->pkt));
    break;
  case NETWOX_SCAN_METHOD_UDP :
  case NETWOX_SCAN_METHOD_UDP_ETHIPSPOOF :
    netwib_er(netwib_iphdr_initdefault(ptarget->dstip.iptype, &ipheader));
    ipheader.src = pscan->pinfo->srcip;
    ipheader.dst = ptarget->dstip;
    netwib_er(netwib_udphdr_initdefault(&udpheader));
    udpheader.src = ptarget->idport;
    udpheader.dst = ptarget->dstport;
    netwib_er(netwib_pkt_append_ipudpdata(&ipheader, &udpheader, NULL,
                                          &pscan->pkt));
    break;
  case NETWOX_SCAN_METHOD_ARP :
  case NETWOX_SCAN_METHOD_ARP_ETHIPSPOOF :
    linkheader.type = NETWIB_DEVICE_DLTTYPE_ETHER;
    linkheader.hdr.ether.src = pscan->pinfo->srceth;
    netwib_er(netwib_eth_init_fields(0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                                     &linkheader.hdr.ether.dst));
    linkheader.hdr.ether.type = NETWIB_ETHERHDRTYPE_ARP;
    netwib_er(netwib_arphdr_initdefault(&arpheader));
    arpheader.op = NETWIB_ARPHDROP_ARPREQ;
    arpheader.ethsrc = pscan->pinfo->srceth;
    arpheader.ipsrc = pscan->pinfo->srcip;
    arpheader.ipdst = ptarget->dstip;
    netwib_er(netwib_pkt_append_linkarp(&linkheader, &arpheader, &pscan->pkt));
    break;
  case NETWOX_PING_METHOD_ICMP6ND :
  case NETWOX_PING_METHOD_ICMP6ND_ETHIPSPOOF :
    netwib_er(netwox_icmp6nd_ns(&pscan->pinfo->srceth,
                                &pscan->pinfo->srcip,
                                &ptarget->dstip, &pscan->pkt));
    break;
  }

  /* send it */
  ret = netwib_io_write(pscan->pio, &pscan->pkt);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_fmt_display("Warning : write to %{ip} failed\n",
                                 &ptarget->dstip));
    ptarget->inuse = NETWIB_FALSE;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_scan_request_ifpossible(netwox_scan *pscan,
                                                 netwib_bool *patleastonewaiting)
{
  netwib_bool needtosettime[NETWOX_SCAN_TARGET_MAX];
  netwib_bool elapsed, containfreeslot;
  netwib_time expireat;
  netwib_uint32 i;

  /* first, check if array contains items in timeout state */
  containfreeslot = NETWIB_FALSE;
  for (i = 0; i < pscan->numtargets; i++) {
    if (pscan->targets[i].inuse) {
      netwib_er(netwib_time_iselapsed(&pscan->targets[i].expireat, &elapsed));
      if (elapsed) {
        netwib_er(netwox_scan_target_display(pscan, &pscan->targets[i],
                                             NETWIB_SCAN_TARGET_DISPLAY_TIMEOUT,
                                             NULL));
        pscan->targets[i].inuse = NETWIB_FALSE;
        containfreeslot = NETWIB_TRUE;
      }
    } else {
      containfreeslot = NETWIB_TRUE;
    }
  }

  /* try to launch more requests */
  if (containfreeslot && !pscan->lasttargettried) {
    for (i = 0; i < pscan->numtargets; i++) {
      needtosettime[i] = NETWIB_FALSE;
      if (!pscan->targets[i].inuse) {
        if (pscan->pinfo->mindelayms) {
          netwib_er(netwib_time_wait_time(&pscan->donotsendnextpacketbefore));
        }
        netwib_er(netwox_scan_request(pscan, &pscan->targets[i]));
        needtosettime[i] = NETWIB_TRUE;
      }
      if (pscan->lasttargettried) break;
    }
    /* set expireeat now. We do not do it when packet is sent, because
       if mindelayms is set, packet might expire before been sniffed. */
    netwib_er(netwib_time_init_now(&expireat));
    netwib_er(netwib_time_plus_msec(&expireat, pscan->pinfo->maxwaitms));
    for (i = 0; i < pscan->numtargets; i++) {
      if (needtosettime[i]) {
        pscan->targets[i].expireat = expireat;
      }
    }
  }

  /* now determine if there is at least an item in use */
  *patleastonewaiting = NETWIB_FALSE;
  for (i = 0; i < pscan->numtargets; i++) {
    if (pscan->targets[i].inuse) {
      *patleastonewaiting = NETWIB_TRUE;
      break;
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
typedef struct {
  netwib_linkhdr linkheader;
  netwib_arphdr arpheader;
  netwib_iphdr ipheader;
  netwib_ipproto ipproto;
  /* tcp */
  netwib_tcphdr tcpheader;
  netwib_bufext tcpdata;
  /* icmp */
  netwib_icmp4 icmp4;
  netwib_icmp6 icmp6;
  /* udp */
  netwib_iphdr icmpipheader;
  netwib_port icmpudpsrc;
  netwib_port icmpudpdst;
} netwox_scan_reply_info;

/*-------------------------------------------------------------*/
static netwib_err netwox_scan_reply_check_target(netwox_scan *pscan,
                                                 netwox_scan_reply_info *pri,
                                                 netwox_scan_target *ptarget,
                                                 netwib_bool *pmatch,
                                                 netwib_scan_target_display *pdisp,
                                                 netwib_eth *peth)
{
  netwib_bool reached;
  netwib_cmp cmp;

  *pmatch = NETWIB_FALSE;

  switch(pscan->method) {
  case NETWOX_SCAN_METHOD_PING :
  case NETWOX_SCAN_METHOD_PING_ETHIPSPOOF :
    netwib_er(netwib_ip_cmp(&pri->ipheader.src, &ptarget->dstip, &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    if (pri->ipproto == NETWIB_IPPROTO_ICMP4) {
      if (pri->icmp4.msg.echo.id != (ptarget->iduint & 0xFF)) {
        return(NETWIB_ERR_OK);
      }
      if (pri->icmp4.msg.echo.seqnum != (ptarget->iduint >> 16)) {
        return(NETWIB_ERR_OK);
      }
    } else {
      if (pri->icmp6.msg.echo.id != (ptarget->iduint & 0xFF)) {
        return(NETWIB_ERR_OK);
      }
      if (pri->icmp6.msg.echo.seqnum != (ptarget->iduint >> 16)) {
        return(NETWIB_ERR_OK);
      }
    }
    *pdisp = NETWIB_SCAN_TARGET_DISPLAY_OPEN;
    break;
  case NETWOX_SCAN_METHOD_TCP :
  case NETWOX_SCAN_METHOD_TCP_ETHIPSPOOF :
    netwib_er(netwib_ip_cmp(&pri->ipheader.src, &ptarget->dstip, &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    if (pri->tcpheader.src != ptarget->dstport) return(NETWIB_ERR_OK);
    if (pri->tcpheader.dst != ptarget->idport) return(NETWIB_ERR_OK);
    if (pri->tcpheader.acknum != ptarget->iduint+1) return(NETWIB_ERR_OK);
    if (pri->tcpheader.syn && pri->tcpheader.ack) {
      *pdisp = NETWIB_SCAN_TARGET_DISPLAY_OPEN;
      if (pscan->method ==  NETWOX_SCAN_METHOD_TCP_ETHIPSPOOF) {
        /* now, send a RST, otherwise, syn flood the host */
        netwib_er(netwox_alive_linkiptcpdata(pscan->pio,
                                             &pscan->pinfo->srceth,
                                             &pscan->pinfo->srcip,
                                             &pri->linkheader,
                                             &pri->ipheader,
                                             &pri->tcpheader, &pri->tcpdata,
                                             NULL));
      }
    } else if (pri->tcpheader.rst && pri->tcpheader.ack) {
      *pdisp = NETWIB_SCAN_TARGET_DISPLAY_CLOSED;
    }
    break;
  case NETWOX_SCAN_METHOD_UDP :
  case NETWOX_SCAN_METHOD_UDP_ETHIPSPOOF :
    netwib_er(netwib_ip_cmp(&pri->icmpipheader.dst, &ptarget->dstip, &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    if (pri->icmpudpsrc != ptarget->idport) return(NETWIB_ERR_OK);
    if (pri->icmpudpdst != ptarget->dstport) return(NETWIB_ERR_OK);
    *pdisp = NETWIB_SCAN_TARGET_DISPLAY_CLOSED;
    break;
  case NETWOX_SCAN_METHOD_ARP :
  case NETWOX_SCAN_METHOD_ARP_ETHIPSPOOF :
    netwib_er(netwib_ip_cmp(&pri->arpheader.ipsrc, &ptarget->dstip, &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    *pdisp = NETWIB_SCAN_TARGET_DISPLAY_OPEN;
    *peth = pri->arpheader.ethsrc;
    break;
  case NETWOX_SCAN_METHOD_ICMP6ND :
  case NETWOX_SCAN_METHOD_ICMP6ND_ETHIPSPOOF :
    netwib_er(netwox_icmp6nd_na(&ptarget->dstip, &pri->linkheader,
                                &pri->ipheader, &pri->icmp6, &reached, peth));
    if (!reached) return(NETWIB_ERR_OK);
    *pdisp = NETWIB_SCAN_TARGET_DISPLAY_OPEN;
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  *pmatch = NETWIB_TRUE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_scan_reply_check(netwox_scan *pscan,
                                          netwox_scan_reply_info *pri)
{
  netwib_scan_target_display disp=0;
  netwib_bool match;
  netwib_eth eth;
  netwib_uint32 i;

  for (i = 0; i < pscan->numtargets; i++) {
    if (pscan->targets[i].inuse) {
      netwib_er(netwox_scan_reply_check_target(pscan, pri, &pscan->targets[i],
                                               &match, &disp, &eth));
      if (match) {
        netwib_er(netwox_scan_target_display(pscan, &pscan->targets[i], disp,
                                             &eth));
        pscan->targets[i].inuse = NETWIB_FALSE;
        break;
      }
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_scan_reply(netwox_scan *pscan)
{
  netwox_scan_reply_info ri;
  netwib_icmp4 icmp4;
  netwib_icmp6 icmp6;
  netwib_cmp cmp;
  netwib_bool answered;
  netwib_err ret;

  /* in those cases, first decode an Eth header */
  switch(pscan->method) {
  case NETWOX_SCAN_METHOD_PING_ETHIPSPOOF :
  case NETWOX_SCAN_METHOD_TCP_ETHIPSPOOF :
  case NETWOX_SCAN_METHOD_UDP_ETHIPSPOOF :
    netwib_er(netwox_alive_ether(pscan->pio,
                                 &pscan->pinfo->srceth,
                                 &pscan->pinfo->srcip,
                                 &pscan->pkt, NETWIB_TRUE,
                                 NETWIB_FALSE, NETWIB_FALSE, &answered));
    if (answered) return(NETWIB_ERR_OK);
    netwib_er(netwib_pkt_decode_layer_link(NETWIB_DEVICE_DLTTYPE_ETHER,
                                           &pscan->pkt, &ri.linkheader));
    if (ri.linkheader.hdr.ether.type != NETWIB_ETHERHDRTYPE_IP4 &&
        ri.linkheader.hdr.ether.type != NETWIB_ETHERHDRTYPE_IP6) {
      return(NETWIB_ERR_OK);
    }
    netwib_er(netwib_eth_cmp(&ri.linkheader.hdr.ether.dst,
                             &pscan->pinfo->srceth, &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    break;
  default :
    break;
  }

  /* decode packet */
  switch(pscan->method) {
  case NETWOX_SCAN_METHOD_PING :
  case NETWOX_SCAN_METHOD_PING_ETHIPSPOOF :
    ret = netwib_pkt_decode_ipicmp4(&pscan->pkt, &ri.ipheader, &ri.icmp4);
    if (ret != NETWIB_ERR_OK) {
      ret = netwib_pkt_decode_ipicmp6(&pscan->pkt, &ri.ipheader, &ri.icmp6);
      if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
    }
    netwib_er(netwib_iphdr_get_proto(&ri.ipheader, &ri.ipproto));
    netwib_er(netwib_ip_cmp(&ri.ipheader.dst, &pscan->pinfo->srcip, &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    if (ri.ipproto == NETWIB_IPPROTO_ICMP4) {
      if (ri.icmp4.type != NETWIB_ICMP4TYPE_ECHOREP) return(NETWIB_ERR_OK);
      if (ri.icmp4.code != NETWIB_ICMP4CODE_ECHOREP) return(NETWIB_ERR_OK);
    } else {
      if (ri.icmp6.type != NETWIB_ICMP6TYPE_ECHOREP) return(NETWIB_ERR_OK);
      if (ri.icmp6.code != NETWIB_ICMP6CODE_ECHOREP) return(NETWIB_ERR_OK);
    }
    break;
  case NETWOX_SCAN_METHOD_TCP :
  case NETWOX_SCAN_METHOD_TCP_ETHIPSPOOF :
    ret = netwib_pkt_decode_iptcpdata(&pscan->pkt, &ri.ipheader,
                                      &ri.tcpheader, &ri.tcpdata);
    if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
    netwib_er(netwib_ip_cmp(&ri.ipheader.dst, &pscan->pinfo->srcip, &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    break;
  case NETWOX_SCAN_METHOD_UDP :
  case NETWOX_SCAN_METHOD_UDP_ETHIPSPOOF :
    icmp6.type = 0; /* compiler warning */
    icmp6.code = 0; /* compiler warning */
    ret = netwib_pkt_decode_ipicmp4(&pscan->pkt, &ri.ipheader, &icmp4);
    if (ret != NETWIB_ERR_OK) {
      ret = netwib_pkt_decode_ipicmp6(&pscan->pkt, &ri.ipheader, &icmp6);
      if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
    }
    netwib_er(netwib_iphdr_get_proto(&ri.ipheader, &ri.ipproto));
    netwib_er(netwib_ip_cmp(&ri.ipheader.dst, &pscan->pinfo->srcip, &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    if (ri.ipproto == NETWIB_IPPROTO_ICMP4) {
      if (icmp4.type != NETWIB_ICMP4TYPE_DSTUNREACH) return(NETWIB_ERR_OK);
      if (icmp4.code != NETWIB_ICMP4CODE_DSTUNREACH_PORT) {
        return(NETWIB_ERR_OK);
      }
      ret = netwib_ip64bits_decode_ipudp(&icmp4.msg.dstunreach.badippacket,
                                         &ri.icmpipheader,
                                         &ri.icmpudpsrc, &ri.icmpudpdst);
      if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
    } else {
      if (icmp6.type != NETWIB_ICMP6TYPE_DSTUNREACH) return(NETWIB_ERR_OK);
      if (icmp6.code != NETWIB_ICMP6CODE_DSTUNREACH_PORT) {
        return(NETWIB_ERR_OK);
      }
      ret = netwib_ip64bits_decode_ipudp(&icmp6.msg.dstunreach.badippacket,
                                         &ri.icmpipheader,
                                         &ri.icmpudpsrc, &ri.icmpudpdst);
      if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
    }
    netwib_er(netwib_ip_cmp(&ri.icmpipheader.src, &pscan->pinfo->srcip,
                            &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    break;
  case NETWOX_SCAN_METHOD_ARP :
  case NETWOX_SCAN_METHOD_ARP_ETHIPSPOOF :
    ret = netwib_pkt_decode_linkarp(NETWIB_DEVICE_DLTTYPE_ETHER,
                                    &pscan->pkt, &ri.linkheader,
                                    &ri.arpheader);
    if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
    netwib_er(netwib_eth_cmp(&ri.linkheader.hdr.ether.dst,
                             &pscan->pinfo->srceth, &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    if (ri.arpheader.op != NETWIB_ARPHDROP_ARPREP) return(NETWIB_ERR_OK);
    netwib_er(netwib_ip_cmp(&ri.arpheader.ipdst, &pscan->pinfo->srcip, &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    netwib_er(netwib_eth_cmp(&ri.arpheader.ethdst, &pscan->pinfo->srceth,
                             &cmp));
    if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
    break;
  case NETWOX_SCAN_METHOD_ICMP6ND :
  case NETWOX_SCAN_METHOD_ICMP6ND_ETHIPSPOOF :
    ret = netwib_pkt_decode_linkipicmp6(NETWIB_DEVICE_DLTTYPE_ETHER,
                                        &pscan->pkt, &ri.linkheader,
                                        &ri.ipheader, &ri.icmp6);
    if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
    break;
  }

  /* check it corresponds */
  netwib_er(netwox_scan_reply_check(pscan, &ri));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_scan_loop(netwox_scan_method method,
                            netwox_scan_info *pinfo)
{
  netwox_scan scan;
  netwib_bool event, atleastonewaiting;
  netwib_time t;
  netwib_err ret;

  /* adapt parameters */
  if (pinfo->numtargets > NETWOX_SCAN_TARGET_MAX) {
    pinfo->numtargets = NETWOX_SCAN_TARGET_MAX;
  }

  /* initialize depending on method */
  netwib_er(netwox_scan_init(method, pinfo, &scan));

  /* main (outer) loop */
  while (NETWIB_TRUE) {
    /* send as much requests as possible */
    netwib_er(netwox_scan_request_ifpossible(&scan, &atleastonewaiting));
    if (!atleastonewaiting) {
      break;
    }
    /* sniff */
    netwib_er(netwib_time_init_now(&t));
    netwib_er(netwib_time_plus_msec(&t, pinfo->maxwaitms));
    while (NETWIB_TRUE) {
      /* wait, and leave inner loop if no answer */
      netwib_er(netwib_io_wait_read(scan.pio, &t, &event));
      if (!event) break;
      /* read */
      netwib__buf_reinit(&scan.pkt);
      ret = netwib_io_read(scan.pio, &scan.pkt);
      if (ret == NETWIB_ERR_DATANOTAVAIL) {
        ret = NETWIB_ERR_OK;
        continue;
      }
      netwib_er(ret);
      /* check it corresponds */
      netwib_er(netwox_scan_reply(&scan));
      /* loop while there are packets in queue */
      netwib_er(netwib_time_init_time(NETWIB_TIME_ZERO, &t));
    }
  }

  /* close */
  netwib_er(netwox_scan_close(&scan));

  return(NETWIB_ERR_OK);
}
