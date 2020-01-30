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
netwib_err netwox_ftpcli_user(netwib_io *pio,
                              netwib_constbuf *puser)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));
  buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE;

  netwib_er(netwib_buf_append_string("USER ", &buf));
  netwib_er(netwib_buf_append_buf(puser, &buf));
  netwib_er(netwib_buf_append_string("\r\n", &buf));

  ret = netwib_io_write(pio, &buf);

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpcli_pass(netwib_io *pio,
                              netwib_constbuf *ppassword)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));
  buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE;

  netwib_er(netwib_buf_append_string("PASS ", &buf));
  netwib_er(netwib_buf_append_buf(ppassword, &buf));
  netwib_er(netwib_buf_append_string("\r\n", &buf));

  ret = netwib_io_write(pio, &buf);

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpcli_quit(netwib_io *pio)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_ext_string("QUIT\r\n", &buf));
  netwib_er(netwib_io_write(pio, &buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpcli_port(netwib_io *pio,
                              netwib_constip *pip,
                              netwib_port port)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_err ret;

  if (pip->iptype != NETWIB_IPTYPE_IP4) {
    return(NETWOX_ERR_NOTIMPLEMENTED);
  }

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));

  netwib_er(netwib_buf_append_fmt(&buf, "PORT %{uint8},%{uint8},%{uint8},%{uint8},%{uint8},%{uint8}\r\n", netwib_c2_uint32_0(pip->ipvalue.ip4), netwib_c2_uint32_1(pip->ipvalue.ip4), netwib_c2_uint32_2(pip->ipvalue.ip4), netwib_c2_uint32_3(pip->ipvalue.ip4), netwib_c2_uint16_0(port), netwib_c2_uint16_1(port)));

  ret = netwib_io_write(pio, &buf);

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpcli_pasv(netwib_io *pio)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_ext_string("PASV\r\n", &buf));
  netwib_er(netwib_io_write(pio, &buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpcli_eprt(netwib_io *pio,
                              netwib_constip *pip,
                              netwib_port port)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));

  if (pip->iptype == NETWIB_IPTYPE_IP4) {
    netwib_er(netwib_buf_append_fmt(&buf, "EPRT |1|%{ip}|%{port}|\r\n",
                                    pip, port));
  } else if (pip->iptype == NETWIB_IPTYPE_IP6) {
    netwib_er(netwib_buf_append_fmt(&buf, "EPRT |2|%{ip}|%{port}|\r\n",
                                    pip, port));
  } else {
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  ret = netwib_io_write(pio, &buf);

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpcli_epsv(netwib_io *pio)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_ext_string("EPSV\r\n", &buf));
  netwib_er(netwib_io_write(pio, &buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpcli_retr(netwib_io *pio,
                              netwib_constbuf *pfilename)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));

  netwib_er(netwib_buf_append_string("RETR ", &buf));
  netwib_er(netwib_buf_append_buf(pfilename, &buf));
  netwib_er(netwib_buf_append_string("\r\n", &buf));

  ret = netwib_io_write(pio, &buf);

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpcli_stor(netwib_io *pio,
                              netwib_constbuf *pfilename)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));

  netwib_er(netwib_buf_append_string("STOR ", &buf));
  netwib_er(netwib_buf_append_buf(pfilename, &buf));
  netwib_er(netwib_buf_append_string("\r\n", &buf));

  ret = netwib_io_write(pio, &buf);

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpcli_dele(netwib_io *pio,
                              netwib_constbuf *pfilename)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));

  netwib_er(netwib_buf_append_string("DELE ", &buf));
  netwib_er(netwib_buf_append_buf(pfilename, &buf));
  netwib_er(netwib_buf_append_string("\r\n", &buf));

  ret = netwib_io_write(pio, &buf);

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpcli_size(netwib_io *pio,
                              netwib_constbuf *pfilename)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));

  netwib_er(netwib_buf_append_string("SIZE ", &buf));
  netwib_er(netwib_buf_append_buf(pfilename, &buf));
  netwib_er(netwib_buf_append_string("\r\n", &buf));

  ret = netwib_io_write(pio, &buf);

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpcli_type(netwib_io *pio,
                              netwib_conststring type)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));

  netwib_er(netwib_buf_append_string("TYPE ", &buf));
  netwib_er(netwib_buf_append_string(type, &buf));
  netwib_er(netwib_buf_append_string("\r\n", &buf));

  ret = netwib_io_write(pio, &buf);

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpcli_mkd(netwib_io *pio,
                             netwib_constbuf *pdirname)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));

  netwib_er(netwib_buf_append_string("MKD ", &buf));
  netwib_er(netwib_buf_append_buf(pdirname, &buf));
  netwib_er(netwib_buf_append_string("\r\n", &buf));

  ret = netwib_io_write(pio, &buf);

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpcli_rmd(netwib_io *pio,
                             netwib_constbuf *pdirname)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));

  netwib_er(netwib_buf_append_string("RMD ", &buf));
  netwib_er(netwib_buf_append_buf(pdirname, &buf));
  netwib_er(netwib_buf_append_string("\r\n", &buf));

  ret = netwib_io_write(pio, &buf);

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpcli_cwd(netwib_io *pio,
                             netwib_constbuf *pdirname)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));

  netwib_er(netwib_buf_append_string("CWD ", &buf));
  netwib_er(netwib_buf_append_buf(pdirname, &buf));
  netwib_er(netwib_buf_append_string("\r\n", &buf));

  ret = netwib_io_write(pio, &buf);

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpcli_list(netwib_io *pio,
                              netwib_constbuf *pdirname)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));

  netwib_er(netwib_buf_append_string("LIST ", &buf));
  netwib_er(netwib_buf_append_buf(pdirname, &buf));
  netwib_er(netwib_buf_append_string("\r\n", &buf));

  ret = netwib_io_write(pio, &buf);

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}
