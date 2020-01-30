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
netwib_conststring t000154_description[] = {
  "This tool passively listen on an IRC channel, and displays its",
  "content. Ask channel operator before running this program.",
  "",
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000154_args[] = {
  NETWOX_SOCK_ARG_TCP_CLIPORT("6667"),
  NETWOX_TOOLARG_REQ_BUF_LOGIN('n', "nickname", "nickname", NULL),
  NETWOX_TOOLARG_REQ_BUF('c', "channel", "channel", "#chan"),
  NETWOX_TOOLARG_OPT_BUF_PASSWORD('w', "password", NULL, NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000154_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_IRC,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000154_info = {
  "IRC client listening on a channel",
  t000154_description,
  NULL,
  t000154_args,
  t000154_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000154_loop(netwox_ircses *pircses,
                               netwib_ptr infos,
                               netwib_constbuf *pline,
                               netwib_constbuf *pprefix,
                               netwox_irc_cmd cmd,
                               netwib_constbuf *pparameters,
                               netwib_bool *pstoploop)
{
  netwib_string pskick;
  netwib_data data;
  netwib_uint32 datasize;


  netwib_er(netwib_fmt_display("%{buf}\n", pline));

  if (cmd == NETWOX_IRC_CMD_KICK) {
    pskick = (netwib_string)infos;
    datasize = netwib__buf_ref_data_size(pparameters);
    if (datasize >= netwib_c_strlen(pskick)) {
      data = netwib__buf_ref_data_ptr(pparameters);
      if (!netwib_c_memcmp(data, (netwib_constdata)pskick,
                           netwib_c_strlen(pskick))) {
        *pstoploop = NETWIB_TRUE;
      }
    }
  }

  pircses = pircses; /* for compiler warning */
  pprefix = pprefix; /* for compiler warning */
  cmd = cmd; /* for compiler warning */
  pstoploop = pstoploop; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err t000154_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwox_ircses ircses;
  netwib_buf password, nickname, channel, kick;
  netwib_string pskick;
  netwib_io *pio;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000154_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_tcp_cli(parg, &sockinfo));
  netwib_er(netwox_arg_buf(parg, 'w', &password));
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwox_arg_buf(parg, 'n', &nickname));
  nickname.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  nickname.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwox_arg_buf(parg, 'c', &channel));

  /* init session */
  netwib_er(netwox_sock_init(&sockinfo, &pio));
  netwib_er(netwox_ircses_init(pio, &password, &nickname, &nickname, &nickname,
                               &ircses));

  netwib_er(netwib_buf_init_mallocdefault(&kick));
  netwib_er(netwib_buf_append_fmt(&kick, "%{buf} %{buf} ", &channel,
                                  &nickname));
  netwib_er(netwib_buf_ref_string(&kick, &pskick));

  /* main loop */
  netwib_er(netwox_ircses_waitready(&ircses));
  netwib_er(netwox_ircses_write_join(&ircses, &channel));
  netwib_er(netwox_ircses_loop(&t000154_loop, &ircses, pskick));
  netwib_er(netwox_ircses_write_quit(&ircses, NULL));

  /* close */
  netwib_er(netwib_buf_close(&kick));
  netwib_er(netwox_ircses_close(&ircses));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
