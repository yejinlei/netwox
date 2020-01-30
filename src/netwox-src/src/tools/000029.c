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
netwib_conststring t000029_description[] = {
  "This tool does the reverse of tool 28.",
  "Editing a binary file is not easy with un-appropriate tools. Tool 28",
  "converts a binary file which can be edited by a text editor. Tool 29",
  "then recreates the binary file.",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000029_args[] = {
  NETWOX_TOOLARG_REQ_BUF_FILE_RD('i', NULL, NULL, NULL),
  NETWOX_TOOLARG_REQ_BUF_FILE_WR('o', NULL, NULL, NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000029_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NOTNET_FILE,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000029_info = {
  "Convert a readable and editable file to a binary file",
  t000029_description,
  "hexedit",
  t000029_args,
  t000029_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000029_loop(netwib_constbuf *pfilein,
                               netwib_constbuf *pfileout)
{
  netwib_buf buf;
  netwib_io *pioin, *pioout;
  netwib_err ret=NETWIB_ERR_OK;

  /* initialize pio */
  netwib_er(netwib_io_init_record_read(pfilein, &pioin));
  netwib_er(netwib_io_init_file_write(pfileout, &pioout));

  /* main loop */
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  while (NETWIB_TRUE) {
    /* read */
    netwib__buf_reinit(&buf);
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
    /* write */
    netwib_er(netwib_io_write(pioout, &buf));
  }

  /* close */
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_io_close(&pioin));
  netwib_er(netwib_io_close(&pioout));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000029_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf filein, fileout;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000029_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'i', &filein));
  netwib_er(netwox_arg_buf(parg, 'o', &fileout));

  netwib_er(t000029_loop(&filein, &fileout));

  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
