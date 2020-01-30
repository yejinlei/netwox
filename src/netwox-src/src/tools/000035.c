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
netwib_conststring t000035_description[] = {
  NETWOX_DESC_spoof_packet,
  NETWOX_DESC_toolpriv_spoof,
  NULL
};
netwox_toolarg t000035_args[] = {
  NETWOX_TOOLARG_OPT_BUF_DEVICE('d', NULL, "device for spoof", NULL),
  NETWOX_TOOLARG_OPT_ETH_SRC('a', "eth-src", "Ethernet src", NULL),
  NETWOX_TOOLARG_OPT_ETH_DST('b', "eth-dst", "Ethernet dst", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('r', "eth-type", "Ethernet type", "2048"),
  NETWOX_TOOLARG_OPTA_UINT32('s', "ip4-ihl", "IP4 ihl", "5"),
  NETWOX_TOOLARG_OPT_UINT32('c', "ip4-tos", "IP4 tos", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('t', "ip4-totlen", "IP4 totlen", NULL),
  NETWOX_TOOLARG_OPT_UINT32('e', "ip4-id", "IP4 id (rand if unset)", NULL),
  NETWOX_TOOLARG_OPT_BOOL('f', "ip4-reserved", "IP4 reserved", NULL),
  NETWOX_TOOLARG_OPT_BOOL('g', "ip4-dontfrag", "IP4 dontfrag", NULL),
  NETWOX_TOOLARG_OPT_BOOL('h', "ip4-morefrag", "IP4 morefrag", NULL),
  NETWOX_TOOLARG_OPT_UINT32('i', "ip4-offsetfrag", "IP4 offsetfrag", NULL),
  NETWOX_TOOLARG_OPT_UINT32('j', "ip4-ttl", "IP4 ttl", NULL),
  NETWOX_TOOLARG_OPT_UINT32('k', "ip4-protocol", "IP4 protocol", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('u', "ip4-checksum", "IP4 checksum", NULL),
  NETWOX_TOOLARG_OPT_IP4_SRC('l', "ip4-src", "IP4 src", NULL),
  NETWOX_TOOLARG_OPT_IP4_DST('m', "ip4-dst", "IP4 dst", NULL),
  NETWOX_TOOLARG_OPT_BUF_IP4OPTS('n', "ip4-opt", NULL, NULL),
  NETWOX_TOOLARG_OPT_PORT_SRC('o', "udp-src", "UDP src", NULL),
  NETWOX_TOOLARG_OPT_PORT_DST('p', "udp-dst", "UDP dst", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('v', "udp-len", "UDP length", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('w', "udp-checksum", "UDP checksum", NULL),
  NETWOX_TOOLARG_OPT_BUF_MIXED('q', "udp-data", NULL, NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000035_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SPOOF_ETH,
  NETWOX_TOOLTREENODETYPE_SPOOF_IP,
  NETWOX_TOOLTREENODETYPE_SPOOF_UDP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000035_info = {
  "Spoof EthernetIp4Udp packet",
  t000035_description,
  "frame, hping, mac, send",
  t000035_args,
  t000035_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000035_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf device, udpdata, pkt, pkt2;
  netwib_io *pio;
  netwib_device_dlttype dlt;
  netwib_linkhdr linkheader;
  netwib_iphdr ipheader;
  netwib_udphdr udpheader;
  netwib_bool isset;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000035_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'd', &device));

  netwib_er(netwib_io_init_spoof_link(&device, &pio));
  netwib_er(netwib_spoof_ctl_get_dlt(pio, &dlt));
  if (dlt != NETWIB_DEVICE_DLTTYPE_ETHER) {
    netwib_er(netwib_fmt_display("Device %{buf} does not have an Ether DLT (%{uint32}\n", &device, dlt));
    return(NETWOX_ERR_SPOOF_INVALIDDLT);
  }

  netwib_er(netwib_linkhdr_initdefault(NETWIB_DEVICE_DLTTYPE_ETHER, &linkheader));
  netwib_er(netwox_arg_eth(parg, 'a', &linkheader.hdr.ether.src));
  netwib_er(netwox_arg_eth(parg, 'b', &linkheader.hdr.ether.dst));
  netwib_er(netwib_iphdr_initdefault(NETWIB_IPTYPE_IP4, &ipheader));
  netwib_er(netwox_arg_uint8(parg, 'c', &ipheader.header.ip4.tos));
  netwib_er(netwox_arg_isset(parg, 'e', &isset));
  if (isset) {
    netwib_er(netwox_arg_uint16(parg, 'e', &ipheader.header.ip4.id));
  }
  netwib_er(netwox_arg_bool(parg, 'f', &ipheader.header.ip4.reserved));
  netwib_er(netwox_arg_bool(parg, 'g', &ipheader.header.ip4.dontfrag));
  netwib_er(netwox_arg_bool(parg, 'h', &ipheader.header.ip4.morefrag));
  netwib_er(netwox_arg_uint16(parg, 'i', &ipheader.header.ip4.offsetfrag));
  netwib_er(netwox_arg_uint8(parg, 'j', &ipheader.ttl));
  netwib_er(netwox_arg_uint32(parg, 'k', (netwib_uint32*)&ipheader.protocol));
  netwib_er(netwox_arg_ip(parg, 'l', &ipheader.src));
  netwib_er(netwox_arg_ip(parg, 'm', &ipheader.dst));
  netwib_er(netwox_arg_buf(parg, 'n', &ipheader.header.ip4.opts));
  netwib_er(netwib_udphdr_initdefault(&udpheader));
  netwib_er(netwox_arg_port(parg, 'o', &udpheader.src));
  netwib_er(netwox_arg_port(parg, 'p', &udpheader.dst));
  netwib_er(netwox_arg_buf(parg, 'q', &udpdata));

  /* construct packet */
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_pkt_append_linkipudpdata(&linkheader, &ipheader,
                                            &udpheader, &udpdata,
                                            &pkt));

  /* now, if user specified invalid fields, use them. We have to decode
     packet, to change fields, and to reconstruct packet. */
  netwib_er(netwox_arg_isset(parg, 'r', &isset));
  if (!isset) { netwib_er(netwox_arg_isset(parg, 's', &isset)); }
  if (!isset) { netwib_er(netwox_arg_isset(parg, 't', &isset)); }
  if (!isset) { netwib_er(netwox_arg_isset(parg, 'u', &isset)); }
  if (!isset) { netwib_er(netwox_arg_isset(parg, 'v', &isset)); }
  if (!isset) { netwib_er(netwox_arg_isset(parg, 'w', &isset)); }
  if (isset) {
    /* decode */
    netwib_er(netwib_pkt_decode_linkipudpdata(NETWIB_DEVICE_DLTTYPE_ETHER,
                                              &pkt,
                                              &linkheader, &ipheader,
                                              &udpheader, &udpdata));
    /* change fields */
    netwib_er(netwox_arg_isset(parg, 'r', &isset));
    if (isset) {
      netwib_er(netwox_arg_uint32(parg, 'r',
                                  (netwib_uint32*)&linkheader.hdr.ether.type));
    }
    netwib_er(netwox_arg_isset(parg, 's', &isset));
    if (isset) {
      netwib_er(netwox_arg_uint8(parg, 's', &ipheader.header.ip4.ihl));
    }
    netwib_er(netwox_arg_isset(parg, 't', &isset));
    if (isset) {
      netwib_er(netwox_arg_uint16(parg, 't', &ipheader.header.ip4.totlen));
    }
    netwib_er(netwox_arg_isset(parg, 'u', &isset));
    if (isset) {
      netwib_er(netwox_arg_uint16(parg, 'u', &ipheader.header.ip4.check));
    }
    netwib_er(netwox_arg_isset(parg, 'v', &isset));
    if (isset) {
      netwib_er(netwox_arg_uint16(parg, 'v', &udpheader.len));
    }
    netwib_er(netwox_arg_isset(parg, 'w', &isset));
    if (isset) {
      netwib_er(netwox_arg_uint16(parg, 'w', &udpheader.check));
    }
    /* reconstruct header after header (no automatic computation) */
    netwib_er(netwib_buf_init_mallocdefault(&pkt2));
    netwib_er(netwib_pkt_append_linkhdr(&linkheader, &pkt2));
    netwib_er(netwib_pkt_append_iphdr(&ipheader, &pkt2));
    netwib_er(netwib_pkt_append_udphdr(&udpheader, &pkt2));
    netwib_er(netwib_buf_append_buf(&udpdata, &pkt2));
    netwib__buf_reinit(&pkt);
    netwib_er(netwib_buf_append_buf(&pkt2, &pkt));
    netwib_er(netwib_buf_close(&pkt2));
    /* warn user */
    netwib_er(netwib_fmt_display("Those options generate an invalid packet. Do not trust sniffer display.\n"));
    netwib_er(netwib_fmt_display("Raw packet display (correct):\n"));
    netwib_er(netwib_buf_display(&pkt, NETWIB_ENCODETYPE_DUMP));
    netwib_er(netwib_fmt_display("Nice packet display (don't trust it):\n"));
  }

  /* display to screen */
  netwib_er(netwib_pkt_link_display(dlt, &pkt, NULL,
                                    NETWIB_ENCODETYPE_ARRAY,
                                    NETWIB_ENCODETYPE_DUMP));

  /* send */
  netwib_er(netwib_io_write(pio, &pkt));

  /* close */
  netwib_er(netwib_buf_close(&pkt));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
