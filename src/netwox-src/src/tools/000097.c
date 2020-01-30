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
netwib_conststring t000097_description[] = {
  "This tool sends a message to a SYSLOG server. It permits to log an",
  "event.",
  "",
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000097_args[] = {
  NETWOX_SOCK_ARG_UDP_CLIPORT("514"),
  NETWOX_TOOLARG_OPT_UINT32('y', "priority", "message's priority", NULL),
  NETWOX_TOOLARG_OPT_BUF('m', "message", "message", "hello"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000097_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_UDP_SYSLOG,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000097_info = {
  "SYSLOG client",
  t000097_description,
  NULL,
  t000097_args,
  t000097_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000097_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_io *pio;
  netwib_ip localip;
  netwox_syslog_priority priority;
  netwib_buf msg, buf;
  netwib_time t;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000097_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_udp_cli(parg, &sockinfo));
  netwib_er(netwox_arg_uint32(parg, 'y', &priority));
  netwib_er(netwox_arg_buf(parg, 'm', &msg));
  netwib_er(netwox_sock_init(&sockinfo, &pio));

  /* send message */
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_sock_ctl_get_local(pio, &localip, NULL));
  netwib_er(netwox_syslog_msg_init(priority, &t, NULL, &localip, &msg, &buf));
  netwib_er(netwib_io_write(pio, &buf));
  netwib_er(netwib_buf_close(&buf));

  /* close */
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}

