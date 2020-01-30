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
netwib_conststring t000179_description[] = {
  "This tool sends an INFORM message requesting DHCP servers to tell us",
  "several parameters (gateway, dns server, etc.).",
  "",
  "Parameters --device and --eth-src and --ip-src permit to send message",
  "using a fake address.",
  NETWOX_DESC_toolpriv_snsp,
  NULL
};
netwox_toolarg t000179_args[] = {
  NETWOX_TOOLARG_OPT_BUF_DEVICE('d', NULL, "device for spoof", NULL),
  NETWOX_TOOLARG_OPT_ETH_SRC('e', "eth-src", "Ethernet src", NULL),
  NETWOX_TOOLARG_OPT_IP_SRC('i', "ip-src", "IP src", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in ms", "30000"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000179_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_UDP_DHCP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000179_info = {
  "DHCP client requesting an INFORM",
  t000179_description,
  NULL,
  t000179_args,
  t000179_nodes,
};

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bufpool *pbufpool;
  netwib_io *pio;
  netwib_uint32 timeoutms;
  netwib_eth clienteth;
  netwib_ip clientip;
  netwox_dhcp4opt_clientid clientid;
  netwib_uint32 xid;
  netwib_uint32 extendleasetime;
} t000179_infos;

/*-------------------------------------------------------------*/
static netwib_err t000179_send_inform(t000179_infos *pinfos)
{
  netwib_linkhdr linkheader;
  netwib_iphdr ipheader;
  netwib_udphdr udpheader;
  netwib_buf *pdhcp4opts, *pudpdata, *ppkt;
  netwox_dhcp4hdr dhcp4hdr;
  netwox_dhcp4opt dhcp4opt;
  netwib_err ret;

  netwib_er(netwib_linkhdr_initdefault(NETWIB_DEVICE_DLTTYPE_ETHER,
                                       &linkheader));
  linkheader.hdr.ether.src = pinfos->clienteth;
  netwib_er(netwib_eth_init_fields(0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                                   &linkheader.hdr.ether.dst));

  netwib_er(netwib_iphdr_initdefault(NETWIB_IPTYPE_IP4, &ipheader));
  ipheader.src = pinfos->clientip;
  netwib_er(netwib_ip_init_ip4(0xFFFFFFFFu, &ipheader.dst));

  netwib_er(netwib_udphdr_initdefault(&udpheader));
  udpheader.src = NETWOX_SERVICES_DHCP4C;
  udpheader.dst = NETWOX_SERVICES_DHCP4S;

  netwib_er(netwox_dhcp4hdr_initdefault(&dhcp4hdr));
  dhcp4hdr.dhcp4opcode = NETWOX_DHCP4OPCODE_BOOTREQUEST;
  pinfos->xid = dhcp4hdr.xid;
  dhcp4hdr.clienteth = pinfos->clienteth;
  dhcp4hdr.clientip = pinfos->clientip;
  netwib_er(netwib_bufpool_buf_init(pinfos->pbufpool, &pdhcp4opts));
  netwib_er(netwox_dhcp4opt_initdefault(NETWOX_DHCP4OPTTYPE_MSGTYPE,
                                        &dhcp4opt));
  dhcp4opt.opt.msgtype = NETWOX_DHCP4OPT_MSGTYPE_INFORM;
  netwib_er(netwox_pkt_append_dhcp4opt(&dhcp4opt, pdhcp4opts));
  netwib_er(netwox_dhcp4opt_initdefault(NETWOX_DHCP4OPTTYPE_CLIENTID,
                                        &dhcp4opt));
  dhcp4opt.opt.clientid = pinfos->clientid;
  netwib_er(netwox_pkt_append_dhcp4opt(&dhcp4opt, pdhcp4opts));
  /* Windows needs us to request what we want */
  netwib_er(netwox_dhcp4opt_reqlist_initall(&dhcp4opt));
  netwib_er(netwox_pkt_append_dhcp4opt(&dhcp4opt, pdhcp4opts));
  netwib_er(netwib_buf_init_ext_buf(pdhcp4opts, &dhcp4hdr.opts));

  netwib_er(netwib_bufpool_buf_init(pinfos->pbufpool, &pudpdata));
  netwib_er(netwox_pkt_append_dhcp4hdr(&dhcp4hdr, pudpdata));
  netwib_er(netwib_bufpool_buf_close(pinfos->pbufpool, &pdhcp4opts));
  netwib_er(netwib_fmt_display("I send a INFORM:\n"));
  netwib_er(netwox_pkt_dhcp4_display(pinfos->pbufpool, pudpdata, NULL,
                                     NETWIB_ENCODETYPE_ARRAY,
                                     NETWIB_ENCODETYPE_DUMP));

  netwib_er(netwib_bufpool_buf_init(pinfos->pbufpool, &ppkt));
  netwib_er(netwib_pkt_append_linkipudpdata(&linkheader, &ipheader,
                                            &udpheader, pudpdata,
                                            ppkt));
  netwib_er(netwib_bufpool_buf_close(pinfos->pbufpool, &pudpdata));

  ret = netwib_io_write(pinfos->pio, ppkt);

  netwib_er(netwib_bufpool_buf_close(pinfos->pbufpool, &ppkt));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err t000179_recv_ack(t000179_infos *pinfos)
{
  netwib_buf *ppkt;
  netwib_linkhdr linkheader;
  netwib_iphdr ipheader;
  netwib_udphdr udpheader;
  netwib_buf udpdata;
  netwox_dhcp4hdr dhcp4hdr;
  netwox_dhcp4optscommon dhcp4optscommon;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pinfos->pbufpool, &ppkt));

  ret = NETWIB_ERR_OK;
  netwib_eg(netwib_io_read(pinfos->pio, ppkt));

  /* check it's for us */
  netwib_eg(netwib_pkt_decode_linkipudpdata(NETWIB_DEVICE_DLTTYPE_ETHER,
                                            ppkt, &linkheader, &ipheader,
                                            &udpheader, &udpdata));
  /* cannot check ether.dst:
      - Linux DHCP server sets to client,
      - Windows DHCP server sets to FF:FF:FF:FF:FF:FF
  */
  /* cannot check ipheader.dst:
      - Linux DHCP server sets to client,
      - Windows DHCP server sets to 255.255.255.255
  */
  if (udpheader.src != NETWOX_SERVICES_DHCP4S) {
    netwib_goto(NETWIB_ERR_NOTCONVERTED);
  }
  if (udpheader.dst != NETWOX_SERVICES_DHCP4C) {
    netwib_goto(NETWIB_ERR_NOTCONVERTED);
  }
  netwib_eg(netwox_pkt_decode_dhcp4hdr(&udpdata, &dhcp4hdr));
  if (dhcp4hdr.xid != pinfos->xid) {
    netwib_goto(NETWIB_ERR_NOTCONVERTED);
  }
  netwib_eg(netwox_dhcp4opts_decode_common(&dhcp4hdr.opts, &dhcp4optscommon));
  if (!dhcp4optscommon.msgtypeset) {
    netwib_goto(NETWIB_ERR_NOTCONVERTED);
  }
  if (dhcp4optscommon.msgtype != NETWOX_DHCP4OPT_MSGTYPE_ACK) {
    netwib_goto(NETWIB_ERR_NOTCONVERTED);
  }

  /* ok, it's for us */
  netwib_er(netwib_fmt_display("\nServer sent us this ACK:\n"));
  netwib_eg(netwox_pkt_dhcp4_display(pinfos->pbufpool, &udpdata, NULL,
                                     NETWIB_ENCODETYPE_ARRAY,
                                     NETWIB_ENCODETYPE_DUMP));

  /* save ipadleasetime field */
  pinfos->extendleasetime = 30;
  if (dhcp4optscommon.ipadleasetimeset) {
    pinfos->extendleasetime = dhcp4optscommon.ipadleasetime / 3;
  }

  /* end */
 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(pinfos->pbufpool, &ppkt));
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err t000179_loop(t000179_infos *pinfos)
{
  netwib_time t;
  netwib_bool event;
  netwib_err ret;

  /* send INFORM (if lost in transit, it is not resent) */
  netwib_er(t000179_send_inform(pinfos));

  /* expect a good ACK within the time limit */
  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_plus_msec(&t, pinfos->timeoutms));
  while(NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();
    netwib_er(netwib_io_wait_read(pinfos->pio, &t, &event));
    if (!event) {
      return(NETWOX_ERR_TIMEOUT);
    }
    ret = t000179_recv_ack(pinfos);
    if (ret == NETWIB_ERR_OK) {
      break;
    } else if (ret == NETWIB_ERR_NOTCONVERTED ||
               ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else {
      return(ret);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err t000179_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf device, filter;
  t000179_infos infos;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000179_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'd', &device));
  netwib_er(netwox_arg_eth(parg, 'e', &infos.clienteth));
  netwib_er(netwox_arg_ip(parg, 'i', &infos.clientip));
  netwib_er(netwox_arg_uint32(parg, 'T', &infos.timeoutms));
  netwib_er(netwib_buf_init_ext_string("udp and (port 67 or port 68)", &filter));
  netwib_er(netwox_snispo_init_eth(&device, &filter, &infos.pio));
  netwib_er(netwib_bufpool_initdefault(&infos.pbufpool));

  /* define our clientid */
  infos.clientid.type = 1;
  netwib_er(netwib_buf_init_ext_arrayfilled(infos.clienteth.b, NETWIB_ETH_LEN,
                                            &infos.clientid.id));

  /* main loop */
  netwib_er(t000179_loop(&infos));

  /* close */
  netwib_er(netwib_bufpool_close(&infos.pbufpool));
  netwib_er(netwib_io_close(&infos.pio));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}

