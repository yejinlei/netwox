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
netwib_err netwox_mimetypes_init(netwox_mimetypes *pmimetypes) {
  netwib_er(netwox_bufarraystore_hash_init(&pmimetypes->phash));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_mimetypes_close(netwox_mimetypes *pmimetypes) {
  netwib_er(netwox_bufarraystore_hash_close(&pmimetypes->phash));
  return(NETWIB_ERR_OK);

}

/*-------------------------------------------------------------*/
netwib_err netwox_mimetypes_add(netwox_mimetypes *pmimetypes,
                                netwib_constbuf *pmimetype,
                                netwib_constbuf *pextensions) {
  netwib_ptr pitem;
  netwib_array *parray;

  /* allow several syntaxes in pextensions : spaces, commas, etc. */
  netwib_er(netwox_bufarraystore_create(&pitem));
  parray = (netwib_array *)pitem;
  netwib_er(netwox_bufarray_add_extensions(parray, pextensions));
  if (parray->size == 0) {
    netwib_er(netwox_bufarraystore_erase(pitem));
    return(NETWIB_ERR_OK);
  }

  /* add the item in the hash */
  netwib_er(netwib_hash_add(pmimetypes->phash, pmimetype, pitem,
                            NETWIB_TRUE));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_mimetypes_display(netwox_mimetypes *pmimetypes) {
  netwib_hash_index *phashindex;
  netwib_buf key, *pitembuf;
  netwib_ptr pitem;
  netwib_array *parray;
  netwib_uint32 i;
  netwib_err ret;

  netwib_er(netwib_buf_init_mallocdefault(&key));
  netwib_er(netwib_hash_index_init(pmimetypes->phash, &phashindex));
  ret = NETWIB_ERR_OK;
  while(NETWIB_TRUE) {
    /* obtain entry */
    netwib__buf_reinit(&key);
    ret = netwib_hash_index_next(phashindex, &key, &pitem);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    /* display */
    netwib_er(netwib_fmt_display("%{buf}:", &key));
    parray = (netwib_array *)pitem;
    for (i = 0; i < parray->size; i++) {
      pitembuf = parray->p[i];
      netwib_er(netwib_fmt_display(" %{buf}", pitembuf));
    }
    netwib_er(netwib_fmt_display("\n"));
  }
  netwib_er(netwib_hash_index_close(&phashindex));
  netwib_er(netwib_buf_close(&key));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_mimetypes_search(netwox_mimetypes *pmimetypes,
                                   netwib_constbuf *pmimetype,
                                   netwib_array **pparray) {
  netwib_ptr pitem;
  netwib_err ret;

  ret = netwib_hash_value(pmimetypes->phash, pmimetype, &pitem);
  if (ret == NETWIB_ERR_OK) {
    *pparray = (netwib_array *)pitem;
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_mimetypes_contains_extension(netwox_mimetypes *pmimetypes,
                                               netwib_constbuf *pextension,
                                               netwib_bool *pyes) {
  netwib_hash_index *phashindex;
  netwib_buf key;
  netwib_ptr pitem;
  netwib_array *parray;
  netwib_bool yes;
  netwib_err ret;

  *pyes = NETWIB_FALSE;
  ret = NETWIB_ERR_OK;
  netwib_er(netwib_buf_init_mallocdefault(&key));
  netwib_er(netwib_hash_index_init(pmimetypes->phash, &phashindex));
  while(NETWIB_TRUE) {
    /* obtain entry */
    netwib__buf_reinit(&key);
    ret = netwib_hash_index_next(phashindex, &key, &pitem);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    /* search */
    parray = (netwib_array *)pitem;
    netwib_er(netwox_bufarray_contains(parray, pextension, &yes));
    if (yes) {
      *pyes = NETWIB_TRUE;
      break;
    }
  }
  netwib_er(netwib_hash_index_close(&phashindex));
  netwib_er(netwib_buf_close(&key));
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_mimetypes_test2(netwox_mimetypes *pmimetypes,
                                         netwib_conststring mimetype,
                                         netwib_conststring extensions) {
  netwib_buf bufmimetype, bufextensions;

  netwib_er(netwib_buf_init_ext_string(mimetype, &bufmimetype));
  netwib_er(netwib_buf_init_ext_string(extensions, &bufextensions));

  netwib_er(netwox_mimetypes_add(pmimetypes, &bufmimetype,
                                    &bufextensions));
  return(NETWIB_ERR_OK);
}
netwib_err netwox_mimetypes_test(void) {
  netwox_mimetypes mimetypes;
  netwib_buf bufmimetype, bufextension;
  netwib_array *parray;
  netwib_bool yes;
  netwib_err ret;

  netwib_er(netwox_mimetypes_init(&mimetypes));
  netwib_er(netwox_mimetypes_test2(&mimetypes, "text/html", " html, ,,HTM,, "));
  netwib_er(netwox_mimetypes_test2(&mimetypes, "image/gif", "gif"));
  netwib_er(netwox_mimetypes_display(&mimetypes));

  netwib_er(netwib_buf_init_ext_string("text/html", &bufmimetype));
  netwib_er(netwox_mimetypes_search(&mimetypes, &bufmimetype, &parray));
  netwib_er(netwib_buf_init_ext_string("text/ehtml", &bufmimetype));
  ret = netwox_mimetypes_search(&mimetypes, &bufmimetype, &parray);
  if (ret != NETWIB_ERR_NOTFOUND) {
    return(NETWIB_ERR_DATAOTHERTYPE);
  }

  netwib_er(netwib_buf_init_ext_string("htm", &bufextension));
  netwib_er(netwox_mimetypes_contains_extension(&mimetypes, &bufextension, &yes));
  if (!yes) {
    return(NETWIB_ERR_DATAOTHERTYPE);
  }
  netwib_er(netwib_buf_init_ext_string("htmm", &bufextension));
  netwib_er(netwox_mimetypes_contains_extension(&mimetypes, &bufextension, &yes));
  if (yes) {
    return(NETWIB_ERR_DATAOTHERTYPE);
  }

  netwib_er(netwox_mimetypes_close(&mimetypes));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_mimetype_init_contenttype(netwib_constbuf *pcontenttype,
                                            netwib_buf *pmimetype) {
  netwib_data datain, dataout;
  netwib_uint32 datainsize, dataoutsize;
  netwib_byte c;

  datainsize = netwib__buf_ref_data_size(pcontenttype);
  if (datainsize == 0) {
    return(NETWIB_ERR_OK);
  }

  datain = netwib__buf_ref_data_ptr(pcontenttype);
  /* skip first spaces */
  while (datainsize) {
    c = *datain;
    if (c != ' ' && c != '\t') {
      break;
    }
    datain++;
    datainsize--;
  }
  /* the mime type is before the optional ';' */
  netwib_er(netwib_buf_wantspace(pmimetype, datainsize, &dataout));
  dataoutsize = 0;
  while (datainsize) {
    c = *datain++;
    if (c == ';' || c == ' ' || c == '\t' || c == '\n' || c == '\r') {
      break;
    }
    *dataout++ = c;
    dataoutsize++;
    datainsize--;
  }
  pmimetype->endoffset += dataoutsize;

  return(NETWIB_ERR_OK);
}
