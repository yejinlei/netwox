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
netwib_conststring t000025_description[] = {
  "This tool ensures a directory is secure. It is not needed for Windows.",
  "Under Unix, a publicly writable directory can be used by intruders",
  "to create a symlink attack.",
  "This tool checks if a directory is publicly writable.",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000025_args[] = {
  NETWOX_TOOLARG_REQ_BUF_DIR_RD('d', "dir", "directory to test", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000025_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NOTNET,
  NETWOX_TOOLTREENODETYPE_NOTNET_FILE,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000025_info = {
  "Test if a directory is secure",
  t000025_description,
  "symlink, verify",
  t000025_args,
  t000025_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000025_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf buf;
  netwib_bool sec;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000025_info, &parg));

  netwib_er(netwox_arg_buf(parg, 'd', &buf));
  netwib_er(netwib_dirname_secure(&buf, &sec));
  netwib_er(netwib_fmt_display("%{bool:0}\n", sec));

  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
