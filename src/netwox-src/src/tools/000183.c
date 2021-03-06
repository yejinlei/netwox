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
netwib_conststring t000183_description[] = {
  NETWOX_DESC_relay,
  "",
  NETWOX_DESC_server,
  NETWOX_DESC_relay_para,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000183_args[] = {
  NETWOX_SOCK_ARG_TCP_SER,
  NETWOX_RELAY_ARG_IP,
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000183_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SERVER_TCP_GENERIC,
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_GENERIC,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000183_info = {
  "TCP relay",
  t000183_description,
  NULL,
  t000183_args,
  t000183_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000183_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_io *plocalio;
  netwox_relay relay;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000183_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_tcp_ser(parg, &sockinfo));
  netwib_er(netwox_sock_init(&sockinfo, &plocalio));
  netwib_er(netwox_relay_init_arg(parg, NETWOX_SOCKTYPE_TCP_SER, &relay));

  /* main loop */
  netwib_er(netwox_relay_loop(&relay, plocalio));

  /* close */
  netwib_er(netwox_relay_close(&relay));
  netwib_er(netwib_io_close(&plocalio));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
