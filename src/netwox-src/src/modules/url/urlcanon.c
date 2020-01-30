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
netwib_err netwox_urlcanon_path(netwib_constbuf *ppath,
                                netwib_buf *pcanonizedpath)
{
  netwib_data data;
  netwib_uint32 datasize;
  netwib_bool endwithslash;

  data = netwib__buf_ref_data_ptr(ppath);
  datasize = netwib__buf_ref_data_size(ppath);

  if (datasize == 0) {
    return(NETWIB_ERR_PAPATHNOTCANON);
  }

  endwithslash = NETWIB_FALSE;
  if (data[datasize-1] == '/') {
    endwithslash = NETWIB_TRUE;
  }

  netwib_er(netwib_path_canon(ppath, pcanonizedpath));

  if (endwithslash) {
    data = netwib__buf_ref_data_ptr(pcanonizedpath);
    datasize = netwib__buf_ref_data_size(pcanonizedpath);
    if (data[datasize-1] != '/') {
      netwib_er(netwib_buf_append_byte('/', pcanonizedpath));
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_urlcanon_query(netwib_bufpool *pbufpool,
                                 netwib_constbuf *pquery,
                                 netwib_buf *pcanonizedquery)
{
  netwib_buf query, *pname, *pvalue;
  netwib_bool addamp, valueset;
  netwib_err ret;

  query = *pquery;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pname));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pvalue));

  addamp = NETWIB_FALSE;
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    netwib__buf_reinit(pname);
    netwib__buf_reinit(pvalue);
    ret = netwox_url_queryvar_decode(&query, pname, &valueset, pvalue);
    if (ret == NETWIB_ERR_DATAEND) {
      ret = NETWIB_ERR_NOTFOUND;
      break;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    if (addamp) {
      netwib_er(netwib_buf_append_byte('&', pcanonizedquery));
    }
    ret = netwox_url_queryvar_encode(pname, valueset, pvalue,
                                     pcanonizedquery);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    addamp = NETWIB_TRUE;
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &pvalue));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pname));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
typedef struct {
  netwib_buf *ppath;
  netwib_buf *pfragment;
  netwib_buf *ptmp;
  netwib_buf *pcanonizedpath;
  netwib_buf *pserveruser;
  netwib_buf *pserverpassword;
  netwib_buf *pserverhost;
  netwib_buf *pcanonizedquery;
} netwox_urlcanon_urlst;

/*-------------------------------------------------------------*/
static netwib_err netwox_urlcanon_url_abs(netwib_bufpool *pbufpool,
                                          netwox_urlcanon_urlst *purlst,
                                          netwib_constbuf *purl,
                                          netwib_buf *pcanonizedurl)
{
  netwib_buf authority, query;
  netwox_url_scheme scheme;
  netwib_port serverport;

  netwib_er(netwox_url_decode(purl, &scheme, &authority, purlst->ppath,
                              &query, purlst->pfragment));
  netwib_er(netwox_url_authority_decode(scheme, &authority,
                                        purlst->pserveruser,
                                        purlst->pserverpassword,
                                        purlst->pserverhost, &serverport));
  netwib_er(netwox_urlcanon_path(purlst->ppath, purlst->pcanonizedpath));
  netwib_er(netwox_urlcanon_query(pbufpool, &query, purlst->pcanonizedquery));
  netwib_er(netwox_url_authority_encode(scheme, purlst->pserveruser,
                                        purlst->pserverpassword,
                                        purlst->pserverhost, serverport,
                                        purlst->ptmp));
  netwib_er(netwox_url_encode(scheme, purlst->ptmp, purlst->pcanonizedpath,
                              purlst->pcanonizedquery, purlst->pfragment,
                              pcanonizedurl));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_urlcanon_url_rel(netwib_bufpool *pbufpool,
                                          netwox_urlcanon_urlst *purlst,
                                          netwib_constbuf *purl,
                                          netwib_buf *pcanonizedurl)
{
  netwib_buf query;

  netwib_er(netwox_url_pqf_decode(purl, purlst->ppath, &query,
                                  purlst->pfragment));
  netwib_er(netwox_urlcanon_path(purlst->ppath, purlst->pcanonizedpath));
  netwib_er(netwox_urlcanon_query(pbufpool, &query, purlst->pcanonizedquery));
  netwib_er(netwox_url_pqf_encode(purlst->pcanonizedpath,
                                  purlst->pcanonizedquery, purlst->pfragment,
                                  pcanonizedurl));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_urlcanon_url(netwib_bufpool *pbufpool,
                               netwib_constbuf *purl,
                               netwib_buf *pcanonizedurl)
{
  netwox_urlcanon_urlst urlst;
  netwox_url_scheme scheme;
  netwib_err ret=NETWIB_ERR_OK;

  /* first, determine scheme */
  netwib_er(netwox_url_decode_scheme(purl, &scheme, NULL));
  if (scheme == NETWOX_URL_SCHEME_UNKNOWN) {
    return(NETWOX_ERR_NOTSUPPORTED);
  }

  /* initialize local variables */
  netwib_er(netwib_bufpool_buf_init(pbufpool, &urlst.ppath));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &urlst.pfragment));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &urlst.pcanonizedpath));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &urlst.pcanonizedquery));

  /* decode url */
  if (scheme == NETWOX_URL_SCHEME_NONE) {
    ret = netwox_urlcanon_url_rel(pbufpool, &urlst, purl, pcanonizedurl);
  } else {
    netwib_er(netwib_bufpool_buf_init(pbufpool, &urlst.ptmp));
    netwib_er(netwib_bufpool_buf_init(pbufpool, &urlst.pserveruser));
    netwib_er(netwib_bufpool_buf_init(pbufpool, &urlst.pserverpassword));
    netwib_er(netwib_bufpool_buf_init(pbufpool, &urlst.pserverhost));
    ret = netwox_urlcanon_url_abs(pbufpool, &urlst, purl, pcanonizedurl);
    netwib_er(netwib_bufpool_buf_close(pbufpool, &urlst.ptmp));
    netwib_er(netwib_bufpool_buf_close(pbufpool, &urlst.pserveruser));
    netwib_er(netwib_bufpool_buf_close(pbufpool, &urlst.pserverpassword));
    netwib_er(netwib_bufpool_buf_close(pbufpool, &urlst.pserverhost));
  }

  /* close */
  netwib_er(netwib_bufpool_buf_close(pbufpool, &urlst.ppath));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &urlst.pfragment));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &urlst.pcanonizedpath));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &urlst.pcanonizedquery));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_urlcanon_file_read(netwib_bufpool *pbufpool,
                                     netwib_constbuf *pfileurl,
                                     netwib_constbuf *pbasehref,
                                     netwib_constbuf *purl,
                                     netwib_buf *pcanonizedurl)
{
  netwib_buf *pcanonurl, *ppath, *ppath2;
  netwib_buf authority;
  netwox_url_scheme scheme;
  netwib_data data;
  netwib_uint32 datasize;
  netwib_err ret=NETWIB_ERR_OK;

  /* if url is absolute (http://...), directly return it */
  netwib_er(netwox_url_decode_scheme(purl, &scheme, NULL));
  if (scheme == NETWOX_URL_SCHEME_UNKNOWN) {
    return(NETWOX_ERR_NOTSUPPORTED);
  }
  if (scheme != NETWOX_URL_SCHEME_NONE) {
    netwib_er(netwox_urlcanon_url(pbufpool, purl, pcanonizedurl));
    return(NETWIB_ERR_OK);
  }

  /* canonize url found in the file */
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pcanonurl));
  ret = netwox_urlcanon_url(pbufpool, purl, pcanonurl);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_bufpool_buf_close(pbufpool, &pcanonurl));
    return(ret);
  }

  /* determine if use pfileurl or pbasehref, and decode it */
  netwib_er(netwib_bufpool_buf_init(pbufpool, &ppath));
  if (netwib__buf_ref_data_sizenull(pbasehref)) {
    ret = netwox_url_decode(pbasehref, &scheme, &authority, ppath, NULL, NULL);
  } else {
    ret = netwox_url_decode(pfileurl, &scheme, &authority, ppath, NULL, NULL);
  }
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_bufpool_buf_close(pbufpool, &ppath));
    netwib_er(netwib_bufpool_buf_close(pbufpool, &pcanonurl));
    return(ret);
  }
  if (scheme == NETWOX_URL_SCHEME_UNKNOWN ||
      scheme == NETWOX_URL_SCHEME_NONE) {
    ret = NETWOX_ERR_NOTSUPPORTED;
  }

  /* construct full url */
  netwib_er(netwox_url_encode_scheme(scheme, pcanonizedurl));
  netwib_er(netwib_buf_append_buf(&authority, pcanonizedurl));
  if (scheme == NETWOX_URL_SCHEME_FTP) {
    netwib_er(netwib_buf_append_byte('/', pcanonizedurl));
  }
  /* append path */
  data = netwib__buf_ref_data_ptr(pcanonurl);
  if (data[0] == '/') {
    netwib_er(netwib_buf_append_buf(pcanonurl, pcanonizedurl));
  } else {
    /* obtain father dir of ppath (end it with '/') */
    data = netwib__buf_ref_data_ptr(ppath);
    datasize = netwib__buf_ref_data_size(ppath) - 1;
    while (NETWIB_TRUE) {
      if (data[datasize] == '/') {
        ppath->endoffset = ppath->beginoffset + datasize + 1;
        break;
      }
      if (datasize == 0) {
        /* possible with FTP which does not start with '/' */
        netwib__buf_reinit(ppath);
        netwib_er(netwib_buf_append_string("./", ppath));
        break;
      }
      datasize--;
    }
    /* append pcanonurl */
    netwib_er(netwib_buf_append_buf(pcanonurl, ppath));
    /* construct */
    if (scheme == NETWOX_URL_SCHEME_FTP) {
      netwib_er(netwib_bufpool_buf_init(pbufpool, &ppath2));
      ret = netwox_urlcanon_url(pbufpool, ppath, ppath2);
      if (ret == NETWIB_ERR_OK) {
        data = netwib__buf_ref_data_ptr(ppath2);
        datasize = netwib__buf_ref_data_size(ppath2);
        if (datasize == 1) {
          if (data[0] == '.') {
            ppath2->beginoffset++;
          }
        } else if (datasize >= 2) {
          if (data[0] == '.' && data[1] == '/') {
            ppath2->beginoffset += 2;
          }
        }
        ret = netwox_url_part_encode(ppath2, NETWOX_URL_PARTTYPE_PATH,
                                     pcanonizedurl);
      }
      netwib_er(netwib_bufpool_buf_close(pbufpool, &ppath2));
    } else {
      ret = netwox_urlcanon_url(pbufpool, ppath, pcanonizedurl);
    }
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &ppath));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pcanonurl));
  return(ret);
}
