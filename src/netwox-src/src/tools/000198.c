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
netwib_conststring t000198_description[] = {
  "This tool is a simple SMB/CIFS client listing shares.",
  "",
  NETWOX_DESC_client,
  "",
  NETWOX_DESC_smbcli_ipc,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000198_args[] = {
  NETWOX_SMBCLI_TOOLARG_IPC,
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000198_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_SMB,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000198_info = {
  "SMB/CIFS client: list shares",
  t000198_description,
  "smbclient",
  t000198_args,
  t000198_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000198_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_smbcli smbcli;
  netwib_ring *pring;
  netwib_ring_index *pringindex;
  netwox_smb_sharelist *psharelist;
  netwib_ptr pitem;
  netwib_err ret=NETWIB_ERR_OK;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000198_info, &parg));
  netwib_er(netwox_smbcli_init_ipc(parg, &smbcli));

  /* SMB */
  netwib_er(netwox_smbcli_begin(&smbcli));

  netwib_er(netwox_smb_sharelist_ring_init(&pring));
  netwib_er(netwox_smbcli_shares(&smbcli, pring));
  netwib_er(netwib_ring_index_init(pring, &pringindex));
  while(NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, &pitem);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) {
        ret = NETWIB_ERR_OK;
      }
      break;
    }
    psharelist = (netwox_smb_sharelist *)pitem;
    netwib_er(netwib_fmt_display("share=%{buf}\ncomment=%{buf}\nsharetype=%{uint32}\n\n", &psharelist->name, &psharelist->comment, psharelist->sharetype));
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  netwib_er(netwox_smb_sharelist_ring_close(&pring));

  netwib_er(netwox_smbcli_end(&smbcli));

  /* close */
  netwib_er(netwox_smbcli_close(&smbcli));
  netwib_er(netwox_arg_close(&parg));

  return(ret);
}
