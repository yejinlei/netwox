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
netwib_conststring t000033_description[] = {
  NETWOX_DESC_spoof_packet,
  NETWOX_DESC_toolpriv_spoof,
  NULL
};
netwox_toolarg t000033_args[] = {
  NETWOX_TOOLARG_OPT_BUF_DEVICE('d', NULL, "device for spoof", NULL),
  NETWOX_TOOLARG_OPT_ETH_SRC('a', "eth-src", "Ethernet src", NULL),
  NETWOX_TOOLARG_OPT_ETH_DST('b', "eth-dst", "Ethernet dst", NULL),
  NETWOX_TOOLARG_OPT_UINT32('c', "eth-type", "Ethernet type : ARP=2054, RARP=32821", "2054"),
  NETWOX_TOOLARG_OPT_UINT32('e', "arp-op", "ARP op : 1=ARPREQ, 2=ARPREP, 3=RARPREQ, 4=RARPREP", "1"),
  NETWOX_TOOLARG_OPT_ETH('f', "arp-ethsrc", "ARP ethsrc", NULL),
  NETWOX_TOOLARG_OPT_IP('g', "arp-ipsrc", "ARP ipsrc", "0.0.0.0"),
  NETWOX_TOOLARG_OPT_ETH('h', "arp-ethdst", "ARP ethdst", "0:0:0:0:0:0"),
  NETWOX_TOOLARG_OPT_IP('i', "arp-ipdst", "ARP ipdst", "0.0.0.0"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000033_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SPOOF_ETH,
  NETWOX_TOOLTREENODETYPE_SPOOF_ARP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000033_info = {
  "Spoof EthernetArp packet",
  t000033_description,
  "frame, hping, mac, send",
  t000033_args,
  t000033_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000033_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf device, pkt;
  netwib_io *pio;
  netwib_device_dlttype dlt;
  netwib_linkhdr linkheader;
  netwib_arphdr arpheader;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000033_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'd', &device));

  netwib_er(netwib_io_init_spoof_link(&device, &pio));
  netwib_er(netwib_spoof_ctl_get_dlt(pio, &dlt));
  if (dlt != NETWIB_DEVICE_DLTTYPE_ETHER) {
    netwib_er(netwib_fmt_display("Device %{buf} does not have an Ether DLT (%{uint32}\n", &device, dlt));
    return(NETWOX_ERR_SPOOF_INVALIDDLT);
  }

  netwib_er(netwib_linkhdr_initdefault(NETWIB_DEVICE_DLTTYPE_ETHER,
                                       &linkheader));
  netwib_er(netwox_arg_eth(parg, 'a', &linkheader.hdr.ether.src));
  netwib_er(netwox_arg_eth(parg, 'b', &linkheader.hdr.ether.dst));
  netwib_er(netwox_arg_uint32(parg, 'c',
                              (netwib_uint32*)&linkheader.hdr.ether.type));
  netwib_er(netwox_arg_uint32(parg, 'e', (netwib_uint32*)&arpheader.op));
  netwib_er(netwox_arg_eth(parg, 'f', &arpheader.ethsrc));
  netwib_er(netwox_arg_ip(parg, 'g', &arpheader.ipsrc));
  netwib_er(netwox_arg_eth(parg, 'h', &arpheader.ethdst));
  netwib_er(netwox_arg_ip(parg, 'i', &arpheader.ipdst));

  /* construct packet */
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_pkt_append_linkarp(&linkheader, &arpheader, &pkt));

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
