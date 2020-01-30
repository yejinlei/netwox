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
netwib_conststring t000133_description[] = {
  "This tool normalizes an url/uri. For example:",
  "  netwox 133 --uri \"http://server/path/../foo\"",
  "returns:",
  "  http://server/foo",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000133_args[] = {
  NETWOX_TOOLARG_REQ_BUF_URI('u', "uri", "uri to convert",
                             "http://server/path/../foo"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000133_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_HTTP,
  NETWOX_TOOLTREENODETYPE_NOTNET,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000133_info = {
  "Convert an url/uri",
  t000133_description,
  NULL,
  t000133_args,
  t000133_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000133_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf url, url2;
  netwib_bufpool *pbufpool;
  netwib_err ret;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000133_info, &parg));
  /* url */
  netwib_er(netwox_arg_buf(parg, 'u', &url));

  /* convert */
  netwib_er(netwib_buf_init_mallocdefault(&url2));
  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  ret = netwox_urlcanon_url(pbufpool, &url, &url2);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_fmt_display("%{buf}\n", &url2));
  }
  netwib_er(netwib_bufpool_close(&pbufpool));
  netwib_er(netwib_buf_close(&url2));

  /* close */
  netwib_er(netwox_arg_close(&parg));

  return(ret);
}
