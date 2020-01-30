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
#define NETWIB_PRIV_RECORD_ENCODETYPE_MAX 40
netwib_err netwib_record_encodetype_init_kbd(netwib_constbuf *pmessage,
                                             netwib_bool displayonlymostuseful,
                                             netwib_record_encodetype defaultencodetype,
                                             netwib_record_encodetype *pencodetype)
{
  netwib_record_encodetype array[NETWIB_PRIV_RECORD_ENCODETYPE_MAX];
  netwib_buf msg, buf;
  netwib_uint32 i, choice, defaultchoice;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  if (pmessage != NULL) {
    netwib_er(netwib_buf_append_buf(pmessage, &buf));
    netwib_er(netwib_buf_append_string("\n", &buf));
  } else {
    netwib_er(netwib_buf_append_string("Select how data will be displayed :\n",
                                     &buf));
  }

  i = 0;
  defaultchoice = NETWIB_UINT32_INIT_KBD_NODEF;

#define netwib_recordet_li(ot) {netwib_er(netwib_buf_append_fmt(&buf, " %{r 2;uint32} - ", i)); netwib_er(netwib_buf_append_record_encodetype(ot, &buf)); netwib_er(netwib_buf_append_fmt(&buf, "\n")); if (defaultencodetype == ot) defaultchoice = i; array[i++] = ot; }

  if (displayonlymostuseful) {
    netwib_recordet_li(NETWIB_RECORD_ENCODETYPE_DUMP);
    netwib_recordet_li(NETWIB_RECORD_ENCODETYPE_HEXA);
    netwib_recordet_li(NETWIB_RECORD_ENCODETYPE_HEXA_WRAP);
    netwib_recordet_li(NETWIB_RECORD_ENCODETYPE_MIXED);
    netwib_recordet_li(NETWIB_RECORD_ENCODETYPE_MIXED_WRAP);
    netwib_recordet_li(NETWIB_RECORD_ENCODETYPE_MIXEDH_WRAP);
    netwib_recordet_li(NETWIB_RECORD_ENCODETYPE_BIN);
    netwib_recordet_li(NETWIB_RECORD_ENCODETYPE_PCAP);
  } else {
    netwib_recordet_li(NETWIB_RECORD_ENCODETYPE_DUMP);
    netwib_recordet_li(NETWIB_RECORD_ENCODETYPE_HEXA0);
    netwib_recordet_li(NETWIB_RECORD_ENCODETYPE_HEXA1);
    netwib_recordet_li(NETWIB_RECORD_ENCODETYPE_HEXA2);
    netwib_recordet_li(NETWIB_RECORD_ENCODETYPE_HEXA4);
    netwib_recordet_li(NETWIB_RECORD_ENCODETYPE_HEXA0_WRAP);
    netwib_recordet_li(NETWIB_RECORD_ENCODETYPE_HEXA1_WRAP);
    netwib_recordet_li(NETWIB_RECORD_ENCODETYPE_HEXA2_WRAP);
    netwib_recordet_li(NETWIB_RECORD_ENCODETYPE_HEXA4_WRAP);
    netwib_recordet_li(NETWIB_RECORD_ENCODETYPE_MIXED0);
    netwib_recordet_li(NETWIB_RECORD_ENCODETYPE_MIXED1);
    netwib_recordet_li(NETWIB_RECORD_ENCODETYPE_MIXED0_WRAP);
    netwib_recordet_li(NETWIB_RECORD_ENCODETYPE_MIXED1_WRAP);
    netwib_recordet_li(NETWIB_RECORD_ENCODETYPE_MIXED0H_WRAP);
    netwib_recordet_li(NETWIB_RECORD_ENCODETYPE_MIXED1H_WRAP);
    netwib_recordet_li(NETWIB_RECORD_ENCODETYPE_BIN);
    netwib_recordet_li(NETWIB_RECORD_ENCODETYPE_PCAP);
  }

  if (i >= NETWIB_PRIV_RECORD_ENCODETYPE_MAX) {
    return(NETWIB_ERR_LOINTERNALERROR);
  }
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_buf_close(&buf));

  if (defaultencodetype == NETWIB_RECORD_ENCODETYPE_INIT_KBD_NODEF) {
    defaultchoice = NETWIB_UINT32_INIT_KBD_NODEF;
  }

  netwib_er(netwib_buf_init_ext_string("Your choice", &msg));
  netwib_er(netwib_uint32_init_kbd(&msg, 0, i-1, defaultchoice,
                                   &choice));

  if (pencodetype != NULL) *pencodetype = array[choice];
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_record_encodetype(netwib_record_encodetype encodetype,
                                               netwib_buf *pbuf)
{
  netwib_conststring pc;

  pc = NULL;

  switch (encodetype) {
  case NETWIB_RECORD_ENCODETYPE_HEXA0:
    pc = "hexadecimal (not spaced)";
    break;
  case NETWIB_RECORD_ENCODETYPE_HEXA1:
    pc = "hexadecimal";
    break;
  case NETWIB_RECORD_ENCODETYPE_HEXA2:
    pc = "hexadecimal (spaced every two byte)";
    break;
  case NETWIB_RECORD_ENCODETYPE_HEXA4:
    pc = "hexadecimal (spaced every four byte)";
    break;
  case NETWIB_RECORD_ENCODETYPE_MIXED0:
    pc = "mixed (not spaced)";
    break;
  case NETWIB_RECORD_ENCODETYPE_MIXED1:
    pc = "mixed";
    break;
  case NETWIB_RECORD_ENCODETYPE_HEXA0_WRAP:
    pc = "wrapped hexa (not spaced, 32 bytes per line)";
    break;
  case NETWIB_RECORD_ENCODETYPE_HEXA1_WRAP:
    pc = "wrapped hexa";
    break;
  case NETWIB_RECORD_ENCODETYPE_HEXA2_WRAP:
    pc = "wrapped hexa (spaced 2, 32 bytes per line)";
    break;
  case NETWIB_RECORD_ENCODETYPE_HEXA4_WRAP:
    pc = "wrapped hexa (spaced 4, 32 bytes per line)";
    break;
  case NETWIB_RECORD_ENCODETYPE_MIXED0_WRAP:
    pc = "wrapped mixed (not spaced, 16 bytes per line)";
    break;
  case NETWIB_RECORD_ENCODETYPE_MIXED1_WRAP:
    pc = "wrapped mixed";
    break;
  case NETWIB_RECORD_ENCODETYPE_DUMP:
    pc = "dump";
    break;
  case NETWIB_RECORD_ENCODETYPE_MIXED0H_WRAP:
    pc = "wrapped mixed (not spaced), with commented hexa";
    break;
  case NETWIB_RECORD_ENCODETYPE_MIXED1H_WRAP:
    pc = "wrapped mixed, with commented hexa";
    break;
  case NETWIB_RECORD_ENCODETYPE_BIN:
    pc = "binary";
    break;
  case NETWIB_RECORD_ENCODETYPE_PCAP:
    pc = "libpcap format";
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  netwib_er(netwib_buf_append_string(pc, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_PRIV_IO_RECORD_FMT_MIXED = 1,
  NETWIB_PRIV_IO_RECORD_FMT_BIN,
  NETWIB_PRIV_IO_RECORD_FMT_PCAP
} netwib_priv_io_record_fmt;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_io *pio;
  netwib_priv_io_record_fmt fmt;
  netwib_file_inittype inittype;
  netwib_record_encodetype encodetype;
  netwib_priv_libpcap libpcap;
} netwib_priv_io_record;

/*-------------------------------------------------------------*/
#define NETWIB_PRIV_IO_RECORD_MAGIC 0xa84c1fe6u

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_record_readfmt(netwib_constbuf *precordname,
                                                netwib_priv_io_record_fmt *pfmt)
{
  netwib_io *pio;
  netwib_buf buf;
  netwib_data data;
  netwib_uint32 datasize, num;
  netwib_err ret;

  *pfmt = NETWIB_PRIV_IO_RECORD_FMT_MIXED;

  netwib_er(netwib_io_init_file_read(precordname, &pio));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  ret = netwib_io_read(pio, &buf);
  if (ret == NETWIB_ERR_OK) {
    datasize = netwib__buf_ref_data_size(&buf);
    if (datasize > 3) {
      data = netwib__buf_ref_data_ptr(&buf);
      num = netwib_c2_uint32_4(data[0], data[1], data[2], data[3]);
      /* libpcap file format start with 0xa1b2c3d4 or 0xa1b2cd34, or reverse */
      if (num == 0xa1b2c3d4u || num == 0xa1b2cd34u ||
          num == 0xd4c3b2a1u || num == 0x34cdb2a1u) {
        *pfmt = NETWIB_PRIV_IO_RECORD_FMT_PCAP;
      } else if (num == NETWIB_PRIV_IO_RECORD_MAGIC) {
        *pfmt = NETWIB_PRIV_IO_RECORD_FMT_BIN;
      }
    }
  }
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_io_close(&pio));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_bin_write_uint32(netwib_io *pio,
                                               netwib_uint32 ui)
{
  netwib_byte array[4];
  netwib_buf buf;
  netwib_err ret;

  array[0] = netwib_c2_uint32_0(ui);
  array[1] = netwib_c2_uint32_1(ui);
  array[2] = netwib_c2_uint32_2(ui);
  array[3] = netwib_c2_uint32_3(ui);
  netwib_er(netwib_buf_init_ext_arrayfilled(array, 4, &buf));
  ret = netwib_io_write(pio, &buf);

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_bin_read_uint32(netwib_io *pio,
                                              netwib_uint32 *pui)
{
  netwib_byte array[4];
  netwib_buf buf;
  netwib_data data;
  netwib_uint32 datasize;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));
  netwib_er(netwib_io_ctl_set_data_fixed_size(pio, NETWIB_IO_WAYTYPE_READ, 4));
  ret = netwib_io_read(pio, &buf);
  if (ret == NETWIB_ERR_OK) {
    datasize = netwib__buf_ref_data_size(&buf);
    if (datasize == 4) {
      if (pui != NULL) {
        data = netwib__buf_ref_data_ptr(&buf);
        *pui = netwib_c2_uint32_4(data[0], data[1], data[2], data[3]);
      }
    } else {
      return(NETWIB_ERR_LOINTERNALERROR);
    }
  }

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_record_init(netwib_constbuf *precordname,
                                             netwib_file_inittype inittype,
                                             netwib_record_encodetype encodetype,
                                             netwib_bool *preadinitialized,
                                             netwib_bool *pwriteinitialized,
                                             netwib_priv_io_record *ptr)
{
  netwib_io *pfileio;
  netwib_buf buf;
  netwib_err ret;

  ptr->inittype = inittype;
  ptr->encodetype = encodetype;

  switch(ptr->inittype) {
    case NETWIB_FILE_INITTYPE_READ :
      netwib_er(netwib_priv_io_record_readfmt(precordname, &ptr->fmt));
      switch(ptr->fmt) {
        case NETWIB_PRIV_IO_RECORD_FMT_MIXED :
          netwib_er(netwib_io_init_file_read(precordname, &pfileio));
          netwib_er(netwib_io_init_data_line(&ptr->pio));
          netwib_er(netwib_io_plug_read(ptr->pio, pfileio));
          break;
        case NETWIB_PRIV_IO_RECORD_FMT_BIN :
          netwib_er(netwib_io_init_file_read(precordname, &pfileio));
          netwib_er(netwib_io_init_data_fixed(&ptr->pio));
          netwib_er(netwib_io_plug_read(ptr->pio, pfileio));
          netwib_er(netwib_priv_bin_read_uint32(ptr->pio, NULL));
          break;
        case NETWIB_PRIV_IO_RECORD_FMT_PCAP :
          /* done after malloc */
          break;
      }
      *preadinitialized = NETWIB_TRUE;
      *pwriteinitialized = NETWIB_FALSE;
      break;
    case NETWIB_FILE_INITTYPE_WRITE :
      switch(ptr->encodetype) {
        case NETWIB_RECORD_ENCODETYPE_BIN :
          ptr->fmt = NETWIB_PRIV_IO_RECORD_FMT_BIN;
          netwib_er(netwib_io_init_file_write(precordname, &ptr->pio));
          netwib_er(netwib_priv_bin_write_uint32(ptr->pio,
                                                 NETWIB_PRIV_IO_RECORD_MAGIC));
          break;
        case NETWIB_RECORD_ENCODETYPE_PCAP :
          ptr->fmt = NETWIB_PRIV_IO_RECORD_FMT_PCAP;
          /* done after malloc */
          break;
        default :
          ptr->fmt = NETWIB_PRIV_IO_RECORD_FMT_MIXED;
          netwib_er(netwib_io_init_file_write(precordname, &ptr->pio));
          netwib_er(netwib_buf_init_ext_string("#The format used in this file is :\n# what is following a # is a comment\n# 01 DB 4c8e # hexadecimal data\n# 'string with a '' in the middle'\n# 'mixed' 03 4D 'line' DEFF 01\n# a blank line separates two packets\n", &buf));
          netwib_er(netwib_io_write(ptr->pio, &buf));
          break;
      }
      *preadinitialized = NETWIB_FALSE;
      *pwriteinitialized = NETWIB_TRUE;
      break;
    case NETWIB_FILE_INITTYPE_APPEND :
      netwib_er(netwib_priv_io_record_readfmt(precordname, &ptr->fmt));
      switch(ptr->fmt) {
        case NETWIB_PRIV_IO_RECORD_FMT_MIXED :
          netwib_er(netwib_io_init_file_append(precordname, &ptr->pio));
          ptr->encodetype = NETWIB_RECORD_ENCODETYPE_MIXED1H_WRAP;
          break;
        case NETWIB_PRIV_IO_RECORD_FMT_BIN :
          netwib_er(netwib_io_init_file_append(precordname, &ptr->pio));
          break;
        case NETWIB_PRIV_IO_RECORD_FMT_PCAP :
          /* we could do it by opening a new file, and copying
             inside data from the pcap file, and then do as for a write */
          return(NETWIB_ERR_LONOTIMPLEMENTED);
          break;
      }
      *preadinitialized = NETWIB_FALSE;
      *pwriteinitialized = NETWIB_TRUE;
      break;
    case NETWIB_FILE_INITTYPE_RDWR :
      /* continue */
    default :
      return(NETWIB_ERR_LONOTSUPPORTED);
  }

  /* we can only set pcap info now (after allocating) */
  ret = NETWIB_ERR_OK;
  if (ptr->fmt == NETWIB_PRIV_IO_RECORD_FMT_PCAP) {
    if (ptr->inittype == NETWIB_FILE_INITTYPE_READ ) {
      ret = netwib_priv_libpcap_init_read(precordname, &ptr->libpcap);
    } else if (ptr->inittype == NETWIB_FILE_INITTYPE_WRITE ) {
      ret = netwib_priv_libpcap_init_write(precordname, &ptr->libpcap);
    }
  }

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_record_close(netwib_priv_io_record *ptr)
{

  switch(ptr->fmt) {
    case NETWIB_PRIV_IO_RECORD_FMT_MIXED :
      netwib_er(netwib_io_close(&ptr->pio));
      break;
    case NETWIB_PRIV_IO_RECORD_FMT_BIN :
      netwib_er(netwib_io_close(&ptr->pio));
      break;
    case NETWIB_PRIV_IO_RECORD_FMT_PCAP :
      netwib_er(netwib_priv_libpcap_close(&ptr->libpcap));
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
#define NETWIB_BUF_MIXED_TMPARR_SIZE 1024
static netwib_err netwib_priv_io_record_read_mixed2(netwib_buf *pmixed,
                                                    netwib_bool *pemptyline,
                                                    netwib_buf *pbuf)
{
  netwib_byte data[NETWIB_BUF_MIXED_TMPARR_SIZE], *pdata;
  netwib_uint32 datasize, quartet1, quartet;
  netwib_bool quartet1set, modehexa;
  netwib_string pc;
  netwib_char c;

  /* obtain a reference to the string */
  netwib_er(netwib_buf_ref_string(pmixed, &pc));

  /* main loop */
  pdata = data;
  datasize = 0;
  quartet1set = NETWIB_FALSE;
  modehexa = NETWIB_TRUE;
  *pemptyline = NETWIB_TRUE;
  quartet1 = '\0'; /* for compiler warning */
  while (NETWIB_TRUE) {
    /* get a char */
    c = *pc++;
    if (c == '\0') break;
    if (modehexa) {
      /* start of comment */
      if (c == '#') {
        *pemptyline = NETWIB_FALSE;
        break;
      }
      /* hexa */
      netwib_c2_cto16_if(c, quartet) else if (c == ' ' || c == '\t') {
        /* skip spaces */
        continue;
      } else if (c == '\'') {
        if (quartet1set) {
          return(NETWIB_ERR_PAHEXAODD);
        }
        modehexa = NETWIB_FALSE;
        continue;
      } else {
        return(NETWIB_ERR_PAHEXACHAR);
      }
      /* treat a quartet */
      if (quartet1set) {
        *pdata++ = (netwib_byte)((quartet1 << 4) | quartet);
        datasize++;
        quartet1set = NETWIB_FALSE;
        if (datasize == NETWIB_BUF_MIXED_TMPARR_SIZE) {
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
      if (datasize == NETWIB_BUF_MIXED_TMPARR_SIZE) {
        /* add the block */
        netwib_er(netwib_buf_append_data(data, datasize, pbuf));
        pdata = data;
        datasize = 0;
        *pemptyline = NETWIB_FALSE;
      }
    }
  }

  /* mixed mode is forbidden here */
  if (!modehexa) {
    return(NETWIB_ERR_PAMIXED);
  }
  /* here, it should not be odd */
  if (modehexa && quartet1set) {
    return(NETWIB_ERR_PAHEXAODD);
  }

  /* add the last part */
  if (datasize) {
    netwib_er(netwib_buf_append_data(data, datasize, pbuf));
    *pemptyline = NETWIB_FALSE;
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_record_read_mixed(netwib_io *pio,
                                                   netwib_buf *pbuf)
{
  netwib_buf buf;
  netwib_bool emptyline;
  netwib_err ret;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_io_read(pio, &buf);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) {
        /* allow badly formatted records (not ending by a newline) */
        if (netwib__buf_ref_data_size(pbuf)) {
          ret = NETWIB_ERR_OK;
        }
      }
      break;
    }
    ret = netwib_priv_io_record_read_mixed2(&buf, &emptyline, pbuf);
    if (ret != NETWIB_ERR_OK) break;
    if (emptyline) break;
    netwib__buf_reinit(&buf);
  }
  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_record_read(netwib_io *pio,
                                             netwib_buf *pbuf)
{
  netwib_priv_io_record *ptr = (netwib_priv_io_record *)pio->pcommon;
  netwib_uint32 datasize;

  switch(ptr->fmt) {
    case NETWIB_PRIV_IO_RECORD_FMT_MIXED :
      netwib_er(netwib_priv_io_record_read_mixed(ptr->pio, pbuf));
      break;
    case NETWIB_PRIV_IO_RECORD_FMT_BIN :
      netwib_er(netwib_priv_bin_read_uint32(ptr->pio, &datasize));
      if (datasize) {
        netwib_er(netwib_io_ctl_set_data_fixed_size(ptr->pio,
                                                    NETWIB_IO_WAYTYPE_READ,
                                                    datasize));
        netwib_er(netwib_io_read(ptr->pio, pbuf));
      }
      break;
    case NETWIB_PRIV_IO_RECORD_FMT_PCAP :
      netwib_er(netwib_priv_libpcap_read(&ptr->libpcap, pbuf));
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_record_write(netwib_io *pio,
                                              netwib_constbuf *pbuf)
{
  netwib_priv_io_record *ptr = (netwib_priv_io_record *)pio->pcommon;
  netwib_uint32 datasize;
  netwib_buf buf;
  netwib_err ret;

  switch(ptr->fmt) {
    case NETWIB_PRIV_IO_RECORD_FMT_MIXED :
      netwib_er(netwib_buf_init_mallocdefault(&buf));
      /* this cast only works because numbers are the same */
      ret = netwib_buf_encode(pbuf, (netwib_encodetype)ptr->encodetype, &buf);
      if (ret == NETWIB_ERR_OK) {
        ret = netwib_io_write(ptr->pio, &buf);
      }
      netwib_er(netwib_buf_close(&buf));
      if (ret != NETWIB_ERR_OK) {
        return(ret);
      }
      switch(ptr->encodetype) {
        case NETWIB_RECORD_ENCODETYPE_HEXA0 :
        case NETWIB_RECORD_ENCODETYPE_HEXA1 :
        case NETWIB_RECORD_ENCODETYPE_HEXA2 :
        case NETWIB_RECORD_ENCODETYPE_HEXA4 :
        case NETWIB_RECORD_ENCODETYPE_MIXED0 :
        case NETWIB_RECORD_ENCODETYPE_MIXED1 :
          datasize = netwib__buf_ref_data_size(pbuf);
          if (datasize) {
            netwib_er(netwib_buf_init_ext_string("\n\n", &buf));
          } else {
            netwib_er(netwib_buf_init_ext_string("\n", &buf));
          }
          netwib_er(netwib_io_write(ptr->pio, &buf));
          break;
        default:
          netwib_er(netwib_buf_init_ext_string("\n", &buf));
          netwib_er(netwib_io_write(ptr->pio, &buf));
          break;
      }
      break;
    case NETWIB_PRIV_IO_RECORD_FMT_BIN :
      datasize = netwib__buf_ref_data_size(pbuf);
      netwib_er(netwib_priv_bin_write_uint32(ptr->pio, datasize));
      if (datasize) {
        netwib_er(netwib_io_write(ptr->pio, pbuf));
      }
      break;
    case NETWIB_PRIV_IO_RECORD_FMT_PCAP :
      netwib_er(netwib_priv_libpcap_write(&ptr->libpcap, pbuf));
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_record_wait(netwib_io *pio,
                                             netwib_io_waytype way,
                                             netwib_consttime *pabstime,
                                             netwib_bool *pevent)
{
  /* always return true because it's a file */
  *pevent = NETWIB_TRUE;

  pio = pio; /* for compiler warning */
  way = way; /* for compiler warning */
  pabstime = pabstime; /* for compiler warning */
  pevent = pevent; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_record_ctl_set(netwib_io *pio,
                                                netwib_io_waytype way,
                                                netwib_io_ctltype type,
                                                netwib_ptr p,
                                                netwib_uint32 ui)
{

  switch(type) {
    case NETWIB_IO_CTLTYPE_RES:
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
    case NETWIB_IO_CTLTYPE_END:
      if (way != NETWIB_IO_WAYTYPE_WRITE) return(NETWIB_ERR_PAINVALIDTYPE);
      netwib_er(netwib_io_ctl_set_support(pio, way, NETWIB_FALSE));
      return(NETWIB_ERR_OK);
      break;
    default:
      return(NETWIB_ERR_PLEASETRYNEXT);
  }

  way = way; /* for compiler warning */
  p = p; /* for compiler warning */
  ui = ui; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_record_ctl_get(netwib_io *pio,
                                                netwib_io_waytype way,
                                                netwib_io_ctltype type,
                                                netwib_ptr p,
                                                netwib_uint32 *pui)
{

  switch(type) {
    case NETWIB_IO_CTLTYPE_RES:
      if (pui != NULL) *pui = NETWIB_IO_RESTYPE_RECORD;
      return(NETWIB_ERR_OK);
    case NETWIB_IO_CTLTYPE_END:
      return(NETWIB_ERR_PAINVALIDTYPE);
    default:
      return(NETWIB_ERR_PLEASETRYNEXT);
  }

  pio = pio; /* for compiler warning */
  way = way; /* for compiler warning */
  p = p; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_record_fclose(netwib_io *pio)
{
  netwib_priv_io_record *ptr = (netwib_priv_io_record *)pio->pcommon;

  netwib_er(netwib_priv_io_record_close(ptr));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_record(netwib_constbuf *precordname,
                                 netwib_file_inittype inittype,
                                 netwib_record_encodetype encodetype,
                                 netwib_io **ppio)
{
  netwib_bool rdinit=NETWIB_FALSE, wrinit=NETWIB_FALSE;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_record), &pcommon));
  ret = netwib_priv_io_record_init(precordname, inittype, encodetype,
                                   &rdinit, &wrinit,
                                   (netwib_priv_io_record *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdinit, wrinit,
                           pcommon,
                           &netwib_priv_io_record_read,
                           &netwib_priv_io_record_write,
                           &netwib_priv_io_record_wait,
                           NULL, /* an io_record is not buffered,
                                    so unread is not supported */
                           &netwib_priv_io_record_ctl_set,
                           &netwib_priv_io_record_ctl_get,
                           &netwib_priv_io_record_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}
