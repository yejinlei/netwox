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
netwib_conststring t000081_description[] = {
  "This tool sends an ICMP timestamp, and waits for an answer.",
  "The timestamp is the number of milliseconds since midnight UT.",
  "It permits to know computer time.",
  NETWOX_DESC_spoofip,
  NETWOX_DESC_toolpriv_snsp,
  NULL
};
netwox_toolarg t000081_args[] = {
  NETWOX_TOOLARG_REQ_IP_DST('i', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPT_SPOOFIP('s', NULL, NULL, NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000081_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NETAUDIT_ICMP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000081_info = {
  "Send an ICMP4 timestamp",
  t000081_description,
  NULL,
  t000081_args,
  t000081_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000081_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_io *psniffio, *pspoofio;
  netwib_spoof_ip_inittype inittype;
  netwib_ip srcip, dstip;
  netwib_buf pkt, filter, device;
  netwib_iphdr ipheader, ipheader2;
  netwib_icmp4 icmp4, icmp42;
  netwib_time t;
  netwib_bool event;
  netwib_cmp cmp;
  netwib_err ret;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000081_info, &parg));
  netwib_er(netwox_arg_spoofip(parg, 's', &inittype));
  netwib_er(netwox_arg_ip(parg, 'i', &dstip));

  /* initialize io */
  netwib_er(netwib_buf_init_mallocdefault(&device));
  netwib_er(netwib_buf_init_mallocdefault(&filter));
  netwib_er(netwib_conf_routes_reach_ip(&dstip, &device, &srcip));
  netwib_er(netwib_io_init_spoof_ip(inittype, &pspoofio));
  netwib_er(netwib_buf_append_fmt(&filter, "icmp and host %{ip}", &dstip));
  netwib_er(netwib_io_init_sniff_ip(&device, &filter, NETWIB_FALSE,
                                    NETWIB_FALSE, &psniffio));
  netwib_er(netwib_buf_close(&filter));
  netwib_er(netwib_buf_close(&device));

  /* create request */
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_iphdr_initdefault(dstip.iptype, &ipheader));
  ipheader.src = srcip;
  ipheader.dst = dstip;
  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_TIMESTAMPREQ, &icmp4));
  netwib_er(netwib_pkt_append_ipicmp4(&ipheader, &icmp4, &pkt));
  netwib_er(netwib_io_write(pspoofio, &pkt));

  /* main loop */
  /* wait 1 second max */
  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_plus_sec(&t, 1));
  while (NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();
    /* sniff */
    netwib_er(netwib_io_wait_read(psniffio, &t, &event));
    if (!event) {
      netwib_er(netwib_fmt_display("No answer.\n"));
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
    ret = netwib_pkt_decode_ipicmp4(&pkt, &ipheader2, &icmp42);
    if (ret != NETWIB_ERR_OK) continue;
    netwib_er(netwib_ip_cmp(&ipheader2.src, &ipheader.dst, &cmp));
    if (cmp != NETWIB_CMP_EQ) continue;
    netwib_er(netwib_ip_cmp(&ipheader2.dst, &ipheader.src, &cmp));
    if (cmp != NETWIB_CMP_EQ) continue;
    if (icmp42.type != NETWIB_ICMP4TYPE_TIMESTAMPREP) continue;
    if (icmp42.code != NETWIB_ICMP4CODE_TIMESTAMPREP) continue;
    if (icmp42.msg.timestamp.id != icmp4.msg.timestamp.id) continue;
    if (icmp42.msg.timestamp.seqnum != icmp4.msg.timestamp.seqnum) continue;
    if (ret != NETWIB_ERR_OK) continue;
    netwib_er(netwib_fmt_display("%{uint32} %{uint32}\n",
                                 icmp4.msg.timestamp.receivetimestamp,
                                 icmp4.msg.timestamp.transmittimestamp));
    break;
  }
  netwib_er(netwib_buf_close(&pkt));

  /* close */
  netwib_er(netwib_io_close(&psniffio));
  netwib_er(netwib_io_close(&pspoofio));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
