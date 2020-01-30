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
netwib_conststring t000128_description[] = {
  "This tool splits a file in chunks. For example, splitting a file in",
  "blocks of 1M44 permits to store it on several floppies. See also tool",
  "129.",
  "For example running 'netwox 128 file --chunk-size 100' will create",
  "several files named file.0, file.1, etc.",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000128_args[] = {
  NETWOX_TOOLARG_REQ_BUF_FILE_RD('f', "file", "file", NULL),
  NETWOX_TOOLARG_OPT_UINT32('s', "chunk-size", "max chunk size in bytes",
                            "1400000"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000128_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NOTNET_FILE,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000128_info = {
  "Split a file in smaller chunks",
  t000128_description,
  "piece",
  t000128_args,
  t000128_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000128_file_init(netwib_bufpool *pbufpool,
                                    netwib_constbuf *pfilein,
                                    netwib_uint32 filenum,
                                    netwib_io **ppioout)
{
  netwib_buf *pbuf;
  netwib_bool exists;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
  netwib_er(netwib_buf_append_fmt(pbuf, "%{buf}.%{uint32}", pfilein, filenum));
  netwib_er(netwib_filename_exists(pbuf, &exists));
  if (exists) {
    netwib_er(netwib_fmt_display("Error : file %{buf} already exists\n",
                                 pbuf));
  } else {
    netwib_er(netwib_io_init_file_write(pbuf, ppioout));
  }
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));
  if (exists) {
    return(NETWOX_ERR_PROTOCOL);
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000128_loop(netwib_constbuf *pfilein,
                               netwib_io *pioin,
                               netwib_uint32 maxsize)
{
  netwib_bufpool *pbufpool;
  netwib_buf *pbuf, buf;
  netwib_io *pioout;
  netwib_uint32 filenum, missingsize, datasize;
  netwib_err ret=NETWIB_ERR_OK;

  /* initialize memory */
  netwib_er(netwib_bufpool_initdefault(&pbufpool));

  /* initialize pio */
  filenum = 1;
  netwib_er(t000128_file_init(pbufpool, pfilein, filenum++, &pioout));

  /* main loop */
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
  missingsize = maxsize;
  while (NETWIB_TRUE) {
    /* read */
    datasize = netwib__buf_ref_data_size(pbuf);
    if (datasize == 0) {
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
      datasize = netwib__buf_ref_data_size(pbuf);
    }
    /* write */
    if (datasize <= missingsize) {
      netwib_er(netwib_io_write(pioout, pbuf));
      netwib__buf_reinit(pbuf);
      missingsize -= datasize;
    } else {
      buf = *pbuf;
      buf.endoffset = buf.beginoffset + missingsize;
      netwib_er(netwib_io_write(pioout, &buf));
      netwib_er(netwib_io_close(&pioout));
      netwib_er(t000128_file_init(pbufpool, pfilein, filenum++, &pioout));
      pbuf->beginoffset += missingsize;
      missingsize = maxsize;
    }
  }

  /* close */
  netwib_er(netwib_io_close(&pioout));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));
  netwib_er(netwib_bufpool_close(&pbufpool));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000128_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf filein;
  netwib_io *pioin;
  netwib_uint32 maxsize;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000128_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'f', &filein));
  netwib_er(netwox_arg_uint32(parg, 's', &maxsize));

  netwib_er(netwib_io_init_file_read(&filein, &pioin));
  netwib_er(t000128_loop(&filein, pioin, maxsize));
  netwib_er(netwib_io_close(&pioin));

  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}

