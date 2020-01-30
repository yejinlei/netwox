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
netwib_conststring t000012_description[] = {
  "This tool displays values to use for some netwox parameters.",
  "Some netwox parameters are not easy to guess. Netwag does not have",
  "this problem, because it lists accepted values.",
  "",
  "For example, running 'netwox 7 --help2' displays (extract)",
  " -R|--paraname recordencode ...",
  "This indicates parameter -R is of type 'recordencode'. Searching",
  "for recordencode in output of tool 12 will list all possible values",
  "(hexa, mixed, ...).",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000012_args[] = {
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000012_nodes[] = {
  NETWOX_TOOLTREENODETYPE_INFO_NETW,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000012_info = {
  "Display which values to use for netwox parameters",
  t000012_description,
  "help, show",
  t000012_args,
  t000012_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000012_core(int argc, char *argv[])
{
  netwox_arg *parg;

  netwib_er(netwox_arg_init(argc, argv, &t000012_info, &parg));
  netwib_er(netwox_arg_close(&parg));

  netwib_er(netwox_buf_toolargtype_help());

  return(NETWIB_ERR_OK);
}
