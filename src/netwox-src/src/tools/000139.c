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
netwib_conststring t000139_description[] = {
  NETWOX_DESC_spider,
  "It stops when all url have been downloaded. So it is not suitable for",
  "web sites with outgoing web branches (use tool 210 or 138 instead).",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000139_args[] = {
  NETWOX_TOOLARG_REQ_BUF_URI('u', "url", "url to download", NULL),
  NETWOX_TOOLARG_OPT_BUF_DIR_RD('r', "rootdir", "local root directory",
                                "./spider"),
  NETWOX_TOOLARG_OPT_BOOL('v', "verbose", "be verbose", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000139_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_HTTP_SPIDER,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000139_info = {
  "Web spider on command line (fully recursive)",
  t000139_description,
  "curl, wget",
  t000139_args,
  t000139_nodes,
};


/*-------------------------------------------------------------*/
netwib_err t000139_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_webspider webspider;
  netwib_buf url, rootdir;
  netwib_bool isset, verbose;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000139_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'u', &url));

  /* spider */
  netwib_er(netwox_webspider_init(&webspider));
  netwib_er(netwox_arg_isset(parg, 'r', &isset));
  if (isset) {
    netwib__buf_reinit(&webspider.localrootdir);
    netwib_er(netwox_arg_buf(parg, 'r', &rootdir));
    netwib_er(netwib_path_canon(&rootdir, &webspider.localrootdir));
  }
  netwib_er(netwox_arg_bool(parg, 'v', &verbose));
  if (verbose) {
    webspider.loglevel = NETWOX_WEBSPIDER_LOGLEVEL_NORMAL;
  } else {
    webspider.loglevel = NETWOX_WEBSPIDER_LOGLEVEL_SCALE;
  }
  netwib_er(netwox_webspider_add_canon(&webspider, &url));
  netwib_er(netwox_webspider_run(&webspider));
  netwib_er(netwox_webspider_relink(&webspider));
  netwib_er(netwox_webspider_index(&webspider));
  netwib_er(netwox_webspider_createurllist(&webspider));
  netwib_er(netwox_webspider_deltmp(&webspider));
  netwib_er(netwox_webspider_close(&webspider));

  /* close */
  netwib_er(netwox_arg_close(&parg));
  return(NETWIB_ERR_OK);
}


