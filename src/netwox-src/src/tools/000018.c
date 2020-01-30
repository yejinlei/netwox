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
netwib_conststring t000018_description[] = {
  NETWOX_DESC_record,
  "",
  "This tool reads packet from one record, tries to reassemble IP packets",
  "and reorder TCP sequences. Please note, packets may be lost.",
  "",
  NETWOX_DESC_record_srcfile,
  NETWOX_DESC_record_indlt,
  NETWOX_DESC_record_dstfile,
  NETWOX_DESC_record_encode,
  NETWOX_DESC_sniff_tcpreord,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000018_args[] = {
  NETWOX_TOOLARG_REQ_BUF_FILE_RD('f', NULL, "input record file", NULL),
  NETWOX_TOOLARG_OPT_DLT('t', "input-dlt", "dlt type of input record", NULL),
  NETWOX_TOOLARG_REQ_BUF_FILE_WR('F', NULL, "output record file", NULL),
  NETWOX_TOOLARG_OPT_RECORDENCODE('r', "recordencode",
                                  "encoding type for output record", NULL),
  NETWOX_TOOLARG_OPT_BOOL('o', "tcpreord", "also reorder TCP packets", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000018_nodes[] = {
  NETWOX_TOOLTREENODETYPE_RECORD,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000018_info = {
  "Reassemble IP packets of a record, and reorder TCP flow",
  t000018_description,
  "capture",
  t000018_args,
  t000018_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000018_loop(netwib_constbuf *pfilenamein,
                               netwib_constbuf *pfilenameout,
                               netwib_device_dlttype dltin,
                               netwib_record_encodetype encodetype,
                               netwib_bool tcpreord)
{
  netwib_buf pkt;
  netwib_io *pioin, *pioin2, *pioout;
  netwib_err ret;

  /* initialize read record */
  netwib_er(netwox_io_init_record_sniff(pfilenamein, dltin, &pioin));
  netwib_er(netwib_io_init_sniff_ipreas(&pioin2));
  netwib_er(netwib_io_plug_read(pioin2, pioin));
  pioin = pioin2;
  if (tcpreord) {
    netwib_er(netwib_io_init_sniff_tcpreord(&pioin2));
    netwib_er(netwib_io_plug_read(pioin2, pioin));
    pioin = pioin2;
  }

  /* initialize write record */
  ret = netwib_io_init_record_write(pfilenameout, encodetype, &pioout);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_io_close(&pioin));
    return(ret);
  }

  /* main loop */
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
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
    netwib_er(netwib_io_write(pioout, &pkt));
  }

  /* close */
  netwib_er(netwib_buf_close(&pkt));
  netwib_er(netwib_io_close(&pioin));
  netwib_er(netwib_io_close(&pioout));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000018_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf filenamein, filenameout;
  netwib_device_dlttype dltin;
  netwib_record_encodetype encodetype;
  netwib_bool tcpreord;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000018_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'f', &filenamein));
  netwib_er(netwox_arg_buf(parg, 'F', &filenameout));
  netwib_er(netwox_arg_dlt(parg, 't', &dltin));
  netwib_er(netwox_arg_recordencode(parg, 'r', &encodetype));
  netwib_er(netwox_arg_bool(parg, 'o', &tcpreord));

  /* main loop */
  netwib_er(t000018_loop(&filenamein, &filenameout, dltin, encodetype,
                         tcpreord));

  /* close */
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
