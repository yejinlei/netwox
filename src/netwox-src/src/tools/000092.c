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
netwib_conststring t000092_description[] = {
  NETWOX_DESC_mulser,
  "",
  NETWOX_DESC_server,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000092_args[] = {
  NETWOX_SOCK_ARG_UDP_MULSER,
  NETWOX_TOOLARG_OPT_BOOL('s', "showscreen", "show data to screen", NULL),
  NETWOX_TOOLARG_OPT_BOOL('b', "echoback", "echo data back to client", "1"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000092_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SERVER_UDP_GENERIC,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000092_info = {
  "UDP server multiclients",
  t000092_description,
  "nc, netcat",
  t000092_args,
  t000092_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000092_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_io *pio;
  netwib_bool showscreen, echoback;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000092_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_udp_mulser(parg, &sockinfo));
  netwib_er(netwox_arg_bool(parg, 's', &showscreen));
  netwib_er(netwox_arg_bool(parg, 'b', &echoback));

  /* main loop */
  netwib_er(netwox_sock_init(&sockinfo, &pio));
  netwib_er(netwox_io_showecho(pio, 0, showscreen, echoback));

  /* close */
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}

