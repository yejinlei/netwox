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
netwib_conststring t000209_description[] = {
  "This tool is a simple SMB/CIFS client recursively deleting a",
  "directory. This tool might be dangerous. Use with care.",
  "",
  NETWOX_DESC_client,
  "",
  NETWOX_DESC_smbcli_share,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000209_args[] = {
  NETWOX_SMBCLI_TOOLARG_SHARE,
  NETWOX_TOOLARG_REQ_BUF_DIR('f', "dirname", "directory name", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000209_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_SMB,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000209_info = {
  "SMB/CIFS client: recursively delete a directory",
  t000209_description,
  "smbclient",
  t000209_args,
  t000209_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000209_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_smbcli smbcli;
  netwib_buf dirnamebuf;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000209_info, &parg));
  netwib_er(netwox_smbcli_init_share(parg, &smbcli));
  netwib_er(netwox_arg_buf(parg, 'f', &dirnamebuf));

  /* SMB */
  netwib_er(netwox_smbcli_begin(&smbcli));

  netwib_er(netwox_smbcli_dirrec_del(&smbcli, &dirnamebuf));

  netwib_er(netwox_smbcli_end(&smbcli));

  /* close */
  netwib_er(netwox_smbcli_close(&smbcli));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
