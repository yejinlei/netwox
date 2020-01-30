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
netwib_conststring t000016_description[] = {
  NETWOX_DESC_record,
  "",
  "This tool converts a record. For example, it can convert a pcap",
  "capture file to a mixed capture file.",
  "",
  NETWOX_DESC_record_srcfile,
  NETWOX_DESC_record_indlt,
  NETWOX_DESC_record_dstfile,
  NETWOX_DESC_record_outdlt,
  NETWOX_DESC_record_encode,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000016_args[] = {
  NETWOX_TOOLARG_REQ_BUF_FILE_RD('f', NULL, "input record file", NULL),
  NETWOX_TOOLARG_OPT_DLT('t', "input-dlt", "dlt type of input record", NULL),
  NETWOX_TOOLARG_REQ_BUF_FILE_WR('F', NULL, "output record file", NULL),
  NETWOX_TOOLARG_OPT_DLT('T', "output-dlt", "dlt type of output record", NULL),
  NETWOX_TOOLARG_OPT_RECORDENCODE('r', "recordencode",
                                  "encoding type for output record", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000016_nodes[] = {
  NETWOX_TOOLTREENODETYPE_RECORD,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000016_info = {
  "Convert a record",
  t000016_description,
  "capture",
  t000016_args,
  t000016_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000016_loop(netwib_constbuf *pfilenamein,
                               netwib_constbuf *pfilenameout,
                               netwib_device_dlttype dltin,
                               netwib_device_dlttype dltout,
                               netwib_record_encodetype encodetype)
{
  netwib_buf pkt, pktout;
  netwib_io *pioin, *pioout;
  netwib_linkhdr linkhdr;
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
  while (NETWIB_TRUE) {
    /* read packet */
    pkt.beginoffset = NETWIB_LINKHDR_MAXLEN;
    pkt.endoffset = NETWIB_LINKHDR_MAXLEN;
    ret = netwib_io_read(pioin, &pkt);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    /* eventually convert packet */
    if (dltout != dltin) {
      netwib_er(netwib_pkt_convert(dltin, &pkt, NETWIB_PKT_NEWFIELD_FILLBLANK,
                                   NETWIB_PKT_NEWFIELD_FILLBLANK,
                                   dltout, &linkhdr, &pktout));
      pkt.beginoffset = pktout.totalptr - pkt.totalptr;
      netwib_er(netwib_pkt_prepend_linkhdr(&linkhdr, &pkt));
    }
    /* write packet */
    netwib_er(netwib_io_write(pioout, &pkt));
  }

  /* close */
  netwib_er(netwib_buf_close(&pkt));
  netwib_er(netwib_io_close(&pioin));
  netwib_er(netwib_io_close(&pioout));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000016_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf filenamein, filenameout;
  netwib_device_dlttype dltin, dltout;
  netwib_record_encodetype encodetype;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000016_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'f', &filenamein));
  netwib_er(netwox_arg_buf(parg, 'F', &filenameout));
  netwib_er(netwox_arg_dlt(parg, 't', &dltin));
  netwib_er(netwox_arg_dlt(parg, 'T', &dltout));
  netwib_er(netwox_arg_recordencode(parg, 'r', &encodetype));

  /* main loop */
  netwib_er(t000016_loop(&filenamein, &filenameout, dltin, dltout,
                         encodetype));

  /* close */
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
