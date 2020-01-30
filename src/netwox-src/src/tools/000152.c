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
netwib_conststring t000152_description[] = {
  "This tool is a simple interactive IRC client.",
  "Under Unix, press Control-R if a message is received during writing.",
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000152_args[] = {
  NETWOX_SOCK_ARG_TCP_CLIPORT("6667"),
  NETWOX_TOOLARG_REQ_BUF_LOGIN('n', "nickname", "nickname", NULL),
  NETWOX_TOOLARG_REQ_BUF_LOGIN('u', "username", "username", NULL),
  NETWOX_TOOLARG_OPT_BUF_PASSWORD('w', "password", NULL, NULL),
  NETWOX_TOOLARG_OPT_BUF('r', "realname", "real name", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000152_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_IRC,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000152_info = {
  "Interactive IRC client",
  t000152_description,
  NULL,
  t000152_args,
  t000152_nodes,
};

/*-------------------------------------------------------------*/
typedef struct {
  netwox_ircses ircses;
  netwib_io *pkbdio;
  netwib_buf channel;
} t000152_params;

/*-------------------------------------------------------------*/
static netwib_err t000152_net(t000152_params *pparams,
                              netwib_constbuf *pbuf)
{
  netwib_buf prefix, parameters;
  netwox_irc_cmd cmd;

#if 0
  /* some channels have Esc chars which make them unreadable */
  netwib_er(netwib_fmt_display("%{buf}\n", pbuf));
#else
  netwib_er(netwib_buf_display(pbuf, NETWIB_ENCODETYPE_TEXT));
  netwib_er(netwib_fmt_display("\n"));
#endif

  /* try to decode line */
  netwib_er(netwox_irc_decode_line(pbuf, &prefix, &cmd, &parameters));

  switch(cmd) {
  case NETWOX_IRC_CMD_PING :
    netwib_er(netwib_fmt_display("I answered to this ping\n"));
    netwib_er(netwox_ircses_write_pong(&pparams->ircses, &parameters));
    break;
#if 0
  case NETWOX_IRC_CMD_UNKNOWN :
    netwib_er(netwib_fmt_display("/!\\Warning : unknown command\n"));
    break;
#endif
  default :
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000152_kbd_cmd(t000152_params *pparams,
                                  netwib_buf *pbuf)
{
  netwib_buf prefix, parameters;
  netwox_irc_cmd cmd;

  /* try to decode line */
  netwib_er(netwox_irc_decode_line(pbuf, &prefix, &cmd, &parameters));

  switch(cmd) {
  case NETWOX_IRC_CMD_JOIN :
    netwib__buf_reinit(&pparams->channel);
    netwib_er(netwib_buf_append_buf(&parameters, &pparams->channel));
    break;
  case NETWOX_IRC_CMD_PART :
    netwib__buf_reinit(&pparams->channel);
    break;
#if 0
  case NETWOX_IRC_CMD_UNKNOWN :
    netwib_er(netwib_fmt_display("/!\\Warning : your command is unknown\n"));
    break;
#endif
  default :
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000152_kbd(t000152_params *pparams,
                              netwib_buf *pbuf)
{
  netwib_char *pfirstchar;

  /* display help */
  if (netwib__buf_ref_data_size(pbuf) == 0) {
    netwib_er(netwib_fmt_display("Enter message or command_starting_with_'/'. For example :\n"));
    netwib_er(netwib_fmt_display("\"/join #chan\", \"hello\", \"/part #chan\", \"/quit\"\n"));
    return(NETWIB_ERR_OK);
  }

  /* deal with commands */
  pfirstchar = (netwib_char*)netwib__buf_ref_data_ptr(pbuf);
  if (*pfirstchar == '/') {
    pbuf->beginoffset++;
    netwib_er(t000152_kbd_cmd(pparams, pbuf));
    netwib_er(netwib_buf_append_string("\r\n", pbuf));
    netwib_er(netwox_ircses_write(&pparams->ircses, pbuf));
    return(NETWIB_ERR_OK);
  }

  /* send messages */
  if (netwib__buf_ref_data_size(&pparams->channel)) {
    netwib_er(netwox_ircses_write_privmsg(&pparams->ircses, &pparams->channel,
                                          pbuf));
  } else {
    netwib_er(netwib_fmt_display("You must join a channel with \"/join #chan\" before writing.\n"));
    return(NETWIB_ERR_OK);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000152_loop(t000152_params *pparams)
{
  netwib_buf buf;
  netwib_wait *pnetwait, *pkbdwait;
  netwib_bool netevent, kbdevent;
  netwib_err ret=NETWIB_ERR_OK;

  /* initialize kbd */
  netwib_er(netwib_io_init_kbddefault(&pparams->pkbdio));
  netwib_er(netwib_kbd_ctl_set_line(pparams->pkbdio, NETWIB_TRUE));
  netwib_er(netwib_kbd_ctl_set_echo(pparams->pkbdio, NETWIB_TRUE));

  /* initialize wait */
  netwib_er(netwib_wait_init_io_read(pparams->ircses.pio, &pnetwait));
  netwib_er(netwib_wait_init_io_read(pparams->pkbdio, &pkbdwait));

  /* initialize buffers */
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  /* main loop */
  while (NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();
    /* wait */
    netwib_er(netwib_wait_wait2(pnetwait, pkbdwait, NETWIB_TIME_INFINITE,
                                &netevent, &kbdevent));
    /* read */
    if (netevent) {
      netwib__buf_reinit(&buf);
      ret = netwib_io_read(pparams->ircses.pio, &buf);
      if (ret == NETWIB_ERR_DATANOTAVAIL) {
        ret = NETWIB_ERR_OK;
        /* continue */
      } else if (ret == NETWIB_ERR_DATAEND) {
        ret = NETWIB_ERR_OK;
        break;
      } else if (ret != NETWIB_ERR_OK) {
        break;
      } else {
        netwib_er(t000152_net(pparams, &buf));
      }
    }
    if (kbdevent) {
      netwib__buf_reinit(&buf);
      ret = netwib_io_read(pparams->pkbdio, &buf);
      if (ret == NETWIB_ERR_DATANOTAVAIL) {
        ret = NETWIB_ERR_OK;
        /* continue */
      } else if (ret == NETWIB_ERR_DATAEND) {
        ret = NETWIB_ERR_OK;
        break;
      } else if (ret != NETWIB_ERR_OK) {
        break;
      } else {
        netwib_er(t000152_kbd(pparams, &buf));
      }
    }
  }

  /* close */
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_wait_close(&pkbdwait));
  netwib_er(netwib_wait_close(&pnetwait));
  netwib_er(netwib_io_close(&pparams->pkbdio));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000152_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  t000152_params params;
  netwib_buf password, nickname, username, realname;
  netwib_io *pio;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000152_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_tcp_cli(parg, &sockinfo));
  netwib_er(netwox_arg_buf(parg, 'w', &password));
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwox_arg_buf(parg, 'n', &nickname));
  nickname.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  nickname.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwox_arg_buf(parg, 'u', &username));
  username.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  username.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwox_arg_buf(parg, 'r', &realname));

  netwib_er(netwib_buf_init_mallocdefault(&params.channel));

  /* main loop */
  netwib_er(netwox_sock_init(&sockinfo, &pio));
  netwib_er(netwox_ircses_init(pio, &password, &nickname, &username, &realname,
                               &params.ircses));
  netwib_er(t000152_loop(&params));

  /* close */
  netwib_er(netwib_buf_close(&params.channel));
  netwib_er(netwox_ircses_close(&params.ircses));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
