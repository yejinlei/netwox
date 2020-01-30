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
netwib_conststring t000153_description[] = {
  "This tool obtains the list of channels provided by an IRC server.",
  "",
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000153_args[] = {
  NETWOX_SOCK_ARG_TCP_CLIPORT("6667"),
  NETWOX_TOOLARG_REQ_BUF_LOGIN('n', "nickname", "nickname", NULL),
  NETWOX_TOOLARG_OPT_BUF_PASSWORD('w', "password", NULL, NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000153_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_IRC,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000153_info = {
  "IRC client listing channels",
  t000153_description,
  NULL,
  t000153_args,
  t000153_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000153_loop(netwox_ircses *pircses,
                               netwib_ptr infos,
                               netwib_constbuf *pline,
                               netwib_constbuf *pprefix,
                               netwox_irc_cmd cmd,
                               netwib_constbuf *pparameters,
                               netwib_bool *pstoploop)
{
  netwib_buf buf;
  netwib_data data, pc;
  netwib_uint32 datasize;

  if (cmd == NETWOX_IRC_CMD_LISTEND) {
    *pstoploop = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  if (cmd != NETWOX_IRC_CMD_LIST) {
    return(NETWIB_ERR_OK);
  }

  buf = *pparameters;
  datasize = netwib__buf_ref_data_size(&buf);
  if (datasize == 0) {
    return(NETWIB_ERR_OK);
  }
  data = netwib__buf_ref_data_ptr(&buf);
  pc = netwib_c_memchr(data, ' ', datasize);
  buf.beginoffset += pc - data + 1;

  netwib_er(netwib_fmt_display("%{buf}\n", &buf));

  pircses = pircses; /* for compiler warning */
  infos = infos; /* for compiler warning */
  pline = pline; /* for compiler warning */
  pprefix = pprefix; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err t000153_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwox_ircses ircses;
  netwib_buf password, nickname;
  netwib_io *pio;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000153_info, &parg));
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
  netwib_er(netwox_ircses_write_list(&ircses));
  netwib_er(netwox_ircses_loop(&t000153_loop, &ircses, NULL));
  netwib_er(netwox_ircses_write_quit(&ircses, NULL));

  /* close */
  netwib_er(netwox_ircses_close(&ircses));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
