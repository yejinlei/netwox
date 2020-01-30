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
netwib_err netwox_webspiderurl2_item_create(netwib_bool parsehtml,
                                            netwib_constbuf *plocalrelativefilename,
                                            netwib_ptr *ppitem)
{
  netwox_webspiderurl2_item *pitem;

  netwib_er(netwib_ptr_malloc(sizeof(netwox_webspiderurl2_item), ppitem));
  pitem = *ppitem;

  pitem->parsehtml = parsehtml;
  netwib_er(netwib_buf_init_mallocdefault(&pitem->localrelativefilename));
  netwib_er(netwib_buf_append_buf(plocalrelativefilename, &pitem->localrelativefilename));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_webspiderurl2_item_erase(netwib_ptr pitem)
{
  netwox_webspiderurl2_item *purlitem;

  purlitem = (netwox_webspiderurl2_item *)pitem;
  netwib_er(netwib_buf_close(&purlitem->localrelativefilename));

  netwib_er(netwib_ptr_free(&pitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_webspiderurl2_add(netwox_webspider *pwebspider,
                                    netwib_constbuf *purl,
                                    netwib_bool parsehtml,
                                    netwib_constbuf *plocalfilename)
{
  netwib_buf localrelativefilename;
  netwib_ptr pitem;

  netwib_er(netwib_buf_init_mallocdefault(&localrelativefilename));
  netwib_er(netwib_path_init_relb(&pwebspider->localrootdir, plocalfilename, &localrelativefilename));

  netwib_er(netwox_webspiderurl2_item_create(parsehtml,
                                             &localrelativefilename,
                                             &pitem));
  netwib_er(netwib_buf_close(&localrelativefilename));
  netwib_er(netwib_hash_add(pwebspider->phashdownloadedurls, purl,
                            pitem, NETWIB_FALSE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_webspiderurl2_downloaded(netwox_webspider *pwebspider,
                                           netwib_constbuf *purl,
                                           netwib_bool *pdownloaded,
                                           netwib_buf *plocalfilename) {
  netwox_webspiderurl2_item *purlitem;
  netwib_buf url, localfilename;
  netwib_data data, pc;
  netwib_uint32 datasize;
  netwib_bool exists;
  netwib_err ret;

  if (pdownloaded != NULL) *pdownloaded = NETWIB_FALSE;

  /* suppress the fragment */
  url = *purl;
  data = netwib__buf_ref_data_ptr(purl);
  datasize = netwib__buf_ref_data_size(purl);
  pc = netwib_c_memchr(data, '#', datasize);
  if (pc != NULL) {
    url.endoffset = url.beginoffset + pc - data;
  }

  ret = netwib_hash_value(pwebspider->phashdownloadedurls, &url,
                          (netwib_ptr*)&purlitem);
  if (ret != NETWIB_ERR_OK) {
    /* do not change pdownloaded value */
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_buf_init_mallocdefault(&localfilename));
  ret = netwib_path_init_concat(&pwebspider->localrootdir,
                                &purlitem->localrelativefilename,
                                &localfilename);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_filename_exists(&localfilename, &exists));
    if (exists) {
      if (pdownloaded != NULL) *pdownloaded = NETWIB_TRUE;
       ret = netwib_buf_append_buf(&localfilename, plocalfilename);
    }
  }
  netwib_er(netwib_buf_close(&localfilename));

  return(ret);
}
