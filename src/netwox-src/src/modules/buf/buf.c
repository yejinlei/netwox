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
netwib_err netwox_buf_append_pad(netwib_byte b,
                                 netwib_uint32 size,
                                 netwib_buf *pbuf)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(pbuf, size, &data));
  netwib_c_memset(data, b, size);

  pbuf->endoffset += size;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_buf_append_xor(netwib_constbuf *pbuf1,
                                 netwib_constbuf *pbuf2,
                                 netwib_buf *pbuf)
{
  netwib_data data1, data2, data;
  netwib_uint32 size;

  size = netwib__buf_ref_data_size(pbuf1);
  if (size != netwib__buf_ref_data_size(pbuf2)) {
    return(NETWIB_ERR_PATOOLOW);
  }

  netwib_er(netwib_buf_wantspace(pbuf, size, &data));
  data1 = netwib__buf_ref_data_ptr(pbuf1);
  data2 = netwib__buf_ref_data_ptr(pbuf2);

  pbuf->endoffset += size;

  while(size--) {
    *data++ = (netwib_byte)((*data1++) ^ (*data2++));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_buf_lstrip(netwib_buf *pbuf)
{
  netwib_data data;
  netwib_uint32 datasize;

  datasize = netwib__buf_ref_data_size(pbuf);
  if (datasize == 0) {
    return(NETWIB_ERR_OK);
  }

  data = netwib__buf_ref_data_ptr(pbuf);
  while (datasize) {
    if (*data == ' ' || *data == '\t' || *data == 0x0D || *data == 0x0A) {
      data++;
      datasize--;
    } else {
      break;
    }
  }
  pbuf->beginoffset += netwib__buf_ref_data_size(pbuf) - datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_buf_rstrip(netwib_buf *pbuf)
{
  netwib_data data;
  netwib_uint32 datasize;

  datasize = netwib__buf_ref_data_size(pbuf);
  if (datasize == 0) {
    return(NETWIB_ERR_OK);
  }

  data = netwib__buf_ref_data_ptr(pbuf) + datasize - 1;
  while (datasize) {
    if (*data == ' ' || *data == '\t' || *data == 0x0D || *data == 0x0A) {
      data--;
      datasize--;
    } else {
      break;
    }
  }
  pbuf->endoffset -= netwib__buf_ref_data_size(pbuf) - datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_buf_strip(netwib_buf *pbuf)
{
  netwib_er(netwox_buf_rstrip(pbuf));
  netwib_er(netwox_buf_lstrip(pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_buf_lowercase(netwib_buf *pbuf) {
  netwib_data data;
  netwib_uint32 datasize;

  datasize = netwib__buf_ref_data_size(pbuf);
  if (datasize == 0) {
    return(NETWIB_ERR_OK);
  }

  data = netwib__buf_ref_data_ptr(pbuf);
  while (datasize) {
    *data = netwib_c2_lc(*data);
    data++;
    datasize--;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_buf_append_strmixed(netwib_conststring mixed,
                                      netwib_buf *pbuf)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_ext_string(mixed, &buf));
  netwib_er(netwib_buf_decode(&buf, NETWIB_DECODETYPE_MIXED, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_buf_check_strmixed(netwib_buf *pbuf,
                                     netwib_conststring wantedmixed)
{
  netwib_buf wanted, buf;
  netwib_cmp cmp;

  netwib_er(netwib_buf_init_mallocdefault(&wanted));
  netwib_er(netwib_buf_init_ext_string(wantedmixed, &buf));
  netwib_er(netwib_buf_decode(&buf, NETWIB_DECODETYPE_MIXED, &wanted));

  netwib_er(netwib_buf_cmp(pbuf, &wanted, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    netwib_er(netwib_fmt_display("Error:\n"));
    netwib_er(netwib_buf_init_mallocdefault(&buf));
    netwib_er(netwib_buf_encode(pbuf, NETWIB_ENCODETYPE_MIXED, &buf));
    netwib_er(netwib_fmt_display("  %{buf}\n", &buf));
    netwib_er(netwib_fmt_display("or:\n"));
    netwib__buf_reinit(&buf);
    netwib_er(netwib_buf_encode(pbuf, NETWIB_ENCODETYPE_HEXA0, &buf));
    netwib_er(netwib_fmt_display("  %{buf}\n", &buf));
    netwib_er(netwib_fmt_display("Different from:\n"));
    netwib_er(netwib_fmt_display("  %s\n", wantedmixed));
    netwib_er(netwib_buf_close(&wanted));
    netwib_er(netwib_buf_close(&buf));
    return(NETWOX_ERR_INTERNALERROR);
  } else {
    netwib_er(netwib_fmt_display("%s OK\n", wantedmixed));
  }

  netwib_er(netwib_buf_close(&wanted));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_buf_check_strtxt(netwib_buf *pbuf,
                                   netwib_conststring wantedtxt)
{
  netwib_buf buf;
  netwib_cmp cmp;

  netwib_er(netwib_buf_cmp_string(pbuf, wantedtxt, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    netwib_er(netwib_fmt_display("Error:\n"));
    netwib_er(netwib_buf_init_mallocdefault(&buf));
    netwib_er(netwib_buf_encode(pbuf, NETWIB_ENCODETYPE_MIXED, &buf));
    netwib_er(netwib_fmt_display("  %{buf}\n", &buf));
    netwib_er(netwib_fmt_display("or:\n"));
    netwib__buf_reinit(&buf);
    netwib_er(netwib_buf_encode(pbuf, NETWIB_ENCODETYPE_HEXA0, &buf));
    netwib_er(netwib_fmt_display("  %{buf}\n", &buf));
    netwib_er(netwib_fmt_display("Different from:\n"));
    netwib_er(netwib_fmt_display("  %s\n", wantedtxt));
    netwib_er(netwib_buf_close(&buf));
    return(NETWOX_ERR_INTERNALERROR);
  } else {
    netwib_er(netwib_fmt_display("%s OK\n", wantedtxt));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_buf_find(netwox_buf_findlist findlist,
                           netwib_bool findlistissorted,
                           netwib_bool casesensitive,
                           netwib_constbuf *pbuf,
                           netwib_uint32 *pui)
{
  netwib_data data;
  netwib_uint32 i;
  netwib_cmp cmp=NETWIB_CMP_LT;
  netwib_char c, cbuf, cprev;

  if (netwib__buf_ref_data_sizenull(pbuf) == 0) {
    cbuf = 0;
  } else {
    data = netwib__buf_ref_data_ptr(pbuf);
    cbuf = data[0];
    if (!casesensitive) {
      netwib_c2_upper(cbuf);
    }
  }

  i = 0;
  cprev = 0;
  while(findlist[i].str != NULL) {
    c = findlist[i].str[0];
    if (!casesensitive) {
      netwib_c2_upper(c);
    }
    if (findlistissorted && (c < cprev)) {
      return(NETWOX_ERR_INTERNALERROR);
    }
    if (c == cbuf) {
      if (casesensitive) {
        netwib_er(netwib_buf_cmp_string(pbuf, findlist[i].str, &cmp));
      } else {
        netwib_er(netwib_buf_casecmp_string(pbuf, findlist[i].str, &cmp));
      }
      if (cmp == NETWIB_CMP_EQ) {
        *pui = findlist[i].ui32;
        return(NETWIB_ERR_OK);
      }
    } else if (findlistissorted && (c > cbuf)) {
      break;
    }
    cprev = c;
    i++;
  }

  return(NETWIB_ERR_NOTFOUND);
}
