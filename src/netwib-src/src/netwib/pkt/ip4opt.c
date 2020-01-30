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
netwib_err netwib_ip4opt_initdefault(netwib_ip4opttype ip4opttype,
                                     netwib_ip4opt *pip4opt)
{
  netwib_uint32 i;

  pip4opt->type = ip4opttype;
  switch(ip4opttype) {
  case NETWIB_IP4OPTTYPE_END :
    break;
  case NETWIB_IP4OPTTYPE_NOOP :
    break;
  case NETWIB_IP4OPTTYPE_RR :
    pip4opt->opt.rr.storagesize = 0;
    pip4opt->opt.rr.storedvalues = 0;
    for (i = 0; i < 9; i++) {
      netwib_er(netwib_ip_init_ip4(0, &pip4opt->opt.rr.ip[i]));
    }
    break;
  case NETWIB_IP4OPTTYPE_TIME :
    pip4opt->opt.time.storagesize = 0;
    pip4opt->opt.time.storedvalues = 0;
    pip4opt->opt.time.overflow = 0;
    pip4opt->opt.time.flag = NETWIB_IP4OPT_TIMEFLAG_TS;
    for (i = 0; i < 4; i++) {
      netwib_er(netwib_ip_init_ip4(0, &pip4opt->opt.time.ip[i]));
    }
    for (i = 0; i < 9; i++) {
      pip4opt->opt.time.timestamp[i] = 0;
    }
    break;
  case NETWIB_IP4OPTTYPE_LSRR :
    pip4opt->opt.lsrr.storagesize = 0;
    pip4opt->opt.lsrr.usedvalues = 0;
    for (i = 0; i < 9; i++) {
      netwib_er(netwib_ip_init_ip4(0, &pip4opt->opt.lsrr.ip[i]));
    }
    break;
  case NETWIB_IP4OPTTYPE_SSRR :
    pip4opt->opt.ssrr.storagesize = 0;
    pip4opt->opt.ssrr.usedvalues = 0;
    for (i = 0; i < 9; i++) {
      netwib_er(netwib_ip_init_ip4(0, &pip4opt->opt.ssrr.ip[i]));
    }
    break;
  default :
    return(NETWIB_ERR_LONOTIMPLEMENTED);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_ip4opt_end(netwib_buf *ppkt)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(ppkt, 1, &data));

  netwib__data_append_uint8(data, NETWIB_IP4OPTTYPE_END);

  ppkt->endoffset++;

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_ip4opt_noop(netwib_buf *ppkt)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(ppkt, 1, &data));

  netwib__data_append_uint8(data, NETWIB_IP4OPTTYPE_NOOP);

  ppkt->endoffset++;

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_ip4opt_rr(netwib_constip4opt_rr *pip4opt,
                                              netwib_buf *ppkt)
{
  netwib_data data;
  netwib_uint32 i, length;

  if (pip4opt->storagesize > 9) {
    return(NETWIB_ERR_PATOOBIGFORHDR);
  }
  if (pip4opt->storedvalues > pip4opt->storagesize) {
    return(NETWIB_ERR_PATOOBIGFORHDR);
  }

  length = 3 + pip4opt->storagesize * 4;
  netwib_er(netwib_buf_wantspace(ppkt, length, &data));

  netwib__data_append_uint8(data, NETWIB_IP4OPTTYPE_RR);
  netwib__data_append_uint8(data, length);
  netwib__data_append_uint8(data, 4 * (pip4opt->storedvalues + 1));

  for (i = 0; i < pip4opt->storedvalues; i++) {
    netwib__data_append_uint32(data, pip4opt->ip[i].ipvalue.ip4);
  }
  for (; i < pip4opt->storagesize; i++) {
    netwib__data_append_uint32(data, 0);
  }

  ppkt->endoffset += length;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_ip4opt_rr(netwib_data data,
                                              netwib_uint32 datasize,
                                              netwib_ip4opt_rr *pip4opt,
                                              netwib_uint32 *pskipsize)
{
  netwib_uint32 length, pointer, i;

  if (datasize < 1) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwib__data_decode_uint8(data, length);
  if (length > datasize) {
    return(NETWIB_ERR_DATAMISSING);
  }
  if (length > NETWIB_IP4OPT_MAXLEN) {
    if (pskipsize != NULL) *pskipsize = datasize;
    return(NETWIB_ERR_NOTCONVERTED);
  }
  if (pskipsize != NULL) *pskipsize = length;
  if (length < 3) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  netwib__data_decode_uint8(data, pointer);
  if (pointer < 4 || pointer > length+1) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  if (pip4opt != NULL) {
    pip4opt->storagesize = (length-3) / 4;
    pip4opt->storedvalues = pointer/4 - 1;
    for (i = 0; i < pip4opt->storedvalues; i++) {
      pip4opt->ip[i].iptype = NETWIB_IPTYPE_IP4;
      netwib__data_decode_uint32(data, pip4opt->ip[i].ipvalue.ip4);
    }
  }

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_ip4opt_srr(netwib_constip4opt_srr *pip4opt,
                                               netwib_ip4opttype ip4opttype,
                                               netwib_buf *ppkt)
{
  netwib_data data;
  netwib_uint32 i, length;

  if (pip4opt->storagesize > 9) {
    return(NETWIB_ERR_PATOOBIGFORHDR);
  }
  if (pip4opt->usedvalues > pip4opt->storagesize) {
    return(NETWIB_ERR_PATOOBIGFORHDR);
  }

  length = 3 + pip4opt->storagesize * 4;
  netwib_er(netwib_buf_wantspace(ppkt, length, &data));

  netwib__data_append_uint8(data, ip4opttype);
  netwib__data_append_uint8(data, length);
  netwib__data_append_uint8(data, 4 * (pip4opt->usedvalues + 1));

  for (i = 0; i < pip4opt->storagesize; i++) {
    netwib__data_append_uint32(data, pip4opt->ip[i].ipvalue.ip4);
  }

  ppkt->endoffset += length;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_ip4opt_srr(netwib_data data,
                                               netwib_uint32 datasize,
                                               netwib_ip4opt_srr *pip4opt,
                                               netwib_uint32 *pskipsize)
{
  netwib_uint32 length, pointer, i;

  if (datasize < 1) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwib__data_decode_uint8(data, length);
  if (length > datasize) {
    return(NETWIB_ERR_DATAMISSING);
  }
  if (length > NETWIB_IP4OPT_MAXLEN) {
    if (pskipsize != NULL) *pskipsize = datasize;
    return(NETWIB_ERR_NOTCONVERTED);
  }
  if (pskipsize != NULL) *pskipsize = length;
  if (length < 3) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  netwib__data_decode_uint8(data, pointer);
  if (pointer < 4 || pointer > length+1) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  if (pip4opt != NULL) {
    pip4opt->storagesize = (length-3) / 4;
    pip4opt->usedvalues = pointer/4 - 1;
    for (i = 0; i < pip4opt->storagesize; i++) {
      pip4opt->ip[i].iptype = NETWIB_IPTYPE_IP4;
      netwib__data_decode_uint32(data, pip4opt->ip[i].ipvalue.ip4);
    }
  }

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_ip4opt_time(netwib_constip4opt_time *pip4opt,
                                                netwib_buf *ppkt)
{
  netwib_data data;
  netwib_uint32 i, length, pointer, overflag;

  if (pip4opt->flag == NETWIB_IP4OPT_TIMEFLAG_TS) {
    if (pip4opt->storagesize > 9) {
      return(NETWIB_ERR_PATOOBIGFORHDR);
    }
  } else {
    if (pip4opt->storagesize > 4) {
      return(NETWIB_ERR_PATOOBIGFORHDR);
    }
  }
  if (pip4opt->storedvalues > pip4opt->storagesize) {
    return(NETWIB_ERR_PATOOBIGFORHDR);
  }
  if (pip4opt->overflow > 0xF) {
    return(NETWIB_ERR_PATOOBIGFORHDR);
  }
  if (pip4opt->flag > 0xF) {
    return(NETWIB_ERR_PATOOBIGFORHDR);
  }

  length = 4;
  pointer = 5;
  overflag = (pip4opt->overflow<<4) | pip4opt->flag;
  if (pip4opt->flag == NETWIB_IP4OPT_TIMEFLAG_TS) {
    length += 4 * pip4opt->storagesize;
    pointer += 4 * pip4opt->storedvalues;
  } else {
    length += 8 * pip4opt->storagesize;
    pointer += 8 * pip4opt->storedvalues;
  }

  netwib_er(netwib_buf_wantspace(ppkt, length, &data));

  netwib__data_append_uint8(data, NETWIB_IP4OPTTYPE_TIME);
  netwib__data_append_uint8(data, length);
  netwib__data_append_uint8(data, pointer);
  netwib__data_append_uint8(data, overflag);

  if (pip4opt->flag == NETWIB_IP4OPT_TIMEFLAG_TS) {
    for (i = 0; i < pip4opt->storedvalues; i++) {
      netwib__data_append_uint32(data, pip4opt->timestamp[i]);
    }
    for (; i < pip4opt->storagesize; i++) {
      netwib__data_append_uint32(data, 0);
    }
  } else if (pip4opt->flag == NETWIB_IP4OPT_TIMEFLAG_IPTS) {
    for (i = 0; i < pip4opt->storedvalues; i++) {
      netwib__data_append_uint32(data, pip4opt->ip[i].ipvalue.ip4);
      netwib__data_append_uint32(data, pip4opt->timestamp[i]);
    }
    for (; i < pip4opt->storagesize; i++) {
      netwib__data_append_uint32(data, 0);
      netwib__data_append_uint32(data, 0);
    }
  } else if (pip4opt->flag == NETWIB_IP4OPT_TIMEFLAG_IPPTS) {
    for (i = 0; i < pip4opt->storedvalues; i++) {
      netwib__data_append_uint32(data, pip4opt->ip[i].ipvalue.ip4);
      netwib__data_append_uint32(data, pip4opt->timestamp[i]);
    }
    for (; i < pip4opt->storagesize; i++) {
      netwib__data_append_uint32(data, pip4opt->ip[i].ipvalue.ip4);
      netwib__data_append_uint32(data, 0);
    }
  } else {
    for (i = 0; i < pip4opt->storagesize; i++) {
      netwib__data_append_uint32(data, pip4opt->ip[i].ipvalue.ip4);
      netwib__data_append_uint32(data, pip4opt->timestamp[i]);
    }
  }

  ppkt->endoffset += length;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_ip4opt_time(netwib_data data,
                                                netwib_uint32 datasize,
                                                netwib_ip4opt_time *pip4opt,
                                                netwib_uint32 *pskipsize)
{
  netwib_uint32 length, pointer, i, overflag;

  if (datasize < 1) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwib__data_decode_uint8(data, length);
  if (length > datasize) {
    return(NETWIB_ERR_DATAMISSING);
  }
  if (length > NETWIB_IP4OPT_MAXLEN) {
    if (pskipsize != NULL) *pskipsize = datasize;
    return(NETWIB_ERR_NOTCONVERTED);
  }
  if (pskipsize != NULL) *pskipsize = length;
  if (length < 4) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  netwib__data_decode_uint8(data, pointer);
  if (pointer < 4 || pointer > length+1) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  if (pip4opt != NULL) {
    netwib__data_decode_uint8(data, overflag);
    pip4opt->overflow = (netwib_uint8)(overflag >> 4);
    pip4opt->flag = (netwib_ip4opt_timeflag)(overflag & 0xF);
    if (pip4opt->flag == NETWIB_IP4OPT_TIMEFLAG_TS) {
      pip4opt->storagesize = (length - 4) / 4;
      pip4opt->storedvalues =  (pointer - 5) / 4;
    } else {
      pip4opt->storagesize = (length - 4) / 8;
      pip4opt->storedvalues =  (pointer - 5) / 8;
    }
    if (pip4opt->flag == NETWIB_IP4OPT_TIMEFLAG_TS) {
      for (i = 0; i < pip4opt->storedvalues; i++) {
        netwib__data_decode_uint32(data, pip4opt->timestamp[i]);
      }
    } else {
      for (i = 0; i < pip4opt->storagesize; i++) {
        pip4opt->ip[i].iptype = NETWIB_IPTYPE_IP4;
        netwib__data_decode_uint32(data, pip4opt->ip[i].ipvalue.ip4);
        netwib__data_decode_uint32(data, pip4opt->timestamp[i]);
      }
    }
  }

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_ip4opt(netwib_constip4opt *pip4opt,
                                    netwib_buf *ppkt)
{
  switch(pip4opt->type) {
    case NETWIB_IP4OPTTYPE_END :
      netwib_er(netwib_pkt_append_ip4opt_end(ppkt));
      break;
    case NETWIB_IP4OPTTYPE_NOOP :
      netwib_er(netwib_pkt_append_ip4opt_noop(ppkt));
      break;
    case NETWIB_IP4OPTTYPE_RR :
      netwib_er(netwib_pkt_append_ip4opt_rr(&pip4opt->opt.rr, ppkt));
      break;
    case NETWIB_IP4OPTTYPE_TIME :
      netwib_er(netwib_pkt_append_ip4opt_time(&pip4opt->opt.time, ppkt));
      break;
    case NETWIB_IP4OPTTYPE_LSRR :
      netwib_er(netwib_pkt_append_ip4opt_srr(&pip4opt->opt.lsrr, pip4opt->type,
                                             ppkt));
      break;
    case NETWIB_IP4OPTTYPE_SSRR :
      netwib_er(netwib_pkt_append_ip4opt_srr(&pip4opt->opt.ssrr, pip4opt->type,
                                             ppkt));
      break;
    default :
      return(NETWIB_ERR_LONOTIMPLEMENTED);
      break;
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_prepend_ip4opt(netwib_constip4opt *pip4opt,
                                     netwib_buf *ppkt)
{
  netwib_byte array[NETWIB_IP4OPT_MAXLEN];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));
  ret = netwib_pkt_append_ip4opt(pip4opt, &buf);
  if (ret == NETWIB_ERR_OK) {
    ret = netwib_buf_prepend_buf(&buf, ppkt);
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_ip4opt(netwib_constbuf *ppkt,
                                    netwib_ip4opt *pip4opt,
                                    netwib_uint32 *pskipsize)
{
  netwib_data data;
  netwib_uint32 datasize, skipsize;
  netwib_ip4opttype ip4opttype;

  datasize = netwib__buf_ref_data_size(ppkt);
  if (pskipsize != NULL) *pskipsize = datasize;
  if (datasize == 0) {
    return(NETWIB_ERR_DATAMISSING);
  }

  data = netwib__buf_ref_data_ptr(ppkt);
  netwib__data_decode_uint8t(data, ip4opttype, netwib_ip4opttype);
  if (pip4opt != NULL) pip4opt->type = ip4opttype;

  switch(ip4opttype) {
    case NETWIB_IP4OPTTYPE_END :
      if (pskipsize != NULL) *pskipsize = 1;
      break;
    case NETWIB_IP4OPTTYPE_NOOP :
      if (pskipsize != NULL) *pskipsize = 1;
      break;
    case NETWIB_IP4OPTTYPE_RR :
      netwib_er(netwib_pkt_decode_ip4opt_rr(data, datasize,
                                         (pip4opt==NULL)?NULL:&pip4opt->opt.rr,
                                          pskipsize));
      break;
    case NETWIB_IP4OPTTYPE_TIME :
      netwib_er(netwib_pkt_decode_ip4opt_time(data, datasize,
                                      (pip4opt==NULL)?NULL:&pip4opt->opt.time,
                                            pskipsize));
      break;
    case NETWIB_IP4OPTTYPE_LSRR :
      netwib_er(netwib_pkt_decode_ip4opt_srr(data, datasize,
                                      (pip4opt==NULL)?NULL:&pip4opt->opt.lsrr,
                                           pskipsize));
      break;
    case NETWIB_IP4OPTTYPE_SSRR :
      netwib_er(netwib_pkt_decode_ip4opt_srr(data, datasize,
                                      (pip4opt==NULL)?NULL:&pip4opt->opt.ssrr,
                                           pskipsize));
      break;
    default :
      if (datasize > 1) {
        netwib__data_decode_uint8(data, skipsize);
        if (skipsize > datasize) {
          skipsize = datasize;
        }
        if (pskipsize != NULL) *pskipsize = datasize;
      }
      return(NETWIB_ERR_LONOTIMPLEMENTED);
      break;
  }

  return(NETWIB_ERR_OK);
}
