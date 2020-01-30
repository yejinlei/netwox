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
netwib_conststring t000219_description[] = {
  "This tool computes the cryptographic hash of a file (MD2, MD4.",
  "M5, RIPEMD-128, RIPEMD-160, SHA-1, SHA-224, SHA-356, SHA-384,",
  "SHA512)",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000219_args[] = {
  NETWOX_TOOLARG_REQ_BUF_FILE_RD('f', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPT_BOOL('t', "title", "display titles", NULL),
  NETWOX_TOOLARG_OPT_BOOL('2', "disp-md2", "display md2", NULL),
  NETWOX_TOOLARG_OPT_BOOL('4', "disp-md4", "display md4", NULL),
  NETWOX_TOOLARG_OPT_BOOL('5', "disp-md5", "display md5", NULL),
  NETWOX_TOOLARG_OPT_BOOL('C', "disp-ripemd128", "display ripemd128", NULL),
  NETWOX_TOOLARG_OPT_BOOL('D', "disp-ripemd160", "display ripemd160", NULL),
  NETWOX_TOOLARG_OPT_BOOL('E', "disp-sha1", "display sha1", NULL),
  NETWOX_TOOLARG_OPT_BOOL('F', "disp-sha224", "display sha224", NULL),
  NETWOX_TOOLARG_OPT_BOOL('G', "disp-sha256", "display sha256", NULL),
  NETWOX_TOOLARG_OPT_BOOL('J', "disp-sha384", "display sha384", NULL),
  NETWOX_TOOLARG_OPT_BOOL('K', "disp-sha512", "display sha512", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000219_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NOTNET_FILE,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000219_info = {
  "Compute cryptographic hash of a file (md5, sha, etc.)",
  t000219_description,
  "md5sum",
  t000219_args,
  t000219_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000219_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_uint32 i;
  netwib_bool disptitle;
  netwib_bool dispcryptobool;
  netwib_bool dispcrypto[NETWOX_CRYPTOHASH_TYPE__LAST+1];
  netwib_buf filename, hash;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000219_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'f', &filename));
  netwib_er(netwox_arg_bool(parg, 't', &disptitle));
  netwib_er(netwox_arg_bool(parg, '2', &dispcrypto[NETWOX_CRYPTOHASH_TYPE_MD2]));
  netwib_er(netwox_arg_bool(parg, '4', &dispcrypto[NETWOX_CRYPTOHASH_TYPE_MD4]));
  netwib_er(netwox_arg_bool(parg, '5', &dispcrypto[NETWOX_CRYPTOHASH_TYPE_MD5]));
  netwib_er(netwox_arg_bool(parg, 'C', &dispcrypto[NETWOX_CRYPTOHASH_TYPE_RIPEMD128]));
  netwib_er(netwox_arg_bool(parg, 'D', &dispcrypto[NETWOX_CRYPTOHASH_TYPE_RIPEMD160]));
  netwib_er(netwox_arg_bool(parg, 'E', &dispcrypto[NETWOX_CRYPTOHASH_TYPE_SHA1]));
  netwib_er(netwox_arg_bool(parg, 'F', &dispcrypto[NETWOX_CRYPTOHASH_TYPE_SHA224]));
  netwib_er(netwox_arg_bool(parg, 'G', &dispcrypto[NETWOX_CRYPTOHASH_TYPE_SHA256]));
  netwib_er(netwox_arg_bool(parg, 'J', &dispcrypto[NETWOX_CRYPTOHASH_TYPE_SHA384]));
  netwib_er(netwox_arg_bool(parg, 'K', &dispcrypto[NETWOX_CRYPTOHASH_TYPE_SHA512]));

  /* if no option is set, they are all displayed */
  if (!disptitle) {
    dispcryptobool = NETWIB_FALSE;
    for (i = NETWOX_CRYPTOHASH_TYPE__FIRST; i <= NETWOX_CRYPTOHASH_TYPE__LAST; i++) {
      if (dispcrypto[i]) {
        dispcryptobool = NETWIB_TRUE;
        break;
      }
    }
    if (!dispcryptobool) {
      disptitle = NETWIB_TRUE;
      for (i = NETWOX_CRYPTOHASH_TYPE__FIRST; i <= NETWOX_CRYPTOHASH_TYPE__LAST; i++) {
        dispcrypto[i] = NETWIB_TRUE;
      }
    }
  }

  /* display */
  netwib_er(netwib_buf_init_mallocdefault(&hash));
  for (i = NETWOX_CRYPTOHASH_TYPE__FIRST; i <= NETWOX_CRYPTOHASH_TYPE__LAST; i++) {
    if (dispcrypto[i]) {
      if (disptitle) {
        netwib__buf_reinit(&hash);
        netwib_er(netwox_buf_append_cryptohash_type(i, &hash));
        netwib_er(netwib_fmt_display("%{buf}: ", &hash));
      }
      netwib__buf_reinit(&hash);
      netwib_er(netwox_cryptohash_compute_file(i, &filename, &hash));
      netwib_er(netwib_fmt_display("%{buf}\n", &hash));
    }
  }

  /* close */
  netwib_er(netwox_arg_close(&parg));
  netwib_er(netwib_buf_close(&hash));

  return(NETWIB_ERR_OK);
}
