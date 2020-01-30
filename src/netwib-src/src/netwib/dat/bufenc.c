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
static netwib_err netwib_priv_buf_encode_data_wrap(netwib_constbuf *pinbuf,
                                                   netwib_uint32 wrapstep,
                                                   netwib_buf *poutbuf)
{
  netwib_data datain, dataout, dataoutsave;
  netwib_uint32 dataoutneededsize;
  netwib_uint32 datainsize, dataoutsize, i;

  /* obtain references */
  datainsize = netwib__buf_ref_data_size(pinbuf);
  if (datainsize == 0) {
    return(NETWIB_ERR_OK);
  }
  datain = netwib__buf_ref_data_ptr(pinbuf);
  dataoutneededsize = datainsize + datainsize/wrapstep + 1;
  netwib_er(netwib_buf_wantspace(poutbuf, dataoutneededsize, &dataout));

  /* set data */
  dataoutsave = dataout; /* needed to compute length */
  i = 0;
  while (NETWIB_TRUE) {
    *dataout++ = *datain++;
    i++;
    if (i == datainsize) {
      *dataout++ = '\n';
      break;
    }
    if (i%wrapstep == 0) {
      *dataout++ = '\n';
    }
  }
  dataoutsize = dataout - dataoutsave;

  /* indicate end */
  poutbuf->endoffset += dataoutsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_buf_encode_hexa0(netwib_constbuf *pinbuf,
                                               netwib_buf *poutbuf)
{
  netwib_data datain, dataout;
  netwib_uint32 dataoutneededsize;
  netwib_uint32 datainsize, dataoutsize, i, quartet;

  /* obtain references */
  datainsize = netwib__buf_ref_data_size(pinbuf);
  if (datainsize == 0) {
    return(NETWIB_ERR_OK);
  }
  datain = netwib__buf_ref_data_ptr(pinbuf);
  dataoutneededsize = 2 * datainsize;
  netwib_er(netwib_buf_wantspace(poutbuf, dataoutneededsize, &dataout));

  /* set data */
  i = 0;
  while (i < datainsize) {
    quartet = *datain >> 4;
    *dataout++ = netwib_c2_16toc(quartet);
    quartet = *datain & 0xF;
    *dataout++ = netwib_c2_16toc(quartet);
    datain++;
    i++;
  }
  dataoutsize = 2 * datainsize;

  /* indicate end */
  poutbuf->endoffset += dataoutsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_buf_encode_hexa_wrap(netwib_constbuf *pinbuf,
                                                   netwib_uint32 spacestep,
                                                   netwib_uint32 wrapstep,
                                                   netwib_buf *poutbuf)
{
  netwib_data datain, dataout, dataoutsave;
  netwib_uint32 dataoutneededsize;
  netwib_uint32 datainsize, dataoutsize, i, quartet;
  netwib_bool notfirst;

  /* obtain references */
  datainsize = netwib__buf_ref_data_size(pinbuf);
  if (datainsize == 0) {
    return(NETWIB_ERR_OK);
  }
  datain = netwib__buf_ref_data_ptr(pinbuf);
  dataoutneededsize = 2 * datainsize;
  if (spacestep) {
    /* this is the worst case "12 12 ..." */
    dataoutneededsize += datainsize;
  }
  dataoutneededsize += dataoutneededsize/wrapstep + 1;
  netwib_er(netwib_buf_wantspace(poutbuf, dataoutneededsize, &dataout));

  /* set data */
  dataoutsave = dataout; /* needed to compute length */
  notfirst = NETWIB_FALSE;
  i = 0;
  while (NETWIB_TRUE) {
    switch(spacestep) {
      case 0:
        break;
      case 1:
        if (notfirst) *dataout++ = ' ';
        break;
      case 2:
        if (notfirst && (i&1) == 0) *dataout++ = ' ';
        break;
      case 4:
        if (notfirst && (i&3) == 0) *dataout++ = ' ';
        break;
    }
    quartet = *datain >> 4;
    *dataout++ = netwib_c2_16toc(quartet);
    quartet = *datain & 0xF;
    *dataout++ = netwib_c2_16toc(quartet);
    datain++;
    i++;
    if (i == datainsize) {
      *dataout++ = '\n';
      break;
    }
    notfirst = NETWIB_TRUE;
    if (i%wrapstep == 0) {
      *dataout++ = '\n';
      notfirst = NETWIB_FALSE;
    }
  }
  dataoutsize = dataout - dataoutsave;

  /* indicate end */
  poutbuf->endoffset += dataoutsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_buf_encode_hexa1(netwib_constbuf *pinbuf,
                                               netwib_buf *poutbuf)
{
  netwib_data datain, dataout, dataoutsave;
  netwib_uint32 dataoutneededsize;
  netwib_uint32 datainsize, dataoutsize, i, quartet;

  /* obtain references */
  datainsize = netwib__buf_ref_data_size(pinbuf);
  if (datainsize == 0) {
    return(NETWIB_ERR_OK);
  }
  datain = netwib__buf_ref_data_ptr(pinbuf);
  dataoutneededsize = 3 * datainsize;
  netwib_er(netwib_buf_wantspace(poutbuf, dataoutneededsize, &dataout));

  /* set data */
  dataoutsave = dataout; /* needed to compute length */
  quartet = *datain >> 4;
  *dataout++ = netwib_c2_16toc(quartet);
  quartet = *datain & 0xF;
  *dataout++ = netwib_c2_16toc(quartet);
  if (datainsize > 1) {
    datain++;
    i = 1;
    while (i < datainsize) {
      *dataout++ = ' ';
      quartet = *datain >> 4;
      *dataout++ = netwib_c2_16toc(quartet);
      quartet = *datain & 0xF;
      *dataout++ = netwib_c2_16toc(quartet);
      datain++;
      i++;
    }
  }
  dataoutsize = dataout - dataoutsave;

  /* indicate end */
  poutbuf->endoffset += dataoutsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_buf_encode_hexa24(netwib_constbuf *pinbuf,
                                                netwib_uint32 spacestep,
                                                netwib_buf *poutbuf)
{
  netwib_data datain, dataout, dataoutsave;
  netwib_uint32 dataoutneededsize;
  netwib_uint32 datainsize, dataoutsize, i, quartet;

  /* obtain references */
  datainsize = netwib__buf_ref_data_size(pinbuf);
  if (datainsize == 0) {
    return(NETWIB_ERR_OK);
  }
  datain = netwib__buf_ref_data_ptr(pinbuf);
  dataoutneededsize = 2 * datainsize + datainsize/spacestep;
  netwib_er(netwib_buf_wantspace(poutbuf, dataoutneededsize, &dataout));

  /* set data */
  dataoutsave = dataout; /* needed to compute length */
  i = 0;
  if (datainsize < spacestep) spacestep = datainsize;
  while (i < spacestep) {
    quartet = *datain >> 4;
    *dataout++ = netwib_c2_16toc(quartet);
    quartet = *datain & 0xF;
    *dataout++ = netwib_c2_16toc(quartet);
    datain++;
    i++;
  }
  while (i < datainsize) {
    switch (spacestep) {
     case 2:
      if ((i&1) == 0) *dataout++ = ' ';
      break;
     case 4:
      if ((i&3) == 0) *dataout++ = ' ';
      break;
     default:
      return(NETWIB_ERR_LOINTERNALERROR);
    }
    quartet = *datain >> 4;
    *dataout++ = netwib_c2_16toc(quartet);
    quartet = *datain & 0xF;
    *dataout++ = netwib_c2_16toc(quartet);
    datain++;
    i++;
  }
  dataoutsize = dataout - dataoutsave;

  /* indicate end */
  poutbuf->endoffset += dataoutsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
#define netwib_priv_isprinthexa(c) (c<32 || c>126 || c=='\\' || c=='"')
static netwib_err netwib_priv_buf_encode_mixed(netwib_constbuf *pinbuf,
                                               netwib_uint32 spacestep,
                                               netwib_buf *poutbuf)
{
  netwib_data datain, dataout, dataoutsave;
  netwib_uint32 dataoutneededsize;
  netwib_uint32 datainsize, dataoutsize, i, quartet;
  netwib_bool modehexa;
  netwib_byte c;

  /* obtain references */
  datainsize = netwib__buf_ref_data_size(pinbuf);
  if (datainsize == 0) {
    return(NETWIB_ERR_OK);
  }
  datain = netwib__buf_ref_data_ptr(pinbuf);
  /* worst case is "'a' 00 ", or "'''' 00 ", so 8 chars for 2 bytes.
     We take 4 char/byte */
  dataoutneededsize = 4 * datainsize;
  netwib_er(netwib_buf_wantspace(poutbuf, dataoutneededsize, &dataout));

  /* set data */
  dataoutsave = dataout; /* needed to compute length */
  /* print first char */
  modehexa = NETWIB_FALSE;
  c = *datain++;
  if (netwib_priv_isprinthexa(c)) {
    quartet = c >> 4;
    *dataout++ = netwib_c2_16toc(quartet);
    quartet = c & 0xF;
    *dataout++ = netwib_c2_16toc(quartet);
    modehexa = NETWIB_TRUE;
  } else {
    *dataout++ = '\'';
    if (c == '\'') {
      *dataout++ = '\'';
      *dataout++ = '\'';
    } else {
      *dataout++ = c;
    }
  }
  /* print next chars */
  for (i = 1; i < datainsize; i++) {
    c = *datain++;
    if (netwib_priv_isprinthexa(c)) {
      if (modehexa) {
        if (spacestep) {
          *dataout++ = ' ';
        }
      } else {
        *dataout++ = '\'';
        *dataout++ = ' ';
        modehexa = NETWIB_TRUE;
      }
      quartet = c >> 4;
      *dataout++ = netwib_c2_16toc(quartet);
      quartet = c & 0xF;
      *dataout++ = netwib_c2_16toc(quartet);
    } else {
      if (modehexa) {
        *dataout++ = ' ';
        *dataout++ = '\'';
        modehexa = NETWIB_FALSE;
      }
      if (c == '\'') {
        *dataout++ = '\'';
        *dataout++ = '\'';
      } else {
        *dataout++ = c;
      }
    }
  }
  if (!modehexa) {
    *dataout++ = '\'';
  }
  dataoutsize = dataout - dataoutsave;

  /* indicate end */
  poutbuf->endoffset += dataoutsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_buf_encode_mixed_wrap(netwib_constbuf *pinbuf,
                                                    netwib_uint32 spacestep,
                                                    netwib_uint32 wrapstep,
                                                    netwib_buf *poutbuf)
{
  netwib_data datain, dataout, dataoutsave;
  netwib_uint32 dataoutneededsize;
  netwib_uint32 datainsize, dataoutsize, i, quartet;
  netwib_bool modehexa, notfirst;
  netwib_byte c;

  /* obtain references */
  datainsize = netwib__buf_ref_data_size(pinbuf);
  if (datainsize == 0) {
    return(NETWIB_ERR_OK);
  }
  datain = netwib__buf_ref_data_ptr(pinbuf);
  dataoutneededsize = 4 * datainsize;
  dataoutneededsize += dataoutneededsize/wrapstep + 1;
  netwib_er(netwib_buf_wantspace(poutbuf, dataoutneededsize, &dataout));

  /* set data */
  dataoutsave = dataout; /* needed to compute length */
  modehexa = NETWIB_TRUE;
  notfirst = NETWIB_FALSE;
  i = 0;
  while (NETWIB_TRUE) {
    c = *datain++;
    if (netwib_priv_isprinthexa(c)) {
      if (modehexa) {
        if (spacestep) {
          if (notfirst) *dataout++ = ' ';
        }
      } else {
        *dataout++ = '\'';
        *dataout++ = ' ';
        modehexa = NETWIB_TRUE;
      }
      quartet = c >> 4;
      *dataout++ = netwib_c2_16toc(quartet);
      quartet = c & 0xF;
      *dataout++ = netwib_c2_16toc(quartet);
    } else {
      if (modehexa) {
        if (notfirst) *dataout++ = ' ';
        *dataout++ = '\'';
        modehexa = NETWIB_FALSE;
      }
      if (c == '\'') {
        *dataout++ = '\'';
        *dataout++ = '\'';
      } else {
        *dataout++ = c;
      }
    }
    i++;
    if (i == datainsize) {
      break;
    }
    notfirst = NETWIB_TRUE;
    if (i%wrapstep == 0) {
      if (!modehexa) {
        *dataout++ = '\'';
        modehexa = NETWIB_TRUE;
      }
      *dataout++ = '\n';
      notfirst = NETWIB_FALSE;
    }
  }
  if (!modehexa) {
    *dataout++ = '\'';
  }
  *dataout++ = '\n';
  dataoutsize = dataout - dataoutsave;

  /* indicate end */
  poutbuf->endoffset += dataoutsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_buf_encode_mixedh_wrap(netwib_constbuf *pinbuf,
                                                     netwib_uint32 spacestep,
                                                     netwib_buf *poutbuf)
{
  netwib_data datain, dataout, dataoutr, dataoutsave;
  netwib_uint32 dataoutneededsize;
  netwib_uint32 datainsize, dataoutsize, i, quartet, nbspaces;
  netwib_bool modehexa, notfirst;
  netwib_byte c;

  /* obtain references */
  datainsize = netwib__buf_ref_data_size(pinbuf);
  if (datainsize == 0) {
    return(NETWIB_ERR_OK);
  }
  datain = netwib__buf_ref_data_ptr(pinbuf);
  dataoutneededsize = 10 * datainsize + 80; /* 80*datainsize/8 + 80 */
  netwib_er(netwib_buf_wantspace(poutbuf, dataoutneededsize, &dataout));

  /* set data */
  dataoutsave = dataout; /* needed to compute length */
  modehexa = NETWIB_TRUE;
  notfirst = NETWIB_FALSE;
  i = 0;
  dataoutr = dataout + 42;
  while (NETWIB_TRUE) {
    c = *datain++;
    if (netwib_priv_isprinthexa(c)) {
      if (modehexa) {
        if (spacestep) {
          if (notfirst) *dataout++ = ' ';
        }
      } else {
        *dataout++ = '\'';
        *dataout++ = ' ';
        modehexa = NETWIB_TRUE;
      }
      quartet = c >> 4;
      *dataout++ = netwib_c2_16toc(quartet);
      quartet = c & 0xF;
      *dataout++ = netwib_c2_16toc(quartet);
    } else {
      if (modehexa) {
        if (notfirst) *dataout++ = ' ';
        *dataout++ = '\'';
        modehexa = NETWIB_FALSE;
      }
      if (c == '\'') {
        *dataout++ = '\'';
        *dataout++ = '\'';
      } else {
        *dataout++ = c;
      }
    }
    *dataoutr++ = ' ';
    quartet = c >> 4;
    *dataoutr++ = netwib_c2_16toc(quartet);
    quartet = c & 0xF;
    *dataoutr++ = netwib_c2_16toc(quartet);
    i++;
    if (i == datainsize) {
      if (!modehexa) {
        *dataout++ = '\'';
      }
      nbspaces = dataoutr - dataout - 3*(datainsize&0x7) - 1;
      netwib_c_memset(dataout, ' ', nbspaces);
      dataout += nbspaces;
      *dataout++ = '#';
      *dataoutr++ = '\n';
      break;
    }
    notfirst = NETWIB_TRUE;
    if ((i&0x7) == 0) {
      if (!modehexa) {
        *dataout++ = '\'';
        modehexa = NETWIB_TRUE;
      }
      nbspaces = dataoutr - dataout - 25;
      netwib_c_memset(dataout, ' ', nbspaces);
      dataout += nbspaces;
      *dataout++ = '#';
      *dataoutr++ = '\n';
      notfirst = NETWIB_FALSE;
      dataout = dataoutr;
      dataoutr = dataout + 42;
    }
  }
  dataoutsize = dataoutr - dataoutsave;

  /* indicate end */
  poutbuf->endoffset += dataoutsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_buf_encode_text(netwib_constbuf *pinbuf,
                                              netwib_buf *poutbuf)
{
  netwib_data datain, dataout;
  netwib_uint32 dataoutneededsize;
  netwib_uint32 datainsize, dataoutsize, i;
  netwib_byte c;

  /* obtain references */
  datainsize = netwib__buf_ref_data_size(pinbuf);
  if (datainsize == 0) {
    return(NETWIB_ERR_OK);
  }
  datain = netwib__buf_ref_data_ptr(pinbuf);
  dataoutneededsize = datainsize;
  netwib_er(netwib_buf_wantspace(poutbuf, dataoutneededsize, &dataout));

  /* set data */
  for (i = 0; i < datainsize; i++) {
    c = *datain++;
    if (netwib_c2_isprint2(c)) {
      *dataout++ = c;
    } else {
      *dataout++ = '.';
    }
  }
  dataoutsize = datainsize;

  /* indicate end */
  poutbuf->endoffset += dataoutsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_buf_encode_lowercase(netwib_constbuf *pinbuf,
                                                   netwib_buf *poutbuf)
{
  netwib_data datain, dataout;
  netwib_uint32 datainsize, i;
  netwib_byte c;

  /* obtain references */
  datainsize = netwib__buf_ref_data_size(pinbuf);
  if (datainsize == 0) {
    return(NETWIB_ERR_OK);
  }
  datain = netwib__buf_ref_data_ptr(pinbuf);
  netwib_er(netwib_buf_wantspace(poutbuf, datainsize, &dataout));

  /* set data */
  for (i = 0; i < datainsize; i++) {
    c = *datain++;
    netwib_c2_lower(c);
    *dataout++ = c;
  }

  /* indicate end */
  poutbuf->endoffset += datainsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_buf_encode_uppercase(netwib_constbuf *pinbuf,
                                                   netwib_buf *poutbuf)
{
  netwib_data datain, dataout;
  netwib_uint32 datainsize, i;
  netwib_byte c;

  /* obtain references */
  datainsize = netwib__buf_ref_data_size(pinbuf);
  if (datainsize == 0) {
    return(NETWIB_ERR_OK);
  }
  datain = netwib__buf_ref_data_ptr(pinbuf);
  netwib_er(netwib_buf_wantspace(poutbuf, datainsize, &dataout));

  /* set data */
  for (i = 0; i < datainsize; i++) {
    c = *datain++;
    netwib_c2_upper(c);
    *dataout++ = c;
  }

  /* indicate end */
  poutbuf->endoffset += datainsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_buf_encode_text_wrap(netwib_constbuf *pinbuf,
                                                   netwib_uint32 wrapstep,
                                                   netwib_buf *poutbuf)
{
  netwib_data datain, dataout, dataoutsave;
  netwib_uint32 dataoutneededsize;
  netwib_uint32 datainsize, dataoutsize, i, truecount;
  netwib_byte c;

  /* obtain references */
  datainsize = netwib__buf_ref_data_size(pinbuf);
  if (datainsize == 0) {
    return(NETWIB_ERR_OK);
  }
  datain = netwib__buf_ref_data_ptr(pinbuf);
  dataoutneededsize = datainsize + datainsize/wrapstep + 1;
  netwib_er(netwib_buf_wantspace(poutbuf, dataoutneededsize, &dataout));

  /* set data */
  dataoutsave = dataout; /* needed to compute length */
  i = 0;
  truecount = 0;
  while (NETWIB_TRUE) {
    c = *datain++;
    if (netwib_c2_isprint2(c)) {
      *dataout++ = c;
    } else {
      *dataout++ = '.';
    }
    i++;
    if (c == '\n') {
      truecount = 0;
    } else {
      truecount++;
    }
    if (i == datainsize) {
      break;
    }
    if (truecount && truecount%wrapstep == 0) {
      *dataout++ = '\n';
      truecount = 0;
    }
  }
  *dataout++ = '\n';
  dataoutsize = dataout - dataoutsave;

  /* indicate end */
  poutbuf->endoffset += dataoutsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static const char netwib_priv_b64_arr[] =
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static netwib_err netwib_priv_buf_encode_base64(netwib_constbuf *pinbuf,
                                                netwib_buf *poutbuf)
{
  netwib_data datain, dataout, dataoutsave;
  netwib_uint32 dataoutneededsize;
  netwib_uint32 datainsize, dataoutsize, i, v1, v2, v3;

  /* obtain references */
  datainsize = netwib__buf_ref_data_size(pinbuf);
  if (datainsize == 0) {
    return(NETWIB_ERR_OK);
  }
  datain = netwib__buf_ref_data_ptr(pinbuf);
  dataoutneededsize = datainsize + datainsize/3 + 3;
  netwib_er(netwib_buf_wantspace(poutbuf, dataoutneededsize, &dataout));

  /* set data */
  dataoutsave = dataout; /* needed to compute length */
  i = 0;
  if (datainsize > 2) {
    while (i < datainsize - 2) {
      v1 = *datain++;
      v2 = *datain++;
      v3 = *datain++;
      *dataout++ = netwib_priv_b64_arr[v1>>2];
      *dataout++ = netwib_priv_b64_arr[((v1&0x3)<<4) | ((v2&0xF0)>>4)];
      *dataout++ = netwib_priv_b64_arr[((v2&0xF)<<2) | ((v3&0xC0)>>6)];
      *dataout++ = netwib_priv_b64_arr[v3&0x3F];
      i += 3;
    }
  }
  if (i < datainsize) {
    v1 = *datain++;
    *dataout++ = netwib_priv_b64_arr[v1>>2];
    if (i+1 == datainsize) {
      *dataout++ = netwib_priv_b64_arr[(v1&0x3)<<4];
      *dataout++ = '=';
    } else {
      v2 = *datain++;
      *dataout++ = netwib_priv_b64_arr[((v1&0x3)<<4) | ((v2&0xF0)>>4)];
      *dataout++ = netwib_priv_b64_arr[(v2&0xF)<<2];
    }
    *dataout++ = '=';
  }
  dataoutsize = dataout - dataoutsave;

  /* indicate end */
  poutbuf->endoffset += dataoutsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_buf_encode_base64_wrap(netwib_constbuf *pinbuf,
                                                     netwib_uint32 wrapstep,
                                                     netwib_buf *poutbuf)
{
  netwib_buf buf;
  netwib_data datain;
  netwib_uint32 datainsize, worksize, dataoutneededsize;
  netwib_err ret;

  datainsize = netwib__buf_ref_data_size(pinbuf);
  if (datainsize == 0) {
    return(NETWIB_ERR_OK);
  }
  datain = netwib__buf_ref_data_ptr(pinbuf);
  dataoutneededsize = datainsize + datainsize/3 + 3 + datainsize/wrapstep + 1;
  netwib_er(netwib_buf_wantspace(poutbuf, dataoutneededsize, NULL));

  while (NETWIB_TRUE) {
    worksize = datainsize;
    if (worksize > wrapstep) {
      worksize = wrapstep;
    }
    netwib_er(netwib_buf_init_ext_arrayfilled(datain, worksize, &buf));
    ret = netwib_priv_buf_encode_base64(&buf, poutbuf);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwib_buf_append_byte('\n', poutbuf));
      return(ret);
    }
    datain += worksize;
    datainsize -= worksize;
    if (datainsize == 0) {
      break;
    }
    netwib_er(netwib_buf_append_byte('\n', poutbuf));
  }
  netwib_er(netwib_buf_append_byte('\n', poutbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_buf_encode_synth(netwib_constbuf *pinbuf,
                                               netwib_buf *poutbuf)
{
  /* messages are :
       size 0 : "0 byte"
       size 1 : "1 byte"
       size n : "n bytes"
     so bigger message is "4000000000 bytes" = 16 char
  */
  netwib_byte array[17];
  netwib_uint32 datainsize;
  netwib_buf buf;
  netwib_err ret;

  datainsize = netwib__buf_ref_data_size(pinbuf);

  netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));
  netwib_er(netwib_buf_append_fmt(&buf, "%{uint32} byte", datainsize));
  if (datainsize > 1) {
    netwib_er(netwib_buf_append_byte('s', &buf));
  }
  ret = netwib_buf_append_buf(&buf, poutbuf);

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_buf_encode_array(netwib_constbuf *pinbuf,
                                               netwib_uint32 fieldsize,
                                               netwib_buf *poutbuf)
{
  netwib_data datain, dataout;
  netwib_uint32 dataoutneededsize;
  netwib_uint32 datainsize, i, maxloop, left;
  netwib_uint32 c, c1, c2;

  /* obtain references */
  datainsize = netwib__buf_ref_data_size(pinbuf);
  if (datainsize == 0) {
    return(NETWIB_ERR_OK);
  }
  datain = netwib__buf_ref_data_ptr(pinbuf);
  dataoutneededsize = 20*datainsize + 160; /* 80*datainsize/4 */
  netwib_er(netwib_buf_wantspace(poutbuf, dataoutneededsize, &dataout));

  /* if no data, do not even output a newline */
  if (datainsize == 0) {
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_show_array_head(NULL, poutbuf));
  switch(fieldsize) {
  case 1:
  case 4:
  case 8:
    i = 0;
    netwib_er(netwib_show_array_line_begin(poutbuf));
    while (NETWIB_TRUE) {
      c = *datain++;
      switch(fieldsize) {
      case 1:
        netwib_er(netwib_show_array_num1(c&1, poutbuf));
        netwib_er(netwib_show_array_num1((c>>1)&1, poutbuf));
        netwib_er(netwib_show_array_num1((c>>2)&1, poutbuf));
        netwib_er(netwib_show_array_num1((c>>3)&1, poutbuf));
        netwib_er(netwib_show_array_num1((c>>4)&1, poutbuf));
        netwib_er(netwib_show_array_num1((c>>5)&1, poutbuf));
        netwib_er(netwib_show_array_num1((c>>6)&1, poutbuf));
        netwib_er(netwib_show_array_num1((c>>7)&1, poutbuf));
        break;
      case 4:
        netwib_er(netwib_show_array_num4(c&0xF, NETWIB_SHOW_ARRAY_NUMTYPE_HEXA,
                                         poutbuf));
        netwib_er(netwib_show_array_num4((c>>4)&0xF,
                                         NETWIB_SHOW_ARRAY_NUMTYPE_HEXA,
                                         poutbuf));
        break;
      case 8:
        netwib_er(netwib_show_array_num8((netwib_byte)c,
                                         NETWIB_SHOW_ARRAY_NUMTYPE_HEXADECCHAR,
                                         poutbuf));
        break;
      }
      i++;
      if (i == datainsize) {
        break;
      }
      if ((i&3) == 0) {
        netwib_er(netwib_show_array_line_end(poutbuf));
        netwib_er(netwib_show_array_line_begin(poutbuf));
      }
    }
    switch (i&3) {
    case 1 :
      netwib_er(netwib_show_array_text8("_______________", poutbuf));
      /* no break */
    case 2 :
      netwib_er(netwib_show_array_text8("_______________", poutbuf));
      /* no break */
    case 3 :
      netwib_er(netwib_show_array_text8("_______________", poutbuf));
      break;
    }
    netwib_er(netwib_show_array_line_end(poutbuf));
    break;
  case 16:
  case 32:
    maxloop = datainsize/4;
    if (maxloop) {
      i = 0;
      switch(fieldsize) {
        case 16:
          netwib_er(netwib_show_array_line_begin(poutbuf));
          while (NETWIB_TRUE) {
            c = *datain++;
            c1 = (c<<8) | *datain++;
            c = *datain++;
            c2 = (c<<8) | *datain++;
            netwib_er(netwib_show_array_num16((netwib_uint16)c1,
                                            NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                            poutbuf));
            netwib_er(netwib_show_array_num16((netwib_uint16)c2,
                                            NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                            poutbuf));
            netwib_er(netwib_show_array_line_end(poutbuf));
            i++;
            if (i == maxloop) {
              break;
            }
            netwib_er(netwib_show_array_line_begin(poutbuf));
          }
          break;
        case 32:
          while (NETWIB_TRUE) {
            c = *datain++;
            c1 = (c<<8) | *datain++;
            c = *datain++;
            c2 = (c<<8) | *datain++;
            netwib_er(netwib_show_array_num32((c1<<16)|c2,
                                            NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                            poutbuf));
            i++;
            if (i == maxloop) {
              break;
            }
          }
          break;
      }
    }
    left = datainsize&3;
    if (left) {
      netwib_er(netwib_show_array_line_begin(poutbuf));
      if (left > 1) {
        c = *datain++;
        c = (c<<8) | *datain++;
        netwib_er(netwib_show_array_num16((netwib_uint16)c,
                                        NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                        poutbuf));
      }
      if (left == 1 || left == 3) {
        c = *datain++;
        netwib_er(netwib_show_array_num8((netwib_uint8)c,
                                       NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
                                       poutbuf));
        netwib_er(netwib_show_array_text8("_______________", poutbuf));
      }
      if (left < 3) {
        netwib_er(netwib_show_array_text16("_______________________________",
                                         poutbuf));
      }
      netwib_er(netwib_show_array_line_end(poutbuf));
    }
    break;
  default:
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_buf_encode_dump(netwib_constbuf *pinbuf,
                                              netwib_buf *poutbuf)
{
  netwib_data datain, dataout, dataoutr, dataoutsave;
  netwib_uint32 dataoutneededsize, nbspaces;
  netwib_uint32 datainsize, dataoutsize, i, quartet;
  netwib_byte c;
  netwib_bool notfirst;

  /* obtain references */
  datainsize = netwib__buf_ref_data_size(pinbuf);
  if (datainsize == 0) {
    return(NETWIB_ERR_OK);
  }
  datain = netwib__buf_ref_data_ptr(pinbuf);
  dataoutneededsize = 5*datainsize + 80; /* 80*datainsize/16 + 80 */
  netwib_er(netwib_buf_wantspace(poutbuf, dataoutneededsize, &dataout));

  /* set data */
  dataoutsave = dataout; /* needed to compute length */
  notfirst = NETWIB_FALSE;
  i = 0;
  dataoutr = dataout + 54;
  while (NETWIB_TRUE) {
    if (notfirst) *dataout++ = ' ';
    c = *datain++;
    quartet = c >> 4;
    *dataout++ = netwib_c2_16toc(quartet);
    quartet = c & 0xF;
    *dataout++ = netwib_c2_16toc(quartet);
    if (netwib_c2_isprint(c)) {
      *dataoutr++ = c;
    } else {
      *dataoutr++ = '.';
    }
    i++;
    if (i == datainsize) {
      nbspaces = dataoutr - dataout - (datainsize&0xF) - 2;
      if ((datainsize&0xF) == 0) nbspaces -= 0x10;
      netwib_c_memset(dataout, ' ', nbspaces);
      dataout += nbspaces;
      *dataout++ = '#';
      *dataout++ = ' ';
      *dataoutr++ = '\n';
      break;
    }
    notfirst = NETWIB_TRUE;
    if ((i&0xF) == 0) {
      *dataout++ = ' ';
      *dataout++ = ' ';
      *dataout++ = '#';
      *dataout++ = ' ';
      *dataoutr++ = '\n';
      notfirst = NETWIB_FALSE;
      dataout = dataoutr;
      dataoutr = dataout + 54;
    } else if ((i&0x3) == 0) {
      *dataout++ = ' ';
    }
  }
  dataoutsize = dataoutr - dataoutsave;

  /* indicate end */
  poutbuf->endoffset += dataoutsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_buf_encode_quoted(netwib_constbuf *pinbuf,
                                                netwib_buf *poutbuf)
{
  netwib_data datain, dataout, dataoutsave;
  netwib_uint32 dataoutneededsize, datainsize, quartet;
  netwib_byte c;

  /* obtain references */
  datainsize = netwib__buf_ref_data_size(pinbuf);
  datain = netwib__buf_ref_data_ptr(pinbuf);
  dataoutneededsize = 4*datainsize + 2; /* \x12 */
  netwib_er(netwib_buf_wantspace(poutbuf, dataoutneededsize, &dataout));

  /* set data */
  dataoutsave = dataout; /* needed to compute length */
  *dataout++ = '"';
  while (datainsize--) {
    c = *datain++;
    if (c == '\\' || c == '"') {
      *dataout++ = '\\';
      *dataout++ = c;
    } else if (netwib_c2_isprint(c)) { /* in second position because \ " are
                                          printable */
      *dataout++ = c;
    } else if (c == '\t') {
      *dataout++ = '\\';
      *dataout++ = 't';
    } else if (c == '\r') {
      *dataout++ = '\\';
      *dataout++ = 'r';
    } else if (c == '\n') {
      *dataout++ = '\\';
      *dataout++ = 'n';
    } else if (c == '\b') {
      *dataout++ = '\\';
      *dataout++ = 'b';
    } else if (c == 0x07) {
      *dataout++ = '\\';
      *dataout++ = 'a';
    } else {
      *dataout++ = '\\';
      *dataout++ = 'x';
      quartet = c >> 4;
      *dataout++ = netwib_c2_16toc(quartet);
      quartet = c & 0xF;
      *dataout++ = netwib_c2_16toc(quartet);
    }
  }
  *dataout++ = '"';

  /* indicate end */
  poutbuf->endoffset += dataout - dataoutsave;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_encode(netwib_constbuf *pbuftoencode,
                             netwib_encodetype encodetype,
                             netwib_buf *pbuf)
{

  netwib__buf_transfersensitive(pbuftoencode, pbuf);

  switch(encodetype) {
    case NETWIB_ENCODETYPE_DATA :
      netwib_er(netwib_buf_append_buf(pbuftoencode, pbuf));
      break;
    case NETWIB_ENCODETYPE_HEXA0 :
      netwib_er(netwib_priv_buf_encode_hexa0(pbuftoencode, pbuf));
      break;
    case NETWIB_ENCODETYPE_HEXA1 :
      netwib_er(netwib_priv_buf_encode_hexa1(pbuftoencode, pbuf));
      break;
    case NETWIB_ENCODETYPE_HEXA2 :
      netwib_er(netwib_priv_buf_encode_hexa24(pbuftoencode, 2, pbuf));
      break;
    case NETWIB_ENCODETYPE_HEXA4 :
      netwib_er(netwib_priv_buf_encode_hexa24(pbuftoencode, 4, pbuf));
      break;
    case NETWIB_ENCODETYPE_MIXED0 :
      netwib_er(netwib_priv_buf_encode_mixed(pbuftoencode, 0, pbuf));
      break;
    case NETWIB_ENCODETYPE_MIXED1 :
      netwib_er(netwib_priv_buf_encode_mixed(pbuftoencode, 1, pbuf));
      break;
    case NETWIB_ENCODETYPE_TEXT :
      netwib_er(netwib_priv_buf_encode_text(pbuftoencode, pbuf));
      break;
    case NETWIB_ENCODETYPE_BASE64 :
      netwib_er(netwib_priv_buf_encode_base64(pbuftoencode, pbuf));
      break;
    case NETWIB_ENCODETYPE_QUOTED :
      netwib_er(netwib_priv_buf_encode_quoted(pbuftoencode, pbuf));
      break;

    case NETWIB_ENCODETYPE_NOTHING :
      break;
    case NETWIB_ENCODETYPE_SYNTH :
      netwib_er(netwib_priv_buf_encode_synth(pbuftoencode, pbuf));
      break;

    case NETWIB_ENCODETYPE_DATA_WRAP :
      netwib_er(netwib_priv_buf_encode_data_wrap(pbuftoencode, 72, pbuf));
      break;
    case NETWIB_ENCODETYPE_HEXA0_WRAP :
      netwib_er(netwib_priv_buf_encode_hexa_wrap(pbuftoencode, 0, 32, pbuf));
      break;
    case NETWIB_ENCODETYPE_HEXA1_WRAP :
      netwib_er(netwib_priv_buf_encode_hexa_wrap(pbuftoencode, 1, 16, pbuf));
      break;
    case NETWIB_ENCODETYPE_HEXA2_WRAP :
      netwib_er(netwib_priv_buf_encode_hexa_wrap(pbuftoencode, 2, 32, pbuf));
      break;
    case NETWIB_ENCODETYPE_HEXA4_WRAP :
      netwib_er(netwib_priv_buf_encode_hexa_wrap(pbuftoencode, 4, 32, pbuf));
      break;
    case NETWIB_ENCODETYPE_MIXED0_WRAP :
      netwib_er(netwib_priv_buf_encode_mixed_wrap(pbuftoencode, 0, 16, pbuf));
      break;
    case NETWIB_ENCODETYPE_MIXED1_WRAP :
      netwib_er(netwib_priv_buf_encode_mixed_wrap(pbuftoencode, 1, 16, pbuf));
      break;
    case NETWIB_ENCODETYPE_TEXT_WRAP :
      netwib_er(netwib_priv_buf_encode_text_wrap(pbuftoencode, 72, pbuf));
      break;
    case NETWIB_ENCODETYPE_BASE64_WRAP :
      netwib_er(netwib_priv_buf_encode_base64_wrap(pbuftoencode, 54, pbuf));
      break;

    case NETWIB_ENCODETYPE_ARRAY1 :
      netwib_er(netwib_priv_buf_encode_array(pbuftoencode, 1, pbuf));
      break;
    case NETWIB_ENCODETYPE_ARRAY4 :
      netwib_er(netwib_priv_buf_encode_array(pbuftoencode, 4, pbuf));
      break;
    case NETWIB_ENCODETYPE_ARRAY8 :
      netwib_er(netwib_priv_buf_encode_array(pbuftoencode, 8, pbuf));
      break;
    case NETWIB_ENCODETYPE_ARRAY16 :
      netwib_er(netwib_priv_buf_encode_array(pbuftoencode, 16, pbuf));
      break;
    case NETWIB_ENCODETYPE_ARRAY32 :
      netwib_er(netwib_priv_buf_encode_array(pbuftoencode, 32, pbuf));
      break;
    case NETWIB_ENCODETYPE_DUMP :
      netwib_er(netwib_priv_buf_encode_dump(pbuftoencode, pbuf));
      break;
    case NETWIB_ENCODETYPE_LOWERCASE :
      netwib_er(netwib_priv_buf_encode_lowercase(pbuftoencode, pbuf));
      break;
    case NETWIB_ENCODETYPE_UPPERCASE :
      netwib_er(netwib_priv_buf_encode_uppercase(pbuftoencode, pbuf));
      break;
    case NETWIB_ENCODETYPE_MIXED0H_WRAP :
      netwib_er(netwib_priv_buf_encode_mixedh_wrap(pbuftoencode, 0, pbuf));
      break;
    case NETWIB_ENCODETYPE_MIXED1H_WRAP :
      netwib_er(netwib_priv_buf_encode_mixedh_wrap(pbuftoencode, 1, pbuf));
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_PRIV_TRANSITIONTYPE_INIT,
  NETWIB_PRIV_TRANSITIONTYPE_END,
  NETWIB_PRIV_TRANSITIONTYPE_NOTHING,
  NETWIB_PRIV_TRANSITIONTYPE_DUMP, /* wrap, array */
  NETWIB_PRIV_TRANSITIONTYPE_SYNTH,
  NETWIB_PRIV_TRANSITIONTYPE_DATA,
  NETWIB_PRIV_TRANSITIONTYPE_TEXT,
  NETWIB_PRIV_TRANSITIONTYPE_HEXA0, /* base64 */
  NETWIB_PRIV_TRANSITIONTYPE_HEXA1  /* hexan, mixed */
} netwib_priv_transitiontype;

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_transitiontype_init(netwib_encodetype encodetype,
                                                  netwib_priv_transitiontype *ptransitiontype)
{

  switch(encodetype) {
  case NETWIB_ENCODETYPE_TRANSITION_INIT :
    *ptransitiontype = NETWIB_PRIV_TRANSITIONTYPE_INIT;
    break;
  case NETWIB_ENCODETYPE_TRANSITION_END :
    *ptransitiontype = NETWIB_PRIV_TRANSITIONTYPE_END;
    break;
  case NETWIB_ENCODETYPE_NOTHING :
    *ptransitiontype = NETWIB_PRIV_TRANSITIONTYPE_NOTHING;
    break;
  case NETWIB_ENCODETYPE_DATA_WRAP :
  case NETWIB_ENCODETYPE_HEXA0_WRAP :
  case NETWIB_ENCODETYPE_HEXA1_WRAP :
  case NETWIB_ENCODETYPE_HEXA2_WRAP :
  case NETWIB_ENCODETYPE_HEXA4_WRAP :
  case NETWIB_ENCODETYPE_MIXED0_WRAP :
  case NETWIB_ENCODETYPE_MIXED1_WRAP :
  case NETWIB_ENCODETYPE_TEXT_WRAP :
  case NETWIB_ENCODETYPE_BASE64_WRAP :
  case NETWIB_ENCODETYPE_ARRAY1 :
  case NETWIB_ENCODETYPE_ARRAY4 :
  case NETWIB_ENCODETYPE_ARRAY8 :
  case NETWIB_ENCODETYPE_ARRAY16 :
  case NETWIB_ENCODETYPE_ARRAY32 :
  case NETWIB_ENCODETYPE_DUMP :
  case NETWIB_ENCODETYPE_MIXED0H_WRAP :
  case NETWIB_ENCODETYPE_MIXED1H_WRAP :
    *ptransitiontype = NETWIB_PRIV_TRANSITIONTYPE_DUMP;
    break;
  case NETWIB_ENCODETYPE_SYNTH :
    *ptransitiontype = NETWIB_PRIV_TRANSITIONTYPE_SYNTH;
    break;
  case NETWIB_ENCODETYPE_DATA :
    *ptransitiontype = NETWIB_PRIV_TRANSITIONTYPE_DATA;
    break;
  case NETWIB_ENCODETYPE_TEXT :
  case NETWIB_ENCODETYPE_LOWERCASE :
  case NETWIB_ENCODETYPE_UPPERCASE :
    *ptransitiontype = NETWIB_PRIV_TRANSITIONTYPE_TEXT;
    break;
  case NETWIB_ENCODETYPE_HEXA0 :
  case NETWIB_ENCODETYPE_BASE64 :
  case NETWIB_ENCODETYPE_QUOTED :
    *ptransitiontype = NETWIB_PRIV_TRANSITIONTYPE_HEXA0;
    break;
  case NETWIB_ENCODETYPE_HEXA1 :
  case NETWIB_ENCODETYPE_HEXA2 :
  case NETWIB_ENCODETYPE_HEXA4 :
  case NETWIB_ENCODETYPE_MIXED0 :
  case NETWIB_ENCODETYPE_MIXED1 :
    *ptransitiontype = NETWIB_PRIV_TRANSITIONTYPE_HEXA1;
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_encode_transition(netwib_encodetype_context *pctx,
                                        netwib_encodetype encodetype,
                                        netwib_buf *pbuf)
{
  netwib_priv_transitiontype transitiontype, lasttransitiontype=0;

  netwib_er(netwib_priv_transitiontype_init(encodetype, &transitiontype));
  if (transitiontype != NETWIB_PRIV_TRANSITIONTYPE_INIT) {
    netwib_er(netwib_priv_transitiontype_init(pctx->last,
                                              &lasttransitiontype));
  }

  switch(transitiontype) {
    case NETWIB_PRIV_TRANSITIONTYPE_INIT:
      pctx->last = encodetype;
      pctx->containnl = NETWIB_FALSE;
      break;
    case NETWIB_PRIV_TRANSITIONTYPE_END:
      switch(lasttransitiontype) {
        case NETWIB_PRIV_TRANSITIONTYPE_INIT:
        case NETWIB_PRIV_TRANSITIONTYPE_END:
        case NETWIB_PRIV_TRANSITIONTYPE_DUMP:
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_NOTHING:
          return(NETWIB_ERR_LOINTERNALERROR);
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_SYNTH:
        case NETWIB_PRIV_TRANSITIONTYPE_DATA:
        case NETWIB_PRIV_TRANSITIONTYPE_TEXT:
        case NETWIB_PRIV_TRANSITIONTYPE_HEXA0:
        case NETWIB_PRIV_TRANSITIONTYPE_HEXA1:
          netwib_er(netwib_buf_append_byte('\n', pbuf));
          break;
      }
      pctx->last = encodetype;
      break;
    case NETWIB_PRIV_TRANSITIONTYPE_NOTHING:
      break;
    case NETWIB_PRIV_TRANSITIONTYPE_DUMP:
      switch(lasttransitiontype) {
        case NETWIB_PRIV_TRANSITIONTYPE_INIT:
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_END:
          if (pctx->containnl) {
            netwib_er(netwib_buf_append_byte('\n', pbuf));
          }
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_NOTHING:
          return(NETWIB_ERR_LOINTERNALERROR);
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_DUMP:
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_SYNTH:
        case NETWIB_PRIV_TRANSITIONTYPE_DATA:
        case NETWIB_PRIV_TRANSITIONTYPE_TEXT:
        case NETWIB_PRIV_TRANSITIONTYPE_HEXA0:
        case NETWIB_PRIV_TRANSITIONTYPE_HEXA1:
          netwib_er(netwib_buf_append_byte('\n', pbuf));
          break;
      }
      pctx->last = encodetype;
      pctx->containnl = NETWIB_TRUE;
      break;
    case NETWIB_PRIV_TRANSITIONTYPE_SYNTH:
      switch(lasttransitiontype) {
        case NETWIB_PRIV_TRANSITIONTYPE_INIT:
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_END:
          if (pctx->containnl) {
            netwib_er(netwib_buf_append_byte('\n', pbuf));
            pctx->containnl = NETWIB_FALSE;
          }
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_NOTHING:
          return(NETWIB_ERR_LOINTERNALERROR);
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_DUMP:
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_DATA:
        case NETWIB_PRIV_TRANSITIONTYPE_TEXT:
          netwib_er(netwib_buf_append_byte('\n', pbuf));
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_SYNTH:
        case NETWIB_PRIV_TRANSITIONTYPE_HEXA0:
        case NETWIB_PRIV_TRANSITIONTYPE_HEXA1:
          netwib_er(netwib_buf_append_string(" - ", pbuf));
          break;
      }
      pctx->last = encodetype;
      break;
    case NETWIB_PRIV_TRANSITIONTYPE_DATA:
      switch(lasttransitiontype) {
        case NETWIB_PRIV_TRANSITIONTYPE_INIT:
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_END:
          if (pctx->containnl) {
            netwib_er(netwib_buf_append_byte('\n', pbuf));
          }
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_NOTHING:
          return(NETWIB_ERR_LOINTERNALERROR);
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_DUMP:
        case NETWIB_PRIV_TRANSITIONTYPE_DATA:
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_TEXT:
          netwib_er(netwib_buf_append_byte('\n', pbuf));
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_SYNTH:
        case NETWIB_PRIV_TRANSITIONTYPE_HEXA0:
        case NETWIB_PRIV_TRANSITIONTYPE_HEXA1:
          netwib_er(netwib_buf_append_byte('\n', pbuf));
          break;
      }
      pctx->containnl = NETWIB_TRUE;
      pctx->last = encodetype;
      break;
    case NETWIB_PRIV_TRANSITIONTYPE_TEXT:
      switch(lasttransitiontype) {
        case NETWIB_PRIV_TRANSITIONTYPE_INIT:
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_END:
          if (pctx->containnl) {
            netwib_er(netwib_buf_append_byte('\n', pbuf));
          }
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_NOTHING:
          return(NETWIB_ERR_LOINTERNALERROR);
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_DUMP:
        case NETWIB_PRIV_TRANSITIONTYPE_TEXT:
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_DATA:
          netwib_er(netwib_buf_append_byte('\n', pbuf));
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_SYNTH:
        case NETWIB_PRIV_TRANSITIONTYPE_HEXA0:
        case NETWIB_PRIV_TRANSITIONTYPE_HEXA1:
          netwib_er(netwib_buf_append_byte('\n', pbuf));
          break;
      }
      pctx->containnl = NETWIB_TRUE;
      pctx->last = encodetype;
      break;

    case NETWIB_PRIV_TRANSITIONTYPE_HEXA0:
      switch(lasttransitiontype) {
        case NETWIB_PRIV_TRANSITIONTYPE_INIT:
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_END:
          if (pctx->containnl) {
            netwib_er(netwib_buf_append_byte('\n', pbuf));
            pctx->containnl = NETWIB_FALSE;
          }
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_NOTHING:
          return(NETWIB_ERR_LOINTERNALERROR);
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_DUMP:
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_DATA:
        case NETWIB_PRIV_TRANSITIONTYPE_TEXT:
          netwib_er(netwib_buf_append_byte('\n', pbuf));
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_SYNTH:
          netwib_er(netwib_buf_append_string(" - ", pbuf));
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_HEXA0:
          netwib_er(netwib_buf_append_byte(' ', pbuf));
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_HEXA1:
          netwib_er(netwib_buf_append_byte('\n', pbuf));
          pctx->containnl = NETWIB_TRUE;
          break;
      }
      pctx->last = encodetype;
      break;
    case NETWIB_PRIV_TRANSITIONTYPE_HEXA1:
      switch(lasttransitiontype) {
        case NETWIB_PRIV_TRANSITIONTYPE_INIT:
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_END:
          if (pctx->containnl) {
            netwib_er(netwib_buf_append_byte('\n', pbuf));
            pctx->containnl = NETWIB_FALSE;
          }
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_NOTHING:
          return(NETWIB_ERR_LOINTERNALERROR);
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_DUMP:
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_DATA:
        case NETWIB_PRIV_TRANSITIONTYPE_TEXT:
          netwib_er(netwib_buf_append_byte('\n', pbuf));
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_SYNTH:
          netwib_er(netwib_buf_append_string(" - ", pbuf));
          break;
        case NETWIB_PRIV_TRANSITIONTYPE_HEXA0:
        case NETWIB_PRIV_TRANSITIONTYPE_HEXA1:
          netwib_er(netwib_buf_append_byte('\n', pbuf));
          pctx->containnl = NETWIB_TRUE;
          break;
      }
      pctx->last = encodetype;
      break;
  }

  return(NETWIB_ERR_OK);
}

