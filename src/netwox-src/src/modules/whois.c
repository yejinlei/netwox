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
netwib_err netwox_whois_chooseserver(netwib_constbuf *pquery,
                                     netwib_bool verbose,
                                     netwib_ip *pserverip)
{
  netwib_byte array[80];
  netwib_buf server, found, re;
  netwib_ip ipad;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &server));

  ret = netwib_ip_init_buf(pquery, NETWIB_IP_DECODETYPE_IP4, &ipad);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_append_string("whois.arin.net", &server));
  } else {
    ret = netwib_ip_init_buf(pquery, NETWIB_IP_DECODETYPE_IP6, &ipad);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_buf_append_string("whois.6bone.net", &server));
    } else {
      netwib_er(netwib_buf_init_ext_string("[.][a-z][a-z]$", &re));
      ret = netwib_buf_search_re(pquery, &re, NETWIB_FALSE, &found);
      if (ret != NETWIB_ERR_OK) {
        netwib_er(netwib_buf_init_ext_string("[.][a-z][a-z][a-z]$", &re));
        ret = netwib_buf_search_re(pquery, &re, NETWIB_FALSE, &found);
      }
      if (ret != NETWIB_ERR_OK) {
        netwib_er(netwib_buf_append_string("whois.internic.net", &server));
      } else {
        found.beginoffset++;
        netwib_er(netwib_buf_append_buf(&found, &server));
        netwib_er(netwib_buf_append_string(".whois-servers.net", &server));
      }
    }
  }

  if (verbose) {
    netwib_er(netwib_fmt_display("Using %{buf} server.\n", &server));
    netwib_er(netwib_fmt_display("You may have to refine your query using another server (tool 196).\n"));
  }

  netwib_er(netwib_ip_init_buf_best(&server, pserverip));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_whois_query(netwib_io *pio,
                              netwib_constbuf *pquery,
                              netwib_uint32 maxwaitms)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));
  netwib_er(netwib_buf_append_buf(pquery, &buf));
  netwib_er(netwib_buf_append_string("\r\n", &buf));

  ret = netwib_io_write(pio, &buf);
  if (ret == NETWIB_ERR_OK) {
    ret = netwox_io_showecho(pio, maxwaitms, NETWIB_TRUE, NETWIB_FALSE);
  }

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}
