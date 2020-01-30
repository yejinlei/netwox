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
netwib_conststring t000188_description[] = {
  "This tool is a SYSLOG client showing messages (--showscreen) or",
  "logging them in a file (--logfile).",
  "",
  NETWOX_DESC_server,
  NETWOX_DESC_allowedclients,
  NETWOX_DESC_toolpriv_port1024,
  NULL
};
netwox_toolarg t000188_args[] = {
  NETWOX_SOCK_ARG_UDP_MULSERPORT("514"),
  NETWOX_TOOLARG_OPT_BOOL('s', "showscreen", "show data to screen", "1"),
  NETWOX_TOOLARG_OPT_BUF_FILE_WR('l', "logfile", "log file", NULL),
  NETWOX_TOOLARG_OPTA_IPS_SU('c', "allowed-clients",
                             "clients allowed to connect", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000188_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SERVER_UDP_SYSLOG,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000188_info = {
  "SYSLOG server",
  t000188_description,
  "syslogd",
  t000188_args,
  t000188_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000188_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf filename, buf;
  netwox_sockinfo sockinfo;
  netwib_io *pio, *pfileio;
  netwib_ips *pallowedclients;
  netwib_ip remoteip;
  netwib_bool showscreen, isset, allowed;
  netwib_err ret=NETWIB_ERR_OK;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000188_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_udp_mulser(parg, &sockinfo));
  netwib_er(netwox_sock_init(&sockinfo, &pio));
  netwib_er(netwox_arg_bool(parg, 's', &showscreen));
  pfileio = NULL;
  netwib_er(netwox_arg_isset(parg, 'l', &isset));
  if (isset) {
    netwib_er(netwox_arg_buf(parg, 'l', &filename));
    netwib_er(netwib_io_init_file_write(&filename, &pfileio));
  }
  pallowedclients= NULL;
  netwib_er(netwox_arg_isset(parg, 'c', &isset));
  if (isset) {
    netwib_er(netwox_arg_ips(parg, 'c', &pallowedclients));
  }

  /* main loop */
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  while (NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();
    /* read */
    netwib__buf_reinit(&buf);
    ret = netwib_io_read(pio, &buf);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      /* continue */
    } else if (ret == NETWIB_ERR_DATAEND) {
      ret = NETWIB_ERR_OK;
      break;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    } else {
      netwib_er(netwib_sock_ctl_get_remote(pio, &remoteip, NULL));
      if (pallowedclients != NULL) {
        netwib_er(netwib_ips_contains_ip(pallowedclients, &remoteip,
                                         &allowed));
        if (!allowed) {
          continue;
        }
      }
      netwib_er(netwib_buf_append_fmt(&buf, " [%{ip}]\n", &remoteip));
      if (showscreen) {
        netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
      }
      if (pfileio != NULL) {
        netwib_er(netwib_io_write(pfileio, &buf));
      }
    }
  }
  netwib_er(netwib_buf_close(&buf));

  /* close */
  if (pfileio != NULL) {
    netwib_er(netwib_io_close(&pfileio));
  }
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(ret);
}
