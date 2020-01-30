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
#include "../../netwox.h"

/*-------------------------------------------------------------*/
typedef struct {
  netwox_priv_io_sockv *pisv;
  netwib_eth remoteeth;
  netwib_ip remoteip;
  netwib_port remoteport;
  netwib_bool clientconnected;
  netwib_io *pio;
  netwib_bufpool *pbufpool;
} netwox_sockv_udp;

/*-------------------------------------------------------------*/
static netwib_err netwox_sockv_thread_udp_spoof_frag(netwib_constbuf *pfrag,
                                                     netwib_ptr infos)
{
  netwox_sockv_udp *psvu = (netwox_sockv_udp *)infos;
  netwib_linkhdr linkheader;
  netwib_buf *pethpkt;
  netwib_iptype iptype;

  netwib_er(netwib_bufpool_buf_init(psvu->pbufpool, &pethpkt));

  netwib_er(netwib_linkhdr_initdefault(NETWIB_DEVICE_DLTTYPE_ETHER,
                                       &linkheader));
  linkheader.hdr.ether.src = psvu->pisv->localeth;
  linkheader.hdr.ether.dst = psvu->remoteeth;
  netwib_er(netwox_pkt_iptype_init_ippkt(pfrag, &iptype));
  switch(iptype) {
  case NETWIB_IPTYPE_IP6 :
    linkheader.hdr.ether.type = NETWIB_ETHERHDRTYPE_IP6;
    break;
  default :
    linkheader.hdr.ether.type = NETWIB_ETHERHDRTYPE_IP4;
    break;
  }
  netwib_er(netwib_pkt_append_linkdata(&linkheader, pfrag, pethpkt));

  netwib_er(netwib_io_write(psvu->pio, pethpkt));

  netwib_er(netwib_bufpool_buf_close(psvu->pbufpool, &pethpkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_sockv_thread_udp_spoof(netwox_sockv_udp *psvu,
                                                netwib_constbuf *pudpdata)
{
  netwib_iphdr ipheader;
  netwib_udphdr udpheader;
  netwib_buf *pippkt;
  netwib_uint32 mtu=NETWIB_MTU_MIN_IP4;

  /* for a mulser, we first need to know who is the client */
  if (psvu->pisv->socktype == NETWOX_SOCKTYPE_UDP_MULSER) {
    netwib_er(netwox_sockv_info_read_cli(psvu->pisv, NETWIB_FALSE,
                                         &psvu->remoteeth, &psvu->remoteip,
                                         &psvu->remoteport));
  }

  netwib_er(netwib_bufpool_buf_init(psvu->pbufpool, &pippkt));

  netwib_er(netwib_iphdr_initdefault(psvu->remoteip.iptype, &ipheader));
  ipheader.src = psvu->pisv->localip;
  ipheader.dst = psvu->remoteip;
  if (psvu->remoteip.iptype == NETWIB_IPTYPE_IP4) {
    ipheader.header.ip4.opts = psvu->pisv->ip4opts;
  } else {
    if (netwib__buf_ref_data_size(&psvu->pisv->ip6exts)) {
      ipheader.protocol = psvu->pisv->ip6extsproto;
      ipheader.header.ip6.exts = psvu->pisv->ip6exts;
    }
    mtu = NETWIB_MTU_MIN_IP6;
  }

  netwib_er(netwib_udphdr_initdefault(&udpheader));
  udpheader.src = psvu->pisv->localport;
  udpheader.dst = psvu->remoteport;

  netwib_er(netwib_pkt_append_ipudpdata(&ipheader, &udpheader, pudpdata,
                                        pippkt));

  netwib_er(netwib_ip_frag(pippkt, 0, mtu, &netwox_sockv_thread_udp_spoof_frag,
                           psvu));

  netwib_er(netwib_bufpool_buf_close(psvu->pbufpool, &pippkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_sockv_thread_udp_sniff(netwox_sockv_udp *psvu,
                                                netwib_buf *pnetdata,
                                                netwib_bool *pcanwrite,
                                                netwib_bool *pdataend)
{
  netwib_linkhdr linkheader;
  netwib_iphdr ipheader;
  netwib_udphdr udpheader;
  netwib_icmp4 icmp4;
  netwib_icmp6 icmp6;
  netwib_bool checkremote, answered;
  netwib_ipproto ipproto;
  netwib_port udpsrc, udpdst;
  netwib_cmp cmp;
  netwib_err ret;

  *pcanwrite = NETWIB_FALSE;
  *pdataend = NETWIB_FALSE;

  /* answer alive */
  netwib_er(netwox_alive_ether(psvu->pio, &psvu->pisv->localeth,
                               &psvu->pisv->localip, pnetdata,
                               NETWIB_TRUE, NETWIB_FALSE, NETWIB_FALSE,
                               &answered));
  if (answered) return(NETWIB_ERR_OK);

  /* decode link header */
  netwib_er(netwib_pkt_decode_layer_link(NETWIB_DEVICE_DLTTYPE_ETHER, pnetdata,
                                         &linkheader));
  switch(linkheader.hdr.ether.type) {
    case NETWIB_ETHERHDRTYPE_IP4 :
    case NETWIB_ETHERHDRTYPE_IP6 :
      /* continue below */
      break;
    default :
      return(NETWIB_ERR_OK);
  }

  /* decode ip header */
  checkremote = (netwib_bool)(psvu->pisv->socktype == NETWOX_SOCKTYPE_UDP_CLI||
    (psvu->pisv->socktype==NETWOX_SOCKTYPE_UDP_SER && psvu->clientconnected));
  netwib_er(netwib_pkt_decode_layer_ip(pnetdata, &ipheader));
  netwib_er(netwib_iphdr_get_proto(&ipheader, &ipproto));
  switch(ipproto) {
    case NETWIB_IPPROTO_ICMP4 :
    case NETWIB_IPPROTO_ICMP6 :
      /* check if it's an error for one of our packets */
      if (psvu->pisv->socktype == NETWOX_SOCKTYPE_UDP_MULSER) {
        /* ignore errors for mulser */
        return(NETWIB_ERR_OK);
      }
      if (ipproto == NETWIB_IPPROTO_ICMP4) {
        netwib_er(netwib_pkt_decode_layer_icmp4(pnetdata, &icmp4));
        if (icmp4.type != NETWIB_ICMP4TYPE_DSTUNREACH) {
          return(NETWIB_ERR_OK);
        }
        ret = netwib_ip64bits_decode_ipudp(&icmp4.msg.dstunreach.badippacket,
                                           &ipheader, &udpsrc, &udpdst);
      } else {
        netwib_er(netwib_pkt_decode_layer_icmp6(pnetdata, &icmp6));
        if (icmp6.type != NETWIB_ICMP6TYPE_DSTUNREACH) {
          return(NETWIB_ERR_OK);
        }
        ret = netwib_ip64bits_decode_ipudp(&icmp6.msg.dstunreach.badippacket,
                                           &ipheader, &udpsrc, &udpdst);
      }
      if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
      netwib_er(netwib_ip_cmp(&ipheader.src, &psvu->pisv->localip, &cmp));
      if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
      if (udpsrc != psvu->pisv->localport) return(NETWIB_ERR_OK);
      if (checkremote) {
        netwib_er(netwib_ip_cmp(&ipheader.dst, &psvu->remoteip, &cmp));
        if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
        if (udpdst != psvu->remoteport) return(NETWIB_ERR_OK);
      }
      /* if we are here, it was an unreachable for us : decide it is end */
      *pdataend = NETWIB_TRUE;
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_IPPROTO_UDP :
      /* continue below */
      break;
    default :
      return(NETWIB_ERR_OK);
  }

  /* ensure it's for us (local is checked in sniff filter) */
  netwib_er(netwib_pkt_decode_layer_udp(pnetdata, &udpheader));
  if (checkremote) {
    netwib_er(netwib_eth_cmp(&linkheader.hdr.ether.src,
                             &psvu->remoteeth,&cmp));
    if (cmp != NETWIB_CMP_EQ) {
      return(NETWIB_ERR_OK);
    }
    netwib_er(netwib_ip_cmp(&ipheader.src, &psvu->remoteip, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      return(NETWIB_ERR_OK);
    }
    if (udpheader.src != psvu->remoteport) {
      return(NETWIB_ERR_OK);
    }
  }

  /* set information */
  switch (psvu->pisv->socktype) {
    case NETWOX_SOCKTYPE_UDP_SER :
      /* on first connection to the server, indicates who is the client */
      if (!psvu->clientconnected) {
        psvu->remoteeth = linkheader.hdr.ether.src;
        psvu->remoteip = ipheader.src;
        psvu->remoteport = udpheader.src;
        psvu->clientconnected = NETWIB_TRUE;
        netwib_er(netwox_sockv_info_write_cli(psvu->pisv, NETWIB_TRUE,
                                              &psvu->remoteeth,
                                              &psvu->remoteip,
                                              psvu->remoteport));
      }
      break;
    case NETWOX_SOCKTYPE_UDP_MULSER :
      /* on each connection the the mulser, indicates who is the client */
      netwib_er(netwox_sockv_info_write_cli(psvu->pisv, NETWIB_TRUE,
                                            &linkheader.hdr.ether.src,
                                            &ipheader.src, udpheader.src));
      break;
    default :
      break;
  }

  /* pnetdata start at udp data */
  *pcanwrite = NETWIB_TRUE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_sockv_thread_udp(netwib_ptr infosin,
                                   netwib_ptr *pinfosout)
{
  netwox_sockv_udp svu;
  netwox_priv_io_sockv *pisv = (netwox_priv_io_sockv *)infosin;
  netwib_wait *pwaitusr, *pwaitusrend, *pwaitsniff;
  netwib_uint32 usrendval;
  netwib_buf filter, *pworkbuf;
  netwib_bool eventusr, eventusrend, eventsniff, canwrite, dataend, supportip6;
  netwib_err ret;

  /* initialize variables */
  svu.pisv = pisv;
  switch (pisv->socktype) {
  case NETWOX_SOCKTYPE_UDP_CLI :
    svu.remoteeth = pisv->remoteeth;
    svu.remoteip = pisv->remoteip;
    svu.remoteport = pisv->remoteport;
    break;
  case NETWOX_SOCKTYPE_UDP_SER :
    svu.clientconnected = NETWIB_FALSE;
    break;
  default :
    break;
  }

  /* initialize sniff/spoof io */
  netwib_er(netwib_buf_init_mallocdefault(&filter));
  netwib_er(netwox_pcapfilt_supports_ip6(&pisv->device, &supportip6));
  if (pisv->localip.iptype == NETWIB_IPTYPE_IP4) {
    netwib_er(netwib_buf_append_fmt(&filter, "arp or (ether dst %{eth} and ip dst %{ip} and ((udp and dst port %{port}) or icmp))", &pisv->localeth, &pisv->localip, pisv->localport));
  } else {
    if (supportip6) {
      netwib_er(netwib_buf_append_fmt(&filter, "icmp6 or (ether dst %{eth} and ip6 dst %{ip} and udp and dst port %{port})", &pisv->localeth, &pisv->localip, pisv->localport));
    }
  }

  netwib_er(netwox_snispo_init_eth(&pisv->device, &filter, &svu.pio));
  netwib_er(netwib_buf_close(&filter));

  /* initialize waiting */
  netwib_er(netwib_wait_init_io_read(pisv->ptlvsysio, &pwaitusr));
  netwib_er(netwib_wait_init_thread_cond(pisv->pendcond, &usrendval,
                                         &pwaitusrend));
  netwib_er(netwib_wait_init_io_read(svu.pio, &pwaitsniff));

  /* initialize working buffer */
  netwib_er(netwib_bufpool_initdefault(&svu.pbufpool));
  netwib_er(netwib_bufpool_buf_init(svu.pbufpool, &pworkbuf));

  /* main loop */
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    netwib_er(netwib_wait_wait3(pwaitusr, pwaitusrend, pwaitsniff,
                                NETWIB_TIME_INFINITE,
                                &eventusr, &eventusrend, &eventsniff));
    if (eventusr) {
      netwib__buf_reinit(pworkbuf);
      netwib_er(netwib_io_read(pisv->ptlvsysio, pworkbuf));
      netwib_er(netwox_sockv_thread_udp_spoof(&svu, pworkbuf));
    }
    if (eventusrend) {
      /* tools such as TFTP client needs to send before leaving. So
         eventusrend is treated after eventusr */
      break;
    }
    if (eventsniff) {
      netwib__buf_reinit(pworkbuf);
      ret = netwib_io_read(svu.pio, pworkbuf);
      if (ret == NETWIB_ERR_OK) {
        netwib_er(netwox_sockv_thread_udp_sniff(&svu, pworkbuf, &canwrite,
                                                &dataend));
        if (dataend) {
          break;
        }
        if (canwrite) {
          /* in this case, pworkbuf has been shifted to indicate
             start of udp data */
          netwib_er(netwib_io_write(pisv->ptlvsysio, pworkbuf));
        }
      } else if (ret != NETWIB_ERR_DATANOTAVAIL) {
        break;
      }
      ret = NETWIB_ERR_OK;
    }
  }

  /* indicates end */
  netwib_er(netwib_io_ctl_set_end_write(pisv->ptlvsysio));

  /* close resources */
  netwib_er(netwib_bufpool_buf_close(svu.pbufpool, &pworkbuf));
  netwib_er(netwib_bufpool_close(&svu.pbufpool));
  netwib_er(netwib_wait_close(&pwaitusr));
  netwib_er(netwib_wait_close(&pwaitusrend));
  netwib_er(netwib_wait_close(&pwaitsniff));
  netwib_er(netwib_io_close(&svu.pio));

  pinfosout = pinfosout; /* for compiler warning */
  return(ret);
}

