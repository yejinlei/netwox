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
netwib_conststring t000028_description[] = {
  "This tool converts a file to a mixed file. See also tool 29.",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000028_args[] = {
  NETWOX_TOOLARG_REQ_BUF_FILE_RD('i', NULL, NULL, NULL),
  NETWOX_TOOLARG_REQ_BUF_FILE_WR('o', NULL, NULL, NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000028_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NOTNET_FILE,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000028_info = {
  "Convert a binary file to readable and editable file",
  t000028_description,
  "hexedit",
  t000028_args,
  t000028_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000028_loop(netwib_constbuf *pfilein,
                               netwib_constbuf *pfileout)
{
  netwib_buf buf, buf2;
  netwib_io *pioin, *pioout;
  netwib_err ret=NETWIB_ERR_OK;

  /* initialize pio */
  netwib_er(netwib_io_init_file_read(pfilein, &pioin));
  netwib_er(netwib_io_init_file_textwrite(pfileout, &pioout));

  /* main loop */
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));
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
    netwib__buf_reinit(&buf2);
    netwib_er(netwib_buf_encode(&buf, NETWIB_ENCODETYPE_DUMP, &buf2));
    netwib_er(netwib_buf_append_string("\n", &buf2));
    netwib_er(netwib_io_write(pioout, &buf2));
  }

  /* close */
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_close(&buf2));
  netwib_er(netwib_io_close(&pioin));
  netwib_er(netwib_io_close(&pioout));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000028_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf filein, fileout;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000028_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'i', &filein));
  netwib_er(netwox_arg_buf(parg, 'o', &fileout));

  netwib_er(t000028_loop(&filein, &fileout));

  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
