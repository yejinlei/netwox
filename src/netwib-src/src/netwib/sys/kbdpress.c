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
netwib_err netwib_kbd_press(netwib_constbuf *pmessage,
                            netwib_char *pchar)
{
  netwib_priv_kbd kbd;

  if (pmessage != NULL) {
    netwib_er(netwib_fmt_display("%{buf} ", pmessage));
  }

  netwib_er(netwib_priv_kbd_initdefault(&kbd));
  netwib_er(netwib_priv_kbd_press(&kbd, pchar));
  netwib_er(netwib_priv_kbd_close(&kbd));

  if (pmessage != NULL) {
    netwib_er(netwib_fmt_display("\n"));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_kbd_purge(void)
{
  netwib_priv_kbd kbd;

  netwib_er(netwib_priv_kbd_initdefault(&kbd));
  netwib_er(netwib_priv_kbd_ctl_set_purge(&kbd));
  netwib_er(netwib_priv_kbd_close(&kbd));

  return(NETWIB_ERR_OK);
}
