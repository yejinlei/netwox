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
static netwib_err netwox_priv_urllocal_validchars(netwib_constbuf *pbufin,
                                                  netwib_buf *pbufout) {
  netwib_data datain, dataout;
  netwib_uint32 datainsize;
  netwib_byte c;

  datainsize = netwib__buf_ref_data_size(pbufin);
  if (datainsize == 0) {
    return(NETWIB_ERR_OK);
  }

  datain = netwib__buf_ref_data_ptr(pbufin);
  netwib_er(netwib_buf_wantspace(pbufout, datainsize, &dataout));
  while (datainsize) {
    c = *datain++;
    netwib_c2_lower(c);
    if (netwib_c2_islower(c) || netwib_c2_isdigit(c) || c == '-' || c == '_') {
      *dataout++ = c;
    } else {
      *dataout++ = '_';
    }
    datainsize--;
  }
  pbufout->endoffset += netwib__buf_ref_data_size(pbufin);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_urllocal_append_ext(netwib_constbuf *pextension,
                                                  netwib_buf *pbuf) {
  netwib_er(netwib_buf_append_byte('_', pbuf));
  netwib_er(netwox_priv_urllocal_validchars(pextension, pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_urllocal_append_delta(netwib_constarray *pextarray,
                                                    netwib_uint32 i,
                                                    netwib_buf *pbuf) {
  netwib_buf *pitembuf0, *pitembufi;

  if (pextarray->size == 0) {
    return(NETWOX_ERR_INTERNALERROR);
  }
  if (i == 0) {
    /* no delta */
    return(NETWIB_ERR_OK);
  }
  if (i >= pextarray->size) {
    return(NETWOX_ERR_INTERNALERROR);
  }

  pitembuf0 = pextarray->p[0];
  pitembufi = pextarray->p[i];
  pitembuf0->endoffset = pitembuf0->beginoffset + netwib__buf_ref_data_size(pitembuf0) - netwib__buf_ref_data_size(pitembufi) - 1;
  netwib_er(netwib_buf_append_byte('_', pbuf));
  netwib_er(netwox_priv_urllocal_validchars(pitembuf0, pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_urllocal_encode_part1normal(netwib_constbuf *purl,
                                              netwib_buf *pfilename,
                                              netwib_array *pextarray,
                                              netwib_bool *pusemimetypes) {
  netwox_url_scheme scheme;
  netwib_buf authority, query, extension, filename;
  netwib_buf path, fragment;
  netwib_data data, pc;
  netwib_uint32 datasize;
  netwib_uint32 i1, i2;
  netwib_bool lastisdot;
  netwib_err ret;

  /* initialize */
  *pusemimetypes = NETWIB_TRUE;
  ret = NETWIB_ERR_OK;
  netwib_er(netwib_buf_init_mallocdefault(&path));
  netwib_er(netwib_buf_init_mallocdefault(&fragment));

  /* decode the url */
  netwib_eg(netwox_url_decode(purl, &scheme, &authority, &path,
                              &query, &fragment));

  /* find the filename (after last /) */
  data = netwib__buf_ref_data_ptr(&path);
  datasize = netwib__buf_ref_data_size(&path);
  while (NETWIB_TRUE) {
    pc = netwib_c_memchr(data, '/', datasize);
    if (pc == NULL) {
      break;
    }
    datasize -= pc - data + 1;
    data = pc + 1;
  }

  /* suppress multiple successive dots such as "..file..ext.."=>".file.ext." */
  if (datasize) {
    lastisdot = NETWIB_FALSE;
    i2 = 0;
    for (i1 = 0; i1 < datasize; i1++) {
      if (data[i1] == '.') {
        if (lastisdot) {
          continue;
        }
        lastisdot = NETWIB_TRUE;
      } else {
        lastisdot = NETWIB_FALSE;
      }
      data[i2] = data[i1];
      i2++;
    }
    datasize = i2;
  }

  /* suppress the ending dot */
  if (datasize) {
    if (data[datasize-1] == '.') {
      datasize--;
    }
  }

  /* a directory index */
  if (datasize == 0) {
    if (netwib__buf_ref_data_size(&query) == 0) {
      *pusemimetypes = NETWIB_FALSE;
      netwib_eg(netwib_buf_append_string("index", pfilename));
      netwib_eg(netwib_buf_init_ext_string("html", &extension));
      netwib_eg(netwox_bufarray_add_buf(pextarray, &extension));
      netwib_goto(NETWIB_ERR_OK);
    } else {
      netwib_eg(netwox_priv_urllocal_validchars(&query, pfilename));
      netwib_goto(NETWIB_ERR_OK);
    }
  }

  /* convert the leading dot (hidden filenames) */
  if (data[0] == '.') {
    data[0] = '_';
  }

  /* convert the filename to lowercase */
  netwib_eg(netwib_buf_init_ext_arrayfilled(data, datasize, &filename));
  netwib_eg(netwox_buf_lowercase(&filename));

  /* if there is a query, convert everything */
  if (netwib__buf_ref_data_size(&query) != 0) {
    netwib_eg(netwox_priv_urllocal_validchars(&filename, pfilename));
    netwib_eg(netwox_priv_urllocal_append_ext(&query, pfilename));
    netwib_goto(NETWIB_ERR_OK);
  }

  /* separate the filename to a filename and ... */
  pc = netwib_c_memchr(data, '.', datasize);
  if (pc == NULL) {
    netwib_eg(netwox_priv_urllocal_validchars(&filename, pfilename));
    netwib_goto(NETWIB_ERR_OK);
  } else {
    netwib_eg(netwib_buf_init_ext_arrayfilled(data, pc - data, &filename));
    netwib_eg(netwox_priv_urllocal_validchars(&filename, pfilename));
  }

  /* ... its extension[s] */
  while (datasize) {
    datasize -= pc - data + 1;
    data = pc + 1;
    netwib_eg(netwib_buf_init_ext_arrayfilled(data, datasize, &extension));
    netwib_eg(netwox_bufarray_add_buf(pextarray, &extension));
    pc = netwib_c_memchr(data, '.', datasize);
    if (pc == NULL) {
      break;
    }
  }

  /* end */
 netwib_gotolabel:
  netwib_er(netwib_buf_close(&fragment));
  netwib_er(netwib_buf_close(&path));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_urllocal_encode_part1(netwib_constbuf *purl,
                                        netwib_bool iserror,
                                        netwib_buf *pfilename,
                                        netwib_array *pextarray,
                                        netwib_bool *pusemimetypes) {
  netwib_array extarray;
  netwib_buf *pitembuf, *pitembuf0, bufhtml, bufhtm;
  netwib_uint32 i;
  netwib_cmp cmp;
  netwib_err ret;

  if (!iserror) {
    return(netwox_urllocal_encode_part1normal(purl, pfilename, pextarray,
                                               pusemimetypes));
  }

  /* initialize */
  ret = NETWIB_ERR_OK;
  netwib_er(netwox_bufarray_init(&extarray));

  /* main */
  netwib_eg(netwox_urllocal_encode_part1normal(purl, pfilename, &extarray,
                                                pusemimetypes));
  *pusemimetypes = NETWIB_FALSE;
  netwib_eg(netwib_buf_init_ext_string("html", &bufhtml));
  netwib_eg(netwib_buf_init_ext_string("htm", &bufhtm));
  for (i = 0; i < extarray.size; i++) {
    pitembuf = extarray.p[i];
    netwib_eg(netwib_buf_cmp(pitembuf, &bufhtml, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      netwib_eg(netwib_buf_cmp(pitembuf, &bufhtm, &cmp));
    }
    if (cmp == NETWIB_CMP_EQ) {
      netwib_eg(netwox_priv_urllocal_append_delta(&extarray, i, pfilename));
      netwib_eg(netwox_bufarray_add_buf(pextarray, pitembuf));
      netwib_goto(NETWIB_ERR_OK);
    }
  }
  if (extarray.size != 0) {
    pitembuf0 = extarray.p[0];
    netwib_eg(netwox_priv_urllocal_append_ext(pitembuf0, pfilename));
  }
  netwib_eg(netwox_bufarray_add_buf(pextarray, &bufhtml));

  /* end */
 netwib_gotolabel:
  netwib_er(netwox_bufarray_close(&extarray));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_urllocal_encode_part2(netwib_constbuf *purl,
                                        netwib_constbuf *pfilename,
                                        netwib_constarray *pextarray,
                                        netwib_bool usemimetypes,
                                        netwox_mimetypes *pmimetypes,
                                        netwib_bool mimetypeswarn,
                                        netwib_constarray *pdeletedextensions,
                                        netwib_constbuf *pcontenttype,
                                        netwib_buf *pfilename2,
                                        netwib_buf *pextension2) {
  netwib_buf *pitembuf0, *pitembuf, *pitembuf0mime, mimetype;
  netwib_array *pmimeextarray;
  netwib_bool yes;
  netwib_uint32 i;
  netwib_err ret;

  /* pfilename2 always starts with pfilename */
  netwib_er(netwib_buf_append_buf(pfilename, pfilename2));

  /* if do not use mime-type (directories indexes or errors) */
  if (!usemimetypes) {
    if (pextarray->size != 1) {
      return(NETWOX_ERR_INTERNALERROR);
    }
    pitembuf0 = pextarray->p[0];
    netwib_er(netwib_buf_append_buf(pitembuf0, pextension2));
    return(NETWIB_ERR_OK);
  }

  /* initialize */
  ret = NETWIB_ERR_OK;
  netwib_er(netwib_buf_init_mallocdefault(&mimetype));

  /* extract MIME type from Content-Type */
  netwib_eg(netwox_mimetype_init_contenttype(pcontenttype, &mimetype));

  /* there is no mime type */
  if (netwib__buf_ref_data_size(&mimetype) == 0) {
    if (pextarray->size == 0) {
      /* there is no extension, use html */
      netwib_eg(netwib_buf_append_string("html", pextension2));
      netwib_goto(NETWIB_ERR_OK);
    }
    /* search the extension in one of the allowed extensions */
    for (i = 0; i < pextarray->size; i++) {
      pitembuf = pextarray->p[i];
      netwib_eg(netwox_mimetypes_contains_extension(pmimetypes, pitembuf,
                                                    &yes));
      if (yes) {
        /* as the extension is in the allowed extensions, use this extension */
        netwib_eg(netwox_priv_urllocal_append_delta(pextarray, i, pfilename2));
        netwib_eg(netwib_buf_append_buf(pitembuf, pextension2));
        netwib_goto(NETWIB_ERR_OK);
      }
    }
    /* not found, so use html */
    for (i = 0; i < pextarray->size; i++) {
      pitembuf = pextarray->p[i];
      netwib_eg(netwox_bufarray_contains(pdeletedextensions, pitembuf, &yes));
      if (yes) {
        netwib_eg(netwox_priv_urllocal_append_delta(pextarray, i, pfilename2));
        netwib_eg(netwib_buf_append_string("html", pextension2));
        netwib_goto(NETWIB_ERR_OK);
      }
    }
    pitembuf0 = pextarray->p[0];
    netwib_eg(netwox_priv_urllocal_append_ext(pitembuf0, pfilename2));
    netwib_eg(netwib_buf_append_string("html", pextension2));
    if (mimetypeswarn) {
      netwib_fmt_display("MIME Warning: This is just a warning for the \"%{buf}\" url. I chose to use a \".html\" extension instead of \".%{buf}\" extension. You may want to add 'none = \"%{buf}, [other extensions]\"' in the 'mimetypes' block to keep the \".%{buf}\" extension. Instead, to suppress this warning, \"%{buf}\" can be added in 'deletedextensions' list.\n", purl, pitembuf0, pitembuf0, pitembuf0, pitembuf0);
    }
    netwib_goto(NETWIB_ERR_OK);
  }

  /* search MIME type */
  ret = netwox_mimetypes_search(pmimetypes, &mimetype, &pmimeextarray);
  if (ret != NETWIB_ERR_OK && ret != NETWIB_ERR_NOTFOUND) {
    netwib_goto(ret);
  }

  /* MIME type not found, so unknown */
  if (ret == NETWIB_ERR_NOTFOUND) {
    if (pextarray->size == 0) {
      /* there is no extension, use html */
      netwib_eg(netwib_buf_append_string("html", pextension2));
      if (mimetypeswarn) {
        netwib_fmt_display("MIME Warning: This is just a warning for the \"%{buf}\" url which uses the unknown \"%{buf}\" MIME type. I chose to use a \".html\" extension. You may want to add '%{buf} = \"list of valid extensions\"' in the 'mimetypes' block.\n", purl, &mimetype, &mimetype);
      }
      netwib_goto(NETWIB_ERR_OK);
    }
    /* use this extension */
    pitembuf0 = pextarray->p[0];
    netwib_eg(netwib_buf_append_buf(pitembuf0, pextension2));
    if (mimetypeswarn) {
      netwib_fmt_display("MIME Warning: This is just a warning for the \"%{buf}\" url which uses the unknown \"%{buf}\" MIME type. I chose to use a \".%{buf}\" extension. You may want to add '%{buf} = \"%{buf}\"' in the 'mimetypes' block if this is correct.\n", purl, &mimetype, pitembuf0, &mimetype, pitembuf0);
    }
    netwib_goto(NETWIB_ERR_OK);
  }

  /* MIME type is known */
  if (pextarray->size == 0) {
    /* there is no extension, use the first entry in the allowed
       extensions for the MIME type */
    if (pmimeextarray->size != 0) {
      pitembuf0mime = pmimeextarray->p[0];
      netwib_eg(netwib_buf_append_buf(pitembuf0mime, pextension2));
    }
    netwib_goto(NETWIB_ERR_OK);
  }
  /* if the extension is in the allowed extensions, use this extension */
  for (i = 0; i < pextarray->size; i++) {
    pitembuf = pextarray->p[i];
    netwib_eg(netwox_bufarray_contains(pmimeextarray, pitembuf, &yes));
    if (yes) {
      netwib_eg(netwox_priv_urllocal_append_delta(pextarray, i, pfilename2));
      netwib_eg(netwib_buf_append_buf(pitembuf, pextension2));
      netwib_goto(NETWIB_ERR_OK);
    }
  }
  /* use the first entry in the allowed extensions for the mime-type */
  if (pmimeextarray->size != 0) {
    pitembuf0mime = pmimeextarray->p[0];
    for (i = 0; i < pextarray->size; i++) {
      pitembuf = pextarray->p[i];
      netwib_eg(netwox_bufarray_contains(pdeletedextensions, pitembuf, &yes));
      if (yes) {
        netwib_eg(netwox_priv_urllocal_append_delta(pextarray, i, pfilename2));
        netwib_eg(netwib_buf_append_buf(pitembuf0mime, pextension2));
        netwib_goto(NETWIB_ERR_OK);
      }
    }
    pitembuf0 = pextarray->p[0];
    netwib_eg(netwox_priv_urllocal_append_ext(pitembuf0, pfilename2));
    netwib_eg(netwib_buf_append_buf(pitembuf0mime, pextension2));
    if (mimetypeswarn) {
      netwib_fmt_display("MIME Warning: This is just a warning for the \"%{buf}\" url which uses the \"%{buf}\" MIME type. I chose to use a \".%{buf}\" extension instead of \".%{buf}\" extension. You may want to add '%{buf} = \"%{buf}\"' in the 'mimetypes' block to keep the \".%{buf}\" extension. Instead, to suppress this warning, \"%{buf}\" can be added in 'deletedextensions' list.\n", purl, &mimetype, pitembuf0mime, pitembuf0, &mimetype, pitembuf0, pitembuf0, pitembuf0);
    }
    netwib_goto(NETWIB_ERR_OK);
  }

  /* end */
 netwib_gotolabel:
  netwib_er(netwib_buf_close(&mimetype));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_urllocal_encode_part3(netwib_constbuf *prootdir,
                                        netwib_uint64 filenumber,
                                        netwib_buf *plocaldir) {
  netwib_buf buf;
  netwib_uint64 tmp, mask, low64;
  netwib_uint32 low32, n;
  netwib_bool onedisplayed;
  netwib_err ret;

  ret = NETWIB_ERR_OK;
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_eg(netwib_buf_append_string("./files", &buf));
  netwib__uint64_init_uint32((1<<6)-1, mask);
  onedisplayed = NETWIB_FALSE;
  for (n = 60 ; n >= 6 ; n -= 6) {
    netwib__uint64_shr(filenumber, n, tmp);
    netwib__uint64_and(tmp, mask, low64);
    netwib__uint32_init_uint64(low64, low32);
    if (low32 || onedisplayed) {
      netwib_eg(netwib_buf_append_byte('/', &buf));
      netwib_eg(netwib_buf_append_byte(netwib_c2_16toc(low32>>4), &buf));
      netwib_eg(netwib_buf_append_byte(netwib_c2_16toc(low32&0xF), &buf));
      onedisplayed = NETWIB_TRUE;
    }
  }

  netwib_eg(netwib_path_init_concat(prootdir, &buf, plocaldir));

 netwib_gotolabel:
  netwib_er(netwib_buf_close(&buf));
  return(ret);
}

/*-------------------------------------------------------------*/
#define NETWOX_URLLOCAL_ENCODE_PART4_MAXTOTLEN 30
#define NETWOX_URLLOCAL_ENCODE_PART4_MAXEXTLEN 12
#define NETWOX_URLLOCAL_ENCODE_PART4_RANDLEN 4
static netwib_err netwox_urllocal_encode_part4_ok(netwib_constbuf *plocaldir,
                                                  netwib_constbuf *pfilename2,
                                                  netwib_constbuf *pextension2,
                                                  netwib_buf *pproposedlocalfilename,
                                                  netwib_bool *pok) {
  netwib_buf buf;
  netwib_uint32 size;
  netwib_cmp cmp;
  netwib_bool yes;

  if (netwib__buf_ref_data_size(pfilename2) == 0) {
    *pok = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  }

  size = netwib__buf_ref_data_size(pfilename2);
  if (netwib__buf_ref_data_size(pextension2) != 0) {
    size += 1+netwib__buf_ref_data_size(pextension2);
  }
  if (size > NETWOX_URLLOCAL_ENCODE_PART4_MAXTOTLEN) {
    *pok = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  }

  if (netwib__buf_ref_data_size(pextension2) == 0) {
    if (netwib__buf_ref_data_size(pfilename2) == 2) {
      netwib_data data;
      data = netwib__buf_ref_data_ptr(pfilename2);
      if (netwib_c2_isxdigit(data[0]) && netwib_c2_isxdigit(data[1])) {
        *pok = NETWIB_FALSE;
        return(NETWIB_ERR_OK);
      }
    }
  }

  netwib_er(netwib_buf_init_ext_string("trans", &buf));
  netwib_er(netwib_buf_cmp(pfilename2, &buf, &cmp));
  if (cmp == NETWIB_CMP_EQ) {
    netwib_er(netwib_buf_init_ext_string("tbl", &buf));
    netwib_er(netwib_buf_cmp(pextension2, &buf, &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      *pok = NETWIB_FALSE;
      return(NETWIB_ERR_OK);
    }
  }

  netwib_er(netwib_path_init_concat(plocaldir, pfilename2, pproposedlocalfilename));
  if (netwib__buf_ref_data_size(pextension2) != 0) {
    netwib_er(netwib_buf_append_byte('.', pproposedlocalfilename));
    netwib_er(netwib_buf_append_buf(pextension2, pproposedlocalfilename));
  }

  netwib_er(netwib_pathname_exists(pproposedlocalfilename, &yes));
  *pok = ! yes;

  return(NETWIB_ERR_OK);
}
netwib_err netwox_urllocal_encode_part4(netwib_constbuf *plocaldir,
                                        netwib_constbuf *pfilename2,
                                        netwib_constbuf *pextension2,
                                        netwib_buf *plocalfilename) {
  netwib_buf proposedfilename2, proposedextension2, proposedlocalfilename;
  netwib_uint32 maxsize;
  netwib_bool ok;
  netwib_err ret;

  ret = NETWIB_ERR_OK;
  netwib_er(netwib_buf_init_mallocdefault(&proposedfilename2));
  netwib_er(netwib_buf_init_mallocdefault(&proposedextension2));
  netwib_er(netwib_buf_init_mallocdefault(&proposedlocalfilename));

  /* check the simple case */
  netwib_eg(netwox_urllocal_encode_part4_ok(plocaldir, pfilename2, pextension2, &proposedlocalfilename, &ok));

  /* generate others */
  if (!ok) {
    netwib_eg(netwib_buf_append_buf(pextension2, &proposedextension2));
    if (netwib__buf_ref_data_size(pextension2) > NETWOX_URLLOCAL_ENCODE_PART4_MAXEXTLEN) {
      proposedextension2.endoffset = proposedextension2.beginoffset + NETWOX_URLLOCAL_ENCODE_PART4_MAXEXTLEN;
      netwib_eg(netwib_buf_append_string(".trunc", &proposedextension2));
    }
    while (!ok) {
      netwib__buf_reinit(&proposedfilename2);
      if (netwib__buf_ref_data_size(pfilename2)) {
        netwib_eg(netwib_buf_append_buf(pfilename2, &proposedfilename2));
        maxsize = NETWOX_URLLOCAL_ENCODE_PART4_MAXTOTLEN - (1+NETWOX_URLLOCAL_ENCODE_PART4_RANDLEN);
        if (netwib__buf_ref_data_size(&proposedextension2)) {
          maxsize -= 1+netwib__buf_ref_data_size(&proposedextension2);
        }
        if (netwib__buf_ref_data_size(&proposedfilename2) > maxsize) {
          proposedfilename2.endoffset = proposedfilename2.beginoffset + maxsize;
        }
        netwib_eg(netwib_buf_append_byte('_', &proposedfilename2));
      }
      netwib_eg(netwib_buf_append_rand(NETWOX_URLLOCAL_ENCODE_PART4_RANDLEN, 'a', 'z', &proposedfilename2));
      netwib__buf_reinit(&proposedlocalfilename);
      netwib_eg(netwox_urllocal_encode_part4_ok(plocaldir, &proposedfilename2, &proposedextension2, &proposedlocalfilename, &ok));
    }
  }

  /* send the valid one */
  netwib_eg(netwib_buf_append_buf(&proposedlocalfilename, plocalfilename));

 netwib_gotolabel:
  netwib_er(netwib_buf_close(&proposedlocalfilename));
  netwib_er(netwib_buf_close(&proposedextension2));
  netwib_er(netwib_buf_close(&proposedfilename2));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_urllocal_encode_part5(netwib_constbuf *pextension2,
                                        netwox_mimetypes *pmimetypes,
                                        netwib_bool *pcontainshtml) {
  netwib_buf buf;
  netwib_array *parray;
  netwib_cmp cmp;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string("html", &buf));
  netwib_er(netwib_buf_cmp(pextension2, &buf, &cmp));
  if (cmp == NETWIB_CMP_EQ) {
    *pcontainshtml = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_buf_init_ext_string("htm", &buf));
  netwib_er(netwib_buf_cmp(pextension2, &buf, &cmp));
  if (cmp == NETWIB_CMP_EQ) {
    *pcontainshtml = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_buf_init_ext_string(NETWOX_MIMETYPE_HTML, &buf));
  ret = netwox_mimetypes_search(pmimetypes, &buf, &parray);
  if (ret != NETWIB_ERR_OK) {
    *pcontainshtml = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwox_bufarray_contains(parray, pextension2, pcontainshtml));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_urllocal_encode(netwib_constbuf *prootdir,
                                  netwib_constbuf *purl,
                                  netwib_bool iserror,
                                  netwox_mimetypes *pmimetypes,
                                  netwib_bool mimetypeswarn,
                                  netwib_constarray *pdeletedextensions,
                                  netwib_constbuf *pcontenttype,
                                  netwib_uint64 filenumber,
                                  netwib_buf *plocalfilename,
                                  netwib_bool *pcontainshtml) {
  netwib_buf filename, filename2, extension2, localdir;
  netwib_array extarray;
  netwib_bool usemimetypes;
  netwib_err ret;

  ret = NETWIB_ERR_OK;
  netwib_er(netwib_buf_init_mallocdefault(&filename));
  netwib_er(netwib_buf_init_mallocdefault(&filename2));
  netwib_er(netwib_buf_init_mallocdefault(&extension2));
  netwib_er(netwib_buf_init_mallocdefault(&localdir));
  netwib_er(netwox_bufarray_init(&extarray));

  netwib_eg(netwox_urllocal_encode_part1(purl, iserror, &filename,
                                         &extarray, &usemimetypes));
  netwib_eg(netwox_urllocal_encode_part2(purl, &filename, &extarray,
                                         usemimetypes, pmimetypes,
                                         mimetypeswarn, pdeletedextensions,
                                         pcontenttype, &filename2,
                                         &extension2));
  netwib_eg(netwox_urllocal_encode_part3(prootdir, filenumber, &localdir));
  netwib_eg(netwox_urllocal_encode_part4(&localdir, &filename2, &extension2,
                                         plocalfilename));
  netwib_eg(netwox_urllocal_encode_part5(&extension2, pmimetypes,
                                         pcontainshtml));

 netwib_gotolabel:
  netwib_er(netwox_bufarray_close(&extarray));
  netwib_er(netwib_buf_close(&localdir));
  netwib_er(netwib_buf_close(&extension2));
  netwib_er(netwib_buf_close(&filename2));
  netwib_er(netwib_buf_close(&filename));
  return(ret);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err netwox_urllocal_encode_part2_test(netwib_constbuf *purl,
                                       netwib_constbuf *pfilename,
                                       netwib_constarray *pextarray,
                                       netwib_bool usemimetypes,
                                       netwox_mimetypes *pmimetypes,
                                       netwib_array *pdeletedextensions,
                                       netwib_conststring contenttype,
                                       netwib_conststring wantedfilename2,
                                       netwib_conststring wantedextension2) {
  netwib_buf bufcontenttype, buffilename2, bufextension2;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string(contenttype, &bufcontenttype));
  netwib_er(netwib_buf_init_mallocdefault(&buffilename2));
  netwib_er(netwib_buf_init_mallocdefault(&bufextension2));

  ret = NETWIB_ERR_OK;
  netwib_eg(netwox_urllocal_encode_part2(purl, pfilename, pextarray, usemimetypes, pmimetypes, NETWIB_TRUE, pdeletedextensions, &bufcontenttype, &buffilename2, &bufextension2));

  netwib_eg(netwox_buf_check_strtxt(&buffilename2, wantedfilename2));
  netwib_eg(netwox_buf_check_strtxt(&bufextension2, wantedextension2));

 netwib_gotolabel:
  netwib_er(netwib_buf_close(&bufextension2));
  netwib_er(netwib_buf_close(&buffilename2));
  return(ret);
}
static netwib_err netwox_urllocal_encode_part12_test(netwib_conststring url,
                                       netwib_bool iserror,
                                       netwib_conststring wantedfilename,
                                       netwib_conststring wantedextensions,
                                       netwib_bool wantedusemimetypes,
                                       netwox_mimetypes *pmimetypes,
                                       netwib_array *pdeletedextensions,
                                       netwib_conststring contenttype,
                                       netwib_conststring wantedfilename2,
                                       netwib_conststring wantedextension2) {
  netwib_buf bufurl, buffilename;
  netwib_array extarray;
  netwib_bool usemimetypes;
  netwib_err ret;

  ret = NETWIB_ERR_OK;
  netwib_er(netwib_fmt_display("\n%s\n", url));
  netwib_er(netwib_buf_init_ext_string(url, &bufurl));
  netwib_er(netwib_buf_init_mallocdefault(&buffilename));
  netwib_er(netwox_bufarray_init(&extarray));

  netwib_eg(netwox_urllocal_encode_part1(&bufurl, iserror, &buffilename, &extarray, &usemimetypes));
  netwib_eg(netwox_buf_check_strtxt(&buffilename, wantedfilename));
  netwib_eg(netwox_bufarray_check_strtxt(&extarray, wantedextensions));
  if (usemimetypes != wantedusemimetypes) {
    netwib_eg(netwib_fmt_display("%{bool} != %{bool}\n", usemimetypes, wantedusemimetypes));
    netwib_goto(NETWOX_ERR_INTERNALERROR);
  }

  if (netwib_c_strlen(wantedfilename2)) {
    netwib_eg(netwox_urllocal_encode_part2_test(&bufurl, &buffilename, &extarray, usemimetypes, pmimetypes, pdeletedextensions, contenttype, wantedfilename2, wantedextension2));
  }

 netwib_gotolabel:
  netwib_er(netwox_bufarray_close(&extarray));
  netwib_er(netwib_buf_close(&buffilename));
  return(ret);
}
static netwib_err netwox_urllocal_encode_part3_test(netwib_uint32 filenumber,
                                                    netwib_conststring wantedlocaldir) {
  netwib_buf bufrootdir, buflocaldir;
  netwib_uint64 filenumber64;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string("rootdir", &bufrootdir));
  netwib_er(netwib_buf_init_mallocdefault(&buflocaldir));

  ret = NETWIB_ERR_OK;
  netwib__uint64_init_uint32(filenumber, filenumber64);
  netwib_eg(netwox_urllocal_encode_part3(&bufrootdir, filenumber64, &buflocaldir));
  netwib_eg(netwox_buf_check_strtxt(&buflocaldir, wantedlocaldir));

 netwib_gotolabel:
  netwib_er(netwib_buf_close(&buflocaldir));
  netwib_er(netwib_buf_close(&bufrootdir));
  return(ret);
}
static netwib_err netwox_urllocal_encode_part4_test(netwib_conststring filename2,
                                                    netwib_conststring extension2) {
  netwib_buf buflocaldir, buffilename2, bufextension2, buflocalfilename;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string("localdir", &buflocaldir));
  netwib_er(netwib_buf_init_ext_string(filename2, &buffilename2));
  netwib_er(netwib_buf_init_ext_string(extension2, &bufextension2));
  netwib_er(netwib_buf_init_mallocdefault(&buflocalfilename));

  ret = NETWIB_ERR_OK;
  netwib_eg(netwox_urllocal_encode_part4(&buflocaldir, &buffilename2, &bufextension2, &buflocalfilename));
  netwib_eg(netwib_fmt_display("%s %s => %{buf}\n", filename2, extension2, &buflocalfilename));

 netwib_gotolabel:
  netwib_er(netwib_buf_close(&buflocalfilename));
  return(ret);
}
static netwib_err netwox_urllocal_encode_part5_test(netwib_conststring extension2,
                                                    netwox_mimetypes *pmimetypes,
                                                    netwib_bool wantedcontainshtml) {
  netwib_buf bufextension2;
  netwib_bool containshtml;

  netwib_er(netwib_fmt_display("%s\n", extension2));
  netwib_er(netwib_buf_init_ext_string(extension2, &bufextension2));

  netwib_er(netwox_urllocal_encode_part5(&bufextension2, pmimetypes, &containshtml));
  if (containshtml != wantedcontainshtml) {
    netwib_er(netwib_fmt_display("%{bool} != %{bool}\n", containshtml, wantedcontainshtml));
    return(NETWOX_ERR_INTERNALERROR);
  }

  return(NETWIB_ERR_OK);
}
netwib_err netwox_urllocal_test(void) {
  netwox_mimetypes mimetypes;
  netwib_buf bufmimetype, bufextensions;
  netwib_array deletedextensions;
  netwib_err ret;

  ret = NETWIB_ERR_OK;
  netwib_er(netwox_mimetypes_init(&mimetypes));
  netwib_er(netwib_buf_init_ext_string("text/html", &bufmimetype));
  netwib_er(netwib_buf_init_ext_string("html,htm,hhttmmll", &bufextensions));
  netwib_er(netwox_mimetypes_add(&mimetypes, &bufmimetype, &bufextensions));
  netwib_er(netwib_buf_init_ext_string("image/gif", &bufmimetype));
  netwib_er(netwib_buf_init_ext_string("gif", &bufextensions));
  netwib_er(netwox_mimetypes_add(&mimetypes, &bufmimetype, &bufextensions));
  netwib_er(netwib_buf_init_ext_string("image/jpeg", &bufmimetype));
  netwib_er(netwib_buf_init_ext_string("jpeg,jpg", &bufextensions));
  netwib_er(netwox_mimetypes_add(&mimetypes, &bufmimetype, &bufextensions));
  netwib_er(netwib_buf_init_ext_string("octet/1", &bufmimetype));
  netwib_er(netwib_buf_init_ext_string("b", &bufextensions));
  netwib_er(netwox_mimetypes_add(&mimetypes, &bufmimetype, &bufextensions));
  netwib_er(netwib_buf_init_ext_string("octet/2", &bufmimetype));
  netwib_er(netwib_buf_init_ext_string("c.d", &bufextensions));
  netwib_er(netwox_mimetypes_add(&mimetypes, &bufmimetype, &bufextensions));
  netwib_er(netwib_buf_init_ext_string("none", &bufmimetype));
  netwib_er(netwib_buf_init_ext_string("tar.gz,dat", &bufextensions));
  netwib_er(netwox_mimetypes_add(&mimetypes, &bufmimetype, &bufextensions));

  netwib_er(netwox_bufarray_init(&deletedextensions));
  netwib_er(netwib_buf_init_ext_string("del1,d1.u", &bufextensions));
  netwib_er(netwox_bufarray_add_extensions(&deletedextensions, &bufextensions));

  /* normal, without test for mime type */
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file", NETWIB_FALSE, "file", "[0]", NETWIB_TRUE, &mimetypes, &deletedextensions, "", "", ""));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/FILE.jPg", NETWIB_FALSE, "file", "[1] 0=>jpg", NETWIB_TRUE, &mimetypes, &deletedextensions, "", "", ""));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/F>L=<E.jPg", NETWIB_FALSE, "f_l__e", "[1] 0=>jpg", NETWIB_TRUE, &mimetypes, &deletedextensions, "", "", ""));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file.tar.gz", NETWIB_FALSE, "file", "[2] 0=>tar.gz 1=>gz", NETWIB_TRUE, &mimetypes, &deletedextensions, "", "", ""));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/f.php?v=a", NETWIB_FALSE, "f_php_v_a", "[0]", NETWIB_TRUE, &mimetypes, &deletedextensions, "", "", ""));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/", NETWIB_FALSE, "index", "[1] 0=>html", NETWIB_FALSE, &mimetypes, &deletedextensions, "", "", ""));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/?v=a", NETWIB_FALSE, "v_a", "[0]", NETWIB_TRUE, &mimetypes, &deletedextensions, "", "", ""));
  /* same as above, but validate special cases */
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/.hidden", NETWIB_FALSE, "_hidden", "[0]", NETWIB_TRUE, &mimetypes, &deletedextensions, "", "", ""));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/.hidden.txt", NETWIB_FALSE, "_hidden", "[1] 0=>txt", NETWIB_TRUE, &mimetypes, &deletedextensions, "", "", ""));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file..jpg", NETWIB_FALSE, "file", "[1] 0=>jpg", NETWIB_TRUE, &mimetypes, &deletedextensions, "", "", ""));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file.....jpg", NETWIB_FALSE, "file", "[1] 0=>jpg", NETWIB_TRUE, &mimetypes, &deletedextensions, "", "", ""));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file.e..jpg", NETWIB_FALSE, "file", "[2] 0=>e.jpg 1=>jpg", NETWIB_TRUE, &mimetypes, &deletedextensions, "", "", ""));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file.", NETWIB_FALSE, "file", "[0]", NETWIB_TRUE, &mimetypes, &deletedextensions, "", "", ""));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file....", NETWIB_FALSE, "file", "[0]", NETWIB_TRUE, &mimetypes, &deletedextensions, "", "", ""));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file.jpg.", NETWIB_FALSE, "file", "[1] 0=>jpg", NETWIB_TRUE, &mimetypes, &deletedextensions, "", "", ""));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/.", NETWIB_FALSE, "index", "[1] 0=>html", NETWIB_FALSE, &mimetypes, &deletedextensions, "", "", ""));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/..", NETWIB_FALSE, "index", "[1] 0=>html", NETWIB_FALSE, &mimetypes, &deletedextensions, "", "", ""));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/..file..ext..", NETWIB_FALSE, "_file", "[1] 0=>ext", NETWIB_TRUE, &mimetypes, &deletedextensions, "", "", ""));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/f.php?V=A", NETWIB_FALSE, "f_php_v_a", "[0]", NETWIB_TRUE, &mimetypes, &deletedextensions, "", "", ""));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/f.php?V=A#foo", NETWIB_FALSE, "f_php_v_a", "[0]", NETWIB_TRUE, &mimetypes, &deletedextensions, "", "", ""));
  /* error, with test for mime type */
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file", NETWIB_TRUE, "file", "[1] 0=>html", NETWIB_FALSE, &mimetypes, &deletedextensions, "image/gif", "file", "html"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/FILE.jPg", NETWIB_TRUE, "file_jpg", "[1] 0=>html", NETWIB_FALSE, &mimetypes, &deletedextensions, "image/gif", "file_jpg", "html"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/FILE.htm", NETWIB_TRUE, "file", "[1] 0=>htm", NETWIB_FALSE, &mimetypes, &deletedextensions, "image/gif", "file", "htm"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/FILE.r.html", NETWIB_TRUE, "file_r", "[1] 0=>html", NETWIB_FALSE, &mimetypes, &deletedextensions, "image/gif", "file_r", "html"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/FILE.r.s.html", NETWIB_TRUE, "file_r_s", "[1] 0=>html", NETWIB_FALSE, &mimetypes, &deletedextensions, "image/gif", "file_r_s", "html"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/F>L=<E.jPg", NETWIB_TRUE, "f_l__e_jpg", "[1] 0=>html", NETWIB_FALSE, &mimetypes, &deletedextensions, "image/gif", "f_l__e_jpg", "html"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file.tar.gz", NETWIB_TRUE, "file_tar_gz", "[1] 0=>html", NETWIB_FALSE, &mimetypes, &deletedextensions, "image/gif", "file_tar_gz", "html"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/f.php?v=a", NETWIB_TRUE, "f_php_v_a", "[1] 0=>html", NETWIB_FALSE, &mimetypes, &deletedextensions, "image/gif", "f_php_v_a", "html"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/", NETWIB_TRUE, "index", "[1] 0=>html", NETWIB_FALSE, &mimetypes, &deletedextensions, "image/gif", "index", "html"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/?v=a", NETWIB_TRUE, "v_a", "[1] 0=>html", NETWIB_FALSE, &mimetypes, &deletedextensions, "image/gif", "v_a", "html"));
  /* normal, with test for mime type */
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file", NETWIB_FALSE, "file", "[0]", NETWIB_TRUE, &mimetypes, &deletedextensions, "", "file", "html"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file.jpg", NETWIB_FALSE, "file", "[1] 0=>jpg", NETWIB_TRUE, &mimetypes, &deletedextensions, "", "file", "jpg"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file.a.b", NETWIB_FALSE, "file", "[2] 0=>a.b 1=>b", NETWIB_TRUE, &mimetypes, &deletedextensions, "", "file_a", "b"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file.a.b.c.d", NETWIB_FALSE, "file", "[4] 0=>a.b.c.d 1=>b.c.d 2=>c.d 3=>d", NETWIB_TRUE, &mimetypes, &deletedextensions, "", "file_a_b", "c.d"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file.boo", NETWIB_FALSE, "file", "[1] 0=>boo", NETWIB_TRUE, &mimetypes, &deletedextensions, "", "file_boo", "html"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file.a.boo", NETWIB_FALSE, "file", "[2] 0=>a.boo 1=>boo", NETWIB_TRUE, &mimetypes, &deletedextensions, "", "file_a_boo", "html"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file.del1", NETWIB_FALSE, "file", "[1] 0=>del1", NETWIB_TRUE, &mimetypes, &deletedextensions, "", "file", "html"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file.a.del1", NETWIB_FALSE, "file", "[2] 0=>a.del1 1=>del1", NETWIB_TRUE, &mimetypes, &deletedextensions, "", "file_a", "html"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file.a.d1.u", NETWIB_FALSE, "file", "[3] 0=>a.d1.u 1=>d1.u 2=>u", NETWIB_TRUE, &mimetypes, &deletedextensions, "", "file_a", "html"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file", NETWIB_FALSE, "file", "[0]", NETWIB_TRUE, &mimetypes, &deletedextensions, "i/foo", "file", "html"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file", NETWIB_FALSE, "file", "[0]", NETWIB_TRUE, &mimetypes, &deletedextensions, "i/foo ; name=ee", "file", "html"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file.jpg", NETWIB_FALSE, "file", "[1] 0=>jpg", NETWIB_TRUE, &mimetypes, &deletedextensions, "i/foo", "file", "jpg"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file.a.b", NETWIB_FALSE, "file", "[2] 0=>a.b 1=>b", NETWIB_TRUE, &mimetypes, &deletedextensions, "i/foo", "file", "a.b"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file.a.b.c.d", NETWIB_FALSE, "file", "[4] 0=>a.b.c.d 1=>b.c.d 2=>c.d 3=>d", NETWIB_TRUE, &mimetypes, &deletedextensions, "i/foo", "file", "a.b.c.d"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file", NETWIB_FALSE, "file", "[0]", NETWIB_TRUE, &mimetypes, &deletedextensions, "image/jpeg", "file", "jpeg"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file.jpg", NETWIB_FALSE, "file", "[1] 0=>jpg", NETWIB_TRUE, &mimetypes, &deletedextensions, "image/jpeg", "file", "jpg"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file.hh", NETWIB_FALSE, "file", "[1] 0=>hh", NETWIB_TRUE, &mimetypes, &deletedextensions, "image/jpeg", "file_hh", "jpeg"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file.del1", NETWIB_FALSE, "file", "[1] 0=>del1", NETWIB_TRUE, &mimetypes, &deletedextensions, "image/jpeg", "file", "jpeg"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file.a.del1", NETWIB_FALSE, "file", "[2] 0=>a.del1 1=>del1", NETWIB_TRUE, &mimetypes, &deletedextensions, "image/jpeg", "file_a", "jpeg"));
  netwib_eg(netwox_urllocal_encode_part12_test("http://server/dir/file.a.d1.u", NETWIB_FALSE, "file", "[3] 0=>a.d1.u 1=>d1.u 2=>u", NETWIB_TRUE, &mimetypes, &deletedextensions, "image/jpeg", "file_a", "jpeg"));

  /**/
  netwib_eg(netwox_urllocal_encode_part3_test(0, "rootdir/files"));
  netwib_eg(netwox_urllocal_encode_part3_test(62, "rootdir/files"));
  netwib_eg(netwox_urllocal_encode_part3_test(63, "rootdir/files"));
  netwib_eg(netwox_urllocal_encode_part3_test(64, "rootdir/files/01"));
  netwib_eg(netwox_urllocal_encode_part3_test(126, "rootdir/files/01"));
  netwib_eg(netwox_urllocal_encode_part3_test(127, "rootdir/files/01"));
  netwib_eg(netwox_urllocal_encode_part3_test(128, "rootdir/files/02"));
  netwib_eg(netwox_urllocal_encode_part3_test(191, "rootdir/files/02"));
  netwib_eg(netwox_urllocal_encode_part3_test(192, "rootdir/files/03"));
  netwib_eg(netwox_urllocal_encode_part3_test(4032, "rootdir/files/3f"));
  netwib_eg(netwox_urllocal_encode_part3_test(4095, "rootdir/files/3f"));
  netwib_eg(netwox_urllocal_encode_part3_test(4096, "rootdir/files/01/00"));
  netwib_eg(netwox_urllocal_encode_part3_test(4097, "rootdir/files/01/00"));
  netwib_eg(netwox_urllocal_encode_part3_test(4159, "rootdir/files/01/00"));
  netwib_eg(netwox_urllocal_encode_part3_test(4160, "rootdir/files/01/01"));
  netwib_eg(netwox_urllocal_encode_part3_test(4167, "rootdir/files/01/01"));
  netwib_eg(netwox_urllocal_encode_part3_test(500000, "rootdir/files/01/3a/04"));
  netwib_eg(netwox_urllocal_encode_part3_test(500070, "rootdir/files/01/3a/05"));

  /**/
  netwib_eg(netwox_urllocal_encode_part4_test("f", "c"));
  netwib_eg(netwox_urllocal_encode_part4_test("", "c"));
  netwib_eg(netwox_urllocal_encode_part4_test("0123456789012345678901234567", "c"));
  netwib_eg(netwox_urllocal_encode_part4_test("01234567890123456789012345678", "c"));
  netwib_eg(netwox_urllocal_encode_part4_test("012345678901234567890123456789", ""));
  netwib_eg(netwox_urllocal_encode_part4_test("0123456789012345678901234567890", ""));
  netwib_eg(netwox_urllocal_encode_part4_test("ff", "c"));
  netwib_eg(netwox_urllocal_encode_part4_test("ff", ""));
  netwib_eg(netwox_urllocal_encode_part4_test("trans", "tbl"));
  netwib_eg(netwox_urllocal_encode_part4_test("", "012345678901"));
  netwib_eg(netwox_urllocal_encode_part4_test("", "0123456789012"));
  netwib_eg(netwox_urllocal_encode_part4_test("0123456789012345678901234567890", "012345678901"));
  netwib_eg(netwox_urllocal_encode_part4_test("0123456789012345678901234567890", "0123456789012"));

  /**/
  netwib_eg(netwox_urllocal_encode_part5_test("ff", &mimetypes, NETWIB_FALSE));
  netwib_eg(netwox_urllocal_encode_part5_test("htm", &mimetypes, NETWIB_TRUE));
  netwib_eg(netwox_urllocal_encode_part5_test("html", &mimetypes, NETWIB_TRUE));
  netwib_eg(netwox_urllocal_encode_part5_test("hhttmmll", &mimetypes, NETWIB_TRUE));

 netwib_gotolabel:
  netwib_er(netwox_bufarray_close(&deletedextensions));
  netwib_er(netwox_mimetypes_close(&mimetypes));
  return(ret);
}

