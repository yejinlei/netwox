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
netwib_err netwox_io_kbd_loop(netwib_io *pio,
                              netwib_bool linebyline,
                              netwib_bool echochars,
                              netwib_decodetype decodetype,
                              netwib_encodetype encodetype)
{
  netwib_buf rawbuf, convertedbuf;
  netwib_wait *pnetwait, *pkbdwait;
  netwib_bool netevent, kbdevent;
  netwib_io *pkbdio;
  netwib_err ret=NETWIB_ERR_OK;

  /* initialize kbd */
  netwib_er(netwib_io_init_kbddefault(&pkbdio));
  netwib_er(netwib_kbd_ctl_set_line(pkbdio, linebyline));
  netwib_er(netwib_kbd_ctl_set_echo(pkbdio, echochars));

  /* initialize wait */
  netwib_er(netwib_wait_init_io_read(pio, &pnetwait));
  netwib_er(netwib_wait_init_io_read(pkbdio, &pkbdwait));

  /* initialize buffers */
  netwib_er(netwib_buf_init_mallocdefault(&rawbuf));
  netwib_er(netwib_buf_init_mallocdefault(&convertedbuf));

  /* main loop */
  while (NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();
    /* wait */
    netwib_er(netwib_wait_wait2(pnetwait, pkbdwait, NETWIB_TIME_INFINITE,
                                &netevent, &kbdevent));
    /* read */
    if (netevent) {
      netwib__buf_reinit(&rawbuf);
      ret = netwib_io_read(pio, &rawbuf);
      if (ret == NETWIB_ERR_DATANOTAVAIL) {
        ret = NETWIB_ERR_OK;
        /* continue */
      } else if (ret == NETWIB_ERR_DATAEND) {
        ret = NETWIB_ERR_OK;
        break;
      } else if (ret != NETWIB_ERR_OK) {
        break;
      } else {
        netwib_er(netwib_buf_display(&rawbuf, encodetype));
      }
    }
    if (kbdevent) {
      netwib__buf_reinit(&rawbuf);
      ret = netwib_io_read(pkbdio, &rawbuf);
      if (ret == NETWIB_ERR_DATANOTAVAIL) {
        ret = NETWIB_ERR_OK;
        /* continue */
      } else if (ret == NETWIB_ERR_DATAEND) {
        ret = NETWIB_ERR_OK;
        break;
      } else if (ret != NETWIB_ERR_OK) {
        break;
      } else {
        if (decodetype == NETWIB_DECODETYPE_DATA) {
          if (linebyline) {
            netwib_er(netwib_buf_append_string("\n", &rawbuf));
          }
          netwib_er(netwib_io_write(pio, &rawbuf));
        } else {
          netwib__buf_reinit(&convertedbuf);
          ret = netwib_buf_decode(&rawbuf, decodetype, &convertedbuf);
          if (ret == NETWIB_ERR_OK) {
            netwib_er(netwib_io_write(pio, &convertedbuf));
          } else {
            netwib_er(netwox_err_display(ret, NETWIB_ERR_ENCODETYPE_NUMTEXT));
          }
        }
      }
    }
  }

  /* close */
  netwib_er(netwib_buf_close(&convertedbuf));
  netwib_er(netwib_buf_close(&rawbuf));
  netwib_er(netwib_wait_close(&pkbdwait));
  netwib_er(netwib_wait_close(&pnetwait));
  netwib_er(netwib_io_close(&pkbdio));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_io_showecho(netwib_io *pio,
                              netwib_uint32 maxwaitms,
                              netwib_bool showscreen,
                              netwib_bool echoback)
{
  netwib_buf buf;
  netwib_time abstime;
  netwib_bool event;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_time_init_now(&abstime));
  netwib_er(netwib_time_plus_msec(&abstime, maxwaitms));

  /* initialize buffers */
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  /* main loop */
  while (NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();
    if (maxwaitms != 0) {
      ret = netwib_io_wait_read(pio, &abstime, &event);
      if (ret == NETWIB_ERR_DATAEND) {
        ret = NETWIB_ERR_OK;
        break;
      } else if (ret != NETWIB_ERR_OK) {
        break;
      }
      if (!event) {
        ret = NETWOX_ERR_TIMEOUT;
        break;
      }
    }
    /* read */
    netwib__buf_reinit(&buf);
    ret = netwib_io_read(pio, &buf);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      /* continue */
    } else if (ret == NETWIB_ERR_DATAEND) {
      ret = NETWIB_ERR_OK;
      break;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    } else {
      if (showscreen) {
        netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
      }
      if (echoback) {
        netwib_er(netwib_io_write(pio, &buf));
      }
    }
  }

  /* close */
  netwib_er(netwib_buf_close(&buf));

  return(ret);
}
