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
netwib_conststring t000172_description[] = {
  "This tool lists the range number of articles provided by a newsgroup.",
  "This range is expressed as 'min max'. If max is lesser than min, this",
  "NNTPserver does does have any message for this newsgroup.",
  "",
  NETWOX_DESC_newsgroup,
  NETWOX_DESC_userpass,
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000172_args[] = {
  NETWOX_SOCK_ARG_TCP_CLIPORT("119"),
  NETWOX_TOOLARG_REQ_BUF_NEWSGROUP('g', "newsgroup", NULL, NULL),
  NETWOX_TOOLARG_OPT_BUF_LOGIN('u', "user", "username", NULL),
  NETWOX_TOOLARG_OPT_BUF_PASSWORD('a', "pass", "password", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in ms", "60000"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000172_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_NNTP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000172_info = {
  "List articles range of a newsgroup",
  t000172_description,
  NULL,
  t000172_args,
  t000172_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000172_analyze_answer(netwox_nntpses *pnntpses)
{
  netwib_uint32 firstart, lastart;
  netwib_err ret;

  ret = netwib_buf_decode_fmt(pnntpses->perrmsg,
                              "%{*;uint32:08} %{uint32:08} %{uint32:08} ",
                              &firstart, &lastart);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_fmt_display("%{uint32} %{uint32}\n", firstart,
                                 lastart));
  } else {
    netwib_er(netwib_fmt_display("%{buf}\n", pnntpses->perrmsg));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err t000172_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_io *pioserver;
  netwib_buf user, password, newsgroup;
  netwox_nntpses nntpses;
  netwib_uint32 maxwaitms;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000172_info, &parg));

  /* connect to server */
  netwib_er(netwox_sockinfo_init_arg_tcp_cli(parg, &sockinfo));
  netwib_er(netwox_sock_init(&sockinfo, &pioserver));
  netwib_er(netwox_arg_buf(parg, 'g', &newsgroup));
  netwib_er(netwox_arg_buf(parg, 'u', &user));
  user.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  user.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwox_arg_buf(parg, 'a', &password));
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwox_arg_uint32(parg, 'T', &maxwaitms));

  /* list groups */
  netwib_er(netwox_nntpses_init(pioserver, &user, &password, maxwaitms, &nntpses));
  netwib_er(netwox_nntpses_banner(&nntpses));
  netwib_er(netwox_nntpses_modereader(&nntpses));
  netwib_er(netwox_nntpses_group(&nntpses, &newsgroup));
  netwib_er(t000172_analyze_answer(&nntpses));
  netwib_er(netwox_nntpses_quit(&nntpses));
  netwib_er(netwox_nntpses_close(&nntpses));

  /* close */
  netwib_er(netwib_io_close(&pioserver));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
