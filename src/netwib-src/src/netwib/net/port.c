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
netwib_err netwib_port_init_kbd(netwib_constbuf *pmessage,
                                netwib_port min,
                                netwib_port max,
                                netwib_port defaultport,
                                netwib_port *pport)
{
  netwib_uint32 ui;

  /* Values NETWIB_PORT_INIT_KBD_NOMIN, NOMAX and NODEF are the same
     as NETWIB_UINT32_INIT_KBD_NOMIN, etc. so there
     is no conversion to do */
  netwib_er(netwib_uint32_init_kbd(pmessage, min, max, defaultport, &ui));

  if (pport != NULL) *pport = ui;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_port_init_buf(netwib_constbuf *pbuf,
                                netwib_port *pport)
{
  netwib_string pstrint;
  unsigned long number;
  char *p;

  /* obtain a reference to the string */
  netwib__constbuf_ref_string(pbuf, pstrint, bufstorage,
                              netwib_port_init_buf(&bufstorage, pport));

  /* special case */
  if (*pstrint == '\0') {
    return(NETWIB_ERR_PAINT);
  }

  /* find number */
  number = strtoul(pstrint, &p, 10);
  if (*p != '\0') {
    return(NETWIB_ERR_PAINT);
  }

  /* number is too big */
  if (number == ULONG_MAX && errno == ERANGE) {
    errno = 0;
    return(NETWIB_ERR_PATOOHIGH);
  }
  if (number > 0xFFFF) {
    return(NETWIB_ERR_PATOOHIGH);
  }

  if (pport != NULL)
    *pport = number;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_port(netwib_port port,
                                  netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_fmt(pbuf, "%{port}", port));
  return(NETWIB_ERR_OK);
}
