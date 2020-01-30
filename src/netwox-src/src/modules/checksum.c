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
netwib_err netwox_checksum_verify_ip(netwib_bufpool *ppool,
                                     netwib_buf *ppkt)
{
  netwib_iphdr ipheader;
  netwib_udphdr udpheader;
  netwib_tcphdr tcpheader;
  netwib_icmp4 icmp4;
  netwib_icmp6 icmp6;
  netwib_buf *pnewpkt;
  netwib_uint16 ipcheck, xpcheck;
  netwib_bool isfrag;
  netwib_ipproto ipproto, originalproto;
  netwib_err ret;

  /* decode ip layer */
  ret = netwib_pkt_decode_layer_ip(ppkt, &ipheader);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_fmt_display("IP packet not decoded\n"));
    return(NETWIB_ERR_OK);
  }
  ipcheck=0;
  if (ipheader.iptype == NETWIB_IPTYPE_IP4) {
    ipcheck = ipheader.header.ip4.check;
    ipheader.header.ip4.check = 0;
  }

  /* fragments are dealt as an unknown protocol */
  netwib_er(netwox_iphdr_isfrag(&ipheader, &isfrag));
  netwib_er(netwib_iphdr_get_proto(&ipheader, &ipproto));
  originalproto = NETWIB_IPPROTO_RAW;
  if (isfrag) {
    originalproto = ipproto;
    ipproto = NETWIB_IPPROTO_RAW;
  }

  /* decode xp layer */
  switch(ipproto) {
    case NETWIB_IPPROTO_UDP :
      /* decode */
      ret = netwib_pkt_decode_layer_udp(ppkt, &udpheader);
      if (ret != NETWIB_ERR_OK) {
        netwib_er(netwib_fmt_display("UDP packet not decoded\n"));
        return(NETWIB_ERR_OK);
      }
      xpcheck = udpheader.check;
      udpheader.check = 0;
      /* reassemble */
      netwib_er(netwib_bufpool_buf_init(ppool, &pnewpkt));
      netwib_er(netwib_pkt_append_ipudpdata(&ipheader, &udpheader, ppkt,
                                            pnewpkt));
      /* redecode */
      netwib_er(netwib_pkt_decode_ipudpdata(pnewpkt, &ipheader, &udpheader,
                                            NULL));
      netwib_er(netwib_bufpool_buf_close(ppool, &pnewpkt));
      /* verify */
      netwib_er(netwib_fmt_display("%{ip}:%{port}->%{ip}:%{port} : ",
                                   &ipheader.src, udpheader.src,
                                   &ipheader.dst, udpheader.dst));
      if (ipheader.iptype == NETWIB_IPTYPE_IP4) {
        if (ipheader.header.ip4.check == ipcheck) {
          netwib_er(netwib_fmt_display("IP:ok "));
        } else {
          netwib_er(netwib_fmt_display("IP:%{uint16}!=%{uint16} ",
                                       ipcheck,
                                       ipheader.header.ip4.check));
        }
      }
      if (udpheader.check == xpcheck) {
        netwib_er(netwib_fmt_display("UDP:ok\n"));
      } else {
        netwib_er(netwib_fmt_display("UDP:%{uint16}!=%{uint16}\n",
                                     xpcheck, udpheader.check));
      }
      break;
    case NETWIB_IPPROTO_TCP :
      /* decode */
      ret = netwib_pkt_decode_layer_tcp(ppkt, &tcpheader);
      if (ret != NETWIB_ERR_OK) {
        netwib_er(netwib_fmt_display("TCP packet not decoded\n"));
        return(NETWIB_ERR_OK);
      }
      xpcheck = tcpheader.check;
      tcpheader.check = 0;
      /* reassemble */
      netwib_er(netwib_bufpool_buf_init(ppool, &pnewpkt));
      netwib_er(netwib_pkt_append_iptcpdata(&ipheader, &tcpheader, ppkt,
                                            pnewpkt));
      /* redecode */
      netwib_er(netwib_pkt_decode_iptcpdata(pnewpkt, &ipheader, &tcpheader,
                                            NULL));
      netwib_er(netwib_bufpool_buf_close(ppool, &pnewpkt));
      /* verify */
      netwib_er(netwib_fmt_display("%{ip}:%{port}->%{ip}:%{port} : ",
                                   &ipheader.src, tcpheader.src,
                                   &ipheader.dst, tcpheader.dst));
      if (ipheader.iptype == NETWIB_IPTYPE_IP4) {
        if (ipheader.header.ip4.check == ipcheck) {
          netwib_er(netwib_fmt_display("IP:ok "));
        } else {
          netwib_er(netwib_fmt_display("IP:%{uint16}!=%{uint16} ",
                                       ipcheck,
                                       ipheader.header.ip4.check));
        }
      }
      if (tcpheader.check == xpcheck) {
        netwib_er(netwib_fmt_display("TCP:ok\n"));
      } else {
        netwib_er(netwib_fmt_display("TCP:%{uint16}!=%{uint16}\n",
                                     xpcheck, tcpheader.check));
      }
      break;
    case NETWIB_IPPROTO_ICMP4 :
      /* decode */
      ret = netwib_pkt_decode_layer_icmp4(ppkt, &icmp4);
      if (ret != NETWIB_ERR_OK) {
        netwib_er(netwib_fmt_display("ICMP4 packet not decoded\n"));
        return(NETWIB_ERR_OK);
      }
      xpcheck = icmp4.check;
      icmp4.check = 0;
      /* reassemble */
      netwib_er(netwib_bufpool_buf_init(ppool, &pnewpkt));
      netwib_er(netwib_pkt_append_ipicmp4(&ipheader, &icmp4, pnewpkt));
      /* redecode */
      netwib_er(netwib_pkt_decode_ipicmp4(pnewpkt, &ipheader, &icmp4));
      netwib_er(netwib_bufpool_buf_close(ppool, &pnewpkt));
      /* verify */
      netwib_er(netwib_fmt_display("%{ip}->%{ip} : ",
                                   &ipheader.src, &ipheader.dst));
      if (ipheader.iptype == NETWIB_IPTYPE_IP4) {
        if (ipheader.header.ip4.check == ipcheck) {
          netwib_er(netwib_fmt_display("IP:ok "));
        } else {
          netwib_er(netwib_fmt_display("IP:%{uint16}!=%{uint16} ",
                                       ipcheck,
                                       ipheader.header.ip4.check));
        }
      }
      if (icmp4.check == xpcheck) {
        netwib_er(netwib_fmt_display("ICMP4:ok\n"));
      } else {
        netwib_er(netwib_fmt_display("ICMP4:%{uint16}!=%{uint16}\n",
                                     xpcheck, icmp4.check));
      }
      break;
    case NETWIB_IPPROTO_ICMP6 :
      /* decode */
      ret = netwib_pkt_decode_layer_icmp6(ppkt, &icmp6);
      if (ret != NETWIB_ERR_OK) {
        netwib_er(netwib_fmt_display("ICMP6 packet not decoded\n"));
        return(NETWIB_ERR_OK);
      }
      xpcheck = icmp6.check;
      icmp6.check = 0;
      /* reassemble */
      netwib_er(netwib_bufpool_buf_init(ppool, &pnewpkt));
      netwib_er(netwib_pkt_append_ipicmp6(&ipheader, &icmp6, pnewpkt));
      /* redecode */
      netwib_er(netwib_pkt_decode_ipicmp6(pnewpkt, &ipheader, &icmp6));
      netwib_er(netwib_bufpool_buf_close(ppool, &pnewpkt));
      /* verify */
      netwib_er(netwib_fmt_display("%{ip}->%{ip} : ",
                                   &ipheader.src, &ipheader.dst));
      if (ipheader.iptype == NETWIB_IPTYPE_IP4) {
        if (ipheader.header.ip4.check == ipcheck) {
          netwib_er(netwib_fmt_display("IP:ok "));
        } else {
          netwib_er(netwib_fmt_display("IP:%{uint16}!=%{uint16} ",
                                       ipcheck,
                                       ipheader.header.ip4.check));
        }
      }
      if (icmp6.check == xpcheck) {
        netwib_er(netwib_fmt_display("ICMP6:ok\n"));
      } else {
        netwib_er(netwib_fmt_display("ICMP6:%{uint16}!=%{uint16}\n",
                                     xpcheck, icmp6.check));
      }
      break;
    default :
      if (ipproto == NETWIB_IPPROTO_RAW) {
        ipproto = originalproto;
      }
      /* reassemble */
      netwib_er(netwib_bufpool_buf_init(ppool, &pnewpkt));
      netwib_er(netwib_pkt_append_ipdata(&ipheader, ppkt, pnewpkt));
      /* redecode */
      netwib_er(netwib_pkt_decode_ipdata(pnewpkt, &ipheader, NULL));
      netwib_er(netwib_bufpool_buf_close(ppool, &pnewpkt));
      /* verify */
      netwib_er(netwib_fmt_display("%{ip}->%{ip} : ",
                                   &ipheader.src, &ipheader.dst));
      if (ipheader.iptype == NETWIB_IPTYPE_IP4) {
        if (ipheader.header.ip4.check == ipcheck) {
          netwib_er(netwib_fmt_display("IP:ok "));
        } else {
          netwib_er(netwib_fmt_display("IP:%{uint16}!=%{uint16} ",
                                       ipcheck,
                                       ipheader.header.ip4.check));
        }
      }
      netwib_er(netwib_fmt_display("\n"));
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_checksum_verify_link(netwib_bufpool *ppool,
                                       netwib_device_dlttype dlttype,
                                       netwib_buf *ppkt)
{
  netwib_linkhdr linkheader;
  netwib_linkhdrproto linkhdrproto;
  netwib_err ret;

  ret = netwib_pkt_decode_layer_link(dlttype, ppkt, &linkheader);
  if (ret != NETWIB_ERR_OK) {
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_linkhdr_get_proto(&linkheader, &linkhdrproto));

  switch(linkhdrproto) {
    case NETWIB_LINKHDRPROTO_IP4 :
    case NETWIB_LINKHDRPROTO_IP6 :
      netwib_er(netwox_checksum_verify_ip(ppool, ppkt));
      break;
    default:
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_checksum_compute_ip(netwib_constbuf *ppktin,
                                      netwib_buf *ppktout)
{
  netwib_iphdr ipheader;
  netwib_udphdr udpheader;
  netwib_tcphdr tcpheader;
  netwib_icmp4 icmp4;
  netwib_icmp6 icmp6;
  netwib_buf pktin;
  netwib_bool isfrag;
  netwib_ipproto ipproto;
  netwib_err ret;

  /* decode ip layer */
  pktin = *ppktin;
  netwib_er(netwib_pkt_decode_layer_ip(&pktin, &ipheader));
  if (ipheader.iptype == NETWIB_IPTYPE_IP4) {
    ipheader.header.ip4.check = 0;
  }

  /* fragments and unrecognized packets are dealt later */
  netwib_er(netwox_iphdr_isfrag(&ipheader, &isfrag));
  if (!isfrag) {
    netwib_er(netwib_iphdr_get_proto(&ipheader, &ipproto));
    switch(ipproto) {
    case NETWIB_IPPROTO_UDP :
      /* decode */
      ret = netwib_pkt_decode_layer_udp(&pktin, &udpheader);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
      udpheader.check = 0;
      /* reassemble */
      netwib_er(netwib_pkt_append_ipudpdata(&ipheader, &udpheader, &pktin,
                                            ppktout));
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_IPPROTO_TCP :
      /* decode */
      ret = netwib_pkt_decode_layer_tcp(&pktin, &tcpheader);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
      tcpheader.check = 0;
      /* reassemble */
      netwib_er(netwib_pkt_append_iptcpdata(&ipheader, &tcpheader, &pktin,
                                            ppktout));
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_IPPROTO_ICMP4 :
      /* decode */
      ret = netwib_pkt_decode_layer_icmp4(&pktin, &icmp4);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
      icmp4.check = 0;
      /* reassemble */
      netwib_er(netwib_pkt_append_ipicmp4(&ipheader, &icmp4, ppktout));
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_IPPROTO_ICMP6 :
      /* decode */
      ret = netwib_pkt_decode_layer_icmp6(&pktin, &icmp6);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
      icmp6.check = 0;
      /* reassemble */
      netwib_er(netwib_pkt_append_ipicmp6(&ipheader, &icmp6, ppktout));
      return(NETWIB_ERR_OK);
      break;
    default :
      break;
    }
  }

  /* only recompute IP */
  netwib_er(netwib_pkt_append_ipdata(&ipheader, &pktin, ppktout));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_checksum_compute_link(netwib_device_dlttype dlttype,
                                        netwib_constbuf *ppktin,
                                        netwib_buf *ppktout)
{
  netwib_linkhdr linkheader;
  netwib_linkhdrproto linkhdrproto;
  netwib_buf pktin;

  pktin = *ppktin;
  netwib_er(netwib_pkt_decode_layer_link(dlttype, &pktin, &linkheader));

  netwib_er(netwib_linkhdr_get_proto(&linkheader, &linkhdrproto));

  switch(linkhdrproto) {
    case NETWIB_LINKHDRPROTO_IP4 :
    case NETWIB_LINKHDRPROTO_IP6 :
      netwib_er(netwib_pkt_append_linkhdr(&linkheader, ppktout));
      netwib_er(netwox_checksum_compute_ip(&pktin, ppktout));
      break;
    default:
      break;
  }

  return(NETWIB_ERR_OK);
}

