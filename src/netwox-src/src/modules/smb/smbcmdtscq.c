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
netwib_err netwox_smbcmdtscq_init(netwox_smbcmdtscq *psmbcmdtscq)
{
  psmbcmdtscq->type = NETWOX_SMBCMDTSCQ_TYPE_UNKNOWN;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscq_selecttype(netwox_smbcmdtscq *psmbcmdtscq,
                                        netwox_smbcmdtscq_type type)
{

  if (psmbcmdtscq->type != NETWOX_SMBCMDTSCQ_TYPE_UNKNOWN) {
    return(NETWOX_ERR_INTERNALERROR);
  }

  psmbcmdtscq->type = type;
  switch(type) {
  case NETWOX_SMBCMDTSCQ_TYPE_NETSHAREENUM :
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETSHAREGETINFO :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmdtscq->value.netsharegetinfo.share));
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETSERVERGETINFO :
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETWKSTAGETINFO :
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETSERVERENUM2 :
    break;
  default :
    psmbcmdtscq->type = NETWOX_SMBCMDTSCQ_TYPE_UNKNOWN;
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscq_setdefault(netwox_smbcmdtscq *psmbcmdtscq)
{

  switch(psmbcmdtscq->type) {
  case NETWOX_SMBCMDTSCQ_TYPE_NETSHAREENUM :
    psmbcmdtscq->value.netshareenum.vfmt = NETWOX_SMBCMDTSC_NETSHAREENUM_VFMT_1;
    psmbcmdtscq->value.netshareenum.receivebufferlen = 4096;
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETSHAREGETINFO :
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmdtscq->value.netsharegetinfo.share));
    psmbcmdtscq->value.netsharegetinfo.vfmt = NETWOX_SMBCMDTSC_NETSHAREGETINFO_VFMT_1;
    psmbcmdtscq->value.netsharegetinfo.receivebufferlen = 4096;
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETSERVERGETINFO :
    psmbcmdtscq->value.netservergetinfo.vfmt = NETWOX_SMBCMDTSC_NETSERVERGETINFO_VFMT_1;
    psmbcmdtscq->value.netservergetinfo.receivebufferlen = 4096;
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETWKSTAGETINFO :
    psmbcmdtscq->value.netwkstagetinfo.vfmt = NETWOX_SMBCMDTSC_NETWKSTAGETINFO_VFMT_1;
    psmbcmdtscq->value.netwkstagetinfo.receivebufferlen = 4096;
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETSERVERENUM2 :
    psmbcmdtscq->value.netserverenum2.vfmt = NETWOX_SMBCMDTSC_NETSERVERENUM2_VFMT_1;
    psmbcmdtscq->value.netserverenum2.receivebufferlen = 4096;
    psmbcmdtscq->value.netserverenum2.servertype = 0;
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscq_close(netwox_smbcmdtscq *psmbcmdtscq)
{

  switch(psmbcmdtscq->type) {
  case NETWOX_SMBCMDTSCQ_TYPE_UNKNOWN :
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETSHAREENUM :
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETSHAREGETINFO :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmdtscq->value.netsharegetinfo.share));
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETSERVERGETINFO :
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETWKSTAGETINFO :
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETSERVERENUM2 :
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* There are 2 kinds of queries (perhaps more) : it depends on
   psmbcmd->value.transaction_q.name
     NETWOX_SMBCMD_TRANSACTION_NAME_PIPE (_p)
     NETWOX_SMBCMD_TRANSACTION_NAME_PIPELANMAN (_plm)
   They both have their sub command numbering
 */
typedef enum {
  NETWOX_SMBCMDTSCQ_P_SUBCMD_DCERPCNMPIPE = 38 /* complex to implement, and I'm
                                                  not sure it's really
                                                  needed */
} netwox_smbcmdtscq_p_subcmd;
typedef enum {
  NETWOX_SMBCMDTSCQ_PLM_SUBCMD_NETSHAREENUM = 0,
  NETWOX_SMBCMDTSCQ_PLM_SUBCMD_NETSHAREGETINFO = 1,
  NETWOX_SMBCMDTSCQ_PLM_SUBCMD_NETSERVERGETINFO = 13,
  NETWOX_SMBCMDTSCQ_PLM_SUBCMD_NETWKSTAGETINFO = 63,
  NETWOX_SMBCMDTSCQ_PLM_SUBCMD_NETSERVERENUM2 = 104
} netwox_smbcmdtscq_plm_subcmd;

/*-------------------------------------------------------------*/
/* used for type 1 and 2 (do not exist now) */
#define NETWOX_SMBCMDTSC_NETSHAREENUM_DESCRIPTOR_PARAMETER_12 "WrLeh"
#define NETWOX_SMBCMDTSC_NETSHAREENUM_DESCRIPTOR_RETURN_1 "B13BWz"
/* when a type 2 will exist, will name it like this */
#define NETWOX_SMBCMDTSC_NETSHAREENUM_DESCRIPTOR_RETURN_2 "not used now"
#define NETWOX_SMBCMDTSC_NETSHAREENUM_FMT_1 1

/*-------------------------------------------------------------*/
#define NETWOX_SMBCMDTSC_NETSHAREGETINFO_DESCRIPTOR_PARAMETER_1 "zWrLh"
#define NETWOX_SMBCMDTSC_NETSHAREGETINFO_DESCRIPTOR_RETURN_1 "B13BWz"
#define NETWOX_SMBCMDTSC_NETSHAREGETINFO_FMT_1 1

/*-------------------------------------------------------------*/
#define NETWOX_SMBCMDTSC_NETSERVERGETINFO_DESCRIPTOR_PARAMETER_1 "WrLh"
#define NETWOX_SMBCMDTSC_NETSERVERGETINFO_DESCRIPTOR_RETURN_1 "B16BBDz"
#define NETWOX_SMBCMDTSC_NETSERVERGETINFO_FMT_1 1

/*-------------------------------------------------------------*/
#define NETWOX_SMBCMDTSC_NETWKSTAGETINFO_DESCRIPTOR_PARAMETER_1 "WrLh"
#define NETWOX_SMBCMDTSC_NETWKSTAGETINFO_DESCRIPTOR_RETURN_1 "zzzBBzz"
#define NETWOX_SMBCMDTSC_NETWKSTAGETINFO_FMT_1 10

/*-------------------------------------------------------------*/
#define NETWOX_SMBCMDTSC_NETSERVERENUM2_DESCRIPTOR_PARAMETER_1 "WrLehDO"
#define NETWOX_SMBCMDTSC_NETSERVERENUM2_DESCRIPTOR_RETURN_1 "B16BBDz"
#define NETWOX_SMBCMDTSC_NETSERVERENUM2_FMT_1 1

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcmdtscq_encode_tsc_p(const netwox_smbcmd_transaction_q *ptsc,
                                                 netwox_smbcmdtscq *psmbcmdtscq)
{
  netwox_smbcmdtscq_p_subcmd subcmd;

  if (ptsc->setupcount == 0) {
    return(NETWIB_ERR_DATAMISSING);
  }
  subcmd = ptsc->setup[0];

  switch(subcmd) {
  case NETWOX_SMBCMDTSCQ_P_SUBCMD_DCERPCNMPIPE :
    return(NETWOX_ERR_NOTIMPLEMENTED);
  default :
    return(NETWIB_ERR_NOTCONVERTED);
  }

  psmbcmdtscq = psmbcmdtscq; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcmdtscq_encode_tsc_plm(const netwox_smbcmd_transaction_q *ptsc,
                                                   netwox_smbcmdtscq *psmbcmdtscq)
{
  netwib_buf descriptor;
  netwib_data para;
  netwib_uint32 parasize, datasize;
  netwib_uint16 fmt;
  netwox_smbcmdtscq_plm_subcmd subcmd;
  netwib_cmp cmp;

  para = netwib__buf_ref_data_ptr(&ptsc->para);
  parasize = netwib__buf_ref_data_size(&ptsc->para);
  datasize = netwib__buf_ref_data_size(&ptsc->data);

  if (parasize < 2) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwib__data_decode_uint16_le(para, subcmd);
  parasize -= 2;

  switch(subcmd) {
  case NETWOX_SMBCMDTSCQ_PLM_SUBCMD_NETSHAREENUM :
    netwib_er(netwox_smbcmdtscq_selecttype(psmbcmdtscq, NETWOX_SMBCMDTSCQ_TYPE_NETSHAREENUM));
    if (datasize) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    if (ptsc->setupcount) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdcmn_data_decodestr(&para, &parasize, NETWIB_FALSE, &descriptor));
    netwib_er(netwib_buf_cmp_string(&descriptor, NETWOX_SMBCMDTSC_NETSHAREENUM_DESCRIPTOR_PARAMETER_12, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdcmn_data_decodestr(&para, &parasize, NETWIB_FALSE, &descriptor));
    netwib_er(netwib_buf_cmp_string(&descriptor, NETWOX_SMBCMDTSC_NETSHAREENUM_DESCRIPTOR_RETURN_1, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize != 4) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, fmt);
    if (fmt != NETWOX_SMBCMDTSC_NETSHAREENUM_FMT_1) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    psmbcmdtscq->value.netshareenum.vfmt = NETWOX_SMBCMDTSC_NETSHAREENUM_VFMT_1;
    netwib__data_decode_uint16_le(para, psmbcmdtscq->value.netshareenum.receivebufferlen);
    break;
  case NETWOX_SMBCMDTSCQ_PLM_SUBCMD_NETSHAREGETINFO :
    netwib_er(netwox_smbcmdtscq_selecttype(psmbcmdtscq, NETWOX_SMBCMDTSCQ_TYPE_NETSHAREGETINFO));
    if (datasize) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    if (ptsc->setupcount) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdcmn_data_decodestr(&para, &parasize, NETWIB_FALSE, &descriptor));
    netwib_er(netwib_buf_cmp_string(&descriptor, NETWOX_SMBCMDTSC_NETSHAREGETINFO_DESCRIPTOR_PARAMETER_1, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdcmn_data_decodestr(&para, &parasize, NETWIB_FALSE, &descriptor));
    netwib_er(netwib_buf_cmp_string(&descriptor, NETWOX_SMBCMDTSC_NETSHAREGETINFO_DESCRIPTOR_RETURN_1, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdcmn_data_decodestr(&para, &parasize, NETWIB_FALSE, &psmbcmdtscq->value.netsharegetinfo.share));
    if (parasize != 4) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, fmt);
    if (fmt != NETWOX_SMBCMDTSC_NETSHAREGETINFO_FMT_1) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    psmbcmdtscq->value.netsharegetinfo.vfmt = NETWOX_SMBCMDTSC_NETSHAREGETINFO_VFMT_1;
    netwib__data_decode_uint16_le(para, psmbcmdtscq->value.netsharegetinfo.receivebufferlen);
    break;
  case NETWOX_SMBCMDTSCQ_PLM_SUBCMD_NETSERVERGETINFO :
    netwib_er(netwox_smbcmdtscq_selecttype(psmbcmdtscq, NETWOX_SMBCMDTSCQ_TYPE_NETSERVERGETINFO));
    if (datasize) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    if (ptsc->setupcount) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdcmn_data_decodestr(&para, &parasize, NETWIB_FALSE, &descriptor));
    netwib_er(netwib_buf_cmp_string(&descriptor, NETWOX_SMBCMDTSC_NETSERVERGETINFO_DESCRIPTOR_PARAMETER_1, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdcmn_data_decodestr(&para, &parasize, NETWIB_FALSE, &descriptor));
    netwib_er(netwib_buf_cmp_string(&descriptor, NETWOX_SMBCMDTSC_NETSERVERGETINFO_DESCRIPTOR_RETURN_1, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize != 4) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, fmt);
    if (fmt != NETWOX_SMBCMDTSC_NETSERVERGETINFO_FMT_1) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    psmbcmdtscq->value.netservergetinfo.vfmt = NETWOX_SMBCMDTSC_NETSERVERGETINFO_VFMT_1;
    netwib__data_decode_uint16_le(para, psmbcmdtscq->value.netservergetinfo.receivebufferlen);
    break;
  case NETWOX_SMBCMDTSCQ_PLM_SUBCMD_NETWKSTAGETINFO :
    netwib_er(netwox_smbcmdtscq_selecttype(psmbcmdtscq, NETWOX_SMBCMDTSCQ_TYPE_NETWKSTAGETINFO));
    if (datasize) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    if (ptsc->setupcount) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdcmn_data_decodestr(&para, &parasize, NETWIB_FALSE, &descriptor));
    netwib_er(netwib_buf_cmp_string(&descriptor, NETWOX_SMBCMDTSC_NETWKSTAGETINFO_DESCRIPTOR_PARAMETER_1, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdcmn_data_decodestr(&para, &parasize, NETWIB_FALSE, &descriptor));
    netwib_er(netwib_buf_cmp_string(&descriptor, NETWOX_SMBCMDTSC_NETWKSTAGETINFO_DESCRIPTOR_RETURN_1, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize != 4) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, fmt);
    if (fmt != NETWOX_SMBCMDTSC_NETWKSTAGETINFO_FMT_1) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    psmbcmdtscq->value.netwkstagetinfo.vfmt = NETWOX_SMBCMDTSC_NETWKSTAGETINFO_VFMT_1;
    netwib__data_decode_uint16_le(para, psmbcmdtscq->value.netwkstagetinfo.receivebufferlen);
    break;
  case NETWOX_SMBCMDTSCQ_PLM_SUBCMD_NETSERVERENUM2 :
    netwib_er(netwox_smbcmdtscq_selecttype(psmbcmdtscq, NETWOX_SMBCMDTSCQ_TYPE_NETSERVERENUM2));
    if (datasize) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    if (ptsc->setupcount) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdcmn_data_decodestr(&para, &parasize, NETWIB_FALSE, &descriptor));
    netwib_er(netwib_buf_cmp_string(&descriptor, NETWOX_SMBCMDTSC_NETSERVERENUM2_DESCRIPTOR_PARAMETER_1, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdcmn_data_decodestr(&para, &parasize, NETWIB_FALSE, &descriptor));
    netwib_er(netwib_buf_cmp_string(&descriptor, NETWOX_SMBCMDTSC_NETSERVERENUM2_DESCRIPTOR_RETURN_1, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize != 8) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, fmt);
    if (fmt != NETWOX_SMBCMDTSC_NETSERVERENUM2_FMT_1) {
      netwib_er(NETWIB_ERR_NOTCONVERTED);
    }
    psmbcmdtscq->value.netserverenum2.vfmt = NETWOX_SMBCMDTSC_NETSERVERENUM2_VFMT_1;
    netwib__data_decode_uint16_le(para, psmbcmdtscq->value.netserverenum2.receivebufferlen);
    netwox__smbcmdcmn_servertype_decode(para, psmbcmdtscq->value.netserverenum2.servertype);
    break;
  default :
    netwib_er(NETWIB_ERR_NOTCONVERTED);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscq_encode_smbcmd(netwox_constsmbcmd *psmbcmd,
                                           netwox_smbcmdtscq *psmbcmdtscq)
{
  netwib_buf buftocmp, bufstr;
  netwib_byte array[80];
  netwib_cmp cmp;

  if (psmbcmd->type != NETWOX_SMBCMD_TYPE_TRANSACTION_Q) {
    return(NETWIB_ERR_PAINVALIDTYPE);
  }
  if (netwib__buf_ref_data_size(&psmbcmd->value.transaction_q.para) != psmbcmd->value.transaction_q.totalparasize) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  if (netwib__buf_ref_data_size(&psmbcmd->value.transaction_q.data) != psmbcmd->value.transaction_q.totaldatasize) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  if (psmbcmd->value.transaction_q.setupcount > NETWOX_SMBCMD_TRANSACTION_SETUP_MAX) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  if (psmbcmd->value.transaction_q.isunicode) {
    netwib_er(netwib_buf_init_ext_string(NETWOX_SMBCMD_TRANSACTION_NAME_PIPELANMAN, &bufstr));
    netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buftocmp));
    netwib_er(netwox_buf_decode_ucs2le(&bufstr, &buftocmp));
    netwib_er(netwib_buf_cmp(&psmbcmd->value.transaction_q.name, &buftocmp,
                             &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      netwib_er(netwox_smbcmdtscq_encode_tsc_plm(&psmbcmd->value.transaction_q, psmbcmdtscq));
      return(NETWIB_ERR_OK);
    }
    netwib_er(netwib_buf_init_ext_string(NETWOX_SMBCMD_TRANSACTION_NAME_PIPE,
                                       &bufstr));
    netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buftocmp));
    netwib_er(netwox_buf_decode_ucs2le(&bufstr, &buftocmp));
    netwib_er(netwib_buf_cmp(&psmbcmd->value.transaction_q.name, &buftocmp,
                             &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      netwib_er(netwox_smbcmdtscq_encode_tsc_p(&psmbcmd->value.transaction_q, psmbcmdtscq));
      return(NETWIB_ERR_OK);
    }
  } else {
    netwib_er(netwib_buf_cmp_string(&psmbcmd->value.transaction_q.name,
                                 NETWOX_SMBCMD_TRANSACTION_NAME_PIPELANMAN,
                                 &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      netwib_er(netwox_smbcmdtscq_encode_tsc_plm(&psmbcmd->value.transaction_q, psmbcmdtscq));
      return(NETWIB_ERR_OK);
    }
    netwib_er(netwib_buf_cmp_string(&psmbcmd->value.transaction_q.name,
                                 NETWOX_SMBCMD_TRANSACTION_NAME_PIPE,
                                 &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      netwib_er(netwox_smbcmdtscq_encode_tsc_p(&psmbcmd->value.transaction_q, psmbcmdtscq));
      return(NETWIB_ERR_OK);
    }
  }

  return(NETWIB_ERR_NOTCONVERTED);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscq_decode_smbcmd(netwox_constsmbcmdtscq *psmbcmdtscq,
                                           netwox_smbcmd *psmbcmd)
{
  netwib_data para;
  netwib_buf bufstr;

  switch(psmbcmdtscq->type) {
  case NETWOX_SMBCMDTSCQ_TYPE_NETSHAREENUM :
    if (psmbcmdtscq->value.netshareenum.vfmt != NETWOX_SMBCMDTSC_NETSHAREENUM_VFMT_1) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (psmbcmd->value.transaction_q.isunicode) {
      netwib_er(netwib_buf_init_ext_string(NETWOX_SMBCMD_TRANSACTION_NAME_PIPELANMAN, &bufstr));
      netwib_er(netwox_buf_decode_ucs2le(&bufstr, &psmbcmd->value.transaction_q.name));
    } else {
      netwib_er(netwib_buf_append_string(NETWOX_SMBCMD_TRANSACTION_NAME_PIPELANMAN, &psmbcmd->value.transaction_q.name));
    }
    netwib_er(netwib_buf_wantspace(&psmbcmd->value.transaction_q.para, 2, &para));
    netwib__data_append_uint16_le(para, NETWOX_SMBCMDTSCQ_PLM_SUBCMD_NETSHAREENUM);
    psmbcmd->value.transaction_q.para.endoffset += 2;
    netwib_er(netwib_buf_init_ext_string(NETWOX_SMBCMDTSC_NETSHAREENUM_DESCRIPTOR_PARAMETER_12, &bufstr));
    netwib_er(netwox_smbcmdcmn_data_appendstr(&bufstr, NETWIB_FALSE, &psmbcmd->value.transaction_q.para));
    netwib_er(netwib_buf_init_ext_string(NETWOX_SMBCMDTSC_NETSHAREENUM_DESCRIPTOR_RETURN_1, &bufstr));
    netwib_er(netwox_smbcmdcmn_data_appendstr(&bufstr, NETWIB_FALSE, &psmbcmd->value.transaction_q.para));
    netwib_er(netwib_buf_wantspace(&psmbcmd->value.transaction_q.para, 4, &para));
    netwib__data_append_uint16_le(para, NETWOX_SMBCMDTSC_NETSHAREENUM_FMT_1);
    netwib__data_append_uint16_le(para, psmbcmdtscq->value.netshareenum.receivebufferlen);
    psmbcmd->value.transaction_q.para.endoffset += 4;
    psmbcmd->value.transaction_q.totalparasize = (netwib_uint16)netwib__buf_ref_data_size(&psmbcmd->value.transaction_q.para);
    psmbcmd->value.transaction_q.totaldatasize = (netwib_uint16)netwib__buf_ref_data_size(&psmbcmd->value.transaction_q.data);
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETSHAREGETINFO :
    if (psmbcmdtscq->value.netsharegetinfo.vfmt != NETWOX_SMBCMDTSC_NETSHAREGETINFO_VFMT_1) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (psmbcmd->value.transaction_q.isunicode) {
      netwib_er(netwib_buf_init_ext_string(NETWOX_SMBCMD_TRANSACTION_NAME_PIPELANMAN, &bufstr));
      netwib_er(netwox_buf_decode_ucs2le(&bufstr, &psmbcmd->value.transaction_q.name));
    } else {
      netwib_er(netwib_buf_append_string(NETWOX_SMBCMD_TRANSACTION_NAME_PIPELANMAN, &psmbcmd->value.transaction_q.name));
    }
    netwib_er(netwib_buf_wantspace(&psmbcmd->value.transaction_q.para, 2, &para));
    netwib__data_append_uint16_le(para, NETWOX_SMBCMDTSCQ_PLM_SUBCMD_NETSHAREGETINFO);
    psmbcmd->value.transaction_q.para.endoffset += 2;
    netwib_er(netwib_buf_init_ext_string(NETWOX_SMBCMDTSC_NETSHAREGETINFO_DESCRIPTOR_PARAMETER_1, &bufstr));
    netwib_er(netwox_smbcmdcmn_data_appendstr(&bufstr, NETWIB_FALSE, &psmbcmd->value.transaction_q.para));
    netwib_er(netwib_buf_init_ext_string(NETWOX_SMBCMDTSC_NETSHAREGETINFO_DESCRIPTOR_RETURN_1, &bufstr));
    netwib_er(netwox_smbcmdcmn_data_appendstr(&bufstr, NETWIB_FALSE, &psmbcmd->value.transaction_q.para));
    netwib_er(netwox_smbcmdcmn_data_appendstr(&psmbcmdtscq->value.netsharegetinfo.share, NETWIB_FALSE, &psmbcmd->value.transaction_q.para));
    netwib_er(netwib_buf_wantspace(&psmbcmd->value.transaction_q.para, 4, &para));
    netwib__data_append_uint16_le(para, NETWOX_SMBCMDTSC_NETSHAREGETINFO_FMT_1);
    netwib__data_append_uint16_le(para, psmbcmdtscq->value.netsharegetinfo.receivebufferlen);
    psmbcmd->value.transaction_q.para.endoffset += 4;
    psmbcmd->value.transaction_q.totalparasize = (netwib_uint16)netwib__buf_ref_data_size(&psmbcmd->value.transaction_q.para);
    psmbcmd->value.transaction_q.totaldatasize = (netwib_uint16)netwib__buf_ref_data_size(&psmbcmd->value.transaction_q.data);
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETSERVERGETINFO :
    if (psmbcmdtscq->value.netservergetinfo.vfmt != NETWOX_SMBCMDTSC_NETSERVERGETINFO_VFMT_1) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (psmbcmd->value.transaction_q.isunicode) {
      netwib_er(netwib_buf_init_ext_string(NETWOX_SMBCMD_TRANSACTION_NAME_PIPELANMAN, &bufstr));
      netwib_er(netwox_buf_decode_ucs2le(&bufstr, &psmbcmd->value.transaction_q.name));
    } else {
      netwib_er(netwib_buf_append_string(NETWOX_SMBCMD_TRANSACTION_NAME_PIPELANMAN, &psmbcmd->value.transaction_q.name));
    }
    netwib_er(netwib_buf_wantspace(&psmbcmd->value.transaction_q.para, 2, &para));
    netwib__data_append_uint16_le(para, NETWOX_SMBCMDTSCQ_PLM_SUBCMD_NETSERVERGETINFO);
    psmbcmd->value.transaction_q.para.endoffset += 2;
    netwib_er(netwib_buf_init_ext_string(NETWOX_SMBCMDTSC_NETSERVERGETINFO_DESCRIPTOR_PARAMETER_1, &bufstr));
    netwib_er(netwox_smbcmdcmn_data_appendstr(&bufstr, NETWIB_FALSE, &psmbcmd->value.transaction_q.para));
    netwib_er(netwib_buf_init_ext_string(NETWOX_SMBCMDTSC_NETSERVERGETINFO_DESCRIPTOR_RETURN_1, &bufstr));
    netwib_er(netwox_smbcmdcmn_data_appendstr(&bufstr, NETWIB_FALSE, &psmbcmd->value.transaction_q.para));
    netwib_er(netwib_buf_wantspace(&psmbcmd->value.transaction_q.para, 4, &para));
    netwib__data_append_uint16_le(para, NETWOX_SMBCMDTSC_NETSERVERGETINFO_FMT_1);
    netwib__data_append_uint16_le(para, psmbcmdtscq->value.netservergetinfo.receivebufferlen);
    psmbcmd->value.transaction_q.para.endoffset += 4;
    psmbcmd->value.transaction_q.totalparasize = (netwib_uint16)netwib__buf_ref_data_size(&psmbcmd->value.transaction_q.para);
    psmbcmd->value.transaction_q.totaldatasize = (netwib_uint16)netwib__buf_ref_data_size(&psmbcmd->value.transaction_q.data);
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETWKSTAGETINFO :
    if (psmbcmdtscq->value.netwkstagetinfo.vfmt != NETWOX_SMBCMDTSC_NETWKSTAGETINFO_VFMT_1) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (psmbcmd->value.transaction_q.isunicode) {
      netwib_er(netwib_buf_init_ext_string(NETWOX_SMBCMD_TRANSACTION_NAME_PIPELANMAN, &bufstr));
      netwib_er(netwox_buf_decode_ucs2le(&bufstr, &psmbcmd->value.transaction_q.name));
    } else {
      netwib_er(netwib_buf_append_string(NETWOX_SMBCMD_TRANSACTION_NAME_PIPELANMAN, &psmbcmd->value.transaction_q.name));
    }
    netwib_er(netwib_buf_wantspace(&psmbcmd->value.transaction_q.para, 2, &para));
    netwib__data_append_uint16_le(para, NETWOX_SMBCMDTSCQ_PLM_SUBCMD_NETWKSTAGETINFO);
    psmbcmd->value.transaction_q.para.endoffset += 2;
    netwib_er(netwib_buf_init_ext_string(NETWOX_SMBCMDTSC_NETWKSTAGETINFO_DESCRIPTOR_PARAMETER_1, &bufstr));
    netwib_er(netwox_smbcmdcmn_data_appendstr(&bufstr, NETWIB_FALSE, &psmbcmd->value.transaction_q.para));
    netwib_er(netwib_buf_init_ext_string(NETWOX_SMBCMDTSC_NETWKSTAGETINFO_DESCRIPTOR_RETURN_1, &bufstr));
    netwib_er(netwox_smbcmdcmn_data_appendstr(&bufstr, NETWIB_FALSE, &psmbcmd->value.transaction_q.para));
    netwib_er(netwib_buf_wantspace(&psmbcmd->value.transaction_q.para, 4, &para));
    netwib__data_append_uint16_le(para, NETWOX_SMBCMDTSC_NETWKSTAGETINFO_FMT_1);
    netwib__data_append_uint16_le(para, psmbcmdtscq->value.netwkstagetinfo.receivebufferlen);
    psmbcmd->value.transaction_q.para.endoffset += 4;
    psmbcmd->value.transaction_q.totalparasize = (netwib_uint16)netwib__buf_ref_data_size(&psmbcmd->value.transaction_q.para);
    psmbcmd->value.transaction_q.totaldatasize = (netwib_uint16)netwib__buf_ref_data_size(&psmbcmd->value.transaction_q.data);
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETSERVERENUM2 :
    if (psmbcmdtscq->value.netserverenum2.vfmt != NETWOX_SMBCMDTSC_NETSERVERENUM2_VFMT_1) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (psmbcmd->value.transaction_q.isunicode) {
      netwib_er(netwib_buf_init_ext_string(NETWOX_SMBCMD_TRANSACTION_NAME_PIPELANMAN, &bufstr));
      netwib_er(netwox_buf_decode_ucs2le(&bufstr, &psmbcmd->value.transaction_q.name));
    } else {
      netwib_er(netwib_buf_append_string(NETWOX_SMBCMD_TRANSACTION_NAME_PIPELANMAN, &psmbcmd->value.transaction_q.name));
    }
    netwib_er(netwib_buf_wantspace(&psmbcmd->value.transaction_q.para, 2, &para));
    netwib__data_append_uint16_le(para, NETWOX_SMBCMDTSCQ_PLM_SUBCMD_NETSERVERENUM2);
    psmbcmd->value.transaction_q.para.endoffset += 2;
    netwib_er(netwib_buf_init_ext_string(NETWOX_SMBCMDTSC_NETSERVERENUM2_DESCRIPTOR_PARAMETER_1, &bufstr));
    netwib_er(netwox_smbcmdcmn_data_appendstr(&bufstr, NETWIB_FALSE, &psmbcmd->value.transaction_q.para));
    netwib_er(netwib_buf_init_ext_string(NETWOX_SMBCMDTSC_NETSERVERENUM2_DESCRIPTOR_RETURN_1, &bufstr));
    netwib_er(netwox_smbcmdcmn_data_appendstr(&bufstr, NETWIB_FALSE, &psmbcmd->value.transaction_q.para));
    netwib_er(netwib_buf_wantspace(&psmbcmd->value.transaction_q.para, 8, &para));
    netwib__data_append_uint16_le(para, NETWOX_SMBCMDTSC_NETSERVERENUM2_FMT_1);
    netwib__data_append_uint16_le(para, psmbcmdtscq->value.netserverenum2.receivebufferlen);
    netwox__smbcmdcmn_servertype_append(para, psmbcmdtscq->value.netserverenum2.servertype);
    psmbcmd->value.transaction_q.para.endoffset += 8;
    psmbcmd->value.transaction_q.totalparasize = (netwib_uint16)netwib__buf_ref_data_size(&psmbcmd->value.transaction_q.para);
    psmbcmd->value.transaction_q.totaldatasize = (netwib_uint16)netwib__buf_ref_data_size(&psmbcmd->value.transaction_q.data);
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscq_show(netwox_constsmbcmdtscq *psmbcmdtscq,
                                 netwib_buf *pbuf)
{

  switch(psmbcmdtscq->type) {
  case NETWOX_SMBCMDTSCQ_TYPE_NETSHAREENUM :
    netwib_er(netwib_show_array_fmt32(pbuf, " Net Share Enum Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " netwox_vfmt=%{uint32}", psmbcmdtscq->value.netshareenum.vfmt));
    netwib_er(netwib_show_array_fmt32(pbuf, " receivebufferlen=%{uint16}", psmbcmdtscq->value.netshareenum.receivebufferlen));
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETSHAREGETINFO :
    netwib_er(netwib_show_array_fmt32(pbuf, " Net Share Get Info Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " netwox_vfmt=%{uint32}", psmbcmdtscq->value.netsharegetinfo.vfmt));
    netwib_er(netwox_smbcmdcmn_data_show(" share", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI, &psmbcmdtscq->value.netsharegetinfo.share, pbuf));
    netwib_er(netwib_show_array_fmt32(pbuf, " receivebufferlen=%{uint16}", psmbcmdtscq->value.netsharegetinfo.receivebufferlen));
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETSERVERGETINFO :
    netwib_er(netwib_show_array_fmt32(pbuf, " Net Server Get Info Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " netwox_vfmt=%{uint32}", psmbcmdtscq->value.netservergetinfo.vfmt));
    netwib_er(netwib_show_array_fmt32(pbuf, " receivebufferlen=%{uint16}", psmbcmdtscq->value.netservergetinfo.receivebufferlen));
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETWKSTAGETINFO :
    netwib_er(netwib_show_array_fmt32(pbuf, " Net Workstation Get Info Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " netwox_vfmt=%{uint32}", psmbcmdtscq->value.netwkstagetinfo.vfmt));
    netwib_er(netwib_show_array_fmt32(pbuf, " receivebufferlen=%{uint16}", psmbcmdtscq->value.netwkstagetinfo.receivebufferlen));
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETSERVERENUM2 :
    netwib_er(netwib_show_array_fmt32(pbuf, " Net Server Enum2 Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " netwox_vfmt=%{uint32}", psmbcmdtscq->value.netserverenum2.vfmt));
    netwib_er(netwib_show_array_fmt32(pbuf, " receivebufferlen=%{uint16}", psmbcmdtscq->value.netserverenum2.receivebufferlen));
    netwib_er(netwox_smbcmdcmn_servertype_show(" servertype",  psmbcmdtscq->value.netserverenum2.servertype, pbuf));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscq_display(netwib_bufpool *pbufpool,
                                     netwox_constsmbcmdtscq *psmbcmdtscq)
{
  netwib_buf *pbuf;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));

  ret = netwox_smbcmdtscq_show(psmbcmdtscq, pbuf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_display(pbuf, NETWIB_ENCODETYPE_DATA));
  } else {
    netwib_er(netwib_fmt_display("This Transaction Query could not be decoded\n"));
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(ret);
}
