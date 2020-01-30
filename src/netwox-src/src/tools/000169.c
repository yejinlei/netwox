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
netwib_conststring t000169_description[] = {
  "This tool displays local configuration.",
  "It's easier to parse than output of tool 1.",
  "Each line corresponds to an IP address. Thay have 3 columns: device",
  "name, IP address and Ethernet address (or text 'notether').",
  NETWOX_DESC_toolpriv_conf,
  NULL
};
netwox_toolarg t000169_args[] = {
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000169_nodes[] = {
  NETWOX_TOOLTREENODETYPE_INFO_LOCAL,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000169_info = {
  "Display simple network configuration easy to parse",
  t000169_description,
  NULL,
  t000169_args,
  t000169_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000169_loop_ip(netwib_conf_devices *pdevconf)
{
  netwib_conf_ip conf;
  netwib_conf_ip_index *pconfindex;
  netwib_err ret;

  netwib_er(netwib_conf_ip_index_init(&conf, &pconfindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_conf_ip_index_next(pconfindex);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    if (conf.devnum != pdevconf->devnum) {
      continue;
    }
    netwib_er(netwib_fmt_display("%{buf} %{ip}", &pdevconf->deviceeasy,
                                 &conf.ip));
    if (pdevconf->hwtype == NETWIB_DEVICE_HWTYPE_ETHER) {
      netwib_er(netwib_fmt_display(" %{eth}\n", &pdevconf->eth));
    } else {
      netwib_er(netwib_fmt_display(" notether\n"));
    }
  }
  netwib_er(netwib_conf_ip_index_close(&pconfindex));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err t000169_loop(void)
{
  netwib_conf_devices conf;
  netwib_conf_devices_index *pconfindex;
  netwib_err ret;

  netwib_er(netwib_conf_devices_index_init(&conf, &pconfindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_conf_devices_index_next(pconfindex);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_er(t000169_loop_ip(&conf));
  }
  netwib_er(netwib_conf_devices_index_close(&pconfindex));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000169_core(int argc, char *argv[])
{
  netwox_arg *parg;

  netwib_er(netwox_arg_init(argc, argv, &t000169_info, &parg));

  netwib_er(t000169_loop());

  netwib_er(netwox_arg_close(&parg));
  return(NETWIB_ERR_OK);
}
