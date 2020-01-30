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
netwib_conststring t000026_description[] = {
  "This tool dumps the content of a file.",
  "",
  "By using default encoding type (dump), its output has two main columns:",
  " - on left, data represented as hexadecimal",
  " - on right, data with unprintable characters converted to dots",
  "",
  "Other encoding types permit to obtain base64 (base64_wrap), mixed,",
  "etc.",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000026_args[] = {
  NETWOX_TOOLARG_REQ_BUF_FILE_RD('f', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPT_ENCODE('e', "encode", "encoding type", "dump"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000026_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NOTNET_FILE,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000026_info = {
  "Dump a file",
  t000026_description,
  "hexadecimal, printable",
  t000026_args,
  t000026_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000026_display(netwib_encodetype encodetype,
                                  netwib_uint32 alignsize,
                                  netwib_buf *pdatabuf,
                                  netwib_buf *ptmpbuf)
{
  netwib_uint32 leftsize;

  netwib__buf_reinit(ptmpbuf);

  leftsize = 0;
  if (alignsize) {
    leftsize = netwib__buf_ref_data_size(pdatabuf) % alignsize;
  }

  if (leftsize) {
    pdatabuf->endoffset -= leftsize;
    netwib_er(netwib_buf_encode(pdatabuf, encodetype, ptmpbuf));
    netwib_er(netwib_buf_display(ptmpbuf, NETWIB_ENCODETYPE_DATA));
    pdatabuf->beginoffset = pdatabuf->endoffset;
    pdatabuf->endoffset += leftsize;
  } else {
    netwib_er(netwib_buf_encode(pdatabuf, encodetype, ptmpbuf));
    netwib_er(netwib_buf_display(ptmpbuf, NETWIB_ENCODETYPE_DATA));
    netwib__buf_reinit(pdatabuf);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000026_loop(netwib_constbuf *pfilename,
                               netwib_encodetype encodetype)
{
  netwib_buf buf, buf2;
  netwib_io *pio;
  netwib_uint32 alignsize;
  netwib_err ret=NETWIB_ERR_OK;

  /* initialize pio */
  netwib_er(netwib_io_init_file_read(pfilename, &pio));

  /* decide if display use fixed/aligned size */
  alignsize = 0; /* no need to align */
  switch(encodetype) {
  case NETWIB_ENCODETYPE_DATA :
  case NETWIB_ENCODETYPE_LOWERCASE :
  case NETWIB_ENCODETYPE_UPPERCASE :
  case NETWIB_ENCODETYPE_HEXA0 :
  case NETWIB_ENCODETYPE_HEXA1 :
  case NETWIB_ENCODETYPE_HEXA2 :
  case NETWIB_ENCODETYPE_HEXA4 :
  case NETWIB_ENCODETYPE_MIXED0 :
  case NETWIB_ENCODETYPE_MIXED1 :
  case NETWIB_ENCODETYPE_TEXT :
  case NETWIB_ENCODETYPE_BASE64 :
  case NETWIB_ENCODETYPE_QUOTED :
  case NETWIB_ENCODETYPE_NOTHING :
  case NETWIB_ENCODETYPE_SYNTH :
  case NETWIB_ENCODETYPE_DATA_WRAP :
  case NETWIB_ENCODETYPE_TEXT_WRAP :
    alignsize = 0;
    break;
  case NETWIB_ENCODETYPE_HEXA0_WRAP :
  case NETWIB_ENCODETYPE_HEXA2_WRAP :
  case NETWIB_ENCODETYPE_HEXA4_WRAP :
    alignsize = 32;
    break;
  case NETWIB_ENCODETYPE_HEXA1_WRAP :
  case NETWIB_ENCODETYPE_MIXED0_WRAP :
  case NETWIB_ENCODETYPE_MIXED1_WRAP :
  case NETWIB_ENCODETYPE_DUMP :
    alignsize = 16;
    break;
  case NETWIB_ENCODETYPE_MIXED0H_WRAP :
  case NETWIB_ENCODETYPE_MIXED1H_WRAP :
    alignsize = 8;
    break;
  case NETWIB_ENCODETYPE_ARRAY1 :
  case NETWIB_ENCODETYPE_ARRAY4 :
  case NETWIB_ENCODETYPE_ARRAY8 :
  case NETWIB_ENCODETYPE_ARRAY16 :
  case NETWIB_ENCODETYPE_ARRAY32 :
    alignsize = 4;
    break;
  case NETWIB_ENCODETYPE_BASE64_WRAP :
    alignsize = 54;
    break;
  default :
    break;
  }

  /* main loop */
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  buf.flags |= NETWIB_BUF_FLAGS_CANSLIDE;
  netwib_er(netwib_buf_init_mallocdefault(&buf2));
  while (NETWIB_TRUE) {
    /* read */
    ret = netwib_io_read(pio, &buf);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret == NETWIB_ERR_DATAEND) {
      /* eventually display left data */
      if (netwib__buf_ref_data_size(&buf) != 0) {
        netwib_er(t000026_display(encodetype, 0, &buf, &buf2));
      }
      ret = NETWIB_ERR_OK;
      break;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* display to screen */
    netwib_er(t000026_display(encodetype, alignsize, &buf, &buf2));
  }

  /* close */
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_close(&buf2));
  netwib_er(netwib_io_close(&pio));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000026_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf filename;
  netwib_encodetype encodetype;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000026_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'f', &filename));
  netwib_er(netwox_arg_encode(parg, 'e', &encodetype));

  netwib_er(t000026_loop(&filename, encodetype));

  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
