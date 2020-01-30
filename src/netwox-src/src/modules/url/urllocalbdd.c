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
#include "../../netwox.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_urllocalbdd_search(netwib_constbuf *prootdir,
                                            netwib_bool searchinfield1,
                                            netwib_constbuf *psearched,
                                            netwib_buf *pfound) {
  netwib_buf urllist, urllistfilename, line, field1, field2;
  netwib_data data, pc;
  netwib_uint32 datasize;
  netwib_io *pfileio, *pio;
  netwib_bool fileioopen, ioopen;
  netwib_cmp cmp;
  netwib_err ret;

  /* initialize */
  ret = NETWIB_ERR_OK;
  fileioopen = NETWIB_FALSE;
  ioopen = NETWIB_FALSE;
  netwib_er(netwib_buf_init_mallocdefault(&urllistfilename));
  netwib_er(netwib_buf_init_mallocdefault(&line));

  /* open file */
  netwib_eg(netwib_buf_init_ext_string("urllist.txt", &urllist));
  netwib_eg(netwib_path_init_concat(prootdir, &urllist, &urllistfilename));
  netwib_eg(netwib_io_init_file_textread(&urllistfilename, &pfileio));
  fileioopen = NETWIB_TRUE;
  netwib_eg(netwib_io_init_data_line(&pio));
  ioopen = NETWIB_TRUE;
  netwib_eg(netwib_io_plug_read(pio, pfileio));

  /* read */
  while(NETWIB_TRUE) {
    /* obtain entry */
    netwib__buf_reinit(&line);
    ret = netwib_io_read(pio, &line);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_NOTFOUND;
      break;
    }
    /* split by fields */
    field1 = line;
    field2 = line;
    data = netwib__buf_ref_data_ptr(&line);
    datasize = netwib__buf_ref_data_size(&line);
    pc = netwib_c_memchr(data, '\t', datasize);
    if (pc == NULL) {
      netwib_goto(NETWIB_ERR_NOTCONVERTED);
    }
    field1.endoffset = field1.beginoffset + pc - data;
    field2.beginoffset = field1.endoffset + 1;
    /**/
    netwib_eg(netwib_buf_cmp(psearched, searchinfield1?(&field1):(&field2),
                             &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      netwib_eg(netwib_buf_append_buf(searchinfield1?(&field2):(&field1),
                                      pfound));
      netwib_goto(NETWIB_ERR_OK);
    }
  }

  /* end */
 netwib_gotolabel:
  if (ioopen) {
    netwib_er(netwib_io_close(&pio));
  } else if (fileioopen) {
    netwib_er(netwib_io_close(&pfileio));
  }
  netwib_er(netwib_buf_close(&line));
  netwib_er(netwib_buf_close(&urllistfilename));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_urllocalbdd_url_init_local(netwib_constbuf *prootdir,
                                             netwib_constbuf *plocalfilename,
                                             netwib_buf *purl) {
  return(netwib_urllocalbdd_search(prootdir, NETWIB_FALSE,
                                   plocalfilename, purl));
}

/*-------------------------------------------------------------*/
netwib_err netwib_urllocalbdd_local_init_url(netwib_constbuf *prootdir,
                                             netwib_constbuf *purl,
                                             netwib_buf *plocalfilename) {
  return(netwib_urllocalbdd_search(prootdir, NETWIB_TRUE,
                                   purl, plocalfilename));
}


