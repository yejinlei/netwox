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
netwib_conststring t000137_description[] = {
  "This tool creates a sample configuration file for web spider (tool",
  "138). After creation, it has to be edited.",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000137_args[] = {
  NETWOX_TOOLARG_REQ_BUF_FILE_WR('f', "conffile", "configuration file", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000137_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_HTTP_SPIDER,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000137_info = {
  "Create a sample configuration file for tool 138",
  t000137_description,
  "curl, wget",
  t000137_args,
  t000137_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000137_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf filename;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000137_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'f', &filename));

  /* write sample file */
  netwib_er(netwox_webspidercf_write_sample(&filename));

  /* close */
  netwib_er(netwox_arg_close(&parg));
  return(NETWIB_ERR_OK);
}
