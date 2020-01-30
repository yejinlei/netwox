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
netwib_err netwox_htmlfile_urllist_item_create(netwib_bool supportreplacing,
                                               netwib_ptr *ppitem)
{
  netwox_htmlfile_urllist_item *pitem;

  netwib_er(netwib_ptr_malloc(sizeof(netwox_htmlfile_urllist_item), ppitem));
  pitem = *ppitem;

  netwib_er(netwib_buf_init_mallocdefault(&pitem->value));
  pitem->supportreplacing = supportreplacing;
  if (supportreplacing) {
    pitem->suppresstag = NETWIB_FALSE;
    pitem->suppressattribute = NETWIB_FALSE;

    netwib_er(netwib_buf_init_mallocdefault(&pitem->replacingvalue));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_htmlfile_urllist_item_erase(netwib_ptr pitem)
{
  netwox_htmlfile_urllist_item *purlitem;

  purlitem = (netwox_htmlfile_urllist_item *)pitem;
  netwib_er(netwib_buf_close(&purlitem->value));
  if (purlitem->supportreplacing) {
    netwib_er(netwib_buf_close(&purlitem->replacingvalue));
  }

  netwib_er(netwib_ptr_free(&pitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_htmlfile_urllist_ring_display(netwib_ring *pring)
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
    netwib_er(netwib_fmt_display("%{uint32} [%{uint32}-%{uint32}] [%{uint32}-%{uint32}] [%{uint32}-%{uint32}] : %{buf}\n", pitem->elmtattr, pitem->tagbegin, pitem->tagend, pitem->attributebegin, pitem->attributeend, pitem->valuebegin, pitem->valueend, &pitem->value));
    if (pitem->supportreplacing) {
      netwib_er(netwib_fmt_display("  tag=%{bool:t} att=%{bool:t} val=%{buf}\n", pitem->suppresstag, pitem->suppressattribute, &pitem->replacingvalue));
    }
  }

  netwib_er(netwib_ring_index_close(&pringindex));

  return(ret);
}

/*-------------------------------------------------------------*/
typedef struct {
  netwib_ring *pring;
  netwib_bool supportreplacing;
  netwib_buf buf;
  netwib_uint32 fileoffset;
} netwox_htmlfile_ctx;

/*-------------------------------------------------------------*/
static netwib_err netwox_htmlfile_attribute(netwox_htmlfile_ctx *pctx,
                                            netwib_constbuf *ptag,
                                            netwox_htmltag_elmt elmt,
                                            netwib_uint32 elementendoffset)
{
  netwib_buf att, val;
  netwib_data data, datasave, pcattbegin, pcattend, pcvalbegin, pcvalend;
  netwib_data pcfullvalbegin, pcfullvalend;
  netwib_uint32 datasize;
  netwox_htmltag_elmtattr elmtattr;
  netwox_htmlfile_urllist_item *pitem;
  netwib_byte delim;
  netwib_bool cansaverefresh;

  /* obtain pointers */
  data = netwib__buf_ref_data_ptr(ptag);
  datasave = data;
  datasize = netwib__buf_ref_data_size(ptag);

  /* skip element */
  data += elementendoffset;
  datasize -= elementendoffset;

  /* decode "att=val att="val" att= 'val' att" */
#define netwox__attribute_skip_char() { data++; datasize--; if (datasize == 0) return(NETWIB_ERR_DATANOTAVAIL); }
#define netwox__attribute_skip_spaces() { while(netwib_c2_isspace(*data)) { netwox__attribute_skip_char() } }
#define netwox__attribute_check_gt() { if (*data == '>') break; }
  cansaverefresh = NETWIB_FALSE;
  while(NETWIB_TRUE) {
    netwox__attribute_skip_spaces();
    netwox__attribute_check_gt();
    /* skip attribute name */
    pcattbegin = data;
    while(!netwib_c2_isspace(*data) && *data != '=' && *data != '>') {
      netwox__attribute_skip_char();
    }
    pcattend = data;
    netwox__attribute_skip_spaces();
    netwox__attribute_check_gt();
    /* check for '=', then value */
    if (*data == '=') {
      netwox__attribute_skip_char();
      netwox__attribute_skip_spaces();
      netwox__attribute_check_gt(); /* no value : ignore */
      /* check for value */
      pcfullvalbegin = data;
      if (*data == '\'' || *data == '"') {
        delim = *data;
        netwox__attribute_skip_char();
        pcvalbegin = data;
        while(*data != delim) {
          netwox__attribute_skip_char();
        }
        pcvalend = data;
        netwox__attribute_skip_char();
      } else {
        pcvalbegin = data;
        while(!netwib_c2_isspace(*data) && *data != '>') {
          netwox__attribute_skip_char();
        }
        pcvalend = data;
      }
      pcfullvalend = data;
      /* now, check if this attribute is interesting */
      netwib_er(netwib_buf_init_ext_arrayfilled(pcattbegin,
                                                pcattend - pcattbegin, &att));
      netwib_er(netwox_htmltag_elmtattr_init_buf(elmt, &att, &elmtattr));
      if (elmtattr == NETWOX_HTMLTAG_ELMTATTR_META_REFRESHCONTENT) {
        /* We save content only if http-equiv was seen :
           <meta http-equiv="refresh" content="0;url=http://s/"> */
        if (!cansaverefresh) {
          /* setting to unknown will not save it below */
          elmtattr = NETWOX_HTMLTAG_ELMTATTR_UNKNOWN;
        }
      }
      if (elmtattr == NETWOX_HTMLTAG_ELMTATTR_META_HTTPEQUIV) {
        if (pcvalend - pcvalbegin == 7) {
          if (!netwib_c_memcasecmp(pcvalbegin,
                                   (netwib_constdata)"refresh", 7)) {
            /* http-equiv was seen, and contain "refresh" */
            cansaverefresh = NETWIB_TRUE;
          }
        }
      } else if (elmtattr != NETWOX_HTMLTAG_ELMTATTR_UNKNOWN) {
        /* add it in the ring */
        netwib_er(netwox_htmlfile_urllist_item_create(pctx->supportreplacing,
                                                      (netwib_ptr*)&pitem));
        netwib_er(netwib_buf_init_ext_arrayfilled(pcvalbegin,
                                                  pcvalend - pcvalbegin,
                                                  &val));
#define NETWOX_HTMLFILE_COMPATIBILITY_TRICK 1
#if NETWOX_HTMLFILE_COMPATIBILITY_TRICK == 1
        /* Now, be compatible with Internet Explorer and Mozilla, but not
           with HTML specification.
           If HTML contains <A HREF=" \t\nuri\n\t "> for example, the leading
           and ending spaces, tabulation and newlines are ignored. */
        {
          netwox_htmltag_elmtattr_uriformat uriformat;
          netwib_data valdata;
          netwib_uint32 valdatasize, i;
          netwib_er(netwox_htmltag_uriformat_init_elmtattr(elmtattr,
                                                           &uriformat));
          if (uriformat == NETWOX_HTMLTAG_ELMTATTR_URIFORMAT_ONE) {
            valdata = netwib__buf_ref_data_ptr(&val);
            valdatasize = netwib__buf_ref_data_size(&val);
            for (i = 0; i < valdatasize; i++) {
              if (valdata[i] != ' ' && valdata[i] != '\t' &&
                  valdata[i] != '\r' && valdata[i] != '\n' ) {
                break;
              }
            }
            if (i == valdatasize) {
              val.endoffset = val.beginoffset;
            } else {
              val.beginoffset += i;
              for (i = valdatasize-1; i != 0/* stop one before is ok */; i--) {
                if (valdata[i] != ' ' && valdata[i] != '\t' &&
                    valdata[i] != '\r' && valdata[i] != '\n' ) {
                  break;
                }
              }
              val.endoffset -= valdatasize - i - 1;
            }
          }
        }
#endif
        netwib_er(netwox_html_data_decode_best(&val, &pitem->value));
        pitem->elmtattr = elmtattr;
        pitem->tagbegin = pctx->fileoffset;
        pitem->tagend = pctx->fileoffset + netwib__buf_ref_data_size(ptag);
        pitem->attributebegin = pctx->fileoffset + pcattbegin - datasave;
        pitem->attributeend = pctx->fileoffset + pcattend - datasave;
        pitem->valuebegin = pctx->fileoffset + pcfullvalbegin - datasave;
        pitem->valueend = pctx->fileoffset + pcfullvalend - datasave;
        netwib_er(netwib_ring_add_last(pctx->pring, (netwib_ptr)pitem));
      }
    }
  }

  /* skip entire TAG */
  pctx->fileoffset += netwib__buf_ref_data_size(ptag);
  pctx->buf.beginoffset += netwib__buf_ref_data_size(ptag);
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_htmlfile_tag_end(netwox_htmlfile_ctx *pctx,
                                          netwib_uint32 *pelementendoffset,
                                          netwib_uint32 *ptagendoffset)
{
  netwib_data data, datasave;
  netwib_uint32 datasize;
  netwib_byte delim;

  data = netwib__buf_ref_data_ptr(&pctx->buf);
  datasave = data;
  datasize = netwib__buf_ref_data_size(&pctx->buf);

#define netwox__tag_end_skip_char() { data++; datasize--; if (datasize == 0) return(NETWIB_ERR_DATANOTAVAIL); }
#define netwox__tag_end_skip_spaces() { while(netwib_c2_isspace(*data)) { netwox__tag_end_skip_char() } }
#define netwox__tag_end_check_gt() { if (*data == '>') { *ptagendoffset = data - datasave + 1; return(NETWIB_ERR_OK); } }

  /* find element end */
  netwox__tag_end_skip_char(); /* leading '<' */
  while(!netwib_c2_isspace(*data) && *data != '>') {
    netwox__tag_end_skip_char();
  }
  *pelementendoffset = data - datasave;

  /* find tag end */
  while(NETWIB_TRUE) {
    netwox__tag_end_skip_spaces();
    netwox__tag_end_check_gt();
    /* skip attribute name */
    while(!netwib_c2_isspace(*data) && *data != '=' && *data != '>') {
      netwox__tag_end_skip_char();
    }
    netwox__tag_end_skip_spaces();
    netwox__tag_end_check_gt();
    /* check for '=', then value */
    if (*data == '=') {
      netwox__tag_end_skip_char();
      netwox__tag_end_skip_spaces();
      netwox__tag_end_check_gt(); /* no value : ignore */
      /* check for value */
      if (*data == '\'' || *data == '"') {
        delim = *data;
        netwox__tag_end_skip_char();
        while(*data != delim) {
          netwox__tag_end_skip_char();
        }
        netwox__tag_end_skip_char();
      } else {
        while(!netwib_c2_isspace(*data) && *data != '>') {
          netwox__tag_end_skip_char();
        }
      }
    }
  }

  return(NETWIB_ERR_DATANOTAVAIL);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_htmlfile_tag(netwox_htmlfile_ctx *pctx)
{
  netwib_buf element, tag;
  netwib_data data, pc;
  netwib_uint32 datasize, tmpsize, elementendoffset, tagendoffset;
  netwox_htmltag_elmt elmt;

  datasize = netwib__buf_ref_data_size(&pctx->buf);
  if (datasize < 3) {
    /* no sufficient data because smallest chunk is "<x>" */
    return(NETWIB_ERR_DATANOTAVAIL);
  }
  data = netwib__buf_ref_data_ptr(&pctx->buf);

  /* The second char determine tag type :
        <[a-Z] : starting tag
        </ : ending tag
        <! : comment
        <. : not a tag
     As we are interested only by URIs, we only look for
     starting tags.
     Notes :
      - an ending tag cannot contain '<' before the ending '>'
        (ex : </A < >), so skipping 1 char ("<") will not
        perturb the searching tag algorithm.
      - a comment can contain :
          <!abc>
          <!-- a<bc -- >
        If we skip the first char, the searching tag algorithm will
        find the '<' between a and b. However, this doesn't bother us
        because the last > will match the tag end. As a comment doesn't
        contain URI, it's not a problem to analyze it improperly.
      - in the last case, we can have "<<A HREF=...", so, skipping
        one char is ok, because it will permit us to detect <A HREF...
     So, if tag is not a starting tag, we skip one char (the '<').
  */
  if (!netwib_c2_isalpha(data[1])) {
    pctx->fileoffset++;
    pctx->buf.beginoffset++;
    return(NETWIB_ERR_OK);
  }

  /* find tag end offset (we cannot simply search '>' because following
     tag is valid : <IMG ALT="a>b" SRC="t.png">). We also compute
     element end offset. */
  netwib_er(netwox_htmlfile_tag_end(pctx, &elementendoffset, &tagendoffset));
  netwib_er(netwib_buf_init_ext_arrayfilled(data+1, elementendoffset-1,
                                            &element));
  netwib_er(netwib_buf_init_ext_arrayfilled(data, tagendoffset, &tag));

  /* script element is particular : ignore everything till </script>.
     It's the same for style element. */
  if (elementendoffset == 7) {
    if (!netwib_c_memcasecmp(data, (netwib_constdata)"<script", 7)) {
      pc = netwib_c_memcasemem(data,datasize, (netwib_constdata)"</script>",9);
      if (pc != NULL) {
        tmpsize = pc - data + 9;
        pctx->fileoffset += tmpsize;
        pctx->buf.beginoffset += tmpsize;
        return(NETWIB_ERR_OK);
      } else {
        return(NETWIB_ERR_DATANOTAVAIL);
      }
    }
  }
  if (elementendoffset == 6) {
    if (!netwib_c_memcasecmp(data, (netwib_constdata)"<style", 6)) {
      pc = netwib_c_memcasemem(data,datasize, (netwib_constdata)"</style>",8);
      if (pc != NULL) {
        tmpsize = pc - data + 8;
        pctx->fileoffset += tmpsize;
        pctx->buf.beginoffset += tmpsize;
        return(NETWIB_ERR_OK);
      } else {
        return(NETWIB_ERR_DATANOTAVAIL);
      }
    }
  }

  /* check if element can contain an uri */
  netwib_er(netwox_htmltag_elmt_init_buf(&element, &elmt));
  if (elmt == NETWOX_HTMLTAG_ELMT_UNKNOWN) {
    /* this tag is not interesting : suppress it */
    pctx->fileoffset += tagendoffset;
    pctx->buf.beginoffset += tagendoffset;
    return(NETWIB_ERR_OK);
  }

  /* decode all attributes */
  netwib_er(netwox_htmlfile_attribute(pctx, &tag, elmt, elementendoffset));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_htmlfile_chunk(netwox_htmlfile_ctx *pctx)
{
  netwib_data data, pc;
  netwib_uint32 datasize, uselessdatasize;
  netwib_err ret;

  ret = NETWIB_ERR_OK;
  while(NETWIB_TRUE) {
    /* chunk of data */
    data = netwib__buf_ref_data_ptr(&pctx->buf);
    datasize = netwib__buf_ref_data_size(&pctx->buf);
    /* search tag */
    pc = netwib_c_memchr(data, '<', datasize);
    if (pc == NULL) {
      /* not found : skip this useless part */
      pctx->fileoffset += datasize;
      netwib__buf_reinit(&pctx->buf);
      break;
    }
    /* skip data before tag */
    uselessdatasize = pc - data;
    pctx->fileoffset += uselessdatasize;
    pctx->buf.beginoffset += uselessdatasize;
    /* analyze this tag */
    ret = netwox_htmlfile_tag(pctx);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_htmlfile_urllist_file_read(netwib_constbuf *pfilename,
                                             netwib_bool supportreplacing,
                                             netwib_ring *pring)
{
  netwox_htmlfile_ctx ctx;
  netwib_io *pio;
  netwib_err ret;

  netwib_er(netwib_io_init_file_read(pfilename, &pio));

  netwib_er(netwib_buf_init_mallocdefault(&ctx.buf));
  ctx.buf.flags |= NETWIB_BUF_FLAGS_CANSLIDE;
  ctx.supportreplacing = supportreplacing;
  ctx.pring = pring;
  ctx.fileoffset = 0;

  ret = NETWIB_ERR_OK;
  while(NETWIB_TRUE) {
    ret = netwib_io_read(pio, &ctx.buf);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    ret = netwox_htmlfile_chunk(&ctx);
    if (ret != NETWIB_ERR_OK && ret != NETWIB_ERR_DATANOTAVAIL) {
      break;
    }
  }

  netwib_er(netwib_buf_close(&ctx.buf));

  netwib_er(netwib_io_close(&pio));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_htmlfile_urllist_file_write(netwib_constbuf *pinfilename,
                                              netwib_ring *pring,
                                              netwib_constbuf *poutfilename)
{
  netwib_buf buf, buf2;
  netwib_io *pioin, *pioout;
  netwib_ring_index *pringindex;
  netwox_htmlfile_urllist_item *pitem=NULL;
  netwib_uint32 fileoffset, bufsize, tmpsize, beginsuppress, endsuppress;
  netwib_bool insertdata, endring, endio, needring, needio;
  netwib_err ret;

  /* init */
  netwib_er(netwib_io_init_file_read(pinfilename, &pioin));
  netwib_er(netwib_io_init_file_write(poutfilename, &pioout));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));
  buf.flags |= NETWIB_BUF_FLAGS_CANSLIDE;
  netwib_er(netwib_ring_index_init(pring, &pringindex));

  /* main loop */
  fileoffset = 0;
  ret = NETWIB_ERR_OK;
  endring = NETWIB_FALSE;
  endio = NETWIB_FALSE;
  needring = NETWIB_TRUE;
  needio = NETWIB_TRUE;
  insertdata = NETWIB_FALSE;
  beginsuppress = 0;
  endsuppress = 0;
  while(NETWIB_TRUE) {
    /* search an item containing replacing information */
    while(needring && !endring) {
      ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pitem);
      if (ret == NETWIB_ERR_OK) {
        if (pitem->supportreplacing) {
          /* compute begin, end and if we insert data */
          if (pitem->suppresstag) {
            beginsuppress = pitem->tagbegin;
            endsuppress = pitem->tagend;
            insertdata = NETWIB_FALSE;
          } else if (pitem->suppressattribute) {
            beginsuppress = pitem->attributebegin;
            endsuppress = pitem->valueend;
            insertdata = NETWIB_FALSE;
          } else {
            beginsuppress = pitem->valuebegin;
            endsuppress = pitem->valueend;
            insertdata = NETWIB_TRUE;
          }
          needring = NETWIB_FALSE;
          break;
        }
      } else if (ret == NETWIB_ERR_DATAEND) {
        endring = NETWIB_TRUE;
        ret = NETWIB_ERR_OK;
        break;
      } else {
        break;
      }
    }
    if (ret != NETWIB_ERR_OK) break;
    /* read data */
    while(needio && !endio) {
      ret = netwib_io_read(pioin, &buf);
      if (ret == NETWIB_ERR_OK) {
        needio = NETWIB_FALSE;
        break;
      } else if (ret == NETWIB_ERR_DATAEND) {
        endio = NETWIB_TRUE;
        ret = NETWIB_ERR_OK;
        break;
      } else if (ret != NETWIB_ERR_DATANOTAVAIL) {
        break;
      }
    }
    if (ret != NETWIB_ERR_OK) break;
    bufsize = netwib__buf_ref_data_size(&buf);
    /* write last chunk */
    if (endring) {
      if (bufsize) {
        netwib_er(netwib_io_write(pioout, &buf));
        netwib__buf_reinit(&buf);
      }
      if (endio) {
        break;
      }
      needio = NETWIB_TRUE;
      continue;
    }
    /* write till begin of suppression */
    if (fileoffset < beginsuppress) {
      if (fileoffset + bufsize <= beginsuppress) {
        netwib_er(netwib_io_write(pioout, &buf));
        fileoffset += bufsize;
        netwib__buf_reinit(&buf);
        needio = NETWIB_TRUE;
        continue;
      } else {
        tmpsize = beginsuppress - fileoffset;
        buf.endoffset = buf.beginoffset + tmpsize;
        netwib_er(netwib_io_write(pioout, &buf));
        buf.endoffset = buf.beginoffset + bufsize;
        buf.beginoffset += tmpsize;
        bufsize -= tmpsize;
        fileoffset += tmpsize;
      }
    }
    /* skip suppressed data */
    if (fileoffset < endsuppress) {
      if (fileoffset + bufsize <= endsuppress) {
        fileoffset += bufsize;
        netwib__buf_reinit(&buf);
        needio = NETWIB_TRUE;
        continue;
      } else {
        tmpsize = endsuppress - fileoffset;
        buf.beginoffset += tmpsize;
        fileoffset += tmpsize;
      }
    }
    /* eventually write replacing data */
    if (insertdata) {
      netwib__buf_reinit(&buf2);
      netwib_er(netwib_buf_append_byte('"', &buf2));
      netwib_er(netwox_html_data_encode(&pitem->replacingvalue, NETWIB_FALSE,
                                        &buf2));
      netwib_er(netwib_buf_append_byte('"', &buf2));
      netwib_er(netwib_io_write(pioout, &buf2));
    }
    /* use next item */
    needring = NETWIB_TRUE;
  }

  /* close */
  netwib_er(netwib_ring_index_close(&pringindex));
  netwib_er(netwib_buf_close(&buf2));
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_io_close(&pioin));
  netwib_er(netwib_io_close(&pioout));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_htmlfile_urllist_canon(netwib_bufpool *pbufpool,
                                         netwib_constbuf *pfileurl,
                                         netwib_bool onlysupported,
                                         netwib_ring *pring)
{
  netwib_ring_index *pringindex;
  netwox_htmlfile_urllist_item *pitem;
  netwox_htmltag_elmtattr_uriformat uriformat;
  netwib_buf *pbasehref, *ptmp;
  netwox_url_scheme scheme;
  netwib_uint32 refresh;
  netwib_err ret;

  netwib_er(netwib_ring_index_init(pring, &pringindex));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbasehref));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &ptmp));

  ret = NETWIB_ERR_OK;
  while(NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pitem);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    /* base href */
    if (pitem->elmtattr == NETWOX_HTMLTAG_ELMTATTR_BASE_HREF) {
      netwib__buf_reinit(pbasehref);
      ret = netwox_urlcanon_url(pbufpool, &pitem->value, pbasehref);
      if (ret != NETWIB_ERR_OK) {
        netwib__buf_reinit(pbasehref);
      }
      if (pitem->supportreplacing) {
        pitem->suppresstag = NETWIB_TRUE;
      }
      continue;
    }
    netwib_er(netwox_htmltag_uriformat_init_elmtattr(pitem->elmtattr,
                                                     &uriformat));
    switch(uriformat) {
    case NETWOX_HTMLTAG_ELMTATTR_URIFORMAT_REFRESH :
      /* <meta http-equiv="refresh" content="0;url=http://s/"> */
      netwib__buf_reinit(ptmp);
      ret = netwib_buf_decode_fmt(&pitem->value, "%{uint32};%{*;buf}=%{buf}%$",
                                  &refresh, ptmp);
      if (ret == NETWIB_ERR_OK) {
        if (pitem->supportreplacing) {
          netwib_er(netwib_buf_append_fmt(&pitem->replacingvalue,
                                          "%{uint32};url=", refresh));
        }
        netwib__buf_reinit(&pitem->value);
        ret = netwox_urlcanon_file_read(pbufpool, pfileurl, pbasehref,
                                        ptmp, &pitem->value);
      }
      break;
    case NETWOX_HTMLTAG_ELMTATTR_URIFORMAT_ONE :
      netwib__buf_reinit(ptmp);
      ret = netwib_buf_append_buf(&pitem->value, ptmp);
      if (ret == NETWIB_ERR_OK) {
        netwib__buf_reinit(&pitem->value);
        ret = netwox_urlcanon_file_read(pbufpool, pfileurl, pbasehref,
                                        ptmp, &pitem->value);
        if (ret == NETWIB_ERR_OK && onlysupported) {
          netwib_er(netwox_url_decode_scheme(&pitem->value, &scheme, NULL));
          switch(scheme) {
          case NETWOX_URL_SCHEME_HTTP :
          case NETWOX_URL_SCHEME_FTP :
            break;
          default :
            ret = NETWOX_ERR_NOTSUPPORTED;
            break;
          }
        }
      }
      break;
    default :
      /* fake error to force tag suppression */
      ret = NETWIB_ERR_DATAEND;
    }
    /* on error, suppress tag */
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwib_ring_index_this_del(pringindex, NETWIB_TRUE));
    }
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &ptmp));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbasehref));
  netwib_er(netwib_ring_index_close(&pringindex));

  return(ret);
}
