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
netwib_conststring t000055_description[] = {
  NETWOX_DESC_ping,
  NETWOX_DESC_pts_arp,
  NETWOX_DESC_toolpriv_snsp,
  NULL
};
netwox_toolarg t000055_args[] = {
  NETWOX_TOOLARG_REQ_IP_DST('i', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPT_UINT32('c', "max-count", "max requests", "4294967295"),
  NETWOX_TOOLARG_OPT_UINT32('m', "max-ms", "max millisecond wait", "1000"),
  NETWOX_TOOLARG_OPT_BOOL('b', "beep", "beep if reached", NULL),
  NETWOX_TOOLARG_OPT_BOOL('1', "display01", "display only 0 or 1", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000055_nodes[] = {
  NETWOX_TOOLTREENODETYPE_PING,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000055_info = {
  "Ping ARP",
  t000055_description,
  NULL,
  t000055_args,
  t000055_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000055_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_ping_addinfo addinfo;
  netwib_ip dstip;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000055_info, &parg));
  netwib_er(netwox_ping_addinfo_initdefault(&addinfo));
  netwib_er(netwox_arg_ip(parg, 'i', &dstip));
  netwib_er(netwox_arg_uint32(parg, 'c', &addinfo.maxcount));
  netwib_er(netwox_arg_uint32(parg, 'm', &addinfo.maxwaitms));
  netwib_er(netwox_arg_bool(parg, 'b', &addinfo.beep));
  netwib_er(netwox_arg_bool(parg, '1', &addinfo.display01));

  /* main loop */
  netwib_er(netwox_ping_loop(NETWOX_PING_METHOD_ARP, &dstip, &addinfo));

  /* close */
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
