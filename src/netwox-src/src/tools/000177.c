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
netwib_conststring t000177_description[] = {
  "This tool connects on a SMTP server, then, send a QUIT.",
  NETWOX_DESC_check_work,
  "",
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000177_args[] = {
  NETWOX_SOCK_ARG_TCP_CLIPORT("25"),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in milliseconds",
                             "60000"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000177_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_SMTP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000177_info = {
  "Check if a SMTP server is up",
  t000177_description,
  NULL,
  t000177_args,
  t000177_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000177_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_buf errmsg;
  netwib_io *pio;
  netwib_uint32 maxwaitms, errnum;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000177_info, &parg));
  netwib_er(netwox_arg_uint32(parg, 'T', &maxwaitms));

  /* connect to server */
  netwib_er(netwox_sockinfo_init_arg_tcp_cli(parg, &sockinfo));
  netwib_er(netwox_sock_init(&sockinfo, &pio));

  /* only send a quit */
  netwib_er(netwib_buf_init_mallocdefault(&errmsg));
  netwib_er(netwox_txtproto_read_reply(pio, maxwaitms, &errnum, &errmsg));
  netwib_er(netwox_txtproto_expect1(errnum, &errmsg,
                                    NETWOX_SMTPREPLY_220_SVCREADY));

  netwib_er(netwox_smtp_write_quit(pio));
  netwib__buf_reinit(&errmsg);
  netwib_er(netwox_txtproto_read_reply(pio, maxwaitms, &errnum, &errmsg));
  netwib_er(netwox_txtproto_expect1(errnum, &errmsg,
                                    NETWOX_SMTPREPLY_221_SVCCLOSING));

  /* close */
  netwib_er(netwib_buf_close(&errmsg));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
