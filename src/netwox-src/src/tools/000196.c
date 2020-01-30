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
netwib_conststring t000196_description[] = {
  "This tool is a whois client.",
  "It obtains information about an object (generally a domain name).",
  "User has to specify a whois server (whois.internic.net, etc.).",
  "For example: netwox 196 --dst-ip whois.nic.fr --query enterprise.fr",
  "",
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000196_args[] = {
  NETWOX_SOCK_ARG_TCP_CLIPORT("43"),
  NETWOX_TOOLARG_REQ_BUF_HOSTNAME('q', "query", "information to query", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in milliseconds",
                             "60000"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000196_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_WHOIS,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000196_info = {
  "WHOIS client",
  t000196_description,
  NULL,
  t000196_args,
  t000196_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000196_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_buf query;
  netwib_io *pio;
  netwib_uint32 maxwaitms;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000196_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'q', &query));
  netwib_er(netwox_arg_uint32(parg, 'T', &maxwaitms));

  /* connect to server */
  netwib_er(netwox_sockinfo_init_arg_tcp_cli(parg, &sockinfo));
  netwib_er(netwox_sock_init(&sockinfo, &pio));

  /* query */
  netwib_er(netwox_whois_query(pio, &query, maxwaitms));

  /* close */
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
