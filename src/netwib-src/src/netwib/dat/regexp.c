/*
                                  NETWIB
                             Network library
                Copyright(c) 1999-2012 Laurent CONSTANTIN
                      http://ntwib.sourceforge.net/
                        laurentconstantin@free.fr
                                  -----
  This file is part of Netwib.

  Netwib is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3
  as published by the Free Software Foundation.

  Netwib is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details (http://www.gnu.org/).

------------------------------------------------------------------------
*/

#include <netwib/inc/maininc.h>

/*-------------------------------------------------------------*/
netwib_err netwib_buf_search_re(netwib_constbuf *pbuf,
                                netwib_constbuf *pregularexpression,
                                netwib_bool casesensitive,
                                netwib_bufext *pfound)
{
  netwib_regexp found;

  netwib_er(netwib_buf_search_regexp(pbuf, pregularexpression, casesensitive,
                                     &found));

  netwib_er(netwib_buf_init_ext_buf(&found.array[0], pfound));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_search_regexp(netwib_constbuf *pbuf,
                                    netwib_constbuf *pregularexpression,
                                    netwib_bool casesensitive,
                                    netwib_regexp *pfound)
{
  netwib_string str, regularexpression;
  netwib_ptr ptrerrmsg;
  netwib_bool foundinvalid;
  netwib_uint32 i, laststart, strlenstr;
  regex_t re;
  regmatch_t pm[NETWIB_REGEXP_MAXLEN + 1];
  int retint;
  netwib_err ret;

  netwib__constbuf_ref_string(pregularexpression, regularexpression, bufstorage,
                              netwib_buf_search_regexp(pbuf, &bufstorage,
                                                       casesensitive, pfound));

  /* if pbuf cannot be converted to string, a new buffer is used, but items
     in pfound have to be recomputed */
  ret = netwib_constbuf_ref_string(pbuf, &str);
  if (ret != NETWIB_ERR_OK) {
    if (ret == NETWIB_ERR_DATANOSPACE) {
      netwib_data bufstoragearray[512];
      netwib_buf bufstorage;
      netwib_er(netwib_buf_init_ext_storagearraysizeof(bufstoragearray,
                                                       &bufstorage));
      netwib_er(netwib_buf_append_buf(pbuf, &bufstorage));
      netwib_er(netwib_buf_append_byte(0, &bufstorage));
      bufstorage.endoffset--;
      ret = netwib_buf_search_regexp(&bufstorage, pregularexpression,
                                     casesensitive, pfound);
      if (ret == NETWIB_ERR_OK && pfound != NULL) {
        for (i = 0; i < pfound->numset; i++) {
          pfound->array[i].totalptr += pbuf->totalptr - bufstorage.totalptr;
          pfound->array[i].beginoffset += pbuf->beginoffset - bufstorage.beginoffset;
          pfound->array[i].endoffset += pbuf->endoffset - bufstorage.endoffset;
        }
      }
      netwib_er(netwib_buf_close(&bufstorage));
    }
    return(ret);
  }

  /* compile expression */
  retint = regcomp(&re, regularexpression,
                   REG_EXTENDED | (casesensitive ? 0 : REG_ICASE));
  if (retint) {
    netwib_er(netwib_ptr_malloc(500, &ptrerrmsg));
    regerror(retint, &re, (netwib_string)ptrerrmsg, 500);
#if ! defined NETWIBDEF_SYSNAME_Solaris
    /* calling regfree on error leads to a crash on older Solaris */
    regfree(&re);
#endif
    netwib_er(netwib_priv_errmsg_string("regular expression is incorrect: "));
    netwib_er(netwib_priv_errmsg_append_string((netwib_conststring)ptrerrmsg));
    netwib_er(netwib_ptr_free(&ptrerrmsg));
    return(NETWIB_ERR_FUREGCOMP);
  }

  /* search data */
  retint = regexec(&re, str, NETWIB_REGEXP_MAXLEN + 1, pm, 0);
  regfree(&re);
  if (retint) {
    return(NETWIB_ERR_NOTFOUND);
  }

  /* set output array */
  if (pfound != NULL) {
    foundinvalid = NETWIB_FALSE;
    laststart = 0;
    strlenstr = netwib_c_strlen(str);
    for (i = 0; i <= NETWIB_REGEXP_MAXLEN; i++) {
      /* check end */
      if (pm[i].rm_so == -1 || pm[i].rm_eo == -1) {
        /* normal ending case */
        foundinvalid = NETWIB_TRUE;
      } else if ((netwib_uint32)pm[i].rm_so > strlenstr ||
                 (netwib_uint32)pm[i].rm_eo > strlenstr ||
                 (netwib_uint32)pm[i].rm_so < laststart) {
        /* SuSE 5.1 does not set -1, and seems to be buggy */
        foundinvalid = NETWIB_TRUE;
      }
      /* end found */
      if (foundinvalid) {
        break;
      }
      /* set buffer*/
      netwib_er(netwib_buf_init_ext_arrayfilled(netwib__buf_ref_data_ptr(pbuf) + pm[i].rm_so, pm[i].rm_eo - pm[i].rm_so, &pfound->array[i]));
      netwib__buf_transfersensitive(pbuf, &pfound->array[i]);
      netwib__buf_transfersensitive(pregularexpression, &pfound->array[i]);
      laststart = pm[i].rm_so;
    }
    pfound->numset = i;
  }

  return(NETWIB_ERR_OK);
}
