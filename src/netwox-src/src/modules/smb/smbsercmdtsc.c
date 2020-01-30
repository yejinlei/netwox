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
#define NETWOX_SMBSERCMDTSC_FAKE_SHARE "share"
#define NETWOX_SMBSERCMDTSC_FAKE_SHARECOMMENT "Netwox share"
#define NETWOX_SMBSERCMDTSC_FAKE_SERVER "server"
#define NETWOX_SMBSERCMDTSC_FAKE_SERVERCOMMENT "Netwox server"
#define NETWOX_SMBSERCMDTSC_FAKE_SERVERMAJORVERSION NETWOX_SMBCMDTSCR_MAJORVERSION_NT4
#define NETWOX_SMBSERCMDTSC_FAKE_SERVERMINORVERSION NETWOX_SMBCMDTSCR_MINORVERSION_NT4
#define NETWOX_SMBSERCMDTSC_FAKE_SERVERTYPE NETWOX_SMBCMDCMN_SERVERTYPE_WORKSTATION | NETWOX_SMBCMDCMN_SERVERTYPE_NTWORKSTATION | NETWOX_SMBCMDCMN_SERVERTYPE_WIN95ANDMORE;


/*-------------------------------------------------------------*/
static netwib_err netwox_smbsercmd_transaction_netshareenum(netwox_smbser *psmbser,
                                                         netwox_constsmbcmdtscq *psmbcmdtscq,
                                                         netwib_constbuf *pshare,
                                                         netwox_smbcmdtscr *psmbcmdtscr)
{
  netwox_smbcmdtscr_netshareenum_share *pitem;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbsercmd_transaction_netshareenum\n"));
  }

  /* init */
  ret = NETWIB_ERR_OK;

  /* check query */
  /* ignore psmbcmdtscq->value.netshareenum.vfmt */
  /* ignore smbcmdtscq.value.netshareenum.receivebufferlen */

  /* construct answer */
  netwib_eg(netwox_smbcmdtscr_selecttype(psmbcmdtscr,
                                         NETWOX_SMBCMDTSCR_TYPE_NETSHAREENUM));
  netwib_eg(netwox_smbcmdtscr_setdefault(psmbcmdtscr));
  psmbcmdtscr->value.netshareenum.totalshares = 2;
  netwib_eg(netwib_array_ctl_set_size(&psmbcmdtscr->value.netshareenum.shares, 2));
  /* ipc$ */
  pitem = (netwox_smbcmdtscr_netshareenum_share *)psmbcmdtscr->value.netshareenum.shares.p[0];
  netwib_eg(netwox_smbcmdtscr_netshareenum_share_init(pitem));
  netwib_eg(netwox_smbcmdtscr_netshareenum_share_setdefault(pitem));
  pitem->sharetype = NETWOX_SMBCMDCMN_SHARETYPE_IPC;
  netwib_eg(netwib_buf_append_string("IPC$", &pitem->name));
  netwib_eg(netwib_buf_append_string("IPC", &pitem->comment));
  /* real share */
  pitem = (netwox_smbcmdtscr_netshareenum_share *)psmbcmdtscr->value.netshareenum.shares.p[1];
  netwib_eg(netwox_smbcmdtscr_netshareenum_share_init(pitem));
  netwib_eg(netwox_smbcmdtscr_netshareenum_share_setdefault(pitem));
  pitem->sharetype = NETWOX_SMBCMDCMN_SHARETYPE_DIR;
  if (netwib__buf_ref_data_size(pshare) != 0) {
    netwib_eg(netwib_buf_append_buf(pshare, &pitem->name));
  } else {
    netwib_eg(netwib_buf_append_string(NETWOX_SMBSERCMDTSC_FAKE_SHARE, &pitem->name));
  }
  netwib_eg(netwib_buf_append_string(NETWOX_SMBSERCMDTSC_FAKE_SHARECOMMENT, &pitem->comment));

  /* end */
 netwib_gotolabel:
  psmbcmdtscq = psmbcmdtscq; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbsercmd_transaction_netsharegetinfo(netwox_smbser *psmbser,
                                                         netwox_constsmbcmdtscq *psmbcmdtscq,
                                                         netwib_constbuf *pshare,
                                                         netwox_smbcmdtscr *psmbcmdtscr)
{
  netwib_cmp cmp = NETWIB_CMP_LT;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbsercmd_transaction_netsharegetinfo\n"));
  }

  /* init */
  ret = NETWIB_ERR_OK;

  /* check query */
  /* ignore psmbcmdtscq->value.netsharegetinfo.vfmt */
  /* ignore smbcmdtscq.value.netsharegetinfo.receivebufferlen */
  if (netwib__buf_ref_data_size(pshare) != 0) {
    netwib_eg(netwib_buf_cmp(&psmbcmdtscq->value.netsharegetinfo.share,
                             pshare, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      psmbser->smberrtosend = NETWOX_SMBERR_BADSHARENAME;
      netwib_goto(NETWIB_ERR_OK);
    }
  }

  /* construct answer */
  netwib_eg(netwox_smbcmdtscr_selecttype(psmbcmdtscr,
                                    NETWOX_SMBCMDTSCR_TYPE_NETSHAREGETINFO));
  netwib_eg(netwox_smbcmdtscr_setdefault(psmbcmdtscr));
  psmbcmdtscr->value.netsharegetinfo.sharetype = NETWOX_SMBCMDCMN_SHARETYPE_DIR;
  if (netwib__buf_ref_data_size(pshare) != 0) {
    netwib_eg(netwib_buf_append_buf(pshare, &psmbcmdtscr->value.netsharegetinfo.name));
  } else {
    netwib_eg(netwib_buf_append_string(NETWOX_SMBSERCMDTSC_FAKE_SHARE, &psmbcmdtscr->value.netsharegetinfo.name));
  }
  netwib_eg(netwib_buf_append_string(NETWOX_SMBSERCMDTSC_FAKE_SHARECOMMENT, &psmbcmdtscr->value.netsharegetinfo.comment));

  /* end */
 netwib_gotolabel:
  psmbcmdtscq = psmbcmdtscq; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbsercmd_transaction_netservergetinfo(netwox_smbser *psmbser,
                                                         netwox_constsmbcmdtscq *psmbcmdtscq,
                                                         netwox_smbcmdtscr *psmbcmdtscr)
{
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbsercmd_transaction_netservergetinfo\n"));
  }

  /* init */
  ret = NETWIB_ERR_OK;

  /* check query */
  /* ignore psmbcmdtscq->value.netservergetinfo.vfmt */
  /* ignore smbcmdtscq.value.netservergetinfo.receivebufferlen */

  /* construct answer */
  netwib_eg(netwox_smbcmdtscr_selecttype(psmbcmdtscr,
                                    NETWOX_SMBCMDTSCR_TYPE_NETSERVERGETINFO));
  netwib_eg(netwox_smbcmdtscr_setdefault(psmbcmdtscr));
  netwib_eg(netwib_buf_append_string(NETWOX_SMBSERCMDTSC_FAKE_SERVER, &psmbcmdtscr->value.netservergetinfo.name));
  psmbcmdtscr->value.netservergetinfo.majorversion = NETWOX_SMBSERCMDTSC_FAKE_SERVERMAJORVERSION;
  psmbcmdtscr->value.netservergetinfo.minorversion = NETWOX_SMBSERCMDTSC_FAKE_SERVERMINORVERSION;
  psmbcmdtscr->value.netservergetinfo.servertype = NETWOX_SMBSERCMDTSC_FAKE_SERVERTYPE;
  netwib_eg(netwib_buf_append_string(NETWOX_SMBSERCMDTSC_FAKE_SERVERCOMMENT, &psmbcmdtscr->value.netservergetinfo.comment));

  /* end */
 netwib_gotolabel:
  psmbcmdtscq = psmbcmdtscq; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbsercmd_transaction_netwkstagetinfo(netwox_smbser *psmbser,
                                                         netwox_constsmbcmdtscq *psmbcmdtscq,
                                                         netwox_smbcmdtscr *psmbcmdtscr)
{
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbsercmd_transaction_netwkstagetinfo\n"));
  }

  /* init */
  ret = NETWIB_ERR_OK;

  /* check query */
  /* ignore psmbcmdtscq->value.netwkstagetinfo.vfmt */
  /* ignore smbcmdtscq.value.netwkstagetinfo.receivebufferlen */

  /* construct answer */
  netwib_eg(netwox_smbcmdtscr_selecttype(psmbcmdtscr,
                                    NETWOX_SMBCMDTSCR_TYPE_NETWKSTAGETINFO));
  netwib_eg(netwox_smbcmdtscr_setdefault(psmbcmdtscr));
  netwib_eg(netwib_buf_append_string(NETWOX_SMBSERCMDTSC_FAKE_SERVER, &psmbcmdtscr->value.netwkstagetinfo.computername));
  psmbcmdtscr->value.netwkstagetinfo.majorversion = NETWOX_SMBCMDTSCR_MAJORVERSION_NT4;
  psmbcmdtscr->value.netwkstagetinfo.minorversion = NETWOX_SMBCMDTSCR_MINORVERSION_NT4;

  /* end */
 netwib_gotolabel:
  psmbcmdtscq = psmbcmdtscq; /* for compiler warning */
  return(ret);
}


/*-------------------------------------------------------------*/
static netwib_err netwox_smbsercmd_transaction_netserverenum2(netwox_smbser *psmbser,
                                                         netwox_constsmbcmdtscq *psmbcmdtscq,
                                                         netwox_smbcmdtscr *psmbcmdtscr)
{
  netwox_smbcmdtscr_netserverenum2_server *pitem;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbsercmd_transaction_netserverenum2\n"));
  }

  /* init */
  ret = NETWIB_ERR_OK;

  /* check query */
  /* ignore psmbcmdtscq->value.netserverenum2.vfmt */
  /* ignore smbcmdtscq.value.netserverenum2.receivebufferlen */

  /* construct answer */
  netwib_eg(netwox_smbcmdtscr_selecttype(psmbcmdtscr,
                                         NETWOX_SMBCMDTSCR_TYPE_NETSERVERENUM2));
  netwib_eg(netwox_smbcmdtscr_setdefault(psmbcmdtscr));
  psmbcmdtscr->value.netserverenum2.totalservers = 1;
  netwib_eg(netwib_array_ctl_set_size(&psmbcmdtscr->value.netserverenum2.servers, 2));
  /* real server */
  pitem = (netwox_smbcmdtscr_netserverenum2_server *)psmbcmdtscr->value.netserverenum2.servers.p[0];
  netwib_eg(netwox_smbcmdtscr_netserverenum2_server_init(pitem));
  netwib_eg(netwox_smbcmdtscr_netserverenum2_server_setdefault(pitem));
  netwib_eg(netwib_buf_append_string(NETWOX_SMBSERCMDTSC_FAKE_SERVER, &pitem->name));
  pitem->majorversion = NETWOX_SMBSERCMDTSC_FAKE_SERVERMAJORVERSION;
  pitem->minorversion = NETWOX_SMBSERCMDTSC_FAKE_SERVERMINORVERSION;
  pitem->servertype = NETWOX_SMBSERCMDTSC_FAKE_SERVERTYPE;
  netwib_eg(netwib_buf_append_string(NETWOX_SMBSERCMDTSC_FAKE_SERVERCOMMENT, &pitem->comment));

  /* end */
 netwib_gotolabel:
  psmbcmdtscq = psmbcmdtscq; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_transaction(netwox_smbser *psmbser,
                                        netwox_constsmbmsg *psmbmsgq,
                                        netwox_constsmbcmd *psmbcmdq,
                                        netwib_constbuf *pshare,
                                        netwib_bool *psendreply)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_buf *pbuf;
  netwox_smbmsg smbmsgr;
  netwox_smbcmd smbcmdr;
  netwox_smbcmdtscq smbcmdtscq;
  netwox_smbcmdtscrstorage smbcmdtscrstorage;
  netwox_smbcmdtscrfmt smbcmdtscrfmt;
  netwox_smbcmdtscr smbcmdtscr;
  netwib_uint32 offset, maxsize;
  netwib_bool needmoremsg;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbsercmd_transaction\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;
  netwib_er(netwox_smbcmdtscq_init(&smbcmdtscq));
  netwib_er(netwox_smbcmdtscrstorage_init(psession->capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_UNICODE, &smbcmdtscrstorage));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pbuf));
  netwib_er(netwox_smbcmdtscr_init(&smbcmdtscr));

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_TRANSACTION_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  /* ignore psmbcmdq->value.transaction_q.maxparasize, maxdatasize and
     maxsetupcount because our reply will not contain a lot of data, so it
     should be ok */
  /* ignore psmbcmdq->value.transaction_q.flags because its meaning is
     unknown */
  /* ignore psmbcmdq->value.transaction_q.timeout because we do not have
     delay in our answer */

  /* decode transaction and prepare answer */
  netwib_eg(netwox_smbcmdtscq_encode_smbcmd(psmbcmdq, &smbcmdtscq));
  if (psmbser->pcommon->debug) {
    netwib_eg(netwib_fmt_display("Received transaction query:\n"));
    netwib_eg(netwox_smbcmdtscq_display(psmbser->pcommon->pbufpool,
                                        &smbcmdtscq));
  }
  netwib_eg(netwox_smbcmdtscrfmt_init_smbcmdtscq(&smbcmdtscq, &smbcmdtscrfmt));
  switch(smbcmdtscq.type) {
  case NETWOX_SMBCMDTSCQ_TYPE_NETSHAREENUM:
    netwib_eg(netwox_smbsercmd_transaction_netshareenum(psmbser, &smbcmdtscq,
                                                        pshare, &smbcmdtscr));
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETSHAREGETINFO:
    netwib_eg(netwox_smbsercmd_transaction_netsharegetinfo(psmbser,
                                                           &smbcmdtscq, pshare,
                                                           &smbcmdtscr));
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETSERVERGETINFO:
    netwib_eg(netwox_smbsercmd_transaction_netservergetinfo(psmbser,
                                                            &smbcmdtscq,
                                                            &smbcmdtscr));
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETWKSTAGETINFO:
    netwib_er(netwox_smbser_nottested(psmbser));
    netwib_eg(netwox_smbsercmd_transaction_netwkstagetinfo(psmbser,
                                                           &smbcmdtscq,
                                                           &smbcmdtscr));
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETSERVERENUM2:
    netwib_er(netwox_smbser_nottested(psmbser));
    netwib_eg(netwox_smbsercmd_transaction_netserverenum2(psmbser, &smbcmdtscq,
                                                          &smbcmdtscr));
    break;
  default :
    /* reply packet is not implemented, so cannot answer */
    netwib_er(netwox_smbser_notimplemented(psmbser));
    psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
    break;
  }
  if (psmbser->smberrtosend != NETWOX_SMBERR_OK) {
    netwib_goto(NETWIB_ERR_OK);
  }
  if (psmbser->pcommon->debug) {
    netwib_eg(netwib_fmt_display("Sent transaction reply:\n"));
    netwib_eg(netwox_smbcmdtscr_display(psmbser->pcommon->pbufpool,
                                        &smbcmdtscr));
  }
  netwib_eg(netwox_smbcmdtscr_decode_storager(&smbcmdtscr, &smbcmdtscrfmt,
                                              &smbcmdtscrstorage));

  /* normally, none of our function should have created a transaction
     too big. But check in case. */
  if (netwib__buf_ref_data_size(&smbcmdtscrstorage.para) > psmbcmdq->value.transaction_q.maxparasize) {
    netwib_er(netwox_smbser_internalerror(psmbser));
    netwib_eg(NETWOX_ERR_INTERNALERROR);
  }
  if (netwib__buf_ref_data_size(&smbcmdtscrstorage.data) > psmbcmdq->value.transaction_q.maxdatasize) {
    netwib_er(netwox_smbser_internalerror(psmbser));
    netwib_eg(NETWOX_ERR_INTERNALERROR);
  }

  /* send replies */
  offset = 0;
  /* difficult to compute, so use a value which should be ok */
  maxsize = psmbser->cmn_maxmsgsize - NETWOX_SMBSER_SUFFICIENT_FOR_HEADERS;
  while(NETWIB_TRUE) {
    netwib_eg(netwox_smbser_send_begin(psmbser, psmbmsgq, &smbmsgr, &smbcmdr,
                                       NETWOX_SMBCMD_TYPE_TRANSACTION_R));
    netwib_eg(netwox_smbcmdtscrstorage_decode_smbcmd(&smbcmdtscrstorage,
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
  netwib_er(netwox_smbcmdtscr_close(&smbcmdtscr));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pbuf));
  netwib_er(netwox_smbcmdtscrstorage_close(&smbcmdtscrstorage));
  netwib_er(netwox_smbcmdtscq_close(&smbcmdtscq));
  return(ret);
}
