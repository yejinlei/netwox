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
netwib_conststring t000168_description[] = {
  "This tool is a FTP server.",
  "",
  NETWOX_DESC_rootdir,
  NETWOX_DESC_loginpassword,
  NETWOX_DESC_allowedclients,
  NETWOX_DESC_servertcpmul,
  NETWOX_DESC_toolpriv_port1024,
  NULL
};
netwox_toolarg t000168_args[] = {
  NETWOX_SOCK_ARG_TCP_MULSERPORT("21"),
  NETWOX_TOOLARG_OPT_BUF_DIR_RD('r', "rootdir", "root directory", NULL),
  NETWOX_TOOLARG_OPT_BUF_LOGIN('l', "login", "login needed to access", NULL),
  NETWOX_TOOLARG_OPT_BUF_PASSWORD('L', "password", "password needed to access",
                                  NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in ms", "180000"),
  NETWOX_TOOLARG_OPTA_IPS_SU('c', "allowed-clients",
                             "clients allowed to connect", NULL),
  NETWOX_TOOLARG_OPTA_BOOL('U', "allow-put", "allow put/write on server", "1"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000168_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SERVER_TCP_FTP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000168_info = {
  "FTP server",
  t000168_description,
  "proftpd, wuftpd",
  t000168_args,
  t000168_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000168_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_ftpserver ftpserver;
  netwox_sockinfo mainsockinfo;
  netwib_buf buf;
  netwib_bool isset;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000168_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_tcp_mulser(parg, &mainsockinfo));
  netwib_er(netwox_ftpserver_init(&ftpserver));
  ftpserver.pmainsockinfo = &mainsockinfo;
  netwib_er(netwox_arg_isset(parg, 'r', &isset));
  if (isset) {
    netwib__buf_reinit(&ftpserver.rootdir);
    netwib_er(netwox_arg_buf(parg, 'r', &buf));
    netwib_er(netwib_path_canon(&buf, &ftpserver.rootdir));
  }
  netwib_er(netwox_arg_buf(parg, 'l', &buf));
  buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwib_buf_append_buf(&buf, &ftpserver.login));
  netwib_er(netwox_arg_buf(parg, 'L', &buf));
  buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwib_buf_append_buf(&buf, &ftpserver.password));
  netwib_er(netwox_arg_uint32(parg, 'T', &ftpserver.timeoutms));
  netwib_er(netwox_arg_ips(parg, 'c', &ftpserver.pallowedclients));
  netwib_er(netwox_arg_bool(parg, 'U', &ftpserver.allowput));

  /* main loop */
  netwib_er(netwox_sock_tcp_mulser(&mainsockinfo, &netwox_ftpserver_session,
                                   &ftpserver));

  /* close */
  netwib_er(netwox_ftpserver_close(&ftpserver));
  netwib_er(netwox_sockinfo_close(&mainsockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
