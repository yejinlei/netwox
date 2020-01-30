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
netwib_conststring t000202_description[] = {
  "This tool is a simple SMB/CIFS client listing contents of a directory.",
  "",
  NETWOX_DESC_client,
  "",
  NETWOX_DESC_smbcli_share,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000202_args[] = {
  NETWOX_SMBCLI_TOOLARG_SHARE,
  NETWOX_TOOLARG_REQ_BUF_DIR('f', "dirname", "directory name", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000202_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_SMB,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000202_info = {
  "SMB/CIFS client: list contents of a directory",
  t000202_description,
  "smbclient",
  t000202_args,
  t000202_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000202_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_smbcli smbcli;
  netwib_buf dirnamebuf;
  netwib_ring *pring;
  netwib_ring_index *pringindex;
  netwox_smb_dirlist *pdirlist;
  netwib_ptr pitem;
  netwib_err ret=NETWIB_ERR_OK;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000202_info, &parg));
  netwib_er(netwox_smbcli_init_share(parg, &smbcli));
  netwib_er(netwox_arg_buf(parg, 'f', &dirnamebuf));

  /* SMB */
  netwib_er(netwox_smbcli_begin(&smbcli));

  netwib_er(netwox_smb_dirlist_ring_init(&pring));
  netwib_er(netwox_smbcli_dir_ls(&smbcli, &dirnamebuf, pring));
  netwib_er(netwib_ring_index_init(pring, &pringindex));
  while(NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, &pitem);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) {
        ret = NETWIB_ERR_OK;
      }
      break;
    }
    pdirlist = (netwox_smb_dirlist *)pitem;
    netwib_er(netwib_buf_display(&pdirlist->pathname, NETWIB_ENCODETYPE_DATA));
    switch(pdirlist->pathstat.type) {
    case NETWIB_PATHSTAT_TYPE_REG :
      netwib_er(netwib_fmt_display(" (file of size %{uint32})\n",
                                   pdirlist->pathstat.size));
      break;
    case NETWIB_PATHSTAT_TYPE_DIR :
      netwib_er(netwib_fmt_display(" (dir)\n"));
      break;
    default :
      netwib_er(netwib_fmt_display("\n"));
      break;
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  netwib_er(netwox_smb_dirlist_ring_close(&pring));

  netwib_er(netwox_smbcli_end(&smbcli));

  /* close */
  netwib_er(netwox_smbcli_close(&smbcli));
  netwib_er(netwox_arg_close(&parg));

  return(ret);
}
