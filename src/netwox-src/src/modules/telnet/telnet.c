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
/* Note : subnegotiation is not used (rfc855).
   So, we only have fixed bytes commands "IAC option", and not variable
   length commands "IAC SB option parameters IAC SE". */

/*-------------------------------------------------------------*/
#define NETWOX_TELNET_LIMIT_INSISTCOUNT 4
#define NETWOX_TELNET_DEBUG_NEGO 0

/*-------------------------------------------------------------*/
#define netwox_telnet_cli_do (netwox_telnet_cmd)(pstate->isaclient?NETWOX_TELNET_CMD_DO:NETWOX_TELNET_CMD_WILL)
#define netwox_telnet_cli_dont (netwox_telnet_cmd)(pstate->isaclient?NETWOX_TELNET_CMD_DONT:NETWOX_TELNET_CMD_WONT)
#define netwox_telnet_cli_will (netwox_telnet_cmd)(pstate->isaclient?NETWOX_TELNET_CMD_WILL:NETWOX_TELNET_CMD_DO)
#define netwox_telnet_cli_wont (netwox_telnet_cmd)(pstate->isaclient?NETWOX_TELNET_CMD_WONT:NETWOX_TELNET_CMD_DONT)

/*-------------------------------------------------------------*/
static netwib_err netwox_telnet_struct_init(netwib_bool isaclient,
                                            netwox_telnet *ptelnet)
{

  /* initialize structures */
  ptelnet->state.isaclient = isaclient;
  if (isaclient) {
  } else {
    ptelnet->state.server.ctrlc = NETWIB_FALSE;
    ptelnet->state.server.ctrld = NETWIB_FALSE;
    ptelnet->state.server.ctrlz = NETWIB_FALSE;
  }
  ptelnet->state.negomode = NETWOX_TELNET_NEGOMODE_NONE;
  ptelnet->state.wantednegomode = NETWOX_TELNET_NEGOMODE_CHARACTER;

  netwib_er(netwib_buf_init_mallocdefault(&ptelnet->bufrd));
  ptelnet->bufrd.flags |= NETWIB_BUF_FLAGS_CANSLIDE;
  netwib_er(netwib_buf_init_mallocdefault(&ptelnet->buftmp));
  netwib_er(netwib_buf_init_mallocdefault(&ptelnet->bufrdready));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_telnet_struct_close(netwox_telnet *ptelnet)
{

  /* close structures */
  netwib_er(netwib_buf_close(&ptelnet->buftmp));
  netwib_er(netwib_buf_close(&ptelnet->bufrd));
  netwib_er(netwib_buf_close(&ptelnet->bufrdready));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
#if NETWOX_TELNET_DEBUG_NEGO
static netwib_err netwox_telnet_opt_display(netwib_conststring msg,
                                            netwox_telnet_cmd cmd,
                                            netwox_telnet_opt opt)
{
  switch(cmd) {
  case NETWOX_TELNET_CMD_DO :
    netwib_er(netwib_fmt_display("%s DO   %{uint32}\n", msg, opt));
    break;
  case NETWOX_TELNET_CMD_DONT :
    netwib_er(netwib_fmt_display("%s DONT %{uint32}\n", msg, opt));
    break;
  case NETWOX_TELNET_CMD_WILL :
    netwib_er(netwib_fmt_display("%s WILL %{uint32}\n", msg, opt));
    break;
  case NETWOX_TELNET_CMD_WONT :
    netwib_er(netwib_fmt_display("%s WONT %{uint32}\n", msg, opt));
    break;
  default :
    netwib_er(netwib_fmt_display("%s UNKNOWN %{uint32}\n", msg, cmd));
    break;
  }

  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
static netwib_err netwox_telnet_opt_decode(netwox_telnet_state *pstate,
                                           netwox_telnet_cmd cmd,
                                           netwox_telnet_opt opt,
                                           netwib_io *pio)
{

#if NETWOX_TELNET_DEBUG_NEGO
  netwib_er(netwox_telnet_opt_display("READ ", cmd, opt));
#endif

  /* Note: telnet request and replies are symetric, so there is no
     way to determine if we've just read a request or a reply to our
     previous request. This can lead to loops. In order to suppress
     them, we must take care when replying. */

  if (opt == NETWOX_TELNET_OPT_ECHO || opt == NETWOX_TELNET_OPT_SGA) {
    /* Server has been requested to :
         - NEGOMODE_LINE: DONT ECHO|SGA
         - NEGOMODE_CHARACTER: DO ECHO|SGA
       So to prevent loops, we must take care about WILL|WONT ECHO|SGA.
       Other cases (DO|DONT) can simply be rejected. This is done at
       function end. */
    if (pstate->wantednegomode == NETWOX_TELNET_NEGOMODE_CHARACTER) {
      if (cmd == netwox_telnet_cli_will) {
        /* all is fine, there is no need to reply */
        if (opt == NETWOX_TELNET_OPT_ECHO) {
          /* we do not care about SGA for determining if LINE or CHAR */
          pstate->negomode = NETWOX_TELNET_NEGOMODE_CHARACTER;
        }
        return(NETWIB_ERR_OK);
      } else if (cmd == netwox_telnet_cli_wont) {
        /* insist (might create a loop, so use a counter to limit) */
        pstate->negoinsistcount++;
        if (pstate->negoinsistcount <= NETWOX_TELNET_LIMIT_INSISTCOUNT) {
          netwib_er(netwox_telnet_write_opt(pio, netwox_telnet_cli_do, opt));
        }
        return(NETWIB_ERR_OK);
      }
    }
    if (pstate->wantednegomode == NETWOX_TELNET_NEGOMODE_LINE) {
      if (cmd == netwox_telnet_cli_wont) {
        /* all is fine, there is no need to reply */
        return(NETWIB_ERR_OK);
      } else if (cmd == netwox_telnet_cli_will) {
        /* insist (might create a loop, so use a counter to limit) */
        pstate->negoinsistcount++;
        if (pstate->negoinsistcount <= NETWOX_TELNET_LIMIT_INSISTCOUNT) {
          netwib_er(netwox_telnet_write_opt(pio, netwox_telnet_cli_dont, opt));
        }
        return(NETWIB_ERR_OK);
      }
    }
  }

  /* unsupported options */
  switch(cmd) {
  case NETWOX_TELNET_CMD_DO :
  case NETWOX_TELNET_CMD_DONT :
    /* answer WON'T */
    netwib_er(netwox_telnet_write_opt(pio, NETWOX_TELNET_CMD_WONT, opt));
    break;
  case NETWOX_TELNET_CMD_WILL :
  case NETWOX_TELNET_CMD_WONT :
    /* answer DON'T */
    netwib_er(netwox_telnet_write_opt(pio, NETWOX_TELNET_CMD_DONT, opt));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_telnet_cmd_decode(netwox_telnet_state *pstate,
                                           netwox_telnet_cmd cmd,
                                           netwib_constdata data,
                                           netwib_uint32 datasize,
                                           netwib_io *pio,
                                           netwib_uint32 *pskipsize)
{

  switch(cmd) {
  case NETWOX_TELNET_CMD_NOP :
    *pskipsize = 2;
    break;
  case NETWOX_TELNET_CMD_DO :
  case NETWOX_TELNET_CMD_DONT :
  case NETWOX_TELNET_CMD_WILL :
  case NETWOX_TELNET_CMD_WONT :
    if (datasize < 3) {
      *pskipsize = 0;
    }
    netwib_er(netwox_telnet_opt_decode(pstate, cmd, data[2], pio));
    *pskipsize = 3;
    break;
  case NETWOX_TELNET_CMD_IP :
    if (!pstate->isaclient) {
      pstate->server.ctrlc = NETWIB_TRUE;
    }
    *pskipsize = 2;
    break;
  case NETWOX_TELNET_CMD_SUSP :
    if (!pstate->isaclient) {
      pstate->server.ctrlz = NETWIB_TRUE;
    }
    *pskipsize = 2;
    break;
  default :
    netwib_er(netwib_fmt_display("Warning : don't know how to decode command %{uint32:02X}\n", cmd));
    *pskipsize = 2;
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_telnet_io_read(netwib_io *pio,
                                        netwib_buf *pbuf)
{
  netwox_telnet *ptelnet = (netwox_telnet *)pio->pcommon;
  netwib_data data, pc;
  netwib_uint32 datasize, chunksize, olddatasize, skipsize;
  netwib_buf *pworkbuf;
  netwox_telnet_cmd cmd;

  /* eventually give data previously parsed */
  if (pbuf != NULL) {
    if (netwib__buf_ref_data_size(&ptelnet->bufrdready) != 0) {
      netwib_er(netwib_buf_append_buf(&ptelnet->bufrdready, pbuf));
      netwib__buf_reinit(&ptelnet->bufrdready);
      return(NETWIB_ERR_OK);
    }
  }

  /* if NULL, work on a buffer ready to read (no more IAC) named rdready */
  pworkbuf = pbuf;
  if (pbuf == NULL) {
    pworkbuf = &ptelnet->bufrdready;
  }
  olddatasize = netwib__buf_ref_data_size(pworkbuf);

  /* read */
  netwib_er(netwib_io_read(pio->rd.pnext, &ptelnet->bufrd));

  /* check for options (answer to them) and data */
  data = netwib__buf_ref_data_ptr(&ptelnet->bufrd);
  datasize = netwib__buf_ref_data_size(&ptelnet->bufrd);
  while (datasize) {
    pc = netwib_c_memchr(data, NETWOX_TELNET_IAC, datasize);
    if (pc == NULL) {
      /* give last part to user */
      netwib_er(netwib_buf_append_data(data, datasize, pworkbuf));
      datasize = 0;
      break;
    } else {
      /* give chunk to user */
      chunksize = pc - data;
      netwib_er(netwib_buf_append_data(data, chunksize, pworkbuf));
      /* skip chunk */
      data += chunksize;
      datasize -= chunksize;
      if (datasize == 1) {
        /* no sufficient data */
        break;
      }
      /* obtain option */
      cmd = data[1];
      if (cmd == NETWOX_TELNET_IAC) {
        /* convert it to 0xFF char (was duplicated) */
        netwib_er(netwib_buf_append_byte(NETWOX_TELNET_IAC, pworkbuf));
        skipsize = 2;
      } else {
        netwib_er(netwox_telnet_cmd_decode(&ptelnet->state, cmd, data,
                                           datasize, pio,
                                           &skipsize));
        if (skipsize == 0) {
          /* no sufficient data */
          break;
        }
      }
      /* skip option */
      data += skipsize;
      datasize -= skipsize;
    }
  }
  if (datasize) {
    ptelnet->bufrd.beginoffset = ptelnet->bufrd.endoffset - datasize;
  } else {
    netwib__buf_reinit(&ptelnet->bufrd);
  }

  if (netwib__buf_ref_data_size(pworkbuf) == olddatasize) {
    return(NETWIB_ERR_DATANOTAVAIL);
  }

  /* telnet protocol can give 0D00 instead of 0D0A which is misleading,
     so replace it with 0D0A (needed in netwox_telnetserver_session_exec
     when writing back in NEGOMODE_CHARACTER) */
  data = netwib__buf_ref_data_ptr(pworkbuf);
  datasize = netwib__buf_ref_data_size(pworkbuf);
  if (pbuf != NULL) {
    data += olddatasize;
    datasize -= olddatasize;
  }
  while (datasize) {
    if (*data == '\r') {
      if (datasize >= 2) {
        if (*(data+1) == '\0') {
          *(data+1) = '\n';
        }
      }
    }
    /* while in loop, we also check for special characters sent by client */
    if (!ptelnet->state.isaclient) {
      if (*data == 0x03) {
        ptelnet->state.server.ctrlc = NETWIB_TRUE;
      }
      if (*data == 0x04) {
        ptelnet->state.server.ctrld = NETWIB_TRUE;
      }
      if (*data == 0x1A) {
        ptelnet->state.server.ctrlz = NETWIB_TRUE;
      }
    }
    data++;
    datasize--;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_telnet_io_write(netwib_io *pio,
                                         netwib_constbuf *pbuf)
{
  netwox_telnet *ptelnet = (netwox_telnet *)pio->pcommon;
  netwib_data data;
  netwib_uint32 datasize;
  netwib_bool rseen;

  /* duplicate 0xFF/NETWOX_TELNET_IAC in user data,
     and add a '\r' before '\n' if not present */
  netwib__buf_reinit(&ptelnet->buftmp);
  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);
  rseen = NETWIB_FALSE;
  while (datasize) {
    if (*data == NETWOX_TELNET_IAC) {
      netwib_er(netwib_buf_append_byte(NETWOX_TELNET_IAC, &ptelnet->buftmp));
      rseen = NETWIB_FALSE;
    } else if (*data == '\r') {
      rseen = NETWIB_TRUE;
    } else if (*data == '\n') {
      if (!rseen) {
        netwib_er(netwib_buf_append_byte('\r', &ptelnet->buftmp));
      } else {
        rseen = NETWIB_FALSE;
      }
    } else {
      rseen = NETWIB_FALSE;
    }
    netwib_er(netwib_buf_append_byte(*data, &ptelnet->buftmp));
    data++;
    datasize--;
  }

  /* write */
  netwib_er(netwib_io_write(pio->wr.pnext, &ptelnet->buftmp));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_telnet_io_wait(netwib_io *pio,
                                        netwib_io_waytype way,
                                        netwib_consttime *pabstime,
                                        netwib_bool *pevent)
{
  netwox_telnet *ptelnet = (netwox_telnet *)pio->pcommon;

  switch(way) {
  case NETWIB_IO_WAYTYPE_READ :
    if (netwib__buf_ref_data_size(&ptelnet->bufrdready) != 0) {
      if (pevent != NULL) *pevent = NETWIB_TRUE;
      return(NETWIB_ERR_OK);
    }
    break;
  case NETWIB_IO_WAYTYPE_RDWR :
  case NETWIB_IO_WAYTYPE_SUPPORTED :
    return(NETWIB_ERR_PLEASECONSTRUCT);
  default :
    break;
  }

  pabstime = pabstime; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_telnet_io_close(netwib_io *pio)
{
  netwox_telnet *ptelnet = (netwox_telnet *)pio->pcommon;
  netwib_er(netwox_telnet_struct_close(ptelnet));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_telnet_init(netwib_bool isaclient,
                              netwib_io **ppio)
{
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwox_telnet), &pcommon));
  ret = netwox_telnet_struct_init(isaclient, (netwox_telnet *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(NETWIB_TRUE, NETWIB_TRUE,
                           pcommon,
                           &netwox_telnet_io_read,
                           &netwox_telnet_io_write,
                           &netwox_telnet_io_wait,
                           NULL, /* unread */
                           NULL, /* set */
                           NULL, /* get */
                           &netwox_telnet_io_close,
                           ppio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_telnet_write_noiac(netwib_io *pio,
                                     netwib_constbuf *pbuf)
{

  netwib_er(netwib_io_write(pio->wr.pnext, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_telnet_write_opt(netwib_io *pio,
                                   netwox_telnet_cmd cmd,
                                   netwox_telnet_opt opt)
{
  netwib_byte array[3];
  netwib_buf buf;

#if NETWOX_TELNET_DEBUG_NEGO
  netwib_er(netwox_telnet_opt_display("WRITE", cmd, opt));
#endif

  array[0] = (netwib_byte)NETWOX_TELNET_IAC;
  array[1] = (netwib_byte)cmd;
  array[2] = (netwib_byte)opt;
  netwib_er(netwib_buf_init_ext_arrayfilled(array, 3, &buf));
  netwib_er(netwox_telnet_write_noiac(pio, &buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_telnet_negotiate(netwib_io *pio,
                                   netwox_telnet_negomode negomode)
{
  netwox_telnet *ptelnet = (netwox_telnet *)pio->pcommon;
  netwox_telnet_state *pstate = &ptelnet->state;
  netwib_err ret;

  ptelnet->state.wantednegomode = negomode;
  if (ptelnet->state.wantednegomode == NETWOX_TELNET_NEGOMODE_LINEMODE) {
    /* not implemented, so fallback to character */
    ptelnet->state.wantednegomode = NETWOX_TELNET_NEGOMODE_CHARACTER;
  }

  if (ptelnet->state.wantednegomode == NETWOX_TELNET_NEGOMODE_LINE) {
    /* for first time, it's not necessary to send DONT, but in case
       this function is called after a CHARACTER negotiation */
    netwib_er(netwox_telnet_write_opt(pio, netwox_telnet_cli_dont,
                                      NETWOX_TELNET_OPT_ECHO));
    netwib_er(netwox_telnet_write_opt(pio, netwox_telnet_cli_dont,
                                      NETWOX_TELNET_OPT_SGA));
    /* LINE mode rejects everything, so it will be always accepted.
       We can say now its negotiated without waiting */
    ptelnet->state.negomode = NETWOX_TELNET_NEGOMODE_LINE;
  } else if (ptelnet->state.wantednegomode==NETWOX_TELNET_NEGOMODE_CHARACTER) {
    netwib_er(netwox_telnet_write_opt(pio, netwox_telnet_cli_do,
                                      NETWOX_TELNET_OPT_ECHO));
    netwib_er(netwox_telnet_write_opt(pio, netwox_telnet_cli_do,
                                      NETWOX_TELNET_OPT_SGA));
  } else {
    return(NETWOX_ERR_NOTIMPLEMENTED);
  }

  ptelnet->state.negoinsistcount = 0;
  while (NETWIB_TRUE) {
    ret = netwox_telnet_io_read(pio, NULL);
    if (ret != NETWIB_ERR_OK && ret != NETWIB_ERR_DATANOTAVAIL) {
      return(ret);
    }
    ret = NETWIB_ERR_OK;
    if (ptelnet->state.negomode != NETWOX_TELNET_NEGOMODE_NONE) {
      break;
    }
    if (ptelnet->state.negoinsistcount > NETWOX_TELNET_LIMIT_INSISTCOUNT) {
      break;
    }
  }

  /* if we could not negotiate CHARACTER, try LINE */
  if (ptelnet->state.negomode == NETWOX_TELNET_NEGOMODE_NONE &&
      ptelnet->state.wantednegomode == NETWOX_TELNET_NEGOMODE_CHARACTER) {
    netwib_er(netwox_telnet_negotiate(pio, NETWOX_TELNET_NEGOMODE_LINE));
  }

  return(NETWIB_ERR_OK);
}
