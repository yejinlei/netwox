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
netwib_conststring t000135_description[] = {
  "This tool converts urls/uris in a file.",
  "For example, file index.html downloaded from http://srv/R/index.html",
  "might contain urls of the form '../S/img.png'. Loading index.html in a",
  "browser will lead to broken images because they are not on the local",
  "computer. This tool converts '../S/img.png' to the absolute real url",
  "'http://srv/S/img.png'. Here is how to use it:",
  "  netwox 135 index.html index2.html http://example.com/R/",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000135_args[] = {
  NETWOX_TOOLARG_REQ_BUF_FILE_RD('i', NULL, "input HTML file", "t.htm"),
  NETWOX_TOOLARG_REQ_BUF_FILE_WR('o', NULL, "created HTML file", "out.htm"),
  NETWOX_TOOLARG_REQ_BUF_URI('u', "url", "absolute url of file",
                             "http://s/t.htm"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000135_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NOTNET,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000135_info = {
  "Convert urls/uris in a HMTL file to absolute urls",
  t000135_description,
  NULL,
  t000135_args,
  t000135_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000135_copy_replace(netwib_ring *pring)
{
  netwib_ring_index *pringindex;
  netwox_htmlfile_urllist_item *pitem;
  netwib_err ret;

  netwib_er(netwib_ring_index_init(pring, &pringindex));

  ret = NETWIB_ERR_OK;
  while(NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pitem);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    if (pitem->elmtattr == NETWOX_HTMLTAG_ELMTATTR_BASE_HREF) {
      pitem->suppresstag = NETWIB_TRUE;
    } else {
      netwib_er(netwib_buf_append_buf(&pitem->value,
                                      &pitem->replacingvalue));
    }
  }

  netwib_er(netwib_ring_index_close(&pringindex));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000135_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_ring *pring;
  netwib_bufpool *pbufpool;
  netwib_buf filenamein, filenameout, filenameurl;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000135_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'i', &filenamein));
  netwib_er(netwox_arg_buf(parg, 'o', &filenameout));
  netwib_er(netwox_arg_buf(parg, 'u', &filenameurl));

  /* display */
  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  netwib_er(netwox_htmlfile_urllist_ring_init(&pring));
  netwib_er(netwox_htmlfile_urllist_file_read(&filenamein, NETWIB_TRUE,
                                              pring));
  /*netwib_er(netwox_htmlfile_urllist_ring_display(pring));*/
  netwib_er(netwox_htmlfile_urllist_canon(pbufpool, &filenameurl, NETWIB_FALSE,
                                          pring));
  /*netwib_er(netwox_htmlfile_urllist_ring_display(pring));*/
  netwib_er(t000135_copy_replace(pring));
  /*netwib_er(netwox_htmlfile_urllist_ring_display(pring));*/
  netwib_er(netwox_htmlfile_urllist_file_write(&filenamein, pring,
                                               &filenameout));
  netwib_er(netwox_htmlfile_urllist_ring_close(&pring));
  netwib_er(netwib_bufpool_close(&pbufpool));

  /* close */
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
