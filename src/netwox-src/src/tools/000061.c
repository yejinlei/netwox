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
netwib_conststring t000061_description[] = {
  NETWOX_DESC_traceroute,
  NETWOX_DESC_pts_udp,
  NETWOX_DESC_spoofip,
  NETWOX_DESC_toolpriv_snsp,
  NULL
};
netwox_toolarg t000061_args[] = {
  NETWOX_TOOLARG_REQ_IP_DST('i', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPTA_SPOOFIP('s', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPT_PORT_DST('p', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "min-ttl", "min ttl", "1"),
  NETWOX_TOOLARG_OPTA_UINT32('t', "max-ttl", "max ttl", "30"),
  NETWOX_TOOLARG_OPTA_UINT32('m', "max-ms", "max millisecond wait", "1000"),
  NETWOX_TOOLARG_OPTA_BOOL('r', "resolve", "resolve hostname", "0"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000061_nodes[] = {
  NETWOX_TOOLTREENODETYPE_TRACEROUTE,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000061_info = {
  "Traceroute UDP",
  t000061_description,
  NULL,
  t000061_args,
  t000061_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000061_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_tracert_addinfo addinfo;
  netwib_ip dstip;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000061_info, &parg));
  netwib_er(netwox_tracert_addinfo_initdefault(&addinfo));
  netwib_er(netwox_arg_ip(parg, 'i', &dstip));
  netwib_er(netwox_arg_spoofip(parg, 's', &addinfo.spoofipinittype));
  netwib_er(netwox_arg_port(parg, 'p', &addinfo.dstport));
  netwib_er(netwox_arg_uint8(parg, 'T', &addinfo.minttl));
  netwib_er(netwox_arg_uint8(parg, 't', &addinfo.maxttl));
  netwib_er(netwox_arg_uint32(parg, 'm', &addinfo.maxwaitms));
  netwib_er(netwox_arg_bool(parg, 'r', &addinfo.resolve));

  /* main loop */
  netwib_er(netwox_tracert_loop(NETWOX_TRACERT_METHOD_UDP, &dstip, &addinfo,
                                NULL));

  /* close */
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
