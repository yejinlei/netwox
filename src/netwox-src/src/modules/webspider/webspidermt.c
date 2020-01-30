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
/* information between manager and thread : initialized by manager
   and freed by manager after thread execution */
typedef struct {
  netwib_uint32 threadid;
  netwox_httpclictx *pctx;
  netwib_buf url;
  netwib_buf contenttype;
  netwib_uint32 recursionlevel;
  netwox_htmltag_elmtattr elmtattr;
  netwib_buf filename;
  netwox_httphdr_statuscode statuscode;
} netwox_webspidermt_info;

static netwib_err netwox_webspidermt_info_init(netwib_uint32 threadid,
                                               netwox_httpclictx *pctx,
                                               netwib_constbuf *purl,
                                               netwib_uint32 recursionlevel,
                                              netwox_htmltag_elmtattr elmtattr,
                                              netwox_webspidermt_info **ppinfo)
{
  netwox_webspidermt_info *pinfo;

  netwib_er(netwib_ptr_malloc(sizeof(netwox_webspidermt_info),
                              (netwib_ptr*)&pinfo));
  *ppinfo = pinfo;

  pinfo->threadid = threadid;
  pinfo->pctx = pctx;
  netwib_er(netwib_buf_init_mallocdefault(&pinfo->url));
  netwib_er(netwib_buf_init_mallocdefault(&pinfo->contenttype));
  netwib_er(netwib_buf_append_buf(purl, &pinfo->url));
  pinfo->recursionlevel = recursionlevel;
  pinfo->elmtattr = elmtattr;
  netwib_er(netwib_buf_init_mallocdefault(&pinfo->filename));

  return(NETWIB_ERR_OK);
}

static netwib_err netwox_webspidermt_info_close(netwox_webspidermt_info **ppinfo)
{
  netwox_webspidermt_info *pinfo;

  pinfo = *ppinfo;

  netwib_er(netwib_buf_close(&pinfo->url));
  netwib_er(netwib_buf_close(&pinfo->contenttype));
  netwib_er(netwib_buf_close(&pinfo->filename));

  netwib_er(netwib_ptr_free((netwib_ptr*)&pinfo));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* each thread executes this function */
static netwib_err netwox_webspidermt_run_mt_thread(netwib_ptr infosin,
                                                   netwib_ptr *pinfosout)
{
  netwox_webspidermt_info *pinfo = infosin;

  /* download url */
  netwib_er(netwox_url_download_file_ct(pinfo->pctx, &pinfo->url,
                                        &pinfo->filename, &pinfo->statuscode,
                                        &pinfo->contenttype));

  *pinfosout = pinfo;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* manager thread for multi-thread */
static netwib_err netwox_webspidermt_run_mt(netwox_webspider *pwebspider,
                                            netwox_webspidermt_pf pfunc)
{
  netwib_bufpool *pbufpool = pwebspider->httpclictx.pbufpool;
  netwib_ring *pthreadlist;
  netwib_thread *pthread;
  netwib_hash_index *phashindex;
  netwox_webspidermt_info *pinfo;
  netwib_buf *purl;
  netwib_ptr pitem;
  netwib_uint32 count, threadid, numdots;
  netwib_bool event, newthreadlaunched;
  netwox_scale scale;
  netwox_fraction fraction;
  netwib_err ret=NETWIB_ERR_OK, ret2;

  netwib_er(netwib_threadlist_init(&pthreadlist));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &purl));
  netwib_er(netwib_hash_index_init(pwebspider->phashurlstodownload,
                                   &phashindex));
  netwib_er(netwox_scale_init(0, &scale));
  netwib_er(netwox_fraction_init(&fraction));

  threadid = 1;
  numdots = 0;
  while(NETWIB_TRUE) {
    /* eventually launch a new thread */
    newthreadlaunched = NETWIB_FALSE;
    netwib_er(netwib_ring_ctl_get_count(pthreadlist, &count));
    if (count < pwebspider->maxthreads) {
      netwib__buf_reinit(purl);
      ret = netwib_hash_index_next(phashindex, purl, &pitem);
      if (ret == NETWIB_ERR_DATAEND) {
        netwib__buf_reinit(purl);
        netwib_er(netwib_hash_index_ctl_set_rewind(phashindex));
        ret = netwib_hash_index_next(phashindex, purl, &pitem);
      }
      if (ret == NETWIB_ERR_OK) {
        /* create a new thread */
        netwib_er(netwox_webspidermt_info_init(threadid, &pwebspider->httpclictx, purl, NETWOX_WEBSPIDERURL1_RECURSIONLEVEL(pitem), NETWOX_WEBSPIDERURL1_ELMTATTR(pitem), &pinfo));
        netwib_er(netwib_buf_append_buf(&pwebspider->localrootdir,
                                        &pinfo->filename));
        netwib_er(netwib_buf_append_fmt(&pinfo->filename,
                                        "/tmp/thread%{uint32}", threadid));
        if (pwebspider->loglevel == NETWOX_WEBSPIDER_LOGLEVEL_NORMAL) {
          netwib_er(netwib_fmt_display("GET(%{uint32}) %{buf}\n", threadid,
                                       purl));
        } else if (pwebspider->loglevel == NETWOX_WEBSPIDER_LOGLEVEL_DOT) {
          netwib_er(netwib_fmt_display("."));
          if (numdots++ == 70) {
            netwib_er(netwib_fmt_display("\n"));
            numdots = 0;
          }
        } else if (pwebspider->loglevel == NETWOX_WEBSPIDER_LOGLEVEL_SCALE) {
          netwib_er(netwox_scale_update(&scale, count+1));
        } else if (pwebspider->loglevel == NETWOX_WEBSPIDER_LOGLEVEL_FRACTION) {
          netwib_er(netwox_fraction_update(&fraction, pwebspider->numtotalurls-count, pwebspider->numtotalurls));
        }
        netwib_er(netwib_thread_init(&netwox_webspidermt_run_mt_thread,
                                     (netwib_ptr)pinfo, &pthread));
        netwib_er(netwib_threadlist_add(pthreadlist, pthread, threadid));
        threadid++;
        newthreadlaunched = NETWIB_TRUE;
        /* remove from hash to download */
        netwib_er(netwib_hash_index_this_del(phashindex, NETWIB_FALSE));
        /* add in hash in progress */
        netwib_er(netwib_hash_add(pwebspider->phashurlsinprogress, purl,
                                  NULL, NETWIB_FALSE));
      }
    }
    /* check if one thread exited */
    ret = netwib_threadlist_wait(pthreadlist, newthreadlaunched?NETWIB_TIME_ZERO:NETWIB_TIME_INFINITE, &event, NULL, &ret2, (netwib_ptr*)&pinfo);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATANOTAVAIL) {
        ret = NETWIB_ERR_OK;
        continue;
      }
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    if (event) {
      if (ret2 != NETWIB_ERR_OK) {
        ret = ret2;
        break;
      }
      /* call user function */
      if (pwebspider->loglevel == NETWOX_WEBSPIDER_LOGLEVEL_NORMAL) {
        netwib_er(netwib_fmt_display("ANALYZE(%{uint32}) %{buf}\n",
                                     pinfo->threadid, &pinfo->url));
      } else if (pwebspider->loglevel == NETWOX_WEBSPIDER_LOGLEVEL_THREAD) {
        netwib_er(netwib_fmt_display("%{uint32}", pinfo->threadid%10));
        if (numdots++ == 70) {
          netwib_er(netwib_fmt_display("\n"));
          numdots = 0;
        }
      }
      ret = (*pfunc)(pwebspider, &pinfo->url, &pinfo->contenttype,
                     pinfo->recursionlevel, pinfo->elmtattr, &pinfo->filename,
                     pinfo->statuscode);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
      /* remove from in progress */
      netwib_er(netwib_hash_del(pwebspider->phashurlsinprogress, &pinfo->url,
                                NETWIB_FALSE));
      /* close */
      netwib_er(netwox_webspidermt_info_close(&pinfo));
      if (pwebspider->millisleep) {
        netwib_er(netwib_time_sleep_msec(pwebspider->millisleep));
      }
    }
  }

  netwib_er(netwox_fraction_close(&fraction));
  netwib_er(netwox_scale_close(&scale));
  netwib_er(netwib_hash_index_close(&phashindex));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &purl));
  netwib_er(netwib_threadlist_close(&pthreadlist));

  if (pwebspider->loglevel == NETWOX_WEBSPIDER_LOGLEVEL_DOT ||
      pwebspider->loglevel == NETWOX_WEBSPIDER_LOGLEVEL_THREAD) {
    if (numdots != 0) {
      netwib_er(netwib_fmt_display("\n"));
    }
  }

  return(ret);
}

/*-------------------------------------------------------------*/
/* manager thread for mono-thread */
static netwib_err netwox_webspidermt_run_mono(netwox_webspider *pwebspider,
                                              netwox_webspidermt_pf pfunc)
{
  netwib_bufpool *pbufpool = pwebspider->httpclictx.pbufpool;
  netwib_hash_index *phashindex;
  netwib_buf *purl, *pcontenttype;
  netwib_ptr pitem;
  netwox_httphdr_statuscode statuscode;
  netwib_uint32 numdots, count;
  netwox_scale scale;
  netwox_fraction fraction;
  netwib_buf *pfilename;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_hash_index_init(pwebspider->phashurlstodownload,
                                   &phashindex));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &purl));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pcontenttype));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pfilename));
  netwib_er(netwox_scale_init(1, &scale));
  netwib_er(netwox_fraction_init(&fraction));

  numdots = 0;
  while(NETWIB_TRUE) {
    /* check for new url */
    netwib__buf_reinit(purl);
    ret = netwib_hash_index_next(phashindex, purl, &pitem);
    if (ret == NETWIB_ERR_DATAEND) {
      netwib__buf_reinit(purl);
      netwib_er(netwib_hash_index_ctl_set_rewind(phashindex));
      ret = netwib_hash_index_next(phashindex, purl, &pitem);
    }
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    /* log */
    if (pwebspider->loglevel == NETWOX_WEBSPIDER_LOGLEVEL_NORMAL) {
      netwib_er(netwib_fmt_display("GET %{buf}\n", purl));
    } else if (pwebspider->loglevel == NETWOX_WEBSPIDER_LOGLEVEL_DOT) {
      netwib_er(netwib_fmt_display("."));
      if (numdots++ == 70) {
        netwib_er(netwib_fmt_display("\n"));
        numdots = 0;
      }
    } else if (pwebspider->loglevel == NETWOX_WEBSPIDER_LOGLEVEL_SCALE) {
      netwib_er(netwib_hash_ctl_get_count(pwebspider->phashurlstodownload,
                                          &count));
      if (count > scale.maxvalue) scale.maxvalue = 125*count/100;
      netwib_er(netwox_scale_update(&scale, count));
    } else if (pwebspider->loglevel == NETWOX_WEBSPIDER_LOGLEVEL_FRACTION) {
      netwib_er(netwib_hash_ctl_get_count(pwebspider->phashurlstodownload,
                                          &count));
      netwib_er(netwox_fraction_update(&fraction, pwebspider->numtotalurls-count, pwebspider->numtotalurls));
    }
    /* download url */
    netwib__buf_reinit(pfilename);
    netwib_er(netwib_buf_append_buf(&pwebspider->localrootdir, pfilename));
    netwib_er(netwib_buf_append_string("/tmp/get", pfilename));
    netwib__buf_reinit(pcontenttype);
    netwib_er(netwox_url_download_file_ct(&pwebspider->httpclictx, purl,
                                          pfilename, &statuscode,
                                          pcontenttype));
    /* call user function */
    if (pwebspider->loglevel == NETWOX_WEBSPIDER_LOGLEVEL_THREAD) {
      netwib_er(netwib_fmt_display("1"));
      if (numdots++ == 70) {
        netwib_er(netwib_fmt_display("\n"));
        numdots = 0;
      }
    }
    ret = (*pfunc)(pwebspider, purl, pcontenttype, NETWOX_WEBSPIDERURL1_RECURSIONLEVEL(pitem), NETWOX_WEBSPIDERURL1_ELMTATTR(pitem), pfilename, statuscode);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    if (pwebspider->millisleep) {
      netwib_er(netwib_time_sleep_msec(pwebspider->millisleep));
    }
    /* remove from hash */
    netwib_er(netwib_hash_index_this_del(phashindex, NETWIB_FALSE));
  }

  netwib_er(netwox_fraction_close(&fraction));
  netwib_er(netwox_scale_close(&scale));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pfilename));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pcontenttype));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &purl));
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
/* manager thread */
netwib_err netwox_webspidermt_run(netwox_webspider *pwebspider,
                                  netwox_webspidermt_pf pfunc)
{
  netwib_err ret;

  if (pwebspider->maxthreads <= 1) {
    ret = netwox_webspidermt_run_mono(pwebspider, pfunc);
  } else {
    ret = netwox_webspidermt_run_mt(pwebspider, pfunc);
  }

  return(ret);
}
