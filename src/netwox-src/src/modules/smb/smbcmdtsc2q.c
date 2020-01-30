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
netwib_err netwox_smbcmdtsc2q_init(netwox_smbcmdtsc2q *psmbcmdtsc2q)
{
  psmbcmdtsc2q->type = NETWOX_SMBCMDTSC2Q_TYPE_UNKNOWN;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2q_selecttype(netwox_smbcmdtsc2q *psmbcmdtsc2q,
                                         netwox_smbcmdtsc2q_type type)
{

  if (psmbcmdtsc2q->type != NETWOX_SMBCMDTSC2Q_TYPE_UNKNOWN) {
    return(NETWOX_ERR_INTERNALERROR);
  }

  psmbcmdtsc2q->type = type;
  switch(type) {
  case NETWOX_SMBCMDTSC2Q_TYPE_FINDFIRST2 :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmdtsc2q->value.findfirst2.searchpattern));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_FINDNEXT2 :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmdtsc2q->value.findnext2.resumefilename));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_QUERYFSINFO :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmdtsc2q->value.queryfsinfo.sharename));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_QUERYPATHINFO :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmdtsc2q->value.querypathinfo.filename));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_QUERYFILEINFO :
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_BASIC :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EBASIC :
    netwib_er(netwox_smbcmdtsc2_fmt_file_basic_init(&psmbcmdtsc2q->value.setfileinfo_basic.basic));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_DISPOSITION :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EDISPOSITION :
    netwib_er(netwox_smbcmdtsc2_fmt_file_disposition_init(&psmbcmdtsc2q->value.setfileinfo_disposition.disposition));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_ALLOCATION :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EALLOCATION :
    netwib_er(netwox_smbcmdtsc2_fmt_file_allocation_init(&psmbcmdtsc2q->value.setfileinfo_allocation.allocation));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EOF :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EEOF :
    netwib_er(netwox_smbcmdtsc2_fmt_file_eof_init(&psmbcmdtsc2q->value.setfileinfo_eof.eof));
    break;
  default :
    psmbcmdtsc2q->type = NETWOX_SMBCMDTSC2Q_TYPE_UNKNOWN;
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2q_setdefault(netwox_smbcmdtsc2q *psmbcmdtsc2q)
{

  switch(psmbcmdtsc2q->type) {
  case NETWOX_SMBCMDTSC2Q_TYPE_FINDFIRST2 :
    netwox__smbcmdcmn_fileattr16_initdefault(psmbcmdtsc2q->value.findfirst2.searchattributes);
    psmbcmdtsc2q->value.findfirst2.maxentries = 0;
    psmbcmdtsc2q->value.findfirst2.flags = 0;
    psmbcmdtsc2q->value.findfirst2.fmt = 0;
    psmbcmdtsc2q->value.findfirst2.storagetype = 0;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmdtsc2q->value.findfirst2.searchpattern));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_FINDNEXT2 :
    psmbcmdtsc2q->value.findnext2.sid = 0;
    psmbcmdtsc2q->value.findnext2.maxentries = 0;
    psmbcmdtsc2q->value.findnext2.fmt = 0;
    psmbcmdtsc2q->value.findnext2.resumekey = 0;
    psmbcmdtsc2q->value.findnext2.flags = 0;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmdtsc2q->value.findnext2.resumefilename));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_QUERYFSINFO :
    psmbcmdtsc2q->value.queryfsinfo.fmt = 0;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmdtsc2q->value.queryfsinfo.sharename));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_QUERYPATHINFO :
    psmbcmdtsc2q->value.querypathinfo.fmt = 0;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmdtsc2q->value.querypathinfo.filename));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_QUERYFILEINFO :
    psmbcmdtsc2q->value.queryfileinfo.fileid = 0;
    psmbcmdtsc2q->value.queryfileinfo.fmt = 0;
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_BASIC :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EBASIC :
    psmbcmdtsc2q->value.setfileinfo_basic.fileid = 0;
    netwib_er(netwox_smbcmdtsc2_fmt_file_basic_setdefault(&psmbcmdtsc2q->value.setfileinfo_basic.basic));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_DISPOSITION :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EDISPOSITION :
    psmbcmdtsc2q->value.setfileinfo_disposition.fileid = 0;
    netwib_er(netwox_smbcmdtsc2_fmt_file_disposition_setdefault(&psmbcmdtsc2q->value.setfileinfo_disposition.disposition));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_ALLOCATION :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EALLOCATION :
    psmbcmdtsc2q->value.setfileinfo_allocation.fileid = 0;
    netwib_er(netwox_smbcmdtsc2_fmt_file_allocation_setdefault(&psmbcmdtsc2q->value.setfileinfo_allocation.allocation));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EOF :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EEOF :
    psmbcmdtsc2q->value.setfileinfo_eof.fileid = 0;
    netwib_er(netwox_smbcmdtsc2_fmt_file_eof_setdefault(&psmbcmdtsc2q->value.setfileinfo_eof.eof));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2q_close(netwox_smbcmdtsc2q *psmbcmdtsc2q)
{

  switch(psmbcmdtsc2q->type) {
  case NETWOX_SMBCMDTSC2Q_TYPE_UNKNOWN :
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_FINDFIRST2 :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmdtsc2q->value.findfirst2.searchpattern));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_FINDNEXT2 :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmdtsc2q->value.findnext2.resumefilename));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_QUERYFSINFO :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmdtsc2q->value.queryfsinfo.sharename));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_QUERYPATHINFO :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmdtsc2q->value.querypathinfo.filename));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_QUERYFILEINFO :
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_BASIC :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EBASIC :
    netwib_er(netwox_smbcmdtsc2_fmt_file_basic_close(&psmbcmdtsc2q->value.setfileinfo_basic.basic));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_DISPOSITION :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EDISPOSITION :
    netwib_er(netwox_smbcmdtsc2_fmt_file_disposition_close(&psmbcmdtsc2q->value.setfileinfo_disposition.disposition));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_ALLOCATION :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EALLOCATION :
    netwib_er(netwox_smbcmdtsc2_fmt_file_allocation_close(&psmbcmdtsc2q->value.setfileinfo_allocation.allocation));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EOF :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EEOF :
    netwib_er(netwox_smbcmdtsc2_fmt_file_eof_close(&psmbcmdtsc2q->value.setfileinfo_eof.eof));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SMBCMDTSC2Q_SUBCMD_FINDFIRST2 = 1,
  NETWOX_SMBCMDTSC2Q_SUBCMD_FINDNEXT2 = 2,
  NETWOX_SMBCMDTSC2Q_SUBCMD_QUERYFSINFO = 3,
  NETWOX_SMBCMDTSC2Q_SUBCMD_QUERYPATHINFO = 5,
  NETWOX_SMBCMDTSC2Q_SUBCMD_QUERYFILEINFO = 7,
  NETWOX_SMBCMDTSC2Q_SUBCMD_SETFILEINFO = 8,
  NETWOX_SMBCMDTSC2Q_SUBCMD_GETDFSREFERRAL = 16 /* not implemented */
} netwox_smbcmdtsc2q_subcmd;

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2q_encode_smbcmd(netwox_constsmbcmd *psmbcmd,
                                            netwox_smbcmdtsc2q *psmbcmdtsc2q)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize;
  netwox_smbcmdtsc2q_subcmd subcmd;
  netwox_smbcmdtsc2q_type type;

  if (psmbcmd->type != NETWOX_SMBCMD_TYPE_TRANSACTION2_Q) {
    return(NETWIB_ERR_PAINVALIDTYPE);
  }
  if (netwib__buf_ref_data_size(&psmbcmd->value.transaction2_q.para) != psmbcmd->value.transaction2_q.totalparasize) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  if (netwib__buf_ref_data_size(&psmbcmd->value.transaction2_q.data) != psmbcmd->value.transaction2_q.totaldatasize) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  if (psmbcmd->value.transaction2_q.setupcount > NETWOX_SMBCMD_TRANSACTION2_SETUP_MAX) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  para = netwib__buf_ref_data_ptr(&psmbcmd->value.transaction2_q.para);
  parasize = netwib__buf_ref_data_size(&psmbcmd->value.transaction2_q.para);
  data = netwib__buf_ref_data_ptr(&psmbcmd->value.transaction2_q.data);
  datasize = netwib__buf_ref_data_size(&psmbcmd->value.transaction2_q.data);

  if (psmbcmd->value.transaction2_q.setupcount == 0) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  subcmd = psmbcmd->value.transaction2_q.setup[0];

  switch(subcmd) {
  case NETWOX_SMBCMDTSC2Q_SUBCMD_FINDFIRST2 :
    netwib_er(netwox_smbcmdtsc2q_selecttype(psmbcmdtsc2q,
                                         NETWOX_SMBCMDTSC2Q_TYPE_FINDFIRST2));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (psmbcmd->value.transaction2_q.setupcount != 1) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize < 12) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwox__smbcmdcmn_fileattr16_decode(para, psmbcmdtsc2q->value.findfirst2.searchattributes);
    netwib__data_decode_uint16_le(para, psmbcmdtsc2q->value.findfirst2.maxentries);
    netwib__data_decode_uint16_le(para, psmbcmdtsc2q->value.findfirst2.flags);
    netwib__data_decode_uint16_le(para, psmbcmdtsc2q->value.findfirst2.fmt);
    netwib__data_decode_uint32_le(para, psmbcmdtsc2q->value.findfirst2.storagetype);
    parasize -= 12;
    netwib_er(netwox_smbcmdcmn_data_decodestr(&para, &parasize, psmbcmd->value.transaction2_q.isunicode, &psmbcmdtsc2q->value.findfirst2.searchpattern));
    if (parasize != 0) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_SMBCMDTSC2Q_SUBCMD_FINDNEXT2 :
    netwib_er(netwox_smbcmdtsc2q_selecttype(psmbcmdtsc2q,
                                         NETWOX_SMBCMDTSC2Q_TYPE_FINDNEXT2));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (psmbcmd->value.transaction2_q.setupcount != 1) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize < 12) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmdtsc2q->value.findnext2.sid);
    netwib__data_decode_uint16_le(para, psmbcmdtsc2q->value.findnext2.maxentries);
    netwib__data_decode_uint16_le(para, psmbcmdtsc2q->value.findnext2.fmt);
    netwib__data_decode_uint32_le(para, psmbcmdtsc2q->value.findnext2.resumekey);
    netwib__data_decode_uint16_le(para, psmbcmdtsc2q->value.findnext2.flags);
    parasize -= 12;
    netwib_er(netwox_smbcmdcmn_data_decodestr(&para, &parasize, psmbcmd->value.transaction2_q.isunicode, &psmbcmdtsc2q->value.findnext2.resumefilename));
    if (parasize != 0) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_SMBCMDTSC2Q_SUBCMD_QUERYFSINFO :
    netwib_er(netwox_smbcmdtsc2q_selecttype(psmbcmdtsc2q,
                                         NETWOX_SMBCMDTSC2Q_TYPE_QUERYFSINFO));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (psmbcmd->value.transaction2_q.setupcount != 1) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize < 2) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmdtsc2q->value.queryfsinfo.fmt);
    parasize -= 2;
    if (parasize) {
      /* because sometimes it is not present */
      para += 4; /* reserved */
      parasize -= 4;
      if (parasize) {
        /* because sometimes it is not present */
        netwib_er(netwox_smbcmdcmn_data_decodestr(&para, &parasize, psmbcmd->value.transaction2_q.isunicode, &psmbcmdtsc2q->value.queryfsinfo.sharename));
        if (parasize != 0) {
          return(NETWIB_ERR_NOTCONVERTED);
        }
      }
    }
    break;
  case NETWOX_SMBCMDTSC2Q_SUBCMD_QUERYPATHINFO :
    netwib_er(netwox_smbcmdtsc2q_selecttype(psmbcmdtsc2q, NETWOX_SMBCMDTSC2Q_TYPE_QUERYPATHINFO));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (psmbcmd->value.transaction2_q.setupcount != 1) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize < 6) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmdtsc2q->value.querypathinfo.fmt);
    parasize -= 2;
    para += 4; /* reserved */
    parasize -= 4;
    netwib_er(netwox_smbcmdcmn_data_decodestr(&para, &parasize, psmbcmd->value.transaction2_q.isunicode, &psmbcmdtsc2q->value.querypathinfo.filename));
    if (parasize != 0) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_SMBCMDTSC2Q_SUBCMD_QUERYFILEINFO :
    netwib_er(netwox_smbcmdtsc2q_selecttype(psmbcmdtsc2q, NETWOX_SMBCMDTSC2Q_TYPE_QUERYFILEINFO));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (psmbcmd->value.transaction2_q.setupcount != 1) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize < 4) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmdtsc2q->value.queryfileinfo.fileid);
    netwib__data_decode_uint16_le(para, psmbcmdtsc2q->value.queryfileinfo.fmt);
    parasize -= 4;
    if (parasize != 0) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_SMBCMDTSC2Q_SUBCMD_SETFILEINFO :
    {
      netwib_uint16 fileid;
      netwox_smbcmdtsc2_fmt_files fmt;
      if (psmbcmd->value.transaction2_q.setupcount != 1) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      if (parasize < 4) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      netwib__data_decode_uint16_le(para, fileid);
      netwib__data_decode_uint16_le(para, fmt);
      parasize -= 4;
      switch(fmt) {
      case NETWOX_SMBCMDTSC2_FMT_FILES_BASIC :
      case NETWOX_SMBCMDTSC2_FMT_FILES_EBASIC :
        type = NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_BASIC;
        if (fmt == NETWOX_SMBCMDTSC2_FMT_FILES_EBASIC) {
          type = NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EBASIC;
        }
        netwib_er(netwox_smbcmdtsc2q_selecttype(psmbcmdtsc2q, type));
        psmbcmdtsc2q->value.setfileinfo_basic.fileid = fileid;
        para += 2; parasize -= 2; /* reserved */
        if (parasize != 0 && parasize != 2/*Win98*/) {
          return(NETWIB_ERR_NOTCONVERTED);
        }
        netwib_er(netwox_smbcmdtsc2_fmt_file_basic_decode(&data, &datasize, &psmbcmdtsc2q->value.setfileinfo_basic.basic));
        if (datasize) {
          return(NETWIB_ERR_NOTCONVERTED);
        }
        break;
      case NETWOX_SMBCMDTSC2_FMT_FILES_DISPOSITION :
      case NETWOX_SMBCMDTSC2_FMT_FILES_EDISPOSITION :
        type = NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_DISPOSITION;
        if (fmt == NETWOX_SMBCMDTSC2_FMT_FILES_EDISPOSITION) {
          type = NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EDISPOSITION;
        }
        netwib_er(netwox_smbcmdtsc2q_selecttype(psmbcmdtsc2q, type));
        psmbcmdtsc2q->value.setfileinfo_disposition.fileid = fileid;
        para += 2; parasize -= 2; /* reserved */
        if (parasize != 0) {
          return(NETWIB_ERR_NOTCONVERTED);
        }
        netwib_er(netwox_smbcmdtsc2_fmt_file_disposition_decode(&data, &datasize, &psmbcmdtsc2q->value.setfileinfo_disposition.disposition));
        if (datasize != 0) {
          return(NETWIB_ERR_NOTCONVERTED);
        }
        break;
      case NETWOX_SMBCMDTSC2_FMT_FILES_ALLOCATION :
      case NETWOX_SMBCMDTSC2_FMT_FILES_EALLOCATION :
        type = NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_ALLOCATION;
        if (fmt == NETWOX_SMBCMDTSC2_FMT_FILES_EALLOCATION) {
          type = NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EALLOCATION;
        }
        netwib_er(netwox_smbcmdtsc2q_selecttype(psmbcmdtsc2q, type));
        psmbcmdtsc2q->value.setfileinfo_allocation.fileid = fileid;
        para += 2; parasize -= 2; /* reserved */
        if (parasize != 0) {
          return(NETWIB_ERR_NOTCONVERTED);
        }
        netwib_er(netwox_smbcmdtsc2_fmt_file_allocation_decode(&data, &datasize, &psmbcmdtsc2q->value.setfileinfo_allocation.allocation));
        if (datasize != 0) {
          return(NETWIB_ERR_NOTCONVERTED);
        }
        break;
      case NETWOX_SMBCMDTSC2_FMT_FILES_EOF :
      case NETWOX_SMBCMDTSC2_FMT_FILES_EEOF :
        type = NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EOF;
        if (fmt == NETWOX_SMBCMDTSC2_FMT_FILES_EEOF) {
          type = NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EEOF;
        }
        netwib_er(netwox_smbcmdtsc2q_selecttype(psmbcmdtsc2q, type));
        psmbcmdtsc2q->value.setfileinfo_eof.fileid = fileid;
        para += 2; parasize -= 2; /* reserved */
        if (parasize != 0) {
          return(NETWIB_ERR_NOTCONVERTED);
        }
        netwib_er(netwox_smbcmdtsc2_fmt_file_eof_decode(&data, &datasize, &psmbcmdtsc2q->value.setfileinfo_eof.eof));
        if (datasize != 0) {
          return(NETWIB_ERR_NOTCONVERTED);
        }
        break;
      default :
        return(NETWIB_ERR_NOTCONVERTED);
      }
    }
    break;
  case NETWOX_SMBCMDTSC2Q_SUBCMD_GETDFSREFERRAL :
    return(NETWOX_ERR_NOTIMPLEMENTED);
  default :
    return(NETWIB_ERR_NOTCONVERTED);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2q_decode_smbcmd(netwox_constsmbcmdtsc2q *psmbcmdtsc2q,
                                            netwox_smbcmd *psmbcmd)
{
  netwib_data para;

  switch(psmbcmdtsc2q->type) {
  case NETWOX_SMBCMDTSC2Q_TYPE_FINDFIRST2 :
    psmbcmd->value.transaction2_q.setupcount = 1;
    psmbcmd->value.transaction2_q.setup[0] = NETWOX_SMBCMDTSC2Q_SUBCMD_FINDFIRST2;
    netwib_er(netwib_buf_wantspace(&psmbcmd->value.transaction2_q.para, 12, &para));
    netwox__smbcmdcmn_fileattr16_append(para, psmbcmdtsc2q->value.findfirst2.searchattributes);
    netwib__data_append_uint16_le(para, psmbcmdtsc2q->value.findfirst2.maxentries);
    netwib__data_append_uint16_le(para, psmbcmdtsc2q->value.findfirst2.flags);
    netwib__data_append_uint16_le(para, psmbcmdtsc2q->value.findfirst2.fmt);
    netwib__data_append_uint32_le(para, psmbcmdtsc2q->value.findfirst2.storagetype);
    psmbcmd->value.transaction2_q.para.endoffset += 12;
    netwib_er(netwox_smbcmdcmn_data_appendstr(&psmbcmdtsc2q->value.findfirst2.searchpattern, psmbcmd->value.transaction2_q.isunicode, &psmbcmd->value.transaction2_q.para));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_FINDNEXT2 :
    psmbcmd->value.transaction2_q.setupcount = 1;
    psmbcmd->value.transaction2_q.setup[0] = NETWOX_SMBCMDTSC2Q_SUBCMD_FINDNEXT2;
    netwib_er(netwib_buf_wantspace(&psmbcmd->value.transaction2_q.para, 12, &para));
    netwib__data_append_uint16_le(para, psmbcmdtsc2q->value.findnext2.sid);
    netwib__data_append_uint16_le(para, psmbcmdtsc2q->value.findnext2.maxentries);
    netwib__data_append_uint16_le(para, psmbcmdtsc2q->value.findnext2.fmt);
    netwib__data_append_uint32_le(para, psmbcmdtsc2q->value.findnext2.resumekey);
    netwib__data_append_uint16_le(para, psmbcmdtsc2q->value.findnext2.flags);
    psmbcmd->value.transaction2_q.para.endoffset += 12;
    netwib_er(netwox_smbcmdcmn_data_appendstr(&psmbcmdtsc2q->value.findnext2.resumefilename, psmbcmd->value.transaction2_q.isunicode, &psmbcmd->value.transaction2_q.para));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_QUERYFSINFO :
    psmbcmd->value.transaction2_q.setupcount = 1;
    psmbcmd->value.transaction2_q.setup[0] = NETWOX_SMBCMDTSC2Q_SUBCMD_QUERYFSINFO;
    netwib_er(netwib_buf_wantspace(&psmbcmd->value.transaction2_q.para, 6, &para));
    netwib__data_append_uint16_le(para, psmbcmdtsc2q->value.queryfsinfo.fmt);
    netwib_c_memset(para, 0, 4); /* reserved */
    psmbcmd->value.transaction2_q.para.endoffset += 6;
    if (netwib__buf_ref_data_size(&psmbcmdtsc2q->value.queryfsinfo.sharename)){
      /* only put it if user set it (has a size) */
      netwib_er(netwox_smbcmdcmn_data_appendstr(&psmbcmdtsc2q->value.queryfsinfo.sharename, psmbcmd->value.transaction2_q.isunicode, &psmbcmd->value.transaction2_q.para));
    }
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_QUERYPATHINFO :
    psmbcmd->value.transaction2_q.setupcount = 1;
    psmbcmd->value.transaction2_q.setup[0] = NETWOX_SMBCMDTSC2Q_SUBCMD_QUERYPATHINFO;
    netwib_er(netwib_buf_wantspace(&psmbcmd->value.transaction2_q.para, 6, &para));
    netwib__data_append_uint16_le(para, psmbcmdtsc2q->value.querypathinfo.fmt);
    netwib_c_memset(para, 0, 4); /* reserved */
    psmbcmd->value.transaction2_q.para.endoffset += 6;
    netwib_er(netwox_smbcmdcmn_data_appendstr(&psmbcmdtsc2q->value.querypathinfo.filename, psmbcmd->value.transaction2_q.isunicode, &psmbcmd->value.transaction2_q.para));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_QUERYFILEINFO :
    psmbcmd->value.transaction2_q.setupcount = 1;
    psmbcmd->value.transaction2_q.setup[0] = NETWOX_SMBCMDTSC2Q_SUBCMD_QUERYFILEINFO;
    netwib_er(netwib_buf_wantspace(&psmbcmd->value.transaction2_q.para, 4, &para));
    netwib__data_append_uint16_le(para, psmbcmdtsc2q->value.queryfileinfo.fileid);
    netwib__data_append_uint16_le(para, psmbcmdtsc2q->value.queryfileinfo.fmt);
    psmbcmd->value.transaction2_q.para.endoffset += 4;
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_BASIC :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EBASIC :
    psmbcmd->value.transaction2_q.setupcount = 1;
    psmbcmd->value.transaction2_q.setup[0] = NETWOX_SMBCMDTSC2Q_SUBCMD_SETFILEINFO;
    netwib_er(netwib_buf_wantspace(&psmbcmd->value.transaction2_q.para, 6, &para));
    netwib__data_append_uint16_le(para, psmbcmdtsc2q->value.setfileinfo_basic.fileid);
    if (psmbcmdtsc2q->type == NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_BASIC) {
      netwib__data_append_uint16_le(para, NETWOX_SMBCMDTSC2_FMT_FILES_BASIC);
    } else {
      netwib__data_append_uint16_le(para, NETWOX_SMBCMDTSC2_FMT_FILES_EBASIC);
    }
    netwib_c_memset(para, 0, 2); /* reserved */
    psmbcmd->value.transaction2_q.para.endoffset += 6;
    netwib_er(netwox_smbcmdtsc2_fmt_file_basic_append(&psmbcmdtsc2q->value.setfileinfo_basic.basic, &psmbcmd->value.transaction2_q.data));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_DISPOSITION :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EDISPOSITION :
    psmbcmd->value.transaction2_q.setupcount = 1;
    psmbcmd->value.transaction2_q.setup[0] = NETWOX_SMBCMDTSC2Q_SUBCMD_SETFILEINFO;
    netwib_er(netwib_buf_wantspace(&psmbcmd->value.transaction2_q.para, 6, &para));
    netwib__data_append_uint16_le(para, psmbcmdtsc2q->value.setfileinfo_disposition.fileid);
    if (psmbcmdtsc2q->type == NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_DISPOSITION) {
      netwib__data_append_uint16_le(para, NETWOX_SMBCMDTSC2_FMT_FILES_DISPOSITION);
    } else {
      netwib__data_append_uint16_le(para, NETWOX_SMBCMDTSC2_FMT_FILES_EDISPOSITION);
    }
    netwib_c_memset(para, 0, 2); /* reserved */
    psmbcmd->value.transaction2_q.para.endoffset += 6;
    netwib_er(netwox_smbcmdtsc2_fmt_file_disposition_append(&psmbcmdtsc2q->value.setfileinfo_disposition.disposition, &psmbcmd->value.transaction2_q.data));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_ALLOCATION :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EALLOCATION :
    psmbcmd->value.transaction2_q.setupcount = 1;
    psmbcmd->value.transaction2_q.setup[0] = NETWOX_SMBCMDTSC2Q_SUBCMD_SETFILEINFO;
    netwib_er(netwib_buf_wantspace(&psmbcmd->value.transaction2_q.para, 6, &para));
    netwib__data_append_uint16_le(para, psmbcmdtsc2q->value.setfileinfo_allocation.fileid);
    if (psmbcmdtsc2q->type == NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_ALLOCATION) {
      netwib__data_append_uint16_le(para, NETWOX_SMBCMDTSC2_FMT_FILES_ALLOCATION);
    } else {
      netwib__data_append_uint16_le(para, NETWOX_SMBCMDTSC2_FMT_FILES_EALLOCATION);
    }
    netwib_c_memset(para, 0, 2); /* reserved */
    psmbcmd->value.transaction2_q.para.endoffset += 6;
    netwib_er(netwox_smbcmdtsc2_fmt_file_allocation_append(&psmbcmdtsc2q->value.setfileinfo_allocation.allocation, &psmbcmd->value.transaction2_q.data));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EOF :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EEOF :
    psmbcmd->value.transaction2_q.setupcount = 1;
    psmbcmd->value.transaction2_q.setup[0] = NETWOX_SMBCMDTSC2Q_SUBCMD_SETFILEINFO;
    netwib_er(netwib_buf_wantspace(&psmbcmd->value.transaction2_q.para, 6, &para));
    netwib__data_append_uint16_le(para, psmbcmdtsc2q->value.setfileinfo_eof.fileid);
    if (psmbcmdtsc2q->type == NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EOF) {
      netwib__data_append_uint16_le(para, NETWOX_SMBCMDTSC2_FMT_FILES_EOF);
    } else {
      netwib__data_append_uint16_le(para, NETWOX_SMBCMDTSC2_FMT_FILES_EEOF);
    }
    netwib_c_memset(para, 0, 2); /* reserved */
    psmbcmd->value.transaction2_q.para.endoffset += 6;
    netwib_er(netwox_smbcmdtsc2_fmt_file_eof_append(&psmbcmdtsc2q->value.setfileinfo_eof.eof, &psmbcmd->value.transaction2_q.data));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  psmbcmd->value.transaction2_q.totalparasize = (netwib_uint16)netwib__buf_ref_data_size(&psmbcmd->value.transaction2_q.para);
  psmbcmd->value.transaction2_q.totaldatasize = (netwib_uint16)netwib__buf_ref_data_size(&psmbcmd->value.transaction2_q.data);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2q_show(netwox_constsmbcmdtsc2q *psmbcmdtsc2q,
                                   netwib_buf *pbuf)
{

  switch(psmbcmdtsc2q->type) {
  case NETWOX_SMBCMDTSC2Q_TYPE_FINDFIRST2 :
    netwib_er(netwib_show_array_fmt32(pbuf, " FINDFIRST2 Query"));
    netwib_er(netwox_smbcmdcmn_fileattr32_show(" searchattributes", psmbcmdtsc2q->value.findfirst2.searchattributes, pbuf));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxentries=%{uint16}", psmbcmdtsc2q->value.findfirst2.maxentries));
    netwib_er(netwib_show_array_fmt32(pbuf, " flags=%{uint16}", psmbcmdtsc2q->value.findfirst2.flags));
    netwib_er(netwib_show_array_fmt32(pbuf, " fmt=%{uint16}", psmbcmdtsc2q->value.findfirst2.fmt));
    netwib_er(netwib_show_array_fmt32(pbuf, " storagetype=%{uint32}", psmbcmdtsc2q->value.findfirst2.storagetype));
    netwib_er(netwox_smbcmdcmn_data_show(" searchpattern", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmdtsc2q->value.findfirst2.searchpattern, pbuf));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_FINDNEXT2 :
    netwib_er(netwib_show_array_fmt32(pbuf, " FINDNEXT2 Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " sid=%{uint16}", psmbcmdtsc2q->value.findnext2.sid));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxentries=%{uint16}", psmbcmdtsc2q->value.findnext2.maxentries));
    netwib_er(netwib_show_array_fmt32(pbuf, " fmt=%{uint16}", psmbcmdtsc2q->value.findnext2.fmt));
    netwib_er(netwib_show_array_fmt32(pbuf, " resumekey=%{uint32}", psmbcmdtsc2q->value.findnext2.resumekey));
    netwib_er(netwib_show_array_fmt32(pbuf, " flags=%{uint16}", psmbcmdtsc2q->value.findnext2.flags));
    netwib_er(netwox_smbcmdcmn_data_show(" resumefilename", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmdtsc2q->value.findnext2.resumefilename, pbuf));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_QUERYFSINFO :
    netwib_er(netwib_show_array_fmt32(pbuf, " QUERYFSINFO Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fmt=%{uint16}", psmbcmdtsc2q->value.queryfsinfo.fmt));
    netwib_er(netwox_smbcmdcmn_data_show(" sharename", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmdtsc2q->value.queryfsinfo.sharename, pbuf));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_QUERYPATHINFO :
    netwib_er(netwib_show_array_fmt32(pbuf, " QUERYPATHINFO Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fmt=%{uint16}", psmbcmdtsc2q->value.querypathinfo.fmt));
    netwib_er(netwox_smbcmdcmn_data_show(" filename", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmdtsc2q->value.querypathinfo.filename, pbuf));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_QUERYFILEINFO :
    netwib_er(netwib_show_array_fmt32(pbuf, " QUERYFILEINFO Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmdtsc2q->value.queryfileinfo.fileid));
    netwib_er(netwib_show_array_fmt32(pbuf, " fmt=%{uint16}", psmbcmdtsc2q->value.queryfileinfo.fmt));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_BASIC :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EBASIC :
    netwib_er(netwib_show_array_fmt32(pbuf, " SETFILEINFO_BASIC Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmdtsc2q->value.setfileinfo_basic.fileid));
    netwib_er(netwox_smbcmdtsc2_fmt_file_basic_show(&psmbcmdtsc2q->value.setfileinfo_basic.basic, pbuf));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_DISPOSITION :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EDISPOSITION :
    netwib_er(netwib_show_array_fmt32(pbuf, " SETFILEINFO_DISPOSITION Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmdtsc2q->value.setfileinfo_disposition.fileid));
    netwib_er(netwox_smbcmdtsc2_fmt_file_disposition_show(&psmbcmdtsc2q->value.setfileinfo_disposition.disposition, pbuf));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_ALLOCATION :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EALLOCATION :
    netwib_er(netwib_show_array_fmt32(pbuf, " SETFILEINFO_ALLOCATION Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmdtsc2q->value.setfileinfo_allocation.fileid));
    netwib_er(netwox_smbcmdtsc2_fmt_file_allocation_show(&psmbcmdtsc2q->value.setfileinfo_allocation.allocation, pbuf));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EOF :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EEOF :
    netwib_er(netwib_show_array_fmt32(pbuf, " SETFILEINFO_EOF Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmdtsc2q->value.setfileinfo_eof.fileid));
    netwib_er(netwox_smbcmdtsc2_fmt_file_eof_show(&psmbcmdtsc2q->value.setfileinfo_eof.eof, pbuf));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2q_display(netwib_bufpool *pbufpool,
                                      netwox_constsmbcmdtsc2q *psmbcmdtsc2q)
{
  netwib_buf *pbuf;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));

  ret = netwox_smbcmdtsc2q_show(psmbcmdtsc2q, pbuf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_display(pbuf, NETWIB_ENCODETYPE_DATA));
  } else {
    netwib_er(netwib_fmt_display("This Transaction2 Query could not be decoded\n"));
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(ret);
}
