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
netwib_conststring t000008_description[] = {
  "This tool sniffs packets, and tries to decode TCP and UDP packets in",
  "order to retrieve port number they use.",
  "",
  "For UDP, spoofed packets are not checked.",
  "For TCP, only SYN-ACK are searched (so port scans are ignored).",
  NETWOX_DESC_sniff_devfilter,
  NETWOX_DESC_toolpriv_sniff,
  NULL
};
netwox_toolarg t000008_args[] = {
  NETWOX_TOOLARG_OPT_BUF_DEVICE('d', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPT_BUF_FILTER('f', NULL, NULL, NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000008_nodes[] = {
  NETWOX_TOOLTREENODETYPE_INFO_REMOTE,
  NETWOX_TOOLTREENODETYPE_SCAN,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000008_info = {
  "Sniff and display open ports",
  t000008_description,
  "capture",
  t000008_args,
  t000008_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000008_analyze(netwib_buf *ppkt)
{
  netwib_iphdr ipheader;
  netwib_udphdr udpheader;
  netwib_tcphdr tcpheader;
  netwib_ipproto ipproto;
  netwib_err ret;

  /*netwib_er(netwib_pkt_ip_display(ppkt, NULL, NETWIB_ENCODETYPE_SYNTH,
    NETWIB_ENCODETYPE_NOTHING));*/

  ret = netwib_pkt_decode_layer_ip(ppkt, &ipheader);
  if (ret != NETWIB_ERR_OK) {
    return(NETWIB_ERR_OK);
  }
  netwib_er(netwib_iphdr_get_proto(&ipheader, &ipproto));

  switch(ipproto) {
    case NETWIB_IPPROTO_UDP :
      ret = netwib_pkt_decode_layer_udp(ppkt, &udpheader);
      if (ret == NETWIB_ERR_OK) {
        netwib_er(netwib_fmt_display("UDP\t%{ip}\t%{port}\n", &ipheader.dst,
                                     udpheader.dst));
      }
      break;
    case NETWIB_IPPROTO_TCP :
      ret = netwib_pkt_decode_layer_tcp(ppkt, &tcpheader);
      if (ret == NETWIB_ERR_OK) {
        if (tcpheader.syn && tcpheader.ack) {
          netwib_er(netwib_fmt_display("TCP\t%{ip}\t%{port}\n", &ipheader.src,
                                       tcpheader.src));
        }
      }
      break;
    default :
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err t000008_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_io *piosniff;
  netwib_buf device, filter, pkt;
  netwib_err ret=NETWIB_ERR_OK;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000008_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'd', &device));
  netwib_er(netwox_arg_buf(parg, 'f', &filter));

  /* initialize io */
  netwib_er(netwib_io_init_sniff_ip(&device, &filter, NETWIB_TRUE,
                                    NETWIB_FALSE, &piosniff));

  /* main loop */
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  while (NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();
    netwib__buf_reinit(&pkt);
    ret = netwib_io_read(piosniff, &pkt);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      /* simply ignore, and try next */
    } else if (ret == NETWIB_ERR_OK) {
      ret = t000008_analyze(&pkt);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
    } else {
      break;
    }
  }

  netwib_er(netwib_buf_close(&pkt));
  netwib_er(netwib_io_close(&piosniff));
  netwib_er(netwox_arg_close(&parg));

  return(ret);
}
