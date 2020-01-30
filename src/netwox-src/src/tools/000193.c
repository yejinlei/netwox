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
netwib_conststring t000193_description[] = {
  "This tool queries an IDENT server.",
  "It obtains information about a current TCP session between local",
  "computer and IDENT server computer. This current session is defined",
  "by --localport and --remoteport.",
  "",
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000193_args[] = {
  NETWOX_SOCK_ARG_TCP_CLIPORT("113"),
  NETWOX_TOOLARG_REQ_PORT_SRC('L', "localport",
                              "local port of session to query", NULL),
  NETWOX_TOOLARG_REQ_PORT_DST('R', "remoteport",
                              "remote port of session to query", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in milliseconds",
                             "60000"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000193_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_IDENT,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000193_info = {
  "IDENT client requesting info about an open session",
  t000193_description,
  NULL,
  t000193_args,
  t000193_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000193_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_buf reply;
  netwib_io *pio, *pioline;
  netwib_uint32 maxwaitms;
  netwib_port localport, remoteport;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000193_info, &parg));
  netwib_er(netwox_arg_port(parg, 'L', &localport));
  netwib_er(netwox_arg_port(parg, 'R', &remoteport));
  netwib_er(netwox_arg_uint32(parg, 'T', &maxwaitms));

  /* connect to server */
  netwib_er(netwox_sockinfo_init_arg_tcp_cli(parg, &sockinfo));
  netwib_er(netwox_sock_init(&sockinfo, &pio));
  netwib_er(netwox_txtproto_io_rd(pio, &pioline));

  /* query */
  netwib_er(netwib_buf_init_mallocdefault(&reply));
  netwib_er(netwox_identclient_query(pioline, localport, remoteport, maxwaitms,
                                     &reply));
  netwib_er(netwib_fmt_display("%{buf}\n", &reply));
  netwib_er(netwib_buf_close(&reply));

  /* close */
  netwib_er(netwib_io_close(&pioline));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
