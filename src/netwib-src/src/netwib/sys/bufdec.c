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
netwib_err netwib_buf_append_decodetype(netwib_decodetype decodetype,
                                        netwib_buf *pbuf)
{
  netwib_conststring pc;

  pc = NULL;

  switch (decodetype) {
  case NETWIB_DECODETYPE_DATA:
    pc = "exact data";
    break;
  case NETWIB_DECODETYPE_HEXA:
    pc = "hexadecimal";
    break;
  case NETWIB_DECODETYPE_MIXED:
    pc = "mixed";
    break;
  case NETWIB_DECODETYPE_BASE64:
    pc = "base64";
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  netwib_er(netwib_buf_append_string(pc, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
#define NETWIB_PRIV_DECODETYPE_MAX 10
netwib_err netwib_decodetype_init_kbd(netwib_constbuf *pmessage,
                                      netwib_decodetype defaultdecodetype,
                                      netwib_decodetype *pdecodetype)
{
  netwib_decodetype array[NETWIB_PRIV_DECODETYPE_MAX];
  netwib_buf msg, buf;
  netwib_uint32 i, choice, defaultchoice;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  if (pmessage != NULL) {
    netwib_er(netwib_buf_append_buf(pmessage, &buf));
    netwib_er(netwib_buf_append_string("\n", &buf));
  }

  i = 0;
  defaultchoice = NETWIB_UINT32_INIT_KBD_NODEF;

#define netwib_decodetype_li(ot) {netwib_er(netwib_buf_append_fmt(&buf, " %{r 2;uint32} - ", i)); netwib_er(netwib_buf_append_decodetype(ot, &buf)); netwib_er(netwib_buf_append_fmt(&buf, "\n")); if (defaultdecodetype == ot) defaultchoice = i; array[i++] = ot; }

  netwib_decodetype_li(NETWIB_DECODETYPE_DATA);
  netwib_decodetype_li(NETWIB_DECODETYPE_HEXA);
  netwib_decodetype_li(NETWIB_DECODETYPE_MIXED);
  netwib_decodetype_li(NETWIB_DECODETYPE_BASE64);

  if (i >= NETWIB_PRIV_DECODETYPE_MAX) {
    return(NETWIB_ERR_LOINTERNALERROR);
  }
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_buf_close(&buf));

  if (defaultdecodetype == NETWIB_DECODETYPE_INIT_KBD_NODEF) {
    defaultchoice = NETWIB_UINT32_INIT_KBD_NODEF;
  }

  netwib_er(netwib_buf_init_ext_string("Your choice", &msg));
  netwib_er(netwib_uint32_init_kbd(&msg, 0, i-1, defaultchoice, &choice));

  if (pdecodetype != NULL) *pdecodetype = array[choice];
  return(NETWIB_ERR_OK);
}
