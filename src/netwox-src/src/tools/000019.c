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
netwib_conststring t000019_description[] = {
  NETWOX_DESC_record,
  "",
  "This tool extracts a range of packets from one record, and save them",
  "in another record. For example, it can extract packet number 2 to",
  "number 5.",
  "",
  NETWOX_DESC_record_srcfile,
  NETWOX_DESC_record_dstfile,
  NETWOX_DESC_record_encode,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000019_args[] = {
  NETWOX_TOOLARG_REQ_BUF_FILE_RD('f', NULL, "input record file", NULL),
  NETWOX_TOOLARG_REQ_BUF_FILE_WR('F', NULL, "output record file", NULL),
  NETWOX_TOOLARG_OPT_RECORDENCODE('r', "recordencode",
                                  "encoding type for output record", NULL),
  NETWOX_TOOLARG_OPT_UINT32('s', "start", "start packet (from 1)", NULL),
  NETWOX_TOOLARG_OPT_UINT32('e', "end", "end packet (0=nolimit)", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000019_nodes[] = {
  NETWOX_TOOLTREENODETYPE_RECORD,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000019_info = {
  "Extract a range of packets from a record",
  t000019_description,
  "capture",
  t000019_args,
  t000019_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000019_loop(netwib_constbuf *pfilenamein,
                               netwib_constbuf *pfilenameout,
                               netwib_record_encodetype encodetype,
                               netwib_uint32 start,
                               netwib_uint32 end)
{
  netwib_buf pkt;
  netwib_io *pioin, *pioout;
  netwib_uint32 i;
  netwib_err ret;

  /* clean input */
  if (start == 0) start = 1;
  if (end == 0) end = 0xFFFFFFFFu; /* may round, but ignore */
  if (start > end) return(NETWIB_ERR_PAINVALIDRANGE);

  /* initialize records */
  netwib_er(netwib_io_init_record_read(pfilenamein, &pioin));
  ret = netwib_io_init_record_write(pfilenameout, encodetype, &pioout);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_io_close(&pioin));
    return(ret);
  }

  /* main loop */
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  i = 1;
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
    /* write packet */
    if (i >= start && i <= end) {
      netwib_er(netwib_io_write(pioout, &pkt));
    }
    i++;
  }

  /* close */
  netwib_er(netwib_buf_close(&pkt));
  netwib_er(netwib_io_close(&pioin));
  netwib_er(netwib_io_close(&pioout));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000019_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf filenamein, filenameout;
  netwib_record_encodetype encodetype;
  netwib_uint32 start, end;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000019_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'f', &filenamein));
  netwib_er(netwox_arg_buf(parg, 'F', &filenameout));
  netwib_er(netwox_arg_recordencode(parg, 'r', &encodetype));
  netwib_er(netwox_arg_uint32(parg, 's', &start));
  netwib_er(netwox_arg_uint32(parg, 'e', &end));

  /* main loop */
  netwib_er(t000019_loop(&filenamein, &filenameout, encodetype, start, end));

  /* close */
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
