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
netwib_conststring t000156_description[] = {
  "This tool connects on tool 155. It permits to measure throughput and",
  "jitter for TCP. So, tool 155 is ran on one computer, and tool 156 on",
  "another.",
  "",
  NETWOX_DESC_perf_chunkdur,
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000156_args[] = {
  NETWOX_SOCK_ARG_TCP_CLI,
  NETWOX_TOOLARG_OPTA_UINT32('s', "chunksize", "chunk size", "500"),
  NETWOX_TOOLARG_OPTA_UINT32('u', "duration", "test duration in ms", "5000"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000156_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_GENERIC,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000156_info = {
  "Network performance measurement : TCP client",
  t000156_description,
  "iperf",
  t000156_args,
  t000156_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000156_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwox_perfinfo perfinfo;
  netwib_err ret;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000156_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_tcp_cli(parg, &sockinfo));
  netwib_er(netwox_arg_uint32(parg, 's', &perfinfo.size));
  netwib_er(netwox_arg_uint32(parg, 'u', &perfinfo.durationms));

  /* main loop */
  netwib_er(netwox_sock_init(&sockinfo, &perfinfo.pio));
  ret = netwox_perf_cli(&perfinfo);

  /* close */
  netwib_er(netwib_io_close(&perfinfo.pio));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(ret);
}
