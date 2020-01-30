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
netwib_conststring t000087_description[] = {
  "This tool connects to a TCP server. It's like telnet command.",
  "",
  NETWOX_DESC_client,
  NETWOX_DESC_cliser_encdec,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000087_args[] = {
  NETWOX_SOCK_ARG_TCP_CLI,
  NETWOX_TOOLARG_OPTA_DECODE('k', NULL, "how keyboard data is entered", NULL),
  NETWOX_TOOLARG_OPTA_ENCODE('n', NULL, "how network data is displayed", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000087_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_GENERIC,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000087_info = {
  "TCP client",
  t000087_description,
  "nc, netcat",
  t000087_args,
  t000087_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000087_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_io *pio;
  netwib_encodetype encodetype;
  netwib_decodetype decodetype;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000087_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_tcp_cli(parg, &sockinfo));
  netwib_er(netwox_arg_decode(parg, 'k', &decodetype));
  netwib_er(netwox_arg_encode(parg, 'n', &encodetype));

  /* main loop */
  netwib_er(netwox_sock_init(&sockinfo, &pio));
  netwib_er(netwox_io_kbd_loop(pio, NETWIB_TRUE, NETWIB_TRUE,
                               decodetype, encodetype));

  /* close */
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
