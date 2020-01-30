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
netwib_conststring t000021_description[] = {
  "This tool converts a number, from/to decimal, binary, octal and",
  "hexadecimal. For example:",
  "  netwox 21 --hexa 3D4B",
  "  netwox 21 --hexa 3D4B --disp-decimal",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000021_args[] = {
  NETWOX_TOOLARG_REQ_BUF('n', "number", "number to convert", "10"),
  NETWOX_TOOLARG_RADIO1_SET('d', "decimal", "number is decimal"),
  NETWOX_TOOLARG_RADIO1('b', "binary", "number is binary"),
  NETWOX_TOOLARG_RADIO1('o', "octal", "number is octal"),
  NETWOX_TOOLARG_RADIO1('x', "hexadecimal", "number is hexadecimal"),
  NETWOX_TOOLARG_RADIO1('c', "char", "number is a character"),
  NETWOX_TOOLARG_OPT_BOOL('t', "title", "display titles", NULL),
  NETWOX_TOOLARG_OPT_BOOL('D', "disp-decimal", "display decimal", NULL),
  NETWOX_TOOLARG_OPT_BOOL('B', "disp-binary", "display binary", NULL),
  NETWOX_TOOLARG_OPT_BOOL('O', "disp-octal", "display octal", NULL),
  NETWOX_TOOLARG_OPT_BOOL('X', "disp-hexadecimal", "display hexadecimal",
                          NULL),
  NETWOX_TOOLARG_OPT_BOOL('C', "disp-char", "display character", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000021_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NOTNET,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000021_info = {
  "Convert a number",
  t000021_description,
  "calc, binary, decimal, hexadecimal, octal",
  t000021_args,
  t000021_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000021_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf buf;
  netwib_uint64 ui64;
  netwib_uint32 ui32;
  netwib_char c, *pc;
  netwib_bool disptitle, dispdec, dispbin, dispoct, disphexa, dispchar;

  /* obtain parameters */
  netwib__uint64_init_uint32(0, ui64);
  netwib_er(netwox_arg_init(argc, argv, &t000021_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'n', &buf));
  netwib_er(netwox_arg_radio1(parg, &c));
  switch(c) {
    case 'b' :
      netwib_er(netwib_buf_decode_fmt(&buf, "%{uint64:b}%$", &ui64));
      break;
    case 'd' :
      netwib_er(netwib_buf_decode_fmt(&buf, "%{uint64}%$", &ui64));
      break;
    case 'o' :
      netwib_er(netwib_buf_decode_fmt(&buf, "%{uint64:o}%$", &ui64));
      break;
    case 'x' :
      netwib_er(netwib_buf_decode_fmt(&buf, "%{uint64:x}%$", &ui64));
      break;
    case 'c' :
      if (netwib__buf_ref_data_size(&buf) > 1) {
        netwib_er(netwib_fmt_display("'%{buf}' is not a character\n", &buf));
        return(NETWOX_ERR_TOOLARG_INVALID);
      }
      pc = (netwib_char*)netwib__buf_ref_data_ptr(&buf);
      netwib__uint64_init_uint32(*pc, ui64);
      break;
  }
  netwib_er(netwox_arg_bool(parg, 't', &disptitle));
  netwib_er(netwox_arg_bool(parg, 'D', &dispdec));
  netwib_er(netwox_arg_bool(parg, 'B', &dispbin));
  netwib_er(netwox_arg_bool(parg, 'O', &dispoct));
  netwib_er(netwox_arg_bool(parg, 'X', &disphexa));
  netwib_er(netwox_arg_bool(parg, 'C', &dispchar));

  /* if no option is set, they are all displayed */
  if (!disptitle && !dispdec && !dispbin && !dispoct && !disphexa
      && !dispchar) {
    disptitle = NETWIB_TRUE;
    dispdec = NETWIB_TRUE;
    dispbin = NETWIB_TRUE;
    dispoct = NETWIB_TRUE;
    disphexa = NETWIB_TRUE;
    dispchar = NETWIB_TRUE;
  }

  /* display number */
  if (dispbin) {
    if (disptitle) {
      netwib_er(netwib_fmt_display("binary:  "));
    }
    netwib_er(netwib_fmt_display("%{uint64:b}\n", ui64));
  }
  if (dispoct) {
    if (disptitle) {
      netwib_er(netwib_fmt_display("octal:   "));
    }
    netwib_er(netwib_fmt_display("%{uint64:o}\n", ui64));
  }
  if (dispdec) {
    if (disptitle) {
      netwib_er(netwib_fmt_display("decimal: "));
    }
    netwib_er(netwib_fmt_display("%{uint64}\n", ui64));
  }
  if (disphexa) {
    if (disptitle) {
      netwib_er(netwib_fmt_display("hexa:    "));
    }
    netwib_er(netwib_fmt_display("%{uint64:X}\n", ui64));
  }
  if (dispchar) {
    netwib__uint32_init_uint64(ui64, ui32);
    if (netwib_c2_isprint(ui32)) {
      c = (netwib_char)ui32;
      if (disptitle) {
        netwib_er(netwib_fmt_display("char:    "));
      }
      netwib_er(netwib_fmt_display("%c\n", c));
    }
  }

  /* close */
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
