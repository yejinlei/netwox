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
netwib_conststring t000078_description[] = {
  "This tool resets every TCP session matching a filter. It permits to",
  "temporarily block a TCP flow without having to change firewall rules.",
  "It also permits to force a renegotiation of session parameters, in",
  "order to sniff the beginning of connection.",
  NETWOX_DESC_sniff_devfilter,
  NETWOX_DESC_spoofip,
  NETWOX_DESC_toolpriv_snsp,
  NULL
};
netwox_toolarg t000078_args[] = {
  NETWOX_TOOLARG_OPT_BUF_DEVICE('d', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPT_BUF_FILTER('f', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPT_SPOOFIP('s', NULL, NULL, "linkbraw"),
  NETWOX_TOOLARG_OPTA_IPS_SU('i', NULL, "limit the list of IP addresses to reset", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000078_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NETAUDIT_TCP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000078_info = {
  "Reset every TCP packet",
  t000078_description,
  NULL,
  t000078_args,
  t000078_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000078_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_io *psniffio, *pspoofio;
  netwib_spoof_ip_inittype inittype;
  netwib_buf pkt, filter, device;
  netwib_ips *pips;
  netwib_iphdr ipheader, ipheader2;
  netwib_tcphdr tcpheader, tcpheader2;
  netwib_bool ipsisset, yes;
  netwib_err ret;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000078_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'd', &device));
  netwib_er(netwox_arg_buf(parg, 'f', &filter));
  netwib_er(netwox_arg_spoofip(parg, 's', &inittype));
  netwib_er(netwox_arg_isset(parg, 'i', &ipsisset));
  netwib_er(netwox_arg_ips(parg, 'i', &pips));

  /* initialize io */
  netwib_er(netwib_io_init_spoof_ip(inittype, &pspoofio));
  netwib_er(netwib_io_init_sniff_ip(&device, &filter, NETWIB_FALSE,
                                    NETWIB_FALSE, &psniffio));

  /* main loop */
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  while (NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();
    /* sniff */
    netwib__buf_reinit(&pkt);
    ret = netwib_io_read(psniffio, &pkt);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    }
    netwib_er(ret);
    /* check if it's TCP */
    ret = netwib_pkt_decode_iptcpdata(&pkt, &ipheader, &tcpheader, NULL);
    if (ret != NETWIB_ERR_OK) continue;
    if (tcpheader.rst || tcpheader.fin) continue;
    /* if an ip range was specified, reset only if addresses are inside */
    if (ipsisset) {
      netwib_er(netwib_ips_contains_ip(pips, &ipheader.src, &yes));
      if (!yes) {
        netwib_er(netwib_ips_contains_ip(pips, &ipheader.dst, &yes));
        if (!yes) {
          continue;
        }
      }
    }
    /* answer */
    netwib_er(netwib_iphdr_initdefault(ipheader.src.iptype, &ipheader2));
    ipheader2.src = ipheader.dst;
    ipheader2.dst = ipheader.src;
    netwib_er(netwib_tcphdr_initdefault(&tcpheader2));
    tcpheader2.rst = NETWIB_TRUE;
    tcpheader2.ack = NETWIB_TRUE;
    tcpheader2.window = 0;
    tcpheader2.src = tcpheader.dst;
    tcpheader2.dst = tcpheader.src;
    tcpheader2.seqnum = tcpheader.acknum;
    tcpheader2.acknum = tcpheader.seqnum + 1;
    netwib__buf_reinit(&pkt);
    netwib_er(netwib_pkt_append_iptcpdata(&ipheader2, &tcpheader2, NULL,
                                          &pkt));
    netwib_er(netwib_io_write(pspoofio, &pkt));
  }
  netwib_er(netwib_buf_close(&pkt));

  /* close */
  netwib_er(netwib_io_close(&psniffio));
  netwib_er(netwib_io_close(&pspoofio));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
