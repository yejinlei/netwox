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
netwib_conststring t000111_description[] = {
  "This tool lists the content of a FTP server directory.",
  "",
  NETWOX_DESC_userpass,
  NETWOX_DESC_ftp_passive,
  "Parameter --dir is the remote FTP directory name to list.",
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000111_args[] = {
  NETWOX_SOCK_ARG_TCP_CLIPORT("21"),
  NETWOX_TOOLARG_OPT_BUF_LOGIN('u', "user", "username", "anonymous"),
  NETWOX_TOOLARG_OPT_BUF_PASSWORD('a', "pass", "password", "user@"),
  NETWOX_TOOLARG_OPT_BOOL('V', "passive", "passive", NULL),
  NETWOX_TOOLARG_REQ_BUF_DIR('D', "dir", "directory name", "/etc/"),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in milliseconds",
                             "60000"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000111_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_FTP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000111_info = {
  "FTP listing a directory",
  t000111_description,
  NULL,
  t000111_args,
  t000111_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000111_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_buf user, password, dirnamebuf;
  netwib_bool passive;
  netwib_ring *pring;
  netwib_ring_index *pringindex;
  netwox_ftpclient ftpclient;
  netwox_ftp_dirlist *pdirlist;
  netwib_ptr pitem;
  netwib_uint32 maxwaitms;
  netwib_err ret;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000111_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_tcp_cli(parg, &sockinfo));
  netwib_er(netwox_arg_buf(parg, 'u', &user));
  user.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  user.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwox_arg_buf(parg, 'a', &password));
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwox_arg_bool(parg, 'V', &passive));
  netwib_er(netwox_arg_buf(parg, 'D', &dirnamebuf));
  netwib_er(netwox_arg_uint32(parg, 'T', &maxwaitms));

  /* FTP session */
  netwib_er(netwox_ftpclient_init(&sockinfo, maxwaitms, &ftpclient));

  netwib_er(netwox_ftpclient_login(&ftpclient, &user, &password));

  if (passive) {
    netwib_er(netwox_ftpclient_passive(&ftpclient));
  } else {
    netwib_er(netwox_ftpclient_active(&ftpclient));
  }

  netwib_er(netwox_ftp_dirlist_ring_init(&pring));
  netwib_er(netwib_ring_index_init(pring, &pringindex));
  netwib_er(netwox_ftpclient_dir_ls(&ftpclient, &dirnamebuf, pring));
  while(NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, &pitem);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    pdirlist = (netwox_ftp_dirlist *)pitem;
    netwib_er(netwib_buf_display(&pdirlist->name, NETWIB_ENCODETYPE_DATA));
    switch(pdirlist->pathstat.type) {
    case NETWIB_PATHSTAT_TYPE_REG :
      netwib_er(netwib_fmt_display(" (file of size %{uint32})\n",
                                   pdirlist->pathstat.size));
      break;
    case NETWIB_PATHSTAT_TYPE_DIR :
      netwib_er(netwib_fmt_display(" (dir)\n"));
      break;
    case NETWIB_PATHSTAT_TYPE_LINK :
      netwib_er(netwib_fmt_display(" (link to %{buf})\n",
                                   &pdirlist->linkname));
      break;
    default :
      netwib_er(netwib_fmt_display("\n"));
      break;
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  netwib_er(netwox_ftp_dirlist_ring_close(&pring));

  netwib_er(netwox_ftpclient_close(&ftpclient));

  /* close */
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
