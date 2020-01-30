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
netwib_conststring t000166_description[] = {
  "This tool stores a file on a TFTP server.",
  "",
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000166_args[] = {
  NETWOX_SOCK_ARG_UDP_CLIPORT("69"),
  NETWOX_TOOLARG_REQ_BUF_FILE_WR('f', "local-file", "local file",
                                 "passwd_copy"),
  NETWOX_TOOLARG_REQ_BUF_FILE('F', "remote-file", "remote file",
                              "/etc/passwd"),
  NETWOX_TOOLARG_OPTA_BUF('m', "mode", "mode (octet or netascii)", "octet"),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in milliseconds",
                             "10000"),
  NETWOX_TOOLARG_OPTA_UINT32('R', "retry", "number of retry", "3"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000166_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_UDP_TFTP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000166_info = {
  "TFTP client : put a file",
  t000166_description,
  "upload",
  t000166_args,
  t000166_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000166_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwox_tftpinfos tftpinfos;
  netwib_buf remotefile, localfile, mode;
  netwox_tftppkt tftppktsend;
  netwib_io *pfileio;
  netwib_bufpool *pbufpool;
  netwib_err ret;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000166_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_udp_cli(parg, &sockinfo));
  netwib_er(netwox_arg_buf(parg, 'F', &remotefile));
  netwib_er(netwox_arg_buf(parg, 'f', &localfile));
  netwib_er(netwox_arg_buf(parg, 'm', &mode));
  netwib_er(netwox_arg_uint32(parg, 'T', &tftpinfos.maxwaitms));
  netwib_er(netwox_arg_uint32(parg, 'R', &tftpinfos.retry));

  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  tftpinfos.pbufpool = pbufpool;

  /* prepare io */
  netwib_er(netwox_sock_init(&sockinfo, &tftpinfos.pio));

  /* send a request */
  tftppktsend.opcode = NETWOX_TFTPOPCODE_WRQ;
  tftppktsend.pkt.rq.filename = remotefile;
  tftppktsend.pkt.rq.mode = mode;
  netwib_er(netwox_tftp_send(&tftpinfos, &tftppktsend));

  /* now, work on the server io */
  netwib_er(netwox_tftp_cli2ser(&sockinfo, &tftpinfos.pio));

  /* open local file */
  netwib_er(netwib_io_init_file_read(&localfile, &pfileio));

  /* loop */
  ret = netwox_tftp_write(&tftpinfos, pfileio, NETWIB_FALSE);

  /* close */
  netwib_er(netwib_io_close(&pfileio));
  netwib_er(netwib_io_close(&tftpinfos.pio));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));
  netwib_er(netwib_bufpool_close(&pbufpool));

  return(ret);
}
