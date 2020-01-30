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
netwib_conststring t000178_description[] = {
  "This tool connects on an IRC server, then quit.",
  NETWOX_DESC_check_work,
  "",
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000178_args[] = {
  NETWOX_SOCK_ARG_TCP_CLIPORT("6667"),
  NETWOX_TOOLARG_REQ_BUF_LOGIN('n', "nickname", "nickname", NULL),
  NETWOX_TOOLARG_OPT_BUF_PASSWORD('w', "password", NULL, NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000178_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_IRC,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000178_info = {
  "Check if an IRC server is up",
  t000178_description,
  NULL,
  t000178_args,
  t000178_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000178_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwox_ircses ircses;
  netwib_buf password, nickname;
  netwib_io *pio;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000178_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_tcp_cli(parg, &sockinfo));
  netwib_er(netwox_arg_buf(parg, 'w', &password));
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwox_arg_buf(parg, 'n', &nickname));
  nickname.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  nickname.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;

  /* init session */
  netwib_er(netwox_sock_init(&sockinfo, &pio));
  netwib_er(netwox_ircses_init(pio, &password, &nickname, &nickname, &nickname,
                               &ircses));

  /* main loop */
  netwib_er(netwox_ircses_waitready(&ircses));
  netwib_er(netwox_ircses_write_quit(&ircses, NULL));

  /* close */
  netwib_er(netwox_ircses_close(&ircses));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
