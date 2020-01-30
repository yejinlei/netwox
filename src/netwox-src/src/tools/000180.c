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
netwib_conststring t000180_description[] = {
  "This tool queries a NTP/SNTP server in order to obtain time.",
  "We use SNTP because it is sufficient for requesting time.",
  "Displayed time is value of field Transmit_Timestamp from server,",
  "without adding the trip delay from server to client. So time is not",
  "really accurate, but it should be sufficient for most usages.",
  "Then, it can be used to set local clock (for example, under Unix,",
  "\"date --set result_of_netwox_180_-u\").",
  "",
  NETWOX_DESC_client,
  "",
  "Set --src-port to a random value greater than 1024 to use it",
  "without admin privilege.",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000180_args[] = {
  NETWOX_SOCK_ARG_UDP_CLIPORT("123"),
  NETWOX_TOOLARG_OPTA_UINT32('V', "version", "protocol version", "3"),
  NETWOX_TOOLARG_RADIO1_SET('r', "disp-rfc822", "display rfc822 date"),
  NETWOX_TOOLARG_RADIO1('u', "disp-unixdate", "display unix date(1) format"),
  NETWOX_TOOLARG_RADIO1('h', "disp-human", "display human readable time"),
  NETWOX_TOOLARG_RADIO1('s', "disp-sec", "display seconds"),
  NETWOX_TOOLARG_RADIO1('n', "disp-secnsec",
                        "display seconds and nanoseconds"),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in milliseconds",
                             "2000"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000180_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_UDP_NTP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000180_info = {
  "SNTP client obtaining time",
  t000180_description,
  "date",
  t000180_args,
  t000180_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000180_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_buf pkt, msg;
  netwox_ntphdr ntphdr;
  netwib_io *pio;
  netwib_uint32 maxwaitms, version;
  netwib_time t;
  netwib_char c;
  netwib_bool event;
  netwib_err ret=NETWIB_ERR_OK;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000180_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_udp_cli(parg, &sockinfo));
  if (sockinfo.localport == 0) {
    sockinfo.localport = NETWOX_SERVICES_NTP;
  }
  netwib_er(netwox_arg_uint32(parg, 'V', &version));
  netwib_er(netwox_arg_radio1(parg, &c));
  netwib_er(netwox_arg_uint32(parg, 'T', &maxwaitms));

  /* prepare io */
  netwib_er(netwox_sock_init(&sockinfo, &pio));

  /* send a request */
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwox_ntphdr_initdefault(&ntphdr));
  ntphdr.ntpversion = version;
  ntphdr.ntpmode = NETWOX_NTPMODE_CLIENT;
  netwib_er(netwox_pkt_append_ntphdr(&ntphdr, &pkt));
  netwib_er(netwib_io_write(pio, &pkt));

  /* read answer */
  netwib_er(netwib_buf_init_mallocdefault(&msg));
  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_plus_msec(&t, maxwaitms));
  while (NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();
    netwib_er(netwib_io_wait_read(pio, &t, &event));
    if (!event) {
      ret = NETWOX_ERR_TIMEOUT;
      break;
    }
    netwib__buf_reinit(&pkt);
    ret = netwib_io_read(pio, &pkt);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    ret = netwox_pkt_decode_ntphdr(&pkt, &ntphdr);
    if (ret == NETWIB_ERR_NOTCONVERTED || ret == NETWIB_ERR_DATAMISSING) {
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    if (ntphdr.ntpversion != version) {
      continue;
    }
    if (ntphdr.ntpmode != NETWOX_NTPMODE_SERVER) {
      continue;
    }
    netwib_er(netwox_time_init_ntptimestamp(&ntphdr.transts, &t));
    switch(c) {
    case 'r' :
      netwib_er(netwox_date_rfc822(&t, &msg));
      break;
    case 'u' :
      netwib_er(netwox_date_unixdate(&t, &msg));
      break;
    case 'h' :
      netwib_er(netwox_date_human(&t, &msg));
      break;
    case 's' :
      netwib_er(netwox_date_sec(&t, &msg));
      break;
    case 'n' :
      netwib_er(netwox_date_secnsec(&t, &msg));
      break;
    }
    netwib_er(netwib_buf_append_byte('\n', &msg));
    netwib_er(netwib_buf_display(&msg, NETWIB_ENCODETYPE_DATA));
    break;
  }

  /* close */
  netwib_er(netwib_buf_close(&msg));
  netwib_er(netwib_buf_close(&pkt));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(ret);
}
