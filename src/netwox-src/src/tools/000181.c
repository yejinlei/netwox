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
netwib_conststring t000181_description[] = {
  "This tool is a SNTP server.",
  "It gives time to clients connecting to it.",
  "It uses no external time reference, but only local clock. All clients",
  "will synchronize on the same time, but this time might be inaccurate.",
  "",
  NETWOX_DESC_server,
  NETWOX_DESC_toolpriv_port1024,
  NULL
};
netwox_toolarg t000181_args[] = {
  NETWOX_SOCK_ARG_UDP_MULSERPORT("123"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000181_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SERVER_UDP_NTP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000181_info = {
  "SNTP server",
  t000181_description,
  NULL,
  t000181_args,
  t000181_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000181_loop(netwib_io *pio)
{
  netwib_buf pkt;
  netwox_ntphdr ntphdr1, ntphdr2;
  netwib_time t;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  while (NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();
    /* read */
    netwib__buf_reinit(&pkt);
    ret = netwib_io_read(pio, &pkt);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* decode a request */
    ret = netwox_pkt_decode_ntphdr(&pkt, &ntphdr1);
    if (ret == NETWIB_ERR_NOTCONVERTED || ret == NETWIB_ERR_DATAMISSING) {
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    if (ntphdr1.ntpversion != NETWOX_NTPVERSION_1 &&
        ntphdr1.ntpversion != NETWOX_NTPVERSION_2 &&
        ntphdr1.ntpversion != NETWOX_NTPVERSION_3 &&
        ntphdr1.ntpversion != NETWOX_NTPVERSION_4) {
      continue;
    }
    if (ntphdr1.ntpmode != NETWOX_NTPMODE_CLIENT) {
      continue;
    }
    /* send a reply */
    netwib__buf_reinit(&pkt);
    netwib_er(netwox_ntphdr_initdefault(&ntphdr2));
    ntphdr2.ntpversion = ntphdr1.ntpversion;
    ntphdr2.ntpmode = NETWOX_NTPMODE_SERVER;
    ntphdr2.stratum = 1;
    ntphdr2.pool = ntphdr1.pool;
    ntphdr2.precision = -6; /* 2^-6 = 1/2^6 = 1/64 = 15ms */
    ntphdr2.refid[0] = 'L';
    ntphdr2.refid[1] = 'O';
    ntphdr2.refid[2] = 'C';
    ntphdr2.refid[3] = 'L';
    netwib_er(netwib_time_init_now(&t));
    netwib_er(netwox_ntptimestamp_init_time(&t, &ntphdr2.refts));
    ntphdr2.origts = ntphdr1.transts;
    ntphdr2.recvts = ntphdr2.refts;
    ntphdr2.transts = ntphdr2.refts;
    netwib_er(netwox_pkt_append_ntphdr(&ntphdr2, &pkt));
    netwib_er(netwib_io_write(pio, &pkt));
  }

  netwib_er(netwib_buf_close(&pkt));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000181_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_io *pio;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000181_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_udp_mulser(parg, &sockinfo));
  netwib_er(netwox_sock_init(&sockinfo, &pio));

  /* loop */
  netwib_er(t000181_loop(pio));

  /* close */
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
