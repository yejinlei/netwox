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
netwib_conststring t000222_description[] = {
  "This tool suppresses links to local urls/uris located in a HTML file.",
  "For example, a HTML file may contain http://s/, mailto:a@a, dir/file.",
  "In this case, only the first one (http://s/) is kept. Other tags are",
  "suppressed.",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000222_args[] = {
  NETWOX_TOOLARG_REQ_BUF_FILE_RD('i', NULL, "input HTML file", "t.htm"),
  NETWOX_TOOLARG_REQ_BUF_FILE_WR('o', NULL, "created HTML file", "out.htm"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000222_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NOTNET,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000222_info = {
  "In a HMTL file, suppress links pointing to local urls",
  t000222_description,
  NULL,
  t000222_args,
  t000222_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000222_suppress_local(netwib_ring *pring)
{
  netwib_ring_index *pringindex;
  netwox_htmlfile_urllist_item *pitem;
  netwox_htmltag_elmtattr_uriformat uriformat;
  netwox_url_scheme scheme;
  netwib_data data;
  netwib_uint32 datasize;
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
      continue;
    }
    netwib_er(netwox_htmltag_uriformat_init_elmtattr(pitem->elmtattr,
                                                     &uriformat));
    if (uriformat != NETWOX_HTMLTAG_ELMTATTR_URIFORMAT_ONE) {
      pitem->suppresstag = NETWIB_TRUE;
      continue;
    }
    datasize = netwib__buf_ref_data_size(&pitem->value);
    if (datasize == 0 ) {
      pitem->suppresstag = NETWIB_TRUE;
      continue;
    }
    data = netwib__buf_ref_data_ptr(&pitem->value);
    if (data[0] != '#') {
      netwib_er(netwox_url_decode_scheme(&pitem->value, &scheme, NULL));
      if (scheme == NETWOX_URL_SCHEME_NONE ||
          scheme == NETWOX_URL_SCHEME_UNKNOWN) {
        pitem->suppresstag = NETWIB_TRUE;
        continue;
      }
    }
    netwib_er(netwib_buf_append_buf(&pitem->value,
                                    &pitem->replacingvalue));
  }

  netwib_er(netwib_ring_index_close(&pringindex));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000222_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_ring *pring;
  netwib_bufpool *pbufpool;
  netwib_buf filenamein, filenameout;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000222_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'i', &filenamein));
  netwib_er(netwox_arg_buf(parg, 'o', &filenameout));

  /* display */
  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  netwib_er(netwox_htmlfile_urllist_ring_init(&pring));
  netwib_er(netwox_htmlfile_urllist_file_read(&filenamein, NETWIB_TRUE,
                                              pring));
  /*netwib_er(netwox_htmlfile_urllist_ring_display(pring));*/
  netwib_er(t000222_suppress_local(pring));
  /*netwib_er(netwox_htmlfile_urllist_ring_display(pring));*/
  netwib_er(netwox_htmlfile_urllist_file_write(&filenamein, pring,
                                               &filenameout));
  netwib_er(netwox_htmlfile_urllist_ring_close(&pring));
  netwib_er(netwib_bufpool_close(&pbufpool));

  /* close */
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
