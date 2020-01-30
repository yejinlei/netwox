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
netwib_err netwox_sample_info_initdefault(netwox_sample_info *pinfo)
{
  netwib_er(netwib_ip_init_ip4(0x01020304, &pinfo->ipsrc));
  netwib_er(netwib_ip_init_ip4(0x05060708, &pinfo->ipdst));

  netwib_er(netwib_buf_init_ext_empty(&pinfo->ip4opts));
  netwib_er(netwib_buf_init_ext_empty(&pinfo->ip6exts));

  pinfo->portdst = NETWOX_SERVICES_WWW;
  netwib_er(netwib_uint32_init_rand(1024, 0xFFFF, &pinfo->portsrc));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_sample_pkt_ipudpsyslog(netwox_sample_info *pinfo,
                                                netwib_buf *ppkt)
{
  netwib_iphdr ipheader;
  netwib_udphdr udpheader;
  netwib_buf udpdata;
  netwib_data array[80];
  netwib_time t;
  netwib_buf msg;
  netwox_syslog_priority priority;

  netwib_er(netwib_iphdr_initdefault(pinfo->ipdst.iptype, &ipheader));
  ipheader.src = pinfo->ipsrc;
  ipheader.dst = pinfo->ipdst;

  if (pinfo->ipdst.iptype == NETWIB_IPTYPE_IP4) {
    ipheader.header.ip4.opts = pinfo->ip4opts;
  } else {
    ipheader.header.ip6.exts = pinfo->ip6exts;
  }

  netwib_er(netwib_udphdr_initdefault(&udpheader));
  udpheader.src = pinfo->portsrc;
  udpheader.dst = NETWOX_SERVICES_SYSLOG;

  netwib_er(netwib_buf_init_mallocdefault(&udpdata));
  netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &msg));
  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_buf_append_fmt(&msg, "Hello %{uint32} %{uint32}",
                                  t.sec, t.nsec));
  priority = netwox_syslog_priority_init(NETWOX_SYSLOG_FACILITY_USER,
                                         NETWOX_SYSLOG_SEVERITY_ALERT);
  netwib_er(netwox_syslog_msg_init(priority, &t, NULL, &ipheader.src, &msg,
                                   &udpdata));

  netwib_er(netwib_pkt_append_ipudpdata(&ipheader, &udpheader, &udpdata,
                                        ppkt));
  netwib_er(netwib_buf_close(&udpdata));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_sample_pkt_iptcpsyn(netwox_sample_info *pinfo,
                                             netwib_buf *ppkt)
{
  netwib_iphdr ipheader;
  netwib_tcphdr tcpheader;

  netwib_er(netwib_iphdr_initdefault(pinfo->ipdst.iptype, &ipheader));
  ipheader.src = pinfo->ipsrc;
  ipheader.dst = pinfo->ipdst;

  if (pinfo->ipdst.iptype == NETWIB_IPTYPE_IP4) {
    ipheader.header.ip4.opts = pinfo->ip4opts;
  } else {
    ipheader.header.ip6.exts = pinfo->ip6exts;
  }

  netwib_er(netwib_tcphdr_initdefault(&tcpheader));
  tcpheader.src = pinfo->portsrc;
  tcpheader.dst = pinfo->portdst;
  tcpheader.syn = NETWIB_TRUE;

  netwib_er(netwib_pkt_append_iptcpdata(&ipheader, &tcpheader, NULL, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_sample_pkt_iptcpsynack(netwox_sample_info *pinfo,
                                                netwib_buf *ppkt)
{
  netwib_iphdr ipheader;
  netwib_tcphdr tcpheader;

  netwib_er(netwib_iphdr_initdefault(pinfo->ipdst.iptype, &ipheader));
  ipheader.src = pinfo->ipsrc;
  ipheader.dst = pinfo->ipdst;

  if (pinfo->ipdst.iptype == NETWIB_IPTYPE_IP4) {
    ipheader.header.ip4.opts = pinfo->ip4opts;
  } else {
    ipheader.header.ip6.exts = pinfo->ip6exts;
  }

  netwib_er(netwib_tcphdr_initdefault(&tcpheader));
  netwib_er(netwib_uint32_init_rand(1024, 0xFFFF, &tcpheader.acknum));
  tcpheader.src = pinfo->portsrc;
  tcpheader.dst = pinfo->portdst;
  tcpheader.syn = NETWIB_TRUE;
  tcpheader.ack = NETWIB_TRUE;

  netwib_er(netwib_pkt_append_iptcpdata(&ipheader, &tcpheader, NULL, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_sample_pkt_iptcpack(netwox_sample_info *pinfo,
                                             netwib_buf *ppkt)
{
  netwib_iphdr ipheader;
  netwib_tcphdr tcpheader;
  netwib_buf tcpdata;

  netwib_er(netwib_iphdr_initdefault(pinfo->ipdst.iptype, &ipheader));
  ipheader.src = pinfo->ipsrc;
  ipheader.dst = pinfo->ipdst;

  if (pinfo->ipdst.iptype == NETWIB_IPTYPE_IP4) {
    ipheader.header.ip4.opts = pinfo->ip4opts;
  } else {
    ipheader.header.ip6.exts = pinfo->ip6exts;
  }

  netwib_er(netwib_tcphdr_initdefault(&tcpheader));
  netwib_er(netwib_uint32_init_rand(1024, 0xFFFF, &tcpheader.acknum));
  tcpheader.src = pinfo->portsrc;
  tcpheader.dst = pinfo->portdst;
  tcpheader.ack = NETWIB_TRUE;

  netwib_er(netwib_buf_init_ext_string("hello, how are you ?", &tcpdata));

  netwib_er(netwib_pkt_append_iptcpdata(&ipheader, &tcpheader, &tcpdata,
                                        ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_sample_pkt_ipicmpping(netwox_sample_info *pinfo,
                                                netwib_buf *ppkt)
{
  netwib_iphdr ipheader;
  netwib_icmp4 icmp4;
  netwib_icmp6 icmp6;

  netwib_er(netwib_iphdr_initdefault(pinfo->ipdst.iptype, &ipheader));
  ipheader.src = pinfo->ipsrc;
  ipheader.dst = pinfo->ipdst;

  if (pinfo->ipdst.iptype == NETWIB_IPTYPE_IP4) {
    ipheader.header.ip4.opts = pinfo->ip4opts;
    netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_ECHOREQ, &icmp4));
    netwib_er(netwib_buf_init_ext_string("hello, how are you ?",
                                       &icmp4.msg.echo.data));
    netwib_er(netwib_pkt_append_ipicmp4(&ipheader, &icmp4, ppkt));
  } else {
    ipheader.header.ip6.exts = pinfo->ip6exts;
    netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_ECHOREQ, &icmp6));
    netwib_er(netwib_buf_init_ext_string("hello, how are you ?",
                                       &icmp6.msg.echo.data));
    netwib_er(netwib_pkt_append_ipicmp6(&ipheader, &icmp6, ppkt));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_sample_pkt(netwox_sample sample,
                             netwox_sample_info *pinfo,
                             netwib_buf *ppkt)
{
  switch(sample) {
    case NETWOX_SAMPLE_IPUDPSYSLOG :
      netwib_er(netwox_sample_pkt_ipudpsyslog(pinfo, ppkt));
      break;
    case NETWOX_SAMPLE_IPTCPSYN :
      netwib_er(netwox_sample_pkt_iptcpsyn(pinfo, ppkt));
      break;
    case NETWOX_SAMPLE_IPTCPSYNACK :
      netwib_er(netwox_sample_pkt_iptcpsynack(pinfo, ppkt));
      break;
    case NETWOX_SAMPLE_IPTCPACK :
      netwib_er(netwox_sample_pkt_iptcpack(pinfo, ppkt));
      break;
    case NETWOX_SAMPLE_IPICMPPING :
      netwib_er(netwox_sample_pkt_ipicmpping(pinfo, ppkt));
      break;
    default :
      return(NETWIB_ERR_PATOOHIGH);
  }

  return(NETWIB_ERR_OK);
}

