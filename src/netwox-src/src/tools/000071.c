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
netwib_conststring t000071_description[] = {
  NETWOX_DESC_scan_host,
  NETWOX_DESC_pts_arp,
  NETWOX_DESC_toolpriv_snsp,
  NULL
};
netwox_toolarg t000071_args[] = {
  NETWOX_TOOLARG_REQ_IPS_U('i', NULL, NULL, "1.2.3.4,5.6.7.8"),
  NETWOX_TOOLARG_OPTA_UINT32('m', "min-ms",
                             "min millisecond delay between packets", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('M', "max-ms",
                             "max millisecond wait for answers", "5000"),
  NETWOX_TOOLARG_OPTA_BOOL('u', "disp-useful", "only display useful info",
                           NULL),
  NETWOX_TOOLARG_OPTA_UINT32('n', "numtargets",
                             "number of simultaneous targets", "20"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000071_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SCAN,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000071_info = {
  "Scan ARP",
  t000071_description,
  "nmap",
  t000071_args,
  t000071_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000071_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_scan_info info;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000071_info, &parg));
  netwib_er(netwox_scan_info_initdefault(&info));
  netwib_er(netwox_arg_ips(parg, 'i', &info.pdstips));
  netwib_er(netwox_arg_uint32(parg, 'm', &info.mindelayms));
  netwib_er(netwox_arg_uint32(parg, 'M', &info.maxwaitms));
  netwib_er(netwox_arg_bool(parg, 'u', &info.onlyuseful));
  netwib_er(netwox_arg_uint32(parg, 'n', &info.numtargets));

  /* main loop */
  netwib_er(netwox_scan_loop(NETWOX_SCAN_METHOD_ARP, &info));

  /* close */
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
