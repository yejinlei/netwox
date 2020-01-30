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
netwib_conststring t000065_description[] = {
  NETWOX_DESC_scan_host,
  NETWOX_DESC_pts_icmp,
  NETWOX_DESC_spoofip,
  NETWOX_DESC_toolpriv_snsp,
  NULL
};
netwox_toolarg t000065_args[] = {
  NETWOX_TOOLARG_REQ_IPS_U('i', NULL, NULL, "1.2.3.4,5.6.7.8"),
  NETWOX_TOOLARG_OPTA_SPOOFIP('s', NULL, NULL, NULL),
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
netwox_tooltreenodetype t000065_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SCAN,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000065_info = {
  "Scan ICMP",
  t000065_description,
  "nmap",
  t000065_args,
  t000065_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000065_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_scan_info info;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000065_info, &parg));
  netwib_er(netwox_scan_info_initdefault(&info));
  netwib_er(netwox_arg_ips(parg, 'i', &info.pdstips));
  netwib_er(netwox_arg_spoofip(parg, 's', &info.spoofipinittype));
  netwib_er(netwox_arg_uint32(parg, 'm', &info.mindelayms));
  netwib_er(netwox_arg_uint32(parg, 'M', &info.maxwaitms));
  netwib_er(netwox_arg_bool(parg, 'u', &info.onlyuseful));
  netwib_er(netwox_arg_uint32(parg, 'n', &info.numtargets));

  /* main loop */
  netwib_er(netwox_scan_loop(NETWOX_SCAN_METHOD_PING, &info));

  /* close */
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
