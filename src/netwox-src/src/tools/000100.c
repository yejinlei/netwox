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
netwib_conststring t000100_description[] = {
  "This tool runs shell commands on a remote TELNET server.",
  "First, it logins, then it runs commands, and finally, it logouts.",
  "For example: netwox 23 -l bob --kbd-w id hostname pwd ls"
  "",
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000100_args[] = {
  NETWOX_SOCK_ARG_TCP_CLIPORT("23"),
  NETWOX_TOOLARG_REQ_BUF_LOGIN('l', "login", "login", NULL),
  NETWOX_TOOLARG_REQ_BUF_PASSWORD('w', "password", "password", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in milliseconds",
                             "60000"),
  NETWOX_TOOLARG_MORE("cmd1 cmd2 ...", "command list"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000100_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_TELNET,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000100_info = {
  "TELNET client executing one or several commands",
  t000100_description,
  "rsh",
  t000100_args,
  t000100_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000100_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_io *ptcpio, *ptelnetio;
  netwib_uint32 maxwaitms, count, i;
  netwib_time abstime;
  netwib_buf login, password, cmd, outbuf;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000100_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_tcp_cli(parg, &sockinfo));
  netwib_er(netwox_arg_uint32(parg, 'T', &maxwaitms));
  netwib_er(netwox_arg_buf(parg, 'l', &login));
  login.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  login.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwox_arg_buf(parg, 'w', &password));
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwox_sock_init(&sockinfo, &ptcpio));

  /* create a telnet io, and plug it on top of tcpio */
  netwib_er(netwox_telnet_init(NETWIB_TRUE, &ptelnetio));
  netwib_er(netwib_io_plug_rdwr(ptelnetio, ptcpio));
  netwib_er(netwox_telnet_negotiate(ptelnetio,
                                    NETWOX_TELNET_NEGOMODE_CHARACTER));

  /* login */
  netwib_er(netwib_time_init_now(&abstime));
  netwib_er(netwib_time_plus_msec(&abstime, maxwaitms));
  netwib_er(netwox_telnetses_login(ptelnetio, &abstime, &login, &password));

  /* execute each command */
  netwib_er(netwox_arg_more_count(parg, &count));
  netwib_er(netwib_buf_init_mallocdefault(&cmd));
  netwib_er(netwib_buf_init_mallocdefault(&outbuf));
  for (i = 0; i < count; i++) {
    netwib__buf_reinit(&cmd);
    netwib_er(netwox_arg_more(parg, i, &cmd));
    netwib__buf_reinit(&outbuf);
    netwib_er(netwib_time_init_now(&abstime));
    netwib_er(netwib_time_plus_msec(&abstime, maxwaitms));
    netwib_er(netwox_telnetses_exec(ptelnetio, &abstime, &cmd, &outbuf));
    netwib_er(netwib_buf_display(&outbuf, NETWIB_ENCODETYPE_DATA));
  }
  netwib_er(netwib_buf_close(&outbuf));
  netwib_er(netwib_buf_close(&cmd));

  /* logout */
  netwib_er(netwox_telnetses_logout(ptelnetio));

  /* close */
  netwib_er(netwib_io_close(&ptelnetio));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
