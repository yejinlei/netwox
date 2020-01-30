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
netwib_err netwib_show_array_head(netwib_conststring title,
                                  netwib_buf *pbuf)
{
  if (title == NULL) {
    netwib_er(netwib_buf_append_string("._______________________________________________________________.\n", pbuf));
  } else {
    netwib_er(netwib_buf_append_fmt(pbuf, "%{l_64;s}.\n", title));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_show_array_tail(netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_string("|_______________________________________________________________|\n", pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_show_array_line_begin(netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_byte('|', pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_show_array_line_end(netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_byte('\n', pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_show_array_text(netwib_uint32 size,
                                  netwib_conststring txt,
                                  netwib_buf *pbuf)
{
  netwib_byte array[80];
  netwib_buf buf;
  netwib_string str;
  netwib_uint32 totalsize;

  /* common cases first */
  switch (size) {
  case 32 :
    netwib_er(netwib_buf_append_fmt(pbuf, "|%{c 63;s}|\n", txt));
    break;
  case 16 :
    netwib_er(netwib_buf_append_fmt(pbuf, "%{c 31;s}|", txt));
    break;
  case 8 :
    netwib_er(netwib_buf_append_fmt(pbuf, "%{c 15;s}|", txt));
    break;
  case 4 :
    netwib_er(netwib_buf_append_fmt(pbuf, "%{c 7;s}|", txt));
    break;
  case 1 :
    if (txt[0] == 0) {
      netwib_er(netwib_buf_append_byte(' ', pbuf));
    } else {
      netwib_er(netwib_buf_append_string(txt, pbuf));
    }
      netwib_er(netwib_buf_append_byte('|', pbuf));
    break;
  default :
    totalsize = 2*size - 1;
    netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));
    netwib_er(netwib_buf_append_fmt(&buf, "%%{c %{uint32};s}|", totalsize));
    netwib_er(netwib_buf_ref_string(&buf, &str));
    netwib_er(netwib_buf_append_fmt(pbuf, str, txt));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_show_array_num(netwib_uint32 size,
                                 netwib_uint32 ui,
                                 netwib_show_array_numtype type,
                                 netwib_buf *pbuf)
{
  netwib_byte array[80], array2[80];
  netwib_buf buf, buf2;
  netwib_string str;
  netwib_byte cstr[4];
  netwib_uint32 totalsize, binsize, hexasize;

  totalsize = 2*size - 1;
  binsize = size;
  hexasize = (binsize+3)/4;

  netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));

  switch(type) {
    case NETWIB_SHOW_ARRAY_NUMTYPE_CHAR :
    case NETWIB_SHOW_ARRAY_NUMTYPE_DECCHAR :
    case NETWIB_SHOW_ARRAY_NUMTYPE_HEXACHAR :
    case NETWIB_SHOW_ARRAY_NUMTYPE_HEXADECCHAR :
      cstr[0] = '\0';
      if (!netwib_c2_isprint(ui)) {
        switch(type) {
        case NETWIB_SHOW_ARRAY_NUMTYPE_CHAR :
          type = NETWIB_SHOW_ARRAY_NUMTYPE_HEXA;
          break;
        case NETWIB_SHOW_ARRAY_NUMTYPE_DECCHAR :
          type = NETWIB_SHOW_ARRAY_NUMTYPE_DEC;
          break;
        case NETWIB_SHOW_ARRAY_NUMTYPE_HEXACHAR :
          type = NETWIB_SHOW_ARRAY_NUMTYPE_HEXA;
          break;
        case NETWIB_SHOW_ARRAY_NUMTYPE_HEXADECCHAR :
          type = NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC;
          break;
        default:
          break;
        }
      } else {
        cstr[0] = '\'';
        cstr[1] = (netwib_byte)ui;
        cstr[2] = '\'';
        cstr[3] = '\0';
      }
      break;
  default :
    break;
  }

  switch(type) {
  case NETWIB_SHOW_ARRAY_NUMTYPE_BIN :
    switch (size) {
    case 32 :
      netwib_er(netwib_buf_append_fmt(pbuf, "|%{c_63;uint32:#32b}|\n", ui));
      break;
    case 16 :
      netwib_er(netwib_buf_append_fmt(pbuf, "%{c_31;uint32:#16b}|", ui));
      break;
    case 8 :
      netwib_er(netwib_buf_append_fmt(pbuf, "%{c_15;uint32:#8b}|", ui));
      break;
    case 4 :
      netwib_er(netwib_buf_append_fmt(pbuf, "%{c_7;uint32:#4b}|", ui));
      break;
    case 1 :
      if (ui) {
        netwib_er(netwib_buf_append_string("1|", pbuf));
      } else {
        netwib_er(netwib_buf_append_string("0|", pbuf));
      }
      break;
    default :
      netwib_er(netwib_buf_append_fmt(&buf,
                                      "%%{c_%{uint32};uint32:#%{uint32}b}|",
                                      totalsize, binsize));
      netwib_er(netwib_buf_ref_string(&buf, &str));
      netwib_er(netwib_buf_append_fmt(pbuf, str, ui));
      break;
    }
    break;
  case NETWIB_SHOW_ARRAY_NUMTYPE_DEC :
    switch (size) {
    case 32 :
      netwib_er(netwib_buf_append_fmt(pbuf, "|%{c_63;uint32}|\n", ui));
      break;
    case 16 :
      netwib_er(netwib_buf_append_fmt(pbuf, "%{c_31;uint32}|", ui));
      break;
    case 8 :
      netwib_er(netwib_buf_append_fmt(pbuf, "%{c_15;uint32}|", ui));
      break;
    case 4 :
      netwib_er(netwib_buf_append_fmt(pbuf, "%{c_7;uint32}|", ui));
      break;
    case 1 :
      netwib_er(netwib_buf_append_fmt(pbuf, "%{c_1;uint32}|", ui));
      break;
    default :
      netwib_er(netwib_buf_append_fmt(&buf, "%%{c_%{uint32};uint32}|",
                                      totalsize));
      netwib_er(netwib_buf_ref_string(&buf, &str));
      netwib_er(netwib_buf_append_fmt(pbuf, str, ui));
      break;
    }
    break;
  case NETWIB_SHOW_ARRAY_NUMTYPE_HEXA :
    switch (size) {
    case 32 :
      netwib_er(netwib_buf_append_fmt(pbuf, "|%{c_63;uint32:#08X}|\n", ui));
      break;
    case 16 :
      netwib_er(netwib_buf_append_fmt(pbuf, "%{c_31;uint32:#04X}|", ui));
      break;
    case 8 :
      netwib_er(netwib_buf_append_fmt(pbuf, "%{c_15;uint32:#02X}|", ui));
      break;
    case 4 :
      netwib_er(netwib_buf_append_fmt(pbuf, "%{c_7;uint32:#01X}|", ui));
      break;
    case 1 :
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
    default :
      netwib_er(netwib_buf_append_fmt(&buf,
                                      "%%{c_%{uint32};uint32:#0%{uint32}X}|",
                                      totalsize, hexasize));
      netwib_er(netwib_buf_ref_string(&buf, &str));
      netwib_er(netwib_buf_append_fmt(pbuf, str, ui));
      break;
    }
    break;
  case NETWIB_SHOW_ARRAY_NUMTYPE_CHAR :
    switch (size) {
    case 32 :
      netwib_er(netwib_buf_append_fmt(pbuf, "|%{c_63;s}|\n", cstr));
      break;
    case 16 :
      netwib_er(netwib_buf_append_fmt(pbuf, "%{c_31;s}|", cstr));
      break;
    case 8 :
      netwib_er(netwib_buf_append_fmt(pbuf, "%{c_15;s}|", cstr));
      break;
    case 4 :
      netwib_er(netwib_buf_append_fmt(pbuf, "%{c_7;s}|", cstr));
      break;
    case 1 :
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
    default :
      netwib_er(netwib_buf_append_fmt(&buf, "%%{c_%{uint32};s}|", totalsize));
      netwib_er(netwib_buf_ref_string(&buf, &str));
      netwib_er(netwib_buf_append_fmt(pbuf, str, cstr));
      break;
    }
    break;
  case NETWIB_SHOW_ARRAY_NUMTYPE_DECCHAR :
    netwib_er(netwib_buf_append_fmt(&buf, "%{uint32}=%s", ui, cstr));
    switch (size) {
    case 32 :
      netwib_er(netwib_buf_append_fmt(pbuf, "|%{c_63;buf}|\n", &buf));
      break;
    case 16 :
      netwib_er(netwib_buf_append_fmt(pbuf, "%{c_31;buf}|", &buf));
      break;
    case 8 :
      netwib_er(netwib_buf_append_fmt(pbuf, "%{c_15;buf}|", &buf));
      break;
    case 4 :
      netwib_er(netwib_buf_append_fmt(pbuf, "%{c_7;buf}|", &buf));
      break;
    case 1 :
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
    default :
      netwib_er(netwib_buf_init_ext_arraysizeofempty(array2, &buf2));
      netwib_er(netwib_buf_append_fmt(&buf2, "%%{c_%{uint32};buf}|",
                                      totalsize));
      netwib_er(netwib_buf_ref_string(&buf2, &str));
      netwib_er(netwib_buf_append_fmt(pbuf, str, &buf));
      break;
    }
    break;
  case NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC :
    switch (size) {
    case 32 :
      netwib_er(netwib_buf_append_fmt(&buf, "%{uint32:#08X}=%{uint32}",ui,ui));
      netwib_er(netwib_buf_append_fmt(pbuf, "|%{c_63;buf}|\n", &buf));
      break;
    case 16 :
      netwib_er(netwib_buf_append_fmt(&buf, "%{uint32:#04X}=%{uint32}",ui,ui));
      netwib_er(netwib_buf_append_fmt(pbuf, "%{c_31;buf}|", &buf));
      break;
    case 8 :
      netwib_er(netwib_buf_append_fmt(&buf, "%{uint32:#02X}=%{uint32}",ui,ui));
      netwib_er(netwib_buf_append_fmt(pbuf, "%{c_15;buf}|", &buf));
      break;
    case 4 :
      netwib_er(netwib_buf_append_fmt(&buf, "%{uint32:#01X}=%{uint32}",ui,ui));
      netwib_er(netwib_buf_append_fmt(pbuf, "%{c_7;buf}|", &buf));
      break;
    case 1 :
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
    default :
      netwib_er(netwib_buf_append_fmt(&buf,
                                      "%%{uint32:#0%{uint32}X}=%%{uint32}",
                                      hexasize));
      netwib_er(netwib_buf_ref_string(&buf, &str));
      netwib_er(netwib_buf_init_ext_arraysizeofempty(array2, &buf2));
      netwib_er(netwib_buf_append_fmt(&buf2, str, ui, ui));
      netwib__buf_reinit(&buf);
      netwib_er(netwib_buf_append_fmt(&buf, "%%{c_%{uint32};buf}|",
                                      totalsize));
      netwib_er(netwib_buf_ref_string(&buf, &str));
      netwib_er(netwib_buf_append_fmt(pbuf, str, &buf2));
      break;
    }
    break;
  case NETWIB_SHOW_ARRAY_NUMTYPE_HEXACHAR :
    switch (size) {
    case 32 :
      netwib_er(netwib_buf_append_fmt(&buf, "%{uint32:#08X}=%s", ui, cstr));
      netwib_er(netwib_buf_append_fmt(pbuf, "|%{c_63;buf}|\n", &buf));
      break;
    case 16 :
      netwib_er(netwib_buf_append_fmt(&buf, "%{uint32:#04X}=%s", ui, cstr));
      netwib_er(netwib_buf_append_fmt(pbuf, "%{c_31;buf}|", &buf));
      break;
    case 8 :
      netwib_er(netwib_buf_append_fmt(&buf, "%{uint32:#02X}=%s", ui, cstr));
      netwib_er(netwib_buf_append_fmt(pbuf, "%{c_15;buf}|", &buf));
      break;
    case 4 :
      netwib_er(netwib_buf_append_fmt(&buf, "%{uint32:#01X}=%s", ui, cstr));
      netwib_er(netwib_buf_append_fmt(pbuf, "%{c_7;buf}|", &buf));
      break;
    case 1 :
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
    default :
      netwib_er(netwib_buf_append_fmt(&buf, "%%{uint32:#0%{uint32}X}=%s",
                                      hexasize, cstr));
      netwib_er(netwib_buf_ref_string(&buf, &str));
      netwib_er(netwib_buf_init_ext_arraysizeofempty(array2, &buf2));
      netwib_er(netwib_buf_append_fmt(&buf2, str, ui));
      netwib__buf_reinit(&buf);
      netwib_er(netwib_buf_append_fmt(&buf, "%%{c_%{uint32};buf}|",
                                      totalsize));
      netwib_er(netwib_buf_ref_string(&buf, &str));
      netwib_er(netwib_buf_append_fmt(pbuf, str, &buf2));
      break;
    }
    break;
  case NETWIB_SHOW_ARRAY_NUMTYPE_HEXADECCHAR :
    switch (size) {
    case 32 :
      netwib_er(netwib_buf_append_fmt(&buf, "%{uint32:#08X}=%{uint32}=%s",
                                      ui, ui, cstr));
      netwib_er(netwib_buf_append_fmt(pbuf, "|%{c_63;buf}|\n", &buf));
      break;
    case 16 :
      netwib_er(netwib_buf_append_fmt(&buf, "%{uint32:#04X}=%{uint32}=%s",
                                      ui, ui, cstr));
      netwib_er(netwib_buf_append_fmt(pbuf, "%{c_31;buf}|", &buf));
      break;
    case 8 :
      netwib_er(netwib_buf_append_fmt(&buf, "%{uint32:#02X}=%{uint32}=%s",
                                      ui, ui, cstr));
      netwib_er(netwib_buf_append_fmt(pbuf, "%{c_15;buf}|", &buf));
      break;
    case 4 :
      netwib_er(netwib_buf_append_fmt(&buf, "%{uint32:#01X}=%{uint32}=%s",
                                      ui, ui, cstr));
      netwib_er(netwib_buf_append_fmt(pbuf, "%{c_7;buf}|", &buf));
      break;
    case 1 :
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
    default :
      netwib_er(netwib_buf_append_fmt(&buf,
                                      "%%{uint32:#0%{uint32}X}=%%{uint32}=%s",
                                      hexasize, cstr));
      netwib_er(netwib_buf_ref_string(&buf, &str));
      netwib_er(netwib_buf_init_ext_arraysizeofempty(array2, &buf2));
      netwib_er(netwib_buf_append_fmt(&buf2, str, ui, ui));
      netwib__buf_reinit(&buf);
      netwib_er(netwib_buf_append_fmt(&buf, "%%{c_%{uint32};buf}|",
                                      totalsize));
      netwib_er(netwib_buf_ref_string(&buf, &str));
      netwib_er(netwib_buf_append_fmt(pbuf, str, &buf2));
      break;
    }
    break;
  default:
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_show_array_fmt(netwib_uint32 size,
                                 netwib_show_array_align align,
                                 netwib_char fillchar,
                                 netwib_buf *pbuf,
                                 netwib_conststring fmt,
                                 ...)
{
  netwib_buf buf, buf2;
  netwib_byte array[80], array2[80];
  netwib_char alignchar;
  netwib_string str;
  va_list ap;
  netwib_err ret;

  alignchar = 'l';
  switch(align) {
  case NETWIB_SHOW_ARRAY_ALIGN_LEFT :
    break;
  case NETWIB_SHOW_ARRAY_ALIGN_RIGHT :
    alignchar = 'r';
    break;
  case NETWIB_SHOW_ARRAY_ALIGN_CENTER :
    alignchar = 'c';
    break;
  default:
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));
  netwib_er(netwib_buf_init_ext_arraysizeofempty(array2, &buf2));

  va_start(ap, fmt);
  ret = netwib_priv_buf_append_vfmt(&buf, fmt, &ap);
  va_end(ap);
  if (ret == NETWIB_ERR_OK) {
    if (size == 32) {
      netwib_er(netwib_buf_append_byte('|', &buf2));
    }
    netwib_er(netwib_buf_append_fmt(&buf2, "%%{%c%c%{uint32};buf}|", alignchar,
                                    fillchar, 2*size-1));
    if (size == 32) {
      netwib_er(netwib_buf_append_string("\n", &buf2));
    }
    netwib_er(netwib_buf_ref_string(&buf2, &str));
    netwib_er(netwib_buf_append_fmt(pbuf, str, &buf));
  }
  netwib_er(netwib_buf_close(&buf));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_show_array_fmt32(netwib_buf *pbuf,
                                   netwib_conststring fmt,
                                   ...)
{
  netwib_buf buf;
  netwib_byte array[80];
  va_list ap;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));

  va_start(ap, fmt);
  ret = netwib_priv_buf_append_vfmt(&buf, fmt, &ap);
  va_end(ap);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_append_fmt(pbuf, "|%{l 63;buf}|\n", &buf));
  }
  netwib_er(netwib_buf_close(&buf));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_show_array_data(netwib_conststring title,
                                  netwib_constbuf *pdata,
                                  netwib_encodetype encodetype,
                                  netwib_char fillchar,
                                  netwib_buf *pbuf)
{
  netwib_buf buf;
  netwib_byte array[512];
  netwib_uint32 i, endoffset, datasize, leftsize, centersize, rightsize;
  netwib_uint32 strlen_title;
  netwib_bool needtostart;
  netwib_err ret;

  /* convert */
  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));
  ret = netwib_buf_encode(pdata, encodetype, &buf);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_buf_close(&buf));
    return(ret);
  }

  /* obtain positions where to write data */
  leftsize = 1;
  netwib_er(netwib_buf_append_byte('|', pbuf));
  if (title != NULL) {
    strlen_title = netwib_c_strlen(title);
    netwib_er(netwib_buf_append_string(title, pbuf));
    netwib_er(netwib_buf_append_byte(':', pbuf));
    if (strlen_title < 40) {
      leftsize += strlen_title + 1;
    } else {
      if (strlen_title < 62) {
        rightsize = 62 - strlen_title;
        for (i = 0; i < rightsize; i++) {
          netwib_er(netwib_buf_append_byte(' ', pbuf));
        }
      }
      netwib_er(netwib_buf_append_string("|\n|", pbuf));
    }
  }
  rightsize = 1;
  centersize = 63 - rightsize - leftsize;

  /* first, easy case where everything is on one line */
  datasize = netwib__buf_ref_data_size(&buf);
  if (datasize <= centersize) {
    netwib_er(netwib_buf_append_byte(fillchar, pbuf));
    netwib_er(netwib_buf_append_buf(&buf, pbuf));
    rightsize += centersize - datasize;
    for (i = 0; i < rightsize; i++) {
      netwib_er(netwib_buf_append_byte(fillchar, pbuf));
    }
    netwib_er(netwib_buf_append_string("|\n", pbuf));
    netwib_er(netwib_buf_close(&buf));
    return(NETWIB_ERR_OK);
  }
  netwib_er(netwib_buf_append_byte(' ', pbuf));

  /* write full chunks */
  endoffset = buf.endoffset;
  needtostart = NETWIB_FALSE;
  while (netwib__buf_ref_data_size(&buf) > centersize) {
    buf.endoffset = buf.beginoffset + centersize;
    if (needtostart) {
      netwib_er(netwib_buf_append_byte('|', pbuf));
      for (i = 0; i < leftsize; i++) {
        netwib_er(netwib_buf_append_byte(' ', pbuf));
      }
    }
    netwib_er(netwib_buf_append_buf(&buf, pbuf));
    for (i = 0; i < rightsize; i++) {
      netwib_er(netwib_buf_append_byte(' ', pbuf));
    }
    netwib_er(netwib_buf_append_string("|\n", pbuf));
    needtostart = NETWIB_TRUE;
    buf.beginoffset = buf.endoffset;
    buf.endoffset = endoffset;
  }

  /* write last chunk */
  netwib_er(netwib_buf_append_byte('|', pbuf));
  for (i = 0; i < leftsize; i++) {
    netwib_er(netwib_buf_append_byte(fillchar, pbuf));
  }
  netwib_er(netwib_buf_append_buf(&buf, pbuf));
  rightsize += centersize - netwib__buf_ref_data_size(&buf);
  for (i = 0; i < rightsize; i++) {
    netwib_er(netwib_buf_append_byte(fillchar, pbuf));
  }
  netwib_er(netwib_buf_append_string("|\n", pbuf));

  netwib_er(netwib_buf_close(&buf));
  return(NETWIB_ERR_OK);
}
