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
netwib_conststring t000174_description[] = {
  "This tool downloads a FTP file and check its MD5.",
  NETWOX_DESC_check_work,
  "",
  NETWOX_DESC_userpass,
  NETWOX_DESC_ftp_passive,
  NETWOX_DESC_check_md5,
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000174_args[] = {
  NETWOX_SOCK_ARG_TCP_CLIPORT("21"),
  NETWOX_TOOLARG_OPT_BUF_LOGIN('u', "user", "username", "anonymous"),
  NETWOX_TOOLARG_OPT_BUF_PASSWORD('a', "pass", "password", "user@"),
  NETWOX_TOOLARG_OPT_BOOL('V', "passive", "passive", NULL),
  NETWOX_TOOLARG_REQ_BUF_FILE('F', "remote-file", "remote file",
                              "/etc/passwd"),
  NETWOX_TOOLARG_REQ_BUF_MD5('m', "md5", NULL, NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in milliseconds",
                             "60000"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000174_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_FTP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000174_info = {
  "FTP client : get a file and check its MD5",
  t000174_description,
  NULL,
  t000174_args,
  t000174_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000174_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_buf user, password, remotefile, tmpfilename, hexamd5, wantedmd5;
  netwib_bool passive;
  netwib_io *ptmpio;
  netwox_ftpclient ftpclient;
  netwib_uint32 maxwaitms;
  netwib_cmp cmp;
  netwib_err ret;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000174_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_tcp_cli(parg, &sockinfo));
  netwib_er(netwox_arg_buf(parg, 'u', &user));
  user.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  user.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwox_arg_buf(parg, 'a', &password));
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwox_arg_bool(parg, 'V', &passive));
  netwib_er(netwox_arg_buf(parg, 'F', &remotefile));
  netwib_er(netwox_arg_buf(parg, 'm', &wantedmd5));
  netwib_er(netwox_arg_uint32(parg, 'T', &maxwaitms));

  /* FTP session */
  netwib_er(netwox_ftpclient_init(&sockinfo, maxwaitms, &ftpclient));
  netwib_er(netwox_ftpclient_login(&ftpclient, &user, &password));
  netwib_er(netwox_ftpclient_bin(&ftpclient));
  if (passive) {
    netwib_er(netwox_ftpclient_passive(&ftpclient));
  } else {
    netwib_er(netwox_ftpclient_active(&ftpclient));
  }
  /* get file */
  netwib_er(netwib_buf_init_mallocdefault(&tmpfilename));
  netwib_er(netwib_io_init_filetemp(NETWIB_FALSE, &tmpfilename, &ptmpio));
  ret = netwox_ftpclient_file_get_io(&ftpclient, &remotefile, ptmpio);
  netwib_er(netwib_io_close(&ptmpio));
  if (ret == NETWIB_ERR_OK) {
    ret = netwox_ftpclient_close(&ftpclient);
  }

  /* check md5 */
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_init_mallocdefault(&hexamd5));
    ret = netwox_cryptohash_compute_file(NETWOX_CRYPTOHASH_TYPE_MD5,
                                         &tmpfilename, &hexamd5);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_buf_cmp(&hexamd5, &wantedmd5, &cmp));
      if (cmp != NETWIB_CMP_EQ) {
        netwib_er(netwib_fmt_display("MD5 is %{buf} instead of %{buf}\n",
                                     &hexamd5, &wantedmd5));
        ret = NETWOX_ERR_BADVALUE;
      }
    }
    netwib_er(netwib_buf_close(&hexamd5));
  }
  netwib_er(netwib_filename_remove(&tmpfilename));

  /* close */
  netwib_er(netwib_buf_close(&tmpfilename));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(ret);
}

