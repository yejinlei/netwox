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
netwib_err netwox_smbser_init(netwox_constsmbserver *pcommon,
                              netwib_io *pio,
                              netwox_smbser *psmbser)
{

  psmbser->pcommon = pcommon;
  psmbser->pio = pio;

  psmbser->negotiated = NETWIB_FALSE;
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool,
                                    &psmbser->pbufrecv));
  psmbser->pbufrecv->flags |= NETWIB_BUF_FLAGS_CANSLIDE;
  psmbser->smberrcmd = NETWOX_SMBMSG_CMD_ENDOFLIST;
  psmbser->smberrtosend = NETWOX_SMBERR_OK;
  netwib_er(netwox_smbserfid_init(&psmbser->smbserfid));
  psmbser->sessiontreecounter = 1;
  netwib_er(netwox_smbser83_init(psmbser->pcommon->pbufpool,
                                 &psmbser->smbser83));
  netwib_er(netwox_smbserglob_init(psmbser->pcommon->pbufpool,
                                   &psmbser->smbserglob));

  psmbser->cmn_secumode = NETWOX_SMBCMD_NEGOTIATE_SECUMODE_USERLEVEL | NETWOX_SMBCMD_NEGOTIATE_SECUMODE_CHALRESP;
  psmbser->cmn_maxmsgsize = 32000;
  psmbser->cmn_vcid = 0;
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool,
                                    &psmbser->pcmn_challenge));

  netwib_er(netwib_array_init(sizeof(netwox_smbser_session), 0, &psmbser->sessions));
  psmbser->currentsession = NETWOX_SMBSER_CURRENTSESSION_NOTFOUND;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbser_close(netwox_smbser *psmbser)
{
  netwox_smbser_session *psession;
  netwib_uint32 i;

  for (i = 0; i < psmbser->sessions.size; i++) {
    psession = (netwox_smbser_session *)psmbser->sessions.p[i];
    netwib_er(netwib_array_close(&psession->trees));
  }
  netwib_er(netwib_array_close(&psmbser->sessions));

  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool,
                                     &psmbser->pcmn_challenge));
  netwib_er(netwox_smbserglob_close(&psmbser->smbserglob));
  netwib_er(netwox_smbser83_close(&psmbser->smbser83));
  netwib_er(netwox_smbserfid_close(&psmbser->smbserfid));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool,
                                     &psmbser->pbufrecv));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbser_session_add(netwox_smbser *psmbser,
                                     netwox_smbser_session **ppsession)
{
  netwox_smbser_session *psession;
  psmbser->currentsession = psmbser->sessions.size;
  netwib_er(netwib_array_ctl_set_size(&psmbser->sessions, psmbser->sessions.size+1));
  psession = (netwox_smbser_session *)psmbser->sessions.p[psmbser->currentsession];
  psession->state = NETWOX_SMBSER_SESSIONTREE_STATE_NOTUSED;
  if (psmbser->sessiontreecounter == 0xFFFF) {
    return(NETWOX_ERR_INTERNALERROR);
  }
  psession->userid = (netwib_uint16)psmbser->sessiontreecounter++;
  psession->capabilities = 0;
  netwib_er(netwib_array_init(sizeof(netwox_smbser_tree), 0,
                              &psession->trees));
  psession->currenttree = NETWOX_SMBSER_CURRENTTREE_NOTFOUND;
  *ppsession = psession;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbser_tree_add(netwox_smbser *psmbser,
                                  netwox_smbser_tree **pptree)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  if (psmbser->currentsession == NETWOX_SMBSER_CURRENTSESSION_NOTFOUND) {
    return(NETWIB_ERR_NOTFOUND);
  }
  psession = (netwox_smbser_session *)psmbser->sessions.p[psmbser->currentsession];
  psession->currenttree = psession->trees.size;
  netwib_er(netwib_array_ctl_set_size(&psession->trees,
                                      psession->trees.size+1));
  ptree = (netwox_smbser_tree *)psession->trees.p[psession->currenttree];
  ptree->state = NETWOX_SMBSER_SESSIONTREE_STATE_NOTUSED;
  if (psmbser->sessiontreecounter == 0xFFFF) {
    return(NETWOX_ERR_INTERNALERROR);
  }
  ptree->treeid = (netwib_uint16)psmbser->sessiontreecounter++;
  ptree->isipc = NETWIB_FALSE;
  *pptree = ptree;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbser_sessiontree_check(netwox_smbser *psmbser,
                                           netwox_smbser_session **ppsession,
                                           netwox_smbser_tree **pptree)
{

  netwib_er(netwox_smbser_sessiontree_get(psmbser, ppsession, pptree));

  /* check session */
  if ((*ppsession)->state != NETWOX_SMBSER_SESSIONTREE_STATE_INUSE) {
    return(NETWOX_ERR_PROTOCOL);
  }

  /* check tree */
  if ((*pptree)->state != NETWOX_SMBSER_SESSIONTREE_STATE_INUSE) {
    return(NETWOX_ERR_PROTOCOL);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbser_sessiontree_get(netwox_smbser *psmbser,
                                         netwox_smbser_session **ppsession,
                                         netwox_smbser_tree **pptree)
{
  netwox_smbser_session *psession;

  /* check session */
  if (psmbser->currentsession == NETWOX_SMBSER_CURRENTSESSION_NOTFOUND) {
    return(NETWOX_ERR_PROTOCOL);
  }
  psession = (netwox_smbser_session *)psmbser->sessions.p[psmbser->currentsession];
  *ppsession = psession;

  /* optionally check tree */
  if (pptree != NULL) {
    if (psession->currenttree == NETWOX_SMBSER_CURRENTTREE_NOTFOUND) {
      return(NETWOX_ERR_PROTOCOL);
    }
    *pptree = (netwox_smbser_tree *)psession->trees.p[psession->currenttree];
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbser_sessiontree_display(netwox_smbser *psmbser)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_uint32 i, j;

  if (psmbser->pcommon->debug) {
    netwib_er(netwib_fmt_display("Sessions and trees:\n"));
    for (i = 0; i < psmbser->sessions.size; i++) {
      psession = (netwox_smbser_session *)psmbser->sessions.p[i];
      for (j = 0; j < psession->trees.size; j++) {
        ptree = (netwox_smbser_tree *)psession->trees.p[j];
        netwib_er(netwib_fmt_display(" userid=%{uint16} treeid=%{uint16} Ustate=%{uint32} Tstate=%{uint32} Tisipc=%{bool}\n", psession->userid, ptree->treeid, psession->state, ptree->state, ptree->isipc));
      }
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbser_smbmsg_setdefault(netwox_smbser *psmbser,
                                           netwox_constsmbmsg *psmbmsgq,
                                           netwox_smbmsg *psmbmsgr)
{
  netwox_smbser_session *psession;

  netwib_er(netwox_smbmsg_setdefault(psmbmsgr));

  /* currently, only support case sensitive filenames
     psmbmsgr->hdr.flags |= NETWOX_SMBMSG_FLAGS_CASELESSPATHNAMES;
  */
  psmbmsgr->hdr.flags |= NETWOX_SMBMSG_FLAGS_REPLY;
  psmbmsgr->hdr.flags2 |= NETWOX_SMBMSG_FLAGS2_ACCEPTSLONGPATHNAME;
  psmbmsgr->hdr.flags2 |= NETWOX_SMBMSG_FLAGS2_USESLONGPATHNAME;
  if (psmbser->currentsession != NETWOX_SMBSER_CURRENTSESSION_NOTFOUND) {
    psession = (netwox_smbser_session *)psmbser->sessions.p[psmbser->currentsession];
    if (psession->capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_NTERROR) {
      psmbmsgr->hdr.flags2 |= NETWOX_SMBMSG_FLAGS2_NTERROR;
    }
    if (psession->capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_UNICODE) {
      psmbmsgr->hdr.flags2 |= NETWOX_SMBMSG_FLAGS2_UNICODESTRINGS;
    }
  }

  /* we simply echo back values. They were verified in recv. In some states,
     treeid and userid may be changed after by functions. */
  psmbmsgr->hdr.processid = psmbmsgq->hdr.processid;
  psmbmsgr->hdr.multiplexid = psmbmsgq->hdr.multiplexid;
  psmbmsgr->hdr.treeid = psmbmsgq->hdr.treeid;
  psmbmsgr->hdr.userid = psmbmsgq->hdr.userid;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbser_smbmsg_send(netwox_smbser *psmbser,
                                     netwox_smbmsg *psmbmsg)
{
  netwib_buf *pbuf;
  netwib_err ret;

  /* show message */
  if (psmbser->pcommon->debug) {
    netwib_er(netwox_smbmsg_display(psmbser->pcommon->pbufpool, psmbmsg));
  }

  /* generate packet */
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pbuf));
  netwib_er(netwox_pkt_append_smbmsghdr_begin(psmbser->pcommon->isnaked,
                                              pbuf));
  netwib_er(netwox_pkt_append_smbmsg(psmbmsg, pbuf));
  netwib_er(netwox_pkt_append_smbmsghdr_end(psmbser->pcommon->isnaked, pbuf));

  /* send it */
  ret = netwib_io_write(psmbser->pio, pbuf);
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pbuf));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbser_smbmsg_recv_nbtss(netwox_smbser *psmbser,
                                                  netwib_uint32 *pskipsize)
{
  netwox_smbnbtss smbnbtssq, smbnbtssr;
  netwib_buf *pbuf;
  netwib_err ret;

  netwib_er(netwox_pkt_decode_smbnbtss(psmbser->pbufrecv, &smbnbtssq,
                                       pskipsize));

  if (smbnbtssq.smbnbtsstype == NETWOX_SMBNBTSSTYPE_KEEPALIVE ||
      smbnbtssq.smbnbtsstype == NETWOX_SMBNBTSSTYPE_KEEPALIVE95) {
    /* nothing to do */
    return(NETWIB_ERR_OK);
  }

  if (smbnbtssq.smbnbtsstype != NETWOX_SMBNBTSSTYPE_REQUEST) {
    /* only deal with request */
    return(NETWIB_ERR_NOTCONVERTED);
  }

  /* always accept session, without checking request values */
  smbnbtssr.smbnbtsstype = NETWOX_SMBNBTSSTYPE_RESPPOSITIVE;
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pbuf));
  netwib_er(netwox_pkt_append_smbnbtss(&smbnbtssr, pbuf));
  ret = netwib_io_write(psmbser->pio, pbuf);
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pbuf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbser_smbmsg_recv(netwox_smbser *psmbser,
                                     netwox_smbmsg *psmbmsg)
{
  netwib_time t;
  netwib_bool event, tryread;
  netwib_bufext smbmsgbuf;
  netwib_uint32 skipsize, i, j;
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_plus_msec(&t, psmbser->pcommon->maxwaitms));
  tryread = NETWIB_FALSE;
  while (NETWIB_TRUE) {
    if (tryread) {
      netwib_er(netwib_io_wait_read(psmbser->pio, &t, &event));
      if (!event) {
        ret = NETWOX_ERR_TIMEOUT;
        break;
      }
      ret = netwib_io_read(psmbser->pio, psmbser->pbufrecv);
      if (ret == NETWIB_ERR_DATANOTAVAIL) {
        ret = NETWIB_ERR_OK;
        continue;
      } else if (ret != NETWIB_ERR_OK) {
        break;
      }
    }
    tryread = NETWIB_TRUE;
    if (!psmbser->pcommon->isnaked) {
      /* special nbtss messages : treat then skip */
      ret = netwox_smbser_smbmsg_recv_nbtss(psmbser, &skipsize);
      if (ret == NETWIB_ERR_OK) {
        psmbser->pbufrecv->beginoffset += skipsize;
        continue;
      }
    }
    ret = netwox_pkt_decode_smbmsghdr(psmbser->pbufrecv,
                                      psmbser->pcommon->isnaked,
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
    psmbser->pbufrecv->beginoffset += skipsize;
    break;
  }

  psmbser->currentsession = NETWOX_SMBSER_CURRENTSESSION_NOTFOUND;
  psmbser->currentsession_beforentbug = NETWOX_SMBSER_CURRENTSESSION_NOTFOUND;
  if (ret == NETWIB_ERR_OK) {
    /* show message */
    if (psmbser->pcommon->debug) {
      netwib_er(netwox_smbmsg_display(psmbser->pcommon->pbufpool, psmbmsg));
    }
    /* check values */
    if (psmbmsg->hdr.error != 0) {
      return(NETWOX_ERR_PROTOCOL);
    }
    if (psmbser->negotiated) {
      netwib_eg(netwox_smbser_sessiontree_display(psmbser));
      /* search current session */
      for (i = 0; i < psmbser->sessions.size; i++) {
        psession = (netwox_smbser_session *)psmbser->sessions.p[i];
        psession->currenttree = NETWOX_SMBSER_CURRENTTREE_NOTFOUND;
        /* authenticated AND with a valid userid */
        if (psession->userid == psmbmsg->hdr.userid) {
          psmbser->currentsession = i;
          for (j = 0; j < psession->trees.size; j++) {
            ptree = (netwox_smbser_tree *)psession->trees.p[j];
            if (ptree->treeid == psmbmsg->hdr.treeid) {
              psession->currenttree = j;
              /* use goto to go directly to end */
              netwib_goto(NETWIB_ERR_OK);
            }
          }
        }
      }
      /* Windows NT sometimes uses the first userid instead of the
         one the treeid was created with. For example, if
           userid=1
            treeid=2
           userid=3
            treeid=4
           Windows NT uses userid=1(instead of 3) and treeid=4
         Note: CIFS-TR-1p00_FINAL.pdf documentation allows to use several
               userid for the same real user. So NT is bugged. If we used
               only one userid, then there is a bug in Windows 2000: it
               might logoff user and continue to use previously opened trees.
         We try to find the good choice: userid=3 and treeid=4.
         If not found, we don't change currentsession nor currenttree.
         Note: we don't have conflicts, because psmbser->sessiontreecounter
               is incremented whatever the userid is.
      */
      if (psmbser->currentsession != NETWOX_SMBSER_CURRENTTREE_NOTFOUND) {
        for (i = 0; i < psmbser->sessions.size; i++) {
          psession = (netwox_smbser_session *)psmbser->sessions.p[i];
          for (j = 0; j < psession->trees.size; j++) {
            ptree = (netwox_smbser_tree *)psession->trees.p[j];
            if (ptree->treeid == psmbmsg->hdr.treeid) {
              psmbser->currentsession_beforentbug = psmbser->currentsession;
              psmbser->currentsession = i;
              psession->currenttree = j;
              /* use goto to go directly to end */
              netwib_goto(NETWIB_ERR_OK);
            }
          }
        }
      }
    }
  }

 netwib_gotolabel:
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbser_send_begin(netwox_smbser *psmbser,
                                    netwox_constsmbmsg *psmbmsgq,
                                    netwox_smbmsg *psmbmsgr,
                                    netwox_smbcmd *psmbcmdr,
                                    netwox_smbcmd_type type)
{
  netwib_er(netwox_smbmsg_init(psmbmsgr));
  netwib_er(netwox_smbser_smbmsg_setdefault(psmbser, psmbmsgq, psmbmsgr));
  netwib_er(netwox_smbcmd_init(psmbcmdr));
  netwib_er(netwox_smbcmd_selecttype(psmbcmdr, type));
  netwib_er(netwox_smbcmd_setdefault(psmbcmdr));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbser_send_end(netwox_smbser *psmbser,
                                  netwox_smbmsg *psmbmsgr,
                                  netwox_smbcmd *psmbcmdr)
{
  netwib_er(netwox_smbmsg_append_smbcmd(psmbcmdr, psmbmsgr));
  netwib_er(netwox_smbcmd_close(psmbcmdr));
  netwib_er(netwox_smbser_smbmsg_send(psmbser, psmbmsgr));
  netwib_er(netwox_smbmsg_close(psmbmsgr));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbser_nottested(netwox_smbser *psmbser)
{
  netwib_uint32 i;
  if (psmbser->pcommon->debug) {
    for (i = 0; i < 8; i++) {
      puts("################################################################");
    }
    puts("############################# NOT TESTED #######################");
    for (i = 0; i < 8; i++) {
      puts("################################################################");
    }
    netwib_er(netwib_beep());
    netwib_er(netwib_time_sleep_sec(1));
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbser_notimplemented(netwox_smbser *psmbser)
{
  netwib_uint32 i;
  if (psmbser->pcommon->debug) {
    for (i = 0; i < 8; i++) {
      puts("################################################################");
    }
    puts("######################### NOT IMPLEMENTED ######################");
    for (i = 0; i < 8; i++) {
      puts("################################################################");
    }
    netwib_er(netwib_beep());
    netwib_er(netwib_time_sleep_sec(1));
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbser_internalerror(netwox_smbser *psmbser)
{
  netwib_uint32 i;
  if (psmbser->pcommon->debug) {
    for (i = 0; i < 8; i++) {
      puts("################################################################");
    }
    puts("######################### INTERNAL ERROR #######################");
    for (i = 0; i < 8; i++) {
      puts("################################################################");
    }
    netwib_er(netwib_beep());
    netwib_er(netwib_time_sleep_sec(1));
  }
  return(NETWIB_ERR_OK);
}
