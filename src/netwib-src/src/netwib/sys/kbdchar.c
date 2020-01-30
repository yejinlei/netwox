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
netwib_err netwib_char_init_kbd(netwib_constbuf *pmessage,
                                netwib_constbuf *pallowedchars,
                                netwib_char defaultchar,
                                netwib_char *pchar)
{
  netwib_char c, prompt;
  netwib_bool havemessage;
  netwib_priv_kbd kbd;

  /* parameters verification */
  if (netwib__buf_ref_data_sizenull(pallowedchars) &&
      defaultchar != NETWIB_CHAR_INIT_KBD_NODEF) {
    if (!netwib_c_memchr(netwib__buf_ref_data_ptr(pallowedchars), defaultchar, netwib__buf_ref_data_size(pallowedchars))) {
      return(NETWIB_ERR_PAINVALIDDEFAULT);
    }
  }

  havemessage = NETWIB_FALSE;
  if (netwib__buf_ref_data_sizenull(pmessage) != 0) {
    havemessage = NETWIB_TRUE;
  }

  /* ask user */
  c = 0;
  prompt = ':';
  netwib_er(netwib_priv_kbd_initdefault(&kbd));
  netwib_er(netwib_priv_kbd_ctl_set_echoline(&kbd, NETWIB_TRUE, NETWIB_FALSE));
  while (NETWIB_TRUE) {
    if (havemessage) {
      if (netwib__buf_ref_data_sizenull(pallowedchars) &&
          defaultchar != NETWIB_CHAR_INIT_KBD_NODEF) {
        netwib_er(netwib_fmt_display("%{buf} (key in %{buf})[%c]%c ", pmessage,
                                     pallowedchars, defaultchar, prompt));
      } else if (netwib__buf_ref_data_sizenull(pallowedchars)) {
        netwib_er(netwib_fmt_display("%{buf} (key in %{buf})%c ", pmessage,
                                     pallowedchars, prompt));
      } else if (defaultchar != NETWIB_CHAR_INIT_KBD_NODEF) {
        netwib_er(netwib_fmt_display("%{buf} [%c]%c ", pmessage,
                                     defaultchar, prompt));
      } else {
        netwib_er(netwib_fmt_display("%{buf}%c ", pmessage, prompt));
      }
    }
    netwib_er(netwib_priv_kbd_read_key(&kbd, &c));
    if (c == 0x0D || c == 0x0A) {
      if (defaultchar != NETWIB_CHAR_INIT_KBD_NODEF) {
        c = defaultchar;
        break;
      }
    } else {
      if (havemessage) {
        netwib_er(netwib_fmt_display("\n"));
      }
    }
    if (netwib__buf_ref_data_sizenull(pallowedchars) == 0) {
      break;
    }
    if (netwib_c_memchr(netwib__buf_ref_data_ptr(pallowedchars), c,
                        netwib__buf_ref_data_size(pallowedchars))) {
      break;
    }
    prompt = '>';
  }
  netwib_er(netwib_priv_kbd_close(&kbd));

  if (pchar != NULL) *pchar = c;
  return(NETWIB_ERR_OK);
}
