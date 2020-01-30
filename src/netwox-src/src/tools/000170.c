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
netwib_conststring t000170_description[] = {
  "This tool is a TELNET server.",
  "It only permits to run simple text commands.",
  "Editors are not correctly supported.",
  "",
  NETWOX_DESC_loginpassword,
  NETWOX_DESC_servertcpmul,
  NETWOX_DESC_toolpriv_port1024,
  NULL
};
netwox_toolarg t000170_args[] = {
  NETWOX_SOCK_ARG_TCP_MULSERPORT("23"),
  NETWOX_TOOLARG_OPT_BUF_LOGIN('l', "login", "login needed to access", NULL),
  NETWOX_TOOLARG_OPT_BUF_PASSWORD('w', "password", "password needed to access",
                                  NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in ms", "180000"),
  NETWOX_TOOLARG_OPTA_IPS_SU('c', "allowed-clients",
                             "clients allowed to connect", NULL),
  NETWOX_TOOLARG_OPTA_BOOL('L', "line-by-line", "line by line mode", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000170_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SERVER_TCP_TELNET,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000170_info = {
  "TELNET server",
  t000170_description,
  NULL,
  t000170_args,
  t000170_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000170_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_telnetserver telnetserver;
  netwox_sockinfo mainsockinfo;
  netwib_buf buf;
  netwib_bool linebyline;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000170_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_tcp_mulser(parg, &mainsockinfo));
  netwib_er(netwox_telnetserver_init(&telnetserver));
  netwib_er(netwox_arg_buf(parg, 'l', &buf));
  buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwib_buf_append_buf(&buf, &telnetserver.login));
  netwib_er(netwox_arg_buf(parg, 'w', &buf));
  buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwib_buf_append_buf(&buf, &telnetserver.password));
  netwib_er(netwox_arg_uint32(parg, 'T', &telnetserver.timeoutms));
  netwib_er(netwox_arg_ips(parg, 'c', &telnetserver.pallowedclients));
  netwib_er(netwox_arg_bool(parg, 'L', &linebyline));
  telnetserver.negomode = NETWOX_TELNET_NEGOMODE_CHARACTER;
  if (linebyline) {
    telnetserver.negomode = NETWOX_TELNET_NEGOMODE_LINE;
  }

  /* main loop */
  netwib_er(netwox_sock_tcp_mulser(&mainsockinfo, &netwox_telnetserver_session,
                                   &telnetserver));

  /* close */
  netwib_er(netwox_arg_close(&parg));
  netwib_er(netwox_telnetserver_close(&telnetserver));
  netwib_er(netwox_sockinfo_close(&mainsockinfo));

  return(NETWIB_ERR_OK);
}
