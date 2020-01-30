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
netwib_conststring t000060_description[] = {
  NETWOX_DESC_traceroute,
  NETWOX_DESC_pts_tcp,
  NETWOX_DESC_pts_ether,
  NETWOX_DESC_toolpriv_snsp,
  NULL
};
netwox_toolarg t000060_args[] = {
  NETWOX_TOOLARG_REQ_IP_DST('i', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPT_BUF_DEVICE('d', NULL, "spoof device", NULL),
  NETWOX_TOOLARG_OPT_ETH_SRC('E', NULL, NULL, "0:a:a:a:a:a"),
  NETWOX_TOOLARG_OPT_ETH_DST('e', NULL, NULL, "0:b:b:b:b:b"),
  NETWOX_TOOLARG_OPT_IP_SRC('I', NULL, NULL, "1.2.3.4"),
  NETWOX_TOOLARG_OPT_PORT_DST('p', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "min-ttl", "min ttl", "1"),
  NETWOX_TOOLARG_OPTA_UINT32('t', "max-ttl", "max ttl", "30"),
  NETWOX_TOOLARG_OPTA_UINT32('m', "max-ms", "max millisecond wait", "1000"),
  NETWOX_TOOLARG_OPTA_BOOL('r', "resolve", "resolve hostname", "0"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000060_nodes[] = {
  NETWOX_TOOLTREENODETYPE_TRACEROUTE,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000060_info = {
  "Traceroute TCP (EthIp spoof)",
  t000060_description,
  "tcptraceroute",
  t000060_args,
  t000060_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000060_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_tracert_addinfo addinfo;
  netwib_ip dstip;
  netwib_bool isset;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000060_info, &parg));
  netwib_er(netwox_tracert_addinfo_initdefault(&addinfo));
  netwib_er(netwox_arg_ip(parg, 'i', &dstip));
  netwib_er(netwox_arg_buf(parg, 'd', &addinfo.device));
  netwib_er(netwox_arg_isset(parg, 'E', &isset));
  if (isset) {
    addinfo.srcethset = NETWIB_TRUE;
    netwib_er(netwox_arg_eth(parg, 'E', &addinfo.srceth));
  }
  netwib_er(netwox_arg_isset(parg, 'e', &isset));
  if (isset) {
    addinfo.dstethset = NETWIB_TRUE;
    netwib_er(netwox_arg_eth(parg, 'e', &addinfo.dsteth));
  }
  netwib_er(netwox_arg_isset(parg, 'I', &isset));
  if (isset) {
    addinfo.srcipset = NETWIB_TRUE;
    netwib_er(netwox_arg_ip(parg, 'I', &addinfo.srcip));
  }
  netwib_er(netwox_arg_port(parg, 'p', &addinfo.dstport));
  netwib_er(netwox_arg_uint8(parg, 'T', &addinfo.minttl));
  netwib_er(netwox_arg_uint8(parg, 't', &addinfo.maxttl));
  netwib_er(netwox_arg_uint32(parg, 'm', &addinfo.maxwaitms));
  netwib_er(netwox_arg_bool(parg, 'r', &addinfo.resolve));

  /* main loop */
  netwib_er(netwox_tracert_loop(NETWOX_TRACERT_METHOD_TCP_ETHIPSPOOF,
                                &dstip, &addinfo, NULL));

  /* close */
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
