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
netwib_conststring t000015_description[] = {
  NETWOX_DESC_record,
  "",
  "This tool reads packets stored in a record (capture file), and",
  "displays them.",
  "",
  NETWOX_DESC_record_file,
  NETWOX_DESC_record_dlt,
  NETWOX_DESC_sniff_encode,
  NETWOX_DESC_sniff_extended,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000015_args[] = {
  NETWOX_TOOLARG_REQ_BUF_FILE_RD('f', "file", "read from record file", NULL),
  NETWOX_TOOLARG_OPT_DLT('t', NULL, "dlt type of data stored in record", NULL),
  NETWOX_TOOLARG_OPT_ENCODE('H', "hdrencode", "header encoding type", "array"),
  NETWOX_TOOLARG_OPT_ENCODE('D', "dataencode", "data encoding type", "dump"),
  NETWOX_TOOLARG_OPT_BOOL('x', "extended", "display other protocols", "1"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000015_nodes[] = {
  NETWOX_TOOLTREENODETYPE_RECORD,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000015_info = {
  "Display content of a record",
  t000015_description,
  "capture, frame, show",
  t000015_args,
  t000015_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000015_loop(netwib_constbuf *pfilename,
                               netwib_device_dlttype recorddlt,
                               netwib_encodetype hdrencodetype,
                               netwib_encodetype dataencodetype,
                               netwib_bool extended)
{
  netwib_buf pkt;
  netwib_bufpool *pbufpool;
  netwib_io *piorecord;
  netwib_encodetype_context screenctx;
  netwib_err ret=NETWIB_ERR_OK;

  /* initialize transition context */
  netwib_er(netwib_buf_encode_transition_init(&screenctx));

  /* initialize record */
  netwib_er(netwib_io_init_record_read(pfilename, &piorecord));

  /* main loop */
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  while (NETWIB_TRUE) {
    /* read packet */
    netwib__buf_reinit(&pkt);
    ret = netwib_io_read(piorecord, &pkt);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    /* display to screen */
    if (extended) {
      netwib_er(netwox_pkt_link_display(pbufpool, recorddlt, &pkt, &screenctx,
                                        hdrencodetype, dataencodetype));
    } else {
      netwib_er(netwib_pkt_link_display(recorddlt, &pkt, &screenctx,
                                        hdrencodetype, dataencodetype));
    }
  }

  /* close */
  netwib_er(netwib_bufpool_close(&pbufpool));
  netwib_er(netwib_buf_close(&pkt));
  netwib_er(netwib_io_close(&piorecord));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000015_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf filename;
  netwib_device_dlttype recorddlt;
  netwib_encodetype hdrencodetype, dataencodetype;
  netwib_bool extended;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000015_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'f', &filename));
  netwib_er(netwox_arg_dlt(parg, 't', &recorddlt))
  netwib_er(netwox_arg_encode(parg, 'H', &hdrencodetype));
  netwib_er(netwox_arg_encode(parg, 'D', &dataencodetype));
  netwib_er(netwox_arg_bool(parg, 'x', &extended));

  /* main loop */
  netwib_er(t000015_loop(&filename, recorddlt, hdrencodetype, dataencodetype,
                         extended));

  /* close */
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
