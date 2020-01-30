/*
                                  NETWIB
                             Network library
                Copyright(c) 1999-2012 Laurent CONSTANTIN
                      http://ntwib.sourceforge.net/
                        laurentconstantin@free.fr
                                  -----
  This file is part of Netwib.

  Netwib is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3
  as published by the Free Software Foundation.

  Netwib is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details (http://www.gnu.org/).

------------------------------------------------------------------------
*/

#include <netwib/inc/maininc.h>

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_kbd(netwib_constbuf *pmessage,
                                 netwib_constbuf *pdefaulttext,
                                 netwib_buf *pbuf)
{
  netwib_er(netwib_priv_kbd_buf_append(pmessage, pdefaulttext,
                                       NETWIB_TRUE, ':', NETWIB_TRUE,
                                       pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_password(netwib_constbuf *pmessage,
                                      netwib_constbuf *pdefaulttext,
                                      netwib_buf *pbuf)
{
  netwib_er(netwib_priv_kbd_buf_append(pmessage, pdefaulttext,
                                       NETWIB_FALSE, ':', NETWIB_TRUE,
                                       pbuf));

  /* suppose this buffer is sensitive */
  pbuf->flags |= NETWIB_BUF_FLAGS_SENSITIVE;

  return(NETWIB_ERR_OK);
}
