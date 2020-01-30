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
netwib_conststring t000077_description[] = {
  "This tool sends a lot of TCP SYN packets, and displays seqnum (ISN) in",
  "TCP SYN-ACK replies. This seqnum should be random, otherwise a blind",
  "spoofing attack is possible.",
  NETWOX_DESC_spoofip,
  NETWOX_DESC_toolpriv_snsp,
  NULL
};
netwox_toolarg t000077_args[] = {
  NETWOX_TOOLARG_REQ_IP_DST('i', NULL, NULL, NULL),
  NETWOX_TOOLARG_REQ_PORT_DST('p', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPT_SPOOFIP('s', NULL, NULL, "linkbraw"),
  NETWOX_TOOLARG_OPT_UINT32('w', "max-wait",
                            "maximum waiting time for answer (in msec)",
                            "1000"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000077_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NETAUDIT_TCP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000077_info = {
  "Check if seqnum are predictible",
  t000077_description,
  "isn",
  t000077_args,
  t000077_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000077_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_io *psniffio, *pspoofio;
  netwib_spoof_ip_inittype inittype;
  netwib_ip srcip, dstip;
  netwib_port dstport;
  netwib_buf pkt, filter, device;
  netwib_iphdr ipheader, ipheader2;
  netwib_tcphdr tcpheader, tcpheader2;
  netwib_time t;
  netwib_uint32 lastseqnum, diff, maxwaitms;
  netwib_char diffsign;
  netwib_bool event, supportip6;
  netwib_cmp cmp;
  netwib_err ret;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000077_info, &parg));
  netwib_er(netwox_arg_spoofip(parg, 's', &inittype));
  netwib_er(netwox_arg_ip(parg, 'i', &dstip));
  netwib_er(netwox_arg_port(parg, 'p', &dstport));
  netwib_er(netwox_arg_uint32(parg, 'w', &maxwaitms));

  /* initialize io */
  netwib_er(netwib_buf_init_mallocdefault(&device));
  netwib_er(netwib_buf_init_mallocdefault(&filter));
  netwib_er(netwib_conf_routes_reach_ip(&dstip, &device, &srcip));
  netwib_er(netwib_io_init_spoof_ip(inittype, &pspoofio));
  netwib_er(netwox_pcapfilt_supports_ip6(&device, &supportip6));
  if (dstip.iptype == NETWIB_IPTYPE_IP4 || supportip6) {
    netwib_er(netwib_buf_append_fmt(&filter, "tcp and host %{ip}", &dstip));
  }
  netwib_er(netwib_io_init_sniff_ip(&device, &filter, NETWIB_FALSE,
                                    NETWIB_FALSE, &psniffio));
  netwib_er(netwib_buf_close(&filter));
  netwib_er(netwib_buf_close(&device));

  /* prepare headers for packet */
  netwib_er(netwib_iphdr_initdefault(dstip.iptype, &ipheader));
  ipheader.src = srcip;
  ipheader.dst = dstip;
  netwib_er(netwib_tcphdr_initdefault(&tcpheader));
  tcpheader.syn = NETWIB_TRUE;
  netwib_er(netwib_uint32_init_rand(1024, 0xFFFF, &tcpheader.src));
  tcpheader.dst = dstport;

  /* main loop */
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  lastseqnum = 0;
  while (NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();
    /* send a SYN */
    netwib__buf_reinit(&pkt);
    tcpheader.seqnum++;
    netwib_er(netwib_pkt_append_iptcpdata(&ipheader, &tcpheader, NULL, &pkt));
    netwib_er(netwib_io_write(pspoofio, &pkt));
    /* wait a SYN ACK during maxwait msec */
    netwib_er(netwib_time_init_now(&t));
    netwib_er(netwib_time_plus_msec(&t, maxwaitms));
    while (NETWIB_TRUE) {
      netwib__debug_ctrlc_pressed_break();
      /* sniff */
      netwib_er(netwib_io_wait_read(psniffio, &t, &event));
      if (!event) {
        netwib_er(netwib_fmt_display("No answer for this SYN (try to increase max-wait)\n"));
        lastseqnum = 0;
        break;
      }
      netwib__buf_reinit(&pkt);
      ret = netwib_io_read(psniffio, &pkt);
      if (ret == NETWIB_ERR_DATANOTAVAIL) {
        ret = NETWIB_ERR_OK;
        continue;
      }
      netwib_er(ret);
      /* check it corresponds */
      ret = netwib_pkt_decode_iptcpdata(&pkt, &ipheader2, &tcpheader2, NULL);
      if (ret == NETWIB_ERR_OK) {
        if (tcpheader2.syn && tcpheader2.ack &&
            tcpheader2.acknum == tcpheader.seqnum + 1 &&
            tcpheader2.src == tcpheader.dst &&
            tcpheader2.dst == tcpheader.src) {
          netwib_er(netwib_ip_cmp(&ipheader2.src, &ipheader.dst, &cmp));
          if (cmp == NETWIB_CMP_EQ) {
            netwib_er(netwib_ip_cmp(&ipheader2.dst, &ipheader.src, &cmp));
            if (cmp == NETWIB_CMP_EQ) {
              diffsign = '+';
              diff = tcpheader2.seqnum - lastseqnum;
              if (lastseqnum > tcpheader2.seqnum) {
                diffsign = '-';
                diff = 0xFFFFFFFFu - diff;
              }
              netwib_er(netwib_fmt_display("%{uint32} [%c%{uint32}]\n",
                                           tcpheader2.seqnum, diffsign, diff));
              lastseqnum = tcpheader2.seqnum;
              break;
            }
          }
        }
      }
    }
  }
  netwib_er(netwib_buf_close(&pkt));

  /* close */
  netwib_er(netwib_io_close(&psniffio));
  netwib_er(netwib_io_close(&pspoofio));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
