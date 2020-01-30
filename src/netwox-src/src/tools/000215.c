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
netwib_conststring t000215_description[] = {
  NETWOX_DESC_tracertdisc,
  "",
  NETWOX_DESC_pts_ether,
  NETWOX_DESC_toolpriv_snsp,
  NULL
};
netwox_toolarg t000215_args[] = {
  NETWOX_TOOLARG_REQ_IPS_SU('i', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPT_PORTS_SU('p', "tcpports", "list of open tcp ports to scan", "21,22,23,25,53,79,80,88,110,113,119,139,143,389,443,445,1080,2401,6000"),
  NETWOX_TOOLARG_OPT_PORTS_SU('P', "udpports", "list of closed udp ports to scan", "1,53,67,68,123,137,138,161,162,177,514"),
  NETWOX_TOOLARG_OPT_BOOL('c', "icmp", "if icmp is also scanned", "1"),
  NETWOX_TOOLARG_OPT_BUF_DEVICE('d', NULL, "spoof device", NULL),
  NETWOX_TOOLARG_OPT_ETH_SRC('E', NULL, NULL, "0:a:a:a:a:a"),
  NETWOX_TOOLARG_OPT_ETH_DST('e', NULL, NULL, "0:b:b:b:b:b"),
  NETWOX_TOOLARG_OPT_IP_SRC('I', NULL, NULL, "1.2.3.4"),
  NETWOX_TOOLARG_OPTA_UINT32('T', "min-ttl", "min ttl", "1"),
  NETWOX_TOOLARG_OPTA_UINT32('t', "max-ttl", "max ttl", "30"),
  NETWOX_TOOLARG_OPTA_UINT32('m', "max-ms", "max millisecond wait", "1000"),
  NETWOX_TOOLARG_OPTA_BOOL('r', "resolve", "resolve hostname", "0"),
  NETWOX_TOOLARG_OPTA_BOOL('v', "verbose", "display intermediary results",
                           NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000215_nodes[] = {
  NETWOX_TOOLTREENODETYPE_TRACEROUTE,
  NETWOX_TOOLTREENODETYPE_SCAN,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000215_info = {
  "Traceroute discovery (EthIp spoof)",
  t000215_description,
  NULL,
  t000215_args,
  t000215_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000215_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_tracertdisc tracertdisc;
  netwib_bool isset;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000215_info, &parg));
  netwib_er(netwox_tracert_addinfo_initdefault(&tracertdisc.addinfo));
  netwib_er(netwox_arg_ips(parg, 'i', &tracertdisc.pdstips));
  netwib_er(netwox_arg_ports(parg, 'p', &tracertdisc.pdsttcpports));
  netwib_er(netwox_arg_ports(parg, 'P', &tracertdisc.pdstudpports));
  netwib_er(netwox_arg_bool(parg, 'c', &tracertdisc.useicmp));
  netwib_er(netwox_arg_buf(parg, 'd', &tracertdisc.addinfo.device));
  netwib_er(netwox_arg_isset(parg, 'E', &isset));
  if (isset) {
    tracertdisc.addinfo.srcethset = NETWIB_TRUE;
    netwib_er(netwox_arg_eth(parg, 'E', &tracertdisc.addinfo.srceth));
  }
  netwib_er(netwox_arg_isset(parg, 'e', &isset));
  if (isset) {
    tracertdisc.addinfo.dstethset = NETWIB_TRUE;
    netwib_er(netwox_arg_eth(parg, 'e', &tracertdisc.addinfo.dsteth));
  }
  netwib_er(netwox_arg_isset(parg, 'I', &isset));
  if (isset) {
    tracertdisc.addinfo.srcipset = NETWIB_TRUE;
    netwib_er(netwox_arg_ip(parg, 'I', &tracertdisc.addinfo.srcip));
  }
  netwib_er(netwox_arg_uint8(parg, 'T', &tracertdisc.addinfo.minttl));
  netwib_er(netwox_arg_uint8(parg, 't', &tracertdisc.addinfo.maxttl));
  netwib_er(netwox_arg_uint32(parg, 'm', &tracertdisc.addinfo.maxwaitms));
  netwib_er(netwox_arg_bool(parg, 'r', &tracertdisc.addinfo.resolve));
  netwib_er(netwox_arg_bool(parg, 'v', &tracertdisc.verbose));
  tracertdisc.ethspoof = NETWIB_TRUE;

  /* main loop */
  netwib_er(netwox_tracertdisc_loop(&tracertdisc));

  /* close */
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
