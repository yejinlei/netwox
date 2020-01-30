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
netwib_conststring t000091_description[] = {
  NETWOX_DESC_mulser,
  "",
  NETWOX_DESC_servertcpmul,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000091_args[] = {
  NETWOX_SOCK_ARG_TCP_MULSER,
  NETWOX_TOOLARG_OPT_BOOL('s', "showscreen", "show data to screen", NULL),
  NETWOX_TOOLARG_OPT_BOOL('b', "echoback", "echo data back to client", "1"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000091_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SERVER_TCP_GENERIC,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000091_info = {
  "TCP server multiclients",
  t000091_description,
  "nc, netcat",
  t000091_args,
  t000091_nodes,
};

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bool showscreen;
  netwib_bool echoback;
} t000091_para;

/*-------------------------------------------------------------*/
static netwib_err t000091_srv(netwib_io *pio,
                              netwib_ptr pinfos)
{
  t000091_para *ppara = (t000091_para *)pinfos;

  netwib_er(netwox_io_showecho(pio, 0, ppara->showscreen, ppara->echoback));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err t000091_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  t000091_para para;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000091_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_tcp_mulser(parg, &sockinfo));
  netwib_er(netwox_arg_bool(parg, 's', &para.showscreen));
  netwib_er(netwox_arg_bool(parg, 'b', &para.echoback));

  /* main loop */
  netwib_er(netwox_sock_tcp_mulser(&sockinfo, &t000091_srv, &para));

  /* close */
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
