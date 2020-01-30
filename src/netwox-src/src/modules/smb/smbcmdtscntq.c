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
netwib_err netwox_smbcmdtscntq_init(netwox_smbcmdtscntq *psmbcmdtscntq)
{
  psmbcmdtscntq->type = NETWOX_SMBCMDTSCNTQ_TYPE_UNKNOWN;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntq_selecttype(netwox_smbcmdtscntq *psmbcmdtscntq,
                                         netwox_smbcmdtscntq_type type)
{

  if (psmbcmdtscntq->type != NETWOX_SMBCMDTSCNTQ_TYPE_UNKNOWN) {
    return(NETWOX_ERR_INTERNALERROR);
  }

  psmbcmdtscntq->type = type;
  switch(type) {
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTIOCTL :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmdtscntq->value.ntioctl.data));
    break;
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTNOTIFY :
    break;
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTQUERYSECUDESC :
    break;
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTSETSECUDESC :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmdtscntq->value.ntsetsecudesc.secudesc));
    break;
  default :
    psmbcmdtscntq->type = NETWOX_SMBCMDTSCNTQ_TYPE_UNKNOWN;
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntq_setdefault(netwox_smbcmdtscntq *psmbcmdtscntq)
{

  switch(psmbcmdtscntq->type) {
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTIOCTL :
    psmbcmdtscntq->value.ntioctl.function = 0;
    psmbcmdtscntq->value.ntioctl.fileid = 0;
    psmbcmdtscntq->value.ntioctl.isfsctl = NETWIB_TRUE;
    psmbcmdtscntq->value.ntioctl.flags = 0;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmdtscntq->value.ntioctl.data));
    break;
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTNOTIFY :
    psmbcmdtscntq->value.ntnotify.completionfilter = 0;
    psmbcmdtscntq->value.ntnotify.fileid = 0;
    psmbcmdtscntq->value.ntnotify.watchtree = 0;
    break;
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTQUERYSECUDESC :
    psmbcmdtscntq->value.ntquerysecudesc.fileid = 0;
    psmbcmdtscntq->value.ntquerysecudesc.secinfo = 0;
    break;
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTSETSECUDESC :
    psmbcmdtscntq->value.ntsetsecudesc.fileid = 0;
    psmbcmdtscntq->value.ntsetsecudesc.secinfo = 0;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmdtscntq->value.ntsetsecudesc.secudesc));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntq_close(netwox_smbcmdtscntq *psmbcmdtscntq)
{

  switch(psmbcmdtscntq->type) {
  case NETWOX_SMBCMDTSCNTQ_TYPE_UNKNOWN :
    break;
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTIOCTL :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmdtscntq->value.ntioctl.data));
    break;
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTNOTIFY :
    break;
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTQUERYSECUDESC :
    break;
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTSETSECUDESC :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmdtscntq->value.ntsetsecudesc.secudesc));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SMBCMDTSCNTQ_SUBCMD_NTIOCTL = 2,
  NETWOX_SMBCMDTSCNTQ_SUBCMD_NTSETSECUDESC = 3,
  NETWOX_SMBCMDTSCNTQ_SUBCMD_NTNOTIFY = 4,
  NETWOX_SMBCMDTSCNTQ_SUBCMD_NTQUERYSECUDESC = 6,
  NETWOX_SMBCMDTSCNTQ_SUBCMD_ntsetsecudesc = 123123
} netwox_smbcmdtscntq_subcmd;

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntq_encode_smbcmd(netwox_constsmbcmd *psmbcmd,
                                          netwox_smbcmdtscntq *psmbcmdtscntq)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize;

  if (psmbcmd->type != NETWOX_SMBCMD_TYPE_NTTRANSACT_Q) {
    return(NETWIB_ERR_PAINVALIDTYPE);
  }
  if (netwib__buf_ref_data_size(&psmbcmd->value.nttransact_q.para) != psmbcmd->value.nttransact_q.totalparasize) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  if (netwib__buf_ref_data_size(&psmbcmd->value.nttransact_q.data) != psmbcmd->value.nttransact_q.totaldatasize) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  if (psmbcmd->value.nttransact_q.setupcount > NETWOX_SMBCMD_NTTRANSACT_SETUP_MAX) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  para = netwib__buf_ref_data_ptr(&psmbcmd->value.nttransact_q.para);
  parasize = netwib__buf_ref_data_size(&psmbcmd->value.nttransact_q.para);
  data = netwib__buf_ref_data_ptr(&psmbcmd->value.nttransact_q.data);
  datasize = netwib__buf_ref_data_size(&psmbcmd->value.nttransact_q.data);

  switch(psmbcmd->value.nttransact_q.subcmd) {
  case NETWOX_SMBCMDTSCNTQ_SUBCMD_NTIOCTL :
    netwib_er(netwox_smbcmdtscntq_selecttype(psmbcmdtscntq,
                                          NETWOX_SMBCMDTSCNTQ_TYPE_NTIOCTL));
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (psmbcmd->value.nttransact_q.setupcount != 4) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    psmbcmdtscntq->value.ntioctl.function = (psmbcmd->value.nttransact_q.setup[1] << 16) | psmbcmd->value.nttransact_q.setup[0];
    psmbcmdtscntq->value.ntioctl.fileid = psmbcmd->value.nttransact_q.setup[2];
    psmbcmdtscntq->value.ntioctl.isfsctl = (netwib_bool)(psmbcmd->value.nttransact_q.setup[3] & 0xFF);
    psmbcmdtscntq->value.ntioctl.flags = (netwib_uint8)(psmbcmd->value.nttransact_q.setup[3] >> 8);
    netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, datasize, &psmbcmdtscntq->value.ntioctl.data));
    break;
  case NETWOX_SMBCMDTSCNTQ_SUBCMD_NTNOTIFY :
    netwib_er(netwox_smbcmdtscntq_selecttype(psmbcmdtscntq,
                                          NETWOX_SMBCMDTSCNTQ_TYPE_NTNOTIFY));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (psmbcmd->value.nttransact_q.setupcount != 4) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    psmbcmdtscntq->value.ntnotify.completionfilter = (psmbcmd->value.nttransact_q.setup[1] << 16) | psmbcmd->value.nttransact_q.setup[0];
    psmbcmdtscntq->value.ntnotify.fileid = psmbcmd->value.nttransact_q.setup[2];
    psmbcmdtscntq->value.ntnotify.watchtree = psmbcmd->value.nttransact_q.setup[3];
    break;
  case NETWOX_SMBCMDTSCNTQ_SUBCMD_NTQUERYSECUDESC :
    netwib_er(netwox_smbcmdtscntq_selecttype(psmbcmdtscntq,
                                NETWOX_SMBCMDTSCNTQ_TYPE_NTQUERYSECUDESC));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (psmbcmd->value.nttransact_q.setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize != 8) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmdtscntq->value.ntquerysecudesc.fileid);
    para += 2; parasize -= 2; /* reserved */
    netwib__data_decode_uint32_le(para, psmbcmdtscntq->value.ntquerysecudesc.secinfo);
    break;
  case NETWOX_SMBCMDTSCNTQ_SUBCMD_NTSETSECUDESC :
    netwib_er(netwox_smbcmdtscntq_selecttype(psmbcmdtscntq,
                                      NETWOX_SMBCMDTSCNTQ_TYPE_NTSETSECUDESC));
    if (psmbcmd->value.nttransact_q.setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize != 8) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmdtscntq->value.ntsetsecudesc.fileid);
    para += 2; parasize -=2; /* reserved */
    netwib__data_decode_uint32_le(para, psmbcmdtscntq->value.ntsetsecudesc.secinfo);
    netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, datasize, &psmbcmdtscntq->value.ntsetsecudesc.secudesc));
    break;
  default :
    return(NETWIB_ERR_NOTCONVERTED);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntq_decode_smbcmd(netwox_constsmbcmdtscntq *psmbcmdtscntq,
                                             netwox_smbcmd *psmbcmd)
{
  netwib_data para;

  switch(psmbcmdtscntq->type) {
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTIOCTL :
    psmbcmd->value.nttransact_q.subcmd = (netwib_uint16)NETWOX_SMBCMDTSCNTQ_SUBCMD_NTIOCTL;
    psmbcmd->value.nttransact_q.setupcount = 4;
    psmbcmd->value.nttransact_q.setup[0] = (netwib_uint16)(psmbcmdtscntq->value.ntioctl.function & 0xFFFF);
    psmbcmd->value.nttransact_q.setup[1] = (netwib_uint16)(psmbcmdtscntq->value.ntioctl.function >> 16);
    psmbcmd->value.nttransact_q.setup[2] = psmbcmdtscntq->value.ntioctl.fileid;
    psmbcmd->value.nttransact_q.setup[3] = (netwib_uint16)((psmbcmdtscntq->value.ntioctl.flags << 8 ) | (psmbcmdtscntq->value.ntioctl.isfsctl?1:0));
    netwib_er(netwib_buf_append_buf(&psmbcmdtscntq->value.ntioctl.data, &psmbcmd->value.nttransact_q.data));
    break;
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTNOTIFY :
    psmbcmd->value.nttransact_q.subcmd = (netwib_uint16)NETWOX_SMBCMDTSCNTQ_SUBCMD_NTNOTIFY;
    psmbcmd->value.nttransact_q.setupcount = 4;
    psmbcmd->value.nttransact_q.setup[0] = (netwib_uint16)(psmbcmdtscntq->value.ntnotify.completionfilter&0xFFFF);
    psmbcmd->value.nttransact_q.setup[1] = (netwib_uint16)(psmbcmdtscntq->value.ntnotify.completionfilter >> 16);
    psmbcmd->value.nttransact_q.setup[2] = psmbcmdtscntq->value.ntnotify.fileid;
    psmbcmd->value.nttransact_q.setup[3] = psmbcmdtscntq->value.ntnotify.watchtree;
    break;
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTQUERYSECUDESC :
    psmbcmd->value.nttransact_q.subcmd = (netwib_uint16)NETWOX_SMBCMDTSCNTQ_SUBCMD_NTQUERYSECUDESC;
    psmbcmd->value.nttransact_q.setupcount = 0;
    netwib_er(netwib_buf_wantspace(&psmbcmd->value.nttransact_q.para, 8, &para));
    netwib__data_append_uint16_le(para, psmbcmdtscntq->value.ntquerysecudesc.fileid);
    netwib_c_memset(para, 0, 2); para += 2; /* reserved */
    netwib__data_append_uint32_le(para, psmbcmdtscntq->value.ntquerysecudesc.secinfo);
    psmbcmd->value.nttransact_q.para.endoffset += 8;
    break;
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTSETSECUDESC :
    psmbcmd->value.nttransact_q.subcmd = (netwib_uint16)NETWOX_SMBCMDTSCNTQ_SUBCMD_NTSETSECUDESC;
    psmbcmd->value.nttransact_q.setupcount = 0;
    netwib_er(netwib_buf_wantspace(&psmbcmd->value.nttransact_q.para, 8, &para));
    netwib__data_append_uint16_le(para, psmbcmdtscntq->value.ntsetsecudesc.fileid);
    netwib_c_memset(para, 0, 2); para += 2; /* reserved */
    netwib__data_append_uint32_le(para, psmbcmdtscntq->value.ntsetsecudesc.secinfo);
    psmbcmd->value.nttransact_q.para.endoffset += 8;
    netwib_er(netwib_buf_append_buf(&psmbcmdtscntq->value.ntsetsecudesc.secudesc, &psmbcmd->value.nttransact_q.data));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  psmbcmd->value.nttransact_q.totalparasize = netwib__buf_ref_data_size(&psmbcmd->value.nttransact_q.para);
  psmbcmd->value.nttransact_q.totaldatasize = netwib__buf_ref_data_size(&psmbcmd->value.nttransact_q.data);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntq_show(netwox_constsmbcmdtscntq *psmbcmdtscntq,
                                   netwib_buf *pbuf)
{

  switch(psmbcmdtscntq->type) {
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTIOCTL :
    netwib_er(netwib_show_array_fmt32(pbuf, " NTIOCTL Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " function=%{uint32}=%{uint32:#08X}", psmbcmdtscntq->value.ntioctl.function, psmbcmdtscntq->value.ntioctl.function));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmdtscntq->value.ntioctl.fileid));
    netwib_er(netwib_show_array_fmt32(pbuf, " isfsctl=%{bool}", psmbcmdtscntq->value.ntioctl.isfsctl));
    netwib_er(netwib_show_array_fmt32(pbuf, " flags=%{uint8:#8b}", psmbcmdtscntq->value.ntioctl.flags));
    netwib_er(netwox_smbcmdcmn_data_show(" data", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmdtscntq->value.ntioctl.data, pbuf));
    break;
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTNOTIFY :
    netwib_er(netwib_show_array_fmt32(pbuf, " NTNOTIFY Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " completionfilter=%{uint32:#32b}", psmbcmdtscntq->value.ntnotify.completionfilter));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmdtscntq->value.ntnotify.fileid));
    netwib_er(netwib_show_array_fmt32(pbuf, " watchtree=%{uint16}", psmbcmdtscntq->value.ntnotify.watchtree));
    break;
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTQUERYSECUDESC :
    netwib_er(netwib_show_array_fmt32(pbuf, " NTQUERYSECUDESC Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmdtscntq->value.ntquerysecudesc.fileid));
    netwib_er(netwib_show_array_fmt32(pbuf, " secinfo=%{uint32:#32b}", psmbcmdtscntq->value.ntquerysecudesc.secinfo));
    break;
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTSETSECUDESC :
    netwib_er(netwib_show_array_fmt32(pbuf, " NTSETSECUDESC Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmdtscntq->value.ntsetsecudesc.fileid));
    netwib_er(netwib_show_array_fmt32(pbuf, " secinfo=%{uint32:#32b}", psmbcmdtscntq->value.ntquerysecudesc.secinfo));
    netwib_er(netwox_smbcmdcmn_data_show(" secudesc", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmdtscntq->value.ntsetsecudesc.secudesc, pbuf));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntq_display(netwib_bufpool *pbufpool,
                                       netwox_constsmbcmdtscntq *psmbcmdtscntq)
{
  netwib_buf *pbuf;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));

  ret = netwox_smbcmdtscntq_show(psmbcmdtscntq, pbuf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_display(pbuf, NETWIB_ENCODETYPE_DATA));
  } else {
    netwib_er(netwib_fmt_display("This TransactionNT Query could not be decoded\n"));
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(ret);
}
