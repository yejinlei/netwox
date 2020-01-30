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
netwib_err netwox_ircses_init(netwib_io *pio,
                              netwib_constbuf *ppassword,
                              netwib_constbuf *pnickname,
                              netwib_constbuf *pusername,
                              netwib_constbuf *prealname,
                              netwox_ircses *pircses)
{

  netwib_er(netwox_txtproto_io_rd(pio, &pircses->pio));

  /* send pass/nick/user */
  if (netwib__buf_ref_data_sizenull(ppassword)) {
    netwib_er(netwox_ircses_write_pass(pircses, ppassword));
  }
  netwib_er(netwox_ircses_write_nick(pircses, pnickname));
  netwib_er(netwox_ircses_write_user(pircses, NULL, pusername, prealname));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ircses_close(netwox_ircses *pircses)
{
  netwib_er(netwib_io_close(&pircses->pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ircses_write(netwox_ircses *pircses,
                               netwib_constbuf *pbuf)
{
  return(netwib_io_write(pircses->pio, pbuf));
}

/*-------------------------------------------------------------*/
netwib_err netwox_ircses_write_cmd(netwox_ircses *pircses,
                                   netwib_constbuf *pprefix,
                                   netwib_conststring command,
                                   netwib_constbuf *pparameter,
                                   netwib_constbuf *ptrailing)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));

  if (pprefix != NULL) {
    netwib_er(netwib_buf_append_byte(':', &buf));
    netwib_er(netwib_buf_append_buf(pprefix, &buf));
  }
  netwib_er(netwib_buf_append_string(command, &buf));
  if (pparameter != NULL) {
    netwib_er(netwib_buf_append_string(" ", &buf));
    netwib_er(netwib_buf_append_buf(pparameter, &buf));
  }
  if (ptrailing != NULL) {
    netwib_er(netwib_buf_append_string(" :", &buf));
    netwib_er(netwib_buf_append_buf(ptrailing, &buf));
  }
  netwib_er(netwib_buf_append_string("\r\n", &buf));

  ret = netwib_io_write(pircses->pio, &buf);

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ircses_write_user(netwox_ircses *pircses,
                                    netwib_constbuf *pprefix,
                                    netwib_constbuf *pusername,
                                    netwib_constbuf *prealname)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_constbuf *preal;
  netwib_ip ipad;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));

  netwib_er(netwib_buf_append_buf(pusername, &buf));
  netwib_er(netwib_buf_append_byte(' ', &buf));
  netwib_er(netwib_sock_ctl_get_local(pircses->pio, &ipad, NULL));
  netwib_er(netwib_buf_append_ip(&ipad, NETWIB_IP_ENCODETYPE_IP, &buf));
  netwib_er(netwib_buf_append_byte(' ', &buf));
  netwib_er(netwib_sock_ctl_get_remote(pircses->pio, &ipad, NULL));
  netwib_er(netwib_buf_append_ip(&ipad, NETWIB_IP_ENCODETYPE_IP, &buf));

  preal = prealname;
  if (!netwib__buf_ref_data_sizenull(prealname)) {
    preal = pusername;
  }
  ret = netwox_ircses_write_cmd(pircses, pprefix, "USER", &buf, preal);

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ircses_loop(netwox_ircses_loop_pf pfunc,
                              netwox_ircses *pircses,
                              netwib_ptr infos)
{
  netwib_buf buf;
  netwib_buf prefix, parameters;
  netwox_irc_cmd cmd;
  netwib_bool stoploop;
  netwib_err ret=NETWIB_ERR_OK;

  /* initialize buffers */
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  /* main loop */
  stoploop = NETWIB_FALSE;
  while (NETWIB_TRUE) {
    /* read */
    netwib__buf_reinit(&buf);
    ret = netwib_io_read(pircses->pio, &buf);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret == NETWIB_ERR_DATAEND) {
      ret = NETWIB_ERR_OK;
      break;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* decode line */
    netwib_er(netwox_irc_decode_line(&buf, &prefix, &cmd, &parameters));
    /* answer ping */
    switch(cmd) {
    case NETWOX_IRC_CMD_PING :
      netwib_er(netwox_ircses_write_pong(pircses, &parameters));
      break;
#if 0
    case NETWOX_IRC_CMD_UNKNOWN :
      netwib_er(netwib_fmt_display("/!\\Warning : unknown command\n"));
      break;
#endif
    default :
      break;
    }
    /* call user function */
    ret = (*pfunc)(pircses, infos, &buf, &prefix, cmd, &parameters, &stoploop);
    if (ret != NETWIB_ERR_OK) break;
    if (stoploop) break;
  }

  /* close */
  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_ircses_waitready_f(netwox_ircses *pircses,
                                            netwib_ptr infos,
                                            netwib_constbuf *pline,
                                            netwib_constbuf *pprefix,
                                            netwox_irc_cmd cmd,
                                            netwib_constbuf *pparameters,
                                            netwib_bool *pstoploop)
{
  if (cmd == NETWOX_IRC_CMD_PING || cmd == NETWOX_IRC_CMD_ENDOFMOTD) {
    *pstoploop = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  pircses = pircses; /* for compiler warning */
  infos = infos; /* for compiler warning */
  pline = pline; /* for compiler warning */
  pprefix = pprefix; /* for compiler warning */
  pparameters = pparameters; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
netwib_err netwox_ircses_waitready(netwox_ircses *pircses)
{
  netwib_er(netwox_ircses_loop(&netwox_ircses_waitready_f, pircses, NULL));
  return(NETWIB_ERR_OK);
}
