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
netwib_conststring t000185_description[] = {
  NETWOX_DESC_relay,
  "This tool is different from tool 183 because several relays can be in",
  "use simultaneously. It can be used like a load balancer.",
  "",
  NETWOX_DESC_servertcpmul,
  NETWOX_DESC_relay_para,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000185_args[] = {
  NETWOX_SOCK_ARG_TCP_MULSER,
  NETWOX_RELAY_ARG_IPS,
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000185_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SERVER_TCP_GENERIC,
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_GENERIC,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000185_info = {
  "TCP multiclient relay",
  t000185_description,
  NULL,
  t000185_args,
  t000185_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000185_relay(netwib_io *pio,
                                netwib_ptr pinfos)
{
  netwox_relay *prelay = (netwox_relay *)pinfos;

  netwib_er(netwox_relay_loop(prelay, pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err t000185_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwox_relay relay;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000185_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_tcp_mulser(parg, &sockinfo));
  netwib_er(netwox_relay_init_arg(parg, NETWOX_SOCKTYPE_TCP_MULSER, &relay));

  /* main loop */
  netwib_er(netwox_sock_tcp_mulser(&sockinfo, &t000185_relay, &relay));

  /* close */
  netwib_er(netwox_relay_close(&relay));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
