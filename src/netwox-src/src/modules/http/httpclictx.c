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
#include "../../netwox.h"

/*-------------------------------------------------------------*/
netwib_err netwox_httpclictx_init(netwox_httpclictx *pctx)
{
  netwib_er(netwox_dnscache_init(NETWIB_TRUE, NETWIB_FALSE, &pctx->dnscache));
  netwib_er(netwib_bufpool_init(NETWIB_TRUE, &pctx->pbufpool));
  pctx->timeoutms = 60000;

  pctx->useproxy = NETWIB_FALSE;
  netwib_er(netwib_buf_init_ext_storage(&pctx->proxy_login));
  pctx->proxy_login.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  netwib_er(netwib_buf_init_ext_storage(&pctx->proxy_password));
  pctx->proxy_password.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  netwib_er(netwib_buf_init_ext_storage(&pctx->useragent));
  pctx->ftp_passive = NETWIB_FALSE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_httpclictx_close(netwox_httpclictx *pctx)
{
  netwib_er(netwox_dnscache_close(&pctx->dnscache));
  netwib_er(netwib_bufpool_close(&pctx->pbufpool));
  netwib_er(netwib_buf_close(&pctx->proxy_login));
  netwib_er(netwib_buf_close(&pctx->proxy_password));
  netwib_er(netwib_buf_close(&pctx->useragent));

  return(NETWIB_ERR_OK);
}
