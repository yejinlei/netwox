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
netwib_conststring t000009_description[] = {
  "This tool sniffs packets, and tries to decode them. Then, Ethernet",
  "address associated to decoded IP address is displayed.",
  "It uses ARP, RARP and IP to obtain this information.",
  "Please note, this will be inaccurate if IP address is outside the",
  "Ethernet LAN. It works well in all other cases.",
  "",
  NETWOX_DESC_sniff_devfilter,
  NETWOX_DESC_toolpriv_sniff,
  NULL
};
netwox_toolarg t000009_args[] = {
  NETWOX_TOOLARG_OPT_BUF_DEVICE('d', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPT_BUF_FILTER('f', NULL, NULL, NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000009_nodes[] = {
  NETWOX_TOOLTREENODETYPE_INFO_REMOTE,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000009_info = {
  "Sniff and display Ethernet addresses",
  t000009_description,
  "capture, mac, show",
  t000009_args,
  t000009_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000009_analyze(netwib_device_dlttype dlttype,
                                  netwib_buf *ppkt)
{
  netwib_linkhdr linkheader;
  netwib_iphdr ipheader;
  netwib_arphdr arpheader;
  netwib_linkhdrproto linkhdrproto;
  netwib_ip ipzero;
  netwib_cmp cmp;
  netwib_err ret;

  /*netwib_er(netwib_pkt_link_display(dlttype, ppkt, NULL,
                                  NETWIB_ENCODETYPE_SYNTH,
                                  NETWIB_ENCODETYPE_NOTHING));*/

  ret = netwib_pkt_decode_layer_link(dlttype, ppkt, &linkheader);
  if (ret != NETWIB_ERR_OK) {
    return(NETWIB_ERR_OK);
  }
  if (linkheader.type != NETWIB_DEVICE_DLTTYPE_ETHER) {
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_linkhdr_get_proto(&linkheader, &linkhdrproto));

  switch(linkhdrproto) {
    case NETWIB_LINKHDRPROTO_IP4 :
    case NETWIB_LINKHDRPROTO_IP6 :
      ret = netwib_pkt_decode_layer_ip(ppkt, &ipheader);
      if (ret != NETWIB_ERR_OK) {
        return(NETWIB_ERR_OK);
      }
      netwib_er(netwib_ip_init_ip6_fields(0,0,0,0, &ipzero));
      netwib_er(netwib_ip_cmp(&ipheader.src, &ipzero, &cmp));
      if (cmp != NETWIB_CMP_EQ) {
        netwib_er(netwib_fmt_display("%{eth}\t%{ip}\n",
                                     &linkheader.hdr.ether.src,
                                     &ipheader.src));
      }
      if (linkheader.hdr.ether.dst.b[0] == 0xFF &&
          linkheader.hdr.ether.dst.b[1] == 0xFF &&
          linkheader.hdr.ether.dst.b[2] == 0xFF &&
          linkheader.hdr.ether.dst.b[3] == 0xFF &&
          linkheader.hdr.ether.dst.b[4] == 0xFF &&
          linkheader.hdr.ether.dst.b[5] == 0xFF ) {
        break;
      }
      if (linkheader.hdr.ether.dst.b[0] == 0x33 &&
          linkheader.hdr.ether.dst.b[1] == 0x33 &&
          linkheader.hdr.ether.dst.b[2] == 0xFF) {
        break;
      }
      netwib_er(netwib_ip_init_ip6_fields(0,0,0,0, &ipzero));
      netwib_er(netwib_ip_cmp(&ipheader.dst, &ipzero, &cmp));
      if (cmp == NETWIB_CMP_EQ) {
        break;
      }
      netwib_er(netwib_fmt_display("%{eth}\t%{ip}\n",
                                   &linkheader.hdr.ether.dst,
                                   &ipheader.dst));
      break;
    case NETWIB_LINKHDRPROTO_ARP :
      ret = netwib_pkt_decode_layer_arp(ppkt, &arpheader);
      if (ret != NETWIB_ERR_OK) {
        return(NETWIB_ERR_OK);
      }
      netwib_er(netwib_fmt_display("%{eth}\t%{ip}\n", &arpheader.ethsrc,
                                   &arpheader.ipsrc));
      if (arpheader.op == NETWIB_ARPHDROP_ARPREP) {
        netwib_er(netwib_fmt_display("%{eth}\t%{ip}\n", &arpheader.ethdst,
                                     &arpheader.ipdst));
      }
      break;
    case NETWIB_LINKHDRPROTO_RARP :
      ret = netwib_pkt_decode_layer_arp(ppkt, &arpheader);
      if (ret != NETWIB_ERR_OK) {
        return(NETWIB_ERR_OK);
      }
      netwib_er(netwib_fmt_display("%{eth}\t%{ip}\n", &arpheader.ethsrc,
                                   &arpheader.ipsrc));
      if (arpheader.op == NETWIB_ARPHDROP_RARPREP) {
        netwib_er(netwib_fmt_display("%{eth}\t%{ip}\n", &arpheader.ethdst,
                                     &arpheader.ipdst));
      }
      break;
    default :
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err t000009_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_io *piosniff;
  netwib_buf device, filter, pkt;
  netwib_device_dlttype dlttype;
  netwib_err ret=NETWIB_ERR_OK;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000009_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'd', &device));
  netwib_er(netwox_arg_buf(parg, 'f', &filter));

  /* initialize io */
  netwib_er(netwib_io_init_sniff(&device, &filter, &piosniff));
  netwib_er(netwib_sniff_ctl_get_dlt(piosniff, &dlttype));
  if (dlttype != NETWIB_DEVICE_DLTTYPE_ETHER) {
    netwib_er(netwib_fmt_display("This device (%{buf}) does not support Ethernet (dlt=%{uint32})\n", &device, dlttype));
    netwib_er(netwib_io_close(&piosniff));
    netwib_er(netwox_arg_close(&parg));
    return(NETWIB_ERR_OK);
  }

  /* main loop */
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  while (NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();
    netwib__buf_reinit(&pkt);
    ret = netwib_io_read(piosniff, &pkt);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      /* simply ignore, and try next */
    } else if (ret == NETWIB_ERR_OK) {
      ret = t000009_analyze(dlttype, &pkt);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
    } else {
      break;
    }
  }

  netwib_er(netwib_buf_close(&pkt));
  netwib_er(netwib_io_close(&piosniff));
  netwib_er(netwox_arg_close(&parg));

  return(ret);
}
