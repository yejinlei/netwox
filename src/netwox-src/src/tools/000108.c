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
netwib_conststring t000108_description[] = {
  "This tool lists newsgroups available on a NNTP server.",
  "",
  NETWOX_DESC_userpass,
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000108_args[] = {
  NETWOX_SOCK_ARG_TCP_CLIPORT("119"),
  NETWOX_TOOLARG_OPT_BUF_LOGIN('u', "user", "username", NULL),
  NETWOX_TOOLARG_OPT_BUF_PASSWORD('a', "pass", "password", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in ms", "60000"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000108_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_NNTP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000108_info = {
  "List newsgroups available on a server",
  t000108_description,
  NULL,
  t000108_args,
  t000108_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000108_list(netwox_nntpses *pnntpses)
{
  netwib_buf *pbuf, *pnewsgroup, *pcanpost;
  netwib_bool event;
  netwib_data data;
  netwib_time t;
  netwib_uint32 firstart, lastart;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_bufpool_buf_init(pnntpses->pbufpool, &pbuf));
  netwib_er(netwib_bufpool_buf_init(pnntpses->pbufpool, &pnewsgroup));
  netwib_er(netwib_bufpool_buf_init(pnntpses->pbufpool, &pcanpost));
  while(NETWIB_TRUE) {
    /* read */
    netwib__buf_reinit(pbuf);
    netwib_er(netwib_time_init_now(&t));
    netwib_er(netwib_time_plus_msec(&t, pnntpses->maxwaitms));
    netwib_er(netwib_io_wait_read(pnntpses->pio, &t, &event));
    if (!event) {
      break;
    }
    ret = netwib_io_read(pnntpses->pio, pbuf);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    if (netwib__buf_ref_data_size(pbuf) == 1) {
      data = netwib__buf_ref_data_ptr(pbuf);
      if (data[0] == '.') {
        break;
      }
    }
    /* display line */
    netwib__buf_reinit(pcanpost);
    netwib__buf_reinit(pnewsgroup);
    ret = netwib_buf_decode_fmt(pbuf, "%{buf} %{uint32:08} %{uint32:08} %{buf}", pnewsgroup, &lastart, &firstart, pcanpost);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_fmt_display("%{buf} first=%{uint32} last=%{uint32} canpost=%{buf}\n", pnewsgroup, firstart, lastart, pcanpost));
    } else {
      netwib_er(netwib_fmt_display("%{buf}\n", pbuf));
    }
  }
  netwib_er(netwib_bufpool_buf_close(pnntpses->pbufpool, &pcanpost));
  netwib_er(netwib_bufpool_buf_close(pnntpses->pbufpool, &pnewsgroup));
  netwib_er(netwib_bufpool_buf_close(pnntpses->pbufpool, &pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err t000108_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_io *pioserver;
  netwib_buf user, password;
  netwox_nntpses nntpses;
  netwib_uint32 maxwaitms;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000108_info, &parg));

  /* connect to server */
  netwib_er(netwox_sockinfo_init_arg_tcp_cli(parg, &sockinfo));
  netwib_er(netwox_sock_init(&sockinfo, &pioserver));
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
  netwib_er(netwox_nntpses_list(&nntpses));
  netwib_er(t000108_list(&nntpses));
  netwib_er(netwox_nntpses_quit(&nntpses));
  netwib_er(netwox_nntpses_close(&nntpses));

  /* close */
  netwib_er(netwib_io_close(&pioserver));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
