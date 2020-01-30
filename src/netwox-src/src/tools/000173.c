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
netwib_conststring t000173_description[] = {
  "This tool downloads the overview of messages in a newsgroup.",
  "It uses a NNTP extension named XOVER.",
  "The overview contains article number, subject, author, date,",
  "message-id, references, byte count, and line count.",
  "",
  NETWOX_DESC_newsgroup,
  NETWOX_DESC_userpass,
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000173_args[] = {
  NETWOX_SOCK_ARG_TCP_CLIPORT("119"),
  NETWOX_TOOLARG_REQ_BUF_NEWSGROUP('g', "newsgroup", NULL, NULL),
  NETWOX_TOOLARG_REQ_UINT32('n', "firstartnum", "first article number", "1"),
  NETWOX_TOOLARG_OPT_UINT32('N', "lastartnum", "last article number", "1"),
  NETWOX_TOOLARG_OPT_BUF_LOGIN('u', "user", "username", NULL),
  NETWOX_TOOLARG_OPT_BUF_PASSWORD('a', "pass", "password", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in ms", "60000"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000173_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_NNTP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000173_info = {
  "Download overview of one, or more, newsgroup messages",
  t000173_description,
  NULL,
  t000173_args,
  t000173_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000173_xover(netwox_nntpses *pnntpses,
                                netwib_uint32 firstartnum,
                                netwib_uint32 lastartnum)
{
  netwib_buf *pbuf;
  netwib_bool event;
  netwib_data data;
  netwib_time t;
  netwib_uint32 datasize;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwox_nntpses_xover(pnntpses, firstartnum, lastartnum));

  netwib_er(netwib_bufpool_buf_init(pnntpses->pbufpool, &pbuf));
  while(NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();
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
    /* check dots */
    datasize = netwib__buf_ref_data_size(pbuf);
    if (datasize > 0) {
      data = netwib__buf_ref_data_ptr(pbuf);
      if (data[0] == '.') {
        if (datasize == 1) {
          /* a dot alone indicates end */
          break;
        } else {
          /* a dot in the message is duplicated */
          pbuf->beginoffset++;
        }
      }
    }
    /* append to file */
    netwib_er(netwib_buf_append_string("\n", pbuf));
    netwib_er(netwib_buf_display(pbuf, NETWIB_ENCODETYPE_DATA));
  }
  netwib_er(netwib_bufpool_buf_close(pnntpses->pbufpool, &pbuf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000173_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_buf newsgroup;
  netwib_uint32 firstartnum, lastartnum;
  netwib_io *pioserver;
  netwox_nntpses nntpses;
  netwib_buf user, password;
  netwib_uint32 maxwaitms;
  netwib_bool isset;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000173_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'g', &newsgroup));
  netwib_er(netwox_arg_uint32(parg, 'n', &firstartnum));
  netwib_er(netwox_arg_isset(parg, 'N', &isset));
  lastartnum = firstartnum;
  if (isset) {
    netwib_er(netwox_arg_uint32(parg, 'N', &lastartnum));
  }
  netwib_er(netwox_arg_buf(parg, 'u', &user));
  user.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  user.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwox_arg_buf(parg, 'a', &password));
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwox_arg_uint32(parg, 'T', &maxwaitms));

  /* connect to server */
  netwib_er(netwox_sockinfo_init_arg_tcp_cli(parg, &sockinfo));
  netwib_er(netwox_sock_init(&sockinfo, &pioserver));

  /* download messages */
  netwib_er(netwox_nntpses_init(pioserver, &user, &password, maxwaitms, &nntpses));
  netwib_er(netwox_nntpses_banner(&nntpses));
  netwib_er(netwox_nntpses_modereader(&nntpses));
  netwib_er(netwox_nntpses_group(&nntpses, &newsgroup));
  netwib_er(t000173_xover(&nntpses, firstartnum, lastartnum));
  netwib_er(netwox_nntpses_quit(&nntpses));
  netwib_er(netwox_nntpses_close(&nntpses));

  /* close */
  netwib_er(netwib_io_close(&pioserver));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
