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
netwib_conststring t000107_description[] = {
  "This tool posts a newsgroup message. It connects on a NNTP server.",
  "",
  NETWOX_DESC_email,
  NETWOX_DESC_newsgroup,
  NETWOX_DESC_userpass,
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000107_args[] = {
  NETWOX_SOCK_ARG_TCP_CLIPORT("119"),
  NETWOX_TOOLARG_REQ_BUF_EMAIL('f', "from", "user email address", NULL),
  NETWOX_TOOLARG_OPTA_BUF('n', "from-name", "from name", NULL),
  NETWOX_TOOLARG_REQ_BUF_NEWSGROUP('g', "newsgroup", NULL, NULL),
  NETWOX_TOOLARG_REQ_BUF('S', "subject", "email subject", "hello"),
  NETWOX_TOOLARG_OPT_BUF_FILE_RD('B', "file-body",
                                 "file containing email body",
                                 "file-body.txt"),
  NETWOX_TOOLARG_OPTA_BUF_FILE_RD('A', "file-att",
                                  "file containing email attachment",
                                  "file-att.tgz"),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in milliseconds",
                             "60000"),
  NETWOX_TOOLARG_OPT_BUF_LOGIN('u', "user", "username", NULL),
  NETWOX_TOOLARG_OPT_BUF_PASSWORD('a', "pass", "password", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000107_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_NNTP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000107_info = {
  "Post a newsgroup message",
  t000107_description,
  "article",
  t000107_args,
  t000107_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000107_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_buf from, fromname, newsgroup, subject, filebody, fileatt;
  netwib_buf user, password, msg;
  netwib_io *piobody, *pioatt, *pioserver;
  netwib_bool isset;
  netwox_nntpses nntpses;
  netwib_uint32 maxwaitms;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000107_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'u', &user));
  user.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  user.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwox_arg_buf(parg, 'a', &password));
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwox_arg_uint32(parg, 'T', &maxwaitms));

  /* obtain email addresses */
  netwib_er(netwox_arg_buf(parg, 'f', &from));
  netwib_er(netwox_arg_buf(parg, 'n', &fromname));
  netwib_er(netwox_arg_buf(parg, 'g', &newsgroup));

  /* obtain subject */
  netwib_er(netwox_arg_buf(parg, 'S', &subject));

  /* open filenames */
  piobody = NULL;
  netwib_er(netwox_arg_isset(parg, 'B', &isset));
  if (isset) {
    netwib_er(netwox_arg_buf(parg, 'B', &filebody));
    netwib_er(netwib_io_init_file_read(&filebody, &piobody));
  }
  pioatt = NULL;
  netwib_er(netwox_arg_isset(parg, 'A', &isset));
  if (isset) {
    netwib_er(netwox_arg_buf(parg, 'A', &fileatt));
    netwib_er(netwib_io_init_file_read(&fileatt, &pioatt));
  }

  /* connect to server */
  netwib_er(netwox_sockinfo_init_arg_tcp_cli(parg, &sockinfo));
  netwib_er(netwox_sock_init(&sockinfo, &pioserver));

  /* construct an message (with or without attachment) */
  netwib_er(netwib_buf_init_mallocdefault(&msg));
  if (piobody != NULL && pioatt != NULL) {
    netwib_er(netwox_mailex_subbodyatt(&from, &fromname, NETWIB_TRUE,
                                       &newsgroup, &subject, piobody,
                                       &fileatt, pioatt, &msg));
  } else if (piobody != NULL) {
    netwib_er(netwox_mailex_subbody(&from, &fromname, NETWIB_TRUE, &newsgroup,
                                    &subject, piobody, &msg));
  } else if (pioatt != NULL) {
    netwib_er(netwox_mailex_subatt(&from, &fromname, NETWIB_TRUE, &newsgroup,
                                   &subject, &fileatt, pioatt, &msg));
  } else {
    netwib_er(netwox_mailex_sub(&from, &fromname, NETWIB_TRUE, &newsgroup,
                                &subject, &msg));
  }

  /* send message */
  netwib_er(netwox_nntpses_init(pioserver, &user, &password, maxwaitms, &nntpses));
  netwib_er(netwox_nntpses_banner(&nntpses));
  netwib_er(netwox_nntpses_modereader(&nntpses));
  netwib_er(netwox_nntpses_group(&nntpses, &newsgroup));
  netwib_er(netwox_nntpses_post(&nntpses, &msg));
  netwib_er(netwox_nntpses_quit(&nntpses));
  netwib_er(netwox_nntpses_close(&nntpses));

  /* close */
  netwib_er(netwib_buf_close(&msg));
  netwib_er(netwib_io_close(&pioserver));
  if (piobody != NULL) {
    netwib_er(netwib_io_close(&piobody));
  }
  if (pioatt != NULL) {
    netwib_er(netwib_io_close(&pioatt));
  }
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
