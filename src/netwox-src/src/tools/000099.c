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
netwib_conststring t000099_description[] = {
  "This tool is a very simple implementation of a telnet client.",
  "In vi editor, keys jklm can be used instead of arrows.",
  "To have a better environment, run 'export TERM=vt100'.",
  "",
  "Parameter --line-by-line defines if client works line by line",
  "(obsolete, but it will work with all servers).",
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000099_args[] = {
  NETWOX_SOCK_ARG_TCP_CLIPORT("23"),
  NETWOX_TOOLARG_OPTA_BOOL('L', "line-by-line", "line by line mode", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000099_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_TELNET,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000099_info = {
  "TELNET client",
  t000099_description,
  NULL,
  t000099_args,
  t000099_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000099_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_io *ptcpio, *ptelnetio;
  netwox_telnet *ptelnet;
  netwox_telnet_negomode negomode;
  netwib_bool b=NETWIB_FALSE, linebyline;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000099_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_tcp_cli(parg, &sockinfo));
  netwib_er(netwox_arg_bool(parg, 'L', &linebyline));

  /* create a telnet io, and plug it on top of tcpio */
  netwib_er(netwox_sock_init(&sockinfo, &ptcpio));
  netwib_er(netwox_telnet_init(NETWIB_TRUE, &ptelnetio));
  netwib_er(netwib_io_plug_rdwr(ptelnetio, ptcpio));

  /* negotiate mode, and deal with what we really obtained */
  negomode = NETWOX_TELNET_NEGOMODE_CHARACTER;
  if (linebyline) {
    negomode = NETWOX_TELNET_NEGOMODE_LINE;
  }
  netwib_er(netwox_telnet_negotiate(ptelnetio, negomode));
  ptelnet = (netwox_telnet *)ptelnetio->pcommon;
  b = NETWIB_FALSE;
  if (ptelnet->state.negomode == NETWOX_TELNET_NEGOMODE_CHARACTER) {
    if (linebyline) {
      netwib_er(netwib_fmt_display("Warning: we wanted line-by-line but obtained char-by-char\n"));
    }
  } else if (ptelnet->state.negomode == NETWOX_TELNET_NEGOMODE_LINE) {
    if (!linebyline) {
      netwib_er(netwib_fmt_display("Warning: we wanted char-by-char but obtained line-by-line\n"));
    }
    b = NETWIB_TRUE;
  } else {
    netwib_er(netwib_fmt_display("Warning: we could not negotiate a mode\n"));
  }

  /* main loop */
  netwib_er(netwox_io_kbd_loop(ptelnetio, b, b, NETWIB_DECODETYPE_DATA,
                               NETWIB_ENCODETYPE_DATA));

  /* close */
  netwib_er(netwib_io_close(&ptelnetio));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
