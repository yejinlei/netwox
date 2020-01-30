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
netwib_conststring t000195_description[] = {
  "This tool is an IDENT server.",
  "If --username is set, it always reply 'USERID : UNIX :user'.",
  "If --username is unset, it always reply 'ERROR : HIDDEN-USER'.",
  "",
  NETWOX_DESC_allowedclients,
  NETWOX_DESC_servertcpmul,
  NETWOX_DESC_toolpriv_port1024,
  NULL
};
netwox_toolarg t000195_args[] = {
  NETWOX_SOCK_ARG_TCP_MULSERPORT("113"),
  NETWOX_TOOLARG_OPT_BUF_LOGIN('u', "username", "username to reply", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in ms", "60000"),
  NETWOX_TOOLARG_OPTA_IPS_SU('c', "allowed-clients",
                             "clients allowed to connect", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000195_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SERVER_TCP_IDENT,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000195_info = {
  "IDENT server",
  t000195_description,
  NULL,
  t000195_args,
  t000195_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000195_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_identserver identserver;
  netwox_sockinfo sockinfo;
  netwib_buf buf;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000195_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_tcp_mulser(parg, &sockinfo));
  netwib_er(netwox_identserver_init(&identserver));
  netwib_er(netwox_arg_buf(parg, 'u', &buf));
  netwib_er(netwib_buf_append_buf(&buf, &identserver.username));
  netwib_er(netwox_arg_uint32(parg, 'T', &identserver.timeoutms));
  netwib_er(netwox_arg_ips(parg, 'c', &identserver.pallowedclients));

  /* main loop */
  netwib_er(netwox_sock_tcp_mulser(&sockinfo, &netwox_identserver_session,
                                   &identserver));

  /* close */
  netwib_er(netwox_identserver_close(&identserver));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
