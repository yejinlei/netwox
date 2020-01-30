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
netwib_conststring t000220_description[] = {
  "This tool converts a file to a base64 encoded file. Base64 encoding is",
  "often used in MIME.",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000220_args[] = {
  NETWOX_TOOLARG_REQ_BUF_FILE_RD('i', NULL, NULL, NULL),
  NETWOX_TOOLARG_REQ_BUF_FILE_WR('o', NULL, NULL, NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000220_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NOTNET_FILE,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000220_info = {
  "Convert a binary file to a base64 encoded file",
  t000220_description,
  NULL,
  t000220_args,
  t000220_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000220_loop(netwib_constbuf *pfilein,
                               netwib_constbuf *pfileout)
{
  netwib_buf buf, buf2, buf3;
  netwib_io *pioin, *pioout;
  netwib_err ret=NETWIB_ERR_OK;

  /* initialize pio */
  netwib_er(netwib_io_init_file_read(pfilein, &pioin));
  netwib_er(netwib_io_init_file_textwrite(pfileout, &pioout));

  /* main loop */
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));
  buf.flags |= NETWIB_BUF_FLAGS_CANSLIDE;
  while (NETWIB_TRUE) {
    /* read */
    ret = netwib_io_read(pioin, &buf);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret != NETWIB_ERR_OK && ret != NETWIB_ERR_DATAEND) {
      break;
    }
    /* write */
    netwib__buf_reinit(&buf2);
    buf3 = buf;
    if (ret != NETWIB_ERR_DATAEND) {
      buf3.endoffset -= (netwib__buf_ref_data_size(&buf) % 54);
    }
    netwib_er(netwib_buf_encode(&buf3, NETWIB_ENCODETYPE_BASE64_WRAP, &buf2));
    buf.beginoffset = buf3.endoffset;
    netwib_er(netwib_io_write(pioout, &buf2));
    if (ret == NETWIB_ERR_DATAEND) {
      ret = NETWIB_ERR_OK;
      break;
    }
  }

  /* close */
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_close(&buf2));
  netwib_er(netwib_io_close(&pioin));
  netwib_er(netwib_io_close(&pioout));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000220_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf filein, fileout;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000220_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'i', &filein));
  netwib_er(netwox_arg_buf(parg, 'o', &fileout));

  netwib_er(t000220_loop(&filein, &fileout));

  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
