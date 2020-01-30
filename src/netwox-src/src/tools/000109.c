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
netwib_conststring t000109_description[] = {
  "This tool downloads newsgroup messages.",
  "",
  NETWOX_DESC_newsgroup,
  "The messages to download are between --firstartnum and --lastartnum.",
  NETWOX_DESC_userpass,
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000109_args[] = {
  NETWOX_SOCK_ARG_TCP_CLIPORT("119"),
  NETWOX_TOOLARG_REQ_BUF_NEWSGROUP('g', "newsgroup", NULL, NULL),
  NETWOX_TOOLARG_REQ_UINT32('n', "firstartnum", "first article number", "1"),
  NETWOX_TOOLARG_OPT_UINT32('N', "lastartnum", "last article number",
                            "4294967295"),
  NETWOX_TOOLARG_OPT_BUF_LOGIN('u', "user", "username", NULL),
  NETWOX_TOOLARG_OPT_BUF_PASSWORD('a', "pass", "password", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in ms", "60000"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000109_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_NNTP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000109_info = {
  "Download one, or more, newsgroup messages",
  t000109_description,
  "article",
  t000109_args,
  t000109_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000109_download(netwox_nntpses *pnntpses,
                                   netwib_uint32 artnum,
                                   netwib_constbuf *pfilename)
{
  netwib_buf *pbuf;
  netwib_bool event;
  netwib_data data;
  netwib_time t;
  netwib_uint32 datasize;
  netwib_io *pfileio;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwox_nntpses_article(pnntpses, artnum));

  netwib_er(netwib_bufpool_buf_init(pnntpses->pbufpool, &pbuf));
  netwib_er(netwib_io_init_file_write(pfilename, &pfileio));
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
    netwib_er(netwib_buf_append_string("\r\n", pbuf));
    netwib_er(netwib_io_write(pfileio, pbuf));
  }
  netwib_er(netwib_io_close(&pfileio));
  netwib_er(netwib_bufpool_buf_close(pnntpses->pbufpool, &pbuf));

  return(ret);
}


/*-------------------------------------------------------------*/
static netwib_err t000109_down(netwox_nntpses *pnntpses,
                               netwib_constbuf *pnewsgroup,
                               netwib_uint32 firstartnum,
                               netwib_uint32 lastartnum)
{
  netwib_buf filename;
  netwib_uint32 artnum;
  netwib_err ret;

  /* download messages */
  netwib_er(netwib_buf_init_mallocdefault(&filename));
  ret = NETWIB_ERR_OK;
  for (artnum = firstartnum; artnum <= lastartnum; artnum++) {
    netwib__buf_reinit(&filename);
    netwib_er(netwib_buf_append_fmt(&filename, "%{buf}_%{uint32}.txt",
                                    pnewsgroup, artnum));
    ret = t000109_download(pnntpses, artnum, &filename);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_NOTFOUND) ret = NETWIB_ERR_OK;
      break;
    }
  }
  netwib_er(netwib_buf_close(&filename));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000109_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_buf newsgroup;
  netwib_uint32 firstartnum, lastartnum;
  netwib_io *pioserver;
  netwox_nntpses nntpses;
  netwib_buf user, password;
  netwib_uint32 maxwaitms;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000109_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'g', &newsgroup));
  netwib_er(netwox_arg_uint32(parg, 'n', &firstartnum));
  netwib_er(netwox_arg_uint32(parg, 'N', &lastartnum));
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
  netwib_er(t000109_down(&nntpses, &newsgroup, firstartnum, lastartnum));
  netwib_er(netwox_nntpses_quit(&nntpses));
  netwib_er(netwox_nntpses_close(&nntpses));

  /* close */
  netwib_er(netwib_io_close(&pioserver));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
