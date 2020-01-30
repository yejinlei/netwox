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
netwib_conststring t000104_description[] = {
  "This tool creates a simple DNS server. It will always return the same",
  "information. It can be used to redirect network flow to a computer.",
  "",
  NETWOX_DESC_dns_hostns,
  "",
  NETWOX_DESC_server,
  NETWOX_DESC_toolpriv_port1024,
  NULL
};
netwox_toolarg t000104_args[] = {
  NETWOX_SOCK_ARG_UDP_MULSERPORT("53"),
  NETWOX_TOOLARG_REQ_BUF_HOSTNAME('h', "hostname", "hostname",
                                  "www.example.com"),
  NETWOX_TOOLARG_REQ_IP('H', "hostnameip", "hostname IP", "1.2.3.4"),
  NETWOX_TOOLARG_REQ_BUF_HOSTNAME('a', "authns", "authoritative name server",
                                  "ns.example.com"),
  NETWOX_TOOLARG_REQ_IP('A', "authnsip", "authns IP", "1.2.3.5"),
  NETWOX_TOOLARG_OPTA_UINT32('T', "ttl", "ttl in seconds", "10"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000104_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SERVER_UDP_DNS,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000104_info = {
  "DNS server always answering same values",
  t000104_description,
  "bind",
  t000104_args,
  t000104_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000104_srv(netwib_bufpool *pbufpool,
                              netwib_io *pio,
                              netwib_constbuf *phostname,
                              netwib_constip *phostnameip,
                              netwib_constbuf *pauthns,
                              netwib_constip *pauthnsip,
                              netwib_uint32 ttl)
{
  netwib_buf buf1, buf2;
  netwib_wait *pwait;
  netwib_err ret=NETWIB_ERR_OK;

  /* initialize wait */
  netwib_er(netwib_wait_init_io_read(pio, &pwait));

  /* initialize buffers */
  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));

  /* main loop */
  while (NETWIB_TRUE) {
    /* wait */
    netwib_er(netwib_wait_wait1(pwait, NETWIB_TIME_INFINITE, NULL));
    /* read */
    netwib__buf_reinit(&buf1);
    ret = netwib_io_read(pio, &buf1);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      /* continue */
    } else if (ret == NETWIB_ERR_DATAEND) {
      ret = NETWIB_ERR_OK;
      break;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    } else {
      netwib_er(netwox_pkt_dns_display(pbufpool, &buf1, NULL,
                                       NETWIB_ENCODETYPE_ARRAY,
                                       NETWIB_ENCODETYPE_DUMP));
      netwib__buf_reinit(&buf2);
      ret = netwox_dnspktex_answer(&buf1, NETWIB_FALSE, phostname,
                                   phostnameip, pauthns, pauthnsip, ttl,&buf2);
      if (ret == NETWIB_ERR_OK) {
        netwib_er(netwox_pkt_dns_display(pbufpool, &buf2, NULL,
                                         NETWIB_ENCODETYPE_ARRAY,
                                         NETWIB_ENCODETYPE_DUMP));
        netwib_er(netwib_io_write(pio, &buf2));
      }
    }
  }

  /* close */
  netwib_er(netwib_buf_close(&buf2));
  netwib_er(netwib_buf_close(&buf1));
  netwib_er(netwib_wait_close(&pwait));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000104_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_bufpool *pbufpool;
  netwox_sockinfo sockinfo;
  netwib_buf hostname, authns;
  netwib_ip hostnameip, authnsip;
  netwib_uint32 ttl;
  netwib_io *pio;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000104_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_udp_mulser(parg, &sockinfo));
  netwib_er(netwox_arg_buf(parg, 'h', &hostname));
  netwib_er(netwox_arg_ip(parg, 'H', &hostnameip));
  netwib_er(netwox_arg_buf(parg, 'a', &authns));
  netwib_er(netwox_arg_ip(parg, 'A', &authnsip));
  netwib_er(netwox_arg_uint32(parg, 'T', &ttl));

  /* main loop */
  netwib_er(netwox_sock_init(&sockinfo, &pio));
  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  netwib_er(t000104_srv(pbufpool, pio, &hostname, &hostnameip, &authns,
                        &authnsip, ttl));
  netwib_er(netwib_bufpool_close(&pbufpool));

  /* close */
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
