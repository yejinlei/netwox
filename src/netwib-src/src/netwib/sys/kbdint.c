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
netwib_err netwib_uint32_init_kbd(netwib_constbuf *pmessage,
                                  netwib_uint32 min,
                                  netwib_uint32 max,
                                  netwib_uint32 defaultnumber,
                                  netwib_uint32 *pnumber)
{
  netwib_buf buf;
  netwib_char prompt;
  netwib_bool havemessage;
  netwib_uint32 number=0;
  netwib_priv_kbd kbd;
  netwib_err ret;

  /* parameters verification */
  if (min > max) {
    return(NETWIB_ERR_PAINVALIDRANGE);
  }
  if (defaultnumber != NETWIB_UINT32_INIT_KBD_NODEF) {
    if (defaultnumber > max) return(NETWIB_ERR_PAINVALIDDEFAULT);
    if (defaultnumber < min) return(NETWIB_ERR_PAINVALIDDEFAULT);
  }

  havemessage = NETWIB_FALSE;
  if (netwib__buf_ref_data_sizenull(pmessage) != 0) {
    havemessage = NETWIB_TRUE;
  }

  /* ask user */
  prompt = ':';
  netwib_er(netwib_priv_kbd_initdefault(&kbd));
  netwib_er(netwib_priv_kbd_ctl_set_echoline(&kbd, NETWIB_TRUE, NETWIB_TRUE));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  while (NETWIB_TRUE) {
    if (havemessage) {
      netwib_er(netwib_fmt_display("%{buf}", pmessage));
      if (min != NETWIB_UINT32_INIT_KBD_NOMIN ||
          max != NETWIB_UINT32_INIT_KBD_NOMAX ||
          defaultnumber != NETWIB_UINT32_INIT_KBD_NODEF) {
        netwib_er(netwib_fmt_display(" "));
        if (min != NETWIB_UINT32_INIT_KBD_NOMIN ||
            max != NETWIB_UINT32_INIT_KBD_NOMAX) {
          netwib_er(netwib_fmt_display("(between %{uint32} and %{uint32})",
                                        min, max));
        }
        if (defaultnumber != NETWIB_UINT32_INIT_KBD_NODEF) {
          netwib_er(netwib_fmt_display("[%{uint32}]", defaultnumber));
        }
      }
      netwib_er(netwib_fmt_display("%c ", prompt));
    }
    netwib_er(netwib_priv_kbd_read_line(&kbd, &buf));
    if (netwib__buf_ref_data_size(&buf) == 0) {
      if (defaultnumber != NETWIB_UINT32_INIT_KBD_NODEF) {
        number = defaultnumber;
        break;
      }
    } else {
      ret = netwib_buf_decode_fmt(&buf, "%{uint32}%$", &number);
      if (ret == NETWIB_ERR_OK) {
        if ((number >= min) && (number <= max)) {
          break;
        }
      }
    }
    prompt = '>';
    netwib__buf_reinit(&buf);
  }
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_priv_kbd_close(&kbd));

  if (pnumber != NULL) *pnumber = number;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_uint64_init_kbd(netwib_constbuf *pmessage,
                                  netwib_uint64 defaultnumber,
                                  netwib_uint64 *pnumber)
{
  netwib_buf buf;
  netwib_char prompt;
  netwib_bool havemessage;
  netwib_uint64 number;
  netwib_priv_kbd kbd;
  netwib_err ret;

  netwib__uint64_init_uint32(0, number);
  havemessage = NETWIB_FALSE;
  if (netwib__buf_ref_data_sizenull(pmessage) != 0) {
    havemessage = NETWIB_TRUE;
  }

  /* ask user */
  prompt = ':';
  netwib_er(netwib_priv_kbd_initdefault(&kbd));
  netwib_er(netwib_priv_kbd_ctl_set_echoline(&kbd, NETWIB_TRUE, NETWIB_TRUE));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  while (NETWIB_TRUE) {
    if (havemessage) {
      netwib_er(netwib_fmt_display("%{buf}", pmessage));
      netwib_er(netwib_fmt_display("%c ", prompt));
    }
    netwib_er(netwib_priv_kbd_read_line(&kbd, &buf));
    if (netwib__buf_ref_data_size(&buf) == 0) {
      number = defaultnumber;
      break;
    } else {
      ret = netwib_buf_decode_fmt(&buf, "%{uint64}%$", &number);
      if (ret == NETWIB_ERR_OK) {
        break;
      }
    }
    prompt = '>';
    netwib__buf_reinit(&buf);
  }
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_priv_kbd_close(&kbd));

  if (pnumber != NULL) *pnumber = number;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_int32_init_kbd(netwib_constbuf *pmessage,
                                 netwib_int32 min,
                                 netwib_int32 max,
                                 netwib_int32 defaultnumber,
                                 netwib_int32 *pnumber)
{
  netwib_buf buf;
  netwib_char prompt;
  netwib_bool havemessage;
  netwib_int32 number=0;
  netwib_priv_kbd kbd;
  netwib_err ret;

  /* parameters verification */
  if (min > max) {
    return(NETWIB_ERR_PAINVALIDRANGE);
  }
  if (defaultnumber != NETWIB_INT32_INIT_KBD_NODEF) {
    if (defaultnumber > max) return(NETWIB_ERR_PAINVALIDDEFAULT);
    if (defaultnumber < min) return(NETWIB_ERR_PAINVALIDDEFAULT);
  }

  havemessage = NETWIB_FALSE;
  if (netwib__buf_ref_data_sizenull(pmessage) != 0) {
    havemessage = NETWIB_TRUE;
  }

  /* ask user */
  prompt = ':';
  netwib_er(netwib_priv_kbd_initdefault(&kbd));
  netwib_er(netwib_priv_kbd_ctl_set_echoline(&kbd, NETWIB_TRUE, NETWIB_TRUE));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  while (NETWIB_TRUE) {
    if (havemessage) {
      netwib_er(netwib_fmt_display("%{buf}", pmessage));
      if (min != NETWIB_INT32_INIT_KBD_NOMIN ||
          max != NETWIB_INT32_INIT_KBD_NOMAX ||
          defaultnumber != NETWIB_INT32_INIT_KBD_NODEF) {
        netwib_er(netwib_fmt_display(" "));
        if (min != NETWIB_INT32_INIT_KBD_NOMIN ||
            max != NETWIB_INT32_INIT_KBD_NOMAX) {
          netwib_er(netwib_fmt_display("(between %{int32} and %{int32})",
                                     min, max));
        }
        if (defaultnumber != NETWIB_INT32_INIT_KBD_NODEF) {
          netwib_er(netwib_fmt_display("[%{int32}]", defaultnumber));
        }
      }
      netwib_er(netwib_fmt_display("%c ", prompt));
    }
    netwib_er(netwib_priv_kbd_read_line(&kbd, &buf));
    if (netwib__buf_ref_data_size(&buf) == 0) {
      if (defaultnumber != NETWIB_INT32_INIT_KBD_NODEF) {
        number = defaultnumber;
        break;
      }
    } else {
      ret = netwib_buf_decode_fmt(&buf, "%{int32}%$", &number);
      if (ret == NETWIB_ERR_OK) {
        if ((number >= min) && (number <= max)) {
          break;
        }
      }
    }
    prompt = '>';
    netwib__buf_reinit(&buf);
  }
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_priv_kbd_initdefault(&kbd));

  if (pnumber != NULL) *pnumber = number;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_int64_init_kbd(netwib_constbuf *pmessage,
                                 netwib_int64 defaultnumber,
                                 netwib_int64 *pnumber)
{
  netwib_buf buf;
  netwib_char prompt;
  netwib_bool havemessage;
  netwib_int64 number;
  netwib_priv_kbd kbd;
  netwib_err ret;

  netwib__int64_init_int32(0, number);
  havemessage = NETWIB_FALSE;
  if (netwib__buf_ref_data_sizenull(pmessage) != 0) {
    havemessage = NETWIB_TRUE;
  }

  /* ask user */
  prompt = ':';
  netwib_er(netwib_priv_kbd_initdefault(&kbd));
  netwib_er(netwib_priv_kbd_ctl_set_echoline(&kbd, NETWIB_TRUE, NETWIB_TRUE));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  while (NETWIB_TRUE) {
    if (havemessage) {
      netwib_er(netwib_fmt_display("%{buf}", pmessage));
      netwib_er(netwib_fmt_display("%c ", prompt));
    }
    netwib_er(netwib_priv_kbd_read_line(&kbd, &buf));
    if (netwib__buf_ref_data_size(&buf) == 0) {
      number = defaultnumber;
      break;
    } else {
      ret = netwib_buf_decode_fmt(&buf, "%{int64}%$", &number);
      if (ret == NETWIB_ERR_OK) {
        break;
      }
    }
    prompt = '>';
    netwib__buf_reinit(&buf);
  }
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_priv_kbd_close(&kbd));

  if (pnumber != NULL) *pnumber = number;
  return(NETWIB_ERR_OK);
}

