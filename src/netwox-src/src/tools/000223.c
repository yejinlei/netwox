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
netwib_conststring t000223_description[] = {
  "This tool forwards an email. It connects on a SMTP server.",
  "",
  NETWOX_DESC_email,
  "Parameter --to sets To header.",
  "Parameter --mail-from sets the MAIL FROM SMTP value. If unset, --from",
  "is used.",
  "Parameter --rcpt-to sets the RCPR TO SMTP value. If unset, --to is",
  "used.",
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000223_args[] = {
  NETWOX_SOCK_ARG_TCP_CLIPORT("25"),
  NETWOX_TOOLARG_OPTA_BUF_EMAIL('F', "mail-from", "mail from address",
                                "me@example.com"),
  NETWOX_TOOLARG_OPTA_BUF_EMAIL('R', "rcpt-to", "rcpt to address",
                                "you@example.fr"),
  NETWOX_TOOLARG_REQ_BUF_EMAIL('f', "from", "from address",
                               "user@example.com"),
  NETWOX_TOOLARG_OPTA_BUF('n', "from-name", "from name", NULL),
  NETWOX_TOOLARG_REQ_BUF_EMAIL('r', "to", "to address", "user2@example.fr"),
  NETWOX_TOOLARG_REQ_BUF('S', "subject", "email subject", "hello"),
  NETWOX_TOOLARG_OPT_BUF_FILE_RD('B', "file-body",
                                 "file containing email body",
                                 "file-body.txt"),
  NETWOX_TOOLARG_REQ_BUF_FILE_RD('A', "file-fwd",
                                 "file containing email to forward",
                                 "file-fwd.eml"),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in milliseconds",
                             "180000"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000223_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_SMTP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000223_info = {
  "Forward an email",
  t000223_description,
  NULL,
  t000223_args,
  t000223_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000223_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_bufpool *pbufpool;
  netwib_buf mailfrom, rcptto, from, fromname, to, subject, filebody, filefwd;
  netwib_buf email;
  netwib_io *piobody, *piofwd, *pioserver, *pioline;
  netwib_bool isset;
  netwib_uint32 maxwaitms;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000223_info, &parg));
  netwib_er(netwox_arg_uint32(parg, 'T', &maxwaitms));

  /* obtain email addresses */
  netwib_er(netwox_arg_buf(parg, 'f', &from));
  netwib_er(netwox_arg_isset(parg, 'F', &isset));
  if (isset) {
    netwib_er(netwox_arg_buf(parg, 'F', &mailfrom));
  } else {
    netwib_er(netwib_buf_init_ext_buf(&from, &mailfrom));
  }
  netwib_er(netwox_arg_buf(parg, 'r', &to));
  netwib_er(netwox_arg_isset(parg, 'R', &isset));
  if (isset) {
    netwib_er(netwox_arg_buf(parg, 'R', &rcptto));
  } else {
    netwib_er(netwib_buf_init_ext_buf(&to, &rcptto));
  }
  netwib_er(netwox_arg_buf(parg, 'n', &fromname));

  /* obtain subject */
  netwib_er(netwox_arg_buf(parg, 'S', &subject));

  /* open filenames */
  piobody = NULL;
  netwib_er(netwox_arg_isset(parg, 'B', &isset));
  if (isset) {
    netwib_er(netwox_arg_buf(parg, 'B', &filebody));
    netwib_er(netwib_io_init_file_read(&filebody, &piobody));
  }
  piofwd = NULL;
  netwib_er(netwox_arg_buf(parg, 'A', &filefwd));
  netwib_er(netwib_io_init_file_read(&filefwd, &piofwd));

  /* connect to server */
  netwib_er(netwox_sockinfo_init_arg_tcp_cli(parg, &sockinfo));
  netwib_er(netwox_sock_init(&sockinfo, &pioserver));

  /* construct an email (with or without attachment) */
  netwib_er(netwib_buf_init_mallocdefault(&email));
  if (piobody != NULL) {
    netwib_er(netwox_mailex_subbodyfwd(&from, &fromname, NETWIB_FALSE, &to,
                                       &subject, piobody, &filefwd, piofwd,
                                       &email));
  } else {
    netwib_er(netwox_mailex_subfwd(&from, &fromname, NETWIB_FALSE, &to,
                                   &subject, &filefwd, piofwd, &email));
  }

  /* send message */
  netwib_er(netwox_txtproto_io_rd(pioserver, &pioline));
  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  netwib_er(netwox_smtp_send(pbufpool, pioline, maxwaitms, &mailfrom,
                             &rcptto, &email));
  netwib_er(netwib_bufpool_close(&pbufpool));
  netwib_er(netwib_buf_close(&email));

  /* close */
  netwib_er(netwib_io_close(&pioline));
  if (piobody != NULL) {
    netwib_er(netwib_io_close(&piobody));
  }
  netwib_er(netwib_io_close(&piofwd));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
