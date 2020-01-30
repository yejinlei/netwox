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
netwib_err netwox_smtpserver_init(netwox_smtpserver *psmtpserver)
{
  /* those can be changed by users */
  psmtpserver->timeoutms = 180000;
  netwib_er(netwib_buf_init_ext_storage(&psmtpserver->maildir));
  psmtpserver->pallowedclients = NULL;

  /* internal */
  netwib_er(netwib_bufpool_init(NETWIB_TRUE, &psmtpserver->pbufpool));
  psmtpserver->mailnum = 1;
  netwib_er(netwib_thread_mutex_init(&psmtpserver->pmailnummutex));

  /* others have to be externally set */

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smtpserver_close(netwox_smtpserver *psmtpserver)
{
  netwib_er(netwib_bufpool_close(&psmtpserver->pbufpool));
  netwib_er(netwib_thread_mutex_close(&psmtpserver->pmailnummutex));

  netwib_er(netwib_buf_close(&psmtpserver->maildir));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smtpserver *pcommon;
  netwib_io *pio;
  netwib_bool insidedata;
  netwib_bool continuetoloop;
} netwox_priv_smtpserver;

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_smtpserver_session_data(netwox_priv_smtpserver *psmtp,
                                                      netwib_constbuf *pline)
{
  netwib_data data;
  netwib_uint32 datasize;

  datasize = netwib__buf_ref_data_size(pline);
  if (datasize == 1) {
    data = netwib__buf_ref_data_ptr(pline);
    if (*data == '.') {
      netwib_er(netwox_txtproto_write_reply_text(psmtp->pio,
                                                 NETWOX_SMTPREPLY_250_OK,
                                                 "Ok."));
      psmtp->insidedata = NETWIB_FALSE;
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_smtpserver_session_cmd(netwox_priv_smtpserver *psmtp,
                                                     netwib_constbuf *pline)
{
  netwox_smtpcmd smtpcmd;
  netwib_buf cmd, para;
  netwib_err ret;

  ret = netwox_ftpser_decode_line(pline, &cmd, &para);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_txtproto_write_reply_text(psmtp->pio,
                                      NETWOX_SMTPREPLY_500_SYNTAXERROR,
                                               "Syntax error."));
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwox_smtpcmd_init_buf(&cmd, &smtpcmd));
  switch(smtpcmd) {
  case NETWOX_SMTPCMD_EHLO :
  case NETWOX_SMTPCMD_HELO :
    netwib_er(netwox_txtproto_write_reply_text(psmtp->pio,
                                               NETWOX_SMTPREPLY_250_OK,
                                               "server"));
    break;
  case NETWOX_SMTPCMD_MAIL :
  case NETWOX_SMTPCMD_RCPT :
  case NETWOX_SMTPCMD_RSET :
  case NETWOX_SMTPCMD_NOOP :
    netwib_er(netwox_txtproto_write_reply_text(psmtp->pio,
                                               NETWOX_SMTPREPLY_250_OK,
                                               "Ok."));
    break;
  case NETWOX_SMTPCMD_DATA :
    netwib_er(netwox_txtproto_write_reply_text(psmtp->pio,
                                               NETWOX_SMTPREPLY_354_OKSENDDATA,
                                             "Send data ending with a dot."));
    psmtp->insidedata = NETWIB_TRUE;
    break;
  case NETWOX_SMTPCMD_VRFY :
  case NETWOX_SMTPCMD_EXPN :
    netwib_er(netwox_txtproto_write_reply_text(psmtp->pio,
                                               NETWOX_SMTPREPLY_252_OKCANTVRFY,
                                               "Cannot verify."));
    break;
  case NETWOX_SMTPCMD_HELP :
    netwib_er(netwox_txtproto_write_reply_text(psmtp->pio,
                                               NETWOX_SMTPREPLY_214_HELP,
                                               "No help available."));
    break;
  case NETWOX_SMTPCMD_QUIT :
    netwib_er(netwox_txtproto_write_reply_text(psmtp->pio,
                                              NETWOX_SMTPREPLY_221_SVCCLOSING ,
                                               "Bye."));
    psmtp->continuetoloop = NETWIB_FALSE;
    break;
  default :
    netwib_er(netwox_txtproto_write_reply_text(psmtp->pio,
                                       NETWOX_SMTPREPLY_502_NOTIMPLEMENTED,
                                               "Command not implemented."));
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_smtpserver_session(netwox_priv_smtpserver *psmtp)
{
  netwib_buf *pline, *pfilename, *pfilename2, crlf;
  netwib_io *pfileio;
  netwib_uint32 mailnum;
  netwib_err ret=NETWIB_ERR_OK;

  /* open file in maildir */
  netwib_er(netwib_thread_mutex_lock(psmtp->pcommon->pmailnummutex,
                                     NETWIB_TIME_INFINITE, NULL));
  mailnum = psmtp->pcommon->mailnum;
  psmtp->pcommon->mailnum++;
  netwib_er(netwib_thread_mutex_unlock(psmtp->pcommon->pmailnummutex));
  netwib_er(netwib_bufpool_buf_init(psmtp->pcommon->pbufpool, &pfilename));
  netwib_er(netwib_bufpool_buf_init(psmtp->pcommon->pbufpool, &pfilename2));
  netwib_er(netwib_buf_append_fmt(pfilename, "msg%{uint32}.txt", mailnum));
  netwib_er(netwib_path_init_concat(&psmtp->pcommon->maildir, pfilename,
                                    pfilename2));
  netwib_er(netwib_io_init_file_write(pfilename2, &pfileio));
  netwib_er(netwib_bufpool_buf_close(psmtp->pcommon->pbufpool, &pfilename2));
  netwib_er(netwib_bufpool_buf_close(psmtp->pcommon->pbufpool, &pfilename));

  /* main loop */
  netwib_er(netwib_bufpool_buf_init(psmtp->pcommon->pbufpool, &pline));
  netwib_er(netwib_buf_init_ext_string("\r\n", &crlf));
  psmtp->continuetoloop = NETWIB_TRUE;
  while(psmtp->continuetoloop) {

    /* read query */
    netwib__buf_reinit(pline);
    ret = netwox_txtproto_read_query(psmtp->pio, psmtp->pcommon->timeoutms,
                                     pline);
    if (ret != NETWIB_ERR_OK) {
      break;
    }

    /* read commands or mail data */
    netwib_er(netwib_io_write(pfileio, pline));
    netwib_er(netwib_io_write(pfileio, &crlf));
    if (psmtp->insidedata) {
      ret = netwox_priv_smtpserver_session_data(psmtp, pline);
    } else {
      ret = netwox_priv_smtpserver_session_cmd(psmtp, pline);
    }
    if (ret != NETWIB_ERR_OK) {
      break;
    }

  }
  netwib_er(netwib_bufpool_buf_close(psmtp->pcommon->pbufpool, &pline));

  netwib_er(netwib_io_close(&pfileio));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smtpserver_session2(netwox_priv_smtpserver *psmtp)
{
  netwib_ip remoteip;
  netwib_bool allowed;

  allowed = NETWIB_TRUE;
  if (psmtp->pcommon->pallowedclients != NULL) {
    netwib_er(netwib_sock_ctl_get_remote(psmtp->pio, &remoteip, NULL));
    netwib_er(netwib_ips_contains_ip(psmtp->pcommon->pallowedclients,
                                     &remoteip, &allowed));
  }

  if (!allowed) {
    netwib_er(netwox_txtproto_write_reply_text(psmtp->pio,
                                       NETWOX_SMTPREPLY_554_TRANSACTIONFAILED,
                                            "Your IP address is forbidden"));
    netwib_er(netwib_fmt_display("IP address %{ip} is not allowed\n",
                                 &remoteip));
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwox_txtproto_write_reply_text(psmtp->pio,
                                             NETWOX_SMTPREPLY_220_SVCREADY,
                                             "Welcome"));
  netwib_er(netwox_priv_smtpserver_session(psmtp));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smtpserver_session(netwib_io *psockio,
                                     netwib_ptr pinfos)
{
  netwox_priv_smtpserver smtpserver;
  netwib_err ret;

  /* prepare variables */
  smtpserver.pcommon = (netwox_smtpserver *)pinfos;
  netwib_er(netwox_txtproto_io_rdwr(psockio, NETWIB_TRUE, &smtpserver.pio));
  smtpserver.insidedata = NETWIB_FALSE;

  /* main */
  ret = netwox_smtpserver_session2(&smtpserver);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_err_display(ret, NETWIB_ERR_ENCODETYPE_TEXT));
  }

  /* close */
  netwib_er(netwib_io_unplug_next_supported(smtpserver.pio, NULL));
  netwib_er(netwib_io_close(&smtpserver.pio));

  return(NETWIB_ERR_OK);
}
