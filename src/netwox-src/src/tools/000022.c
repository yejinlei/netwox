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
netwib_conststring t000022_description[] = {
  "This tool converts a text, from/to text, hexadecimal, mixed, base64",
  "and md4/md5 (md4/md5 sum can only be computed).",
  "For example:",
  "  netwox 22 --hexa 4142",
  "  netwox 22 --hexa 4142 --disp-mixed",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000022_args[] = {
  NETWOX_TOOLARG_REQ_BUF('d', NULL, "data to convert", "hello"),
  NETWOX_TOOLARG_RADIO1_SET('s', "string", "data is string"),
  NETWOX_TOOLARG_RADIO1('h', "hexa", "data is hexa"),
  NETWOX_TOOLARG_RADIO1('m', "mixed", "data is mixed"),
  NETWOX_TOOLARG_RADIO1('b', "base64", "data is base64"),
  NETWOX_TOOLARG_OPT_BOOL('t', "title", "display titles", NULL),
  NETWOX_TOOLARG_OPT_BOOL('S', "disp-string", "display string", NULL),
  NETWOX_TOOLARG_OPT_BOOL('H', "disp-hexa", "display hexa", NULL),
  NETWOX_TOOLARG_OPT_BOOL('M', "disp-mixed", "display mixed", NULL),
  NETWOX_TOOLARG_OPT_BOOL('B', "disp-base64", "display base64", NULL),
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
netwox_tooltreenodetype t000022_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NOTNET,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000022_info = {
  "Convert a string",
  t000022_description,
  "base64, hexadecimal, text, mixed, md5sum",
  t000022_args,
  t000022_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000022_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf buf, buf2, buf3;
  netwib_uint32 i;
  netwib_char c;
  netwib_bool disptitle, dispstring, disphexa, dispmixed, dispbase64;
  netwib_bool dispcryptobool;
  netwib_bool dispcrypto[NETWOX_CRYPTOHASH_TYPE__LAST+1];

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000022_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'd', &buf));
  netwib_er(netwox_arg_radio1(parg, &c));
  netwib_er(netwox_arg_bool(parg, 't', &disptitle));
  netwib_er(netwox_arg_bool(parg, 'S', &dispstring));
  netwib_er(netwox_arg_bool(parg, 'H', &disphexa));
  netwib_er(netwox_arg_bool(parg, 'M', &dispmixed));
  netwib_er(netwox_arg_bool(parg, 'B', &dispbase64));
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

  netwib_er(netwib_buf_init_mallocdefault(&buf2));
  switch(c) {
    case 's' :
      netwib_er(netwib_buf_append_buf(&buf, &buf2));
      break;
    case 'h' :
      netwib_er(netwib_buf_decode(&buf, NETWIB_DECODETYPE_HEXA, &buf2));
      break;
    case 'm' :
      netwib_er(netwib_buf_decode(&buf, NETWIB_DECODETYPE_MIXED, &buf2));
      break;
    case 'b' :
      netwib_er(netwib_buf_decode(&buf, NETWIB_DECODETYPE_BASE64, &buf2));
      break;
  }

  /* if no option is set, they are all displayed */
  if (!disptitle && !dispstring && !disphexa && !dispmixed && !dispbase64) {
    dispcryptobool = NETWIB_FALSE;
    for (i = NETWOX_CRYPTOHASH_TYPE__FIRST; i <= NETWOX_CRYPTOHASH_TYPE__LAST; i++) {
      if (dispcrypto[i]) {
        dispcryptobool = NETWIB_TRUE;
        break;
      }
    }
    if (!dispcryptobool) {
      disptitle = NETWIB_TRUE;
      dispstring = NETWIB_TRUE;
      disphexa = NETWIB_TRUE;
      dispmixed = NETWIB_TRUE;
      dispbase64 = NETWIB_TRUE;
      for (i = NETWOX_CRYPTOHASH_TYPE__FIRST; i <= NETWOX_CRYPTOHASH_TYPE__LAST; i++) {
        dispcrypto[i] = NETWIB_TRUE;
      }
    }
  }

  /* display */
  netwib_er(netwib_buf_init_mallocdefault(&buf3));
  if (dispstring) {
    if (disptitle) {
      netwib_er(netwib_fmt_display("string: "));
    }
    netwib_er(netwib_fmt_display("%{buf}\n", &buf2));
  }
  if (disphexa) {
    if (disptitle) {
      netwib_er(netwib_fmt_display("hexa: "));
    }
    netwib_er(netwib_buf_encode(&buf2, NETWIB_ENCODETYPE_HEXA, &buf3));
    netwib_er(netwib_fmt_display("%{buf}\n", &buf3));
  }
  if (dispmixed) {
    if (disptitle) {
      netwib_er(netwib_fmt_display("mixed: "));
    }
    netwib__buf_reinit(&buf3);
    netwib_er(netwib_buf_encode(&buf2, NETWIB_ENCODETYPE_MIXED, &buf3));
    netwib_er(netwib_fmt_display("%{buf}\n", &buf3));
  }
  if (dispbase64) {
    if (disptitle) {
      netwib_er(netwib_fmt_display("base64: "));
    }
    netwib__buf_reinit(&buf3);
    netwib_er(netwib_buf_encode(&buf2, NETWIB_ENCODETYPE_BASE64, &buf3));
    netwib_er(netwib_fmt_display("%{buf}\n", &buf3));
  }
  for (i = NETWOX_CRYPTOHASH_TYPE__FIRST; i <= NETWOX_CRYPTOHASH_TYPE__LAST; i++) {
    if (dispcrypto[i]) {
      if (disptitle) {
        netwib__buf_reinit(&buf3);
        netwib_er(netwox_buf_append_cryptohash_type(i, &buf3));
        netwib_er(netwib_fmt_display("%{buf}: ", &buf3));
      }
      netwib__buf_reinit(&buf3);
      netwib_er(netwox_cryptohash_compute_hexa(i, &buf2, &buf3));
      netwib_er(netwib_fmt_display("%{buf}\n", &buf3));
    }
  }

  /* close */
  netwib_er(netwox_arg_close(&parg));
  netwib_er(netwib_buf_close(&buf2));
  netwib_er(netwib_buf_close(&buf3));

  return(NETWIB_ERR_OK);
}
