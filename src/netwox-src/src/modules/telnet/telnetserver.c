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
netwib_err netwox_telnetserver_init(netwox_telnetserver *ptelnetserver)
{
  /* those can be changed by users */
  ptelnetserver->timeoutms = 180000;
  netwib_er(netwib_buf_init_ext_storage(&ptelnetserver->login));
  ptelnetserver->login.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  netwib_er(netwib_buf_init_ext_storage(&ptelnetserver->password));
  ptelnetserver->password.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  netwib_er(netwib_buf_init_ext_storage(&ptelnetserver->root));
  ptelnetserver->pallowedclients = NULL;

  /* internal */
  netwib_er(netwib_bufpool_init(NETWIB_TRUE, &ptelnetserver->pbufpool));

  /* others have to be externally set */

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_telnetserver_close(netwox_telnetserver *ptelnetserver)
{
  netwib_er(netwib_bufpool_close(&ptelnetserver->pbufpool));

  netwib_er(netwib_buf_close(&ptelnetserver->login));
  netwib_er(netwib_buf_close(&ptelnetserver->password));
  netwib_er(netwib_buf_close(&ptelnetserver->root));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* structure specific to each thread */
typedef struct {
  netwox_consttelnetserver *pcommon;
  netwox_telnet_state *pstate;
  netwib_io *pio;
  netwib_bool endsession;
} netwox_priv_telnetserver;

/*-------------------------------------------------------------*/
static netwib_err netwox_telnetserver_readline_line(netwib_buf *pline,
                                                    netwib_buf *pbuf,
                                                    netwib_bool *pendloop)
{
  netwib_data data;
  netwib_uint32 datasize;

  /* suppress newline */
  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);
  while(datasize) {
    if (*data == '\r' || *data == '\n') {
      pbuf->endoffset -= datasize;
      break;
    }
    data++;
    datasize--;
  }

  netwib_er(netwib_buf_append_buf(pbuf, pline));
  *pendloop = NETWIB_TRUE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_telnetserver_readline_char(netwox_priv_telnetserver *ptelnet,
                                                    netwib_bool echoback,
                                                    netwib_buf *pline,
                                                    netwib_buf *pbuf,
                                                    netwib_bool *pendloop)
{
  netwib_buf *pwrbuf;
  netwib_data data;
  netwib_uint32 datasize;

  netwib_er(netwib_bufpool_buf_init(ptelnet->pcommon->pbufpool, &pwrbuf));

  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);
  while(datasize) {
    if (*data == '\r' || *data == '\n') {
      netwib_er(netwib_buf_append_string("\r\n", pwrbuf));
      *pendloop = NETWIB_TRUE;
      break;
    } else if (*data == '\b' || *data == 0x7F) {
      if (netwib__buf_ref_data_size(pline) != 0) {
        pline->endoffset--;
        if (echoback) {
          netwib_er(netwib_buf_append_string("\b \b", pwrbuf));
        }
      }
    } else {
      netwib_er(netwib_buf_append_byte(*data, pline));
      if (echoback) {
        netwib_er(netwib_buf_append_byte(*data, pwrbuf));
      }
    }
    data++;
    datasize--;
  }

  netwib_er(netwib_io_write(ptelnet->pio, pwrbuf));
  netwib_er(netwib_bufpool_buf_close(ptelnet->pcommon->pbufpool, &pwrbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_telnetserver_readline(netwox_priv_telnetserver *ptelnet,
                                               netwib_bool echoback,
                                               netwib_buf *pline)
{
  netwib_time abstime;
  netwib_buf msg, *pbuf;
  netwib_bool event, endloop;
  netwib_err ret;

  netwib_er(netwib_time_init_now(&abstime));
  netwib_er(netwib_time_plus_msec(&abstime, ptelnet->pcommon->timeoutms));

  netwib_er(netwib_bufpool_buf_init(ptelnet->pcommon->pbufpool, &pbuf));

  ret = NETWIB_ERR_OK;
  endloop = NETWIB_FALSE;
  ptelnet->pstate->server.ctrld = NETWIB_FALSE;
  while(NETWIB_TRUE) {
    /* wait */
    netwib_er(netwib_io_wait_read(ptelnet->pio, &abstime, &event));
    if (!event) {
      netwib_er(netwib_buf_init_ext_string("Timeout\n", &msg));
      netwib_er(netwib_io_write(ptelnet->pio, &msg));
      ptelnet->endsession = NETWIB_TRUE;
      break;
    }
    /* read */
    netwib__buf_reinit(pbuf);
    ret = netwib_io_read(ptelnet->pio, pbuf);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret == NETWIB_ERR_DATAEND) {
      ptelnet->endsession = NETWIB_TRUE;
      ret = NETWIB_ERR_OK;
      break;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    if (ptelnet->pstate->server.ctrld) {
      ptelnet->endsession = NETWIB_TRUE;
      break;
    }
    /* deal with data */
    if (ptelnet->pstate->negomode == NETWOX_TELNET_NEGOMODE_CHARACTER) {
      netwib_er(netwox_telnetserver_readline_char(ptelnet, echoback,
                                                  pline, pbuf, &endloop));
    } else {
      netwib_er(netwox_telnetserver_readline_line(pline, pbuf, &endloop));
    }
    if (endloop) {
      break;
    }
  }

  netwib_er(netwib_bufpool_buf_close(ptelnet->pcommon->pbufpool, &pbuf));

#if 0
  if (ret == NETWIB_ERR_OK && !ptelnet->endsession) {
    netwib_er(netwib_fmt_display("Read: "));
    netwib_er(netwib_buf_display(pline, NETWIB_ENCODETYPE_MIXED));
    netwib_er(netwib_fmt_display("\n"));
  }
#endif

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_telnetserver_session_ip(netwox_priv_telnetserver *ptelnet)
{
  netwib_ip remoteip;
  netwib_buf *pmsg;
  netwib_bool allowed;

  netwib_er(netwib_bufpool_buf_init(ptelnet->pcommon->pbufpool, &pmsg));
  allowed = NETWIB_TRUE;

  netwib_er(netwib_sock_ctl_get_remote(ptelnet->pio, &remoteip, NULL));
  if (ptelnet->pcommon->pallowedclients != NULL) {
    netwib_er(netwib_ips_contains_ip(ptelnet->pcommon->pallowedclients,
                                     &remoteip, &allowed));
    if (!allowed) {
      netwib_er(netwib_buf_append_fmt(pmsg,
                                      "IP address %{ip} is not allowed\n",
                                      &remoteip));
      ptelnet->endsession = NETWIB_TRUE;
    }
  }

  if (allowed) {
    netwib_er(netwib_buf_append_fmt(pmsg, "Welcome %{ip}\n", &remoteip));
  }

  netwib_er(netwib_io_write(ptelnet->pio, pmsg));

  netwib_er(netwib_bufpool_buf_close(ptelnet->pcommon->pbufpool, &pmsg));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_telnetserver_session_login(netwox_priv_telnetserver *ptelnet)
{
  netwib_buf msg, *pline;
  netwib_cmp cmp;

  netwib_er(netwib_bufpool_buf_init(ptelnet->pcommon->pbufpool, &pline));

  if (netwib__buf_ref_data_size(&ptelnet->pcommon->login) != 0) {
    netwib_er(netwib_buf_init_ext_string("Login: ", &msg));
    netwib_er(netwib_io_write(ptelnet->pio, &msg));
    netwib_er(netwox_telnetserver_readline(ptelnet, NETWIB_TRUE, pline));
    if (!ptelnet->endsession) {
      netwib_er(netwib_buf_cmp(&ptelnet->pcommon->login, pline, &cmp));
      if (cmp != NETWIB_CMP_EQ) {
        ptelnet->endsession = NETWIB_TRUE;
      }
    }
  }

  if (netwib__buf_ref_data_size(&ptelnet->pcommon->password) != 0) {
    netwib_er(netwib_buf_init_ext_string("Password: ", &msg));
    netwib_er(netwib_io_write(ptelnet->pio, &msg));
    netwib__buf_reinit(pline);
    netwib_er(netwox_telnetserver_readline(ptelnet, NETWIB_FALSE, pline));
    if (!ptelnet->endsession) {
      netwib_er(netwib_buf_cmp(&ptelnet->pcommon->password, pline, &cmp));
      if (cmp != NETWIB_CMP_EQ) {
        ptelnet->endsession = NETWIB_TRUE;
      }
    }
  }

  if (ptelnet->endsession) {
    netwib_er(netwib_buf_init_ext_string("Login incorrect\n", &msg));
    netwib_er(netwib_io_write(ptelnet->pio, &msg));
  } else {
    netwib__buf_reinit(pline);
    netwib_er(netwib_buf_append_string("\nYou can enter a command.\n", pline));
    netwib_er(netwib_buf_append_string("Examples: /bin/ls, /bin/sh -c \"pwd;ls\", cmd /c dir c:, /bin/bash -i\n", pline));
    netwib_er(netwib_buf_append_string("Note: full path has to be specified\n", pline));
    netwib_er(netwib_buf_append_string("Note: cannot use 'prog1 | prog2', 'program > file' or 'program < file'\n", pline));
    netwib_er(netwib_io_write(ptelnet->pio, pline));
  }

  netwib_er(netwib_bufpool_buf_close(ptelnet->pcommon->pbufpool, &pline));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_telnetserver_session_exec(netwox_priv_telnetserver *ptelnet,
                                                   netwib_constbuf *pcmd)
{
  netwib_io *pexecio;
  netwib_wait *pstdinwait, *pstdoutwait;
  netwib_buf *pbuf;
  netwib_bool stdinevent, stdoutevent;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_io_init_exec(pcmd, NETWIB_IO_WAYTYPE_RDWR, NETWIB_TRUE,
                                NULL, NULL, &pexecio));
  netwib_er(netwib_wait_init_io_read(ptelnet->pio, &pstdinwait));
  netwib_er(netwib_wait_init_io_read(pexecio, &pstdoutwait));

  /* main loop */
  ptelnet->pstate->server.ctrlc = NETWIB_FALSE;
  netwib_er(netwib_bufpool_buf_init(ptelnet->pcommon->pbufpool, &pbuf));
  while(NETWIB_TRUE) {
    /* wait */
    netwib_er(netwib_wait_wait2(pstdinwait, pstdoutwait, NETWIB_TIME_INFINITE,
                                &stdinevent, &stdoutevent));
    /* read */
    netwib__buf_reinit(pbuf);
    if (stdinevent) {
      /* stdin data */
      ret = netwib_io_read(ptelnet->pio, pbuf);
      /* check user cancel request */
      if (ptelnet->pstate->server.ctrlc) {
        ptelnet->pstate->server.ctrlc = NETWIB_FALSE;
        ret = NETWIB_ERR_OK;
        break;
      }
    } else if (stdoutevent) {
      /* stdout data */
      ret = netwib_io_read(pexecio, pbuf);
    }
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATANOTAVAIL) {
        ret = NETWIB_ERR_OK;
        continue;
      }
      if (ret == NETWIB_ERR_DATAEND) {
        ret = NETWIB_ERR_OK;
        break;
      }
      break;
    }
    /* write */
    if (stdinevent) {
      ret = netwib_io_write(pexecio, pbuf);
      if (ret == NETWIB_ERR_OK) {
        if (ptelnet->pstate->negomode == NETWOX_TELNET_NEGOMODE_CHARACTER) {
          /* write back to echo user data. However, we don't know if it's
             a password. We also don't know how to deal with '\b'. */
          ret = netwib_io_write(ptelnet->pio, pbuf);
        }
      }
    } else if (stdoutevent) {
      ret = netwib_io_write(ptelnet->pio, pbuf);
    }
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_bufpool_buf_close(ptelnet->pcommon->pbufpool, &pbuf));

  netwib_er(netwib_wait_close(&pstdinwait));
  netwib_er(netwib_wait_close(&pstdoutwait));
  netwib_er(netwib_io_close(&pexecio));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_telnetserver_session_loop(netwox_priv_telnetserver *ptelnet)
{
  netwib_buf buf, *pline;
  netwib_cmp cmp;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(ptelnet->pcommon->pbufpool, &pline));

  while(NETWIB_TRUE) {
    /* display prompt */
    netwib_er(netwib_buf_init_ext_string("$ ", &buf));
    netwib_er(netwib_io_write(ptelnet->pio, &buf));
    /* read command */
    netwib__buf_reinit(pline);
    netwib_er(netwox_telnetserver_readline(ptelnet, NETWIB_TRUE, pline));
    if (ptelnet->endsession) {
      break;
    }
    /* end of session (exit, logout) */
    netwib_er(netwib_buf_cmp_string(pline, "exit", &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      break;
    }
    netwib_er(netwib_buf_cmp_string(pline, "logout", &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      break;
    }
    /* execute it */
    if (netwib__buf_ref_data_size(pline) != 0) {
      ret = netwox_telnetserver_session_exec(ptelnet, pline);
      if (ret != NETWIB_ERR_OK) {
        netwib_er(netwib_buf_init_ext_string("Could not run this command\n",
                                           &buf));
        netwib_er(netwib_io_write(ptelnet->pio, &buf));
      }
    }
  }

  netwib_er(netwib_bufpool_buf_close(ptelnet->pcommon->pbufpool, &pline));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_telnetserver_session(netwib_io *psockio,
                                       netwib_ptr pinfos)
{
  netwox_priv_telnetserver telnetserver;

  /* prepare variables */
  telnetserver.pcommon = (netwox_consttelnetserver *)pinfos;
  netwib_er(netwox_telnet_init(NETWIB_FALSE, &telnetserver.pio));
  telnetserver.pstate = &((netwox_telnet *)telnetserver.pio->pcommon)->state;
  netwib_er(netwib_io_plug_rdwr(telnetserver.pio, psockio));
  telnetserver.endsession = NETWIB_FALSE;
  netwib_er(netwox_telnet_negotiate(telnetserver.pio,
                                    telnetserver.pcommon->negomode));

  /* check IP */
  netwib_er(netwox_telnetserver_session_ip(&telnetserver));

  /* login */
  if (!telnetserver.endsession) {
    netwib_er(netwox_telnetserver_session_login(&telnetserver));
  }

  /* loop */
  if (!telnetserver.endsession) {
    netwib_er(netwox_telnetserver_session_loop(&telnetserver));
  }

  /* close */
  netwib_er(netwib_io_unplug_next_supported(telnetserver.pio, NULL));
  netwib_er(netwib_io_close(&telnetserver.pio));

  return(NETWIB_ERR_OK);
}
