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
netwib_err netwox_webspiderurl1_add(netwox_webspider *pwebspider,
                                    netwib_constbuf *purl,
                                    netwib_uint32 recursionlevel,
                                    netwox_htmltag_elmtattr elmtattr)
{
  netwox_htmltag_elmtattr_uricontain uricontain;
  netwib_buf url;
  netwib_data data, pc;
  netwib_uint32 datasize;
  netwib_bool match;

  /* check type */
  netwib_er(netwox_htmltag_uricontain_init_elmtattr(elmtattr, &uricontain));
  switch(uricontain) {
  case NETWOX_HTMLTAG_ELMTATTR_URICONTAIN_HTML :
    break;
  case NETWOX_HTMLTAG_ELMTATTR_URICONTAIN_IMAGE :
    if (!pwebspider->getimage) {
      return(NETWIB_ERR_OK);
    }
    break;
  case NETWOX_HTMLTAG_ELMTATTR_URICONTAIN_MULTIMEDIA :
    if (!pwebspider->getmultimedia) {
      return(NETWIB_ERR_OK);
    }
    break;
  case NETWOX_HTMLTAG_ELMTATTR_URICONTAIN_CODE :
    if (!pwebspider->getcode) {
      return(NETWIB_ERR_OK);
    }
    break;
  default :
    return(NETWIB_ERR_OK);
  }

  /* check recursion level */
  if(pwebspider->maxrecursion != 0) {
    if (recursionlevel == pwebspider->maxrecursion + 1) {
      if (uricontain == NETWOX_HTMLTAG_ELMTATTR_URICONTAIN_HTML) {
        return(NETWIB_ERR_OK);
      }
    } else if (recursionlevel > pwebspider->maxrecursion) {
      return(NETWIB_ERR_OK);
    }
  }

  /* suppress fragment */
  url = *purl;
  data = netwib__buf_ref_data_ptr(&url);
  datasize = netwib__buf_ref_data_size(&url);
  pc = netwib_c_memchr(data, '#', datasize);
  if (pc != NULL) {
    url.endoffset = url.beginoffset + pc - data;
  }

  /* check if already in the hashes */
  netwib_er(netwib_hash_contains(pwebspider->phashurlstodownload, &url,
                                 &match));
  if (match) {
    return(NETWIB_ERR_OK);
  }
  netwib_er(netwib_hash_contains(pwebspider->phashdownloadedurls, &url,
                                 &match));
  if (match) {
    return(NETWIB_ERR_OK);
  }
  if (pwebspider->maxthreads > 1) {
    netwib_er(netwib_hash_contains(pwebspider->phashurlsinprogress, &url,
                                   &match));
    if (match) {
      return(NETWIB_ERR_OK);
    }
  }

  /* check ACL */
  if (pwebspider->aclurlset) {
    netwib_er(netwox_rules_match(pwebspider->paclurl, 1, &url, &match));
    if (!match) {
      return(NETWIB_ERR_OK);
    }
  }

  /* add in the hash */
  if (pwebspider->loglevel == NETWOX_WEBSPIDER_LOGLEVEL_NORMAL) {
    netwib_er(netwib_fmt_display("ADD %{buf}\n", &url));
  }
  if (recursionlevel == 0xFFFF) {
    /* we cannot limit this branch, but ignore */
    recursionlevel = 0xFFFE;
  }
  if (elmtattr > 0xFFFF) {
    return(NETWOX_ERR_INTERNALERROR);
  }
  netwib_er(netwib_hash_add(pwebspider->phashurlstodownload, &url,
                            (netwib_constptr)(netwib_uintptr)((recursionlevel<<16)|elmtattr),
                            NETWIB_FALSE));
  pwebspider->numtotalurls++;

  return(NETWIB_ERR_OK);
}

