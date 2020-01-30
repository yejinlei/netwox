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
netwib_conststring t000155_description[] = {
  "This tool is a TCP server. Use tool 156 to connect to it and obtain a",
  "network performance measurement.",
  "",
  NETWOX_DESC_server,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000155_args[] = {
  NETWOX_SOCK_ARG_TCP_SER,
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000155_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SERVER_TCP_GENERIC,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000155_info = {
  "Network performance measurement : TCP server",
  t000155_description,
  "iperf",
  t000155_args,
  t000155_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000155_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_io *pio;
  netwib_err ret;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000155_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_tcp_ser(parg, &sockinfo));

  /* main loop */
  netwib_er(netwox_sock_init(&sockinfo, &pio));
  ret = netwox_perf_ser(pio);

  /* close */
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(ret);
}
