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
netwib_err netwox_identclient_query(netwib_io *pio,
                                    netwib_port localport,
                                    netwib_port remoteport,
                                    netwib_uint32 maxwaitms,
                                    netwib_buf *panswer)
{
  netwib_byte array[80];
  netwib_buf query;

  netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &query));

  netwib_er(netwib_buf_append_fmt(&query, "%{uint32},%{uint32}\r\n",
                                  remoteport, localport));
  netwib_er(netwib_io_write(pio, &query));

  netwib_er(netwox_txtproto_read_query(pio, maxwaitms, panswer));

  return(NETWIB_ERR_OK);
}
