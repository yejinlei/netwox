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
#define NETWIB_BUF_TMPARR_SIZE 1024

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_buf_append_hexa(netwib_constbuf *phexa,
                                              netwib_buf *pbuf)
{
  netwib_byte data[NETWIB_BUF_TMPARR_SIZE], *pdata;
  netwib_uint32 datasize, quartet1, quartet;
  netwib_bool quartet1set;
  netwib_string pc;
  netwib_char c;
  netwib_err ret;

  /* obtain a reference to the string */
  netwib__constbuf_ref_string(phexa, pc, bufstorage,
                              netwib_priv_buf_append_hexa(&bufstorage, pbuf));

  /* main loop */
  pdata = data;
  datasize = 0;
  quartet1set = NETWIB_FALSE;
  quartet1 = '\0'; /* for compiler warning */
  while (NETWIB_TRUE) {
    /* get a char */
    c = *pc++;
    if (c == '\0') break;
    netwib_c2_cto16_if(c, quartet) else if (c == ' ' || c == '\t' ||
                                            c == '\r' || c == '\n') {
      /* skip spaces */
      continue;
    } else {
      netwib__localarray_ifbuf_wipe(phexa, data);
      return(NETWIB_ERR_PAHEXACHAR);
    }
    /* treat a quartet */
    if (quartet1set) {
      *pdata++ = (netwib_byte)((quartet1 << 4) | quartet);
      datasize++;
      quartet1set = NETWIB_FALSE;
      if (datasize == NETWIB_BUF_TMPARR_SIZE) {
        /* add the block */
        ret = netwib_buf_append_data(data, datasize, pbuf);
        if (ret != NETWIB_ERR_OK) {
          netwib__localarray_ifbuf_wipe(phexa, data);
          return(ret);
        }
        pdata = data;
        datasize = 0;
      }
    } else {
      quartet1 = quartet;
      quartet1set = NETWIB_TRUE;
    }
  }

  /* here, it should not be odd */
  if (quartet1set) {
    netwib__localarray_ifbuf_wipe(phexa, data);
    return(NETWIB_ERR_PAHEXAODD);
  }

  /* add the last part */
  ret = netwib_buf_append_data(data, datasize, pbuf);
  netwib__localarray_ifbuf_wipe(phexa, data);
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_buf_append_mixed(netwib_constbuf *pmixed,
                                               netwib_buf *pbuf)
{
  netwib_byte data[NETWIB_BUF_TMPARR_SIZE], *pdata;
  netwib_uint32 datasize, quartet1, quartet;
  netwib_bool quartet1set, modehexa;
  netwib_string pc;
  netwib_char c;
  netwib_err ret;

  /* obtain a reference to the string */
  netwib__constbuf_ref_string(pmixed, pc, bufstorage,
                              netwib_priv_buf_append_mixed(&bufstorage, pbuf));

  /* main loop */
  pdata = data;
  datasize = 0;
  quartet1set = NETWIB_FALSE;
  modehexa = NETWIB_TRUE;
  quartet1 = '\0'; /* for compiler warning */
  while (NETWIB_TRUE) {
    /* get a char */
    c = *pc++;
    if (c == '\0') break;
    if (modehexa) {
      /* hexa */
      netwib_c2_cto16_if(c, quartet) else if (c == ' ' || c == '\t' ||
                                              c == '\r' || c == '\n') {
        /* skip spaces */
        continue;
      } else if (c == '\'') {
        if (quartet1set) {
          netwib__localarray_ifbuf_wipe(pmixed, data);
          return(NETWIB_ERR_PAHEXAODD);
        }
        modehexa = NETWIB_FALSE;
        continue;
      } else {
        netwib__localarray_ifbuf_wipe(pmixed, data);
        return(NETWIB_ERR_PAHEXACHAR);
      }
      /* treat a quartet */
      if (quartet1set) {
        *pdata++ = (netwib_byte)((quartet1 << 4) | quartet);
        datasize++;
        quartet1set = NETWIB_FALSE;
        if (datasize == NETWIB_BUF_TMPARR_SIZE) {
          /* add the block */
          netwib_er(netwib_buf_append_data(data, datasize, pbuf));
          pdata = data;
          datasize = 0;
        }
      } else {
        quartet1 = quartet;
        quartet1set = NETWIB_TRUE;
      }
    } else {
      /* mixed */
      if (c == '\'' ) {
        if (*pc == '\'') { /* pc currently points to next
                              char (or perhaps '\0') */
          pc++;
          *pdata++ = '\'';
          datasize++;
        } else {
          modehexa = NETWIB_TRUE;
          continue;
        }
      } else {
        *pdata++ = c;
        datasize++;
      }
      if (datasize == NETWIB_BUF_TMPARR_SIZE) {
        /* add the block */
        ret = netwib_buf_append_data(data, datasize, pbuf);
        if (ret != NETWIB_ERR_OK) {
          netwib__localarray_ifbuf_wipe(pmixed, data);
          return(ret);
        }
        pdata = data;
        datasize = 0;
      }
    }
  }

  /* mixed mode is forbidden here */
  if (!modehexa) {
    netwib__localarray_ifbuf_wipe(pmixed, data);
    return(NETWIB_ERR_PAMIXED);
  }
  /* here, it should not be odd */
  if (modehexa && quartet1set) {
    netwib__localarray_ifbuf_wipe(pmixed, data);
    return(NETWIB_ERR_PAHEXAODD);
  }

  /* add the last part */
  ret = netwib_buf_append_data(data, datasize, pbuf);
  netwib__localarray_ifbuf_wipe(pmixed, data);
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_buf_append_base64(netwib_constbuf *pbase64,
                                                netwib_buf *pbuf)
{
  netwib_byte data[NETWIB_BUF_TMPARR_SIZE], *pdata;
  netwib_uint32 datasize, b, bytecur, byte0, byte1, byte2;
  netwib_string pc;
  netwib_char c;
  netwib_bool continuetoloop;
  netwib_err ret;

  /* obtain a reference to the string */
  netwib__constbuf_ref_string(pbase64, pc, bufstorage,
                              netwib_priv_buf_append_base64(&bufstorage, pbuf));

  /* main loop */
  pdata = data;
  datasize = 0;
  byte0 = 0;
  byte1 = 0;
  byte2 = 0;
  bytecur = 0;
  b = 0;
  continuetoloop = NETWIB_TRUE;
  while (continuetoloop) {
    /* get a char */
    c = *pc++;
    if (c == '\0') {
      if (bytecur != 0) {
        /* string's size must a multiple of 4 */
        netwib__localarray_ifbuf_wipe(pbase64, data);
        return(NETWIB_ERR_PABASE64X4);
      }
      continuetoloop = NETWIB_FALSE;
      continue;
    }
    /* set b to its value */
    if (c >= 'A' && c <= 'Z') {
      b = c - 'A';
    } else if (c >= 'a' && c <= 'z') {
      b = c - 'a' + 26;
    } else if (c >= '0' && c <= '9') {
      b = c - '0' + 52;
    } else if (c == '+') {
      b = 62;
    } else if (c == '/') {
      b = 63;
    } else if (c == '=') {
      b = 64; /* means a '=' was found */
    } else {
      netwib__localarray_ifbuf_wipe(pbase64, data);
      return(NETWIB_ERR_PABASE64CHAR);
    }
    /* set corresponding byte */
    switch(bytecur) {
      case 0 :
        if (b == 64) {
          /* end "=..." is not valid */
          netwib__localarray_ifbuf_wipe(pbase64, data);
          return(NETWIB_ERR_PABASE64LO);
        }
        byte0 = b << 2;
        bytecur = 1;
        break;
      case 1 :
        if (b == 64) {
          /* end ".=.." is not valid */
          netwib__localarray_ifbuf_wipe(pbase64, data);
          return(NETWIB_ERR_PABASE64LO);
        }
        byte0 |= b >> 4;
        byte1 = (b&0xF) << 4;
        bytecur = 2;
        break;
      case 2 :
        if (b == 64) {
          if (*pc++ != '=') {
            /* only "..==" is valid */
            netwib__localarray_ifbuf_wipe(pbase64, data);
            return(NETWIB_ERR_PABASE64LO);
          }
          if (*pc != '\0') {
            /* only "..==" is valid */
            netwib__localarray_ifbuf_wipe(pbase64, data);
            return(NETWIB_ERR_PABASE64LO);
          }
          if (byte1 != 0) {
            /* when "..==" is used, the second byte must be null */
            netwib__localarray_ifbuf_wipe(pbase64, data);
            return(NETWIB_ERR_PABASE64LO);
          }
          *pdata++ = (netwib_byte)byte0;
          datasize++;
          continuetoloop = NETWIB_FALSE;
          continue;
        }
        byte1 |= b >> 2;
        byte2 = (b&3) << 6;
        bytecur = 3;
        break;
      case 3 :
        if (b == 64) {
          if (*pc != '\0' ) {
            /* only "...=" is valid */
            netwib__localarray_ifbuf_wipe(pbase64, data);
            return(NETWIB_ERR_PABASE64LO);
          }
          if (byte2 != 0) {
            /* when "...=" is used, the third byte must be null */
            netwib__localarray_ifbuf_wipe(pbase64, data);
            return(NETWIB_ERR_PABASE64LO);
          }
          *pdata++ = (netwib_byte)byte0;
          *pdata++ = (netwib_byte)byte1;
          datasize += 2;
          continuetoloop = NETWIB_FALSE;
          continue;
        }
        byte2 |= b;
        /* set data */
        *pdata++ = (netwib_byte)byte0;
        *pdata++ = (netwib_byte)byte1;
        *pdata++ = (netwib_byte)byte2;
        datasize += 3;
        if (datasize > NETWIB_BUF_TMPARR_SIZE - 4) {
          /* add the block */
          ret = netwib_buf_append_data(data, datasize, pbuf);
          if (ret != NETWIB_ERR_OK) {
            netwib__localarray_ifbuf_wipe(pbase64, data);
            return(ret);
          }
          pdata = data;
          datasize = 0;
        }
        bytecur = 0;
        break;
    }
  }

  /* add the last part */
  ret = netwib_buf_append_data(data, datasize, pbuf);
  netwib__localarray_ifbuf_wipe(pbase64, data);
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_decode(netwib_constbuf *pbuftodecode,
                             netwib_decodetype decodetype,
                             netwib_buf *pbuf)
{
  netwib_buf buf;
  netwib_uint32 savedsize;
  netwib_err ret;

  if (pbuftodecode == NULL) {
    /* append nothing */
    return(NETWIB_ERR_OK);
  }

  if (pbuf == NULL) {
    /* we work on a fake buf, just to test error */
    netwib_er(netwib_buf_init_mallocdefault(&buf));
    ret = netwib_buf_decode(pbuftodecode, decodetype, &buf);
    netwib_er(netwib_buf_close(&buf));
    return(ret);
  }

  /* output is sensitive even if this function fails */
  netwib__buf_transfersensitive(pbuftodecode, pbuf);

  /* save position in case of error */
  savedsize = netwib__buf_ref_data_size(pbuf);

  /* call main function */
  switch(decodetype) {
    case NETWIB_DECODETYPE_DATA :
      ret = netwib_buf_append_buf(pbuftodecode, pbuf);
      break;
    case NETWIB_DECODETYPE_HEXA :
      ret = netwib_priv_buf_append_hexa(pbuftodecode, pbuf);
      break;
    case NETWIB_DECODETYPE_MIXED :
      ret = netwib_priv_buf_append_mixed(pbuftodecode, pbuf);
      break;
    case NETWIB_DECODETYPE_BASE64 :
      ret = netwib_priv_buf_append_base64(pbuftodecode, pbuf);
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  /* on error, restore first pos */
  if (ret != NETWIB_ERR_OK) {
    pbuf->endoffset = pbuf->beginoffset + savedsize;
  }
  return(ret);
}


/*-------------------------------------------------------------*/
netwib_err netwib_buf_decode_quoted(netwib_buf *pbuftodecode,
                                    netwib_buf *pbuf)
{
  netwib_data datain, dataout, datainsave, dataoutsave;
  netwib_uint32 datainsize, tmpval;
  netwib_bool foundlast;
  netwib_byte c;

  /* obtain references */
  datainsize = netwib__buf_ref_data_size(pbuftodecode);
  datain = netwib__buf_ref_data_ptr(pbuftodecode);
  netwib_er(netwib_buf_wantspace(pbuf, datainsize, &dataout));
  dataoutsave = dataout;
  datainsave = datain;

  netwib__buf_transfersensitive(pbuftodecode, pbuf);

  /* skip leading spaces */
  while (datainsize) {
    c = *datain;
    if ( ! (c == ' ' || c == '\t') ) {
      break;
    }
    datain++;
    datainsize--;
  }
  if (datainsize == 0) {
    return(NETWIB_ERR_DATAEND);
  }

  /* set data */
  if (*datain == '"') {
    datain++;
    datainsize--;
    foundlast = NETWIB_FALSE;
    while (datainsize) {
      c = *datain++;
      datainsize--;
      if (c == '"') {
        foundlast = NETWIB_TRUE;
        break;
      } else if (c == '\\') {
        if (datainsize == 0) return(NETWIB_ERR_DATANOTAVAIL);
        c = *datain++;
        datainsize--;
        if (c == 'a') {
          *dataout++ = 0x07;
        } else if (c == 'b') {
          *dataout++ = '\b';
        } else if (c == 't') {
          *dataout++ = '\t';
        } else if (c == 'n') {
          *dataout++ = '\n';
        } else if (c == 'r') {
          *dataout++ = '\r';
        } else if (c == 'x') {
          if (datainsize == 0) return(NETWIB_ERR_DATANOTAVAIL);
          c = *datain++;
          datainsize--;
          if (!netwib_c2_isxdigit(c)) return(NETWIB_ERR_NOTCONVERTED);
          tmpval = netwib_c2_cto16(c);
          if (datainsize == 0) return(NETWIB_ERR_DATANOTAVAIL);
          c = *datain++;
          datainsize--;
          if (!netwib_c2_isxdigit(c)) return(NETWIB_ERR_NOTCONVERTED);
          tmpval = tmpval<<4 | netwib_c2_cto16(c);
          *dataout++ = (netwib_byte)tmpval;
        } else {
          *dataout++ = c;
        }
      } else {
        *dataout++ = c;
      }
    }
    if (!foundlast) {
      return(NETWIB_ERR_DATANOTAVAIL);
    }
  } else {
    while (datainsize) {
      c = *datain;
      if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
        break;
      }
      *dataout++ = c;
      datain++;
      datainsize--;
    }
  }

  /* skip ending spaces */
  while (datainsize) {
    c = *datain;
    if ( ! (c == ' ' || c == '\t' || c == '\r' || c == '\n') ) {
      break;
    }
    datain++;
    datainsize--;
  }

  /* update pquoted */
  pbuftodecode->beginoffset += datain - datainsave;

  /* indicate end */
  pbuf->endoffset += dataout - dataoutsave;

  return(NETWIB_ERR_OK);
}
