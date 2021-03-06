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
netwib_conststring t000149_description[] = {
  NETWOX_DESC_ping,
  NETWOX_DESC_pts_icmp6nd,
  NETWOX_DESC_pts_ether,
  NETWOX_DESC_toolpriv_snsp,
  NULL
};
netwox_toolarg t000149_args[] = {
  NETWOX_TOOLARG_REQ_IP6_DST('i', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPT_BUF_DEVICE('d', NULL, "spoof device", NULL),
  NETWOX_TOOLARG_OPT_ETH_SRC('E', NULL, NULL, "0:a:a:a:a:a"),
  NETWOX_TOOLARG_OPT_ETH_DST('e', NULL, NULL, "0:b:b:b:b:b"),
  NETWOX_TOOLARG_OPT_IP_SRC('I', NULL, NULL, "1.2.3.4"),
  NETWOX_TOOLARG_OPTA_UINT32('c', "max-count", "max requests", "4294967295"),
  NETWOX_TOOLARG_OPTA_UINT32('m', "max-ms", "max millisecond wait", "1000"),
  NETWOX_TOOLARG_OPTA_BOOL('b', "beep", "beep if reached", NULL),
  NETWOX_TOOLARG_OPTA_BOOL('1', "display01", "display only 0 or 1", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000149_nodes[] = {
  NETWOX_TOOLTREENODETYPE_PING,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000149_info = {
  "Ping ICMP6 Neighbor Discovery (EthIp spoof)",
  t000149_description,
  "ip6",
  t000149_args,
  t000149_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000149_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_ping_addinfo addinfo;
  netwib_ip dstip;
  netwib_bool isset;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000149_info, &parg));
  netwib_er(netwox_ping_addinfo_initdefault(&addinfo));
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
  netwib_er(netwox_arg_uint32(parg, 'c', &addinfo.maxcount));
  netwib_er(netwox_arg_uint32(parg, 'm', &addinfo.maxwaitms));
  netwib_er(netwox_arg_bool(parg, 'b', &addinfo.beep));
  netwib_er(netwox_arg_bool(parg, '1', &addinfo.display01));

  /* main loop */
  netwib_er(netwox_ping_loop(NETWOX_PING_METHOD_ICMP6ND_ETHIPSPOOF,
                             &dstip, &addinfo));

  /* close */
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
