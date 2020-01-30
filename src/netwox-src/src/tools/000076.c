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
netwib_conststring t000076_description[] = {
  "This tool sends a lot of TCP SYN packets.",
  "It permits to check how a firewall behaves when receiving packets",
  "which have to be ignored.",
  NETWOX_DESC_spoofip,
  NETWOX_DESC_toolpriv_spoof,
  NULL
};
netwox_toolarg t000076_args[] = {
  NETWOX_TOOLARG_REQ_IP_DST('i', NULL, NULL, NULL),
  NETWOX_TOOLARG_REQ_PORT_DST('p', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPT_SPOOFIP('s', NULL, NULL, "linkbraw"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000076_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NETAUDIT_TCP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000076_info = {
  "Synflood",
  t000076_description,
  NULL,
  t000076_args,
  t000076_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000076_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_io *pio;
  netwib_spoof_ip_inittype inittype;
  netwib_ip dstip;
  netwib_port dstport;
  netwib_buf pkt;
  netwib_uint32 ui;
  netwib_iphdr ipheader;
  netwib_tcphdr tcpheader;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000076_info, &parg));

  netwib_er(netwox_arg_spoofip(parg, 's', &inittype));
  netwib_er(netwib_io_init_spoof_ip(inittype, &pio));
  netwib_er(netwox_arg_ip(parg, 'i', &dstip));
  netwib_er(netwox_arg_port(parg, 'p', &dstport));

  /* main loop */
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  while (NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();
    netwib_er(netwib_iphdr_initdefault(dstip.iptype, &ipheader));
    if (dstip.iptype == NETWIB_IPTYPE_IP4) {
      netwib_er(netwib_uint32_init_rand(1, 0xFFFFFFFEu, &ui));
      netwib_er(netwib_ip_init_ip4(ui, &ipheader.src));
    } else {
      ipheader.src = dstip;
      netwib_er(netwib_uint32_init_rand(1, 0xFEu, &ui));
      ipheader.src.ipvalue.ip6.b[12] = (netwib_byte)ui;
      netwib_er(netwib_uint32_init_rand(1, 0xFEu, &ui));
      ipheader.src.ipvalue.ip6.b[13] = (netwib_byte)ui;
      netwib_er(netwib_uint32_init_rand(1, 0xFEu, &ui));
      ipheader.src.ipvalue.ip6.b[14] = (netwib_byte)ui;
      netwib_er(netwib_uint32_init_rand(1, 0xFEu, &ui));
      ipheader.src.ipvalue.ip6.b[15] = (netwib_byte)ui;
    }
    ipheader.dst = dstip;
    netwib_er(netwib_tcphdr_initdefault(&tcpheader));
    tcpheader.syn = NETWIB_TRUE;
    netwib_er(netwib_uint32_init_rand(1024, 0xFFFF, &tcpheader.src));
    tcpheader.dst = dstport;
    netwib__buf_reinit(&pkt);
    netwib_er(netwib_pkt_append_iptcpdata(&ipheader, &tcpheader, NULL, &pkt));
    netwib_er(netwib_io_write(pio, &pkt));
  }
  netwib_er(netwib_buf_close(&pkt));

  /* close */
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
