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
netwib_conststring t000105_description[] = {
  "This tool sniffs and replies to DNS requests. It will always return",
  "the same information. It can be used to redirect network flow to a",
  "computer.",
  "The 'hostname' parameter is not used for type A queries.",
  "The 'hostnameip' parameter is not used for type PTR queries.",
  "",
  NETWOX_DESC_dns_hostns,
  "",
  NETWOX_DESC_sniff_devfilter,
  NETWOX_DESC_spoofip,
  NETWOX_DESC_toolpriv_snsp,
  NULL
};
netwox_toolarg t000105_args[] = {
  NETWOX_TOOLARG_REQ_BUF_HOSTNAME('h', "hostname", "hostname",
                                  "www.example.com"),
  NETWOX_TOOLARG_REQ_IP('H', "hostnameip", "hostname IP", "1.2.3.4"),
  NETWOX_TOOLARG_REQ_BUF_HOSTNAME('a', "authns", "authoritative name server",
                                  "ns.example.com"),
  NETWOX_TOOLARG_REQ_IP('A', "authnsip", "authns IP", "1.2.3.5"),
  NETWOX_TOOLARG_OPTA_UINT32('T', "ttl", "ttl in seconds", "10"),
  NETWOX_TOOLARG_OPT_BUF_DEVICE('d', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPTA_BUF_FILTER('f', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPTA_SPOOFIP('s', NULL, NULL, NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000105_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SERVER_UDP_DNS,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000105_info = {
  "Sniff and send DNS answers",
  t000105_description,
  "bind",
  t000105_args,
  t000105_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000105_srv2(netwib_bufpool *pbufpool,
                               netwib_io *pio,
                               netwib_constbuf *ppkt,
                               netwib_constbuf *phostname,
                               netwib_constip *phostnameip,
                               netwib_constbuf *pauthns,
                               netwib_constip *pauthnsip,
                               netwib_uint32 ttl)
{
  netwib_buf buf, buf2, udpdata;
  netwib_iphdr ipheader;
  netwib_udphdr udpheader;
  netwib_ip ipad;
  netwib_port port;
  netwib_err ret;

  ret = netwib_pkt_decode_ipudpdata(ppkt, &ipheader, &udpheader, &udpdata);
  if (ret != NETWIB_ERR_OK) {
    if (ret == NETWIB_ERR_NOTCONVERTED) ret = NETWIB_ERR_OK;
    if (ret == NETWIB_ERR_DATAMISSING) ret = NETWIB_ERR_OK;
    if (ret == NETWIB_ERR_LONOTIMPLEMENTED) ret = NETWIB_ERR_OK;
    return(ret);
  }

  if (udpheader.src != NETWOX_SERVICES_DOMAIN &&
      udpheader.dst != NETWOX_SERVICES_DOMAIN) {
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwox_pkt_dns_display(pbufpool, &udpdata, NULL,
                                   NETWIB_ENCODETYPE_ARRAY,
                                   NETWIB_ENCODETYPE_DUMP));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  ret = netwox_dnspktex_answer(&udpdata, NETWIB_TRUE, phostname,
                               phostnameip, pauthns, pauthnsip, ttl, &buf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwox_pkt_dns_display(pbufpool, &buf, NULL,
                                     NETWIB_ENCODETYPE_ARRAY,
                                     NETWIB_ENCODETYPE_DUMP));
    /* quick swap for easy answer */
    ipad = ipheader.src;
    ipheader.src = ipheader.dst;
    ipheader.dst = ipad;
    port = udpheader.src;
    udpheader.src = udpheader.dst;
    udpheader.dst = port;
    netwib_er(netwib_buf_init_mallocdefault(&buf2));
    ret = netwib_pkt_append_ipudpdata(&ipheader, &udpheader, &buf, &buf2);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_io_write(pio, &buf2));
    }
    netwib_er(netwib_buf_close(&buf2));
  }
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000105_srv(netwib_bufpool *pbufpool,
                              netwib_io *pio,
                              netwib_constbuf *phostname,
                              netwib_constip *phostnameip,
                              netwib_constbuf *pauthns,
                              netwib_constip *pauthnsip,
                              netwib_uint32 ttl)
{
  netwib_buf buf;
  netwib_wait *pwait;
  netwib_err ret=NETWIB_ERR_OK;

  /* initialize wait */
  netwib_er(netwib_wait_init_io_read(pio, &pwait));

  /* initialize buffers */
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  /* main loop */
  while (NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();

    /* wait */
    netwib_er(netwib_wait_wait1(pwait, NETWIB_TIME_INFINITE, NULL));
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
      ret = t000105_srv2(pbufpool, pio, &buf, phostname, phostnameip, pauthns,
                         pauthnsip, ttl);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
    }
  }

  /* close */
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_wait_close(&pwait));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000105_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_bufpool *pbufpool;
  netwib_buf hostname, authns, device, filter, filter2;
  netwib_ip hostnameip, authnsip;
  netwib_spoof_ip_inittype spoofipinittype;
  netwib_uint32 ttl;
  netwib_io *pio;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000105_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'h', &hostname));
  netwib_er(netwox_arg_ip(parg, 'H', &hostnameip));
  netwib_er(netwox_arg_buf(parg, 'a', &authns));
  netwib_er(netwox_arg_ip(parg, 'A', &authnsip));
  netwib_er(netwox_arg_uint32(parg, 'T', &ttl));
  netwib_er(netwox_arg_buf(parg, 'd', &device));
  netwib_er(netwox_arg_buf(parg, 'f', &filter));
  netwib_er(netwox_arg_spoofip(parg, 's', &spoofipinittype));

  /* main loop */
  netwib_er(netwib_buf_init_mallocdefault(&filter2));
  if (netwib__buf_ref_data_size(&filter)) {
    netwib_er(netwib_buf_append_fmt(&filter2, "(%{buf}) and ", &filter));
  }
  netwib_er(netwib_buf_append_string("udp and port 53", &filter2));
  netwib_er(netwox_snispo_init_ip(&device, &filter2, NETWIB_TRUE, NETWIB_FALSE,
                                  spoofipinittype, &pio));
  netwib_er(netwib_buf_close(&filter2));
  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  netwib_er(t000105_srv(pbufpool, pio, &hostname, &hostnameip, &authns,
                        &authnsip, ttl));
  netwib_er(netwib_bufpool_close(&pbufpool));

  /* close */
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
