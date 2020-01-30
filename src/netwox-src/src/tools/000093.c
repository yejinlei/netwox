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
netwib_conststring t000093_description[] = {
  "This tool permits to remotely administer computer. Tools 94, 95 and 96",
  "will run a command, get a file and store a file on this server.",
  "",
  "Parameter --password defines password to use for connection.",
  NETWOX_DESC_allowedclients,
  NETWOX_DESC_rootdir,
  "Setting --no-allow-exec, --no-allow-get or --no-allow-put restricts",
  "allowed operations.",
  NETWOX_DESC_servertcpmul,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000093_args[] = {
  NETWOX_SOCK_ARG_TCP_MULSER,
  NETWOX_TOOLARG_OPT_BUF_PASSWORD('w', "password", "password", "netwox"),
  NETWOX_TOOLARG_OPTA_IPS_SU('c', "allowed-clients",
                             "clients allowed to connect", NULL),
  NETWOX_TOOLARG_OPTA_BUF_DIR_RD('r', "rootdir", "root directory", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in seconds", "60"),
  NETWOX_TOOLARG_OPTA_BOOL('X', "allow-exec", "allow exec", "1"),
  NETWOX_TOOLARG_OPTA_BOOL('G', "allow-get", "allow get", "1"),
  NETWOX_TOOLARG_OPTA_BOOL('U', "allow-put", "allow put", "1"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000093_nodes[] = {
  NETWOX_TOOLTREENODETYPE_REMADM,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000093_info = {
  "TCP remote administration server",
  t000093_description,
  NULL,
  t000093_args,
  t000093_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000093_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwox_remadmtcp_ser_para remadmtcp;
  netwib_ips *pips;
  netwib_bool isset;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000093_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_tcp_mulser(parg, &sockinfo));
  netwib_er(netwox_arg_buf(parg, 'w', &remadmtcp.password));
  remadmtcp.password.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  remadmtcp.password.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwox_arg_isset(parg, 'r', &isset));
  if (isset) {
    netwib_er(netwox_arg_buf(parg, 'r', &remadmtcp.rootdir));
  } else {
    netwib_er(netwib_buf_init_ext_string(".", &remadmtcp.rootdir));
  }
  netwib_er(netwox_arg_ips(parg, 'c', &pips));
  netwib_er(netwox_arg_uint32(parg, 'T', &remadmtcp.timeoutsec));
  netwib_er(netwox_arg_bool(parg, 'X', &remadmtcp.allowexec));
  netwib_er(netwox_arg_bool(parg, 'G', &remadmtcp.allowget));
  netwib_er(netwox_arg_bool(parg, 'U', &remadmtcp.allowput));

  /* reconvert ip list to string (easier for multithread access) */
  netwib_er(netwib_buf_init_mallocdefault(&remadmtcp.allowedclients));
  netwib_er(netwib_buf_append_ips(pips, NETWIB_IPS_ENCODETYPE_BEST,
                                  &remadmtcp.allowedclients));

  /* main loop */
  netwib_er(netwox_sock_tcp_mulser(&sockinfo, &netwox_remadmtcp_ser_loop,
                                   &remadmtcp));

  /* close */
  netwib_er(netwib_buf_close(&remadmtcp.allowedclients));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));
  return(NETWIB_ERR_OK);
}
