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
netwib_conststring t000031_description[] = {
  "This tool converts a file using Windows end of lines style (0x0D0A) to",
  "a file using Unix end of lines style (0x0A).",
  "This tool permits to convert text files. It does not work on binary",
  "files.",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000031_args[] = {
  NETWOX_TOOLARG_REQ_BUF_FILE_RD('i', NULL, NULL, NULL),
  NETWOX_TOOLARG_REQ_BUF_FILE_WR('o', NULL, NULL, NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000031_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NOTNET_FILE,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000031_info = {
  "Convert a file from dos to unix",
  t000031_description,
  "dos2unix",
  t000031_args,
  t000031_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000031_loop(netwib_constbuf *pfilein,
                               netwib_buf *pfileouttmp)
{
  netwib_buf buf;
  netwib_io *pioin, *pioin2, *pioout;
  netwib_err ret=NETWIB_ERR_OK;

  /* initialize pio */
  netwib_er(netwib_io_init_file_read(pfilein, &pioin2));
  netwib_er(netwib_io_init_data_line(&pioin));
  netwib_er(netwib_io_plug_read(pioin, pioin2));
  netwib_er(netwib_io_init_filetemp(NETWIB_FALSE, pfileouttmp, &pioout));

  /* main loop */
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  while (NETWIB_TRUE) {
    /* read */
    ret = netwib_io_read(pioin, &buf);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret == NETWIB_ERR_DATAEND) {
      ret = NETWIB_ERR_OK;
      break;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    netwib_er(netwib_buf_append_string("\n", &buf));
    /* write when sufficient (windows cannot write more than 32k) */
    if (netwib__buf_ref_data_size(&buf) > 31000) {
      netwib_er(netwib_io_write(pioout, &buf));
      netwib__buf_reinit(&buf);
    }
  }
  /* write left data */
  if (netwib__buf_ref_data_size(&buf)) {
    netwib_er(netwib_io_write(pioout, &buf));
  }

  /* close */
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_io_close(&pioin));
  netwib_er(netwib_io_close(&pioout));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000031_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf filein, fileouttmp, fileout;
  netwib_err ret;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000031_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'i', &filein));
  netwib_er(netwox_arg_buf(parg, 'o', &fileout));

  netwib_er(netwib_buf_init_mallocdefault(&fileouttmp));
  netwib_er(netwib_buf_append_buf(&fileout, &fileouttmp));
  ret = t000031_loop(&filein, &fileouttmp);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_filename_rename(&fileouttmp, &fileout));
  } else {
    netwib_er(netwib_filename_remove(&fileouttmp));
  }

  netwib_er(netwib_buf_close(&fileouttmp));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
