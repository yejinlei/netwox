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
netwib_err netwox_bufarray_init(netwib_array *parray) {
  netwib_er(netwib_array_init(sizeof(netwib_buf), 0, parray));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_bufarray_close(netwib_array *parray) {
  netwib_buf *pitembuf;
  netwib_uint32 i;

  for (i = 0; i < parray->size; i++) {
    pitembuf = parray->p[i];
    netwib_er(netwib_buf_close(pitembuf));
  }

  netwib_er(netwib_array_close(parray));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_bufarray_add_buf(netwib_array *parray,
                                   netwib_constbuf *pbuf) {
  netwib_buf *pitembuf;

  netwib_er(netwib_array_ctl_set_size(parray, parray->size+1));
  pitembuf = parray->p[parray->size-1];
  netwib_er(netwib_buf_init_mallocdefault(pitembuf));
  netwib_er(netwib_buf_append_buf(pbuf, pitembuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_bufarray_contains(netwib_constarray *parray,
                                    netwib_constbuf *pbuf,
                                    netwib_bool *pyes) {
  netwib_buf *pitembuf;
  netwib_uint32 i;
  netwib_cmp cmp;

  *pyes = NETWIB_FALSE;
  for (i = 0; i < parray->size; i++) {
    pitembuf = parray->p[i];
    netwib_er(netwib_buf_cmp(pitembuf, pbuf, &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      *pyes = NETWIB_TRUE;
      break;
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_bufarray_check_strtxt(netwib_constarray *parray,
                                        netwib_conststring wantedtxt) {
  netwib_buf buf, *pitembuf;
  netwib_uint32 i;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_fmt(&buf, "[%{uint32}]", parray->size));
  for (i = 0; i < parray->size; i++) {
    pitembuf = parray->p[i];
    netwib_er(netwib_buf_append_fmt(&buf, " %{uint32}=>%{buf}", i, pitembuf));
  }
  netwib_er(netwox_buf_check_strtxt(&buf, wantedtxt));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_bufarray_add_extensions(netwib_array *parray,
                                          netwib_constbuf *pextensions) {
  netwib_data data, databegin;
  netwib_uint32 datasize;
  netwib_buf buf, buflc;

  /* allow several syntaxes in pextensions : spaces, commas, etc. */
  data = netwib__buf_ref_data_ptr(pextensions);
  datasize = netwib__buf_ref_data_size(pextensions);
  while (datasize) {
    databegin = data;
    while (datasize) {
      if (*databegin != ' ' && *databegin != ',' && *databegin != '.') {
        break;
      }
      databegin++;
      datasize--;
    }
    data = databegin;
    while (datasize) {
      if (*data == ' ' || *data == ',') {
        break;
      }
      data++;
      datasize--;
    }
    if (data - databegin) {
      netwib_er(netwib_buf_init_ext_arrayfilled(databegin, data - databegin,
                                                &buf));
      netwib_er(netwib_buf_init_mallocdefault(&buflc));
      netwib_er(netwib_buf_encode(&buf, NETWIB_ENCODETYPE_LOWERCASE, &buflc));
      netwib_er(netwox_bufarray_add_buf(parray, &buflc));
      netwib_er(netwib_buf_close(&buflc));
    }
  }

  return(NETWIB_ERR_OK);
}
