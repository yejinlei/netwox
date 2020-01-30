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
netwib_conststring t000027_description[] = {
  "This tool computes the MD5 checksum of a file.",
  "A MD5 checksum is a hash ensuring a file has not been corrupted.",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000027_args[] = {
  NETWOX_TOOLARG_REQ_BUF_FILE_RD('f', NULL, NULL, NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000027_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NOTNET_FILE,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000027_info = {
  "Compute MD5 of a file",
  t000027_description,
  "hash, md5sum, verify",
  t000027_args,
  t000027_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000027_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf filename, hexamd5;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000027_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'f', &filename));
  netwib_er(netwib_buf_init_mallocdefault(&hexamd5));

  netwib_er(netwox_cryptohash_compute_file(NETWOX_CRYPTOHASH_TYPE_MD5,
                                           &filename, &hexamd5));
  netwib_er(netwib_fmt_display("%{buf}\n", &hexamd5));

  netwib_er(netwib_buf_close(&hexamd5));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
