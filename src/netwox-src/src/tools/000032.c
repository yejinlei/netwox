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
netwib_conststring t000032_description[] = {
  NETWOX_DESC_spoof_packet,
  NETWOX_DESC_toolpriv_spoof,
  NULL
};
netwox_toolarg t000032_args[] = {
  NETWOX_TOOLARG_OPT_BUF_DEVICE('d', NULL, "device for spoof", NULL),
  NETWOX_TOOLARG_OPT_ETH_SRC('a', "eth-src", "Ethernet src", NULL),
  NETWOX_TOOLARG_OPT_ETH_DST('b', "eth-dst", "Ethernet dst", NULL),
  NETWOX_TOOLARG_OPT_UINT32('c', "eth-type", "Ethernet type", NULL),
  NETWOX_TOOLARG_OPT_BUF_MIXED('e', "eth-data", "Ethernet data", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000032_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SPOOF_ETH,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000032_info = {
  "Spoof Ethernet packet",
  t000032_description,
  "frame, hping, mac, send",
  t000032_args,
  t000032_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000032_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf device, data, pkt;
  netwib_io *pio;
  netwib_device_dlttype dlt;
  netwib_linkhdr linkheader;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000032_info, &parg));
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
  netwib_er(netwox_arg_uint32(parg, 'c',
                              (netwib_uint32*)&linkheader.hdr.ether.type));
  netwib_er(netwox_arg_buf(parg, 'e', &data));

  /* construct packet */
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_pkt_append_linkdata(&linkheader, &data, &pkt));

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
