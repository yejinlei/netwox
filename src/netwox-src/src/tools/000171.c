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
netwib_conststring t000171_description[] = {
  "This tool obtains an IP address from a DHCP server.",
  "It sends a DISCOVER message, and wait for server replies. The first",
  "server to answer is accepted, and its proposed address is displayed.",
  "Then, it periodically updates in order to keep this address. Finally,",
  "when user presses Q key, address ie released.",
  "This tool permits to obtain an IP address on a DHCP network, in order",
  "to temporarily configure a host not supporting DHCP.",
  "",
  "Parameters --device and --eth-src permit to send message using a fake",
  "address.",
  NETWOX_DESC_toolpriv_snsp,
  NULL
};
netwox_toolarg t000171_args[] = {
  NETWOX_TOOLARG_OPT_BUF_DEVICE('d', NULL, "device for spoof", NULL),
  NETWOX_TOOLARG_OPT_ETH_SRC('e', "eth-src", "Ethernet src", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in ms", "30000"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000171_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_UDP_DHCP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000171_info = {
  "DHCP client",
  t000171_description,
  NULL,
  t000171_args,
  t000171_nodes,
};

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bufpool *pbufpool;
  netwib_io *pio;
  netwib_uint32 timeoutms;
  netwib_eth clienteth;
  netwib_ip clientip;
  netwib_eth servereth;
  netwib_ip serverip;
  netwox_dhcp4opt_clientid clientid;
  netwib_uint32 xid;
  netwib_uint32 extendleasetime;
} t000171_infos;

/*-------------------------------------------------------------*/
static netwib_err t000171_send_discover(t000171_infos *pinfos)
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
  netwib_er(netwib_ip_init_ip4(0, &ipheader.src));
  netwib_er(netwib_ip_init_ip4(0xFFFFFFFFu, &ipheader.dst));

  netwib_er(netwib_udphdr_initdefault(&udpheader));
  udpheader.src = NETWOX_SERVICES_DHCP4C;
  udpheader.dst = NETWOX_SERVICES_DHCP4S;

  netwib_er(netwox_dhcp4hdr_initdefault(&dhcp4hdr));
  dhcp4hdr.dhcp4opcode = NETWOX_DHCP4OPCODE_BOOTREQUEST;
  pinfos->xid = dhcp4hdr.xid;
  dhcp4hdr.clienteth = pinfos->clienteth;
  netwib_er(netwib_bufpool_buf_init(pinfos->pbufpool, &pdhcp4opts));
  netwib_er(netwox_dhcp4opt_initdefault(NETWOX_DHCP4OPTTYPE_MSGTYPE,
                                        &dhcp4opt));
  dhcp4opt.opt.msgtype = NETWOX_DHCP4OPT_MSGTYPE_DISCOVER;
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
  netwib_er(netwib_fmt_display("I send a DISCOVER:\n"));
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
static netwib_err t000171_send_request_accept(t000171_infos *pinfos)
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
  netwib_er(netwib_ip_init_ip4(0, &ipheader.src));
  netwib_er(netwib_ip_init_ip4(0xFFFFFFFFu, &ipheader.dst));

  netwib_er(netwib_udphdr_initdefault(&udpheader));
  udpheader.src = NETWOX_SERVICES_DHCP4C;
  udpheader.dst = NETWOX_SERVICES_DHCP4S;

  netwib_er(netwox_dhcp4hdr_initdefault(&dhcp4hdr));
  dhcp4hdr.dhcp4opcode = NETWOX_DHCP4OPCODE_BOOTREQUEST;
  dhcp4hdr.xid = pinfos->xid; /* use previous */
  dhcp4hdr.clienteth = pinfos->clienteth;
  netwib_er(netwib_bufpool_buf_init(pinfos->pbufpool, &pdhcp4opts));
  netwib_er(netwox_dhcp4opt_initdefault(NETWOX_DHCP4OPTTYPE_MSGTYPE,
                                        &dhcp4opt));
  dhcp4opt.opt.msgtype = NETWOX_DHCP4OPT_MSGTYPE_REQUEST;
  netwib_er(netwox_pkt_append_dhcp4opt(&dhcp4opt, pdhcp4opts));
  netwib_er(netwox_dhcp4opt_initdefault(NETWOX_DHCP4OPTTYPE_CLIENTID,
                                        &dhcp4opt));
  dhcp4opt.opt.clientid = pinfos->clientid;
  netwib_er(netwox_pkt_append_dhcp4opt(&dhcp4opt, pdhcp4opts));
  netwib_er(netwox_dhcp4opt_initdefault(NETWOX_DHCP4OPTTYPE_REQUESTEDIPAD,
                                        &dhcp4opt));
  dhcp4opt.opt.requestedipad = pinfos->clientip;
  netwib_er(netwox_pkt_append_dhcp4opt(&dhcp4opt, pdhcp4opts));
  netwib_er(netwox_dhcp4opt_initdefault(NETWOX_DHCP4OPTTYPE_SERVERID,
                                        &dhcp4opt));
  dhcp4opt.opt.requestedipad = pinfos->serverip;
  netwib_er(netwox_pkt_append_dhcp4opt(&dhcp4opt, pdhcp4opts));
  /* Windows needs us to request what we want */
  netwib_er(netwox_dhcp4opt_reqlist_initall(&dhcp4opt));
  netwib_er(netwox_pkt_append_dhcp4opt(&dhcp4opt, pdhcp4opts));
  netwib_er(netwib_buf_init_ext_buf(pdhcp4opts, &dhcp4hdr.opts));

  netwib_er(netwib_bufpool_buf_init(pinfos->pbufpool, &pudpdata));
  netwib_er(netwox_pkt_append_dhcp4hdr(&dhcp4hdr, pudpdata));
  netwib_er(netwib_bufpool_buf_close(pinfos->pbufpool, &pdhcp4opts));
  netwib_er(netwib_fmt_display("\nI accept previous OFFER:\n"));
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
static netwib_err t000171_send_request_extendlease(t000171_infos *pinfos)
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
  linkheader.hdr.ether.dst = pinfos->servereth;

  netwib_er(netwib_iphdr_initdefault(NETWIB_IPTYPE_IP4, &ipheader));
  ipheader.src = pinfos->clientip;
  ipheader.dst = pinfos->serverip;

  netwib_er(netwib_udphdr_initdefault(&udpheader));
  udpheader.src = NETWOX_SERVICES_DHCP4C;
  udpheader.dst = NETWOX_SERVICES_DHCP4S;

  netwib_er(netwox_dhcp4hdr_initdefault(&dhcp4hdr));
  dhcp4hdr.dhcp4opcode = NETWOX_DHCP4OPCODE_BOOTREQUEST;
  pinfos->xid = dhcp4hdr.xid;
  dhcp4hdr.clientip = pinfos->clientip;
  dhcp4hdr.clienteth = pinfos->clienteth;
  netwib_er(netwib_bufpool_buf_init(pinfos->pbufpool, &pdhcp4opts));
  netwib_er(netwox_dhcp4opt_initdefault(NETWOX_DHCP4OPTTYPE_MSGTYPE,
                                        &dhcp4opt));
  dhcp4opt.opt.msgtype = NETWOX_DHCP4OPT_MSGTYPE_REQUEST;
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
  netwib_er(netwib_fmt_display("\nI REQUEST a lease extension:\n"));
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
static netwib_err t000171_send_release(t000171_infos *pinfos)
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
  linkheader.hdr.ether.dst = pinfos->servereth;

  netwib_er(netwib_iphdr_initdefault(NETWIB_IPTYPE_IP4, &ipheader));
  ipheader.src = pinfos->clientip;
  ipheader.dst = pinfos->serverip;

  netwib_er(netwib_udphdr_initdefault(&udpheader));
  udpheader.src = NETWOX_SERVICES_DHCP4C;
  udpheader.dst = NETWOX_SERVICES_DHCP4S;

  netwib_er(netwox_dhcp4hdr_initdefault(&dhcp4hdr));
  dhcp4hdr.dhcp4opcode = NETWOX_DHCP4OPCODE_BOOTREQUEST;
  pinfos->xid = dhcp4hdr.xid;
  dhcp4hdr.clientip = pinfos->clientip;
  dhcp4hdr.clienteth = pinfos->clienteth;
  netwib_er(netwib_bufpool_buf_init(pinfos->pbufpool, &pdhcp4opts));
  netwib_er(netwox_dhcp4opt_initdefault(NETWOX_DHCP4OPTTYPE_MSGTYPE,
                                        &dhcp4opt));
  dhcp4opt.opt.msgtype = NETWOX_DHCP4OPT_MSGTYPE_RELEASE;
  netwib_er(netwox_pkt_append_dhcp4opt(&dhcp4opt, pdhcp4opts));
  netwib_er(netwox_dhcp4opt_initdefault(NETWOX_DHCP4OPTTYPE_SERVERID,
                                        &dhcp4opt));
  dhcp4opt.opt.serverid = pinfos->serverip;
  netwib_er(netwox_pkt_append_dhcp4opt(&dhcp4opt, pdhcp4opts));
  netwib_er(netwox_dhcp4opt_initdefault(NETWOX_DHCP4OPTTYPE_CLIENTID,
                                        &dhcp4opt));
  dhcp4opt.opt.clientid = pinfos->clientid;
  netwib_er(netwox_pkt_append_dhcp4opt(&dhcp4opt, pdhcp4opts));
  netwib_er(netwib_buf_init_ext_buf(pdhcp4opts, &dhcp4hdr.opts));

  netwib_er(netwib_bufpool_buf_init(pinfos->pbufpool, &pudpdata));
  netwib_er(netwox_pkt_append_dhcp4hdr(&dhcp4hdr, pudpdata));
  netwib_er(netwib_bufpool_buf_close(pinfos->pbufpool, &pdhcp4opts));
  netwib_er(netwib_fmt_display("\nI RELEASE the address:\n"));
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
static netwib_err t000171_recv_offer(t000171_infos *pinfos)
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
  if (dhcp4optscommon.msgtype != NETWOX_DHCP4OPT_MSGTYPE_OFFER) {
    netwib_goto(NETWIB_ERR_NOTCONVERTED);
  }

  /* ok, it's for us */
  netwib_er(netwib_fmt_display("\nServer sent us this OFFER:\n"));
  netwib_eg(netwox_pkt_dhcp4_display(pinfos->pbufpool, &udpdata, NULL,
                                     NETWIB_ENCODETYPE_ARRAY,
                                     NETWIB_ENCODETYPE_DUMP));

  /* save fields */
  pinfos->servereth = linkheader.hdr.ether.src;
  pinfos->serverip = dhcp4hdr.serverip;
  pinfos->clientip = dhcp4hdr.yourip;
  netwib_er(netwib_fmt_display("Server %{ip}(%{eth}) proposes address %{ip}\n",
                               &pinfos->serverip, &pinfos->servereth,
                               &pinfos->clientip));

  /* end */
 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(pinfos->pbufpool, &ppkt));
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err t000171_recv_ack(t000171_infos *pinfos)
{
  netwib_buf *ppkt;
  netwib_linkhdr linkheader;
  netwib_iphdr ipheader;
  netwib_udphdr udpheader;
  netwib_buf udpdata;
  netwox_dhcp4hdr dhcp4hdr;
  netwox_dhcp4optscommon dhcp4optscommon;
  netwib_cmp cmp;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pinfos->pbufpool, &ppkt));

  ret = NETWIB_ERR_OK;
  netwib_eg(netwib_io_read(pinfos->pio, ppkt));

  /* check it's for us */
  netwib_eg(netwib_pkt_decode_linkipudpdata(NETWIB_DEVICE_DLTTYPE_ETHER,
                                            ppkt, &linkheader, &ipheader,
                                            &udpheader, &udpdata));
  netwib_eg(netwib_eth_cmp(&linkheader.hdr.ether.src, &pinfos->servereth,
                           &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    netwib_goto(NETWIB_ERR_NOTCONVERTED);
  }
  /* cannot check ether.dst:
      - Linux DHCP server sets to client,
      - Windows DHCP server sets to FF:FF:FF:FF:FF:FF
  */
  netwib_eg(netwib_ip_cmp(&ipheader.src, &pinfos->serverip, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    netwib_goto(NETWIB_ERR_NOTCONVERTED);
  }
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
  netwib_er(netwib_fmt_display("Server %{ip}(%{eth}) gave address %{ip}\n",
                               &pinfos->serverip, &pinfos->servereth,
                               &pinfos->clientip));

  /* end */
 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(pinfos->pbufpool, &ppkt));
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err t000171_loop_extendlease(t000171_infos *pinfos)
{
  netwib_buf *pkbd;
  netwib_io *piokbd;
  netwib_wait *pwaitsniff, *pwaitkbd;
  netwib_bool eventsniff, eventkbd;
  netwib_string kbdstr;
  netwib_time t;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_io_init_kbddefault(&piokbd));
  netwib_er(netwib_kbd_ctl_set_line(piokbd, NETWIB_FALSE));
  netwib_er(netwib_kbd_ctl_set_echo(piokbd, NETWIB_FALSE));
  netwib_er(netwib_wait_init_io_read(pinfos->pio, &pwaitsniff));
  netwib_er(netwib_wait_init_io_read(piokbd, &pwaitkbd));
  netwib_er(netwib_bufpool_buf_init(pinfos->pbufpool, &pkbd));

  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_plus_sec(&t, pinfos->extendleasetime));
  netwib_er(netwib_fmt_display("Press q to quit.\n"));
  while(NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();
    netwib_er(netwib_wait_wait2(pwaitsniff, pwaitkbd, &t,
                                &eventsniff, &eventkbd));
    if (eventkbd) {
      netwib__buf_reinit(pkbd);
      ret = netwib_io_read(piokbd, pkbd);
      if (ret == NETWIB_ERR_DATANOTAVAIL) {
        ret = NETWIB_ERR_OK;
        continue;
      } else if (ret != NETWIB_ERR_OK) {
        break;
      }
      netwib_er(netwib_buf_ref_string(pkbd, &kbdstr));
      if (kbdstr[0] == 'q' || kbdstr[0] == 'Q') {
        break;
      } else {
        netwib_er(netwib_fmt_display("Press q to quit.\n"));
      }
    } else if (eventsniff) {
      ret = t000171_recv_ack(pinfos);
      if (ret == NETWIB_ERR_NOTCONVERTED || ret == NETWIB_ERR_DATANOTAVAIL) {
        continue;
      } else if (ret != NETWIB_ERR_OK) {
        break;
      }
    } else {
      netwib_er(netwib_time_plus_sec(&t, pinfos->extendleasetime));
      netwib_er(t000171_send_request_extendlease(pinfos));
    }
  }

  netwib_er(netwib_bufpool_buf_close(pinfos->pbufpool, &pkbd));
  netwib_er(netwib_io_close(&piokbd));
  netwib_er(netwib_wait_close(&pwaitsniff));
  netwib_er(netwib_wait_close(&pwaitkbd));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err t000171_loop(t000171_infos *pinfos)
{
  netwib_time t;
  netwib_bool event;
  netwib_err ret;

  /* send DISCOVER (if lost in transit, it is not resent) */
  netwib_er(t000171_send_discover(pinfos));

  /* expect a good OFFER within the time limit */
  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_plus_msec(&t, pinfos->timeoutms));
  while(NETWIB_TRUE) {
    netwib_er(netwib_io_wait_read(pinfos->pio, &t, &event));
    if (!event) {
      return(NETWOX_ERR_TIMEOUT);
    }
    ret = t000171_recv_offer(pinfos);
    if (ret == NETWIB_ERR_OK) {
      break;
    } else if (ret == NETWIB_ERR_NOTCONVERTED ||
               ret == NETWIB_ERR_DATANOTAVAIL) {
      continue;
    } else {
      return(ret);
    }
  }

  /* send a REQUEST (accept) */
  netwib_er(t000171_send_request_accept(pinfos));
  while(NETWIB_TRUE) {
    netwib_er(netwib_io_wait_read(pinfos->pio, &t, &event));
    if (!event) {
      return(NETWOX_ERR_TIMEOUT);
    }
    ret = t000171_recv_ack(pinfos);
    if (ret == NETWIB_ERR_OK) {
      break;
    } else if (ret == NETWIB_ERR_NOTCONVERTED ||
               ret == NETWIB_ERR_DATANOTAVAIL) {
      continue;
    } else {
      return(ret);
    }
  }

  /* periodically send REQUEST for extendlease */
  netwib_er(t000171_loop_extendlease(pinfos));

  /* user quited, so send a RELEASE */
  netwib_er(t000171_send_release(pinfos));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err t000171_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf device, filter;
  t000171_infos infos;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000171_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'd', &device));
  netwib_er(netwox_arg_eth(parg, 'e', &infos.clienteth));
  netwib_er(netwox_arg_uint32(parg, 'T', &infos.timeoutms));
  netwib_er(netwib_buf_init_ext_string("udp and (port 67 or port 68)", &filter));
  netwib_er(netwox_snispo_init_eth(&device, &filter, &infos.pio));
  netwib_er(netwib_bufpool_initdefault(&infos.pbufpool));

  /* define our clientid */
  infos.clientid.type = 1;
  netwib_er(netwib_buf_init_ext_arrayfilled(infos.clienteth.b, NETWIB_ETH_LEN,
                                            &infos.clientid.id));

  /* main loop */
  netwib_er(t000171_loop(&infos));

  /* close */
  netwib_er(netwib_bufpool_close(&infos.pbufpool));
  netwib_er(netwib_io_close(&infos.pio));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}

