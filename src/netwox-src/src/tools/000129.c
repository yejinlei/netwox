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
netwib_conststring t000129_description[] = {
  "This tool reassembles split file. See also tool 128.",
  "For example, if tool 128 created file.0, file.1, etc., tool 129 will",
  "reassemble chunks: 'netwox 129 file'. Please note filename has to be",
  "the same as the one used for tool 128 (the original filename).",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000129_args[] = {
  NETWOX_TOOLARG_REQ_BUF_FILE_WR('f', "file",
                                 "filename (without .1/.2/etc extension)",
                                 NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000129_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NOTNET_FILE,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000129_info = {
  "Reassemble chunks of a file",
  t000129_description,
  "piece",
  t000129_args,
  t000129_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000129_file_init(netwib_bufpool *pbufpool,
                                    netwib_constbuf *pfileout,
                                    netwib_uint32 filenum,
                                    netwib_io **ppioin)
{
  netwib_buf *pbuf;
  netwib_bool exists;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
  netwib_er(netwib_buf_append_fmt(pbuf, "%{buf}.%{uint32}", pfileout,
                                  filenum));
  netwib_er(netwib_filename_exists(pbuf, &exists));
  if (exists) {
    netwib_er(netwib_fmt_display("Reading %{buf}\n", pbuf));
    netwib_er(netwib_io_init_file_read(pbuf, ppioin));
  }
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));
  if (!exists) {
    return(NETWIB_ERR_DATAEND);
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000129_loop(netwib_bufpool *pbufpool,
                               netwib_constbuf *pfileout,
                               netwib_io *pioout)
{
  netwib_buf *pbuf;
  netwib_io *pioin;
  netwib_uint32 filenum;
  netwib_err ret=NETWIB_ERR_OK;

  /* initialize pio */
  filenum = 1;
  netwib_er(t000129_file_init(pbufpool, pfileout, filenum++, &pioin));

  /* main loop */
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
  while (NETWIB_TRUE) {
    /* read */
    ret = netwib_io_read(pioin, pbuf);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret == NETWIB_ERR_DATAEND) {
      netwib_er(netwib_io_close(&pioin));
      ret = t000129_file_init(pbufpool, pfileout, filenum++, &pioin);
      if (ret != NETWIB_ERR_OK) {
        if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
        break;
      }
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* write */
    netwib_er(netwib_io_write(pioout, pbuf));
    netwib__buf_reinit(pbuf);
  }

  /* close */
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000129_core(int argc, char *argv[])
{
  netwib_bufpool *pbufpool;
  netwox_arg *parg;
  netwib_buf fileout, *pbuf;
  netwib_io *pioout;
  netwib_bool exists;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000129_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'f', &fileout));

  /* initialize memory */
  netwib_er(netwib_bufpool_initdefault(&pbufpool));

  /* checks */
  netwib_er(netwib_filename_exists(&fileout, &exists));
  if (exists) {
    netwib_er(netwib_fmt_display("Error : file %{buf} already exists\n",
                                 &fileout));
    return(NETWOX_ERR_PROTOCOL);
  }
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
  netwib_er(netwib_buf_append_buf(&fileout, pbuf));
  netwib_er(netwib_buf_append_string(".1", pbuf));
  netwib_er(netwib_filename_exists(pbuf, &exists));
  if (!exists) {
    netwib_er(netwib_fmt_display("Error : file %{buf}.1 does not exist\n",
                                 &fileout));
    return(NETWOX_ERR_PROTOCOL);
  }

  /* open and loop */
  netwib_er(netwib_io_init_file_write(&fileout, &pioout));
  netwib_er(t000129_loop(pbufpool, &fileout, pioout));
  netwib_er(netwib_io_close(&pioout));

  /* close */
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));
  netwib_er(netwib_bufpool_close(&pbufpool));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}

