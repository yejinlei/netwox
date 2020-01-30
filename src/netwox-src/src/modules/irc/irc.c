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
netwib_err netwox_irc_cmd_init_buf(netwib_constbuf *ptxtcmd,
                                   netwox_irc_cmd *pcmd)
{
  netwib_data data;
  netwib_uint32 datasize;
  netwib_char firstchar;

  datasize = netwib__buf_ref_data_size(ptxtcmd);
  if (datasize == 0) {
    if (pcmd != NULL) *pcmd = NETWOX_IRC_CMD_UNKNOWN;
    return(NETWIB_ERR_OK);
  }
  data = netwib__buf_ref_data_ptr(ptxtcmd);

  /* errors are numbers */
  if (datasize == 3) {
    if (netwib_c2_isdigit(data[0]) && netwib_c2_isdigit(data[1]) &&
        netwib_c2_isdigit(data[2])) {
      if (pcmd != NULL) {
        *pcmd = 100*netwib_c2_cto9(data[0]) +
          10*netwib_c2_cto9(data[1]) + netwib_c2_cto9(data[2]);
      }
      return(NETWIB_ERR_OK);
    }
  }

  /* commands are text strings */
#define netwox__irc_cmd_init_buf_cmp(txt,val) if (!netwib_c_memcasecmp(data, (netwib_constdata)txt, datasize)) { if (pcmd != NULL) *pcmd = val; return(NETWIB_ERR_OK); }
  firstchar = data[0];
  switch(firstchar) {
  case 'j' :
  case 'J' :
    switch(datasize) {
    case 4 :
      netwox__irc_cmd_init_buf_cmp("JOIN", NETWOX_IRC_CMD_JOIN);
      break;
    }
    break;
  case 'k' :
  case 'K' :
    switch(datasize) {
    case 4 :
      netwox__irc_cmd_init_buf_cmp("KICK", NETWOX_IRC_CMD_KICK);
      break;
    }
    break;
  case 'n' :
  case 'N' :
    switch(datasize) {
    case 4 :
      netwox__irc_cmd_init_buf_cmp("NICK", NETWOX_IRC_CMD_NICK);
      break;
    case 6 :
      netwox__irc_cmd_init_buf_cmp("NOTICE", NETWOX_IRC_CMD_NOTICE);
      break;
    }
    break;
  case 'p' :
  case 'P' :
    switch(datasize) {
    case 4 :
      netwox__irc_cmd_init_buf_cmp("PING", NETWOX_IRC_CMD_PING);
      netwox__irc_cmd_init_buf_cmp("PART", NETWOX_IRC_CMD_PART);
      break;
    case 7 :
      netwox__irc_cmd_init_buf_cmp("PRIVMSG", NETWOX_IRC_CMD_PRIVMSG);
      break;
    }
    break;
  case 'q' :
  case 'Q' :
    switch(datasize) {
    case 4 :
      netwox__irc_cmd_init_buf_cmp("QUIT", NETWOX_IRC_CMD_QUIT);
      break;
    }
    break;
  }


  if (pcmd != NULL) *pcmd = NETWOX_IRC_CMD_UNKNOWN;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_irc_decode_line(netwib_constbuf *pline,
                                  netwib_bufext *pprefix,
                                  netwox_irc_cmd *pcmd,
                                  netwib_bufext *pparameters)
{
  netwib_buf buf;
  netwib_data data, pc;
  netwib_uint32 datasize;

  datasize = netwib__buf_ref_data_size(pline);
  if (datasize == 0) {
    return(NETWOX_ERR_PROTOCOL);
  }
  data = netwib__buf_ref_data_ptr(pline);

  netwib_er(netwib_buf_init_ext_empty(pprefix));
  netwib_er(netwib_buf_init_ext_empty(pparameters));

  /* check if start with ':' */
  if (data[0] == ':') {
    /* search ' ' */
    pc = netwib_c_memchr(data, ' ', datasize);
    if (pc == NULL) {
      return(NETWOX_ERR_PROTOCOL);
    }
    netwib_er(netwib_buf_init_ext_arrayfilled(data+1, pc - data - 1, pprefix));
    datasize -= pc - data + 1;
    data = pc + 1;
  }

  /* extract parameters */
  pc = netwib_c_memchr(data, ' ', datasize);
  if (pc != NULL) {
    netwib_er(netwib_buf_init_ext_arrayfilled(pc + 1,
                                              datasize - (pc - data + 1),
                                              pparameters));
    datasize = pc - data;
  }

  /* analyze cmd */
  netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, &buf));
  netwib_er(netwox_irc_cmd_init_buf(&buf, pcmd));

  return(NETWIB_ERR_OK);
}
