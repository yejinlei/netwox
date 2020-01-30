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
netwib_err netwox_webspider_init(netwox_webspider *pwebspider)
{
  netwox_rules_target defaulttarget;

  netwib_er(netwib_buf_init_mallocdefault(&pwebspider->localrootdir));
  netwib_er(netwib_buf_append_string("./spider", &pwebspider->localrootdir));

  netwib_er(netwox_httpclictx_init(&pwebspider->httpclictx));

  netwib_er(netwox_bufstore_ring_init(&pwebspider->pliststarturl));
  netwib_er(netwox_webspiderurl1_hash_init(&pwebspider->phashurlstodownload));
  netwib_er(netwox_webspiderurl2_hash_init(&pwebspider->phashdownloadedurls));
  netwib_er(netwib_hash_init(NULL, NULL, &pwebspider->phashurlsinprogress));

  pwebspider->maxrecursion = 0;
  pwebspider->maxthreads = 5;
  pwebspider->millisleep = 0;

  pwebspider->aclurlset = NETWIB_FALSE;
  defaulttarget.targettype = NETWOX_RULES_TARGETTYPE_ACCEPT;
  netwib_er(netwox_rules_init(&netwox_rulesre_ruleinfos_match,
                              &netwox_rulesre_ruleinfos_erase,
                              &netwox_rulesre_ruleinfos_display,
                              &defaulttarget, NETWIB_FALSE,
                              &pwebspider->paclurl));

  pwebspider->getimage = NETWIB_TRUE;
  pwebspider->getmultimedia = NETWIB_TRUE;
  pwebspider->getcode = NETWIB_TRUE;
  pwebspider->linkconv = NETWOX_WEBSPIDER_LINKCONV_WEB;
  pwebspider->loglevel = NETWOX_WEBSPIDER_LOGLEVEL_NONE;

  netwib_er(netwox_mimetypes_init(&pwebspider->mimetypes));
  pwebspider->mimetypeswarn = NETWIB_TRUE;
  netwib_er(netwox_bufarray_init(&pwebspider->deletedextensions));

  pwebspider->numtotalurls = 0;
  netwib__uint64_init_uint32(0, pwebspider->numdownloadedurls);
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_webspider_close(netwox_webspider *pwebspider)
{

  netwib_er(netwib_buf_close(&pwebspider->localrootdir));

  netwib_er(netwox_httpclictx_close(&pwebspider->httpclictx));

  netwib_er(netwib_hash_close(&pwebspider->phashurlsinprogress, NETWIB_FALSE));
  netwib_er(netwox_webspiderurl2_hash_close(&pwebspider->phashdownloadedurls));
  netwib_er(netwox_webspiderurl1_hash_close(&pwebspider->phashurlstodownload));
  netwib_er(netwox_bufstore_ring_close(&pwebspider->pliststarturl));

  netwib_er(netwox_rules_close(&pwebspider->paclurl));

  netwib_er(netwox_mimetypes_close(&pwebspider->mimetypes));
  netwib_er(netwox_bufarray_close(&pwebspider->deletedextensions));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_webspider_add(netwox_webspider *pwebspider,
                                netwib_constbuf *purl)
{
  netwib_ptr pitem;

  netwib_er(netwox_webspiderurl1_add(pwebspider, purl, 1,
                                     NETWOX_HTMLTAG_ELMTATTR_A_HREF));

  netwib_er(netwox_bufstore_create(purl, &pitem));
  netwib_er(netwib_ring_add_last(pwebspider->pliststarturl, pitem));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_webspider_add_canon(netwox_webspider *pwebspider,
                                      netwib_constbuf *purl)
{
  netwib_buf *pcanonizedurl;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pwebspider->httpclictx.pbufpool,
                                    &pcanonizedurl));
  ret = netwox_urlcanon_url(pwebspider->httpclictx.pbufpool, purl,
                            pcanonizedurl);
  if (ret == NETWIB_ERR_OK) {
    ret = netwox_webspider_add(pwebspider, pcanonizedurl);
  }
  netwib_er(netwib_bufpool_buf_close(pwebspider->httpclictx.pbufpool,
                                     &pcanonizedurl));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_webspider_urllist_add(netwox_webspider *pwebspider,
                                                    netwib_ring *pring,
                                                    netwib_uint32 recursionlevel)
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
    if (pitem->elmtattr != NETWOX_HTMLTAG_ELMTATTR_BASE_HREF) {
      netwib_er(netwox_webspiderurl1_add(pwebspider, &pitem->value,
                                         recursionlevel, pitem->elmtattr));
    }
  }

  netwib_er(netwib_ring_index_close(&pringindex));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_webspider_run(netwox_webspider *pwebspider,
                                            netwib_constbuf *purl,
                                            netwib_constbuf *pcontenttype,
                                            netwib_uint32 recursionlevel,
                                            netwox_htmltag_elmtattr elmtattr,
                                            netwib_constbuf *pfilename,
                                 netwox_httphdr_statuscode statuscode)
{
  netwib_bufpool *pbufpool = pwebspider->httpclictx.pbufpool;
  netwib_buf *plocalfilename;
  netwox_htmltag_elmtattr_uricontain uricontain;
  netwib_ring *pring;
  netwib_bool iserror, containshtml, parsehtml;

  /*netwib_er(netwib_fmt_display("Saved %{buf} [%{uint32};%{uint32}] in %{buf} (status %{uint32})\n", purl, recursionlevel, elmtattr, pfilename,statuscode));*/

  /* obtain local filename */
  iserror = NETWIB_FALSE;
  if (statuscode != NETWOX_HTTPHDR_STATUSCODE_OK) {
    iserror = NETWIB_TRUE;
  }
  netwib_er(netwib_bufpool_buf_init(pbufpool, &plocalfilename));
  netwib__uint64_inc(pwebspider->numdownloadedurls);
  netwib_er(netwox_urllocal_encode(&pwebspider->localrootdir, purl, iserror,
                                   &pwebspider->mimetypes,
                                   pwebspider->mimetypeswarn,
                                   &pwebspider->deletedextensions,
                                   pcontenttype,
                                   pwebspider->numdownloadedurls,
                                   plocalfilename, &containshtml));
  parsehtml = containshtml;
  if (!iserror && containshtml) {
    netwib_er(netwox_htmltag_uricontain_init_elmtattr(elmtattr, &uricontain));
    if (uricontain != NETWOX_HTMLTAG_ELMTATTR_URICONTAIN_HTML) {
      parsehtml = NETWIB_FALSE;
    }
  }

  /* obtain urls of the file */
  if (parsehtml) {
    netwib_er(netwox_htmlfile_urllist_ring_init(&pring));
    netwib_er(netwox_htmlfile_urllist_file_read(pfilename, NETWIB_FALSE,
                                                pring));
    netwib_er(netwox_htmlfile_urllist_canon(pbufpool, purl, NETWIB_TRUE,
                                            pring));
    /*netwib_er(netwox_htmlfile_urllist_ring_display(pring));*/
    netwib_er(netwox_priv_webspider_urllist_add(pwebspider, pring,
                                                recursionlevel+1));
    netwib_er(netwox_htmlfile_urllist_ring_close(&pring));
  }

  /* rename file */
  netwib_er(netwib_filename_rename(pfilename, plocalfilename));
  if (pwebspider->loglevel == NETWOX_WEBSPIDER_LOGLEVEL_NORMAL) {
    netwib_er(netwib_fmt_display("STORE %{buf}\n", plocalfilename));
  }

  /* add in downloaded hash */
  netwib_er(netwox_webspiderurl2_add(pwebspider, purl, parsehtml,
                                     plocalfilename));

  netwib_er(netwib_bufpool_buf_close(pbufpool, &plocalfilename));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_webspider_run(netwox_webspider *pwebspider)
{
  netwib_err ret;

  ret = netwox_webspidermt_run(pwebspider, &netwox_priv_webspider_run);

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_webspider_urllist_relink(netwox_webspider *pwebspider,
                                                    netwib_constbuf *pfilename,
                                                       netwib_ring *pring)
{
  netwib_bufpool *pbufpool = pwebspider->httpclictx.pbufpool;
  netwib_ring_index *pringindex;
  netwox_htmlfile_urllist_item *pitem;
  netwib_buf *purlfilename, *pdirname, fragment;
  netwib_data data, pc;
  netwib_uint32 datasize;
  netwib_bool downloaded;
  netwib_err ret;

  netwib_er(netwib_ring_index_init(pring, &pringindex));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &purlfilename));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pdirname));

  netwib_er(netwib_path_decode_parent(pfilename, pdirname));

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
      /* check if url was downloaded */
      netwib__buf_reinit(purlfilename);
      netwib_er(netwox_webspiderurl2_downloaded(pwebspider, &pitem->value,
                                                &downloaded, purlfilename));
      if (!downloaded) {
        /* if not downloaded, add absolute url */
        netwib_er(netwib_buf_append_buf(&pitem->value,
                                        &pitem->replacingvalue));
      } else {
        /* if downloaded, add relative path to local file */
        netwib_er(netwib_path_init_relb(pdirname, purlfilename,
                                        &pitem->replacingvalue));
        /* now, add fragment */
        data = netwib__buf_ref_data_ptr(&pitem->value);
        datasize = netwib__buf_ref_data_size(&pitem->value);
        pc = netwib_c_memchr(data, '#', datasize);
        if (pc != NULL) {
          fragment = pitem->value;
          fragment.beginoffset += pc - data;
          netwib_er(netwib_buf_append_buf(&fragment, &pitem->replacingvalue));
        }
      }
    }
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &pdirname));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &purlfilename));
  netwib_er(netwib_ring_index_close(&pringindex));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_webspider_relink(netwox_webspider *pwebspider,
                                               netwib_constbuf *purl,
                                               netwib_constbuf *plocalrelativefilename)
{
  netwib_bufpool *pbufpool = pwebspider->httpclictx.pbufpool;
  netwib_ring *pring;
  netwib_buf *purlfilename, *ptmpfilename, tmprelink;

  /* obtain real filename */
  netwib_er(netwib_bufpool_buf_init(pbufpool, &purlfilename));
  netwib_er(netwib_path_init_concat(&pwebspider->localrootdir, plocalrelativefilename, purlfilename));

  /* create temporary name */
  netwib_er(netwib_bufpool_buf_init(pbufpool, &ptmpfilename));
  netwib_er(netwib_buf_init_ext_string("tmp/relink", &tmprelink));
  netwib_er(netwib_path_init_concat(&pwebspider->localrootdir, &tmprelink,
                                    ptmpfilename));

  /* obtain urls of the file */
  netwib_er(netwox_htmlfile_urllist_ring_init(&pring));
  netwib_er(netwox_htmlfile_urllist_file_read(purlfilename, NETWIB_TRUE,
                                              pring));
  netwib_er(netwox_htmlfile_urllist_canon(pbufpool, purl, NETWIB_TRUE, pring));
  /*netwib_er(netwox_htmlfile_urllist_ring_display(pring));*/
  netwib_er(netwox_priv_webspider_urllist_relink(pwebspider, purlfilename,
                                                 pring));
  /*netwib_er(netwox_htmlfile_urllist_ring_display(pring));*/

  /* recreate file */
  netwib_er(netwox_htmlfile_urllist_file_write(purlfilename, pring,
                                               ptmpfilename));
  netwib_er(netwib_filename_rename(ptmpfilename, purlfilename));

  /* close */
  netwib_er(netwox_htmlfile_urllist_ring_close(&pring));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &ptmpfilename));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &purlfilename));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_webspider_relink(netwox_webspider *pwebspider)
{
  netwib_bufpool *pbufpool = pwebspider->httpclictx.pbufpool;
  netwib_hash_index *phashindex;
  netwox_webspiderurl2_item *pitem;
  netwib_buf *pkey;
  netwox_scale scale;
  netwox_fraction fraction;
  netwib_uint32 numdots, count;
  netwib_err ret;

  if (pwebspider->linkconv == NETWOX_WEBSPIDER_LINKCONV_NONE) {
    /* not needed */
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_hash_index_init(pwebspider->phashdownloadedurls, &phashindex));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pkey));
  netwib_er(netwox_scale_init(pwebspider->numtotalurls, &scale));
  netwib_er(netwox_fraction_init(&fraction));

  ret = NETWIB_ERR_OK;
  numdots = 0;
  count = 0;
  while(NETWIB_TRUE) {
    /* obtain entry */
    netwib__buf_reinit(pkey);
    ret = netwib_hash_index_next(phashindex, pkey, (netwib_ptr*)&pitem);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    if (pitem->parsehtml) {
      /* log */
      if (pwebspider->loglevel == NETWOX_WEBSPIDER_LOGLEVEL_NORMAL) {
        netwib_er(netwib_fmt_display("RELINK %{buf}\n", pkey));
      } else if (pwebspider->loglevel == NETWOX_WEBSPIDER_LOGLEVEL_DOT) {
        netwib_er(netwib_fmt_display("."));
        if (numdots++ == 70) {
          netwib_er(netwib_fmt_display("\n"));
          numdots = 0;
        }
      } else if (pwebspider->loglevel == NETWOX_WEBSPIDER_LOGLEVEL_THREAD) {
        netwib_er(netwib_fmt_display("1"));
        if (numdots++ == 70) {
          netwib_er(netwib_fmt_display("\n"));
          numdots = 0;
        }
      } else if (pwebspider->loglevel == NETWOX_WEBSPIDER_LOGLEVEL_SCALE) {
        netwib_er(netwox_scale_update(&scale, count++));
      } else if (pwebspider->loglevel == NETWOX_WEBSPIDER_LOGLEVEL_FRACTION) {
        netwib_er(netwox_fraction_update(&fraction, count++, pwebspider->numtotalurls));
      }
      /* relink */
      netwib_er(netwox_priv_webspider_relink(pwebspider, pkey, &pitem->localrelativefilename));
    }
  }

  netwib_er(netwox_fraction_close(&fraction));
  netwib_er(netwox_scale_close(&scale));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pkey));
  netwib_er(netwib_hash_index_close(&phashindex));

  if (pwebspider->loglevel == NETWOX_WEBSPIDER_LOGLEVEL_DOT ||
      pwebspider->loglevel == NETWOX_WEBSPIDER_LOGLEVEL_THREAD) {
    if (numdots != 0) {
      netwib_er(netwib_fmt_display("\n"));
    }
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_webspider_index(netwox_webspider *pwebspider)
{
  netwib_bufpool *pbufpool = pwebspider->httpclictx.pbufpool;
  netwib_ring_index *pringindex;
  netwib_buf url, *pindexfilename, *purlfilename, *pmsg, msg, indexhtml;
  netwib_ptr *pitem;
  netwib_io *pio;
  netwib_bool downloaded;
  netwib_err ret;

  netwib_er(netwib_ring_index_init(pwebspider->pliststarturl, &pringindex));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &purlfilename));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pindexfilename));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pmsg));

  netwib_er(netwib_buf_init_ext_string("index.html", &indexhtml));
  netwib_er(netwib_path_init_concat(&pwebspider->localrootdir, &indexhtml,
                                    pindexfilename));
  netwib_er(netwib_io_init_file_textwrite(pindexfilename, &pio));

  netwib_er(netwib_buf_init_ext_string("<HTML><BODY> downloaded urls :<BR>\n",
                                     &msg));
  netwib_er(netwib_io_write(pio, &msg));

  ret = NETWIB_ERR_OK;
  while(NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pitem);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    /* check if url was downloaded */
    netwib__buf_reinit(purlfilename);
    netwib_er(netwox_bufstore_decode(pitem, &url));
    netwib_er(netwox_webspiderurl2_downloaded(pwebspider, &url,
                                              &downloaded, purlfilename));
    if (downloaded) {
      netwib__buf_reinit(pmsg);
      netwib_er(netwib_buf_append_string("&nbsp;<A HREF=\"", pmsg));
      netwib_er(netwib_path_init_relb(&pwebspider->localrootdir,
                                      purlfilename, pmsg));
      netwib_er(netwib_buf_append_string("\">", pmsg));
      netwib_er(netwib_buf_append_buf(&url, pmsg));
      netwib_er(netwib_buf_append_string("</A><BR>\n", pmsg));
      netwib_er(netwib_io_write(pio, pmsg));
    }
  }

  netwib_er(netwib_buf_init_ext_string("</BODY></HTML>\n",
                                     &msg));
  netwib_er(netwib_io_write(pio, &msg));

  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pmsg));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pindexfilename));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &purlfilename));
  netwib_er(netwib_ring_index_close(&pringindex));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_webspider_createurllist(netwox_webspider *pwebspider) {
  netwib_bufpool *pbufpool = pwebspider->httpclictx.pbufpool;
  netwib_hash_index *phashindex;
  netwox_webspiderurl2_item *pitem;
  netwib_buf *purl, *purllistfilename, *pdata, urllist;
  netwib_io *pio;
  netwib_bool ioopen;
  netwib_err ret;

  /* initialize */
  ret = NETWIB_ERR_OK;
  ioopen = NETWIB_FALSE;
  netwib_er(netwib_hash_index_init(pwebspider->phashdownloadedurls, &phashindex));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &purl));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &purllistfilename));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pdata));

  /* open file */
  netwib_eg(netwib_buf_init_ext_string("urllist.txt", &urllist));
  netwib_eg(netwib_path_init_concat(&pwebspider->localrootdir, &urllist,
                                    purllistfilename));
  netwib_eg(netwib_io_init_file_textwrite(purllistfilename, &pio));
  ioopen = NETWIB_TRUE;

  /* write */
  while(NETWIB_TRUE) {
    /* obtain entry */
    netwib__buf_reinit(purl);
    ret = netwib_hash_index_next(phashindex, purl, (netwib_ptr*)&pitem);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    /* write */
    netwib_eg(netwib_buf_append_buf(purl, pdata));
    netwib_eg(netwib_buf_append_byte('\t', pdata));
    netwib_eg(netwib_buf_append_buf(&pitem->localrelativefilename, pdata));
    netwib_eg(netwib_buf_append_string("\n", pdata));
    if (netwib__buf_ref_data_size(pdata) > 32000) {
      netwib_eg(netwib_io_write(pio, pdata));
      netwib__buf_reinit(pdata);
    }
  }
  netwib_eg(netwib_io_write(pio, pdata));

  /* end */
 netwib_gotolabel:
  if (ioopen) {
    netwib_er(netwib_io_close(&pio));
  }
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pdata));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &purllistfilename));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &purl));
  netwib_er(netwib_hash_index_close(&phashindex));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_webspider_deltmp(netwox_webspider *pwebspider) {
  netwib_bufpool *pbufpool = pwebspider->httpclictx.pbufpool;
  netwib_buf tmpdir, *ptmpdir;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &ptmpdir));

  netwib_er(netwib_buf_init_ext_string("tmp", &tmpdir));
  netwib_er(netwib_path_init_concat(&pwebspider->localrootdir, &tmpdir,
                                    ptmpdir));

  /*ignore*/netwib_dirname_remove(ptmpdir);

  netwib_er(netwib_bufpool_buf_close(pbufpool, &ptmpdir));
  return(NETWIB_ERR_OK);
}
