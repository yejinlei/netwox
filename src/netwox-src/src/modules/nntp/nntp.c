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
netwib_err netwox_nntp_write_modereader(netwib_io *pio)
{
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string("MODE READER\r\n", &buf));
  ret = netwib_io_write(pio, &buf);
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_nntp_write_group(netwib_io *pio,
                                   netwib_constbuf *pnewsgroup)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));

  netwib_er(netwib_buf_append_string("GROUP ", &buf));
  netwib_er(netwib_buf_append_buf(pnewsgroup, &buf));
  netwib_er(netwib_buf_append_string("\r\n", &buf));

  ret = netwib_io_write(pio, &buf);

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_nntp_write_article(netwib_io *pio,
                                     netwib_uint32 artnum)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));

  netwib_er(netwib_buf_append_fmt(&buf, "ARTICLE %{uint32}\r\n", artnum));

  ret = netwib_io_write(pio, &buf);

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_nntp_write_list(netwib_io *pio)
{
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string("LIST\r\n", &buf));
  ret = netwib_io_write(pio, &buf);
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_nntp_write_quit(netwib_io *pio)
{
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string("QUIT\r\n", &buf));
  ret = netwib_io_write(pio, &buf);
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_nntp_write_authinfo_user(netwib_io *pio,
                                           netwib_constbuf *puser)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));
  buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE;

  netwib_er(netwib_buf_append_string("AUTHINFO USER ", &buf));
  netwib_er(netwib_buf_append_buf(puser, &buf));
  netwib_er(netwib_buf_append_string("\r\n", &buf));

  ret = netwib_io_write(pio, &buf);

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_nntp_write_authinfo_pass(netwib_io *pio,
                                           netwib_constbuf *ppassword)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));
  buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE;

  netwib_er(netwib_buf_append_string("AUTHINFO PASS ", &buf));
  netwib_er(netwib_buf_append_buf(ppassword, &buf));
  netwib_er(netwib_buf_append_string("\r\n", &buf));

  ret = netwib_io_write(pio, &buf);

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_nntp_write_xover(netwib_io *pio,
                                   netwib_uint32 firstartnum,
                                   netwib_uint32 lastartnum)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));

  netwib_er(netwib_buf_append_string("XOVER ", &buf));
  netwib_er(netwib_buf_append_fmt(&buf, "%{uint32}", firstartnum));
  if (lastartnum > firstartnum) {
    netwib_er(netwib_buf_append_fmt(&buf, "-%{uint32}", lastartnum));
  }
  netwib_er(netwib_buf_append_string("\r\n", &buf));

  ret = netwib_io_write(pio, &buf);

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_nntp_write_post_begin(netwib_io *pio)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_ext_string("POST\r\n", &buf));
  netwib_er(netwib_io_write(pio, &buf));

  return(NETWIB_ERR_OK);
}
