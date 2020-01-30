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
netwib_conststring t000074_description[] = {
  "This tool sends a lot of random IP fragments to a host.",
  "It permits to check how a firewall behaves when receiving fragments",
  "which have to be ignored.",
  NETWOX_DESC_toolpriv_spoof,
  NULL
};
netwox_toolarg t000074_args[] = {
  NETWOX_TOOLARG_REQ_IP_DST('i', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPT_UINT32('p', "protocol", "protocol (0=rand)", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000074_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NETAUDIT_IP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000074_info = {
  "Flood a host with random fragments",
  t000074_description,
  NULL,
  t000074_args,
  t000074_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000074_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_ip dstip;
  netwib_ipproto ipproto;
  netwib_iphdr ipheader;
  netwib_ip6ext ip6ext;
  netwib_buf pkt, ip6exts, data;
  netwib_uint32 ui;
  netwib_io *pio;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000074_info, &parg));
  netwib_er(netwox_arg_ip(parg, 'i', &dstip));
  netwib_er(netwox_arg_uint32(parg, 'p', (netwib_uint32*)&ipproto));
  netwib_er(netwox_arg_close(&parg));

  /* init spoofio */
  netwib_er(netwib_io_init_spoof_ip(NETWIB_SPOOF_IP_INITTYPE_LINKBRAW, &pio));

  /* main loop */
  netwib_er(netwib_iphdr_initdefault(dstip.iptype, &ipheader));
  ipheader.dst = dstip;
  if (dstip.iptype == NETWIB_IPTYPE_IP4) {
    ipheader.protocol = ipproto;
  } else {
    ipheader.protocol = NETWIB_IPPROTO_FRAGMENT;
    netwib_er(netwib_ip6ext_initdefault(NETWIB_IPPROTO_FRAGMENT, &ip6ext));
    ip6ext.nextproto = ipproto;
  }
  netwib_er(netwib_buf_init_ext_string("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz", &data));
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_buf_init_mallocdefault(&ip6exts));
  while (NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();

    /* construct first packet */
    netwib__buf_reinit(&pkt);
    if (dstip.iptype == NETWIB_IPTYPE_IP4) {
      netwib_er(netwib_uint32_init_rand(0, 0xFFFF, &ui));
      ipheader.header.ip4.id = (netwib_uint16)ui;
      if (!ipproto) {
        netwib_er(netwib_uint32_init_rand(0, 0xFF, &ui));
        ipheader.protocol = ui;
      }
      netwib_er(netwib_uint32_init_rand(1, 0xFFFFFFFEu, &ui));
      netwib_er(netwib_ip_init_ip4(ui, &ipheader.src));
      ipheader.header.ip4.dontfrag = NETWIB_FALSE;
      ipheader.header.ip4.morefrag = NETWIB_TRUE;
      ipheader.header.ip4.offsetfrag = 0;
    } else {
      netwib__buf_reinit(&ip6exts);
      ipheader.src = ipheader.dst;
      netwib_er(netwib_uint32_init_rand(1, 0xFEu, &ui));
      ipheader.src.ipvalue.ip6.b[12] = (netwib_byte)ui;
      netwib_er(netwib_uint32_init_rand(1, 0xFEu, &ui));
      ipheader.src.ipvalue.ip6.b[13] = (netwib_byte)ui;
      netwib_er(netwib_uint32_init_rand(1, 0xFEu, &ui));
      ipheader.src.ipvalue.ip6.b[14] = (netwib_byte)ui;
      netwib_er(netwib_uint32_init_rand(1, 0xFEu, &ui));
      ipheader.src.ipvalue.ip6.b[15] = (netwib_byte)ui;
      if (!ipproto) {
        netwib_er(netwib_uint32_init_rand(0, 0xFF, &ui));
        ip6ext.nextproto = ui;
      }
      ip6ext.ext.fragment.fragmentoffset = 0;
      ip6ext.ext.fragment.morefrag = NETWIB_TRUE;
      netwib_er(netwib_uint32_init_rand(1, 0xFFFFFFFu, &ui));
      ip6ext.ext.fragment.id = ui;
      netwib_er(netwib_pkt_append_ip6ext(&ip6ext, &ip6exts));
      ipheader.header.ip6.exts = ip6exts;
    }
    netwib_er(netwib_pkt_append_ipdata(&ipheader, &data, &pkt));
    netwib_er(netwib_io_write(pio, &pkt));

    /* construct middle packet */
    netwib__buf_reinit(&pkt);
    if (dstip.iptype == NETWIB_IPTYPE_IP4) {
      netwib_er(netwib_uint32_init_rand(0, 0xFFFF, &ui));
      ipheader.header.ip4.id = (netwib_uint16)ui;
      ipheader.header.ip4.morefrag = NETWIB_TRUE;
      netwib_er(netwib_uint32_init_rand(10, 0x1FFFu, &ui));
      ipheader.header.ip4.offsetfrag = (netwib_uint16)ui;
    } else {
      netwib__buf_reinit(&ip6exts);
      netwib_er(netwib_uint32_init_rand(10, 0x1FFFu, &ui));
      ip6ext.ext.fragment.fragmentoffset = (netwib_uint16)ui;
      ip6ext.ext.fragment.morefrag = NETWIB_TRUE;
      netwib_er(netwib_pkt_append_ip6ext(&ip6ext, &ip6exts));
      ipheader.header.ip6.exts = ip6exts;
    }
    netwib_er(netwib_pkt_append_ipdata(&ipheader, &data, &pkt));
    netwib_er(netwib_io_write(pio, &pkt));

  }
  netwib_er(netwib_buf_close(&ip6exts));
  netwib_er(netwib_buf_close(&pkt));

  /* close */
  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}
