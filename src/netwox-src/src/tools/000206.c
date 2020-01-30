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
netwib_conststring t000206_description[] = {
  "This tool is a simple SMB/CIFS client putting a file.",
  "",
  NETWOX_DESC_client,
  "",
  NETWOX_DESC_smbcli_share,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000206_args[] = {
  NETWOX_SMBCLI_TOOLARG_SHARE,
  NETWOX_TOOLARG_REQ_BUF_FILE_RD('f', "localfilename", "local file name",
                                 NULL),
  NETWOX_TOOLARG_REQ_BUF_FILE('F', "remotefilename", "remote file name", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000206_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_SMB,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000206_info = {
  "SMB/CIFS client: put a file",
  t000206_description,
  "smbclient",
  t000206_args,
  t000206_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000206_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_smbcli smbcli;
  netwib_buf remotefilename, localfilename;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000206_info, &parg));
  netwib_er(netwox_smbcli_init_share(parg, &smbcli));
  netwib_er(netwox_arg_buf(parg, 'f', &localfilename));
  netwib_er(netwox_arg_buf(parg, 'F', &remotefilename));

  /* SMB */
  netwib_er(netwox_smbcli_begin(&smbcli));

  netwib_er(netwox_smbcli_file_put(&smbcli, &localfilename, &remotefilename));

  netwib_er(netwox_smbcli_end(&smbcli));

  /* close */
  netwib_er(netwox_smbcli_close(&smbcli));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
