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
netwib_conststring t000217_description[] = {
  "This tool is a SMB/CIFS server. Microsoft Windows and Samba clients",
  "can be used to connect to this server, in order to access shared",
  "directory.",
  "",
  "When this tool is running, users can connect to it from Windows using",
  "\\\\1.2.3.4\\share or \\\\server\\share (might need to edit lmhost).",
  "",
  "This tool does not support filenames with accents nor with Unicode",
  "characters. It also has other limitations, but most users will not",
  "notice them (see smbser.h in source files).",
  "",
  NETWOX_DESC_rootdir,
  NETWOX_DESC_loginpassword,
  NETWOX_DESC_allowedclients,
  NETWOX_DESC_servertcpmul,
  NETWOX_DESC_toolpriv_port1024,
  NULL
};
netwox_toolarg t000217_args[] = {
  NETWOX_SOCK_ARG_TCP_MULSERPORT("139"),
  NETWOX_TOOLARG_OPT_BUF_SMBSHARE('s', "share", "share name", "share"),
  NETWOX_TOOLARG_OPT_BUF_DIR_RD('r', "rootdir", "root directory", NULL),
  NETWOX_TOOLARG_OPT_BUF_LOGIN('u', "user", "username needed to access", NULL),
  NETWOX_TOOLARG_OPT_BUF_PASSWORD('w', "password", "password needed to access",
                                  NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in milliseconds",
                             "600000"),
  NETWOX_TOOLARG_OPTA_IPS_SU('c', "allowed-clients",
                             "clients allowed to connect", NULL),
  NETWOX_TOOLARG_OPTA_BOOL('U', "allow-put", "allow put/write on server", "1"),
  NETWOX_TOOLARG_OPTA_BOOL('V', "verbose", "display SMB trace", "0"),
  NETWOX_TOOLARG_OPTA_BOOL('D', "debug", "display SMB packets", "0"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000217_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SERVER_TCP_SMB,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000217_info = {
  "SMB/CIFS server",
  t000217_description,
  "Samba",
  t000217_args,
  t000217_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000217_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_smbserver smbserver;
  netwox_sockinfo sockinfo;
  netwib_buf buf;
  netwib_bool isset;
  netwib_err ret;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000217_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_tcp_mulser(parg, &sockinfo));
  netwib_er(netwox_smbserver_init(&sockinfo, &smbserver));
  netwib_er(netwox_arg_isset(parg, 's', &isset));
  if (isset) {
    netwib__buf_reinit(&smbserver.share);
    netwib_er(netwox_arg_buf(parg, 's', &buf));
    netwib_er(netwib_buf_append_buf(&buf, &smbserver.share));
  }
  netwib_er(netwox_arg_isset(parg, 'r', &isset));
  if (isset) {
    netwib__buf_reinit(&smbserver.rootdir);
    netwib_er(netwox_arg_buf(parg, 'r', &buf));
    netwib_er(netwib_path_canon(&buf, &smbserver.rootdir));
  }
  netwib_er(netwox_arg_buf(parg, 'u', &buf));
  buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwib_buf_append_buf(&buf, &smbserver.user));
  netwib_er(netwox_arg_buf(parg, 'w', &buf));
  buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwib_buf_append_buf(&buf, &smbserver.password));
  netwib_er(netwox_arg_uint32(parg, 'T', &smbserver.maxwaitms));
  netwib_er(netwox_arg_ips(parg, 'c', &smbserver.pallowedclients));
  netwib_er(netwox_arg_bool(parg, 'U', &smbserver.allowput));
  netwib_er(netwox_arg_bool(parg, 'V', &smbserver.verbose));
  netwib_er(netwox_arg_bool(parg, 'D', &smbserver.debug));

  /* main loop */
  ret = netwox_sock_tcp_mulser(&sockinfo, &netwox_smbserver_session,
                               &smbserver);

  /* close */
  netwib_er(netwox_smbserver_close(&smbserver));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(ret);
}
