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
netwib_conststring t000011_description[] = {
  "This tool sniffs and verify checksums (IP header, TCP, UDP, etc.).",
  "If a bad checksum is encountered, it is displayed along with the",
  "expected value.",
  "",
  NETWOX_DESC_sniff_devfilter,
  NETWOX_DESC_sniff_rawip,
  NETWOX_DESC_sniff_ipreas,
  NETWOX_DESC_toolpriv_sniff,
  NULL
};
netwox_toolarg t000011_args[] = {
  NETWOX_TOOLARG_OPT_BUF_DEVICE('d', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPT_BUF_FILTER('f', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPTA_BOOL('r', "rawip", "sniff at IP level", NULL),
  NETWOX_TOOLARG_OPTA_BOOL('i', "ipreas", "reassemble IP packets", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000011_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SNIFF,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000011_info = {
  "Sniff and verify checksums",
  t000011_description,
  "capture",
  t000011_args,
  t000011_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000011_init(netwib_constbuf *pdevice,
                               netwib_constbuf *pfilter,
                               netwib_bool rawip,
                               netwib_bool ipreas,
                               netwib_io **ppio)
{
  netwib_io *pio1, *pio2;

  if (rawip) {
    netwib_er(netwib_io_init_sniff_ip(pdevice, pfilter, ipreas, NETWIB_FALSE,
                                    ppio));
  } else {
    netwib_er(netwib_io_init_sniff(pdevice, pfilter, &pio1));
    if (ipreas) {
      netwib_er(netwib_io_init_sniff_ipreas(&pio2));
      netwib_er(netwib_io_plug_read(pio2, pio1));
      *ppio = pio2;
    } else {
      *ppio = pio1;
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000011_loop(netwib_io *piosniff)
{
  netwib_bufpool *ppool;
  netwib_buf *ppkt;
  netwib_device_dlttype dlttype;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_sniff_ctl_get_dlt(piosniff, &dlttype));

  /* main loop */
  netwib_er(netwib_bufpool_initdefault(&ppool));
  netwib_er(netwib_bufpool_buf_init(ppool, &ppkt));
  while (NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();
    /* read packets */
    netwib__buf_reinit(ppkt);
    ret = netwib_io_read(piosniff, ppkt);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* verify checksum */
    netwib_er(netwox_checksum_verify_link(ppool, dlttype, ppkt));
  }

  /* close */
  netwib_er(netwib_bufpool_buf_close(ppool, &ppkt));
  netwib_er(netwib_bufpool_close(&ppool));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000011_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_io *pio;
  netwib_buf device, filter;
  netwib_bool rawip, ipreas;
  netwib_err ret;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000011_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'd', &device));
  netwib_er(netwox_arg_buf(parg, 'f', &filter));
  netwib_er(netwox_arg_bool(parg, 'r', &rawip));
  netwib_er(netwox_arg_bool(parg, 'i', &ipreas));

  /* initialize io */
  netwib_er(t000011_init(&device, &filter, rawip, ipreas, &pio));

  /* main loop */
  ret = t000011_loop(pio);

  /* close */
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwox_arg_close(&parg));

  return(ret);
}
