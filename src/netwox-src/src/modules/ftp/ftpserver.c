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
netwib_err netwox_ftpserver_init(netwox_ftpserver *pftpserver)
{
  /* those can be changed by users */
  pftpserver->timeoutms = 180000;
  netwib_er(netwib_buf_init_ext_storage(&pftpserver->login));
  pftpserver->login.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  netwib_er(netwib_buf_init_ext_storage(&pftpserver->password));
  pftpserver->password.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  netwib_er(netwib_buf_init_ext_storage(&pftpserver->rootdir));
  netwib_er(netwib_buf_append_string(".", &pftpserver->rootdir));
  pftpserver->pallowedclients = NULL;
  pftpserver->allowput = NETWIB_TRUE;

  /* internal */
  netwib_er(netwib_bufpool_init(NETWIB_TRUE, &pftpserver->pbufpool));

  /* others have to be externally set */

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpserver_close(netwox_ftpserver *pftpserver)
{
  netwib_er(netwib_bufpool_close(&pftpserver->pbufpool));

  netwib_er(netwib_buf_close(&pftpserver->login));
  netwib_er(netwib_buf_close(&pftpserver->password));
  netwib_er(netwib_buf_close(&pftpserver->rootdir));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
typedef struct {
  netwox_constftpserver *pcommon;
  netwib_io *pctlio;
  netwib_bool userok;
  netwib_bool passwordok;
  netwib_buf *pcwd;
  netwib_buf *prnfr;
  netwib_bool ascii;
  netwib_bool active;
  netwib_io *pdataio;
} netwox_priv_ftpserver;

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_ftpserver_session(netwox_priv_ftpserver *pftp)
{
  netwib_buf *pline, cmd, para;
  netwox_ftpcmd ftpcmd;
  netwib_ip remoteip;
  netwib_uint32 authcount;
  netwib_bool continuetoloop, allowed;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_bufpool_buf_init(pftp->pcommon->pbufpool, &pline));

  authcount = 0;
  continuetoloop = NETWIB_TRUE;
  while(continuetoloop) {

    /* read query */
    netwib__buf_reinit(pline);
    ret = netwox_txtproto_read_query(pftp->pctlio, pftp->pcommon->timeoutms,
                                     pline);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    ret = netwox_ftpser_decode_line(pline, &cmd, &para);
    if (ret != NETWIB_ERR_OK) {
       netwib_er(netwox_txtproto_write_reply_text(pftp->pctlio,
                                        NETWOX_FTPREPLY_500_SYNTAXERROR,
                                                  "Syntax error."));
       continue;
    }
    netwib_er(netwox_ftpcmd_init_buf(&cmd, &ftpcmd));
    /*netwib_er(netwib_fmt_display("CMD=%{buf} %{buf} %{uint32}\n", &cmd,
      &para, ftpcmd));*/

    /* check if authentication is needed */
    if (!pftp->userok || !pftp->passwordok) {
      switch(ftpcmd) {
      case NETWOX_FTPCMD_USER :
      case NETWOX_FTPCMD_PASS :
      case NETWOX_FTPCMD_QUIT :
        break;
      default :
        netwib_er(netwox_txtproto_write_reply_text(pftp->pctlio,
                                                   NETWOX_FTPREPLY_530_AUTHBAD,
                                                   "Not logged in."));
        continue;
      }
    }

    /* check if writing is allowed */
    if (!pftp->pcommon->allowput) {
      switch(ftpcmd) {
      case NETWOX_FTPCMD_UNKNOWN :
      case NETWOX_FTPCMD_USER :
      case NETWOX_FTPCMD_PASS :
      case NETWOX_FTPCMD_ACCT :
      case NETWOX_FTPCMD_CWD :
      case NETWOX_FTPCMD_CDUP :
      case NETWOX_FTPCMD_SMNT :
      case NETWOX_FTPCMD_REIN :
      case NETWOX_FTPCMD_QUIT :
      case NETWOX_FTPCMD_PORT :
      case NETWOX_FTPCMD_PASV :
      case NETWOX_FTPCMD_TYPE :
      case NETWOX_FTPCMD_STRU :
      case NETWOX_FTPCMD_MODE :
      case NETWOX_FTPCMD_RETR :
      case NETWOX_FTPCMD_REST :
      case NETWOX_FTPCMD_ABOR :
      case NETWOX_FTPCMD_PWD :
      case NETWOX_FTPCMD_LIST :
      case NETWOX_FTPCMD_NLST :
      case NETWOX_FTPCMD_SITE :
      case NETWOX_FTPCMD_SYST :
      case NETWOX_FTPCMD_STAT :
      case NETWOX_FTPCMD_HELP :
      case NETWOX_FTPCMD_NOOP :
      case NETWOX_FTPCMD_SIZE :
      case NETWOX_FTPCMD_EPRT :
      case NETWOX_FTPCMD_EPSV :
      case NETWOX_FTPCMD_ALLO :
        break;
      case NETWOX_FTPCMD_STOR :
      case NETWOX_FTPCMD_STOU :
      case NETWOX_FTPCMD_APPE :
      case NETWOX_FTPCMD_RNTO :
        netwib_er(netwox_txtproto_write_reply_text(pftp->pctlio,
                                         NETWOX_FTPREPLY_532_FILECANTSTORE,
                                                   "Not allowed to write."));
        continue;
        break;
      case NETWOX_FTPCMD_DELE :
      case NETWOX_FTPCMD_RNFR :
        netwib_er(netwox_txtproto_write_reply_text(pftp->pctlio,
                                       NETWOX_FTPREPLY_450_FILEACTIONNOTTAKEN,
                                                   "Not allowed to write."));
        continue;
        break;
      case NETWOX_FTPCMD_RMD :
      case NETWOX_FTPCMD_MKD :
      default:
        netwib_er(netwox_txtproto_write_reply_text(pftp->pctlio,
                                       NETWOX_FTPREPLY_550_ACTIONNOTTAKEN,
                                                   "Not allowed to write."));
        continue;
        break;
      }
    }

    /* check brute force attacks (for example, do not allow user change) */
    if (ftpcmd == NETWOX_FTPCMD_USER || ftpcmd == NETWOX_FTPCMD_PASS) {
      authcount++;
      if (authcount > 2) {
        netwib_er(netwox_txtproto_write_reply_text(pftp->pctlio,
                                           NETWOX_FTPREPLY_421_SVCNOTAVAILABLE,
                                           "Too many authentications tried."));
        break;
      }
    }

    /* deal with dataio */
    switch(ftpcmd) {
    case NETWOX_FTPCMD_QUIT :
    case NETWOX_FTPCMD_PORT :
    case NETWOX_FTPCMD_PASV :
    case NETWOX_FTPCMD_EPRT :
    case NETWOX_FTPCMD_EPSV :
      /* if dataio was previously open, close it */
      if (pftp->pdataio != NULL) {
        netwib_er(netwib_io_close(&pftp->pdataio));
      }
      break;
    case NETWOX_FTPCMD_RETR :
    case NETWOX_FTPCMD_STOR :
    case NETWOX_FTPCMD_LIST :
    case NETWOX_FTPCMD_NLST :
      /* dataio need to be open */
      if (pftp->pdataio == NULL) {
        netwib_er(netwox_txtproto_write_reply_text(pftp->pctlio,
                               NETWOX_FTPREPLY_425_CANTOPENDATA,
                               "Please use PORT, PASV, EPRT or EPSV before."));
        continue;
      }
      /* check client (already done in active mode) */
      if (!pftp->active && pftp->pcommon->pallowedclients != NULL) {
        netwib_er(netwib_io_wait_read(pftp->pdataio, NETWIB_TIME_ZERO, NULL));
        ret = netwib_sock_ctl_get_remote(pftp->pdataio, &remoteip, NULL);
        if (ret != NETWIB_ERR_OK) {
          netwib_er(netwox_txtproto_write_reply_text(pftp->pctlio,
                               NETWOX_FTPREPLY_425_CANTOPENDATA,
              "No client is connected on data port."));
          continue;
        }
        netwib_er(netwib_ips_contains_ip(pftp->pcommon->pallowedclients,
                                         &remoteip, &allowed));
        if (!allowed) {
          netwib_er(netwox_txtproto_write_reply_text(pftp->pctlio,
                               NETWOX_FTPREPLY_425_CANTOPENDATA,
              "IP address of client connected on data port is not allowed."));
          netwib_er(netwib_io_close(&pftp->pdataio));
          continue;
        }
      }
      break;
    default:
      break;
    }

    /* execute command (do not check, for command needing no parameter, if
       para was set. Just ignore it) */
    switch(ftpcmd) {
    case NETWOX_FTPCMD_USER :
      netwib_er(netwox_ftpser_user(pftp->pctlio, &pftp->pcommon->login,
                                   pftp->passwordok, &para, &pftp->userok));
      break;
    case NETWOX_FTPCMD_PASS :
      netwib_er(netwox_ftpser_pass(pftp->pctlio, pftp->userok,
                                   &pftp->pcommon->password, &para,
                                   &pftp->passwordok));
      break;
    case NETWOX_FTPCMD_CWD :
      netwib_er(netwox_ftpser_cwd(pftp->pcommon->pbufpool,
                                  &pftp->pcommon->rootdir, pftp->pcwd,
                                  pftp->pctlio, &para));
      break;
    case NETWOX_FTPCMD_CDUP :
      netwib_er(netwox_ftpser_cdup(pftp->pcommon->pbufpool,
                                   &pftp->pcommon->rootdir, pftp->pcwd,
                                   pftp->pctlio));
      break;
    case NETWOX_FTPCMD_QUIT :
      netwib_er(netwox_ftpser_quit(pftp->pctlio));
      continuetoloop = NETWIB_FALSE;
      break;
    case NETWOX_FTPCMD_SYST :
      netwib_er(netwox_ftpser_syst(pftp->pctlio));
      break;
    case NETWOX_FTPCMD_PORT :
      netwib_er(netwox_ftpser_port(pftp->pcommon->pmainsockinfo,
                                   pftp->pcommon->pallowedclients,
                                   &para, pftp->pctlio, &pftp->pdataio));
      pftp->active = NETWIB_TRUE;
      break;
    case NETWOX_FTPCMD_PASV :
      netwib_er(netwox_ftpser_pasv(pftp->pcommon->pbufpool,
                                   pftp->pcommon->pmainsockinfo,
                                   pftp->pctlio, &pftp->pdataio));
      pftp->active = NETWIB_FALSE;
      break;
    case NETWOX_FTPCMD_TYPE :
      netwib_er(netwox_ftpser_type(pftp->pctlio, &para, &pftp->ascii));
      break;
    case NETWOX_FTPCMD_STRU :
      netwib_er(netwox_ftpser_stru(pftp->pctlio, &para));
      break;
    case NETWOX_FTPCMD_MODE :
      netwib_er(netwox_ftpser_mode(pftp->pctlio, &para));
      break;
    case NETWOX_FTPCMD_RETR :
      netwib_er(netwox_ftpser_retr(pftp->pcommon->pbufpool,
                                   &pftp->pcommon->rootdir, pftp->pcwd,
                                   pftp->pctlio, &pftp->pdataio, &para,
                                   pftp->ascii));
      break;
    case NETWOX_FTPCMD_STOR :
      netwib_er(netwox_ftpser_stor(pftp->pcommon->pbufpool,
                                   &pftp->pcommon->rootdir, pftp->pcwd,
                                   pftp->pctlio, &pftp->pdataio, &para,
                                   pftp->ascii));
      break;
    case NETWOX_FTPCMD_RNFR :
      netwib_er(netwox_ftpser_rnfr(pftp->pcommon->pbufpool,
                                   &pftp->pcommon->rootdir, pftp->pcwd,
                                   pftp->prnfr, pftp->pctlio, &para));
      break;
    case NETWOX_FTPCMD_RNTO :
      netwib_er(netwox_ftpser_rnto(pftp->pcommon->pbufpool,
                                   &pftp->pcommon->rootdir, pftp->pcwd,
                                   pftp->prnfr, pftp->pctlio, &para));
      break;
    case NETWOX_FTPCMD_DELE :
      netwib_er(netwox_ftpser_dele(pftp->pcommon->pbufpool,
                                   &pftp->pcommon->rootdir, pftp->pcwd,
                                   pftp->pctlio, &para));
      break;
    case NETWOX_FTPCMD_RMD :
      netwib_er(netwox_ftpser_rmd(pftp->pcommon->pbufpool,
                                  &pftp->pcommon->rootdir, pftp->pcwd,
                                  pftp->pctlio, &para));
      break;
    case NETWOX_FTPCMD_MKD :
      netwib_er(netwox_ftpser_mkd(pftp->pcommon->pbufpool,
                                  &pftp->pcommon->rootdir, pftp->pcwd,
                                  pftp->pctlio, &para));
      break;
    case NETWOX_FTPCMD_PWD :
      netwib_er(netwox_ftpser_pwd(pftp->pcommon->pbufpool,
                                  &pftp->pcommon->rootdir, pftp->pcwd,
                                  pftp->pctlio));
      break;
    case NETWOX_FTPCMD_LIST :
      netwib_er(netwox_ftpser_list(pftp->pcommon->pbufpool,
                                   &pftp->pcommon->rootdir, pftp->pcwd,
                                   pftp->pctlio, &pftp->pdataio, &para,
                                   NETWIB_TRUE));
      break;
    case NETWOX_FTPCMD_NLST :
      netwib_er(netwox_ftpser_list(pftp->pcommon->pbufpool,
                                   &pftp->pcommon->rootdir, pftp->pcwd,
                                   pftp->pctlio, &pftp->pdataio, &para,
                                   NETWIB_FALSE));
      break;
    case NETWOX_FTPCMD_NOOP :
      netwib_er(netwox_ftpser_noop(pftp->pctlio));
      break;
    case NETWOX_FTPCMD_SIZE :
      netwib_er(netwox_ftpser_size(pftp->pcommon->pbufpool,
                                   &pftp->pcommon->rootdir, pftp->pcwd,
                                   pftp->pctlio, &para));
      break;
    case NETWOX_FTPCMD_EPRT :
      netwib_er(netwox_ftpser_eprt(pftp->pcommon->pmainsockinfo,
                                   pftp->pcommon->pallowedclients,
                                   &para, pftp->pctlio, &pftp->pdataio));
      pftp->active = NETWIB_TRUE;
      break;
    case NETWOX_FTPCMD_EPSV :
      netwib_er(netwox_ftpser_epsv(pftp->pcommon->pbufpool,
                                   pftp->pcommon->pmainsockinfo,
                                   pftp->pctlio, &pftp->pdataio));
      pftp->active = NETWIB_FALSE;
      break;
    default :
      netwib_er(netwox_txtproto_write_reply_text(pftp->pctlio,
                                   NETWOX_FTPREPLY_502_CMDNOTIMPLEMENTED,
                                                 "Command not implemented."));
      break;
    }

  }

  if (pftp->pdataio != NULL) {
    netwib_er(netwib_io_close(&pftp->pdataio));
  }
  netwib_er(netwib_bufpool_buf_close(pftp->pcommon->pbufpool, &pline));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_ftpserver_session2(netwib_io *psockio,
                                            netwox_priv_ftpserver *pftp)
{
  netwib_ip remoteip;
  netwib_port remoteport;
  netwib_bool allowed;

  allowed = NETWIB_TRUE;
  if (pftp->pcommon->pallowedclients != NULL) {
    netwib_er(netwib_sock_ctl_get_remote(psockio, &remoteip, &remoteport));
    netwib_er(netwib_ips_contains_ip(pftp->pcommon->pallowedclients,
                                     &remoteip, &allowed));
  }

  if (!allowed) {
    netwib_er(netwox_txtproto_write_reply_text(pftp->pctlio,
                                      NETWOX_FTPREPLY_421_SVCNOTAVAILABLE,
                                            "Your IP address is forbidden"));
    netwib_er(netwib_fmt_display("IP address %{ip} is not allowed\n",
                                 &remoteip));
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwox_txtproto_write_reply_text(pftp->pctlio,
                                             NETWOX_FTPREPLY_220_SVCREADY,
                                             "Welcome"));
  netwib_er(netwox_priv_ftpserver_session(pftp));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpserver_session(netwib_io *psockio,
                                    netwib_ptr pinfos)
{
  netwox_priv_ftpserver ftpserver;
  netwib_err ret;

  /* prepare variables */
  ftpserver.pcommon = (netwox_constftpserver *)pinfos;
  netwib_er(netwox_txtproto_io_rdwr(psockio, NETWIB_TRUE,
                                    &ftpserver.pctlio));
  ftpserver.userok = NETWIB_FALSE;
  ftpserver.passwordok = NETWIB_FALSE;
  if (netwib__buf_ref_data_size(&ftpserver.pcommon->login) == 0) {
    ftpserver.userok = NETWIB_TRUE;
  }
  if (netwib__buf_ref_data_size(&ftpserver.pcommon->password) == 0) {
    ftpserver.passwordok = NETWIB_TRUE;
  }
  netwib_er(netwib_bufpool_buf_init(ftpserver.pcommon->pbufpool,
                                    &ftpserver.pcwd));
  netwib_er(netwib_buf_append_string("/", ftpserver.pcwd));
  netwib_er(netwib_bufpool_buf_init(ftpserver.pcommon->pbufpool,
                                    &ftpserver.prnfr));
  /* rfc 959 3.1.1.1 says ASCII is the default */
  ftpserver.ascii = NETWIB_TRUE;
  ftpserver.pdataio = NULL;

  /* main */
  ret = netwox_ftpserver_session2(psockio, &ftpserver);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_err_display(ret, NETWIB_ERR_ENCODETYPE_TEXT));
  }

  /* close */
  netwib_er(netwib_bufpool_buf_close(ftpserver.pcommon->pbufpool,
                                     &ftpserver.prnfr));
  netwib_er(netwib_bufpool_buf_close(ftpserver.pcommon->pbufpool,
                                     &ftpserver.pcwd));
  netwib_er(netwib_io_unplug_next_supported(ftpserver.pctlio, NULL));
  netwib_er(netwib_io_close(&ftpserver.pctlio));

  return(NETWIB_ERR_OK);
}
