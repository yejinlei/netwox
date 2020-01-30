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
netwib_conststring t000082_description[] = {
  NETWOX_DESC_icmp_error,
  NETWOX_DESC_sniff_devfilter,
  NETWOX_DESC_spoofip,
  NETWOX_DESC_toolpriv_snsp,
  NULL
};
netwox_toolarg t000082_args[] = {
  NETWOX_TOOLARG_OPT_BUF_DEVICE('d', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPT_BUF_FILTER('f', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPTA_SPOOFIP('s', NULL, NULL, "linkbraw"),
  NETWOX_TOOLARG_OPT_UINT32('c', "code", "ICMP code", NULL),
  NETWOX_TOOLARG_OPT_IP_SRC('i', NULL, NULL, NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000082_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NETAUDIT_ICMP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000082_info = {
  "Sniff and send ICMP4/ICMP6 destination unreachable",
  t000082_description,
  NULL,
  t000082_args,
  t000082_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000082_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_spoof_ip_inittype inittype;
  netwox_icmperr_info info;
  netwib_buf filter, device;

  /* obtain generic parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000082_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'd', &device));
  netwib_er(netwox_arg_buf(parg, 'f', &filter));
  netwib_er(netwox_arg_spoofip(parg, 's', &inittype));

  /* initialize io */
  netwib_er(netwib_io_init_spoof_ip(inittype, &info.pspoofio));
  netwib_er(netwib_io_init_sniff_ip(&device, &filter, NETWIB_FALSE,
                                    NETWIB_FALSE, &info.psniffio));

  /* set specific parameters */
  netwib_er(netwox_arg_ip(parg, 'i', &info.srcip));
  info.iptype = info.srcip.iptype;
  if (info.iptype == NETWIB_IPTYPE_IP4) {
    info.type4 = NETWIB_ICMP4TYPE_DSTUNREACH;
    netwib_er(netwox_arg_uint32(parg, 'c', (netwib_uint32*)&info.code4));
  } else {
    info.type6 = NETWIB_ICMP6TYPE_DSTUNREACH;
    netwib_er(netwox_arg_uint32(parg, 'c', (netwib_uint32*)&info.code6));
  }

  /* main loop */
  netwib_er(netwox_icmperr_loop(&info));

  /* close */
  netwib_er(netwib_io_close(&info.psniffio));
  netwib_er(netwib_io_close(&info.pspoofio));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
