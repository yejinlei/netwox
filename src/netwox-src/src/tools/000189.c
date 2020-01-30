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
netwib_conststring t000189_description[] = {
  "This tool is a SMTP server. It only stores commands and messages",
  "in a local directory. It does not try to forward messages.",
  "",
  NETWOX_DESC_allowedclients,
  NETWOX_DESC_servertcpmul,
  NETWOX_DESC_toolpriv_port1024,
  NULL
};
netwox_toolarg t000189_args[] = {
  NETWOX_SOCK_ARG_TCP_MULSERPORT("25"),
  NETWOX_TOOLARG_REQ_BUF_DIR_RD('m', "maildir", "mail directory", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in ms", "180000"),
  NETWOX_TOOLARG_OPTA_IPS_SU('c', "allowed-clients",
                             "clients allowed to connect", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000189_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SERVER_TCP_SMTP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000189_info = {
  "SMTP server",
  t000189_description,
  "sendmail, qmail, postfix",
  t000189_args,
  t000189_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000189_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_smtpserver smtpserver;
  netwox_sockinfo sockinfo;
  netwib_buf buf;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000189_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_tcp_mulser(parg, &sockinfo));
  netwib_er(netwox_smtpserver_init(&smtpserver));
  netwib_er(netwox_arg_buf(parg, 'm', &buf));
  netwib_er(netwib_path_canon(&buf, &smtpserver.maildir));
  netwib_er(netwox_arg_uint32(parg, 'T', &smtpserver.timeoutms));
  netwib_er(netwox_arg_ips(parg, 'c', &smtpserver.pallowedclients));

  /* main loop */
  netwib_er(netwox_sock_tcp_mulser(&sockinfo, &netwox_smtpserver_session,
                                   &smtpserver));

  /* close */
  netwib_er(netwox_smtpserver_close(&smtpserver));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
