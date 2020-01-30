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
netwib_conststring t000212_description[] = {
  "This tool indicates in which local filename an url is stored.",
  "For example http://server/f.php may be stored in files/02/f.html.",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000212_args[] = {
  NETWOX_TOOLARG_REQ_BUF_FILE_RD('c', "conffile", "configuration file", NULL),
  NETWOX_TOOLARG_REQ_BUF_URI('u', "url", "url to check", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000212_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_HTTP_SPIDER,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000212_info = {
  "Web spider : converts an url to its local downloaded filename",
  t000212_description,
  NULL,
  t000212_args,
  t000212_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000212_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf conffile, url, localfilename;
  netwox_webspider webspider;
  netwib_err ret;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000212_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'c', &conffile));
  netwib_er(netwox_arg_buf(parg, 'u', &url));

  /* convert */
  netwib_er(netwib_buf_init_mallocdefault(&localfilename));
  netwib_er(netwox_webspider_init(&webspider));
  netwib_er(netwox_webspidercf_read(&conffile, &webspider));
  ret = netwib_urllocalbdd_local_init_url(&webspider.localrootdir, &url,
                                          &localfilename);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_fmt_display("%{buf}\n", &localfilename));
  }
  netwib_er(netwox_webspider_close(&webspider));
  netwib_er(netwib_buf_close(&localfilename));

  /* close */
  netwib_er(netwox_arg_close(&parg));
  return(ret);
}
