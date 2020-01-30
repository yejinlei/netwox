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
netwib_conststring t000049_description[] = {
  NETWOX_DESC_ping,
  NETWOX_DESC_pts_icmp,
  NETWOX_DESC_spoofip,
  NETWOX_DESC_toolpriv_snsp,
  NULL
};
netwox_toolarg t000049_args[] = {
  NETWOX_TOOLARG_REQ_IP_DST('i', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPTA_SPOOFIP('s', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPTA_UINT32('c', "max-count", "max requests", "4294967295"),
  NETWOX_TOOLARG_OPTA_UINT32('m', "max-ms", "max millisecond wait", "1000"),
  NETWOX_TOOLARG_OPTA_BOOL('b', "beep", "beep if reached", NULL),
  NETWOX_TOOLARG_OPTA_BOOL('1', "display01", "display only 0 or 1", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000049_nodes[] = {
  NETWOX_TOOLTREENODETYPE_PING,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000049_info = {
  "Ping ICMP",
  t000049_description,
  NULL,
  t000049_args,
  t000049_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000049_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_ping_addinfo addinfo;
  netwib_ip dstip;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000049_info, &parg));
  netwib_er(netwox_ping_addinfo_initdefault(&addinfo));
  netwib_er(netwox_arg_ip(parg, 'i', &dstip));
  netwib_er(netwox_arg_spoofip(parg, 's', &addinfo.spoofipinittype));
  netwib_er(netwox_arg_uint32(parg, 'c', &addinfo.maxcount));
  netwib_er(netwox_arg_uint32(parg, 'm', &addinfo.maxwaitms));
  netwib_er(netwox_arg_bool(parg, 'b', &addinfo.beep));
  netwib_er(netwox_arg_bool(parg, '1', &addinfo.display01));

  /* main loop */
  netwib_er(netwox_ping_loop(NETWOX_PING_METHOD_PING, &dstip, &addinfo));

  /* close */
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
