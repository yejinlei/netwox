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
netwib_conststring t000114_description[] = {
  "This tool removes a file of a FTP server.",
  "",
  NETWOX_DESC_userpass,
  NETWOX_DESC_ftp_passive,
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000114_args[] = {
  NETWOX_SOCK_ARG_TCP_CLIPORT("21"),
  NETWOX_TOOLARG_OPT_BUF_LOGIN('u', "user", "username", "anonymous"),
  NETWOX_TOOLARG_OPT_BUF_PASSWORD('a', "pass", "password", "user@"),
  NETWOX_TOOLARG_OPT_BOOL('V', "passive", "passive", NULL),
  NETWOX_TOOLARG_REQ_BUF_FILE('F', "remote-file", "remote file to delete",
                              NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in milliseconds",
                             "60000"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000114_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_FTP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000114_info = {
  "FTP client : del a file",
  t000114_description,
  NULL,
  t000114_args,
  t000114_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000114_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_buf user, password, remotefile;
  netwib_bool passive;
  netwox_ftpclient ftpclient;
  netwib_uint32 maxwaitms;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000114_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_tcp_cli(parg, &sockinfo));
  netwib_er(netwox_arg_buf(parg, 'u', &user));
  user.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  user.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwox_arg_buf(parg, 'a', &password));
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwox_arg_bool(parg, 'V', &passive));
  netwib_er(netwox_arg_buf(parg, 'F', &remotefile));
  netwib_er(netwox_arg_uint32(parg, 'T', &maxwaitms));

  /* FTP session */
  netwib_er(netwox_ftpclient_init(&sockinfo, maxwaitms, &ftpclient));
  netwib_er(netwox_ftpclient_login(&ftpclient, &user, &password));
  if (passive) {
    netwib_er(netwox_ftpclient_passive(&ftpclient));
  } else {
    netwib_er(netwox_ftpclient_active(&ftpclient));
  }
  netwib_er(netwox_ftpclient_file_delete(&ftpclient, &remotefile));
  netwib_er(netwox_ftpclient_close(&ftpclient));

  /* close */
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
