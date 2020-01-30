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
netwib_conststring t000214_description[] = {
  NETWOX_DESC_tracertdisc,
  "",
  NETWOX_DESC_spoofip,
  NETWOX_DESC_toolpriv_snsp,
  NULL
};
netwox_toolarg t000214_args[] = {
  NETWOX_TOOLARG_REQ_IPS_SU('i', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPT_PORTS_SU('p', "tcpports", "list of open tcp ports to scan", "21,22,23,25,53,79,80,88,110,113,119,139,143,389,443,445,1080,2401,6000"),
  NETWOX_TOOLARG_OPT_PORTS_SU('P', "udpports", "list of closed udp ports to scan", "1,53,67,68,123,137,138,161,162,177,514"),
  NETWOX_TOOLARG_OPT_BOOL('c', "icmp", "if icmp is also scanned", "1"),
  NETWOX_TOOLARG_OPTA_SPOOFIP('s', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "min-ttl", "min ttl", "1"),
  NETWOX_TOOLARG_OPTA_UINT32('t', "max-ttl", "max ttl", "30"),
  NETWOX_TOOLARG_OPTA_UINT32('m', "max-ms", "max millisecond wait", "1000"),
  NETWOX_TOOLARG_OPTA_BOOL('r', "resolve", "resolve hostname", "0"),
  NETWOX_TOOLARG_OPTA_BOOL('v', "verbose", "display intermediary results",
                           NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000214_nodes[] = {
  NETWOX_TOOLTREENODETYPE_TRACEROUTE,
  NETWOX_TOOLTREENODETYPE_SCAN,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000214_info = {
  "Traceroute discovery: graph of network topology",
  t000214_description,
  NULL,
  t000214_args,
  t000214_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000214_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_tracertdisc tracertdisc;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000214_info, &parg));
  netwib_er(netwox_tracert_addinfo_initdefault(&tracertdisc.addinfo));
  netwib_er(netwox_arg_ips(parg, 'i', &tracertdisc.pdstips));
  netwib_er(netwox_arg_ports(parg, 'p', &tracertdisc.pdsttcpports));
  netwib_er(netwox_arg_ports(parg, 'P', &tracertdisc.pdstudpports));
  netwib_er(netwox_arg_bool(parg, 'c', &tracertdisc.useicmp));
  netwib_er(netwox_arg_spoofip(parg, 's', &tracertdisc.addinfo.spoofipinittype));
  netwib_er(netwox_arg_uint8(parg, 'T', &tracertdisc.addinfo.minttl));
  netwib_er(netwox_arg_uint8(parg, 't', &tracertdisc.addinfo.maxttl));
  netwib_er(netwox_arg_uint32(parg, 'm', &tracertdisc.addinfo.maxwaitms));
  netwib_er(netwox_arg_bool(parg, 'r', &tracertdisc.addinfo.resolve));
  netwib_er(netwox_arg_bool(parg, 'v', &tracertdisc.verbose));
  tracertdisc.ethspoof = NETWIB_FALSE;

  /* main loop */
  netwib_er(netwox_tracertdisc_loop(&tracertdisc));

  /* close */
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
