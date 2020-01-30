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
netwib_err netwox_httpserctx_init(netwox_httpserctx *pctx)
{
  /* those can be changed by users */
  pctx->timeoutms = 60000;
  netwib_er(netwib_buf_init_ext_storage(&pctx->login));
  pctx->login.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  netwib_er(netwib_buf_init_ext_storage(&pctx->realm));
  netwib_er(netwib_buf_init_ext_storage(&pctx->password));
  pctx->password.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  netwib_er(netwib_buf_init_ext_storage(&pctx->rootdir));
  netwib_er(netwib_buf_append_string(".", &pctx->rootdir));
  pctx->pallowedclients = NULL;

  /* internally set */
  netwib_er(netwib_bufpool_init(NETWIB_TRUE, &pctx->pbufpool));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_httpserctx_close(netwox_httpserctx *pctx)
{
  netwib_er(netwib_bufpool_close(&pctx->pbufpool));
  netwib_er(netwib_buf_close(&pctx->realm));
  netwib_er(netwib_buf_close(&pctx->login));
  netwib_er(netwib_buf_close(&pctx->password));
  netwib_er(netwib_buf_close(&pctx->rootdir));

  return(NETWIB_ERR_OK);
}
