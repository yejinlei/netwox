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
netwib_err netwib_buf_append_encodetype(netwib_encodetype encodetype,
                                        netwib_buf *pbuf)
{
  netwib_conststring pc;

  pc = NULL;

  switch (encodetype) {
  case NETWIB_ENCODETYPE_DATA:
    pc = "exact data";
    break;
  case NETWIB_ENCODETYPE_HEXA0:
    pc = "hexadecimal (not spaced)";
    break;
  case NETWIB_ENCODETYPE_HEXA1:
    pc = "hexadecimal";
    break;
  case NETWIB_ENCODETYPE_HEXA2:
    pc = "hexadecimal (spaced every two byte)";
    break;
  case NETWIB_ENCODETYPE_HEXA4:
    pc = "hexadecimal (spaced every four byte)";
    break;
  case NETWIB_ENCODETYPE_MIXED0:
    pc = "mixed (not spaced)";
    break;
  case NETWIB_ENCODETYPE_MIXED1:
    pc = "mixed";
    break;
  case NETWIB_ENCODETYPE_TEXT:
    pc = "printable text";
    break;
  case NETWIB_ENCODETYPE_BASE64:
    pc = "base64";
    break;
  case NETWIB_ENCODETYPE_QUOTED:
    pc = "quoted";
    break;
  case NETWIB_ENCODETYPE_NOTHING:
    pc = "print nothing";
    break;
  case NETWIB_ENCODETYPE_SYNTH:
    pc = "print a synthetic form";
    break;
  case NETWIB_ENCODETYPE_DATA_WRAP:
    pc = "wrapped exact data";
    break;
  case NETWIB_ENCODETYPE_HEXA0_WRAP:
    pc = "wrapped hexa (not spaced, 32 bytes per line)";
    break;
  case NETWIB_ENCODETYPE_HEXA1_WRAP:
    pc = "wrapped hexa";
    break;
  case NETWIB_ENCODETYPE_HEXA2_WRAP:
    pc = "wrapped hexa (spaced 2, 32 bytes per line)";
    break;
  case NETWIB_ENCODETYPE_HEXA4_WRAP:
    pc = "wrapped hexa (spaced 4, 32 bytes per line)";
    break;
  case NETWIB_ENCODETYPE_MIXED0_WRAP:
    pc = "wrapped mixed (not spaced, 16 bytes per line)";
    break;
  case NETWIB_ENCODETYPE_MIXED1_WRAP:
    pc = "wrapped mixed";
    break;
  case NETWIB_ENCODETYPE_TEXT_WRAP:
    pc = "wrapped printable text";
    break;
  case NETWIB_ENCODETYPE_BASE64_WRAP:
    pc = "wrapped base64";
    break;
  case NETWIB_ENCODETYPE_ARRAY1:
    pc = "array (32 bit per line)";
    break;
  case NETWIB_ENCODETYPE_ARRAY4:
    pc = "array (8 quartets per line)";
    break;
  case NETWIB_ENCODETYPE_ARRAY8:
    pc = "array (4 bytes per line)";
    break;
  case NETWIB_ENCODETYPE_ARRAY16:
    pc = "array (2 doublebyte(word) per line)";
    break;
  case NETWIB_ENCODETYPE_ARRAY32:
    pc = "array (1 doubleword per line)";
    break;
  case NETWIB_ENCODETYPE_DUMP:
    pc = "dump";
    break;
  case NETWIB_ENCODETYPE_LOWERCASE:
    pc = "lower case";
    break;
  case NETWIB_ENCODETYPE_UPPERCASE:
    pc = "upper case";
    break;
  case NETWIB_ENCODETYPE_MIXED0H_WRAP:
    pc = "wrapped mixed, with commented hexa";
    break;
  case NETWIB_ENCODETYPE_MIXED1H_WRAP:
    pc = "wrapped mixed, with commented hexa";
    break;
  case NETWIB_ENCODETYPE_TRANSITION_INIT:
    pc = "transition initialization";
    break;
  case NETWIB_ENCODETYPE_TRANSITION_END:
    pc = "transition end";
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  netwib_er(netwib_buf_append_string(pc, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
#define NETWIB_PRIV_ENCODETYPE_MAX 40
netwib_err netwib_encodetype_init_kbd(netwib_constbuf *pmessage,
                                      netwib_bool displayonlymostuseful,
                                      netwib_encodetype defaultencodetype,
                                      netwib_encodetype *pencodetype)
{
  netwib_encodetype array[NETWIB_PRIV_ENCODETYPE_MAX];
  netwib_buf msg, buf;
  netwib_uint32 i, choice, defaultchoice;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  if (pmessage != NULL) {
    netwib_er(netwib_buf_append_buf(pmessage, &buf));
    netwib_er(netwib_buf_append_string("\n", &buf));
  }

  i = 0;
  defaultchoice = NETWIB_UINT32_INIT_KBD_NODEF;

#define netwib_encodetype_li(ot) {netwib_er(netwib_buf_append_fmt(&buf, " %{r 2;uint32} - ", i)); netwib_er(netwib_buf_append_encodetype(ot, &buf)); netwib_er(netwib_buf_append_fmt(&buf, "\n")); if (defaultencodetype == ot) defaultchoice = i; array[i++] = ot; }

  if (displayonlymostuseful) {
    netwib_encodetype_li(NETWIB_ENCODETYPE_DUMP);
    netwib_encodetype_li(NETWIB_ENCODETYPE_HEXA1);
    netwib_encodetype_li(NETWIB_ENCODETYPE_HEXA1_WRAP);
    netwib_encodetype_li(NETWIB_ENCODETYPE_MIXED1);
    netwib_encodetype_li(NETWIB_ENCODETYPE_MIXED1_WRAP);
    netwib_encodetype_li(NETWIB_ENCODETYPE_MIXED1H_WRAP);
    netwib_encodetype_li(NETWIB_ENCODETYPE_NOTHING);
    netwib_encodetype_li(NETWIB_ENCODETYPE_SYNTH);
    netwib_encodetype_li(NETWIB_ENCODETYPE_DATA);
    netwib_encodetype_li(NETWIB_ENCODETYPE_DATA_WRAP);
    netwib_encodetype_li(NETWIB_ENCODETYPE_TEXT);
    netwib_encodetype_li(NETWIB_ENCODETYPE_TEXT_WRAP);
    netwib_encodetype_li(NETWIB_ENCODETYPE_BASE64);
    netwib_encodetype_li(NETWIB_ENCODETYPE_QUOTED);
    netwib_encodetype_li(NETWIB_ENCODETYPE_ARRAY8);
  } else {
    netwib_encodetype_li(NETWIB_ENCODETYPE_DUMP);
    netwib_encodetype_li(NETWIB_ENCODETYPE_HEXA0);
    netwib_encodetype_li(NETWIB_ENCODETYPE_HEXA1);
    netwib_encodetype_li(NETWIB_ENCODETYPE_HEXA2);
    netwib_encodetype_li(NETWIB_ENCODETYPE_HEXA4);
    netwib_encodetype_li(NETWIB_ENCODETYPE_HEXA0_WRAP);
    netwib_encodetype_li(NETWIB_ENCODETYPE_HEXA1_WRAP);
    netwib_encodetype_li(NETWIB_ENCODETYPE_HEXA2_WRAP);
    netwib_encodetype_li(NETWIB_ENCODETYPE_HEXA4_WRAP);
    netwib_encodetype_li(NETWIB_ENCODETYPE_MIXED0);
    netwib_encodetype_li(NETWIB_ENCODETYPE_MIXED1);
    netwib_encodetype_li(NETWIB_ENCODETYPE_MIXED0_WRAP);
    netwib_encodetype_li(NETWIB_ENCODETYPE_MIXED1_WRAP);
    netwib_encodetype_li(NETWIB_ENCODETYPE_MIXED0H_WRAP);
    netwib_encodetype_li(NETWIB_ENCODETYPE_MIXED1H_WRAP);
    netwib_encodetype_li(NETWIB_ENCODETYPE_NOTHING);
    netwib_encodetype_li(NETWIB_ENCODETYPE_SYNTH);
    netwib_encodetype_li(NETWIB_ENCODETYPE_DATA);
    netwib_encodetype_li(NETWIB_ENCODETYPE_DATA_WRAP);
    netwib_encodetype_li(NETWIB_ENCODETYPE_TEXT);
    netwib_encodetype_li(NETWIB_ENCODETYPE_TEXT_WRAP);
    netwib_encodetype_li(NETWIB_ENCODETYPE_BASE64);
    netwib_encodetype_li(NETWIB_ENCODETYPE_BASE64_WRAP);
    netwib_encodetype_li(NETWIB_ENCODETYPE_QUOTED);
    netwib_encodetype_li(NETWIB_ENCODETYPE_ARRAY1);
    netwib_encodetype_li(NETWIB_ENCODETYPE_ARRAY4);
    netwib_encodetype_li(NETWIB_ENCODETYPE_ARRAY8);
    netwib_encodetype_li(NETWIB_ENCODETYPE_ARRAY16);
    netwib_encodetype_li(NETWIB_ENCODETYPE_ARRAY32);
    netwib_encodetype_li(NETWIB_ENCODETYPE_LOWERCASE);
    netwib_encodetype_li(NETWIB_ENCODETYPE_UPPERCASE);
  }

  if (i >= NETWIB_PRIV_ENCODETYPE_MAX) {
    return(NETWIB_ERR_LOINTERNALERROR);
  }
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_buf_close(&buf));

  if (defaultencodetype == NETWIB_ENCODETYPE_INIT_KBD_NODEF) {
    defaultchoice = NETWIB_UINT32_INIT_KBD_NODEF;
  }

  netwib_er(netwib_buf_init_ext_string("Your choice", &msg));
  netwib_er(netwib_uint32_init_kbd(&msg, 0, i-1, defaultchoice,
                                 &choice));

  if (pencodetype != NULL) *pencodetype = array[choice];
  return(NETWIB_ERR_OK);
}
