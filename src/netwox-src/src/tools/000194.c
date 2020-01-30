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
netwib_conststring t000194_description[] = {
  "This tool queries an IDENT server.",
  "It creates a TCP session to another port of the IDENT server, and",
  "obtains information about this TCP session. The port to connect to",
  "is --remoteport (for example 80, 22, 25, etc.).",
  "",
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000194_args[] = {
  NETWOX_SOCK_ARG_TCP_CLIPORT("113"),
  NETWOX_TOOLARG_REQ_PORT_DST('R', "remoteport", "remote port to query", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in milliseconds",
                             "60000"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000194_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_IDENT,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000194_info = {
  "IDENT client creating a session and requesting its info",
  t000194_description,
  NULL,
  t000194_args,
  t000194_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000194_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfosvc, sockinfoident;
  netwib_buf reply;
  netwib_io *piosvc, *pioident, *pioidentline;
  netwib_uint32 maxwaitms;
  netwib_port remoteportsvc, remoteportident, localportsvc;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000194_info, &parg));
  netwib_er(netwox_arg_port(parg, 'R', &remoteportsvc));
  netwib_er(netwox_arg_uint32(parg, 'T', &maxwaitms));

  /* connect to server (first on service, then on ident) */
  netwib_er(netwox_sockinfo_init_arg_tcp_cli(parg, &sockinfosvc));
  remoteportident = sockinfosvc.remoteport;
  sockinfosvc.remoteport = remoteportsvc;
  netwib_er(netwox_sock_init(&sockinfosvc, &piosvc));
  netwib_er(netwib_sock_ctl_get_local(piosvc, NULL, &localportsvc));
  netwib_er(netwox_sockinfo_init_connect(&sockinfosvc, piosvc, remoteportident,
                                         &sockinfoident));
  netwib_er(netwox_sock_init(&sockinfoident, &pioident));
  netwib_er(netwox_txtproto_io_rd(pioident, &pioidentline));

  /* query */
  netwib_er(netwib_buf_init_mallocdefault(&reply));
  netwib_er(netwox_identclient_query(pioidentline, localportsvc, remoteportsvc,
                                     maxwaitms, &reply));
  netwib_er(netwib_fmt_display("%{buf}\n", &reply));
  netwib_er(netwib_buf_close(&reply));

  /* close */
  netwib_er(netwib_io_close(&piosvc));
  netwib_er(netwib_io_close(&pioidentline));
  netwib_er(netwox_sockinfo_close(&sockinfosvc));
  netwib_er(netwox_sockinfo_close(&sockinfoident));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
