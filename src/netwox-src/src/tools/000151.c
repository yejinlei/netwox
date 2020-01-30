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
netwib_conststring t000151_description[] = {
  NETWOX_DESC_scan_host,
  NETWOX_DESC_pts_icmp6nd,
  NETWOX_DESC_pts_ether,
  NETWOX_DESC_toolpriv_snsp,
  NULL
};
netwox_toolarg t000151_args[] = {
  NETWOX_TOOLARG_REQ_IPS_U('i', NULL, NULL, "fec0::1,fec0::2"),
  NETWOX_TOOLARG_OPT_BUF_DEVICE('d', NULL, "spoof device", NULL),
  NETWOX_TOOLARG_OPT_ETH_SRC('E', NULL, NULL, "0:a:a:a:a:a"),
  NETWOX_TOOLARG_OPT_IP_SRC('I', NULL, NULL, "1.2.3.4"),
  NETWOX_TOOLARG_OPTA_UINT32('m', "min-ms",
                             "min millisecond delay between packets", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('M', "max-ms",
                             "max millisecond wait for answers", "5000"),
  NETWOX_TOOLARG_OPT_BOOL('u', "disp-useful", "only display useful info",
                          NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000151_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SCAN,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000151_info = {
  "Scan ICMP6 Neighbor Discovery (EthIp spoof)",
  t000151_description,
  "nmap",
  t000151_args,
  t000151_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000151_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_scan_info info;
  netwib_bool isset;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000151_info, &parg));
  netwib_er(netwox_scan_info_initdefault(&info));
  netwib_er(netwox_arg_ips(parg, 'i', &info.pdstips));
  netwib_er(netwox_arg_buf(parg, 'd', &info.device));
  netwib_er(netwox_arg_isset(parg, 'E', &isset));
  if (isset) {
    info.srcethset = NETWIB_TRUE;
    netwib_er(netwox_arg_eth(parg, 'E', &info.srceth));
  }
  netwib_er(netwox_arg_isset(parg, 'I', &isset));
  if (isset) {
    info.srcipset = NETWIB_TRUE;
    netwib_er(netwox_arg_ip(parg, 'I', &info.srcip));
  }
  netwib_er(netwox_arg_uint32(parg, 'm', &info.mindelayms));
  netwib_er(netwox_arg_uint32(parg, 'M', &info.maxwaitms));
  netwib_er(netwox_arg_bool(parg, 'u', &info.onlyuseful));

  /* main loop */
  netwib_er(netwox_scan_loop(NETWOX_SCAN_METHOD_ICMP6ND_ETHIPSPOOF, &info));

  /* close */
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
