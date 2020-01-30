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
netwib_conststring t000096_description[] = {
  "This tool put a file to server of tool 93.",
  "",
  "Parameter --src-file contains the local filename.",
  "Parameter --file defines the remote filename to write.",
  NETWOX_DESC_remadm_passcyp,
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000096_args[] = {
  NETWOX_SOCK_ARG_TCP_CLI,
  NETWOX_TOOLARG_REQ_BUF_FILE_RD('f', NULL, NULL, NULL),
  NETWOX_TOOLARG_REQ_BUF_FILE('F', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPT_BUF_PASSWORD('w', "password", "password", "netwox"),
  NETWOX_TOOLARG_OPTA_BOOL('x', "cypher", "if data is encrypted", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in seconds", "100"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000096_nodes[] = {
  NETWOX_TOOLTREENODETYPE_REMADM,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000096_info = {
  "TCP remote administration client (put file)",
  t000096_description,
  NULL,
  t000096_args,
  t000096_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000096_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_io *psockio, *plocalio;
  netwox_remadmtcp_cli remadmtcp;
  netwib_buf password, remotefile, localfile;
  netwib_bool cypher;
  netwib_uint32 timeoutsec;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000096_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_tcp_cli(parg, &sockinfo));
  netwib_er(netwox_arg_buf(parg, 'w', &password));
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwox_arg_buf(parg, 'f', &localfile));
  netwib_er(netwox_arg_buf(parg, 'F', &remotefile));
  netwib_er(netwox_arg_bool(parg, 'x', &cypher));
  netwib_er(netwox_arg_uint32(parg, 'T', &timeoutsec));

  /* initialize localio */
  netwib_er(netwib_io_init_file_read(&localfile, &plocalio));

  /* initialize structure */
  netwib_er(netwox_sock_init(&sockinfo, &psockio));
  netwib_er(netwox_remadmtcp_cli_init(psockio, timeoutsec, &remadmtcp));

  /* cypher */
  if (cypher) {
    netwib_er(netwox_remadmtcp_cli_cypher(&remadmtcp, &password));
  }

  /* authenticate */
  netwib_er(netwox_remadmtcp_cli_auth(&remadmtcp, &password));

  /* transfer file */
  netwib_er(netwox_remadmtcp_cli_put(&remadmtcp, plocalio, &remotefile));

  /* quit */
  netwib_er(netwox_remadmtcp_cli_quit(&remadmtcp));

  /* close */
  netwib_er(netwox_remadmtcp_cli_close(&remadmtcp));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwib_io_close(&plocalio));
  netwib_er(netwib_io_close(&psockio));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
