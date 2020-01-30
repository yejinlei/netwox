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
netwib_err netwox_smbcli_init(netwox_arg *parg,
                              netwib_bool isipc,
                              netwox_smbcli *psmbcli)
{
  netwib_buf buf;
  netwib_port serverport;
  netwib_bool isset;

  psmbcli->isipc = isipc;

  netwib_er(netwib_bufpool_initdefault(&psmbcli->pbufpool));

  netwib_er(netwox_sockinfo_init_arg_tcp_cli(parg, &psmbcli->sockinfo));
  netwib_er(netwox_sock_init(&psmbcli->sockinfo, &psmbcli->pio));
  netwib_er(netwox_arg_uint32(parg, 'T', &psmbcli->maxwaitms));
  netwib_er(netwox_arg_bool(parg, 'V', &psmbcli->verbose));
  netwib_er(netwox_arg_bool(parg, 'D', &psmbcli->debug));
  netwib_er(netwox_arg_buf(parg, 'N', &buf));
  netwib_er(netwib_bufpool_buf_init(psmbcli->pbufpool,
                                    &psmbcli->pnetbiosname));
  netwib_er(netwib_buf_append_buf(&buf, psmbcli->pnetbiosname));
  netwib_er(netwib_bufpool_buf_init(psmbcli->pbufpool, &psmbcli->pshare));
  if (psmbcli->isipc) {
    netwib_er(netwib_buf_append_string("IPC$", psmbcli->pshare));
  } else {
    netwib_er(netwox_arg_buf(parg, 's', &buf));
    netwib_er(netwib_buf_append_buf(&buf, psmbcli->pshare));
  }
  netwib_er(netwox_arg_buf(parg, 'u', &buf));
  netwib_er(netwib_bufpool_buf_init(psmbcli->pbufpool, &psmbcli->puser));
  netwib_er(netwib_buf_append_buf(&buf, psmbcli->puser));
  netwib_er(netwox_arg_buf(parg, 'w', &buf));
  netwib_er(netwib_bufpool_buf_init(psmbcli->pbufpool, &psmbcli->ppassword));
  netwib_er(netwib_buf_append_buf(&buf, psmbcli->ppassword));
  netwib_er(netwox_arg_isset(parg, 'v', &isset));
  if (isset) {
    netwib_er(netwox_arg_uint32(parg, 'v', (netwib_uint32*)&psmbcli->authversion));
  } else {
    psmbcli->authversion = NETWOX_SMBCLI_AUTHVERSION_UNSET;
  }

  netwib_er(netwib_sock_ctl_get_remote(psmbcli->pio, NULL, &serverport));
  switch(serverport) {
  case NETWOX_SERVICES_SMBNBTSS :
    psmbcli->isnaked = NETWIB_FALSE;
    break;
  case NETWOX_SERVICES_SMBNAKED :
    psmbcli->isnaked = NETWIB_TRUE;
    break;
  default :
    /* don't know if this is correct */
    psmbcli->isnaked = NETWIB_TRUE;
    break;
  }

  netwib_er(netwib_bufpool_buf_init(psmbcli->pbufpool, &psmbcli->pbufrecv));
  psmbcli->pbufrecv->flags |= NETWIB_BUF_FLAGS_CANSLIDE;

  netwib_er(netwib_uint32_init_rand(1, 0xFFFF, &psmbcli->processid));
  psmbcli->multiplexid = 0;

  psmbcli->negotiate_set = NETWIB_FALSE;
  psmbcli->negotiate_secumode = 0;
  psmbcli->negotiate_maxmsgsize = 1400;
  psmbcli->negotiate_vcid = 0;
  psmbcli->negotiate_capabilities = 0; /* will be negotiated later */
  netwib_er(netwib_bufpool_buf_init(psmbcli->pbufpool,
                                    &psmbcli->pnegotiate_challenge));
  netwib_er(netwib_bufpool_buf_init(psmbcli->pbufpool,
                                    &psmbcli->pnegotiate_domain));

  psmbcli->sessionsetup_set = NETWIB_FALSE;
  psmbcli->sessionsetup_userid = 0;

  psmbcli->treeconnect_set = NETWIB_FALSE;
  psmbcli->treeconnect_treeid = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_close(netwox_smbcli *psmbcli)
{
  netwib_er(netwib_bufpool_buf_close(psmbcli->pbufpool,
                                     &psmbcli->pnetbiosname));
  netwib_er(netwib_bufpool_buf_close(psmbcli->pbufpool, &psmbcli->pshare));
  netwib_er(netwib_bufpool_buf_close(psmbcli->pbufpool, &psmbcli->puser));
  netwib_er(netwib_bufpool_buf_close(psmbcli->pbufpool, &psmbcli->ppassword));
  netwib_er(netwib_bufpool_buf_close(psmbcli->pbufpool,
                                     &psmbcli->pnegotiate_challenge));
  netwib_er(netwib_bufpool_buf_close(psmbcli->pbufpool,
                                     &psmbcli->pnegotiate_domain));
  netwib_er(netwib_bufpool_buf_close(psmbcli->pbufpool, &psmbcli->pbufrecv));
  netwib_er(netwib_bufpool_close(&psmbcli->pbufpool));
  netwib_er(netwib_io_close(&psmbcli->pio));
  netwib_er(netwox_sockinfo_close(&psmbcli->sockinfo));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_sessioninit(netwox_smbcli *psmbcli)
{
  netwib_buf *pbuf, bufencoded, bufname;
  netwox_smbnbtss smbnbtss;
  netwib_time t;
  netwib_uint32 skipsize;
  netwib_bool event;
  netwib_err ret;

  if (psmbcli->isnaked) {
    /* not needed */
    return(NETWIB_ERR_OK);
  }
  if (psmbcli->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbcli_sessioninit\n"));
  }

  ret = NETWIB_ERR_OK;
  netwib_er(netwib_bufpool_buf_init(psmbcli->pbufpool, &pbuf));

  /* send request */
  smbnbtss.smbnbtsstype = NETWOX_SMBNBTSSTYPE_REQUEST;
  smbnbtss.payload.request.callednamelabel = 0x20;
  netwib_eg(netwib_buf_init_ext_arraysizeofempty(smbnbtss.payload.request.calledname, &bufencoded));
  if (netwib__buf_ref_data_size(psmbcli->pnetbiosname)) {
    netwib_byte bufnamearray[16];
    netwib_data data;
    netwib_uint32 datasize, i;
    if (netwib__buf_ref_data_size(psmbcli->pnetbiosname) >
        sizeof(bufnamearray)-1) {
      netwib_eg(NETWIB_ERR_PATOOHIGH);
    }
    data = netwib__buf_ref_data_ptr(psmbcli->pnetbiosname);
    datasize = netwib__buf_ref_data_size(psmbcli->pnetbiosname);
    for (i = 0; i < datasize; i++) {
      bufnamearray[i] = (netwib_byte)netwib_c2_uc(data[i]);
    }
    for (i = datasize; i < sizeof(bufnamearray); i++) {
      bufnamearray[i] = ' ';
    }
    netwib_eg(netwib_buf_init_ext_arraysizeoffilled(bufnamearray, &bufname));
  } else {
    netwib_eg(netwib_buf_init_ext_string("*SMBSERVER      ", &bufname));
  }
  netwib_eg(netwox_buf_decode_netbiosname(&bufname, &bufencoded));
  smbnbtss.payload.request.callingnamelabel = 0x20;
  netwib_eg(netwib_buf_init_ext_arraysizeofempty(smbnbtss.payload.request.callingname, &bufencoded));
  netwib_eg(netwib_buf_init_ext_string("*SMBCLIENT     \0", &bufname));
  bufname.endoffset++; /* because we want the '\0' */
  netwib_eg(netwox_buf_decode_netbiosname(&bufname, &bufencoded));
  netwib_eg(netwox_pkt_append_smbnbtss(&smbnbtss, pbuf));
  netwib_eg(netwib_io_write(psmbcli->pio, pbuf));

  netwib__buf_reinit(pbuf);
  netwib_eg(netwib_time_init_now(&t));
  netwib_eg(netwib_time_plus_msec(&t, psmbcli->maxwaitms));
  while (NETWIB_TRUE) {
    netwib_eg(netwib_io_wait_read(psmbcli->pio, &t, &event));
    if (!event) {
      ret = NETWOX_ERR_TIMEOUT;
      break;
    }
    ret = netwib_io_read(psmbcli->pio, pbuf);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    ret = netwox_pkt_decode_smbnbtss(pbuf, &smbnbtss, &skipsize);
    if (ret == NETWIB_ERR_DATAMISSING) {
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    if (smbnbtss.smbnbtsstype != NETWOX_SMBNBTSSTYPE_RESPPOSITIVE) {
      if (psmbcli->verbose) {
        netwib_eg(netwib_fmt_display("Server refused our session (try setting a NetBIOS name with --netbiosname, or use port 445)\n"));
      }
      ret = NETWOX_ERR_PROTOCOL;
      break;
    }
    break;
  }

 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(psmbcli->pbufpool, &pbuf));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcli_smbmsg_setdefault(netwox_smbcli *psmbcli,
                                                  netwox_smbmsg *psmbmsg)
{

  netwib_er(netwox_smbmsg_setdefault(psmbmsg));

  psmbmsg->hdr.flags |= NETWOX_SMBMSG_FLAGS_CASELESSPATHNAMES;
  psmbmsg->hdr.flags2 |= NETWOX_SMBMSG_FLAGS2_ACCEPTSLONGPATHNAME;
  psmbmsg->hdr.flags2 |= NETWOX_SMBMSG_FLAGS2_USESLONGPATHNAME;
  if (psmbcli->negotiate_capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_NTERROR) {
    psmbmsg->hdr.flags2 |= NETWOX_SMBMSG_FLAGS2_NTERROR;
  }
  if (psmbcli->negotiate_capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_UNICODE) {
    psmbmsg->hdr.flags2 |= NETWOX_SMBMSG_FLAGS2_UNICODESTRINGS;
  }
  psmbmsg->hdr.treeid = psmbcli->treeconnect_treeid;
  psmbmsg->hdr.processid = psmbcli->processid;
  psmbmsg->hdr.userid = psmbcli->sessionsetup_userid;
  psmbmsg->hdr.multiplexid = psmbcli->multiplexid;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcli_smbmsg_send(netwox_smbcli *psmbcli,
                                            netwox_smbmsg *psmbmsg)
{
  netwib_buf *pbuf;
  netwib_err ret;

  /* show message */
  if (psmbcli->debug) {
    netwib_er(netwox_smbmsg_display(psmbcli->pbufpool, psmbmsg));
  }

  /* generate packet */
  netwib_er(netwib_bufpool_buf_init(psmbcli->pbufpool, &pbuf));
  netwib_er(netwox_pkt_append_smbmsghdr_begin(psmbcli->isnaked, pbuf));
  netwib_er(netwox_pkt_append_smbmsg(psmbmsg, pbuf));
  netwib_er(netwox_pkt_append_smbmsghdr_end(psmbcli->isnaked, pbuf));

  /* send it */
  ret = netwib_io_write(psmbcli->pio, pbuf);
  netwib_er(netwib_bufpool_buf_close(psmbcli->pbufpool, &pbuf));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcli_smbmsg_recv(netwox_smbcli *psmbcli,
                                            netwox_smbmsg *psmbmsg)
{
  netwib_time t;
  netwib_bool event, tryread;
  netwib_bufext smbmsgbuf;
  netwib_uint32 skipsize;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_plus_msec(&t, psmbcli->maxwaitms));
  tryread = NETWIB_FALSE;
  while (NETWIB_TRUE) {
    if (tryread) {
      netwib_er(netwib_io_wait_read(psmbcli->pio, &t, &event));
      if (!event) {
        ret = NETWOX_ERR_TIMEOUT;
        break;
      }
      ret = netwib_io_read(psmbcli->pio, psmbcli->pbufrecv);
      if (ret == NETWIB_ERR_DATANOTAVAIL) {
        ret = NETWIB_ERR_OK;
        continue;
      } else if (ret != NETWIB_ERR_OK) {
        break;
      }
    }
    tryread = NETWIB_TRUE;
    ret = netwox_pkt_decode_smbmsghdr(psmbcli->pbufrecv, psmbcli->isnaked,
                                      &smbmsgbuf, &skipsize);
    if (ret == NETWIB_ERR_DATAMISSING) {
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    ret = netwox_pkt_decode_smbmsg(&smbmsgbuf, psmbmsg);
    if (ret == NETWIB_ERR_DATAMISSING) {
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    psmbcli->pbufrecv->beginoffset += skipsize;
    break;
  }

  if (ret == NETWIB_ERR_OK) {
    /* show message */
    if (psmbcli->debug) {
      netwib_er(netwox_smbmsg_display(psmbcli->pbufpool, psmbmsg));
    }
    /* check values */
    if (psmbcli->processid != psmbmsg->hdr.processid) {
      return(NETWOX_ERR_PROTOCOL);
    }
    if (psmbcli->multiplexid != psmbmsg->hdr.multiplexid) {
      return(NETWOX_ERR_PROTOCOL);
    }
    if (psmbcli->sessionsetup_set &&
        (psmbcli->sessionsetup_userid != psmbmsg->hdr.userid)) {
      return(NETWOX_ERR_PROTOCOL);
    }
    if (psmbcli->treeconnect_set &&
        (psmbcli->treeconnect_treeid != psmbmsg->hdr.treeid)) {
      return(NETWOX_ERR_PROTOCOL);
    }
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_negotiate(netwox_smbcli *psmbcli)
{
  netwox_smbmsg smbmsgq, smbmsgr;
  netwox_smbcmd smbcmdq, smbcmdr;
  netwib_err ret;

  if (psmbcli->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbcli_negotiate\n"));
  }

  ret = NETWIB_ERR_OK;
  netwib_er(netwox_smbmsg_init(&smbmsgq));
  netwib_er(netwox_smbmsg_init(&smbmsgr));
  netwib_er(netwox_smbcmd_init(&smbcmdq));
  netwib_er(netwox_smbcmd_init(&smbcmdr));

  /* send request */
  netwib_eg(netwox_smbcli_smbmsg_setdefault(psmbcli, &smbmsgq));
  netwib_eg(netwox_smbcmd_selecttype(&smbcmdq,
                                     NETWOX_SMBCMD_TYPE_NEGOTIATE_Q));
  netwib_eg(netwox_smbcmd_setdefault(&smbcmdq));
  netwib_eg(netwox_smbmsg_append_smbcmd(&smbcmdq, &smbmsgq));
  netwib_eg(netwox_smbcli_smbmsg_send(psmbcli, &smbmsgq));

  /* decode reply */
  netwib_eg(netwox_smbcli_smbmsg_recv(psmbcli, &smbmsgr));
  if (smbmsgr.hdr.error != 0) {
    if (psmbcli->verbose) {
      netwib_eg(netwox_smbmsg_error_display(psmbcli->pbufpool, &smbmsgr));
    }
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbmsgr.numcmds != 1) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbmsgr.cmds[0].cmd != NETWOX_SMBMSG_CMD_NEGOTIATE) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  netwib_eg(netwox_smbmsg_decode_smbcmd(&smbmsgr, 0, &smbcmdr));
  if (smbcmdr.type != NETWOX_SMBCMD_TYPE_NEGOTIATE_R0) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbcmdr.value.negotiate_r0.dialectindex != 0) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  psmbcli->negotiate_secumode = smbcmdr.value.negotiate_r0.secumode;
  psmbcli->negotiate_maxmsgsize = smbcmdr.value.negotiate_r0.maxmsgsize;
  psmbcli->negotiate_vcid = smbcmdr.value.negotiate_r0.vcid;
  /* save capabilitites we support (will send them back in sessionsetup) */
  psmbcli->negotiate_capabilities = 0;
  if (smbcmdr.value.negotiate_r0.capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_UNICODE) {
    psmbcli->negotiate_capabilities |= NETWOX_SMBCMDCMN_NEGOCAPA_UNICODE;
  }
  if (smbcmdr.value.negotiate_r0.capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_LARGEFILES) {
    psmbcli->negotiate_capabilities |= NETWOX_SMBCMDCMN_NEGOCAPA_LARGEFILES;
  }
  if (smbcmdr.value.negotiate_r0.capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_NTERROR) {
    psmbcli->negotiate_capabilities |= NETWOX_SMBCMDCMN_NEGOCAPA_NTERROR;
  }
  if (smbcmdr.value.negotiate_r0.capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_LARGEREADANDX) {
    psmbcli->negotiate_capabilities |= NETWOX_SMBCMDCMN_NEGOCAPA_LARGEREADANDX;
  }
  if (smbcmdr.value.negotiate_r0.capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_LARGEWRITEANDX) {
    psmbcli->negotiate_capabilities |= NETWOX_SMBCMDCMN_NEGOCAPA_LARGEWRITEANDX;
  }
  netwib_eg(netwib_buf_append_buf(&smbcmdr.value.negotiate_r0.challenge,
                                  psmbcli->pnegotiate_challenge));
  /* domain is always stored in unicode (NETWOX_SMBCMDCMN_DATATYPE_STRING
     ONLY_UNINOPAD), even if unicode flag in flags2 is unset. So, do not
     use function netwox_smbcmd_decode_buf(). */
  netwib_eg(netwox_buf_append_ucs2le(&smbcmdr.value.negotiate_r0.domain,
                                     psmbcli->pnegotiate_domain));
  psmbcli->negotiate_set = NETWIB_TRUE;

 netwib_gotolabel:
  netwib_er(netwox_smbcmd_close(&smbcmdr));
  netwib_er(netwox_smbcmd_close(&smbcmdq));
  netwib_er(netwox_smbmsg_close(&smbmsgr));
  netwib_er(netwox_smbmsg_close(&smbmsgq));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_sessionsetup(netwox_smbcli *psmbcli,
                                      netwox_smbcli_authversion authversion)
{
  netwox_smbmsg smbmsgq, smbmsgr;
  netwox_smbcmd smbcmdq, smbcmdr;
  netwib_buf *phashorblob, buf;
  netwib_err ret;

  if (psmbcli->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbcli_sessionsetup: "));
    switch(authversion) {
    case NETWOX_SMBCLI_AUTHVERSION_LM :
      netwib_er(netwib_fmt_display("Lanman"));
      break;
    case NETWOX_SMBCLI_AUTHVERSION_NTLMV1 :
      netwib_er(netwib_fmt_display("NTLMv1"));
      break;
    case NETWOX_SMBCLI_AUTHVERSION_NTLMV2 :
      netwib_er(netwib_fmt_display("NTLMv2"));
      break;
    default :
      return(NETWOX_ERR_BADVALUE);
    }
    netwib_er(netwib_fmt_display(" protocol\n"));
  }
  if (!psmbcli->negotiate_set) {
    netwib_er(netwib_fmt_display("netwox_smbcli_negotiate() must be called before\n"));
    return(NETWOX_ERR_INTERNALERROR);
  }

  ret = NETWIB_ERR_OK;
  netwib_er(netwox_smbmsg_init(&smbmsgq));
  netwib_er(netwox_smbmsg_init(&smbmsgr));
  netwib_er(netwox_smbcmd_init(&smbcmdq));
  netwib_er(netwox_smbcmd_init(&smbcmdr));
  netwib_er(netwib_bufpool_buf_init(psmbcli->pbufpool, &phashorblob));

  /* send request */
  netwib_eg(netwox_smbcli_smbmsg_setdefault(psmbcli, &smbmsgq));
  netwib_eg(netwox_smbcmd_selecttype(&smbcmdq,
                                     NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_Q0));
  netwib_eg(netwox_smbcmd_setdefault(&smbcmdq));
  smbcmdq.value.sessionsetupandx_q0.maxmsgsize = (netwib_uint16)psmbcli->negotiate_maxmsgsize;
  smbcmdq.value.sessionsetupandx_q0.vcid=psmbcli->negotiate_vcid;
  smbcmdq.value.sessionsetupandx_q0.capabilities = psmbcli->negotiate_capabilities;
  if (psmbcli->negotiate_secumode&NETWOX_SMBCMD_NEGOTIATE_SECUMODE_USERLEVEL) {
    if (netwib__buf_ref_data_size(psmbcli->ppassword) == 0) {
      netwib_eg(netwib_buf_append_byte('\0', &smbcmdq.value.sessionsetupandx_q0.password));
      /* put nothing in unicodepassword */
    } else {
      switch(authversion) {
      case NETWOX_SMBCLI_AUTHVERSION_LM :
        netwib_eg(netwox_winauth_lm_hash(psmbcli->ppassword, phashorblob));
        netwib_eg(netwox_winauth_lm_chal_answer(psmbcli->pnegotiate_challenge, phashorblob, &smbcmdq.value.sessionsetupandx_q0.password));
        /* put nothing in unicodepassword */
        break;
      case NETWOX_SMBCLI_AUTHVERSION_NTLMV1 :
        /* NTLM value is stored in both fields */
        netwib_eg(netwox_winauth_ntlmv1_hash(psmbcli->ppassword, phashorblob));
        netwib_eg(netwox_winauth_ntlmv1_chal_answer(psmbcli->pnegotiate_challenge, phashorblob, &smbcmdq.value.sessionsetupandx_q0.password));
        netwib_eg(netwib_buf_append_buf(&smbcmdq.value.sessionsetupandx_q0.password, &smbcmdq.value.sessionsetupandx_q0.unicodepassword));
        break;
      case NETWOX_SMBCLI_AUTHVERSION_NTLMV2 :
        netwib_eg(netwox_winauth_ntlmv2_blob_gene(NULL, psmbcli->pnegotiate_domain, phashorblob));
        netwib_eg(netwox_winauth_ntlmv2_chal_answer(psmbcli->pnegotiate_challenge, psmbcli->puser, psmbcli->ppassword, psmbcli->pnegotiate_domain, phashorblob, &smbcmdq.value.sessionsetupandx_q0.unicodepassword, &smbcmdq.value.sessionsetupandx_q0.password));
        break;
      default :
        netwib_eg(NETWOX_ERR_BADVALUE);
      }
    }
  } else {
    /* password is set in treeconnect, here just put '\0', even if Unicode */
    netwib_eg(netwib_buf_append_byte('\0', &smbcmdq.value.sessionsetupandx_q0.password));
    netwib_eg(netwib_buf_append_byte('\0', &smbcmdq.value.sessionsetupandx_q0.unicodepassword));
  }
  netwib_eg(netwox_smbcmd_append_buf(&smbmsgq, psmbcli->puser, &smbcmdq.value.sessionsetupandx_q0.user));
  netwib_eg(netwox_smbcmd_append_buf(&smbmsgq, psmbcli->pnegotiate_domain, &smbcmdq.value.sessionsetupandx_q0.domain));
  netwib_er(netwib_buf_init_ext_string("Netwox", &buf));
  netwib_eg(netwox_smbcmd_append_buf(&smbmsgq, &buf, &smbcmdq.value.sessionsetupandx_q0.nativeos));
  netwib_eg(netwox_smbcmd_append_buf(&smbmsgq, &buf, &smbcmdq.value.sessionsetupandx_q0.nativelanman));
  netwib_eg(netwox_smbmsg_append_smbcmd(&smbcmdq, &smbmsgq));
  netwib_eg(netwox_smbcli_smbmsg_send(psmbcli, &smbmsgq));

  /* decode reply */
  netwib_eg(netwox_smbcli_smbmsg_recv(psmbcli, &smbmsgr));
  if (smbmsgr.hdr.error != 0) {
    if (psmbcli->verbose) {
      netwib_eg(netwox_smbmsg_error_display(psmbcli->pbufpool, &smbmsgr));
    }
    netwib_eg(NETWOX_ERR_AUTHERROR);
  }
  if (smbmsgr.numcmds != 1) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbmsgr.cmds[0].cmd != NETWOX_SMBMSG_CMD_SESSIONSETUPANDX) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  netwib_eg(netwox_smbmsg_decode_smbcmd(&smbmsgr, 0, &smbcmdr));
  if (smbcmdr.type != NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_R0) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  psmbcli->sessionsetup_userid = smbmsgr.hdr.userid;
  psmbcli->sessionsetup_set = NETWIB_TRUE;

 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(psmbcli->pbufpool, &phashorblob));
  netwib_er(netwox_smbcmd_close(&smbcmdr));
  netwib_er(netwox_smbcmd_close(&smbcmdq));
  netwib_er(netwox_smbmsg_close(&smbmsgr));
  netwib_er(netwox_smbmsg_close(&smbmsgq));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_treeconnect(netwox_smbcli *psmbcli)
{
  netwox_smbmsg smbmsgq, smbmsgr;
  netwox_smbcmd smbcmdq, smbcmdr;
  netwib_buf *ppath, *phash;
  netwib_ip serverip;
  netwib_err ret;

  if (psmbcli->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbcli_treeconnect\n"));
  }
  if (!psmbcli->negotiate_set) {
    netwib_er(netwib_fmt_display("netwox_smbcli_negotiate() must be called before\n"));
    return(NETWOX_ERR_INTERNALERROR);
  }

  ret = NETWIB_ERR_OK;
  netwib_er(netwox_smbmsg_init(&smbmsgq));
  netwib_er(netwox_smbmsg_init(&smbmsgr));
  netwib_er(netwox_smbcmd_init(&smbcmdq));
  netwib_er(netwox_smbcmd_init(&smbcmdr));
  netwib_er(netwib_bufpool_buf_init(psmbcli->pbufpool, &ppath));
  netwib_er(netwib_bufpool_buf_init(psmbcli->pbufpool, &phash));

  /* send request */
  netwib_eg(netwox_smbcli_smbmsg_setdefault(psmbcli, &smbmsgq));
  netwib_eg(netwox_smbcmd_selecttype(&smbcmdq,
                                     NETWOX_SMBCMD_TYPE_TREECONNECTANDX_Q));
  netwib_eg(netwox_smbcmd_setdefault(&smbcmdq));
  if (psmbcli->negotiate_secumode&NETWOX_SMBCMD_NEGOTIATE_SECUMODE_USERLEVEL ||
      netwib__buf_ref_data_size(psmbcli->ppassword) == 0) {
    netwib_eg(netwib_buf_append_byte('\0', &smbcmdq.value.treeconnectandx_q.password));
  } else {
    if (psmbcli->authversion != NETWOX_SMBCLI_AUTHVERSION_LM &&
        psmbcli->authversion != NETWOX_SMBCLI_AUTHVERSION_UNSET) {
      if (psmbcli->verbose) {
        netwib_er(netwib_fmt_display("For Share Level shares, only Lanman is supported (set -v 0)\n"));
      }
      netwib_eg(NETWOX_ERR_PROTOCOL);
    }
    netwib_eg(netwox_winauth_lm_hash(psmbcli->ppassword, phash));
    netwib_eg(netwox_winauth_lm_chal_answer(psmbcli->pnegotiate_challenge, phash, &smbcmdq.value.treeconnectandx_q.password));
  }
  netwib_er(netwib_sock_ctl_get_remote(psmbcli->pio, &serverip, NULL));
  if (netwib__buf_ref_data_size(psmbcli->pnetbiosname)) {
    netwib_eg(netwib_buf_append_fmt(ppath, "\\\\%{buf}\\",
                                    psmbcli->pnetbiosname));
  } else {
    netwib_eg(netwib_buf_append_fmt(ppath, "\\\\%{ip}\\", &serverip));
  }
  netwib_eg(netwib_buf_append_buf(psmbcli->pshare, ppath));
  netwib_eg(netwox_smbcmd_append_buf_uc(&smbmsgq, ppath, &smbcmdq.value.treeconnectandx_q.path));
  if (psmbcli->isipc) {
    netwib_eg(netwib_buf_append_string(NETWOX_SMBCMD_TREECONNECTANDX_SERVICE_IPC, &smbcmdq.value.treeconnectandx_q.service));
  } else {
    netwib_eg(netwib_buf_append_string(NETWOX_SMBCMD_TREECONNECTANDX_SERVICE_SHARE, &smbcmdq.value.treeconnectandx_q.service));
  }
  netwib_eg(netwox_smbmsg_append_smbcmd(&smbcmdq, &smbmsgq));
  netwib_eg(netwox_smbcli_smbmsg_send(psmbcli, &smbmsgq));

  /* decode reply */
  netwib_eg(netwox_smbcli_smbmsg_recv(psmbcli, &smbmsgr));
  if (smbmsgr.hdr.error != 0) {
    if (psmbcli->verbose) {
      netwib_eg(netwox_smbmsg_error_display(psmbcli->pbufpool, &smbmsgr));
    }
    netwib_eg(NETWOX_ERR_AUTHERROR);
  }
  if (smbmsgr.numcmds != 1) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbmsgr.cmds[0].cmd != NETWOX_SMBMSG_CMD_TREECONNECTANDX) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  netwib_eg(netwox_smbmsg_decode_smbcmd(&smbmsgr, 0, &smbcmdr));
  if (smbcmdr.type != NETWOX_SMBCMD_TYPE_TREECONNECTANDX_R) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  psmbcli->treeconnect_treeid = smbmsgr.hdr.treeid;
  psmbcli->treeconnect_set = NETWIB_TRUE;

 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(psmbcli->pbufpool, &phash));
  netwib_er(netwib_bufpool_buf_close(psmbcli->pbufpool, &ppath));
  netwib_er(netwox_smbcmd_close(&smbcmdr));
  netwib_er(netwox_smbcmd_close(&smbcmdq));
  netwib_er(netwox_smbmsg_close(&smbmsgr));
  netwib_er(netwox_smbmsg_close(&smbmsgq));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_begin(netwox_smbcli *psmbcli)
{
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwox_smbcli_sessioninit(psmbcli));

  netwib_er(netwox_smbcli_negotiate(psmbcli));

  switch(psmbcli->authversion) {
  case NETWOX_SMBCLI_AUTHVERSION_LM :
    netwib_er(netwox_smbcli_sessionsetup(psmbcli,
                                         NETWOX_SMBCLI_AUTHVERSION_LM));
    break;
  case NETWOX_SMBCLI_AUTHVERSION_NTLMV1 :
    netwib_er(netwox_smbcli_sessionsetup(psmbcli,
                                         NETWOX_SMBCLI_AUTHVERSION_NTLMV1));
    break;
  case NETWOX_SMBCLI_AUTHVERSION_NTLMV2 :
    netwib_er(netwox_smbcli_sessionsetup(psmbcli,
                                         NETWOX_SMBCLI_AUTHVERSION_NTLMV2));
    break;
  case NETWOX_SMBCLI_AUTHVERSION_UNSET :
    ret = netwox_smbcli_sessionsetup(psmbcli,
                                     NETWOX_SMBCLI_AUTHVERSION_NTLMV2);
    if (ret != NETWIB_ERR_OK) {
      ret = netwox_smbcli_sessionsetup(psmbcli,
                                       NETWOX_SMBCLI_AUTHVERSION_NTLMV1);
    }
    if (ret != NETWIB_ERR_OK) {
      ret = netwox_smbcli_sessionsetup(psmbcli,
                                       NETWOX_SMBCLI_AUTHVERSION_LM);
    }
    break;
  default :
    return(NETWOX_ERR_BADVALUE);
    break;
  }

  netwib_er(netwox_smbcli_treeconnect(psmbcli));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_treedisconnect(netwox_smbcli *psmbcli)
{
  netwox_smbmsg smbmsgq, smbmsgr;
  netwox_smbcmd smbcmdq, smbcmdr;
  netwib_err ret;

  if (psmbcli->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbcli_treedisconnect\n"));
  }

  if (!psmbcli->treeconnect_set) {
    return(NETWIB_ERR_OK);
  }

  ret = NETWIB_ERR_OK;
  netwib_er(netwox_smbmsg_init(&smbmsgq));
  netwib_er(netwox_smbmsg_init(&smbmsgr));
  netwib_er(netwox_smbcmd_init(&smbcmdq));
  netwib_er(netwox_smbcmd_init(&smbcmdr));

  /* send request */
  netwib_eg(netwox_smbcli_smbmsg_setdefault(psmbcli, &smbmsgq));
  netwib_eg(netwox_smbcmd_selecttype(&smbcmdq,
                                     NETWOX_SMBCMD_TYPE_TREEDISCONNECT_Q));
  netwib_eg(netwox_smbcmd_setdefault(&smbcmdq));
  netwib_eg(netwox_smbmsg_append_smbcmd(&smbcmdq, &smbmsgq));
  netwib_eg(netwox_smbcli_smbmsg_send(psmbcli, &smbmsgq));

  /* decode reply */
  netwib_eg(netwox_smbcli_smbmsg_recv(psmbcli, &smbmsgr));
  if (smbmsgr.hdr.error != 0) {
    if (psmbcli->verbose) {
      netwib_eg(netwox_smbmsg_error_display(psmbcli->pbufpool, &smbmsgr));
    }
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbmsgr.numcmds != 1) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbmsgr.cmds[0].cmd != NETWOX_SMBMSG_CMD_TREEDISCONNECT) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  netwib_eg(netwox_smbmsg_decode_smbcmd(&smbmsgr, 0, &smbcmdr));
  if (smbcmdr.type != NETWOX_SMBCMD_TYPE_TREEDISCONNECT_R) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  psmbcli->treeconnect_set = NETWIB_FALSE;
  psmbcli->treeconnect_treeid = 0;

 netwib_gotolabel:
  netwib_er(netwox_smbcmd_close(&smbcmdr));
  netwib_er(netwox_smbcmd_close(&smbcmdq));
  netwib_er(netwox_smbmsg_close(&smbmsgr));
  netwib_er(netwox_smbmsg_close(&smbmsgq));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_logoff(netwox_smbcli *psmbcli)
{
  netwox_smbmsg smbmsgq, smbmsgr;
  netwox_smbcmd smbcmdq, smbcmdr;
  netwib_err ret;

  if (psmbcli->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbcli_logoff\n"));
  }

  if (!psmbcli->sessionsetup_set) {
    return(NETWIB_ERR_OK);
  }

  ret = NETWIB_ERR_OK;
  netwib_er(netwox_smbmsg_init(&smbmsgq));
  netwib_er(netwox_smbmsg_init(&smbmsgr));
  netwib_er(netwox_smbcmd_init(&smbcmdq));
  netwib_er(netwox_smbcmd_init(&smbcmdr));

  /* send request */
  netwib_eg(netwox_smbcli_smbmsg_setdefault(psmbcli, &smbmsgq));
  netwib_eg(netwox_smbcmd_selecttype(&smbcmdq,
                                     NETWOX_SMBCMD_TYPE_LOGOFFANDX_Q));
  netwib_eg(netwox_smbcmd_setdefault(&smbcmdq));
  netwib_eg(netwox_smbmsg_append_smbcmd(&smbcmdq, &smbmsgq));
  netwib_eg(netwox_smbcli_smbmsg_send(psmbcli, &smbmsgq));

  /* decode reply */
  netwib_eg(netwox_smbcli_smbmsg_recv(psmbcli, &smbmsgr));
  if (smbmsgr.hdr.error != 0) {
    if (psmbcli->verbose) {
      netwib_eg(netwox_smbmsg_error_display(psmbcli->pbufpool, &smbmsgr));
    }
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbmsgr.numcmds != 1) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbmsgr.cmds[0].cmd != NETWOX_SMBMSG_CMD_LOGOFFANDX) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  netwib_eg(netwox_smbmsg_decode_smbcmd(&smbmsgr, 0, &smbcmdr));
  if (smbcmdr.type != NETWOX_SMBCMD_TYPE_LOGOFFANDX_R) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  psmbcli->sessionsetup_set = NETWIB_FALSE;
  psmbcli->sessionsetup_userid = 0;

 netwib_gotolabel:
  netwib_er(netwox_smbcmd_close(&smbcmdr));
  netwib_er(netwox_smbcmd_close(&smbcmdq));
  netwib_er(netwox_smbmsg_close(&smbmsgr));
  netwib_er(netwox_smbmsg_close(&smbmsgq));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_end(netwox_smbcli *psmbcli)
{
  netwib_er(netwox_smbcli_treedisconnect(psmbcli));
  netwib_er(netwox_smbcli_logoff(psmbcli));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smb_sharelist_init(netwox_smb_sharelist *psharelist)
{
  netwib_er(netwib_buf_init_mallocdefault(&psharelist->name));
  netwib_er(netwib_buf_init_mallocdefault(&psharelist->comment));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smb_sharelist_close(netwox_smb_sharelist *psharelist)
{
  netwib_er(netwib_buf_close(&psharelist->comment));
  netwib_er(netwib_buf_close(&psharelist->name));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smb_sharelist_erase(netwib_ptr pitem)
{
  netwib_er(netwox_smb_sharelist_close((netwox_smb_sharelist *)pitem));
  netwib_er(netwib_ptr_free(&pitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smb_sharelist_duplicate(netwib_constptr pitem,
                                        netwib_ptr *pdupofitem)
{
  const netwox_smb_sharelist *psharelist = (const netwox_smb_sharelist *)pitem;
  netwox_smb_sharelist *psharelistdup;
  netwib_ptr ptr;

  netwib_er(netwib_ptr_malloc(sizeof(netwox_smb_sharelist), &ptr));
  psharelistdup = (netwox_smb_sharelist *)ptr;
  *pdupofitem = ptr;

  netwib_er(netwox_smb_sharelist_init(psharelistdup));
  netwib_er(netwib_buf_append_buf(&psharelist->name, &psharelistdup->name));
  psharelistdup->sharetype = psharelist->sharetype;
  netwib_er(netwib_buf_append_buf(&psharelist->comment, &psharelistdup->comment));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcli_shares_addring(const netwox_smbcmdtscr_netshareenum_share *pitem,
                                               netwib_ring *pring)
{
  netwox_smb_sharelist *psharelist;
  netwib_ptr ptr;

  /* initialize the item */
  netwib_er(netwib_ptr_malloc(sizeof(netwox_smb_sharelist), &ptr));
  psharelist = (netwox_smb_sharelist *)ptr;
  netwib_er(netwox_smb_sharelist_init(psharelist));

  netwib_er(netwib_buf_append_buf(&pitem->name, &psharelist->name));
  psharelist->sharetype = pitem->sharetype;
  netwib_er(netwib_buf_append_buf(&pitem->comment, &psharelist->comment));

  /* add in ring */
  netwib_er(netwib_ring_add_last(pring, ptr));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_shares(netwox_smbcli *psmbcli,
                                netwib_ring *pring)
{
  netwox_smbmsg smbmsgq, smbmsgr;
  netwox_smbcmd smbcmdq, smbcmdr;
  netwox_smbcmdtscq smbcmdtscq;
  netwox_smbcmdtscrstorage storager;
  netwox_smbcmdtscr smbcmdtscr;
  netwox_smbcmdtscrfmt smbcmdtscrfmt;
  netwox_smbcmdtscr_netshareenum_share *pitem;
  netwib_bool needmoremsg;
  netwib_uint32 i;
  netwib_err ret;

  if (psmbcli->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbcli_shares\n"));
  }

  ret = NETWIB_ERR_OK;
  netwib_er(netwox_smbmsg_init(&smbmsgq));
  netwib_er(netwox_smbmsg_init(&smbmsgr));
  netwib_er(netwox_smbcmd_init(&smbcmdq));
  netwib_er(netwox_smbcmd_init(&smbcmdr));
  netwib_er(netwox_smbcmdtscq_init(&smbcmdtscq));
  netwib_er(netwox_smbcmdtscrstorage_init(psmbcli->negotiate_capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_UNICODE, &storager));
  netwib_er(netwox_smbcmdtscr_init(&smbcmdtscr));

  /* send request */
  /*  init msg to set unicode flag */
  netwib_eg(netwox_smbcli_smbmsg_setdefault(psmbcli, &smbmsgq));
  /*  construct transaction data */
  netwib_eg(netwox_smbcmdtscq_selecttype(&smbcmdtscq, NETWOX_SMBCMDTSCQ_TYPE_NETSHAREENUM));
  netwib_eg(netwox_smbcmdtscq_setdefault(&smbcmdtscq));
  /*  construct command */
  netwib_eg(netwox_smbcmd_selecttype(&smbcmdq,
                                     NETWOX_SMBCMD_TYPE_TRANSACTION_Q));
  netwib_eg(netwox_smbcmd_setdefault(&smbcmdq));
  smbcmdq.value.transaction_q.isunicode = psmbcli->negotiate_capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_UNICODE;
  smbcmdq.value.transaction_q.timeout = psmbcli->maxwaitms;
  if (psmbcli->debug) {
    netwib_eg(netwib_fmt_display("Transaction query:\n"));
    netwib_eg(netwox_smbcmdtscq_display(psmbcli->pbufpool, &smbcmdtscq));
  }
  netwib_eg(netwox_smbcmdtscq_decode_smbcmd(&smbcmdtscq, &smbcmdq));
  /*  send command */
  netwib_eg(netwox_smbmsg_append_smbcmd(&smbcmdq, &smbmsgq));
  netwib_eg(netwox_smbcli_smbmsg_send(psmbcli, &smbmsgq));

  /* decode reply/replies */
  while (NETWIB_TRUE) {
    netwib_eg(netwox_smbcli_smbmsg_recv(psmbcli, &smbmsgr));
    if (smbmsgr.hdr.error != 0) {
      if (psmbcli->verbose) {
        netwib_eg(netwox_smbmsg_error_display(psmbcli->pbufpool, &smbmsgr));
      }
      netwib_eg(NETWOX_ERR_PROTOCOL);
    }
    if (smbmsgr.numcmds != 1) {
      netwib_eg(NETWOX_ERR_PROTOCOL);
    }
    if (smbmsgr.cmds[0].cmd != NETWOX_SMBMSG_CMD_TRANSACTION) {
      netwib_eg(NETWOX_ERR_PROTOCOL);
    }
    netwib_eg(netwox_smbmsg_decode_smbcmd(&smbmsgr, 0, &smbcmdr));
    if (smbcmdr.type != NETWOX_SMBCMD_TYPE_TRANSACTION_R) {
      netwib_eg(NETWOX_ERR_PROTOCOL);
    }
    netwib_eg(netwox_smbcmdtscrstorage_append_smbcmd(&smbcmdr, &storager,
                                                     &needmoremsg));
    if (!needmoremsg) {
      break;
    }
    netwib_er(netwox_smbcmd_close(&smbcmdr));
    netwib_er(netwox_smbmsg_close(&smbmsgr));
    netwib_er(netwox_smbmsg_init(&smbmsgr));
    netwib_er(netwox_smbcmd_init(&smbcmdr));
  }
  netwib_eg(netwox_smbcmdtscrfmt_init_smbcmdtscq(&smbcmdtscq, &smbcmdtscrfmt));
  netwib_eg(netwox_smbcmdtscr_encode_storager(&storager, &smbcmdtscrfmt,
                                              &smbcmdtscr));
  if (psmbcli->debug) {
    netwib_eg(netwib_fmt_display("Transaction reply:\n"));
    netwib_eg(netwox_smbcmdtscr_display(psmbcli->pbufpool, &smbcmdtscr));
  }
  if (smbcmdtscr.type != NETWOX_SMBCMDTSCR_TYPE_NETSHAREENUM) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbcmdtscr.value.netshareenum.status != 0) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  for (i = 0; i < smbcmdtscr.value.netshareenum.shares.size; i++) {
    pitem = (netwox_smbcmdtscr_netshareenum_share*)smbcmdtscr.value.netshareenum.shares.p[i];
    netwib_eg(netwox_smbcli_shares_addring(pitem, pring));
  }

 netwib_gotolabel:
  netwib_er(netwox_smbcmdtscr_close(&smbcmdtscr));
  netwib_er(netwox_smbcmdtscrstorage_close(&storager));
  netwib_er(netwox_smbcmdtscq_close(&smbcmdtscq));
  netwib_er(netwox_smbcmd_close(&smbcmdr));
  netwib_er(netwox_smbcmd_close(&smbcmdq));
  netwib_er(netwox_smbmsg_close(&smbmsgr));
  netwib_er(netwox_smbmsg_close(&smbmsgq));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_echo(netwox_smbcli *psmbcli)
{
  netwox_smbmsg smbmsgq, smbmsgr;
  netwox_smbcmd smbcmdq, smbcmdr;
  netwib_err ret;

  if (psmbcli->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbcli_echo\n"));
  }

  ret = NETWIB_ERR_OK;
  netwib_er(netwox_smbmsg_init(&smbmsgq));
  netwib_er(netwox_smbmsg_init(&smbmsgr));
  netwib_er(netwox_smbcmd_init(&smbcmdq));
  netwib_er(netwox_smbcmd_init(&smbcmdr));

  /* send request */
  netwib_eg(netwox_smbcli_smbmsg_setdefault(psmbcli, &smbmsgq));
  netwib_eg(netwox_smbcmd_selecttype(&smbcmdq, NETWOX_SMBCMD_TYPE_ECHO_Q));
  netwib_eg(netwox_smbcmd_setdefault(&smbcmdq));
  netwib_eg(netwib_buf_append_string("Hello", &smbcmdq.value.echo_q.data));
  netwib_eg(netwox_smbmsg_append_smbcmd(&smbcmdq, &smbmsgq));
  netwib_eg(netwox_smbcli_smbmsg_send(psmbcli, &smbmsgq));

  /* decode reply */
  netwib_eg(netwox_smbcli_smbmsg_recv(psmbcli, &smbmsgr));
  if (smbmsgr.hdr.error != 0) {
    if (psmbcli->verbose) {
      netwib_eg(netwox_smbmsg_error_display(psmbcli->pbufpool, &smbmsgr));
    }
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbmsgr.numcmds != 1) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbmsgr.cmds[0].cmd != NETWOX_SMBMSG_CMD_ECHO) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  netwib_eg(netwox_smbmsg_decode_smbcmd(&smbmsgr, 0, &smbcmdr));
  if (smbcmdr.type != NETWOX_SMBCMD_TYPE_ECHO_R) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbcmdr.value.echo_r.number != 1) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }

 netwib_gotolabel:
  netwib_er(netwox_smbcmd_close(&smbcmdr));
  netwib_er(netwox_smbcmd_close(&smbcmdq));
  netwib_er(netwox_smbmsg_close(&smbmsgr));
  netwib_er(netwox_smbmsg_close(&smbmsgq));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcli_stat_32(netwox_smbcli *psmbcli,
                                        netwib_constbuf *ppathname,
                                        netwib_pathstat *ppathstat)
{
  netwox_smbmsg smbmsgq, smbmsgr;
  netwox_smbcmd smbcmdq, smbcmdr;
  netwib_err ret;

  if (psmbcli->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbcli_stat_32 of %{buf}\n",
                                 ppathname));
  }

  ret = NETWIB_ERR_OK;
  netwib_er(netwox_smbmsg_init(&smbmsgq));
  netwib_er(netwox_smbmsg_init(&smbmsgr));
  netwib_er(netwox_smbcmd_init(&smbcmdq));
  netwib_er(netwox_smbcmd_init(&smbcmdr));

  /* send request */
  netwib_eg(netwox_smbcli_smbmsg_setdefault(psmbcli, &smbmsgq));
  netwib_eg(netwox_smbcmd_selecttype(&smbcmdq, NETWOX_SMBCMD_TYPE_QUERYINFORMATION_Q));
  netwib_eg(netwox_smbcmd_setdefault(&smbcmdq));
  netwib_eg(netwox_smbcmd_append_buf_path(&smbmsgq, ppathname, &smbcmdq.value.queryinformation_q.filename));
  netwib_eg(netwox_smbmsg_append_smbcmd(&smbcmdq, &smbmsgq));
  netwib_eg(netwox_smbcli_smbmsg_send(psmbcli, &smbmsgq));

  /* decode reply */
  netwib_eg(netwox_smbcli_smbmsg_recv(psmbcli, &smbmsgr));
  if (smbmsgr.hdr.error != 0) {
    if (psmbcli->verbose) {
      netwib_eg(netwox_smbmsg_error_display(psmbcli->pbufpool, &smbmsgr));
    }
    netwib_eg(NETWOX_ERR_BADVALUE);
  }
  if (smbmsgr.numcmds != 1) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbmsgr.cmds[0].cmd != NETWOX_SMBMSG_CMD_QUERYINFORMATION) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  netwib_eg(netwox_smbmsg_decode_smbcmd(&smbmsgr, 0, &smbcmdr));
  if (smbcmdr.type != NETWOX_SMBCMD_TYPE_QUERYINFORMATION_R) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbcmdr.value.queryinformation_r.fileattributes & NETWOX_SMBCMDCMN_FILEATTR16_DIR) {
    ppathstat->type = NETWIB_PATHSTAT_TYPE_DIR;
  } else {
    ppathstat->type = NETWIB_PATHSTAT_TYPE_REG;
  }
  ppathstat->size = smbcmdr.value.queryinformation_r.filesize;
  netwib__uint64_init_uint32(ppathstat->size, ppathstat->size64);
  netwib_eg(netwox_time_init_time1970l(smbcmdr.value.queryinformation_r.lastwritetime, &ppathstat->mtime));

 netwib_gotolabel:
  netwib_er(netwox_smbcmd_close(&smbcmdr));
  netwib_er(netwox_smbcmd_close(&smbcmdq));
  netwib_er(netwox_smbmsg_close(&smbmsgr));
  netwib_er(netwox_smbmsg_close(&smbmsgq));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcli_stat_64(netwox_smbcli *psmbcli,
                                        netwib_constbuf *ppathname,
                                        netwib_uint64 *psize64)
{
  netwox_smbmsg smbmsgq, smbmsgr;
  netwox_smbcmd smbcmdq, smbcmdr;
  netwox_smbcmdtsc2q smbcmdtsc2q;
  netwox_smbcmdtsc2rstorage storager;
  netwox_smbcmdtsc2r smbcmdtsc2r;
  netwox_smbcmdtsc2rfmt smbcmdtsc2rfmt;
  netwib_bool needmoremsg;
  netwib_err ret;

  if (psmbcli->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbcli_stat_64 of %{buf}\n",
                                 ppathname));
  }

  ret = NETWIB_ERR_OK;
  netwib_er(netwox_smbmsg_init(&smbmsgq));
  netwib_er(netwox_smbmsg_init(&smbmsgr));
  netwib_er(netwox_smbcmd_init(&smbcmdq));
  netwib_er(netwox_smbcmd_init(&smbcmdr));
  netwib_er(netwox_smbcmdtsc2q_init(&smbcmdtsc2q));
  netwib_er(netwox_smbcmdtsc2rstorage_init(psmbcli->negotiate_capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_UNICODE, &storager));
  netwib_er(netwox_smbcmdtsc2r_init(&smbcmdtsc2r));

  /* send request */
  /*  init msg to set unicode flag */
  netwib_eg(netwox_smbcli_smbmsg_setdefault(psmbcli, &smbmsgq));
  /*  construct transaction data */
  netwib_eg(netwox_smbcmdtsc2q_selecttype(&smbcmdtsc2q, NETWOX_SMBCMDTSC2Q_TYPE_QUERYPATHINFO));
  netwib_eg(netwox_smbcmdtsc2q_setdefault(&smbcmdtsc2q));
  smbcmdtsc2q.value.querypathinfo.fmt = NETWOX_SMBCMDTSC2_FMT_FILEQ_STANDARD;
  netwib_eg(netwox_smbcmd_append_buf_path(&smbmsgq, ppathname, &smbcmdtsc2q.value.querypathinfo.filename));
  /*  construct command */
  netwib_eg(netwox_smbcmd_selecttype(&smbcmdq,
                                     NETWOX_SMBCMD_TYPE_TRANSACTION2_Q));
  netwib_eg(netwox_smbcmd_setdefault(&smbcmdq));
  smbcmdq.value.transaction2_q.isunicode = psmbcli->negotiate_capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_UNICODE;
  smbcmdq.value.transaction2_q.timeout = psmbcli->maxwaitms;
  if (psmbcli->debug) {
    netwib_eg(netwib_fmt_display("Transaction2 query:\n"));
    netwib_eg(netwox_smbcmdtsc2q_display(psmbcli->pbufpool, &smbcmdtsc2q));
  }
  netwib_eg(netwox_smbcmdtsc2q_decode_smbcmd(&smbcmdtsc2q, &smbcmdq));
  /*  send command */
  netwib_eg(netwox_smbmsg_append_smbcmd(&smbcmdq, &smbmsgq));
  netwib_eg(netwox_smbcli_smbmsg_send(psmbcli, &smbmsgq));

  /* decode reply/replies */
  while (NETWIB_TRUE) {
    netwib_eg(netwox_smbcli_smbmsg_recv(psmbcli, &smbmsgr));
    if (smbmsgr.hdr.error != 0) {
      if (psmbcli->verbose) {
        netwib_eg(netwox_smbmsg_error_display(psmbcli->pbufpool, &smbmsgr));
      }
      netwib_eg(NETWOX_ERR_BADVALUE);
    }
    if (smbmsgr.numcmds != 1) {
      netwib_eg(NETWOX_ERR_PROTOCOL);
    }
    if (smbmsgr.cmds[0].cmd != NETWOX_SMBMSG_CMD_TRANSACTION2) {
      netwib_eg(NETWOX_ERR_PROTOCOL);
    }
    netwib_eg(netwox_smbmsg_decode_smbcmd(&smbmsgr, 0, &smbcmdr));
    if (smbcmdr.type != NETWOX_SMBCMD_TYPE_TRANSACTION2_R) {
      netwib_eg(NETWOX_ERR_PROTOCOL);
    }
    netwib_eg(netwox_smbcmdtsc2rstorage_append_smbcmd(&smbcmdr, &storager,
                                                      &needmoremsg));
    if (!needmoremsg) {
      break;
    }
    netwib_er(netwox_smbcmd_close(&smbcmdr));
    netwib_er(netwox_smbmsg_close(&smbmsgr));
    netwib_er(netwox_smbmsg_init(&smbmsgr));
    netwib_er(netwox_smbcmd_init(&smbcmdr));
  }
  netwib_eg(netwox_smbcmdtsc2rfmt_init_smbcmdtsc2q(&smbcmdtsc2q,
                                                   &smbcmdtsc2rfmt));
  netwib_eg(netwox_smbcmdtsc2r_encode_storager(&storager, &smbcmdtsc2rfmt,
                                               &smbcmdtsc2r));
  if (psmbcli->debug) {
    netwib_eg(netwib_fmt_display("Transaction2 reply:\n"));
    netwib_eg(netwox_smbcmdtsc2r_display(psmbcli->pbufpool, &smbcmdtsc2r));
  }
  if (smbcmdtsc2r.type != NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_STANDARD) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  *psize64 = smbcmdtsc2r.value.querypathfileinfo_standard.filesize;

 netwib_gotolabel:
  netwib_er(netwox_smbcmdtsc2r_close(&smbcmdtsc2r));
  netwib_er(netwox_smbcmdtsc2rstorage_close(&storager));
  netwib_er(netwox_smbcmdtsc2q_close(&smbcmdtsc2q));
  netwib_er(netwox_smbcmd_close(&smbcmdr));
  netwib_er(netwox_smbcmd_close(&smbcmdq));
  netwib_er(netwox_smbmsg_close(&smbmsgr));
  netwib_er(netwox_smbmsg_close(&smbmsgq));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_stat(netwox_smbcli *psmbcli,
                              netwib_constbuf *ppathname,
                              netwib_pathstat *ppathstat)
{

  netwib_er(netwox_smbcli_stat_32(psmbcli, ppathname, ppathstat));

  if (psmbcli->negotiate_capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_LARGEFILES) {
    netwib_er(netwox_smbcli_stat_64(psmbcli, ppathname, &ppathstat->size64));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_dir_create(netwox_smbcli *psmbcli,
                                    netwib_constbuf *pdirname)
{
  netwox_smbmsg smbmsgq, smbmsgr;
  netwox_smbcmd smbcmdq, smbcmdr;
  netwib_err ret;

  if (psmbcli->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbcli_dir_create of %{buf}\n", pdirname));
  }

  ret = NETWIB_ERR_OK;
  netwib_er(netwox_smbmsg_init(&smbmsgq));
  netwib_er(netwox_smbmsg_init(&smbmsgr));
  netwib_er(netwox_smbcmd_init(&smbcmdq));
  netwib_er(netwox_smbcmd_init(&smbcmdr));

  /* send request */
  netwib_eg(netwox_smbcli_smbmsg_setdefault(psmbcli, &smbmsgq));
  netwib_eg(netwox_smbcmd_selecttype(&smbcmdq, NETWOX_SMBCMD_TYPE_CREATEDIRECTORY_Q));
  netwib_eg(netwox_smbcmd_setdefault(&smbcmdq));
  netwib_eg(netwox_smbcmd_append_buf_path(&smbmsgq, pdirname, &smbcmdq.value.createdirectory_q.dirname));
  netwib_eg(netwox_smbmsg_append_smbcmd(&smbcmdq, &smbmsgq));
  netwib_eg(netwox_smbcli_smbmsg_send(psmbcli, &smbmsgq));

  /* decode reply */
  netwib_eg(netwox_smbcli_smbmsg_recv(psmbcli, &smbmsgr));
  if (smbmsgr.hdr.error != 0) {
    if (psmbcli->verbose) {
      netwib_eg(netwox_smbmsg_error_display(psmbcli->pbufpool, &smbmsgr));
    }
    netwib_eg(NETWOX_ERR_BADVALUE);
  }
  if (smbmsgr.numcmds != 1) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbmsgr.cmds[0].cmd != NETWOX_SMBMSG_CMD_CREATEDIRECTORY) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  netwib_eg(netwox_smbmsg_decode_smbcmd(&smbmsgr, 0, &smbcmdr));
  if (smbcmdr.type != NETWOX_SMBCMD_TYPE_CREATEDIRECTORY_R) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }

 netwib_gotolabel:
  netwib_er(netwox_smbcmd_close(&smbcmdr));
  netwib_er(netwox_smbcmd_close(&smbcmdq));
  netwib_er(netwox_smbmsg_close(&smbmsgr));
  netwib_er(netwox_smbmsg_close(&smbmsgq));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_dir_delete(netwox_smbcli *psmbcli,
                                    netwib_constbuf *pdirname)
{
  netwox_smbmsg smbmsgq, smbmsgr;
  netwox_smbcmd smbcmdq, smbcmdr;
  netwib_err ret;

  if (psmbcli->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbcli_dir_delete of %{buf}\n", pdirname));
  }

  ret = NETWIB_ERR_OK;
  netwib_er(netwox_smbmsg_init(&smbmsgq));
  netwib_er(netwox_smbmsg_init(&smbmsgr));
  netwib_er(netwox_smbcmd_init(&smbcmdq));
  netwib_er(netwox_smbcmd_init(&smbcmdr));

  /* send request */
  netwib_eg(netwox_smbcli_smbmsg_setdefault(psmbcli, &smbmsgq));
  netwib_eg(netwox_smbcmd_selecttype(&smbcmdq, NETWOX_SMBCMD_TYPE_DELETEDIRECTORY_Q));
  netwib_eg(netwox_smbcmd_setdefault(&smbcmdq));
  netwib_eg(netwox_smbcmd_append_buf_path(&smbmsgq, pdirname, &smbcmdq.value.deletedirectory_q.dirname));
  netwib_eg(netwox_smbmsg_append_smbcmd(&smbcmdq, &smbmsgq));
  netwib_eg(netwox_smbcli_smbmsg_send(psmbcli, &smbmsgq));

  /* decode reply */
  netwib_eg(netwox_smbcli_smbmsg_recv(psmbcli, &smbmsgr));
  if (smbmsgr.hdr.error != 0) {
    if (psmbcli->verbose) {
      netwib_eg(netwox_smbmsg_error_display(psmbcli->pbufpool, &smbmsgr));
    }
    netwib_eg(NETWOX_ERR_BADVALUE);
  }
  if (smbmsgr.numcmds != 1) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbmsgr.cmds[0].cmd != NETWOX_SMBMSG_CMD_DELETEDIRECTORY) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  netwib_eg(netwox_smbmsg_decode_smbcmd(&smbmsgr, 0, &smbcmdr));
  if (smbcmdr.type != NETWOX_SMBCMD_TYPE_DELETEDIRECTORY_R) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }

 netwib_gotolabel:
  netwib_er(netwox_smbcmd_close(&smbcmdr));
  netwib_er(netwox_smbcmd_close(&smbcmdq));
  netwib_er(netwox_smbmsg_close(&smbmsgr));
  netwib_er(netwox_smbmsg_close(&smbmsgq));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_dir_rename(netwox_smbcli *psmbcli,
                                    netwib_constbuf *polddirname,
                                    netwib_constbuf *pnewdirname)
{
  netwox_smbmsg smbmsgq, smbmsgr;
  netwox_smbcmd smbcmdq, smbcmdr;
  netwib_err ret;

  if (psmbcli->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbcli_dir_rename of %{buf} to %{buf}\n", polddirname, pnewdirname));
  }

  ret = NETWIB_ERR_OK;
  netwib_er(netwox_smbmsg_init(&smbmsgq));
  netwib_er(netwox_smbmsg_init(&smbmsgr));
  netwib_er(netwox_smbcmd_init(&smbcmdq));
  netwib_er(netwox_smbcmd_init(&smbcmdr));

  /* send request */
  netwib_eg(netwox_smbcli_smbmsg_setdefault(psmbcli, &smbmsgq));
  netwib_eg(netwox_smbcmd_selecttype(&smbcmdq, NETWOX_SMBCMD_TYPE_RENAME_Q));
  netwib_eg(netwox_smbcmd_setdefault(&smbcmdq));
  smbcmdq.value.rename_q.searchattributes = NETWOX_SMBCMDCMN_FILEATTR16_DIR | NETWOX_SMBCMDCMN_FILEATTR16_HIDDEN | NETWOX_SMBCMDCMN_FILEATTR16_SYSTEM | NETWOX_SMBCMDCMN_FILEATTR16_ARCHIVE;
  netwib_eg(netwox_smbcmd_append_buf_path(&smbmsgq, polddirname, &smbcmdq.value.rename_q.oldname));
  netwib_eg(netwox_smbcmd_append_buf_path(&smbmsgq, pnewdirname, &smbcmdq.value.rename_q.newname));
  netwib_eg(netwox_smbmsg_append_smbcmd(&smbcmdq, &smbmsgq));
  netwib_eg(netwox_smbcli_smbmsg_send(psmbcli, &smbmsgq));

  /* decode reply */
  netwib_eg(netwox_smbcli_smbmsg_recv(psmbcli, &smbmsgr));
  if (smbmsgr.hdr.error != 0) {
    if (psmbcli->verbose) {
      netwib_eg(netwox_smbmsg_error_display(psmbcli->pbufpool, &smbmsgr));
    }
    netwib_eg(NETWOX_ERR_BADVALUE);
  }
  if (smbmsgr.numcmds != 1) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbmsgr.cmds[0].cmd != NETWOX_SMBMSG_CMD_RENAME) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  netwib_eg(netwox_smbmsg_decode_smbcmd(&smbmsgr, 0, &smbcmdr));
  if (smbcmdr.type != NETWOX_SMBCMD_TYPE_RENAME_R) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }

 netwib_gotolabel:
  netwib_er(netwox_smbcmd_close(&smbcmdr));
  netwib_er(netwox_smbcmd_close(&smbcmdq));
  netwib_er(netwox_smbmsg_close(&smbmsgr));
  netwib_er(netwox_smbmsg_close(&smbmsgq));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_file_delete(netwox_smbcli *psmbcli,
                                     netwib_constbuf *pfilename)
{
  netwox_smbmsg smbmsgq, smbmsgr;
  netwox_smbcmd smbcmdq, smbcmdr;
  netwib_err ret;

  if (psmbcli->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbcli_file_delete of %{buf}\n", pfilename));
  }

  ret = NETWIB_ERR_OK;
  netwib_er(netwox_smbmsg_init(&smbmsgq));
  netwib_er(netwox_smbmsg_init(&smbmsgr));
  netwib_er(netwox_smbcmd_init(&smbcmdq));
  netwib_er(netwox_smbcmd_init(&smbcmdr));

  /* send request */
  netwib_eg(netwox_smbcli_smbmsg_setdefault(psmbcli, &smbmsgq));
  netwib_eg(netwox_smbcmd_selecttype(&smbcmdq, NETWOX_SMBCMD_TYPE_DELETE_Q));
  netwib_eg(netwox_smbcmd_setdefault(&smbcmdq));
  smbcmdq.value.delete_q.searchattributes = NETWOX_SMBCMDCMN_FILEATTR16_HIDDEN | NETWOX_SMBCMDCMN_FILEATTR16_SYSTEM | NETWOX_SMBCMDCMN_FILEATTR16_ARCHIVE;
  netwib_eg(netwox_smbcmd_append_buf_path(&smbmsgq, pfilename, &smbcmdq.value.delete_q.filename));
  netwib_eg(netwox_smbmsg_append_smbcmd(&smbcmdq, &smbmsgq));
  netwib_eg(netwox_smbcli_smbmsg_send(psmbcli, &smbmsgq));

  /* decode reply */
  netwib_eg(netwox_smbcli_smbmsg_recv(psmbcli, &smbmsgr));
  if (smbmsgr.hdr.error != 0) {
    if (psmbcli->verbose) {
      netwib_eg(netwox_smbmsg_error_display(psmbcli->pbufpool, &smbmsgr));
    }
    netwib_eg(NETWOX_ERR_BADVALUE);
  }
  if (smbmsgr.numcmds != 1) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbmsgr.cmds[0].cmd != NETWOX_SMBMSG_CMD_DELETE) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  netwib_eg(netwox_smbmsg_decode_smbcmd(&smbmsgr, 0, &smbcmdr));
  if (smbcmdr.type != NETWOX_SMBCMD_TYPE_DELETE_R) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }

 netwib_gotolabel:
  netwib_er(netwox_smbcmd_close(&smbcmdr));
  netwib_er(netwox_smbcmd_close(&smbcmdq));
  netwib_er(netwox_smbmsg_close(&smbmsgr));
  netwib_er(netwox_smbmsg_close(&smbmsgq));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_file_rename(netwox_smbcli *psmbcli,
                                     netwib_constbuf *poldfilename,
                                     netwib_constbuf *pnewfilename)
{
  netwox_smbmsg smbmsgq, smbmsgr;
  netwox_smbcmd smbcmdq, smbcmdr;
  netwib_err ret;

  if (psmbcli->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbcli_file_rename of %{buf} to %{buf}\n", poldfilename, pnewfilename));
  }

  ret = NETWIB_ERR_OK;
  netwib_er(netwox_smbmsg_init(&smbmsgq));
  netwib_er(netwox_smbmsg_init(&smbmsgr));
  netwib_er(netwox_smbcmd_init(&smbcmdq));
  netwib_er(netwox_smbcmd_init(&smbcmdr));

  /* send request */
  netwib_eg(netwox_smbcli_smbmsg_setdefault(psmbcli, &smbmsgq));
  netwib_eg(netwox_smbcmd_selecttype(&smbcmdq, NETWOX_SMBCMD_TYPE_RENAME_Q));
  netwib_eg(netwox_smbcmd_setdefault(&smbcmdq));
  smbcmdq.value.rename_q.searchattributes = NETWOX_SMBCMDCMN_FILEATTR16_HIDDEN | NETWOX_SMBCMDCMN_FILEATTR16_SYSTEM | NETWOX_SMBCMDCMN_FILEATTR16_ARCHIVE;
  netwib_eg(netwox_smbcmd_append_buf_path(&smbmsgq, poldfilename, &smbcmdq.value.rename_q.oldname));
  netwib_eg(netwox_smbcmd_append_buf_path(&smbmsgq, pnewfilename, &smbcmdq.value.rename_q.newname));
  netwib_eg(netwox_smbmsg_append_smbcmd(&smbcmdq, &smbmsgq));
  netwib_eg(netwox_smbcli_smbmsg_send(psmbcli, &smbmsgq));

  /* decode reply */
  netwib_eg(netwox_smbcli_smbmsg_recv(psmbcli, &smbmsgr));
  if (smbmsgr.hdr.error != 0) {
    if (psmbcli->verbose) {
      netwib_eg(netwox_smbmsg_error_display(psmbcli->pbufpool, &smbmsgr));
    }
    netwib_eg(NETWOX_ERR_BADVALUE);
  }
  if (smbmsgr.numcmds != 1) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbmsgr.cmds[0].cmd != NETWOX_SMBMSG_CMD_RENAME) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  netwib_eg(netwox_smbmsg_decode_smbcmd(&smbmsgr, 0, &smbcmdr));
  if (smbcmdr.type != NETWOX_SMBCMD_TYPE_RENAME_R) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }

 netwib_gotolabel:
  netwib_er(netwox_smbcmd_close(&smbcmdr));
  netwib_er(netwox_smbcmd_close(&smbcmdq));
  netwib_er(netwox_smbmsg_close(&smbmsgr));
  netwib_er(netwox_smbmsg_close(&smbmsgq));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcli_file_open(netwox_smbcli *psmbcli,
                                          netwib_constbuf *pfilename,
                                          netwib_file_inittype inittype,
                                          netwib_uint16 *pfileid)
{
  netwox_smbmsg smbmsgq, smbmsgr;
  netwox_smbcmd smbcmdq, smbcmdr;
  netwib_time t;
  netwib_err ret;

  if (psmbcli->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbcli_file_open of %{buf}\n", pfilename));
  }

  ret = NETWIB_ERR_OK;
  netwib_er(netwox_smbmsg_init(&smbmsgq));
  netwib_er(netwox_smbmsg_init(&smbmsgr));
  netwib_er(netwox_smbcmd_init(&smbcmdq));
  netwib_er(netwox_smbcmd_init(&smbcmdr));

  /* send request */
  netwib_eg(netwox_smbcli_smbmsg_setdefault(psmbcli, &smbmsgq));
  netwib_eg(netwox_smbcmd_selecttype(&smbcmdq, NETWOX_SMBCMD_TYPE_OPENANDX_Q));
  netwib_eg(netwox_smbcmd_setdefault(&smbcmdq));
  switch(inittype) {
  case NETWIB_FILE_INITTYPE_READ :
    smbcmdq.value.openandx_q.desiredaccess = NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS & NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS_R;
    smbcmdq.value.openandx_q.searchattributes = NETWOX_SMBCMDCMN_FILEATTR16_HIDDEN | NETWOX_SMBCMDCMN_FILEATTR16_SYSTEM | NETWOX_SMBCMDCMN_FILEATTR16_ARCHIVE;
    smbcmdq.value.openandx_q.openfunction = (NETWOX_SMBCMDCMN_OPENFUNCTION_CREATEFILENOEXIST & NETWOX_SMBCMDCMN_OPENFUNCTION_CREATEFILENOEXIST_ERROR) | (NETWOX_SMBCMDCMN_OPENFUNCTION_OPENFILEEXIST & NETWOX_SMBCMDCMN_OPENFUNCTION_OPENFILEEXIST_OPEN);
    break;
  case NETWIB_FILE_INITTYPE_WRITE :
    smbcmdq.value.openandx_q.desiredaccess = NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS & NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS_W;
    netwib_eg(netwib_time_init_now(&t));
    netwib_eg(netwox_time1970l_init_time(&t, &smbcmdq.value.openandx_q.creationtime));
    smbcmdq.value.openandx_q.openfunction = (NETWOX_SMBCMDCMN_OPENFUNCTION_CREATEFILENOEXIST & NETWOX_SMBCMDCMN_OPENFUNCTION_CREATEFILENOEXIST_OVERWRITE) | (NETWOX_SMBCMDCMN_OPENFUNCTION_OPENFILEEXIST & NETWOX_SMBCMDCMN_OPENFUNCTION_OPENFILEEXIST_TRUNCATE);
    break;
  default :
    netwib_eg(NETWOX_ERR_NOTIMPLEMENTED);
  }
  netwib_eg(netwox_smbcmd_append_buf_path(&smbmsgq, pfilename, &smbcmdq.value.openandx_q.filename));
  netwib_eg(netwox_smbmsg_append_smbcmd(&smbcmdq, &smbmsgq));
  netwib_eg(netwox_smbcli_smbmsg_send(psmbcli, &smbmsgq));

  /* decode reply */
  netwib_eg(netwox_smbcli_smbmsg_recv(psmbcli, &smbmsgr));
  if (smbmsgr.hdr.error != 0) {
    if (psmbcli->verbose) {
      netwib_eg(netwox_smbmsg_error_display(psmbcli->pbufpool, &smbmsgr));
    }
    netwib_eg(NETWOX_ERR_BADVALUE);
  }
  if (smbmsgr.numcmds != 1) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbmsgr.cmds[0].cmd != NETWOX_SMBMSG_CMD_OPENANDX) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  netwib_eg(netwox_smbmsg_decode_smbcmd(&smbmsgr, 0, &smbcmdr));
  if (smbcmdr.type != NETWOX_SMBCMD_TYPE_OPENANDX_R) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  switch(inittype) {
  case NETWIB_FILE_INITTYPE_READ :
    if ((smbcmdr.value.openandx_r.grantedaccess & NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS) != NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS_R) {
      netwib_eg(NETWOX_ERR_PROTOCOL);
    }
    if ((smbcmdr.value.openandx_r.action & NETWOX_SMBCMDCMN_CREATEACTION_ACTION) != NETWOX_SMBCMDCMN_CREATEACTION_ACTION_EXISTOPENED) {
      netwib_eg(NETWOX_ERR_PROTOCOL);
    }
    break;
  case NETWIB_FILE_INITTYPE_WRITE :
    /* Don't check because Windows 2000 says "ACCESS_READ"
    if ((smbcmdr.value.openandx_r.grantedaccess & NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS) != NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS_W) [
      netwib_eg(NETWOX_ERR_PROTOCOL);
    ]
    */
    switch (smbcmdr.value.openandx_r.action &
            NETWOX_SMBCMDCMN_CREATEACTION_ACTION) {
    case NETWOX_SMBCMDCMN_CREATEACTION_ACTION_NOEXISTCREATED :
    case NETWOX_SMBCMDCMN_CREATEACTION_ACTION_EXISTTRUNCATED :
      break;
    default :
      netwib_eg(NETWOX_ERR_PROTOCOL);
    }
    break;
  default :
    netwib_eg(NETWOX_ERR_NOTIMPLEMENTED);
  }
  *pfileid = smbcmdr.value.openandx_r.fileid;

 netwib_gotolabel:
  netwib_er(netwox_smbcmd_close(&smbcmdr));
  netwib_er(netwox_smbcmd_close(&smbcmdq));
  netwib_er(netwox_smbmsg_close(&smbmsgr));
  netwib_er(netwox_smbmsg_close(&smbmsgq));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcli_file_close(netwox_smbcli *psmbcli,
                                           netwib_uint16 fileid)
{
  netwox_smbmsg smbmsgq, smbmsgr;
  netwox_smbcmd smbcmdq, smbcmdr;
  netwib_err ret;

  if (psmbcli->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbcli_file_close\n"));
  }

  ret = NETWIB_ERR_OK;
  netwib_er(netwox_smbmsg_init(&smbmsgq));
  netwib_er(netwox_smbmsg_init(&smbmsgr));
  netwib_er(netwox_smbcmd_init(&smbcmdq));
  netwib_er(netwox_smbcmd_init(&smbcmdr));

  /* send request */
  netwib_eg(netwox_smbcli_smbmsg_setdefault(psmbcli, &smbmsgq));
  netwib_eg(netwox_smbcmd_selecttype(&smbcmdq, NETWOX_SMBCMD_TYPE_CLOSE_Q));
  netwib_eg(netwox_smbcmd_setdefault(&smbcmdq));
  smbcmdq.value.close_q.fileid = fileid;
  netwib_eg(netwox_smbmsg_append_smbcmd(&smbcmdq, &smbmsgq));
  netwib_eg(netwox_smbcli_smbmsg_send(psmbcli, &smbmsgq));

  /* decode reply */
  netwib_eg(netwox_smbcli_smbmsg_recv(psmbcli, &smbmsgr));
  if (smbmsgr.hdr.error != 0) {
    if (psmbcli->verbose) {
      netwib_eg(netwox_smbmsg_error_display(psmbcli->pbufpool, &smbmsgr));
    }
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbmsgr.numcmds != 1) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbmsgr.cmds[0].cmd != NETWOX_SMBMSG_CMD_CLOSE) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  netwib_eg(netwox_smbmsg_decode_smbcmd(&smbmsgr, 0, &smbcmdr));
  if (smbcmdr.type != NETWOX_SMBCMD_TYPE_CLOSE_R) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }

 netwib_gotolabel:
  netwib_er(netwox_smbcmd_close(&smbcmdr));
  netwib_er(netwox_smbcmd_close(&smbcmdq));
  netwib_er(netwox_smbmsg_close(&smbmsgr));
  netwib_er(netwox_smbmsg_close(&smbmsgq));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcli_file_read(netwox_smbcli *psmbcli,
                                          netwib_uint16 fileid,
                                          netwib_uint64 *poffset,
                                          netwib_buf *pbuf)
{
  netwox_smbmsg smbmsgq, smbmsgr;
  netwox_smbcmd smbcmdq, smbcmdr;
  netwib_uint64 size64, tmp;
  netwib_err ret;

  if (psmbcli->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbcli_file_read\n"));
  }

  ret = NETWIB_ERR_OK;
  netwib_er(netwox_smbmsg_init(&smbmsgq));
  netwib_er(netwox_smbmsg_init(&smbmsgr));
  netwib_er(netwox_smbcmd_init(&smbcmdq));
  netwib_er(netwox_smbcmd_init(&smbcmdr));

  /* send request */
  netwib_eg(netwox_smbcli_smbmsg_setdefault(psmbcli, &smbmsgq));
  netwib_eg(netwox_smbcmd_selecttype(&smbcmdq, NETWOX_SMBCMD_TYPE_READANDX_Q));
  netwib_eg(netwox_smbcmd_setdefault(&smbcmdq));
  smbcmdq.value.readandx_q.fileid = fileid;
  smbcmdq.value.readandx_q.offset = *poffset;
#define NETWOX_SMBCLI_FILE_BLOCKLEN 1024
  smbcmdq.value.readandx_q.maxsize = NETWOX_SMBCLI_FILE_BLOCKLEN;
  smbcmdq.value.readandx_q.minsize = NETWOX_SMBCLI_FILE_BLOCKLEN;
  smbcmdq.value.readandx_q.remaining = NETWOX_SMBCMD_READANDX_Q_REMAINING_SAFE(smbcmdq.value.readandx_q.maxsize);
  netwib_eg(netwox_smbmsg_append_smbcmd(&smbcmdq, &smbmsgq));
  netwib_eg(netwox_smbcli_smbmsg_send(psmbcli, &smbmsgq));

  /* decode reply */
  netwib_eg(netwox_smbcli_smbmsg_recv(psmbcli, &smbmsgr));
  if (smbmsgr.hdr.error != 0) {
    if (psmbcli->verbose) {
      netwib_eg(netwox_smbmsg_error_display(psmbcli->pbufpool, &smbmsgr));
    }
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbmsgr.numcmds != 1) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbmsgr.cmds[0].cmd != NETWOX_SMBMSG_CMD_READANDX) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  netwib_eg(netwox_smbmsg_decode_smbcmd(&smbmsgr, 0, &smbcmdr));
  if (smbcmdr.type != NETWOX_SMBCMD_TYPE_READANDX_R) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (netwib__buf_ref_data_size(&smbcmdr.value.readandx_r.data) == 0) {
    netwib_eg(NETWIB_ERR_DATAEND);
  }
  netwib_eg(netwib_buf_append_buf(&smbcmdr.value.readandx_r.data, pbuf));
  netwib__uint64_init_uint32(netwib__buf_ref_data_size(&smbcmdr.value.readandx_r.data), size64);
  tmp = *poffset;
  netwib__uint64_add(size64, tmp, *poffset);

 netwib_gotolabel:
  netwib_er(netwox_smbcmd_close(&smbcmdr));
  netwib_er(netwox_smbcmd_close(&smbcmdq));
  netwib_er(netwox_smbmsg_close(&smbmsgr));
  netwib_er(netwox_smbmsg_close(&smbmsgq));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcli_file_write(netwox_smbcli *psmbcli,
                                           netwib_uint16 fileid,
                                           netwib_uint64 *poffset,
                                           netwib_constbuf *pbuf)
{
  netwox_smbmsg smbmsgq, smbmsgr;
  netwox_smbcmd smbcmdq, smbcmdr;
  netwib_uint64 size64, tmp;
  netwib_err ret;

  if (psmbcli->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbcli_file_write\n"));
  }

  ret = NETWIB_ERR_OK;
  netwib_er(netwox_smbmsg_init(&smbmsgq));
  netwib_er(netwox_smbmsg_init(&smbmsgr));
  netwib_er(netwox_smbcmd_init(&smbcmdq));
  netwib_er(netwox_smbcmd_init(&smbcmdr));

  /* send request */
  netwib_eg(netwox_smbcli_smbmsg_setdefault(psmbcli, &smbmsgq));
  netwib_eg(netwox_smbcmd_selecttype(&smbcmdq, NETWOX_SMBCMD_TYPE_WRITEANDX_Q));
  netwib_eg(netwox_smbcmd_setdefault(&smbcmdq));
  smbcmdq.value.writeandx_q.fileid = fileid;
  smbcmdq.value.writeandx_q.offset = *poffset;
  smbcmdq.value.writeandx_q.remaining = NETWOX_SMBCMD_WRITEANDX_Q_REMAINING_SAFE(pbuf);
  netwib_eg(netwib_buf_init_ext_buf(pbuf, &smbcmdq.value.writeandx_q.data));
  netwib_eg(netwox_smbmsg_append_smbcmd(&smbcmdq, &smbmsgq));
  netwib_eg(netwox_smbcli_smbmsg_send(psmbcli, &smbmsgq));

  /* decode reply */
  netwib_eg(netwox_smbcli_smbmsg_recv(psmbcli, &smbmsgr));
  if (smbmsgr.hdr.error != 0) {
    if (psmbcli->verbose) {
      netwib_eg(netwox_smbmsg_error_display(psmbcli->pbufpool, &smbmsgr));
    }
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbmsgr.numcmds != 1) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbmsgr.cmds[0].cmd != NETWOX_SMBMSG_CMD_WRITEANDX) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  netwib_eg(netwox_smbmsg_decode_smbcmd(&smbmsgr, 0, &smbcmdr));
  if (smbcmdr.type != NETWOX_SMBCMD_TYPE_WRITEANDX_R) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (smbcmdr.value.writeandx_r.writtensize != netwib__buf_ref_data_size(&smbcmdq.value.writeandx_q.data)) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  netwib__uint64_init_uint32(smbcmdr.value.writeandx_r.writtensize, size64);
  tmp = *poffset;
  netwib__uint64_add(size64, tmp, *poffset);

 netwib_gotolabel:
  netwib_er(netwox_smbcmd_close(&smbcmdr));
  netwib_er(netwox_smbcmd_close(&smbcmdq));
  netwib_er(netwox_smbmsg_close(&smbmsgr));
  netwib_er(netwox_smbmsg_close(&smbmsgq));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_file_get(netwox_smbcli *psmbcli,
                                  netwib_constbuf *premotefile,
                                  netwib_constbuf *plocalfile)
{
  netwib_io *pfileio;
  netwib_err ret;

  netwib_er(netwib_io_init_file_write(plocalfile, &pfileio));
  ret = netwox_smbcli_file_get_io(psmbcli, premotefile, pfileio);
  netwib_er(netwib_io_close(&pfileio));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_file_get_io(netwox_smbcli *psmbcli,
                                     netwib_constbuf *premotefile,
                                     netwib_io *plocalio)
{
  netwib_uint64 offset;
  netwib_uint16 fileid;
  netwib_buf *pbuf;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwox_smbcli_file_open(psmbcli, premotefile,
                                    NETWIB_FILE_INITTYPE_READ, &fileid));

  netwib_er(netwib_bufpool_buf_init(psmbcli->pbufpool, &pbuf));
  netwib__uint64_init_uint32(0, offset);
  while(NETWIB_TRUE) {
    netwib__buf_reinit(pbuf);
    ret = netwox_smbcli_file_read(psmbcli, fileid, &offset, pbuf);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) {
        ret = NETWIB_ERR_OK;
      }
      break;
    }
    ret = netwib_io_write(plocalio, pbuf);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_bufpool_buf_close(psmbcli->pbufpool, &pbuf));

  netwib_er(netwox_smbcli_file_close(psmbcli, fileid));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_file_put(netwox_smbcli *psmbcli,
                                  netwib_constbuf *plocalfile,
                                  netwib_constbuf *premotefile)
{
  netwib_io *pfileio;
  netwib_err ret;

  netwib_er(netwib_io_init_file_read(plocalfile, &pfileio));
  ret = netwox_smbcli_file_put_io(psmbcli, pfileio, premotefile);
  netwib_er(netwib_io_close(&pfileio));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_file_put_io(netwox_smbcli *psmbcli,
                                     netwib_io *plocalio,
                                     netwib_constbuf *premotefile)
{
  netwib_uint64 offset;
  netwib_uint32 endoffsetori;
  netwib_uint16 fileid;
  netwib_buf *pbuf;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwox_smbcli_file_open(psmbcli, premotefile,
                                    NETWIB_FILE_INITTYPE_WRITE, &fileid));

  netwib_er(netwib_bufpool_buf_init(psmbcli->pbufpool, &pbuf));
  netwib__uint64_init_uint32(0, offset);
  while(NETWIB_TRUE) {
    if (netwib__buf_ref_data_size(pbuf) == 0) {
      netwib__buf_reinit(pbuf);
      ret = netwib_io_read(plocalio, pbuf);
      if (ret != NETWIB_ERR_OK) {
        if (ret == NETWIB_ERR_DATAEND) {
          ret = NETWIB_ERR_OK;
        }
        break;
      }
    }
    if (netwib__buf_ref_data_size(pbuf) > NETWOX_SMBCLI_FILE_BLOCKLEN) {
      endoffsetori = pbuf->endoffset;
      pbuf->endoffset = pbuf->beginoffset + NETWOX_SMBCLI_FILE_BLOCKLEN;
      ret = netwox_smbcli_file_write(psmbcli, fileid, &offset, pbuf);
      pbuf->beginoffset += NETWOX_SMBCLI_FILE_BLOCKLEN;
      pbuf->endoffset = endoffsetori;
    } else {
      ret = netwox_smbcli_file_write(psmbcli, fileid, &offset, pbuf);
      netwib__buf_reinit(pbuf);
    }
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_bufpool_buf_close(psmbcli->pbufpool, &pbuf));

  netwib_er(netwox_smbcli_file_close(psmbcli, fileid));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smb_dirlist_init(netwox_smb_dirlist *pdirlist)
{
  netwib_er(netwib_buf_init_mallocdefault(&pdirlist->pathname));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smb_dirlist_close(netwox_smb_dirlist *pdirlist)
{
  netwib_er(netwib_buf_close(&pdirlist->pathname));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smb_dirlist_erase(netwib_ptr pitem)
{
  netwib_er(netwox_smb_dirlist_close((netwox_smb_dirlist *)pitem));
  netwib_er(netwib_ptr_free(&pitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smb_dirlist_duplicate(netwib_constptr pitem,
                                        netwib_ptr *pdupofitem)
{
  const netwox_smb_dirlist *pdirlist = (const netwox_smb_dirlist *)pitem;
  netwox_smb_dirlist *pdirlistdup;
  netwib_ptr ptr;

  netwib_er(netwib_ptr_malloc(sizeof(netwox_smb_dirlist), &ptr));
  pdirlistdup = (netwox_smb_dirlist *)ptr;
  *pdupofitem = ptr;

  netwib_er(netwox_smb_dirlist_init(pdirlistdup));
  netwib_er(netwib_buf_append_buf(&pdirlist->pathname,
                                  &pdirlistdup->pathname));
  pdirlistdup->pathstat = pdirlist->pathstat;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcli_dir_ls_addring(netwox_constsmbmsg *psmbmsg,
                         const netwox_smbcmdtsc2_fmt_find_entry *pfe,
                                               netwib_ring *pring)
{
  netwox_smb_dirlist *pdirlist;
  netwib_string name;
  netwib_ptr ptr;
  netwib_err ret;

  /* initialize the item */
  netwib_er(netwib_ptr_malloc(sizeof(netwox_smb_dirlist), &ptr));
  pdirlist = (netwox_smb_dirlist *)ptr;
  netwib_er(netwox_smb_dirlist_init(pdirlist));

  if (pfe->fileattr32 & NETWOX_SMBCMDCMN_FILEATTR32_DIR) {
    pdirlist->pathstat.type = NETWIB_PATHSTAT_TYPE_DIR;
  } else {
    pdirlist->pathstat.type = NETWIB_PATHSTAT_TYPE_REG;
  }
  pdirlist->pathstat.size64 = pfe->filesize;
  if (netwib_c2_uint64_32high(pfe->filesize)) {
    pdirlist->pathstat.size = NETWIB_PATHSTAT_SIZE_GT2G;
  } else {
    pdirlist->pathstat.size = netwib_c2_uint64_32low(pfe->filesize);
  }
  ret = netwox_time_init_time1601(pfe->lastwritetime,
                                  &pdirlist->pathstat.mtime);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_smb_dirlist_erase(ptr));
    return(ret);
  }
  ret = netwox_smbcmd_decode_buf_path(psmbmsg, &pfe->longfilename,
                                      &pdirlist->pathname);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_smb_dirlist_erase(ptr));
    return(ret);
  }

  /* add in ring */
  netwib_er(netwib_buf_ref_string(&pdirlist->pathname, &name));
  if (!netwib_c_strcmp(name, ".") || !netwib_c_strcmp(name, "..")) {
    netwib_er(netwox_smb_dirlist_erase(ptr));
  } else {
    netwib_er(netwib_ring_add_last(pring, ptr));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 sid;
  netwib_bool endofsearch;
  netwib_buf lastname;
} netwox_smbcli_dirstate;

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcli_dir_ls_findfirst(netwox_smbcli *psmbcli,
                                                 netwib_constbuf *pdirname,
                                                 netwib_ring *pring,
                                           netwox_smbcli_dirstate *pdirstate)
{
  netwox_smbmsg smbmsgq, smbmsgr;
  netwox_smbcmd smbcmdq, smbcmdr;
  netwox_smbcmdtsc2q smbcmdtsc2q;
  netwox_smbcmdtsc2rstorage storager;
  netwox_smbcmdtsc2r smbcmdtsc2r;
  netwox_smbcmdtsc2rfmt smbcmdtsc2rfmt;
  netwox_smbcmdtsc2_fmt_find_entry *pfe;
  netwib_bool needmoremsg;
  netwib_uint32 i;
  netwib_err ret;

  ret = NETWIB_ERR_OK;
  netwib_er(netwox_smbmsg_init(&smbmsgq));
  netwib_er(netwox_smbmsg_init(&smbmsgr));
  netwib_er(netwox_smbcmd_init(&smbcmdq));
  netwib_er(netwox_smbcmd_init(&smbcmdr));
  netwib_er(netwox_smbcmdtsc2q_init(&smbcmdtsc2q));
  netwib_er(netwox_smbcmdtsc2rstorage_init(psmbcli->negotiate_capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_UNICODE, &storager));
  netwib_er(netwox_smbcmdtsc2r_init(&smbcmdtsc2r));

  /* send request */
  /*  init msg to set unicode flag */
  netwib_eg(netwox_smbcli_smbmsg_setdefault(psmbcli, &smbmsgq));
  /*  construct transaction data */
  netwib_eg(netwox_smbcmdtsc2q_selecttype(&smbcmdtsc2q, NETWOX_SMBCMDTSC2Q_TYPE_FINDFIRST2));
  netwib_eg(netwox_smbcmdtsc2q_setdefault(&smbcmdtsc2q));
  smbcmdtsc2q.value.findfirst2.searchattributes = NETWOX_SMBCMDCMN_FILEATTR16_DIR | NETWOX_SMBCMDCMN_FILEATTR16_HIDDEN | NETWOX_SMBCMDCMN_FILEATTR16_SYSTEM | NETWOX_SMBCMDCMN_FILEATTR16_ARCHIVE;
  smbcmdtsc2q.value.findfirst2.maxentries = 100;
  smbcmdtsc2q.value.findfirst2.flags = NETWOX_SMBCMDTSC2Q_FIND2_FLAGS_CLOSEONEOS;
  smbcmdtsc2q.value.findfirst2.fmt = NETWOX_SMBCMDTSC2_FMT_FIND_BOTHDIRECTORY;
  netwib_eg(netwox_smbcmd_append_buf_path(&smbmsgq, pdirname, &smbcmdtsc2q.value.findfirst2.searchpattern));
  {
    netwib_data data;
    netwib_buf buf;
    netwib_eg(netwib_buf_init_ext_string("/*", &buf));
    if (netwib__buf_ref_data_size(pdirname)) {
      data = netwib__buf_ref_data_ptr(pdirname);
      data += netwib__buf_ref_data_size(pdirname) - 1;
      if (*data == '/' || *data == '\\') {
        netwib_eg(netwib_buf_init_ext_string("*", &buf));
      }
    }
    netwib_eg(netwox_smbcmd_append_buf_path(&smbmsgq, &buf, &smbcmdtsc2q.value.findfirst2.searchpattern));
  }
  /*  construct command */
  netwib_eg(netwox_smbcmd_selecttype(&smbcmdq,
                                     NETWOX_SMBCMD_TYPE_TRANSACTION2_Q));
  netwib_eg(netwox_smbcmd_setdefault(&smbcmdq));
  smbcmdq.value.transaction2_q.isunicode = psmbcli->negotiate_capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_UNICODE;
  smbcmdq.value.transaction2_q.timeout = psmbcli->maxwaitms;
  if (psmbcli->debug) {
    netwib_eg(netwib_fmt_display("Transaction2 query:\n"));
    netwib_eg(netwox_smbcmdtsc2q_display(psmbcli->pbufpool, &smbcmdtsc2q));
  }
  netwib_eg(netwox_smbcmdtsc2q_decode_smbcmd(&smbcmdtsc2q, &smbcmdq));
  /*  send command */
  netwib_eg(netwox_smbmsg_append_smbcmd(&smbcmdq, &smbmsgq));
  netwib_eg(netwox_smbcli_smbmsg_send(psmbcli, &smbmsgq));

  /* decode reply/replies */
  while (NETWIB_TRUE) {
    netwib_eg(netwox_smbcli_smbmsg_recv(psmbcli, &smbmsgr));
    if (smbmsgr.hdr.error != 0) {
      if (psmbcli->verbose) {
        netwib_eg(netwox_smbmsg_error_display(psmbcli->pbufpool, &smbmsgr));
      }
      netwib_eg(NETWOX_ERR_BADVALUE);
    }
    if (smbmsgr.numcmds != 1) {
      netwib_eg(NETWOX_ERR_PROTOCOL);
    }
    if (smbmsgr.cmds[0].cmd != NETWOX_SMBMSG_CMD_TRANSACTION2) {
      netwib_eg(NETWOX_ERR_PROTOCOL);
    }
    netwib_eg(netwox_smbmsg_decode_smbcmd(&smbmsgr, 0, &smbcmdr));
    if (smbcmdr.type != NETWOX_SMBCMD_TYPE_TRANSACTION2_R) {
      netwib_eg(NETWOX_ERR_PROTOCOL);
    }
    netwib_eg(netwox_smbcmdtsc2rstorage_append_smbcmd(&smbcmdr, &storager,
                                                      &needmoremsg));
    if (!needmoremsg) {
      break;
    }
    netwib_er(netwox_smbcmd_close(&smbcmdr));
    netwib_er(netwox_smbmsg_close(&smbmsgr));
    netwib_er(netwox_smbmsg_init(&smbmsgr));
    netwib_er(netwox_smbcmd_init(&smbcmdr));
  }
  netwib_eg(netwox_smbcmdtsc2rfmt_init_smbcmdtsc2q(&smbcmdtsc2q,
                                                   &smbcmdtsc2rfmt));
  netwib_eg(netwox_smbcmdtsc2r_encode_storager(&storager, &smbcmdtsc2rfmt,
                                               &smbcmdtsc2r));
  if (psmbcli->debug) {
    netwib_eg(netwib_fmt_display("Transaction2 reply:\n"));
    netwib_eg(netwox_smbcmdtsc2r_display(psmbcli->pbufpool, &smbcmdtsc2r));
  }
  if (smbcmdtsc2r.type != NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_BOTHDIRECTORY) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  pdirstate->sid = smbcmdtsc2r.value.findfirst2_common.sid;
  pdirstate->endofsearch = smbcmdtsc2r.value.findfirst2_common.endofsearch;
  for (i = 0; i < smbcmdtsc2r.value.findfirst2_common.entries.size; i++) {
    pfe = (netwox_smbcmdtsc2_fmt_find_entry*)smbcmdtsc2r.value.findfirst2_common.entries.p[i];
    netwib_eg(netwox_smbcli_dir_ls_addring(&smbmsgr, pfe, pring));
  }
  if (!pdirstate->endofsearch) {
    if (smbcmdtsc2r.value.findfirst2_common.entries.size) {
      netwib__buf_reinit(&pdirstate->lastname);
      i = smbcmdtsc2r.value.findfirst2_common.entries.size - 1;
      pfe = (netwox_smbcmdtsc2_fmt_find_entry*)smbcmdtsc2r.value.findfirst2_common.entries.p[i];
      netwib_er(netwox_smbcmd_decode_buf_path(&smbmsgr, &pfe->longfilename,
                                              &pdirstate->lastname));
    } else {
      pdirstate->endofsearch = NETWIB_TRUE;
    }
  }

 netwib_gotolabel:
  netwib_er(netwox_smbcmdtsc2r_close(&smbcmdtsc2r));
  netwib_er(netwox_smbcmdtsc2rstorage_close(&storager));
  netwib_er(netwox_smbcmdtsc2q_close(&smbcmdtsc2q));
  netwib_er(netwox_smbcmd_close(&smbcmdr));
  netwib_er(netwox_smbcmd_close(&smbcmdq));
  netwib_er(netwox_smbmsg_close(&smbmsgr));
  netwib_er(netwox_smbmsg_close(&smbmsgq));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcli_dir_ls_findnext(netwox_smbcli *psmbcli,
                                                netwib_ring *pring,
                                             netwox_smbcli_dirstate *pdirstate)
{
  netwox_smbmsg smbmsgq, smbmsgr;
  netwox_smbcmd smbcmdq, smbcmdr;
  netwox_smbcmdtsc2q smbcmdtsc2q;
  netwox_smbcmdtsc2rstorage storager;
  netwox_smbcmdtsc2r smbcmdtsc2r;
  netwox_smbcmdtsc2rfmt smbcmdtsc2rfmt;
  netwox_smbcmdtsc2_fmt_find_entry *pfe;
  netwib_bool needmoremsg;
  netwib_uint32 i;
  netwib_err ret;

  ret = NETWIB_ERR_OK;
  netwib_er(netwox_smbmsg_init(&smbmsgq));
  netwib_er(netwox_smbmsg_init(&smbmsgr));
  netwib_er(netwox_smbcmd_init(&smbcmdq));
  netwib_er(netwox_smbcmd_init(&smbcmdr));
  netwib_er(netwox_smbcmdtsc2q_init(&smbcmdtsc2q));
  netwib_er(netwox_smbcmdtsc2rstorage_init(psmbcli->negotiate_capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_UNICODE, &storager));
  netwib_er(netwox_smbcmdtsc2r_init(&smbcmdtsc2r));

  /* send request */
  /*  init msg to set unicode flag */
  netwib_eg(netwox_smbcli_smbmsg_setdefault(psmbcli, &smbmsgq));
  /*  construct transaction data */
  netwib_eg(netwox_smbcmdtsc2q_selecttype(&smbcmdtsc2q, NETWOX_SMBCMDTSC2Q_TYPE_FINDNEXT2));
  netwib_eg(netwox_smbcmdtsc2q_setdefault(&smbcmdtsc2q));
  smbcmdtsc2q.value.findnext2.sid = pdirstate->sid;
  smbcmdtsc2q.value.findnext2.maxentries = 100;
  smbcmdtsc2q.value.findnext2.fmt = NETWOX_SMBCMDTSC2_FMT_FIND_BOTHDIRECTORY;
  smbcmdtsc2q.value.findnext2.flags = NETWOX_SMBCMDTSC2Q_FIND2_FLAGS_CLOSEONEOS;
  netwib_eg(netwox_smbcmd_append_buf_path(&smbmsgq, &pdirstate->lastname, &smbcmdtsc2q.value.findnext2.resumefilename));
  /*  construct command */
  netwib_eg(netwox_smbcmd_selecttype(&smbcmdq,
                                     NETWOX_SMBCMD_TYPE_TRANSACTION2_Q));
  netwib_eg(netwox_smbcmd_setdefault(&smbcmdq));
  smbcmdq.value.transaction2_q.isunicode = psmbcli->negotiate_capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_UNICODE;
  smbcmdq.value.transaction2_q.timeout = psmbcli->maxwaitms;
  if (psmbcli->debug) {
    netwib_eg(netwib_fmt_display("Transaction2 query:\n"));
    netwib_eg(netwox_smbcmdtsc2q_display(psmbcli->pbufpool, &smbcmdtsc2q));
  }
  netwib_eg(netwox_smbcmdtsc2q_decode_smbcmd(&smbcmdtsc2q, &smbcmdq));
  /*  send command */
  netwib_eg(netwox_smbmsg_append_smbcmd(&smbcmdq, &smbmsgq));
  netwib_eg(netwox_smbcli_smbmsg_send(psmbcli, &smbmsgq));

  /* decode reply/replies */
  while (NETWIB_TRUE) {
    netwib_eg(netwox_smbcli_smbmsg_recv(psmbcli, &smbmsgr));
    if (smbmsgr.hdr.error != 0) {
      if (psmbcli->verbose) {
        netwib_eg(netwox_smbmsg_error_display(psmbcli->pbufpool, &smbmsgr));
      }
      netwib_eg(NETWOX_ERR_PROTOCOL);
    }
    if (smbmsgr.numcmds != 1) {
      netwib_eg(NETWOX_ERR_PROTOCOL);
    }
    if (smbmsgr.cmds[0].cmd != NETWOX_SMBMSG_CMD_TRANSACTION2) {
      netwib_eg(NETWOX_ERR_PROTOCOL);
    }
    netwib_eg(netwox_smbmsg_decode_smbcmd(&smbmsgr, 0, &smbcmdr));
    if (smbcmdr.type != NETWOX_SMBCMD_TYPE_TRANSACTION2_R) {
      netwib_eg(NETWOX_ERR_PROTOCOL);
    }
    netwib_eg(netwox_smbcmdtsc2rstorage_append_smbcmd(&smbcmdr, &storager,
                                                      &needmoremsg));
    if (!needmoremsg) {
      break;
    }
    netwib_er(netwox_smbcmd_close(&smbcmdr));
    netwib_er(netwox_smbmsg_close(&smbmsgr));
    netwib_er(netwox_smbmsg_init(&smbmsgr));
    netwib_er(netwox_smbcmd_init(&smbcmdr));
  }
  netwib_eg(netwox_smbcmdtsc2rfmt_init_smbcmdtsc2q(&smbcmdtsc2q,
                                                   &smbcmdtsc2rfmt));
  netwib_eg(netwox_smbcmdtsc2r_encode_storager(&storager, &smbcmdtsc2rfmt,
                                               &smbcmdtsc2r));
  if (psmbcli->debug) {
    netwib_eg(netwib_fmt_display("Transaction2 reply:\n"));
    netwib_eg(netwox_smbcmdtsc2r_display(psmbcli->pbufpool, &smbcmdtsc2r));
  }
  if (smbcmdtsc2r.type != NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_BOTHDIRECTORY) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  pdirstate->endofsearch = smbcmdtsc2r.value.findnext2_common.endofsearch;
  for (i = 0; i < smbcmdtsc2r.value.findnext2_common.entries.size; i++) {
    pfe = (netwox_smbcmdtsc2_fmt_find_entry*)smbcmdtsc2r.value.findnext2_common.entries.p[i];
    netwib_eg(netwox_smbcli_dir_ls_addring(&smbmsgr, pfe, pring));
  }
  if (!pdirstate->endofsearch) {
    if (smbcmdtsc2r.value.findnext2_common.entries.size) {
      netwib__buf_reinit(&pdirstate->lastname);
      i = smbcmdtsc2r.value.findnext2_common.entries.size - 1;
      pfe = (netwox_smbcmdtsc2_fmt_find_entry*)smbcmdtsc2r.value.findnext2_common.entries.p[i];
      netwib_er(netwox_smbcmd_decode_buf_path(&smbmsgr, &pfe->longfilename,
                                              &pdirstate->lastname));
    } else {
      pdirstate->endofsearch = NETWIB_TRUE;
    }
  }

 netwib_gotolabel:
  netwib_er(netwox_smbcmdtsc2r_close(&smbcmdtsc2r));
  netwib_er(netwox_smbcmdtsc2rstorage_close(&storager));
  netwib_er(netwox_smbcmdtsc2q_close(&smbcmdtsc2q));
  netwib_er(netwox_smbcmd_close(&smbcmdr));
  netwib_er(netwox_smbcmd_close(&smbcmdq));
  netwib_er(netwox_smbmsg_close(&smbmsgr));
  netwib_er(netwox_smbmsg_close(&smbmsgq));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_dir_ls(netwox_smbcli *psmbcli,
                                netwib_constbuf *pdirname,
                                netwib_ring *pring)
{
  netwox_smbcli_dirstate dirstate;
  netwib_err ret;

  if (psmbcli->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbcli_dir_ls of %{buf}\n", pdirname));
  }

  ret = NETWIB_ERR_OK;
  netwib_er(netwib_buf_init_mallocdefault(&dirstate.lastname));

  netwib_eg(netwox_smbcli_dir_ls_findfirst(psmbcli, pdirname, pring,
                                           &dirstate));

  while (!dirstate.endofsearch) {
    netwib_eg(netwox_smbcli_dir_ls_findnext(psmbcli, pring, &dirstate));
  }

 netwib_gotolabel:
  netwib_er(netwib_buf_close(&dirstate.lastname));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_dirrec_get(netwox_smbcli *psmbcli,
                                    netwib_constbuf *premotedirname,
                                    netwib_constbuf *plocaldirname)
{
  netwib_ring *pring;
  netwib_ring_index *pringindex;
  netwox_smb_dirlist *pdirlist;
  netwib_ptr pitem;
  netwib_buf *premotepath, *plocalpath;
  netwib_bool exists;
  netwib_err ret;

  if (psmbcli->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbcli_dirrec_get from %{buf} to %{buf}\n", premotedirname, plocaldirname));
  }

  /* create local dir */
  netwib_er(netwib_dirname_exists(plocaldirname, &exists));
  if (!exists) {
    netwib_er(netwib_dirname_create(plocaldirname));
  }

  /* init */
  ret = NETWIB_ERR_OK;
  netwib_er(netwox_smb_dirlist_ring_init(&pring));
  netwib_er(netwib_ring_index_init(pring, &pringindex));
  netwib_er(netwib_bufpool_buf_init(psmbcli->pbufpool, &premotepath));
  netwib_er(netwib_bufpool_buf_init(psmbcli->pbufpool, &plocalpath));

  /* loop for each file in remote dir */
  netwib_eg(netwox_smbcli_dir_ls(psmbcli, premotedirname, pring));
  while(NETWIB_TRUE) {
    /* get remote filename */
    ret = netwib_ring_index_next(pringindex, &pitem);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    pdirlist = (netwox_smb_dirlist *)pitem;
    /* append this name to both path */
    netwib__buf_reinit(premotepath);
    netwib_er(netwib_buf_append_buf(premotedirname, premotepath));
    netwib_er(netwib_buf_append_byte('/', premotepath));
    netwib_er(netwib_buf_append_buf(&pdirlist->pathname, premotepath));
    netwib__buf_reinit(plocalpath);
    netwib_er(netwib_buf_append_buf(plocaldirname, plocalpath));
    netwib_er(netwib_buf_append_byte('/', plocalpath));
    netwib_er(netwib_buf_append_buf(&pdirlist->pathname, plocalpath));
    if (psmbcli->verbose) {
      netwib_er(netwib_fmt_display("Path %{buf}\n", premotepath));
    }
    /* create local file depending on its type */
    switch(pdirlist->pathstat.type) {
    case NETWIB_PATHSTAT_TYPE_REG :
      ret = netwox_smbcli_file_get(psmbcli, premotepath, plocalpath);
      break;
    case NETWIB_PATHSTAT_TYPE_DIR :
      ret = netwox_smbcli_dirrec_get(psmbcli, premotepath, plocalpath);
      break;
    default :
      break;
    }
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }

  /* close */
 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(psmbcli->pbufpool, &plocalpath));
  netwib_er(netwib_bufpool_buf_close(psmbcli->pbufpool, &premotepath));
  netwib_er(netwib_ring_index_close(&pringindex));
  netwib_er(netwox_smb_dirlist_ring_close(&pring));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_dirrec_put(netwox_smbcli *psmbcli,
                                    netwib_constbuf *plocaldirname,
                                    netwib_constbuf *premotedirname)
{
  netwib_dir *pdir;
  netwib_pathstat pathstat;
  netwib_buf *premotepath, *plocalpath, *ppathname;
  netwib_err ret;

  if (psmbcli->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbcli_dirrec_put from %{buf} to %{buf}\n", plocaldirname, premotedirname));
  }

  /* create remote dir if does not exist */
  ret = netwox_smbcli_stat(psmbcli, premotedirname, &pathstat);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_smbcli_dir_create(psmbcli, premotedirname));
  }

  /* init */
  ret = NETWIB_ERR_OK;
  netwib_er(netwib_dir_init(plocaldirname, &pdir));
  netwib_er(netwib_bufpool_buf_init(psmbcli->pbufpool, &ppathname));
  netwib_er(netwib_bufpool_buf_init(psmbcli->pbufpool, &premotepath));
  netwib_er(netwib_bufpool_buf_init(psmbcli->pbufpool, &plocalpath));

  /* loop for each file in local dir */
  while(NETWIB_TRUE) {
    /* get local filename */
    netwib__buf_reinit(ppathname);
    ret = netwib_dir_next(pdir, ppathname);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    /* append this name to both path */
    netwib__buf_reinit(plocalpath);
    netwib_er(netwib_buf_append_buf(plocaldirname, plocalpath));
    netwib_er(netwib_buf_append_byte('/', plocalpath));
    netwib_er(netwib_buf_append_buf(ppathname, plocalpath));
    netwib__buf_reinit(premotepath);
    netwib_er(netwib_buf_append_buf(premotedirname, premotepath));
    netwib_er(netwib_buf_append_byte('/', premotepath));
    netwib_er(netwib_buf_append_buf(ppathname, premotepath));
    if (psmbcli->verbose) {
      netwib_er(netwib_fmt_display("Path %{buf}\n", plocalpath));
    }
    /* create remote file depending on its type */
    netwib_eg(netwib_pathstat_init(plocalpath, &pathstat));
    switch(pathstat.type) {
    case NETWIB_PATHSTAT_TYPE_REG :
      ret = netwox_smbcli_file_put(psmbcli, plocalpath, premotepath);
      break;
    case NETWIB_PATHSTAT_TYPE_DIR :
      ret = netwox_smbcli_dirrec_put(psmbcli, plocalpath, premotepath);
      break;
    default :
      break;
    }
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }

  /* close */
 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(psmbcli->pbufpool, &plocalpath));
  netwib_er(netwib_bufpool_buf_close(psmbcli->pbufpool, &premotepath));
  netwib_er(netwib_bufpool_buf_close(psmbcli->pbufpool, &ppathname));
  netwib_er(netwib_dir_close(&pdir));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_dirrec_del(netwox_smbcli *psmbcli,
                                    netwib_constbuf *pdirname)
{
  netwib_ring *pring;
  netwib_ring_index *pringindex;
  netwox_smb_dirlist *pdirlist;
  netwib_ptr pitem;
  netwib_buf *ppath;
  netwib_pathtype pathtype;
  netwib_err ret;

  if (psmbcli->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbcli_dirrec_del of %{buf}\n", pdirname));
  }

  /* init */
  ret = NETWIB_ERR_OK;
  netwib_er(netwox_smb_dirlist_ring_init(&pring));
  netwib_er(netwib_ring_index_init(pring, &pringindex));
  netwib_er(netwib_bufpool_buf_init(psmbcli->pbufpool, &ppath));

  /* loop for each file in remote dir */
  netwib_eg(netwox_smbcli_dir_ls(psmbcli, pdirname, pring));
  while(NETWIB_TRUE) {
    /* get remote filename */
    ret = netwib_ring_index_next(pringindex, &pitem);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    pdirlist = (netwox_smb_dirlist *)pitem;
    /* append this name to path */
    netwib__buf_reinit(ppath);
    netwib_er(netwib_buf_append_buf(pdirname, ppath));
    netwib_er(netwib_buf_append_byte('/', ppath));
    netwib_er(netwib_buf_append_buf(&pdirlist->pathname, ppath));
    if (psmbcli->verbose) {
      netwib_er(netwib_fmt_display("Path %{buf}\n", ppath));
    }
    /* delete remote file depending on its type */
    switch(pdirlist->pathstat.type) {
    case NETWIB_PATHSTAT_TYPE_REG :
      ret = netwox_smbcli_file_delete(psmbcli, ppath);
      break;
    case NETWIB_PATHSTAT_TYPE_DIR :
      ret = netwox_smbcli_dirrec_del(psmbcli, ppath);
      break;
    default :
      break;
    }
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }

  /* remove remote dir */
  if (ret == NETWIB_ERR_OK) {
    netwib_eg(netwib_pathtype_init(pdirname, &pathtype));
    if (!(pathtype & NETWIB_PATHTYPE_ROOT)) {
      ret = netwox_smbcli_dir_delete(psmbcli, pdirname);
    }
  }

  /* close */
 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(psmbcli->pbufpool, &ppath));
  netwib_er(netwib_ring_index_close(&pringindex));
  netwib_er(netwox_smb_dirlist_ring_close(&pring));
  return(ret);
}
