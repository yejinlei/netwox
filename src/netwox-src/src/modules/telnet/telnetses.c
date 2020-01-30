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
#define NETWOX_TELNETSES_EXPECT_PROMPT "[$%#>] $"
#define NETWOX_TELNETSES_EXPECT_LOGIN1 "login[ :]*$"
#define NETWOX_TELNETSES_EXPECT_LOGIN2 "username[ :]*$"
#define NETWOX_TELNETSES_EXPECT_PASSWORD "password[ :]*$"
#define NETWOX_TELNETSES_EXPECT_BADLOGIN "login incorrect"

/*-------------------------------------------------------------*/
netwib_err netwox_telnetses_login(netwib_io *pio,
                                  netwib_consttime *pabstime,
                                  netwib_constbuf *plogin,
                                  netwib_constbuf *ppassword)
{
  netwib_buf buf, bufnl;
  netwib_uint32 numfound;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  /* expect banner */
  netwib_er(netwox_expect2(&buf, NETWOX_TELNETSES_EXPECT_LOGIN1,
                           NETWOX_TELNETSES_EXPECT_LOGIN2, NETWIB_FALSE,
                           pio, pabstime, &numfound));
  if (numfound == 0) {
    netwib_er(netwib_buf_close(&buf));
    return(NETWOX_ERR_TIMEOUT);
  }

  /* write login */
  netwib_er(netwib_io_write(pio, plogin));
  netwib_er(netwib_buf_init_ext_string("\n", &bufnl));
  netwib_er(netwib_io_write(pio, &bufnl));

  /* expect banner */
  netwib__buf_reinit(&buf);
  netwib_er(netwox_expect1(&buf, NETWOX_TELNETSES_EXPECT_PASSWORD,
                           NETWIB_FALSE, pio, pabstime, &numfound));
  if (numfound == 0) {
    netwib_er(netwib_buf_close(&buf));
    return(NETWOX_ERR_TIMEOUT);
  }

  /* write password */
  netwib_er(netwib_io_write(pio, ppassword));
  netwib_er(netwib_buf_init_ext_string("\n", &bufnl));
  netwib_er(netwib_io_write(pio, &bufnl));

  /* expect banner */
  netwib__buf_reinit(&buf);
  netwib_er(netwox_expect4(&buf, NETWOX_TELNETSES_EXPECT_PROMPT,
                           NETWOX_TELNETSES_EXPECT_LOGIN1,
                           NETWOX_TELNETSES_EXPECT_LOGIN2,
                           NETWOX_TELNETSES_EXPECT_BADLOGIN, NETWIB_FALSE,
                           pio, pabstime, &numfound));
  if (numfound == 0) {
    netwib_er(netwib_buf_close(&buf));
    return(NETWOX_ERR_TIMEOUT);
  }
  if (numfound != 1) {
    netwib_er(netwib_buf_close(&buf));
    return(NETWOX_ERR_AUTHERROR);
  }

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_telnetses_logout(netwib_io *pio)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_ext_string("logout\n", &buf));
  netwib_er(netwib_io_write(pio, &buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_telnetses_exec(netwib_io *pio,
                                 netwib_consttime *pabstime,
                                 netwib_constbuf *pcmd,
                                 netwib_buf *pbuf)
{
  netwib_buf buf, bufnl;
  netwib_data data, pc;
  netwib_uint32 datasize, numfound;

  /* write command */
  netwib_er(netwib_io_write(pio, pcmd));
  netwib_er(netwib_buf_init_ext_string("\n", &bufnl));
  netwib_er(netwib_io_write(pio, &bufnl));

  /* expect prompt */
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwox_expect1(&buf, NETWOX_TELNETSES_EXPECT_PROMPT,
                           NETWIB_FALSE, pio, pabstime, &numfound));
  if (numfound == 0) {
    netwib_er(netwib_buf_close(&buf));
    return(NETWOX_ERR_TIMEOUT);
  }

  /* convert read buffer to command output buffer : suppress first line
     (contain our command) and last line (contain prompt) */
  data = netwib__buf_ref_data_ptr(&buf);
  datasize = netwib__buf_ref_data_size(&buf);
  while (datasize) {
    datasize--;
    if (data[datasize] == '\n') {
      datasize++;
      break;
    }
  }
  pc = netwib_c_memchr(data, '\n', datasize);
  if (pc) {
    pc++;
    netwib_er(netwib_buf_append_data(pc, datasize - (pc - data), pbuf));
  } else {
    netwib_er(netwib_buf_append_data(data, datasize, pbuf));
  }

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

