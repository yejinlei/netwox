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
netwib_conststring t000013_description[] = {
  "This tool reports:",
  " - if raw IP4 spoof is supported",
  " - if raw IP6 spoof is supported",
  " - for each device:",
  "    + if sniff is supported, and its DLT (data link type)",
  "    + if spoof is supported, and its DLT (data link type)",
  NETWOX_DESC_toolpriv_snsp,
  NULL
};
netwox_toolarg t000013_args[] = {
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000013_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SNIFF,
  NETWOX_TOOLTREENODETYPE_SPOOF_ETH,
  NETWOX_TOOLTREENODETYPE_SPOOF_IP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000013_info = {
  "Obtain DLT type for sniff and spoof for each device",
  t000013_description,
  "capture, device",
  t000013_args,
  t000013_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000013_display(void)
{
  netwib_conf_devices conf;
  netwib_conf_devices_index *pconfindex;
  netwib_device_dlttype dlt;
  netwib_buf buf;
  netwib_io *pio;
  netwib_err ret;

  /* spoof IP4 */
  ret = netwib_io_init_spoof_ip4(&pio);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_fmt_display("Spoof ip4 supported\n"));
    netwib_er(netwib_spoof_ctl_get_dlt(pio, &dlt));
    if (dlt != NETWIB_DEVICE_DLTTYPE_RAW) {
      netwib_er(netwib_fmt_display(" dlt=%{uint32} != wanted\n", dlt));
    }
    netwib_er(netwib_io_close(&pio));
  } else if (ret == NETWIB_ERR_LONOTSUPPORTED) {
    netwib_er(netwib_fmt_display("Spoof ip4 not supported\n"));
  } else {
    netwib_er(netwib_err_display(ret, NETWIB_ERR_ENCODETYPE_FULL));
  }

  /* spoof IP6 */
  ret = netwib_io_init_spoof_ip6(&pio);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_fmt_display("Spoof ip6 supported\n"));
    netwib_er(netwib_spoof_ctl_get_dlt(pio, &dlt));
    if (dlt != NETWIB_DEVICE_DLTTYPE_RAW) {
      netwib_er(netwib_fmt_display(" dlt=%{uint32} != wanted\n", dlt));
    }
    netwib_er(netwib_io_close(&pio));
  } else if (ret == NETWIB_ERR_LONOTSUPPORTED) {
    netwib_er(netwib_fmt_display("Spoof ip6 not supported\n"));
  } else {
    netwib_er(netwib_err_display(ret, NETWIB_ERR_ENCODETYPE_FULL));
  }

  /* DLT */
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_conf_devices_index_init(&conf, &pconfindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_conf_devices_index_next(pconfindex);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib__buf_reinit(&buf);
    netwib_er(netwib_buf_append_device_hwtype(conf.hwtype, &buf));
    netwib_er(netwib_fmt_display("Dev %{buf} of hwtype=%{uint32}=%{buf} :\n",
                               &conf.device, conf.hwtype, &buf));
    /* sniff */
    ret = netwib_io_init_sniff(&conf.device, NULL, &pio);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_sniff_ctl_get_dlt(pio, &dlt));
      netwib__buf_reinit(&buf);
      netwib_er(netwib_buf_append_device_dlttype(dlt, &buf));
      netwib_er(netwib_fmt_display(" sniff dlt=%{uint32}=%{buf}\n", dlt,
                                   &buf));
      netwib_er(netwib_io_close(&pio));
    } else if (ret == NETWIB_ERR_LONOTSUPPORTED) {
      netwib_er(netwib_fmt_display(" sniff not supported\n"));
    } else {
      netwib_er(netwib_err_display(ret, NETWIB_ERR_ENCODETYPE_FULL));
    }
    /* spoof */
    ret = netwib_io_init_spoof_link(&conf.device, &pio);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_spoof_ctl_get_dlt(pio, &dlt));
      netwib__buf_reinit(&buf);
      netwib_er(netwib_buf_append_device_dlttype(dlt, &buf));
      netwib_er(netwib_fmt_display(" spoof dlt=%{uint32}=%{buf}\n", dlt,
                                   &buf));
      netwib_er(netwib_io_close(&pio));
    } else if (ret == NETWIB_ERR_LONOTSUPPORTED) {
      netwib_er(netwib_fmt_display(" spoof not supported\n"));
    } else {
      netwib_er(netwib_err_display(ret, NETWIB_ERR_ENCODETYPE_FULL));
    }
  }
  netwib_er(netwib_conf_devices_index_close(&pconfindex));
  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000013_core(int argc, char *argv[])
{
  netwox_arg *parg;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000013_info, &parg));
  netwib_er(netwox_arg_close(&parg));

  netwib_er(t000013_display());

  return(NETWIB_ERR_OK);
}

