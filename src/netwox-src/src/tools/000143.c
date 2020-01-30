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
netwib_conststring t000143_description[] = {
  NETWOX_DESC_spoof_packet,
  NETWOX_DESC_toolpriv_spoof,
  NULL
};
netwox_toolarg t000143_args[] = {
  NETWOX_TOOLARG_OPT_BUF_DEVICE('d', NULL, "device for spoof", NULL),
  NETWOX_TOOLARG_OPT_ETH_SRC('a', "eth-src", "Ethernet src", NULL),
  NETWOX_TOOLARG_OPT_ETH_DST('b', "eth-dst", "Ethernet dst", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('q', "eth-type", "Ethernet type", "2048"),
  NETWOX_TOOLARG_OPT_UINT32('c', "ip6-trafficclass", "IP6 traffic class",
                            NULL),
  NETWOX_TOOLARG_OPT_UINT32('l', "ip6-flowlabel", "IP6 flow label", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('e', "ip6-payloadlength", "IP6 payload length",
                             NULL),
  NETWOX_TOOLARG_OPT_UINT32('f', "ip6-protocol", "IP6 next_header/protocol",
                            NULL),
  NETWOX_TOOLARG_OPT_UINT32('g', "ip6-ttl", "IP6 hop_limit/ttl", NULL),
  NETWOX_TOOLARG_OPT_IP6_SRC('h', "ip6-src", "IP6 src", NULL),
  NETWOX_TOOLARG_OPT_IP6_DST('i', "ip6-dst", "IP6 dst", NULL),
  NETWOX_TOOLARG_OPT_BUF_IP6EXTS('j', "ip6-exts", "IP6 extensions", NULL),
  NETWOX_TOOLARG_OPT_UINT32('o', "icmp-type", "ICMP type", NULL),
  NETWOX_TOOLARG_OPT_UINT32('p', "icmp-code", "ICMP code", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('r', "icmp-checksum", "ICMP checksum", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000143_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SPOOF_ETH,
  NETWOX_TOOLTREENODETYPE_SPOOF_IP,
  NETWOX_TOOLTREENODETYPE_SPOOF_ICMP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000143_info = {
  "Spoof EthernetIp6Icmp6 packet",
  t000143_description,
  "frame, hping, mac, send",
  t000143_args,
  t000143_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000143_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf device, pkt, pkt2, buf;
  netwib_io *pio;
  netwib_device_dlttype dlt;
  netwib_linkhdr linkheader;
  netwib_iphdr ipheader;
  netwib_icmp6 icmp6;
  netwib_bool isset;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000143_info, &parg));
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
  netwib_er(netwib_iphdr_initdefault(NETWIB_IPTYPE_IP6, &ipheader));
  netwib_er(netwox_arg_uint8(parg, 'c', &ipheader.header.ip6.trafficclass));
  netwib_er(netwox_arg_uint32(parg, 'l', &ipheader.header.ip6.flowlabel));
  netwib_er(netwox_arg_uint32(parg, 'f', (netwib_uint32*)&ipheader.protocol));
  netwib_er(netwox_arg_uint8(parg, 'g', &ipheader.ttl));
  netwib_er(netwox_arg_isset(parg, 'h', &isset));
  if (isset) {
    netwib_er(netwox_arg_ip(parg, 'h', &ipheader.src));
  }
  netwib_er(netwox_arg_isset(parg, 'i', &isset));
  if (isset) {
    netwib_er(netwox_arg_ip(parg, 'i', &ipheader.dst));
  }
  netwib_er(netwox_arg_isset(parg, 'j', &isset));
  if (isset) {
    netwib_er(netwox_arg_buf(parg, 'j', &buf));
    netwib_er(netwox_ip6exts_decode_para(&buf, &ipheader.protocol,
                                         &ipheader.header.ip6.exts));
  }
  icmp6.type = NETWIB_ICMP6TYPE_ECHOREQ;
  netwib_er(netwox_arg_isset(parg, 'o', &isset));
  if (isset) {
    netwib_er(netwox_arg_uint32(parg, 'o', (netwib_uint32*)&icmp6.type));
  }
  netwib_er(netwib_icmp6_initdefault(icmp6.type, &icmp6));
  netwib_er(netwox_arg_isset(parg, 'p', &isset));
  if (isset) {
    netwib_er(netwox_arg_uint32(parg, 'p', (netwib_uint32*)&icmp6.code));
  }

  /* construct packet */
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_pkt_append_linkipicmp6(&linkheader, &ipheader, &icmp6,
                                          &pkt));

  /* now, if user specified invalid fields, use them. We have to decode
     packet, to change fields, and to reconstruct packet. */
  netwib_er(netwox_arg_isset(parg, 'e', &isset));
  if (!isset) { netwib_er(netwox_arg_isset(parg, 'q', &isset)); }
  if (!isset) { netwib_er(netwox_arg_isset(parg, 'r', &isset)); }
  if (isset) {
    /* decode */
    netwib_er(netwib_pkt_decode_linkipicmp6(NETWIB_DEVICE_DLTTYPE_ETHER, &pkt,
                                            &linkheader, &ipheader, &icmp6));
    /* change fields */
    netwib_er(netwox_arg_isset(parg, 'q', &isset));
    if (isset) {
      netwib_er(netwox_arg_uint32(parg, 'q',
                                  (netwib_uint32*)&linkheader.hdr.ether.type));
    }
    netwib_er(netwox_arg_isset(parg, 'e', &isset));
    if (isset) {
      netwib_er(netwox_arg_uint16(parg, 'e',
                                  &ipheader.header.ip6.payloadlength));
    }
    netwib_er(netwox_arg_isset(parg, 'r', &isset));
    if (isset) {
      netwib_er(netwox_arg_uint16(parg, 'r', &icmp6.check));
    }
    /* reconstruct header after header (no automatic computation) */
    netwib_er(netwib_buf_init_mallocdefault(&pkt2));
    netwib_er(netwib_pkt_append_linkhdr(&linkheader, &pkt2));
    netwib_er(netwib_pkt_append_iphdr(&ipheader, &pkt2));
    netwib_er(netwib_pkt_append_icmp6(&icmp6, &pkt2));
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
