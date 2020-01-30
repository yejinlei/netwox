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
netwib_err netwox_httpserresphdrs_init(netwox_httpserctx *pctx,
                                       netwib_io *pio,
                                       netwox_httphdr_statuscode statuscode,
                                       netwib_conststring statusmessage)
{
  netwib_buf *pbuf, bufmsg;

  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pbuf));

  netwib_er(netwib_buf_init_ext_string(statusmessage, &bufmsg));
  netwib_er(netwox_httphdr_status(statuscode, &bufmsg, pbuf));
  if (netwib__buf_ref_data_size(&pctx->login)) {
    netwib_er(netwox_httphdr_wwwauth(&pctx->realm, pbuf));
  }
  netwib_er(netwox_httphdr_connection_close(pbuf));
  netwib_er(netwib_io_write(pio, pbuf));

  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pbuf));

  return(NETWIB_ERR_OK);
}
