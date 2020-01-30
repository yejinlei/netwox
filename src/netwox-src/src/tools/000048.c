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
netwib_conststring t000048_description[] = {
  NETWOX_DESC_spoof_sample,
  NETWOX_DESC_spoof_sampleopt,
  "IP4 options contains an IP Prespecified Timestamp option.",
  NETWOX_DESC_spoofip,
  NETWOX_DESC_toolpriv_spoof,
  NULL
};
netwox_toolarg t000048_args[] = {
  NETWOX_TOOLARG_OPTA_SPOOFIP('a', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPT_IP_SRC('s', "ip4-src", "IP4 src", NULL),
  NETWOX_TOOLARG_OPT_IP_DST('d', "ip4-dst", "IP4 dst", NULL),
  NETWOX_TOOLARG_OPT_PORT_DST('S', "tcp-src", "TCP port src", NULL),
  NETWOX_TOOLARG_OPT_PORT_DST('D', "tcp-dst", "TCP port dst", NULL),
  NETWOX_TOOLARG_OPT_UINT32('n', "sample", "number of sample (1 to n)", "1"),
  NETWOX_TOOLARG_OPT_UINT32('f', "fragsize", "fragment size (0=nofrag)", NULL),
  NETWOX_TOOLARG_OPT_BOOL('x', "display", "display", NULL),
  NETWOX_TOOLARG_OPT_IP('i', "ip4opt-ip", "IP for IPPTS", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000048_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SPOOF_IP,
  NETWOX_TOOLTREENODETYPE_SPOOF_UDP,
  NETWOX_TOOLTREENODETYPE_SPOOF_TCP,
  NETWOX_TOOLTREENODETYPE_SPOOF_ICMP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000048_info = {
  "Spoof of packet samples : fragment, ip4opt:ippts",
  t000048_description,
  "ip4 option, send",
  t000048_args,
  t000048_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000048_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf pkt;
  netwib_io *pio;
  netwib_spoof_ip_inittype inittype;
  netwib_uint32 fragsize;
  netwox_sample_info sampleinfo;
  netwox_sample sample;
  netwib_data ip4optsarray[NETWIB_IP4OPTS_MAXLEN];
  netwib_buf ip4opts;
  netwib_bool display, isset;
  netwib_ip router;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000048_info, &parg));

  netwib_er(netwox_arg_spoofip(parg, 'a', &inittype));
  netwib_er(netwox_arg_uint32(parg, 'f', &fragsize));
  netwib_er(netwox_arg_bool(parg, 'x', &display));
  netwib_er(netwox_spooffrag_init(inittype, fragsize, 0, display, &pio));

  netwib_er(netwox_sample_info_initdefault(&sampleinfo));
  netwib_er(netwox_arg_ip(parg, 's', &sampleinfo.ipsrc));
  netwib_er(netwox_arg_ip(parg, 'd', &sampleinfo.ipdst));
  netwib_er(netwox_arg_isset(parg, 'S', &isset));
  if (isset) {
    netwib_er(netwox_arg_port(parg, 'S', &sampleinfo.portsrc));
  }
  netwib_er(netwox_arg_isset(parg, 'D', &isset));
  if (isset) {
    netwib_er(netwox_arg_port(parg, 'D', &sampleinfo.portdst));
  }

  netwib_er(netwox_arg_uint32(parg, 'n', (netwib_uint32*)&sample));

  /* construct ip4opts */
  netwib_er(netwib_buf_init_ext_arraysizeofempty(ip4optsarray, &ip4opts));
  netwib_er(netwox_arg_ip(parg, 'i', &router));
  netwib_er(netwox_ip4opts_pkt_ippts(&router, &ip4opts));
  sampleinfo.ip4opts = ip4opts;

  /* construct packet */
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwox_sample_pkt(sample, &sampleinfo, &pkt));

  /* send */
  netwib_er(netwib_io_write(pio, &pkt));

  /* close */
  netwib_er(netwib_buf_close(&pkt));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
