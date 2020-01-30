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
netwib_conststring t000001_description[] = {
  "This tool displays network configuration:",
  " - the list of devices/interfaces:",
  "    + nu: device number",
  "    + dev: easy device name",
  "    + eth_hw: Ethernet address or hardware type (if not Ethernet)",
  "    + mtu: MTU (maximum size of packets)",
  "    + real_dev: real device name",
  " - the list of IP addresses:",
  "    + nu: device number of device associated to this address",
  "    + ip: IP address",
  "    + netmask: network mask",
  "    + ppp: if true(1), this address is a Point To Point",
  "    + ppp_with: if ppp, this is the address of remote endpoint",
  " - the IP4 ARP cache or IP6 neighbor (this contains Ethernet",
  "   addresses for other computers)",
  "    + nu: device number of device associated to this entry",
  "    + eth: Ethernet address of computer",
  "    + ip: IP address of computer",
  " - the routes",
  "    + nu: device number of device associated to this entry",
  "    + destination/netmask: destination addresses",
  "    + source: source IP address, or local for a local route",
  "    + gateway: gateway (first router) to use",
  "    + metric: metric of route",
  "",
  "Parameter --device ask to display devices list.",
  "Parameter --ip ask to display ip list.",
  "Parameter --arpcache ask to display ARP cache and neighbors.",
  "Parameter --routes ask to display routes list.",
  "If no Parameter is set, they are all displayed.",
  NETWOX_DESC_toolpriv_conf,
  NULL
};
netwox_toolarg t000001_args[] = {
  NETWOX_TOOLARG_OPT_BOOL('d', "devices", "display devices", NULL),
  NETWOX_TOOLARG_OPT_BOOL('i', "ip", "display ip addresses", NULL),
  NETWOX_TOOLARG_OPT_BOOL('a', "arpcache", "display arp cache and neighbors",
                         NULL),
  NETWOX_TOOLARG_OPT_BOOL('r', "routes", "display routes", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000001_nodes[] = {
  NETWOX_TOOLTREENODETYPE_INFO_LOCAL,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000001_info = {
  "Display network configuration",
  t000001_description,
  "address, arp, device, gateway, ifconfig, interface, ipconfig, mac, neighbor, netmask, route, show",
  t000001_args,
  t000001_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000001_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_bool wantd, wanti, wanta, wantr, disp;

  netwib_er(netwox_arg_init(argc, argv, &t000001_info, &parg));
  netwib_er(netwox_arg_bool(parg, 'd', &wantd));
  netwib_er(netwox_arg_bool(parg, 'i', &wanti));
  netwib_er(netwox_arg_bool(parg, 'a', &wanta));
  netwib_er(netwox_arg_bool(parg, 'r', &wantr));
  netwib_er(netwox_arg_close(&parg));

  disp = NETWIB_FALSE;
  if (wantd) {
    netwib_er(netwib_conf_devices_display());
    disp = NETWIB_TRUE;
  }
  if (wanti) {
    netwib_er(netwib_conf_ip_display());
    disp = NETWIB_TRUE;
  }
  if (wanta) {
    netwib_er(netwib_conf_arpcache_display());
    disp = NETWIB_TRUE;
  }
  if (wantr) {
    netwib_er(netwib_conf_routes_display());
    disp = NETWIB_TRUE;
  }

  if ( ! disp) {
    netwib_er(netwib_conf_display());
  }

  return(NETWIB_ERR_OK);
}
