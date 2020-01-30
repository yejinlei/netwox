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
static netwib_err netwox_url_download_http(netwox_httpclictx *pctx,
                                           netwib_constbuf *purl,
                                           netwib_bool downloadfile,
                                           netwib_io *pfileio,
                                           netwib_bool obtainsize,
                                           netwib_uint32 *psize,
                                        netwox_httphdr_statuscode *pstatuscode,
                                           netwib_buf *pcontenttype)
{
  netwox_httphdr_statuscode statuscode;
  netwox_httpcliresphdrs resphdrs;
  netwox_httphdr_method method;
  netwib_buf *pmsg;
  netwib_err ret;

  method = NETWOX_HTTPHDR_METHOD_GET;
  if (obtainsize) {
    method = NETWOX_HTTPHDR_METHOD_HEAD;
  }
  netwib_er(netwox_httpcliresphdrs_init(&resphdrs));
  ret = netwox_httpcli_method(method, pctx, purl,
                              &resphdrs, &statuscode, NULL, pfileio);
  if (ret == NETWIB_ERR_OK) {
    if (resphdrs.contenttypeset) {
      netwib_er(netwib_buf_append_buf(&resphdrs.contenttype, pcontenttype));
    }
    if (downloadfile) {
      if (resphdrs.locationset) {
        if (statuscode == NETWOX_HTTPHDR_STATUSCODE_MOVEDPERMANENTLY ||
            statuscode == NETWOX_HTTPHDR_STATUSCODE_MOVEDTEMPORARILY) {
          netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pmsg));
          netwib_er(netwib_buf_append_fmt(pmsg, "</SCRIPT></STYLE><BR>Added by the spider : <A HREF=\"%{buf}\">new location</A>.</BODY></HTML>\n", &resphdrs.location));
          netwib_er(netwib_io_write(pfileio, pmsg));
          netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pmsg));
        }
      }
      if (pstatuscode != NULL) *pstatuscode = statuscode;
    }
    if (obtainsize) {
      if (statuscode == NETWOX_HTTPHDR_STATUSCODE_OK) {
        if (resphdrs.contentlengthset) {
          if (psize != NULL) *psize = resphdrs.contentlength;
          if (pstatuscode != NULL) *pstatuscode = NETWOX_HTTPHDR_STATUSCODE_OK;
        }
      }
    }
  }
  netwib_er(netwox_httpcliresphdrs_close(&resphdrs));

  return(ret);
}

/*-------------------------------------------------------------*/
/* should return OK or NOTFOUND */
static netwib_err netwox_url_download_ftp3_dir(netwox_httpclictx *pctx,
                                               netwox_ftpclient *pftpclient,
                                               netwib_constbuf *purl,
                                               netwib_constbuf *ppath,
                                               netwib_bool isdir,
                                               netwib_io *pfileio,
                                               netwib_buf *pcontenttype)
{
  netwib_ring *pring;
  netwib_ring_index *pringindex;
  netwox_ftp_dirlist *pdirlist;
  netwib_ptr pitem;
  netwib_buf *pmsg, dot;
  netwib_err ret;

  ret = netwox_ftpclient_dir_cd(pftpclient, ppath);
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  netwib_er(netwib_buf_append_string(NETWOX_MIMETYPE_HTML, pcontenttype));

  /* as for HTTP, we create a redirect page */
  if (!isdir) {
    netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pmsg));
    netwib_er(netwib_buf_append_string("<HTML><BODY><PRE>\n", pmsg));
    netwib_er(netwib_buf_append_string("This directory is at <A HREF=\"", pmsg));
    netwib_er(netwox_html_data_encode(purl, NETWIB_FALSE, pmsg));
    netwib_er(netwib_buf_append_byte('/', pmsg));
    netwib_er(netwib_buf_append_string("\">", pmsg));
    netwib_er(netwox_html_data_encode(purl, NETWIB_FALSE, pmsg));
    netwib_er(netwib_buf_append_byte('/', pmsg));
    netwib_er(netwib_buf_append_string("</A>.", pmsg));
    netwib_er(netwib_buf_append_string("</PRE></BODY></HTML>\n", pmsg));
    ret = netwib_io_write(pfileio, pmsg);
    netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pmsg));
    return(ret);
  }

  netwib_er(netwox_ftp_dirlist_ring_init(&pring));
  netwib_er(netwib_buf_init_ext_string(".", &dot));
  ret = netwox_ftpclient_dir_ls(pftpclient, &dot, pring);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_ftp_dirlist_ring_close(&pring));
    return(ret);
  }

  netwib_er(netwib_ring_index_init(pring, &pringindex));

  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pmsg));
  netwib_er(netwib_buf_append_string("<HTML><BODY><PRE>\n", pmsg));
  while(NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, &pitem);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    pdirlist = (netwox_ftp_dirlist *)pitem;
    if (pdirlist->pathstat.type == NETWIB_PATHSTAT_TYPE_DIR) {
      netwib_er(netwib_buf_append_byte('/', &pdirlist->name));
    }
    netwib_er(netwib_buf_append_string("<A HREF=\"", pmsg));
    netwib_er(netwox_html_data_encode(&pdirlist->name, NETWIB_FALSE, pmsg));
    netwib_er(netwib_buf_append_string("\">", pmsg));
    netwib_er(netwox_html_data_encode(&pdirlist->name, NETWIB_FALSE, pmsg));
    netwib_er(netwib_buf_append_string("</A>", pmsg));
    switch(pdirlist->pathstat.type) {
    case NETWIB_PATHSTAT_TYPE_REG :
      netwib_er(netwib_buf_append_fmt(pmsg, " (file of size %{uint32})",
                                      pdirlist->pathstat.size));
      break;
    case NETWIB_PATHSTAT_TYPE_DIR :
      netwib_er(netwib_buf_append_fmt(pmsg, " (dir)"));
      break;
    case NETWIB_PATHSTAT_TYPE_LINK :
      netwib_er(netwib_buf_append_string(" (link to <A HREF=\"", pmsg));
      netwib_er(netwox_html_data_encode(&pdirlist->linkname, NETWIB_FALSE,
                                        pmsg));
      netwib_er(netwib_buf_append_string("\">", pmsg));
      netwib_er(netwox_html_data_encode(&pdirlist->linkname, NETWIB_FALSE,
                                        pmsg));
      netwib_er(netwib_buf_append_string("</A>)", pmsg));
      break;
    default :
      break;
    }
    netwib_er(netwib_buf_append_string("\n", pmsg));
  }
  netwib_er(netwib_buf_append_string("</PRE></BODY></HTML>\n", pmsg));
  ret = netwib_io_write(pfileio, pmsg);
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pmsg));

  netwib_er(netwib_ring_index_close(&pringindex));

  netwib_er(netwox_ftp_dirlist_ring_close(&pring));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_url_download_ftp2(netwox_httpclictx *pctx,
                                           netwib_constbuf *purl,
                                           netwib_constip *pip,
                                           netwib_port port,
                                           netwib_constbuf *puser,
                                           netwib_constbuf *ppassword,
                                           netwib_constbuf *ppath,
                                           netwib_bool downloadfile,
                                           netwib_io *pfileio,
                                           netwib_bool obtainsize,
                                           netwib_uint32 *psize,
                                      netwox_httphdr_statuscode *pstatuscode,
                                      netwib_buf *pcontenttype)
{
  netwox_ftpclient ftpclient;
  netwox_sockinfo sockinfo;
  netwib_data data;
  netwib_uint32 datasize;
  netwib_buf *pmsg;
  netwib_bool isdir;
  netwib_err ret;

  netwib_er(netwox_sockinfo_init_values_real_tcp_cli_easy(pip, port,
                                                          &sockinfo));
  ret = netwox_ftpclient_init(&sockinfo, pctx->timeoutms, &ftpclient);
  netwib_er(netwox_sockinfo_close(&sockinfo));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  if (netwib__buf_ref_data_size(puser)) {
    ret = netwox_ftpclient_login(&ftpclient, puser, ppassword);
  } else {
    ret = netwox_ftpclient_login_anonymous(&ftpclient);
  }
  if (ret != NETWIB_ERR_OK) {
    *pstatuscode = NETWOX_HTTPHDR_STATUSCODE_UNAUTHORIZED;
  }

  if (ret == NETWIB_ERR_OK) {
    ret = netwox_ftpclient_bin(&ftpclient);
  }

  if (ret == NETWIB_ERR_OK) {
    if (pctx->ftp_passive) {
      ret = netwox_ftpclient_passive(&ftpclient);
    } else {
      ret = netwox_ftpclient_active(&ftpclient);
    }
  }

  if (ret == NETWIB_ERR_OK) {
    /* check if directory (last is '/') */
    isdir = NETWIB_FALSE;
    data = netwib__buf_ref_data_ptr(ppath);
    datasize = netwib__buf_ref_data_size(ppath);
    if (datasize) {
      if (data[datasize-1] == '/') {
        isdir = NETWIB_TRUE;
      }
    }
    if (downloadfile) {
      /* first, try file then directory */
      if (!isdir) {
        ret = netwox_ftpclient_file_get_io(&ftpclient, ppath, pfileio);
      }
      if (isdir || ret != NETWIB_ERR_OK) {
        ret = netwox_url_download_ftp3_dir(pctx, &ftpclient, purl, ppath,
                                           isdir, pfileio, pcontenttype);
      }
    }
    if (obtainsize) {
      if (isdir) {
        ret = NETWIB_ERR_PAINVALIDTYPE;
      } else {
        ret = netwox_ftpclient_file_size(&ftpclient, ppath, psize);
      }
    }
    if (ret == NETWIB_ERR_OK) {
      *pstatuscode = NETWOX_HTTPHDR_STATUSCODE_OK;
    } else if (ret == NETWIB_ERR_NOTFOUND) {
      *pstatuscode = NETWOX_HTTPHDR_STATUSCODE_NOTFOUND;
      if (downloadfile) {
        netwib_er(netwib_buf_append_string(NETWOX_MIMETYPE_HTML, pcontenttype));
        netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pmsg));
        netwib_er(netwib_buf_append_fmt(pmsg, "<HTML><BODY>Error : FTP url '%{buf}' not found</BODY></HTML>\n", purl));
        netwib_er(netwib_io_write(pfileio, pmsg));
        netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pmsg));
      }
      ret = NETWIB_ERR_OK;
    }
  }

  netwib_er(netwox_ftpclient_close(&ftpclient));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_url_download_ftp(netwox_httpclictx *pctx,
                                          netwib_constbuf *purl,
                                          netwib_bool downloadfile,
                                          netwib_io *pfileio,
                                          netwib_bool obtainsize,
                                          netwib_uint32 *psize,
                                    netwox_httphdr_statuscode *pstatuscode,
                                      netwib_buf *pcontenttype)
{
  netwox_httphdr_statuscode statuscode=0;
  netwib_buf authority, *pserveruser, *pserverpassword, *pserverhost, *ppath;
  netwib_buf *pmsg;
  netwib_ip serverip;
  netwib_port serverport=0;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pserveruser));
  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pserverpassword));
  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pserverhost));
  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &ppath));

  ret = netwox_url_decode_ftp(purl, &authority, ppath);
  if (ret == NETWIB_ERR_OK) {
    ret = netwox_url_authority_decode(NETWOX_URL_SCHEME_FTP,
                                      &authority, pserveruser,
                                      pserverpassword, pserverhost,
                                      &serverport);
    if (ret == NETWIB_ERR_OK) {
      ret = netwox_dnscache_ip(&pctx->dnscache, pserverhost, &serverip);
    }
  }
  if (ret != NETWIB_ERR_OK) {
    if (downloadfile) {
      netwib_er(netwib_buf_append_string(NETWOX_MIMETYPE_HTML, pcontenttype));
      netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pmsg));
      netwib_er(netwib_buf_append_fmt(pmsg, "<HTML><BODY>Error : FTP url '%{buf}' cannot be decoded</BODY></HTML>\n", purl));
      netwib_er(netwib_io_write(pfileio, pmsg));
      netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pmsg));
    }
    ret = NETWIB_ERR_OK;
  } else {
    ret = netwox_url_download_ftp2(pctx, purl, &serverip, serverport,
                                   pserveruser, pserverpassword,
                                   ppath, downloadfile, pfileio,
                                   obtainsize, psize, &statuscode,
                                   pcontenttype);
    if (ret == NETWIB_ERR_OK) {
      if (pstatuscode != NULL) {
        *pstatuscode = statuscode;
      }
    }
  }

  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pserveruser));
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pserverpassword));
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pserverhost));
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &ppath));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_url_download(netwox_httpclictx *pctx,
                                      netwib_constbuf *purl,
                                      netwib_bool downloadfile,
                                      netwib_io *pfileio,
                                      netwib_bool obtainsize,
                                      netwib_uint32 *psize,
                                      netwox_httphdr_statuscode *pstatuscode,
                                      netwib_buf *pcontenttype) {
  netwib_buf *pcanonurl, *pmsg;
  netwox_url_scheme scheme;
  netwib_err ret;

  /* init */
  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pcanonurl));
  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pmsg));
  if (pstatuscode != NULL) {
    *pstatuscode = NETWOX_HTTPHDR_STATUSCODE_SERVICEUNAVAILABLE;
  }

  /* check url */
  ret = netwox_urlcanon_url(pctx->pbufpool, purl, pcanonurl);
  if (ret != NETWIB_ERR_OK) {
    if (downloadfile) {
      netwib_er(netwib_buf_append_fmt(pmsg, "<HTML><BODY>Error : url '%{buf}' cannot be decoded</BODY></HTML>\n", purl));
      netwib_er(netwib_io_write(pfileio, pmsg));
      netwib_er(netwib_buf_append_string(NETWOX_MIMETYPE_HTML, pcontenttype));
    }
  } else {
    if (pctx->useproxy) {
      ret = netwox_url_download_http(pctx, pcanonurl, downloadfile, pfileio,
                                     obtainsize, psize, pstatuscode,
                                     pcontenttype);
    } else {
      netwib_er(netwox_url_decode_scheme(pcanonurl, &scheme, NULL));
      switch (scheme) {
      case NETWOX_URL_SCHEME_HTTP :
        ret = netwox_url_download_http(pctx, pcanonurl, downloadfile, pfileio,
                                       obtainsize, psize, pstatuscode,
                                       pcontenttype);
        break;
      case NETWOX_URL_SCHEME_FTP :
        ret = netwox_url_download_ftp(pctx, pcanonurl, downloadfile, pfileio,
                                      obtainsize, psize, pstatuscode,
                                      pcontenttype);
        break;
      default :
        if (downloadfile) {
          netwib_er(netwib_buf_append_fmt(pmsg, "<HTML><BODY>Error : scheme of url '%{buf}' is not supported (should be http or ftp)</BODY></HTML>\n", purl));
          netwib_er(netwib_io_write(pfileio, pmsg));
          netwib_er(netwib_buf_append_string(NETWOX_MIMETYPE_HTML, pcontenttype));
        }
        ret = NETWIB_ERR_OK;
        break;
      }
    }
    if (ret != NETWIB_ERR_OK) {
      if (downloadfile) {
        netwib_er(netwib_buf_append_fmt(pmsg, "<HTML><BODY>Error : downloading url '%{buf}' returned error %{uint32} (", pcanonurl, ret));
        netwib_er(netwox_buf_append_err(ret, NETWIB_ERR_ENCODETYPE_TEXT,
                                        pmsg));
        netwib_er(netwib_buf_append_string(")</BODY></HTML>\n", pmsg));
        netwib_er(netwib_io_write(pfileio, pmsg));
        netwib_er(netwib_buf_append_string(NETWOX_MIMETYPE_HTML, pcontenttype));
      }
    }
  }

  /* close */
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pmsg));
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pcanonurl));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_url_download_file_ct(netwox_httpclictx *pctx,
                                       netwib_constbuf *purl,
                                       netwib_constbuf *pfilename,
                                       netwox_httphdr_statuscode *pstatuscode,
                                       netwib_buf *pcontenttype)
{
  netwib_io *pfileio;
  netwib_err ret;

  netwib_er(netwib_io_init_file_write(pfilename, &pfileio));
  ret = netwox_url_download(pctx, purl, NETWIB_TRUE, pfileio,
                            NETWIB_FALSE, NULL, pstatuscode, pcontenttype);
  netwib_er(netwib_io_close(&pfileio));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_url_download_io_ct(netwox_httpclictx *pctx,
                                     netwib_constbuf *purl,
                                     netwib_io *pfileio,
                                     netwox_httphdr_statuscode *pstatuscode,
                                     netwib_buf *pcontenttype)
{
  netwib_er(netwox_url_download(pctx, purl, NETWIB_TRUE, pfileio,
                                NETWIB_FALSE, NULL, pstatuscode, pcontenttype));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_url_download_size(netwox_httpclictx *pctx,
                                    netwib_constbuf *purl,
                                    netwib_uint32 *psize)
{
  netwox_httphdr_statuscode statuscode;

  netwib_er(netwox_url_download(pctx, purl, NETWIB_FALSE, NULL,
                                NETWIB_TRUE, psize, &statuscode, NULL));
  if (statuscode != NETWOX_HTTPHDR_STATUSCODE_OK) {
    /* the true error is in statuscode */
    return(NETWIB_ERR_NOTFOUND);
  }

  return(NETWIB_ERR_OK);
}
