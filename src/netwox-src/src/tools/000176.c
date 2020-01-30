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
netwib_conststring t000176_description[] = {
  "This tool downloads a TFTP file and check its MD5.",
  NETWOX_DESC_check_work,
  "",
  NETWOX_DESC_check_md5,
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000176_args[] = {
  NETWOX_SOCK_ARG_UDP_CLIPORT("69"),
  NETWOX_TOOLARG_REQ_BUF_FILE('F', "remote-file", "remote file",
                              "/etc/passwd"),
  NETWOX_TOOLARG_REQ_BUF_MD5('s', "md5", NULL, NULL),
  NETWOX_TOOLARG_OPTA_BUF('m', "mode", "mode (octet or netascii)", "octet"),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in milliseconds",
                             "60000"),
  NETWOX_TOOLARG_OPTA_UINT32('R', "retry", "number of retry", "3"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000176_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_UDP_TFTP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000176_info = {
  "TFTP client : get a file and check its MD5",
  t000176_description,
  NULL,
  t000176_args,
  t000176_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000176_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwox_tftpinfos tftpinfos;
  netwib_buf remotefile, mode, tmpfilename, hexamd5, wantedmd5;
  netwox_tftppkt tftppktsend;
  netwib_io *ptmpio;
  netwib_bufpool *pbufpool;
  netwib_cmp cmp;
  netwib_err ret;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000176_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_udp_cli(parg, &sockinfo));
  netwib_er(netwox_arg_buf(parg, 'F', &remotefile));
  netwib_er(netwox_arg_buf(parg, 's', &wantedmd5));
  netwib_er(netwox_arg_buf(parg, 'm', &mode));
  netwib_er(netwox_arg_uint32(parg, 'T', &tftpinfos.maxwaitms));
  netwib_er(netwox_arg_uint32(parg, 'R', &tftpinfos.retry));

  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  tftpinfos.pbufpool = pbufpool;

  /* prepare io */
  netwib_er(netwox_sock_init(&sockinfo, &tftpinfos.pio));

  /* send a request */
  tftppktsend.opcode = NETWOX_TFTPOPCODE_RRQ;
  tftppktsend.pkt.rq.filename = remotefile;
  tftppktsend.pkt.rq.mode = mode;
  netwib_er(netwox_tftp_send(&tftpinfos, &tftppktsend));

  /* now, work on the server io */
  netwib_er(netwox_tftp_cli2ser(&sockinfo, &tftpinfos.pio));

  /* request */
  netwib_er(netwib_buf_init_mallocdefault(&tmpfilename));
  netwib_er(netwib_io_init_filetemp(NETWIB_FALSE, &tmpfilename, &ptmpio));
  ret = netwox_tftp_read(&tftpinfos, ptmpio, NETWIB_FALSE);
  netwib_er(netwib_io_close(&ptmpio));

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
  netwib_er(netwib_io_close(&tftpinfos.pio));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));
  netwib_er(netwib_bufpool_close(&pbufpool));

  return(ret);
}
