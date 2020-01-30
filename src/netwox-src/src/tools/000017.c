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
netwib_conststring t000017_description[] = {
  NETWOX_DESC_record,
  "",
  "This tool reads packet from one record, recompute their checksums, and",
  "write them in another record.",
  "",
  NETWOX_DESC_record_srcfile,
  NETWOX_DESC_record_indlt,
  NETWOX_DESC_record_dstfile,
  NETWOX_DESC_record_encode,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000017_args[] = {
  NETWOX_TOOLARG_REQ_BUF_FILE_RD('f', NULL, "input record file", NULL),
  NETWOX_TOOLARG_OPT_DLT('t', "input-dlt", "dlt type of input record", NULL),
  NETWOX_TOOLARG_REQ_BUF_FILE_WR('F', NULL, "output record file", NULL),
  NETWOX_TOOLARG_OPT_RECORDENCODE('r', "recordencode",
                                  "encoding type for output record", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000017_nodes[] = {
  NETWOX_TOOLTREENODETYPE_RECORD,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000017_info = {
  "Recompute checksums of packets in a record",
  t000017_description,
  "capture, verify",
  t000017_args,
  t000017_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000017_loop(netwib_constbuf *pfilenamein,
                               netwib_constbuf *pfilenameout,
                               netwib_device_dlttype dltin,
                               netwib_record_encodetype encodetype)
{
  netwib_buf pkt, pktout;
  netwib_io *pioin, *pioout;
  netwib_err ret;

  /* initialize record */
  netwib_er(netwib_io_init_record_read(pfilenamein, &pioin));
  ret = netwib_io_init_record_write(pfilenameout, encodetype, &pioout);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_io_close(&pioin));
    return(ret);
  }

  /* main loop */
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_buf_init_mallocdefault(&pktout));
  while (NETWIB_TRUE) {
    /* read packet */
    netwib__buf_reinit(&pkt);
    ret = netwib_io_read(pioin, &pkt);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    /* recompute checksum */
    netwib__buf_reinit(&pktout);
    ret = netwox_checksum_compute_link(dltin, &pkt, &pktout);
    /* write packet */
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_io_write(pioout, &pktout));
    } else {
      netwib_er(netwib_io_write(pioout, &pkt));
    }
  }

  /* close */
  netwib_er(netwib_buf_close(&pkt));
  netwib_er(netwib_buf_close(&pktout));
  netwib_er(netwib_io_close(&pioin));
  netwib_er(netwib_io_close(&pioout));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000017_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf filenamein, filenameout;
  netwib_device_dlttype dltin;
  netwib_record_encodetype encodetype;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000017_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'f', &filenamein));
  netwib_er(netwox_arg_buf(parg, 'F', &filenameout));
  netwib_er(netwox_arg_dlt(parg, 't', &dltin));
  netwib_er(netwox_arg_recordencode(parg, 'r', &encodetype));

  /* main loop */
  netwib_er(t000017_loop(&filenamein, &filenameout, dltin, encodetype));

  /* close */
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
