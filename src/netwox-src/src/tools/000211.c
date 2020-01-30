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
netwib_conststring t000211_description[] = {
  "This tool indicates the original url of a local filename.",
  "For example files/02/f.html may be the local copy of http://s/f.php.",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000211_args[] = {
  NETWOX_TOOLARG_REQ_BUF_FILE_RD('c', "conffile", "configuration file", NULL),
  NETWOX_TOOLARG_REQ_BUF_FILE('f', "filename", "filename to convert", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000211_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_HTTP_SPIDER,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000211_info = {
  "Web spider : converts a local downloaded filename to its original url",
  t000211_description,
  NULL,
  t000211_args,
  t000211_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000211_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf conffile, localfilename, url;
  netwox_webspider webspider;
  netwib_err ret;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000211_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'c', &conffile));
  netwib_er(netwox_arg_buf(parg, 'f', &localfilename));

  /* convert */
  netwib_er(netwib_buf_init_mallocdefault(&url));
  netwib_er(netwox_webspider_init(&webspider));
  netwib_er(netwox_webspidercf_read(&conffile, &webspider));
  ret = netwib_urllocalbdd_url_init_local(&webspider.localrootdir,
                                          &localfilename, &url);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_fmt_display("%{buf}\n", &url));
  }
  netwib_er(netwox_webspider_close(&webspider));
  netwib_er(netwib_buf_close(&url));

  /* close */
  netwib_er(netwox_arg_close(&parg));
  return(ret);
}


