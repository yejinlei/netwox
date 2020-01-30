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
netwib_conststring t000005_description[] = {
  "This tool sends ARP Requests (or IP6 Neighbor Discovery) to obtain",
  "Ethernet addresses of several computers.",
  "For example: 'netwox 5 192.168.0.0/24,10.0.0.1-10.0.0.4'.",
  "If parameter --no-unresolved is used, unresolved addresses are not",
  "displayed.",
  NETWOX_DESC_toolpriv_snsp_resolveth,
  NULL
};
netwox_toolarg t000005_args[] = {
  NETWOX_TOOLARG_OPT_BOOL('u', "unresolved", "display unresolved items", NULL),
  NETWOX_TOOLARG_REQ_IPS_U('i', NULL, NULL, "192.168.1.0/24"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000005_nodes[] = {
  NETWOX_TOOLTREENODETYPE_INFO_REMOTE,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000005_info = {
  "Obtain Ethernet addresses of computers in an IP list",
  t000005_description,
  "host, mac, resolve, show",
  t000005_args,
  t000005_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000005_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_bool displayunresolved;
  netwib_ips_index *pipsindex;
  netwib_ips *pips;
  netwib_ip ipad;
  netwib_eth eth;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwox_arg_init(argc, argv, &t000005_info, &parg));
  netwib_er(netwox_arg_bool(parg, 'u', &displayunresolved));
  netwib_er(netwox_arg_ips(parg, 'i', &pips));

  netwib_er(netwib_ips_index_init(pips, &pipsindex));
  while (NETWIB_TRUE) {
    ret = netwib_ips_index_next_ip(pipsindex, &ipad);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    ret = netwib_eth_init_ip(&ipad, &eth);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_fmt_display("%{ip}\t%{eth}\n", &ipad, &eth));
    } else {
      if (displayunresolved) {
        netwib_er(netwib_fmt_display("%{ip}\tunresolved\n", &ipad));
      }
    }
  }
  netwib_er(netwib_ips_index_close(&pipsindex));

  netwib_er(netwox_arg_close(&parg));

  return(ret);
}
