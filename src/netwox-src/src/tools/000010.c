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
netwib_conststring t000010_description[] = {
  "This tool sniffs and displays some basic network statistics.",
  "It supports 7 groups: Ethernet/link, ARP, IP4, IP6, UDP, TCP, ICMP.",
  "For each one, it displays: packet count (count), sum of packet sizes",
  "(size), percentage of packet count (c%) and percentage of size (s%).",
  "Display is updated at most every second.",
  "",
  NETWOX_DESC_sniff_devfilter,
  NETWOX_DESC_toolpriv_sniff,
  NULL
};
netwox_toolarg t000010_args[] = {
  NETWOX_TOOLARG_OPT_BUF_DEVICE('d', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPT_BUF_FILTER('f', NULL, NULL, NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000010_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SNIFF,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000010_info = {
  "Sniff and display network statistics",
  t000010_description,
  "capture, icmp, ip4, ip6, mac, packet, show",
  t000010_args,
  t000010_nodes,
};

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 countlink;
  netwib_uint32 countip4;
  netwib_uint32 countip6;
  netwib_uint32 countudp;
  netwib_uint32 counttcp;
  netwib_uint32 counticmp;
  netwib_uint32 countarp;
  netwib_uint32 sizelink;
  netwib_uint32 sizeip4;
  netwib_uint32 sizeip6;
  netwib_uint32 sizeudp;
  netwib_uint32 sizetcp;
  netwib_uint32 sizeicmp;
  netwib_uint32 sizearp;
} t000010_netstat;

/*-------------------------------------------------------------*/
static netwib_err t000010_netstat_init(t000010_netstat *pnetstat)
{
  pnetstat->countlink = 0;
  pnetstat->countip4 = 0;
  pnetstat->countip6 = 0;
  pnetstat->countudp = 0;
  pnetstat->counttcp = 0;
  pnetstat->counticmp = 0;
  pnetstat->countarp = 0;
  pnetstat->sizelink = 0;
  pnetstat->sizeip4 = 0;
  pnetstat->sizeip6 = 0;
  pnetstat->sizeudp = 0;
  pnetstat->sizetcp = 0;
  pnetstat->sizeicmp = 0;
  pnetstat->sizearp = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000010_netstat_display(t000010_netstat *pnetstat)
{
  float flink, fip;

  netwib_er(netwib_fmt_display("           LINK       ARP       IP4       IP6       UDP       TCP     ICMP\n"));
  netwib_er(netwib_fmt_display("count %{r 9;uint32} %{r 9;uint32} %{r 9;uint32} %{r 9;uint32} %{r 9;uint32} %{r 9;uint32} %{r 9;uint32}\n", pnetstat->countlink, pnetstat->countarp, pnetstat->countip4, pnetstat->countip6, pnetstat->countudp, pnetstat->counttcp, pnetstat->counticmp));
  netwib_er(netwib_fmt_display("size  %{r 9;uint32} %{r 9;uint32} %{r 9;uint32} %{r 9;uint32} %{r 9;uint32} %{r 9;uint32} %{r 9;uint32}\n", pnetstat->sizelink, pnetstat->sizearp, pnetstat->sizeip4, pnetstat->sizeip6, pnetstat->sizeudp, pnetstat->sizetcp, pnetstat->sizeicmp));
  flink = (float)pnetstat->countlink;
  fip = (float)pnetstat->countip4 + pnetstat->countip6;
  netwib_er(netwib_fmt_display("c%%          %{r 3;uint32}     [ %{r 3;uint32}       %{r 3;uint32}       %{r 3;uint32} ]   [ %{r 3;uint32}       %{r 3;uint32}       %{r 3;uint32} ]\n", 100, (netwib_uint32)(100.0*pnetstat->countarp/flink), (netwib_uint32)(100.0*pnetstat->countip4/flink), (netwib_uint32)(100.0*pnetstat->countip6/flink), (netwib_uint32)(100.0*pnetstat->countudp/fip), (netwib_uint32)(100.0*pnetstat->counttcp/fip), (netwib_uint32)(100.0*pnetstat->counticmp/fip)));
  flink = (float)pnetstat->sizelink;
  fip = (float)pnetstat->sizeip4 + pnetstat->sizeip6;
  netwib_er(netwib_fmt_display("s%%          %{r 3;uint32}     [ %{r 3;uint32}       %{r 3;uint32}       %{r 3;uint32} ]   [ %{r 3;uint32}       %{r 3;uint32}       %{r 3;uint32} ]\n", 100, (netwib_uint32)(100.0*pnetstat->sizearp/flink), (netwib_uint32)(100.0*pnetstat->sizeip4/flink), (netwib_uint32)(100.0*pnetstat->sizeip6/flink), (netwib_uint32)(100.0*pnetstat->sizeudp/fip), (netwib_uint32)(100.0*pnetstat->sizetcp/fip), (netwib_uint32)(100.0*pnetstat->sizeicmp/fip)));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000010_analyze(netwib_device_dlttype dlttype,
                                  netwib_buf *ppkt,
                                  t000010_netstat *pnetstat)
{
  netwib_linkhdr linkheader;
  netwib_iphdr ipheader;
  netwib_linkhdrproto linkhdrproto;
  netwib_ipproto ipproto;
  netwib_uint32 pktsize;
  netwib_err ret;

  ret = netwib_pkt_decode_layer_link(dlttype, ppkt, &linkheader);
  if (ret != NETWIB_ERR_OK) {
    return(NETWIB_ERR_OK);
  }

  pktsize = ppkt->endoffset - ppkt->beginoffset;
  pnetstat->countlink++;
  pnetstat->sizelink += pktsize;

  netwib_er(netwib_linkhdr_get_proto(&linkheader, &linkhdrproto));

  switch(linkhdrproto) {
    case NETWIB_LINKHDRPROTO_IP4 :
    case NETWIB_LINKHDRPROTO_IP6 :
      ret = netwib_pkt_decode_layer_ip(ppkt, &ipheader);
      if (ret != NETWIB_ERR_OK) {
        return(NETWIB_ERR_OK);
      }
      if (linkhdrproto == NETWIB_LINKHDRPROTO_IP4) {
        pnetstat->countip4++;
        pnetstat->sizeip4 += pktsize;
      } else {
        pnetstat->countip6++;
        pnetstat->sizeip6 += pktsize;
      }
      netwib_er(netwib_iphdr_get_proto(&ipheader, &ipproto));
      switch(ipproto) {
      case NETWIB_IPPROTO_UDP :
        pnetstat->countudp++;
        pnetstat->sizeudp += pktsize;
        break;
      case NETWIB_IPPROTO_TCP :
        pnetstat->counttcp++;
        pnetstat->sizetcp += pktsize;
        break;
      case NETWIB_IPPROTO_ICMP4 :
      case NETWIB_IPPROTO_ICMP6 :
        pnetstat->counticmp++;
        pnetstat->sizeicmp += pktsize;
        break;
      default :
        break;
      }
      break;
    case NETWIB_LINKHDRPROTO_ARP :
      pnetstat->countarp++;
      pnetstat->sizearp += pktsize;
      break;
    default :
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000010_loop(netwib_io *pio)
{
  netwib_buf pkt;
  netwib_device_dlttype dlttype;
  t000010_netstat netstat;
  netwib_time t;
  netwib_uint32 lastdisptime;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_sniff_ctl_get_dlt(pio, &dlttype));

  /* main loop */
  lastdisptime = 0;
  netwib_er(t000010_netstat_init(&netstat));
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  while (NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();
    /* read packet */
    netwib__buf_reinit(&pkt);
    ret = netwib_io_read(pio, &pkt);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* analyze and add in statistics */
    netwib_er(t000010_analyze(dlttype, &pkt, &netstat));
    /* eventually display statistics */
    netwib_er(netwib_time_init_now(&t));
    if (t.sec > lastdisptime) {
      netwib_er(t000010_netstat_display(&netstat));
      lastdisptime = t.sec;
    }
  }

  /* close */
  netwib_er(netwib_buf_close(&pkt));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000010_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_io *pio;
  netwib_buf device, filter;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000010_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'd', &device));
  netwib_er(netwox_arg_buf(parg, 'f', &filter));

  /* initialize io */
  netwib_er(netwib_io_init_sniff(&device, &filter, &pio));

  /* main loop */
  netwib_er(t000010_loop(pio));

  /* close */
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
