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
static netwib_err netwox_smbsercmd_nttransact_ntioctl(netwox_smbser *psmbser,
                                                   netwox_constsmbmsg *psmbmsgq,
                                                   netwox_constsmbcmdtscntq *psmbcmdtscntq,
                                                   netwox_smbcmdtscntr *psmbcmdtscntr)
{
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbsercmd_nttransact_ntioctl\n"));
  }

  /* init */
  ret = NETWIB_ERR_OK;

  /* check query */
  if (!psmbcmdtscntq->value.ntioctl.isfsctl) {
    psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
    netwib_goto(NETWIB_ERR_OK);
  }
  /* ignore psmbcmdtscntq->value.ntioctl.flags */

  /* construct answer */
  switch(psmbcmdtscntq->value.ntioctl.function) {
  case NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_ISVOLUMEMOUNTED:
    /* just answer it's mounted (no data to set) */
    netwib_eg(netwox_smbcmdtscntr_selecttype(psmbcmdtscntr, NETWOX_SMBCMDTSCNTR_TYPE_NTIOCTL));
    netwib_eg(netwox_smbcmdtscntr_setdefault(psmbcmdtscntr));
    break;
  case NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_GETREPARSEPOINT:
    psmbser->smberrtosend = NETWOX_SMBERR_NOTAREPARSEPOINT;
    netwib_goto(NETWIB_ERR_OK);
    break;
  case NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_CREATEORGETOBJECTID:
    /* too complex */
    psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
    netwib_goto(NETWIB_ERR_OK);
    break;
  default:
    psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
    netwib_goto(NETWIB_ERR_OK);
    break;
  }

  /* end */
 netwib_gotolabel:
  psmbmsgq = psmbmsgq; /* for compiler warning */
  psmbcmdtscntq = psmbcmdtscntq; /* for compiler warning */
  psmbcmdtscntr = psmbcmdtscntr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbsercmd_nttransact_ntnotify(netwox_smbser *psmbser,
                                                    netwox_constsmbmsg *psmbmsgq,
                                                    netwox_constsmbcmdtscntq *psmbcmdtscntq,
                                                    netwox_smbcmdtscntr *psmbcmdtscntr)
{
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbsercmd_nttransact_ntnotify\n"));
  }

  /* init */
  ret = NETWIB_ERR_OK;

  /* ignore psmbcmdtscntq->value.ntnotify.completionfilter, fileid, watchtree */

  /* construct answer */
  /* currently not implemented */
  netwib_er(netwox_smbser_notimplemented(psmbser));
  psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
  netwib_goto(NETWIB_ERR_OK);

  /* end */
 netwib_gotolabel:
  psmbmsgq = psmbmsgq; /* for compiler warning */
  psmbcmdtscntq = psmbcmdtscntq; /* for compiler warning */
  psmbcmdtscntr = psmbcmdtscntr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbsercmd_nttransact_ntquerysecudesc(netwox_smbser *psmbser,
                                                           netwox_constsmbmsg *psmbmsgq,
                                                           netwox_constsmbcmdtscntq *psmbcmdtscntq,
                                                           netwox_smbcmdtscntr *psmbcmdtscntr)
{
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbsercmd_nttransact_ntquerysecudesc\n"));
  }

  /* init */
  ret = NETWIB_ERR_OK;

  /* ignore psmbcmdtscntq->value.ntquerysecudesc.fileid, secinfo */

  /* construct answer */
  /* currently not implemented */
  psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
  netwib_goto(NETWIB_ERR_OK);

  /* end */
 netwib_gotolabel:
  psmbmsgq = psmbmsgq; /* for compiler warning */
  psmbcmdtscntq = psmbcmdtscntq; /* for compiler warning */
  psmbcmdtscntr = psmbcmdtscntr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbsercmd_nttransact_ntsetsecudesc(netwox_smbser *psmbser,
                                                         netwox_constsmbmsg *psmbmsgq,
                                                         netwox_constsmbcmdtscntq *psmbcmdtscntq,
                                                         netwox_smbcmdtscntr *psmbcmdtscntr)
{
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbsercmd_nttransact_ntsetsecudesc\n"));
  }

  /* init */
  ret = NETWIB_ERR_OK;

  /* ignore psmbcmdtscntq->value.ntsetsecudesc.fileid, secinfo, secudesc */

  /* construct answer */
  /* currently not implemented */
  netwib_er(netwox_smbser_notimplemented(psmbser));
  psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
  netwib_goto(NETWIB_ERR_OK);

  /* end */
 netwib_gotolabel:
  psmbmsgq = psmbmsgq; /* for compiler warning */
  psmbcmdtscntq = psmbcmdtscntq; /* for compiler warning */
  psmbcmdtscntr = psmbcmdtscntr; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_nttransact(netwox_smbser *psmbser,
                                       netwox_constsmbmsg *psmbmsgq,
                                       netwox_constsmbcmd *psmbcmdq,
                                       netwib_bool *psendreply)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_buf *pbuf;
  netwox_smbmsg smbmsgr;
  netwox_smbcmd smbcmdr;
  netwox_smbcmdtscntq smbcmdtscntq;
  netwox_smbcmdtscntrstorage smbcmdtscntrstorage;
  netwox_smbcmdtscntrfmt smbcmdtscntrfmt;
  netwox_smbcmdtscntr smbcmdtscntr;
  netwib_uint32 offset, maxsize;
  netwib_bool needmoremsg;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbsercmd_nttransact\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;
  netwib_er(netwox_smbcmdtscntq_init(&smbcmdtscntq));
  netwib_er(netwox_smbcmdtscntrstorage_init(psession->capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_UNICODE, &smbcmdtscntrstorage));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pbuf));
  netwib_er(netwox_smbcmdtscntr_init(&smbcmdtscntr));

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_NTTRANSACT_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  /* ignore psmbcmdq->value.nttransact_q.maxparasize, maxdatasize and
     maxsetupcount because our reply will not contain a lot of data, so it
     should be ok */

  /* decode nttransact and prepare answer */
  netwib_eg(netwox_smbcmdtscntq_encode_smbcmd(psmbcmdq, &smbcmdtscntq));
  if (psmbser->pcommon->debug) {
    netwib_eg(netwib_fmt_display("Received nttransact query:\n"));
    netwib_eg(netwox_smbcmdtscntq_display(psmbser->pcommon->pbufpool,
                                          &smbcmdtscntq));
  }
  netwib_eg(netwox_smbcmdtscntrfmt_init_smbcmdtscntq(&smbcmdtscntq, &smbcmdtscntrfmt));
  switch(smbcmdtscntq.type) {
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTIOCTL:
    netwib_eg(netwox_smbsercmd_nttransact_ntioctl(psmbser, psmbmsgq, &smbcmdtscntq, &smbcmdtscntr));
    break;
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTNOTIFY:
    netwib_eg(netwox_smbsercmd_nttransact_ntnotify(psmbser, psmbmsgq, &smbcmdtscntq, &smbcmdtscntr));
    break;
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTQUERYSECUDESC:
    netwib_eg(netwox_smbsercmd_nttransact_ntquerysecudesc(psmbser, psmbmsgq, &smbcmdtscntq, &smbcmdtscntr));
    break;
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTSETSECUDESC:
    netwib_eg(netwox_smbsercmd_nttransact_ntsetsecudesc(psmbser, psmbmsgq, &smbcmdtscntq, &smbcmdtscntr));
    break;
  default :
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (psmbser->smberrtosend != NETWOX_SMBERR_OK) {
    netwib_goto(NETWIB_ERR_OK);
  }
  if (psmbser->pcommon->debug) {
    netwib_eg(netwib_fmt_display("Sent nttransact reply:\n"));
    netwib_eg(netwox_smbcmdtscntr_display(psmbser->pcommon->pbufpool,
                                          &smbcmdtscntr));
  }
  netwib_eg(netwox_smbcmdtscntr_decode_storager(&smbcmdtscntr,
                                                &smbcmdtscntrfmt,
                                                &smbcmdtscntrstorage));

  /* normally, none of our function should have created a transaction
     too big. But check in case. */
  if (netwib__buf_ref_data_size(&smbcmdtscntrstorage.para) > psmbcmdq->value.nttransact_q.maxparasize) {
    netwib_er(netwox_smbser_internalerror(psmbser));
    netwib_eg(NETWOX_ERR_INTERNALERROR);
  }
  if (netwib__buf_ref_data_size(&smbcmdtscntrstorage.data) > psmbcmdq->value.nttransact_q.maxdatasize) {
    netwib_er(netwox_smbser_internalerror(psmbser));
    netwib_eg(NETWOX_ERR_INTERNALERROR);
  }

  /* send replies */
  offset = 0;
  /* difficult to compute, so use a value which should be ok */
  maxsize = psmbser->cmn_maxmsgsize - NETWOX_SMBSER_SUFFICIENT_FOR_HEADERS;
  while(NETWIB_TRUE) {
    netwib_eg(netwox_smbser_send_begin(psmbser, psmbmsgq, &smbmsgr, &smbcmdr,
                                       NETWOX_SMBCMD_TYPE_NTTRANSACT_R));
    netwib_eg(netwox_smbcmdtscntrstorage_decode_smbcmd(&smbcmdtscntrstorage,
                                                     &offset, maxsize,
                                                     &smbcmdr, &needmoremsg));
    netwib_eg(netwox_smbser_send_end(psmbser, &smbmsgr, &smbcmdr));
    if (!needmoremsg) {
      break;
    }
  }
  *psendreply = NETWIB_FALSE;

  /* end */
 netwib_gotolabel:
  netwib_er(netwox_smbcmdtscntr_close(&smbcmdtscntr));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pbuf));
  netwib_er(netwox_smbcmdtscntrstorage_close(&smbcmdtscntrstorage));
  netwib_er(netwox_smbcmdtscntq_close(&smbcmdtscntq));
  return(ret);
}
