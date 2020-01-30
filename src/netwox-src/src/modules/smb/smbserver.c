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
netwib_err netwox_smbserver_init(netwox_sockinfo *psockinfo,
                                 netwox_smbserver *psmbserver)
{
  /* those can be changed by users */
  psmbserver->maxwaitms = 180000;
  psmbserver->pallowedclients = NULL;
  netwib_er(netwib_buf_init_ext_storage(&psmbserver->user));
  netwib_er(netwib_buf_init_ext_storage(&psmbserver->password));
  psmbserver->password.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  netwib_er(netwib_buf_init_ext_storage(&psmbserver->domain));
  netwib_er(netwib_buf_append_string("mydomain", &psmbserver->domain));
  netwib_er(netwib_buf_init_ext_storage(&psmbserver->share));
  netwib_er(netwib_buf_init_ext_storage(&psmbserver->rootdir));
  netwib_er(netwib_buf_append_string(".", &psmbserver->rootdir));
  psmbserver->allowput = NETWIB_TRUE;
  psmbserver->debug = NETWIB_FALSE;
  psmbserver->verbose = NETWIB_FALSE;

  /* internal */
  netwib_er(netwib_bufpool_init(NETWIB_TRUE, &psmbserver->pbufpool));

  switch(psockinfo->localport) {
  case NETWOX_SERVICES_SMBNBTSS :
    psmbserver->isnaked = NETWIB_FALSE;
    break;
  case NETWOX_SERVICES_SMBNAKED :
    psmbserver->isnaked = NETWIB_TRUE;
    break;
  default :
    /* don't know if this is correct */
    psmbserver->isnaked = NETWIB_TRUE;
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbserver_close(netwox_smbserver *psmbserver)
{
  netwib_er(netwib_bufpool_close(&psmbserver->pbufpool));

  netwib_er(netwib_buf_close(&psmbserver->user));
  netwib_er(netwib_buf_close(&psmbserver->password));
  netwib_er(netwib_buf_close(&psmbserver->share));
  netwib_er(netwib_buf_close(&psmbserver->domain));
  netwib_er(netwib_buf_close(&psmbserver->rootdir));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_smbserver_reply(netwox_constsmbserver *psmbserver,
                                              netwox_smbser *psmbser,
                                              netwox_constsmbmsg *psmbmsgq,
                                              netwox_constsmbcmd *psmbcmdq,
                                              netwox_smbmsg *psmbmsgr,
                                              netwox_smbcmd *psmbcmdr,
                                              netwib_bool *psendreply)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_bool isipc;
  netwib_err ret;

  isipc = NETWIB_FALSE;
  ret = netwox_smbser_sessiontree_check(psmbser, &psession, &ptree);
  if (ret == NETWIB_ERR_OK) {
    isipc = ptree->isipc;
  }

  /* check allowput and isipc */
  switch(psmbcmdq->type) {
  case NETWOX_SMBCMD_TYPE_TRANSACTION_Q :
  case NETWOX_SMBCMD_TYPE_ECHO_Q :
  case NETWOX_SMBCMD_TYPE_TREEDISCONNECT_Q :
  case NETWOX_SMBCMD_TYPE_NEGOTIATE_Q :
  case NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_Q0 :
  case NETWOX_SMBCMD_TYPE_LOGOFFANDX_Q :
  case NETWOX_SMBCMD_TYPE_TREECONNECTANDX_Q :
    break;
  case NETWOX_SMBCMD_TYPE_CREATEDIRECTORY_Q :
  case NETWOX_SMBCMD_TYPE_DELETEDIRECTORY_Q :
  case NETWOX_SMBCMD_TYPE_CREATE_Q :
  case NETWOX_SMBCMD_TYPE_FLUSH_Q :
  case NETWOX_SMBCMD_TYPE_DELETE_Q :
  case NETWOX_SMBCMD_TYPE_RENAME_Q :
  case NETWOX_SMBCMD_TYPE_SETINFORMATION_Q :
  case NETWOX_SMBCMD_TYPE_WRITE_Q :
  case NETWOX_SMBCMD_TYPE_CREATETEMPORARY_Q :
  case NETWOX_SMBCMD_TYPE_CREATENEW_Q :
  case NETWOX_SMBCMD_TYPE_SETINFORMATION2_Q :
  case NETWOX_SMBCMD_TYPE_WRITEANDX_Q :
  case NETWOX_SMBCMD_TYPE_NTRENAME_Q :
    if (!psmbserver->allowput) {
      psmbser->smberrtosend = NETWOX_SMBERR_DEVICEWRITEPROTECTED;
    }
    if (isipc) {
      psmbser->smberrtosend = NETWOX_SMBERR_NOTSUPPORTED;
    }
    break;
  case NETWOX_SMBCMD_TYPE_OPEN_Q :
  case NETWOX_SMBCMD_TYPE_CLOSE_Q :
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATION_Q :
  case NETWOX_SMBCMD_TYPE_READ_Q :
  case NETWOX_SMBCMD_TYPE_CHECKDIRPATH_Q :
  case NETWOX_SMBCMD_TYPE_SEEK_Q :
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATION2_Q :
  case NETWOX_SMBCMD_TYPE_LOCKINGANDX_Q :
  case NETWOX_SMBCMD_TYPE_OPENANDX_Q :
  case NETWOX_SMBCMD_TYPE_READANDX_Q :
  case NETWOX_SMBCMD_TYPE_CLOSETREEDISCONNECT_Q :
  case NETWOX_SMBCMD_TYPE_TRANSACTION2_Q :
  case NETWOX_SMBCMD_TYPE_FINDCLOSE2_Q :
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATIONDISK_Q :
  case NETWOX_SMBCMD_TYPE_SEARCH_Q :
  case NETWOX_SMBCMD_TYPE_NTTRANSACT_Q :
  case NETWOX_SMBCMD_TYPE_NTCREATEANDX_Q :
  case NETWOX_SMBCMD_TYPE_NTCANCEL_Q :
    if (isipc) {
      psmbser->smberrtosend = NETWOX_SMBERR_NOTSUPPORTED;
    }
    break;
  default :
    netwib_er(netwox_smbser_notimplemented(psmbser));
    psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
  }
  if (psmbser->smberrtosend != NETWOX_SMBERR_OK) {
    return(NETWIB_ERR_OK);
  }

  /* answer */
  switch(psmbcmdq->type) {
  case NETWOX_SMBCMD_TYPE_CREATEDIRECTORY_Q :
    netwib_er(netwox_smbsercmd_createdirectory(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr, &psmbserver->rootdir));
    break;
  case NETWOX_SMBCMD_TYPE_DELETEDIRECTORY_Q :
    netwib_er(netwox_smbsercmd_deletedirectory(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr, &psmbserver->rootdir));
    break;
  case NETWOX_SMBCMD_TYPE_CREATE_Q :
    netwib_er(netwox_smbsercmd_create(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr, &psmbserver->rootdir));
    break;
  case NETWOX_SMBCMD_TYPE_OPEN_Q :
    netwib_er(netwox_smbser_nottested(psmbser));
    netwib_er(netwox_smbsercmd_open(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr, &psmbserver->rootdir));
    break;
  case NETWOX_SMBCMD_TYPE_CLOSE_Q :
    netwib_er(netwox_smbsercmd_close(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr));
    break;
  case NETWOX_SMBCMD_TYPE_FLUSH_Q :
    netwib_er(netwox_smbsercmd_flush(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr));
    break;
  case NETWOX_SMBCMD_TYPE_DELETE_Q :
    netwib_er(netwox_smbsercmd_delete(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr, &psmbserver->rootdir));
    break;
  case NETWOX_SMBCMD_TYPE_RENAME_Q :
    netwib_er(netwox_smbsercmd_rename(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr, &psmbserver->rootdir));
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATION_Q :
    netwib_er(netwox_smbsercmd_queryinformation(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr, &psmbserver->rootdir));
    break;
  case NETWOX_SMBCMD_TYPE_SETINFORMATION_Q :
    netwib_er(netwox_smbsercmd_setinformation(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr, &psmbserver->rootdir));
    break;
  case NETWOX_SMBCMD_TYPE_READ_Q :
    netwib_er(netwox_smbsercmd_read(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr));
    break;
  case NETWOX_SMBCMD_TYPE_WRITE_Q :
    netwib_er(netwox_smbsercmd_write(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr));
    break;
  case NETWOX_SMBCMD_TYPE_CREATETEMPORARY_Q :
    netwib_er(netwox_smbser_nottested(psmbser));
    netwib_er(netwox_smbsercmd_createtemporary(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr, &psmbserver->rootdir));
    break;
  case NETWOX_SMBCMD_TYPE_CREATENEW_Q :
    netwib_er(netwox_smbser_nottested(psmbser));
    netwib_er(netwox_smbsercmd_createnew(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr, &psmbserver->rootdir));
    break;
  case NETWOX_SMBCMD_TYPE_CHECKDIRPATH_Q :
    netwib_er(netwox_smbsercmd_checkdirpath(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr, &psmbserver->rootdir));
    break;
  case NETWOX_SMBCMD_TYPE_SEEK_Q :
    netwib_er(netwox_smbsercmd_seek(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr));
    break;
  case NETWOX_SMBCMD_TYPE_SETINFORMATION2_Q :
    netwib_er(netwox_smbser_nottested(psmbser));
    netwib_er(netwox_smbsercmd_setinformation2(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr));
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATION2_Q :
    netwib_er(netwox_smbser_nottested(psmbser));
    netwib_er(netwox_smbsercmd_queryinformation2(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr));
    break;
  case NETWOX_SMBCMD_TYPE_LOCKINGANDX_Q :
    netwib_er(netwox_smbsercmd_lockingandx(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr));
    break;
  case NETWOX_SMBCMD_TYPE_TRANSACTION_Q :
    netwib_er(netwox_smbsercmd_transaction(psmbser, psmbmsgq, psmbcmdq, &psmbserver->share, psendreply));
    break;
  case NETWOX_SMBCMD_TYPE_ECHO_Q :
    netwib_er(netwox_smbsercmd_echo(psmbser, psmbmsgq, psmbcmdq, psendreply));
    break;
  case NETWOX_SMBCMD_TYPE_OPENANDX_Q :
    netwib_er(netwox_smbsercmd_openandx(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr, &psmbserver->rootdir));
    break;
  case NETWOX_SMBCMD_TYPE_READANDX_Q :
    netwib_er(netwox_smbsercmd_readandx(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr));
    break;
  case NETWOX_SMBCMD_TYPE_WRITEANDX_Q :
    netwib_er(netwox_smbsercmd_writeandx(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr));
    break;
  case NETWOX_SMBCMD_TYPE_CLOSETREEDISCONNECT_Q :
    netwib_er(netwox_smbsercmd_closetreedisconnect(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr));
    break;
  case NETWOX_SMBCMD_TYPE_TRANSACTION2_Q :
    netwib_er(netwox_smbsercmd_transaction2(psmbser, psmbmsgq, psmbcmdq, &psmbserver->rootdir, psendreply));
    break;
  case NETWOX_SMBCMD_TYPE_FINDCLOSE2_Q :
    netwib_er(netwox_smbsercmd_findclose2(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr));
    break;
  case NETWOX_SMBCMD_TYPE_TREEDISCONNECT_Q :
    netwib_er(netwox_smbsercmd_treedisconnect(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr));
    break;
  case NETWOX_SMBCMD_TYPE_NEGOTIATE_Q :
    netwib_er(netwox_smbsercmd_negotiate(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr, &psmbserver->domain));
    break;
  case NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_Q0 :
    netwib_er(netwox_smbsercmd_sessionsetupandx(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr, &psmbserver->domain, &psmbserver->user, &psmbserver->password));
    break;
  case NETWOX_SMBCMD_TYPE_LOGOFFANDX_Q :
    netwib_er(netwox_smbsercmd_logoffandx(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr));
    break;
  case NETWOX_SMBCMD_TYPE_TREECONNECTANDX_Q :
    netwib_er(netwox_smbsercmd_treeconnectandx(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr, &psmbserver->share));
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATIONDISK_Q :
    netwib_er(netwox_smbsercmd_queryinformationdisk(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr));
    break;
  case NETWOX_SMBCMD_TYPE_SEARCH_Q :
    netwib_er(netwox_smbsercmd_search(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr, &psmbserver->rootdir));
    break;
  case NETWOX_SMBCMD_TYPE_NTTRANSACT_Q :
    netwib_er(netwox_smbsercmd_nttransact(psmbser, psmbmsgq, psmbcmdq, psendreply));
    break;
  case NETWOX_SMBCMD_TYPE_NTCREATEANDX_Q :
    netwib_er(netwox_smbsercmd_ntcreateandx(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr, &psmbserver->rootdir));
    break;
  case NETWOX_SMBCMD_TYPE_NTCANCEL_Q :
    netwib_er(netwox_smbser_nottested(psmbser));
    netwib_er(netwox_smbsercmd_ntcancel(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr));
    break;
  case NETWOX_SMBCMD_TYPE_NTRENAME_Q :
    netwib_er(netwox_smbser_nottested(psmbser));
    netwib_er(netwox_smbsercmd_ntrename(psmbser, psmbmsgq, psmbcmdq, psmbmsgr, psmbcmdr, &psmbserver->rootdir));
    break;
  default :
    netwib_er(netwox_smbser_notimplemented(psmbser));
    psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbserver_session2(netwox_constsmbserver *psmbserver,
                                            netwib_io *psockio,
                                            netwox_smbser *psmbser)
{
  netwib_ip remoteip;
  netwib_port remoteport;
  netwib_bool allowed, sendreply;
  netwox_smbmsg smbmsgq, smbmsgr;
  netwox_smbcmd smbcmdq, smbcmdr;
  netwib_uint32 i;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Start of session\n"));
  }

  /* check allowed clients */
  allowed = NETWIB_TRUE;
  if (psmbser->pcommon->pallowedclients != NULL) {
    netwib_er(netwib_sock_ctl_get_remote(psockio, &remoteip, &remoteport));
    netwib_er(netwib_ips_contains_ip(psmbser->pcommon->pallowedclients,
                                     &remoteip, &allowed));
  }
  if (!allowed) {
    netwib_er(netwib_fmt_display("IP address %{ip} is not allowed\n",
                                 &remoteip));
    return(NETWIB_ERR_OK);
  }

  /* loop for each received message, and analyze every command */
  ret = NETWIB_ERR_OK;
  while(ret == NETWIB_ERR_OK) {
    /* receive a message */
    if (psmbser->pcommon->verbose) {
      netwib_er(netwib_fmt_display("Waiting for next message\n"));
    }
    psmbser->smberrcmd = NETWOX_SMBMSG_CMD_ENDOFLIST;
    psmbser->smberrtosend = NETWOX_SMBERR_OK;
    netwib_er(netwox_smbmsg_init(&smbmsgq));
    netwib_er(netwox_smbmsg_init(&smbmsgr));
    ret = netwox_smbser_smbmsg_recv(psmbser, &smbmsgq);
    if (ret == NETWIB_ERR_OK) {
      sendreply = NETWIB_TRUE;
      /* set header */
      netwib_er(netwox_smbser_smbmsg_setdefault(psmbser, &smbmsgq, &smbmsgr));
      /* decode its commands */
      for (i = 0; i < smbmsgq.numcmds; i++) {
        netwib_er(netwox_smbcmd_init(&smbcmdq));
        netwib_er(netwox_smbcmd_init(&smbcmdr));
        ret = netwox_smbmsg_decode_smbcmd(&smbmsgq, i, &smbcmdq);
        if (ret == NETWIB_ERR_OK) {
          if (psmbser->pcommon->verbose) {
            netwib_er(netwib_fmt_display("A message command is analyzed\n"));
          }
          sendreply = NETWIB_TRUE;
          ret = netwox_priv_smbserver_reply(psmbserver, psmbser,
                                            &smbmsgq, &smbcmdq,
                                            &smbmsgr, &smbcmdr, &sendreply);
          if (sendreply) {
            if (ret == NETWIB_ERR_OK &&
                psmbser->smberrtosend == NETWOX_SMBERR_OK) {
              ret = netwox_smbmsg_append_smbcmd(&smbcmdr, &smbmsgr);
            } else {
              psmbser->smberrcmd = smbmsgq.cmds[i].cmd;
              /* force a premature end of loop */
              i = smbmsgq.numcmds;
            }
          } else {
            i = smbmsgq.numcmds;
          }
        } else {
          netwib_er(netwox_smbser_notimplemented(psmbser));
          if (psmbser->pcommon->verbose) {
            netwib_er(netwib_fmt_display("This command is not implemented\n"));
          }
          psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
          i = smbmsgq.numcmds;
        }
        netwib_er(netwox_smbcmd_close(&smbcmdr));
        netwib_er(netwox_smbcmd_close(&smbcmdq));
      }
      /* on error, prepare an error answer */
      if (sendreply) {
        if (ret != NETWIB_ERR_OK ||
            psmbser->smberrtosend != NETWOX_SMBERR_OK) {
          if (psmbser->smberrtosend == NETWOX_SMBERR_OK) {
            /* should return something like "internal error", but I don't
               know error code, so return a memory problem */
            netwib_er(netwox_smbser_internalerror(psmbser));
            psmbser->smberrtosend = NETWOX_SMBERR_INTERNALERROR;
          }
          netwib_er(netwox_smberr_init(psmbser->smberrtosend,
                           smbmsgr.hdr.flags2&NETWOX_SMBMSG_FLAGS2_NTERROR,
                                       &smbmsgr.hdr.error));
          smbmsgr.cmds[smbmsgr.numcmds].cmd = psmbser->smberrcmd;
          smbmsgr.numcmds++;
        }
        /* send reply */
        ret = netwox_smbser_smbmsg_send(psmbser, &smbmsgr);
      }
    }
    netwib_er(netwox_smbmsg_close(&smbmsgr));
    netwib_er(netwox_smbmsg_close(&smbmsgq));
  }

  if (ret == NETWIB_ERR_DATAEND) {
    ret = NETWIB_ERR_OK;
    if (psmbser->pcommon->verbose) {
      netwib_er(netwib_fmt_display("End of session\n"));
    }
  }
  if (ret != NETWIB_ERR_OK) {
    if (psmbser->pcommon->verbose) {
      netwib_er(netwox_err_display(ret, NETWIB_ERR_ENCODETYPE_FULL));
    }
    netwib_er(netwox_smbser_internalerror(psmbser));
  }
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbserver_session(netwib_io *psockio,
                                    netwib_ptr pinfos)
{
  netwox_constsmbserver *psmbserver = (netwox_constsmbserver *)pinfos;
  netwox_smbser smbser;
  netwib_err ret;

  /* init */
  netwib_er(netwox_smbser_init(psmbserver, psockio, &smbser));

  /* main */
  ret = netwox_smbserver_session2(psmbserver, psockio, &smbser);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_err_display(ret, NETWIB_ERR_ENCODETYPE_TEXT));
  }

  /* close */
  netwib_er(netwox_smbser_close(&smbser));
  return(NETWIB_ERR_OK);
}
