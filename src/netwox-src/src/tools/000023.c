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
netwib_conststring t000023_description[] = {
  "This tool displays ASCII table.",
  "It shows values for characters 0 to 127 or 255.",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000023_args[] = {
  NETWOX_TOOLARG_OPT_BOOL('e', "extended", "also display extended chars",
                          NULL),
  NETWOX_TOOLARG_OPT_BOOL('E', "all", "display all extended chars", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000023_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NOTNET,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000023_info = {
  "Display ASCII table",
  t000023_description,
  NULL,
  t000023_args,
  t000023_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000023_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_bool ext, all;
  netwib_uint32 i, n;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000023_info, &parg));
  netwib_er(netwox_arg_bool(parg, 'e', &ext));
  netwib_er(netwox_arg_bool(parg, 'E', &all));
  if (all) ext = NETWIB_TRUE;
  netwib_er(netwox_arg_close(&parg));

  netwib_er(netwib_fmt_display("   0 00 \\0 | 22 16 SYN| 44 2C  , | 66 42  B | 88 58  X | 110 6E  n\n"));
  netwib_er(netwib_fmt_display("   1 01 SOH| 23 17 ETB| 45 2D  - | 67 43  C | 89 59  Y | 111 6F  o\n"));
  netwib_er(netwib_fmt_display("   2 02 STX| 24 18 CAN| 46 2E  . | 68 44  D | 90 5A  Z | 112 70  p\n"));
  netwib_er(netwib_fmt_display("   3 03 ETX| 25 19 EM | 47 2F  / | 69 45  E | 91 5B  [ | 113 71  q\n"));
  netwib_er(netwib_fmt_display("   4 04 EOT| 26 1A SUB| 48 30 '0'| 70 46  F | 92 5C  \\ | 114 72  r\n"));
  netwib_er(netwib_fmt_display("   5 05 ENQ| 27 1B ESC| 49 31 '1'| 71 47  G | 93 5D  ] | 115 73  s\n"));
  netwib_er(netwib_fmt_display("   6 06 ACK| 28 1C FS | 50 32 '2'| 72 48  H | 94 5E  ^ | 116 74  t\n"));
  netwib_er(netwib_fmt_display("   7 07 \\a | 29 1D GS | 51 33 '3'| 73 49  I | 95 5F  _ | 117 75  u\n"));
  netwib_er(netwib_fmt_display("   8 08 \\b | 30 1E RS | 52 34 '4'| 74 4A  J | 96 60  ` | 118 76  v\n"));
  netwib_er(netwib_fmt_display("   9 09 \\t | 31 1F US | 53 35 '5'| 75 4B  K | 97 61  a | 119 77  w\n"));
  netwib_er(netwib_fmt_display("  10 0A \\n | 32 20 ' '| 54 36 '6'| 76 4C  L | 98 62  b | 120 78  x\n"));
  netwib_er(netwib_fmt_display("  11 0B \\v | 33 21  ! | 55 37 '7'| 77 4D  M | 99 63  c | 121 79  y\n"));
  netwib_er(netwib_fmt_display("  12 0C \\f | 34 22  \" | 56 38 '8'| 78 4E  N |100 64  d | 122 7A  z\n"));
  netwib_er(netwib_fmt_display("  13 0D \\r | 35 23  # | 57 39 '9'| 79 4F  O |101 65  e | 123 7B  {\n"));
  netwib_er(netwib_fmt_display("  14 0E SO | 36 24  $ | 58 3A  : | 80 50  P |102 66  f | 124 7C  |\n"));
  netwib_er(netwib_fmt_display("  15 0F SI | 37 25  %% | 59 3B  ; | 81 51  Q |103 67  g | 125 7D  }\n"));
  netwib_er(netwib_fmt_display("  16 10 DLE| 38 26  & | 60 3C  < | 82 52  R |104 68  h | 126 7E  ~\n"));
  netwib_er(netwib_fmt_display("  17 11 DC1| 39 27  ' | 61 3D  = | 83 53  S |105 69  i | 127 7F DEL\n"));
  netwib_er(netwib_fmt_display("  18 12 DC2| 40 28  ( | 62 3E  > | 84 54  T |106 6A  j | \n"));
  netwib_er(netwib_fmt_display("  19 13 DC3| 41 29  ) | 63 3F  ? | 85 55  U |107 6B  k | \n"));
  netwib_er(netwib_fmt_display("  20 14 DC4| 42 2A  *"));
  netwib_er(netwib_fmt_display(" | 64 40  @ | 86 56  V |108 6C  l | \n"));
  netwib_er(netwib_fmt_display("  21 15 NAK| 43 2B  + | 65 41  A | 87 57  W |109 6D  m | \n"));

  if (ext) {
    netwib_er(netwib_fmt_display("--\n"));
    for (i = 128; i < 150; i++) {
      n = i;
      netwib_er(netwib_fmt_display("  %{uint32} %{uint32:X} %c", n, n,
                                   (all)?(netwib_char)n:' '));
      n += 22;
      netwib_er(netwib_fmt_display(" | %{uint32} %{uint32:X} %c", n, n,
                                   (!all&&n<0xA0)?' ':(netwib_char)n));
      n += 22;
      netwib_er(netwib_fmt_display(" | %{uint32} %{uint32:X} %c", n, n,
                                   (netwib_char)n));
      n += 22;
      netwib_er(netwib_fmt_display(" | %{uint32} %{uint32:X} %c", n, n,
                                   (netwib_char)n));
      n += 22;
      netwib_er(netwib_fmt_display(" | %{uint32} %{uint32:X} %c", n, n,
                                   (netwib_char)n));
      n += 22;
      if (n < 256) {
        netwib_er(netwib_fmt_display(" | %{uint32} %{uint32:X} %c\n", n, n,
                                     (netwib_char)n));
      } else {
        netwib_er(netwib_fmt_display("\n"));
      }
    }
  }

  return(NETWIB_ERR_OK);
}

