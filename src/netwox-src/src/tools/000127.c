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
netwib_conststring t000127_description[] = {
  "This tool permits to cypher and decypher a file. This algorithm is not",
  "very strong, and can be defeated using a known text attack.",
  "It uses the --password parameter to generate a pseudo-random string.",
  "Then, a XOR is done between the string and the file's content.",
  "Due to XOR properties, this tool can also decrypt.",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000127_args[] = {
  NETWOX_TOOLARG_REQ_BUF_PASSWORD('p', NULL, NULL, NULL),
  NETWOX_TOOLARG_REQ_BUF_FILE_RD('i', NULL, NULL, NULL),
  NETWOX_TOOLARG_REQ_BUF_FILE_WR('o', NULL, NULL, NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000127_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NOTNET_FILE,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000127_info = {
  "Cypher/decypher a file using a xor",
  t000127_description,
  NULL,
  t000127_args,
  t000127_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000127_loop(netwib_constbuf *pfilein,
                               netwib_buf *pfileouttmp,
                               netwib_constbuf *ppassword)
{
  netwib_bufpool *pbufpool;
  netwib_buf *pbuf, *pkey;
  netwib_io *pioin, *pioout;
  netwox_xor_context context;
  netwib_err ret=NETWIB_ERR_OK;

  /* initialize memory */
  netwib_er(netwib_bufpool_initdefault(&pbufpool));

  /* initialize pio */
  netwib_er(netwib_io_init_file_read(pfilein, &pioin));
  netwib_er(netwib_io_init_filetemp(NETWIB_FALSE, pfileouttmp, &pioout));

  /* initialize xor */
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pkey));
  netwib_er(netwox_xor_gene_key(pbufpool, ppassword, 1024, pkey));
  netwib_er(netwox_xor_init(pkey, &context));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pkey));

  /* main loop */
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
  while (NETWIB_TRUE) {
    /* read */
    ret = netwib_io_read(pioin, pbuf);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret == NETWIB_ERR_DATAEND) {
      ret = NETWIB_ERR_OK;
      break;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* cypher */
    netwib_er(netwox_xor_cypher(&context, pbuf));

    /* write */
    netwib_er(netwib_io_write(pioout, pbuf));
    netwib__buf_reinit(pbuf);
  }

  /* close */
  netwib_er(netwox_xor_close(&context));
  netwib_er(netwib_io_close(&pioin));
  netwib_er(netwib_io_close(&pioout));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));
  netwib_er(netwib_bufpool_close(&pbufpool));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000127_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf filein, fileouttmp, fileout, password;
  netwib_err ret;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000127_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'p', &password));
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwox_arg_buf(parg, 'i', &filein));
  netwib_er(netwox_arg_buf(parg, 'o', &fileout));

  netwib_er(netwib_buf_init_mallocdefault(&fileouttmp));
  netwib_er(netwib_buf_append_buf(&fileout, &fileouttmp));
  ret = t000127_loop(&filein, &fileouttmp, &password);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_filename_rename(&fileouttmp, &fileout));
  } else {
    netwib_er(netwib_filename_remove(&fileouttmp));
  }

  netwib_er(netwib_buf_close(&fileouttmp));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}

