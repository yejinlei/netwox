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
netwib_conststring t000138_description[] = {
  NETWOX_DESC_spider,
  "It needs a configuration file created with tool 137.",
  "It decides which url to download base on rules in configuration file.",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000138_args[] = {
  NETWOX_TOOLARG_REQ_BUF_FILE_RD('f', "conffile", "configuration file", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000138_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_HTTP_SPIDER,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000138_info = {
  "Web spider (use configuration file created by tool 137)",
  t000138_description,
  "curl, wget",
  t000138_args,
  t000138_nodes,
};


/*-------------------------------------------------------------*/
netwib_err t000138_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_webspider webspider;
  netwib_buf filename;
  netwib_err ret;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000138_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'f', &filename));

  /* spider */
  netwib_er(netwox_webspider_init(&webspider));
  ret = netwox_webspidercf_read(&filename, &webspider);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwox_webspider_run(&webspider));
    netwib_er(netwox_webspider_relink(&webspider));
    netwib_er(netwox_webspider_index(&webspider));
    netwib_er(netwox_webspider_createurllist(&webspider));
    netwib_er(netwox_webspider_deltmp(&webspider));
  }
  netwib_er(netwox_webspider_close(&webspider));

  /* close */
  netwib_er(netwox_arg_close(&parg));
  return(ret);
}


