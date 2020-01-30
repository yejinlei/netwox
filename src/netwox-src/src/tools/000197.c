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
netwib_conststring t000197_description[] = {
  "This tool is a whois client.",
  "It obtains information about an object (generally a domain name).",
  "It tries to guess the best whois server.",
  "For example: netwox 197 --query enterprise.fr",
  "",
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000197_args[] = {
  NETWOX_TOOLARG_REQ_BUF_HOSTNAME('q', "query", "information to query", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in milliseconds",
                             "60000"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000197_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_WHOIS,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000197_info = {
  "WHOIS client guessing server",
  t000197_description,
  NULL,
  t000197_args,
  t000197_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000197_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf query;
  netwib_io *pio;
  netwib_ip serverip;
  netwib_uint32 maxwaitms;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000197_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'q', &query));
  netwib_er(netwox_arg_uint32(parg, 'T', &maxwaitms));

  /* connect to server */
  netwib_er(netwox_whois_chooseserver(&query, NETWIB_TRUE, &serverip));
  netwib_er(netwib_io_init_sock_tcp_cli_easy(&serverip, NETWOX_SERVICES_WHOIS,
                                             &pio));

  /* query */
  netwib_er(netwox_whois_query(pio, &query, maxwaitms));

  /* close */
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
