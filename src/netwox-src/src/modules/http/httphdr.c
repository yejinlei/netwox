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
netwib_err netwox_httphdr_methoduri(netwox_httphdr_method method,
                                    netwib_constbuf *puri,
                                    netwib_buf *pbuf)
{
  netwib_conststring str=NULL;

  switch(method) {
  case NETWOX_HTTPHDR_METHOD_GET : str = "GET"; break;
  case NETWOX_HTTPHDR_METHOD_HEAD : str = "HEAD"; break;
  case NETWOX_HTTPHDR_METHOD_POST : str = "POST"; break;
  case NETWOX_HTTPHDR_METHOD_PUT : str = "PUT"; break;
  case NETWOX_HTTPHDR_METHOD_DELETE : str = "DELETE"; break;
  case NETWOX_HTTPHDR_METHOD_TRACE : str = "TRACE"; break;
  case NETWOX_HTTPHDR_METHOD_OPTIONS : str = "OPTIONS"; break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  netwib_er(netwib_buf_append_string(str, pbuf));
  netwib_er(netwib_buf_append_byte(' ', pbuf));
  netwib_er(netwib_buf_append_buf(puri, pbuf));
  netwib_er(netwib_buf_append_string(" HTTP/1.0\r\n", pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_httphdr_host(netwib_constbuf *phost,
                               netwib_port port,
                               netwib_buf *pbuf)
{
  netwib_ip ipad;
  netwib_err ret;

  netwib_er(netwib_buf_append_string("Host: ", pbuf));

  ret = netwib_ip_init_buf(phost, NETWIB_IP_DECODETYPE_IP6, &ipad);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_append_byte('[', pbuf));
    netwib_er(netwib_buf_append_ip(&ipad, NETWIB_IP_ENCODETYPE_IP, pbuf));
    netwib_er(netwib_buf_append_byte(']', pbuf));
  } else {
    netwib_er(netwib_buf_append_buf(phost, pbuf));
  }

  if (port != NETWOX_SERVICES_HTTP) {
    netwib_er(netwib_buf_append_fmt(pbuf, ":%{port}", port));
  }

  netwib_er(netwib_buf_append_string("\r\n", pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_httphdr_authbasic(netwib_constbuf *plogin,
                                    netwib_constbuf *ppassword,
                                    netwib_buf *pbuf)
{
  netwib_byte array[128];
  netwib_buf buf;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));

  netwib_er(netwib_buf_append_buf(plogin, &buf));
  netwib_er(netwib_buf_append_byte(':', &buf));
  netwib_er(netwib_buf_append_buf(ppassword, &buf));

  netwib_er(netwib_buf_append_string("Authorization: Basic ", pbuf));
  netwib_er(netwib_buf_encode(&buf, NETWIB_ENCODETYPE_BASE64, pbuf));
  netwib_er(netwib_buf_append_string("\r\n", pbuf));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_httphdr_proxyauthbasic(netwib_constbuf *plogin,
                                         netwib_constbuf *ppassword,
                                         netwib_buf *pbuf)
{
  netwib_byte array[128];
  netwib_buf buf;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));

  netwib_er(netwib_buf_append_buf(plogin, &buf));
  netwib_er(netwib_buf_append_byte(':', &buf));
  netwib_er(netwib_buf_append_buf(ppassword, &buf));

  netwib_er(netwib_buf_append_string("Proxy-Authorization: Basic ", pbuf));
  netwib_er(netwib_buf_encode(&buf, NETWIB_ENCODETYPE_BASE64, pbuf));
  netwib_er(netwib_buf_append_string("\r\n", pbuf));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_httphdr_useragent(netwib_constbuf *pua,
                                    netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_string("User-Agent: ", pbuf));
  netwib_er(netwib_buf_append_buf(pua, pbuf));
  netwib_er(netwib_buf_append_string("\r\n", pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_httphdr_endheader(netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_string("\r\n", pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_httphdr_status(netwox_httphdr_statuscode statuscode,
                                 netwib_constbuf *pstatusmessage,
                                 netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_fmt(pbuf, "HTTP/1.0 %{uint32} %{buf}\r\n",
                                  statuscode, pstatusmessage));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_httphdr_connection(netwib_conststring type,
                                     netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_string("Connection: ", pbuf));
  netwib_er(netwib_buf_append_string(type, pbuf));
  netwib_er(netwib_buf_append_string("\r\n", pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_httphdr_contenttype(netwib_conststring type,
                                      netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_string("Content-Type: ", pbuf));
  netwib_er(netwib_buf_append_string(type, pbuf));
  netwib_er(netwib_buf_append_string("\r\n", pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_httphdr_contentlength(netwib_uint32 len,
                                        netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_fmt(pbuf, "Content-Length: %{uint32}\r\n", len));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_httphdr_wwwauth(netwib_constbuf *prealm,
                                  netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_string("WWW-Authenticate: Basic realm=\"", pbuf));
  netwib_er(netwib_buf_append_buf(prealm, pbuf));
  netwib_er(netwib_buf_append_string("\"\r\n", pbuf));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
netwib_err netwox_httphdr_location(netwib_constbuf *plocation,
                                   netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_string("Location: ", pbuf));
  netwib_er(netwib_buf_append_buf(plocation, pbuf));
  netwib_er(netwib_buf_append_string("\r\n", pbuf));

  return(NETWIB_ERR_OK);
}
