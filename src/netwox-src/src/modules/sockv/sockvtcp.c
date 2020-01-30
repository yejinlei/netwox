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
#define NETWOX_SOCKV_TCP_STATEMACHINE_DEBUG 0

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SOCKV_TCPSTATE_LISTEN_INIT = 1,
  NETWOX_SOCKV_TCPSTATE_SYNSENT_INIT,
  NETWOX_SOCKV_TCPSTATE_SYNRECEIVED_INIT,
  NETWOX_SOCKV_TCPSTATE_ESTABLISHED_INIT,
  NETWOX_SOCKV_TCPSTATE_FINWAIT1_INIT,
  NETWOX_SOCKV_TCPSTATE_FINWAIT2_INIT,
  NETWOX_SOCKV_TCPSTATE_CLOSEWAIT_INIT,
  NETWOX_SOCKV_TCPSTATE_CLOSING_INIT,
  NETWOX_SOCKV_TCPSTATE_LASTACK_INIT,
  NETWOX_SOCKV_TCPSTATE_TIMEWAIT_INIT,
  NETWOX_SOCKV_TCPSTATE_CLOSED_INIT,
  NETWOX_SOCKV_TCPSTATE_CLOSED_ONRESET_INIT,
  NETWOX_SOCKV_TCPSTATE_ENDINIT = 100,
  NETWOX_SOCKV_TCPSTATE_LISTEN = 101,
  NETWOX_SOCKV_TCPSTATE_SYNSENT,
  NETWOX_SOCKV_TCPSTATE_SYNRECEIVED,
  NETWOX_SOCKV_TCPSTATE_ESTABLISHED,
  NETWOX_SOCKV_TCPSTATE_FINWAIT1,
  NETWOX_SOCKV_TCPSTATE_FINWAIT2,
  NETWOX_SOCKV_TCPSTATE_CLOSEWAIT,
  NETWOX_SOCKV_TCPSTATE_CLOSING,
  NETWOX_SOCKV_TCPSTATE_LASTACK,
  NETWOX_SOCKV_TCPSTATE_TIMEWAIT,
  NETWOX_SOCKV_TCPSTATE_CLOSED,
  NETWOX_SOCKV_TCPSTATE_CLOSED_ONRESET
} netwox_sockv_tcpstate;

/*-------------------------------------------------------------*/
typedef struct {
  /* GENERIC */
  netwox_priv_io_sockv *pisv;
  netwib_eth remoteeth;
  netwib_ip remoteip;
  netwib_port remoteport;
  netwib_io *pio;
  netwib_bufpool *pbufpool;
  netwib_uint32 isn;
  netwib_uint32 currentlocalseqnum;
  netwib_uint32 currentremoteseqnum;
  netwib_uint32 maxchunksize;
  netwib_uint32 snifflastacknum;
  netwib_uint32 snifflastwindow;
  netwib_uint32 usrbufseqnumbegin;
  netwib_uint32 usrbufsentsize;
  netwib_uint32 numberofsynsent;
  netwox_sockv_tcpstate currentstate;
  netwib_bool handshakedone;
  netwib_bool continuetoloop;
  /* EVENTS */
  /* abstime for event wait */
  netwib_time *pabstime;
  netwib_time abstime;
  /* timeout event */
  netwib_bool eventtimeout;
  /* end event */
  netwib_bool eventusrend;
  /* usr data event */
  netwib_bool eventusr;
  netwib_buf usrbuf;
  /* sniff event */
  netwib_bool eventsniff;
  netwib_tcphdr snifftcpheader;
  netwib_buf sniffbuf;/* at first contain all packet, and then only tcp data */
  /* change state event */
  /*netwib_bool eventstate; not used, directly compared */
  netwox_sockv_tcpstate stateold;
  /* ACTIONS */
  /* spoof action */
  netwib_bool actionspoof;
  netwib_tcphdr spooftcpheader;
  netwib_buf *pspoofbuf; /* address of a buffer containing tcp data or NULL */
  /* user data action */
  netwib_bool actionusr;
  netwib_buf *pusrbuf; /* address of a buffer containing tcp data or NULL */
} netwox_sockv_tcp;

/*-------------------------------------------------------------*/
static netwib_err netwox_sockv_thread_tcp_actionusr(netwox_sockv_tcp *psvt)
{
  netwib_er(netwib_io_write(psvt->pisv->ptlvsysio, psvt->pusrbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_sockv_thread_tcp_actionspoof2(netwox_sockv_tcp *psvt,
                                                       netwib_constbuf *pbuf)
{
  netwib_linkhdr linkheader;
  netwib_iphdr ipheader;
  netwib_buf *pethpkt;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(psvt->pbufpool, &pethpkt));

  netwib_er(netwib_linkhdr_initdefault(NETWIB_DEVICE_DLTTYPE_ETHER,
                                       &linkheader));
  linkheader.hdr.ether.src = psvt->pisv->localeth;
  linkheader.hdr.ether.dst = psvt->remoteeth;
  switch(psvt->remoteip.iptype) {
  case NETWIB_IPTYPE_IP6 :
    linkheader.hdr.ether.type = NETWIB_ETHERHDRTYPE_IP6;
    break;
  default :
    linkheader.hdr.ether.type = NETWIB_ETHERHDRTYPE_IP4;
    break;
  }

  netwib_er(netwib_iphdr_initdefault(psvt->remoteip.iptype, &ipheader));
  ipheader.src = psvt->pisv->localip;
  ipheader.dst = psvt->remoteip;
  switch(psvt->remoteip.iptype) {
  case NETWIB_IPTYPE_IP6 :
    if (netwib__buf_ref_data_size(&psvt->pisv->ip6exts)) {
      ipheader.protocol = psvt->pisv->ip6extsproto;
      ipheader.header.ip6.exts = psvt->pisv->ip6exts;
    }
    break;
  default :
    ipheader.header.ip4.opts = psvt->pisv->ip4opts;
    break;
  }

  psvt->spooftcpheader.src = psvt->pisv->localport;
  psvt->spooftcpheader.dst = psvt->remoteport;

  netwib_er(netwib_pkt_append_linkiptcpdata(&linkheader, &ipheader,
                                            &psvt->spooftcpheader,
                                            pbuf, pethpkt));

  ret = netwib_io_write(psvt->pio, pethpkt);

  netwib_er(netwib_bufpool_buf_close(psvt->pbufpool, &pethpkt));

  psvt->currentlocalseqnum += netwib__buf_ref_data_sizenull(pbuf);
  psvt->spooftcpheader.seqnum = psvt->currentlocalseqnum;

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_sockv_thread_tcp_actionspoof(netwox_sockv_tcp *psvt)
{
  netwib_buf buf;
  netwib_uint32 datasizewhichcanbesent, endoffset;
  netwib_int32 ackedsize;

  if (psvt->pspoofbuf == NULL) {
    netwib_er(netwox_sockv_thread_tcp_actionspoof2(psvt, NULL));
    return(NETWIB_ERR_OK);
  }

  /* eventually update begin of buffer, if data was acknowledged */
  ackedsize = psvt->snifflastacknum - psvt->usrbufseqnumbegin;
  if (ackedsize > 0) {
    if ((netwib_uint32)ackedsize > psvt->usrbufsentsize) {
      /* perhaps a fake packet : ignore it */
      psvt->snifflastacknum = psvt->usrbufseqnumbegin;
      psvt->snifflastwindow = 0;
      psvt->eventusr = NETWIB_FALSE;
      return(NETWIB_ERR_OK);
    }
    psvt->pspoofbuf->beginoffset += ackedsize;
    psvt->usrbufseqnumbegin = psvt->snifflastacknum;
    psvt->usrbufsentsize -= ackedsize;
    netwib_er(netwib_time_init_now(&psvt->abstime));
    netwib_er(netwib_time_plus_sec(&psvt->abstime, 2));
  }

  /* check if window is not reached */
  if (psvt->usrbufsentsize >= psvt->snifflastwindow) {
    /* simply ignore for now. We'll retry later */
    psvt->eventusr = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  }

  /* compute how much we can send */
  datasizewhichcanbesent = psvt->snifflastwindow - psvt->usrbufsentsize;
  buf = *psvt->pspoofbuf;
  buf.beginoffset += psvt->usrbufsentsize;
  if (datasizewhichcanbesent > netwib__buf_ref_data_size(&buf)) {
    datasizewhichcanbesent = netwib__buf_ref_data_size(&buf);
  }
  if (datasizewhichcanbesent == 0) {
    psvt->eventusr = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  }

  /* send each chunk */
  buf.endoffset = buf.beginoffset + datasizewhichcanbesent;
  endoffset = buf.endoffset;
  while (netwib__buf_ref_data_size(&buf) > psvt->maxchunksize) {
    buf.endoffset = buf.beginoffset + psvt->maxchunksize;
    netwib_er(netwox_sockv_thread_tcp_actionspoof2(psvt, &buf));
    buf.beginoffset += psvt->maxchunksize;
    buf.endoffset = endoffset;
  }
  netwib_er(netwox_sockv_thread_tcp_actionspoof2(psvt, &buf));

  psvt->usrbufsentsize += datasizewhichcanbesent;
  psvt->eventusr = NETWIB_FALSE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_sockv_thread_tcp_eventusr(netwox_sockv_tcp *psvt)
{
  netwib_er(netwib_io_read(psvt->pisv->ptlvsysio, &psvt->usrbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_sockv_thread_tcp_eventsniff(netwox_sockv_tcp *psvt)
{
  netwib_linkhdr linkheader;
  netwib_iphdr ipheader;
  netwib_ipproto ipproto;
  netwib_bool checkremote, answered;
  netwib_cmp cmp;
  netwib_err ret;

  /* unset event. Will set it again if packet is really for us */
  psvt->eventsniff = NETWIB_FALSE;

  /* read */
  netwib__buf_reinit(&psvt->sniffbuf);
  ret = netwib_io_read(psvt->pio, &psvt->sniffbuf);
  if (ret == NETWIB_ERR_DATANOTAVAIL) {
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  /* answer alive */
  netwib_er(netwox_alive_ether(psvt->pio, &psvt->pisv->localeth,
                               &psvt->pisv->localip, &psvt->sniffbuf,
                               NETWIB_TRUE, NETWIB_FALSE, NETWIB_FALSE,
                               &answered));
  if (answered) return(NETWIB_ERR_OK);

  /* to ensure it's for us (local is checked in sniff filter) */
  checkremote = (netwib_bool)(psvt->pisv->socktype == NETWOX_SOCKTYPE_TCP_CLI||
    (psvt->pisv->socktype==NETWOX_SOCKTYPE_TCP_SER && psvt->handshakedone));

  /* decode link header */
  netwib_er(netwib_pkt_decode_layer_link(NETWIB_DEVICE_DLTTYPE_ETHER,
                                         &psvt->sniffbuf, &linkheader));
  switch(linkheader.hdr.ether.type) {
    case NETWIB_ETHERHDRTYPE_IP4 :
    case NETWIB_ETHERHDRTYPE_IP6 :
      /* continue below */
      break;
    default :
      return(NETWIB_ERR_OK);
  }
  if (checkremote) {
    netwib_er(netwib_eth_cmp(&linkheader.hdr.ether.src,
                             &psvt->remoteeth, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      return(NETWIB_ERR_OK);
    }
  }

  /* decode ip header */
  netwib_er(netwib_pkt_decode_layer_ip(&psvt->sniffbuf, &ipheader));
  netwib_er(netwib_iphdr_get_proto(&ipheader, &ipproto));
  if (ipproto !=  NETWIB_IPPROTO_TCP) {
    return(NETWIB_ERR_OK);
  }
  if (checkremote) {
    netwib_er(netwib_ip_cmp(&ipheader.src, &psvt->remoteip, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      return(NETWIB_ERR_OK);
    }
  }

  /* decode tcp header */
  netwib_er(netwib_pkt_decode_layer_tcp(&psvt->sniffbuf,
                                        &psvt->snifftcpheader));
  if (checkremote) {
    if (psvt->snifftcpheader.src != psvt->remoteport) {
      return(NETWIB_ERR_OK);
    }
  }

  if (psvt->pisv->socktype==NETWOX_SOCKTYPE_TCP_SER && !psvt->handshakedone) {
    /* saves who is the client */
    psvt->remoteeth = linkheader.hdr.ether.src;
    psvt->remoteip = ipheader.src;
    psvt->remoteport = psvt->snifftcpheader.src;
  }

  /* it is for us */
  psvt->eventsniff = NETWIB_TRUE;

  /* we need to respect window parameter */
  if (psvt->snifftcpheader.acknum - psvt->snifflastacknum > 0) {
    psvt->snifflastacknum = psvt->snifftcpheader.acknum;
    psvt->snifflastwindow = psvt->snifftcpheader.window;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
#define SOCKV_MASK_TIMEOUT (1<<30)
#define SOCKV_MASK_USREND (1<<29)
#define SOCKV_MASK_USR (1<<28)
#define SOCKV_MASK_SNIFF (1<<27)
#define netwox__set_sarf(s,a,r,f) {psvt->spooftcpheader.syn=s;psvt->spooftcpheader.ack=a;psvt->spooftcpheader.rst=r;psvt->spooftcpheader.fin=f;}
#define netwox__get_sarf(s,a,r,f) (psvt->snifftcpheader.syn==s&&psvt->snifftcpheader.ack==a&&psvt->snifftcpheader.rst==r&&psvt->snifftcpheader.fin==f)

static netwib_err netwox_sockv_tcp_statemachine(netwox_sockv_tcp *psvt)
{
  netwib_uint32 mask=0;

  if (psvt->currentstate < NETWOX_SOCKV_TCPSTATE_ENDINIT) {
    /* in init state, we reset timeout value */
    psvt->pabstime = NETWIB_TIME_INFINITE;
  } else {
    /* in working state, we'll differentiate case values. They'll
       be treated once at a time : usrend, then sniff, then usr then timeout */
    mask = (psvt->eventusrend?SOCKV_MASK_USREND:0);
    if (!mask && psvt->eventsniff) {
      mask = SOCKV_MASK_SNIFF;
    }
    if (!mask && psvt->eventusr) {
      mask = SOCKV_MASK_USR;
    }
    if (!mask && psvt->eventtimeout) {
      mask = SOCKV_MASK_TIMEOUT;
    }
  }

  switch(psvt->currentstate | mask) {
    case NETWOX_SOCKV_TCPSTATE_LISTEN_INIT :
      psvt->currentstate = NETWOX_SOCKV_TCPSTATE_LISTEN;
      break;
    case NETWOX_SOCKV_TCPSTATE_LISTEN :
      break;
    case NETWOX_SOCKV_TCPSTATE_LISTEN | SOCKV_MASK_TIMEOUT :
      return(NETWOX_ERR_INTERNALERROR);
      break;
   case NETWOX_SOCKV_TCPSTATE_LISTEN | SOCKV_MASK_USREND :
      return(NETWOX_ERR_INTERNALERROR);
      break;
    case NETWOX_SOCKV_TCPSTATE_LISTEN | SOCKV_MASK_USR :
      return(NETWOX_ERR_INTERNALERROR);
      break;
    case NETWOX_SOCKV_TCPSTATE_LISTEN | SOCKV_MASK_SNIFF :
      if (netwox__get_sarf(NETWIB_TRUE, NETWIB_FALSE, NETWIB_FALSE,
                           NETWIB_FALSE)) {
        /* client sent us a syn : spoof a syn-ack */
        psvt->currentlocalseqnum = psvt->isn;
        psvt->currentremoteseqnum = psvt->snifftcpheader.seqnum + 1;
        psvt->actionspoof = NETWIB_TRUE;
        netwox__set_sarf(NETWIB_TRUE, NETWIB_TRUE, NETWIB_FALSE, NETWIB_FALSE);
        psvt->spooftcpheader.seqnum = psvt->currentlocalseqnum;
        psvt->currentlocalseqnum++;
        psvt->usrbufseqnumbegin = psvt->currentlocalseqnum;
        psvt->spooftcpheader.acknum = psvt->currentremoteseqnum;
        psvt->pspoofbuf = NULL;
        psvt->currentstate = NETWOX_SOCKV_TCPSTATE_SYNRECEIVED_INIT;
      }
      psvt->eventsniff = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_SYNSENT_INIT :
      psvt->actionspoof = NETWIB_TRUE;
      netwox__set_sarf(NETWIB_TRUE, NETWIB_FALSE, NETWIB_FALSE, NETWIB_FALSE);
      psvt->currentlocalseqnum = psvt->isn;
      psvt->spooftcpheader.seqnum = psvt->currentlocalseqnum;
      psvt->spooftcpheader.acknum = 0;
      psvt->pspoofbuf = NULL;
      netwib_er(netwib_time_init_now(&psvt->abstime));
      netwib_er(netwib_time_plus_sec(&psvt->abstime, 1));
      psvt->pabstime = &psvt->abstime;
      psvt->numberofsynsent++;
      psvt->currentstate = NETWOX_SOCKV_TCPSTATE_SYNSENT;
      break;
    case NETWOX_SOCKV_TCPSTATE_SYNSENT :
      break;
    case NETWOX_SOCKV_TCPSTATE_SYNSENT | SOCKV_MASK_TIMEOUT :
      if (psvt->numberofsynsent > 5) {
        return(NETWOX_ERR_SOCKVTCP_NOSYNACK);
      }
      psvt->currentstate = NETWOX_SOCKV_TCPSTATE_SYNSENT_INIT;
      psvt->eventtimeout = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_SYNSENT | SOCKV_MASK_USREND :
      return(NETWOX_ERR_INTERNALERROR);
      break;
    case NETWOX_SOCKV_TCPSTATE_SYNSENT | SOCKV_MASK_USR :
      return(NETWOX_ERR_INTERNALERROR);
      break;
    case NETWOX_SOCKV_TCPSTATE_SYNSENT | SOCKV_MASK_SNIFF :
      if (netwox__get_sarf(NETWIB_FALSE, NETWIB_TRUE, NETWIB_TRUE,
                           NETWIB_FALSE) &&
          psvt->snifftcpheader.seqnum == 0 &&
          psvt->snifftcpheader.acknum == psvt->currentlocalseqnum + 1) {
        /* server sent us a reset */
        return(NETWOX_ERR_SOCKVTCP_RST);
      }
      if (netwox__get_sarf(NETWIB_TRUE, NETWIB_TRUE, NETWIB_FALSE,
                           NETWIB_FALSE) &&
          psvt->snifftcpheader.acknum == psvt->currentlocalseqnum + 1) {
        /* server sent us a syn ack */
        psvt->currentlocalseqnum++;
        psvt->usrbufseqnumbegin = psvt->currentlocalseqnum;
        psvt->currentremoteseqnum = psvt->snifftcpheader.seqnum + 1;
        /* spoof an ack */
        psvt->actionspoof = NETWIB_TRUE;
        netwox__set_sarf(NETWIB_FALSE, NETWIB_TRUE, NETWIB_FALSE,
                         NETWIB_FALSE);
        psvt->spooftcpheader.seqnum = psvt->currentlocalseqnum;
        psvt->spooftcpheader.acknum = psvt->currentremoteseqnum;
        psvt->pspoofbuf = NULL;
        psvt->currentstate = NETWOX_SOCKV_TCPSTATE_ESTABLISHED_INIT;
        /* inform about handshake end */
        netwib_er(netwox_sockv_info_write_err(psvt->pisv, NETWIB_ERR_OK));
        psvt->handshakedone = NETWIB_TRUE;
      }
      psvt->eventsniff = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_SYNRECEIVED_INIT :
      netwib_er(netwib_time_init_now(&psvt->abstime));
      netwib_er(netwib_time_plus_sec(&psvt->abstime, 2));
      psvt->pabstime = &psvt->abstime;
      psvt->currentstate = NETWOX_SOCKV_TCPSTATE_SYNRECEIVED;
      break;
    case NETWOX_SOCKV_TCPSTATE_SYNRECEIVED :
      break;
    case NETWOX_SOCKV_TCPSTATE_SYNRECEIVED | SOCKV_MASK_TIMEOUT :
      /* jump again to listen state */
      psvt->currentstate = NETWOX_SOCKV_TCPSTATE_LISTEN_INIT;
      psvt->eventtimeout = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_SYNRECEIVED | SOCKV_MASK_USREND :
      return(NETWOX_ERR_INTERNALERROR);
      break;
    case NETWOX_SOCKV_TCPSTATE_SYNRECEIVED | SOCKV_MASK_USR :
      return(NETWOX_ERR_INTERNALERROR);
      break;
    case NETWOX_SOCKV_TCPSTATE_SYNRECEIVED | SOCKV_MASK_SNIFF :
      if (netwox__get_sarf(NETWIB_FALSE, NETWIB_TRUE, NETWIB_FALSE,
                           NETWIB_FALSE) &&
          psvt->snifftcpheader.acknum == psvt->currentlocalseqnum) {
        psvt->currentstate = NETWOX_SOCKV_TCPSTATE_ESTABLISHED_INIT;
        /* inform about handshake end */
        netwib_er(netwox_sockv_info_write_err(psvt->pisv, NETWIB_ERR_OK));
        psvt->handshakedone = NETWIB_TRUE;
        /* on first connection to the server, indicates who is the client */
        netwib_er(netwox_sockv_info_write_cli(psvt->pisv, NETWIB_TRUE,
                                            &psvt->remoteeth,
                                            &psvt->remoteip,
                                            psvt->remoteport));
      }
      psvt->eventsniff = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_ESTABLISHED_INIT :
      netwib_er(netwib_time_init_now(&psvt->abstime));
      netwib_er(netwib_time_plus_sec(&psvt->abstime, 2));
      psvt->pabstime = &psvt->abstime;
      psvt->currentstate = NETWOX_SOCKV_TCPSTATE_ESTABLISHED;
      break;
    case NETWOX_SOCKV_TCPSTATE_ESTABLISHED :
      break;
    case NETWOX_SOCKV_TCPSTATE_ESTABLISHED | SOCKV_MASK_TIMEOUT :
      if (psvt->usrbufsentsize) {
        psvt->actionspoof = NETWIB_TRUE;
        netwox__set_sarf(NETWIB_FALSE, NETWIB_TRUE, NETWIB_FALSE,
                         NETWIB_FALSE);
        psvt->currentlocalseqnum -= psvt->usrbufsentsize;
        psvt->spooftcpheader.seqnum = psvt->currentlocalseqnum;
        psvt->spooftcpheader.acknum = psvt->currentremoteseqnum;
        psvt->pspoofbuf = &psvt->usrbuf;
        psvt->usrbufsentsize = 0;
      }
      netwib_er(netwib_time_plus_sec(&psvt->abstime, 2));
      psvt->eventtimeout = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_ESTABLISHED | SOCKV_MASK_USREND :
      /* user wants to terminate */
      psvt->actionspoof = NETWIB_TRUE;
      netwox__set_sarf(NETWIB_FALSE, NETWIB_TRUE, NETWIB_FALSE, NETWIB_TRUE);
      psvt->spooftcpheader.seqnum = psvt->currentlocalseqnum;
      psvt->currentlocalseqnum++;
      psvt->spooftcpheader.acknum = psvt->currentremoteseqnum;
      psvt->pspoofbuf = NULL;
      psvt->currentstate = NETWOX_SOCKV_TCPSTATE_FINWAIT1_INIT;
      /* reset the event */
      psvt->eventusrend = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_ESTABLISHED | SOCKV_MASK_USR :
      /* user send us data : we'll spoof it */
      psvt->actionspoof = NETWIB_TRUE;
      netwox__set_sarf(NETWIB_FALSE, NETWIB_TRUE, NETWIB_FALSE, NETWIB_FALSE);
      psvt->spooftcpheader.seqnum = psvt->currentlocalseqnum;
      psvt->spooftcpheader.acknum = psvt->currentremoteseqnum;
      psvt->pspoofbuf = &psvt->usrbuf;
      psvt->eventusr = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_ESTABLISHED | SOCKV_MASK_SNIFF :
      if (netwox__get_sarf(NETWIB_FALSE, NETWIB_FALSE, NETWIB_TRUE,
                           NETWIB_FALSE) &&
          psvt->snifftcpheader.seqnum == psvt->currentremoteseqnum) {
        /* a reset was received */
        psvt->currentstate = NETWOX_SOCKV_TCPSTATE_CLOSED_ONRESET;
      }
      if (netwox__get_sarf(NETWIB_FALSE, NETWIB_FALSE, NETWIB_FALSE,
                           NETWIB_TRUE) &&
          psvt->snifftcpheader.seqnum == psvt->currentremoteseqnum) {
        /* a fin was received */
        psvt->actionspoof = NETWIB_TRUE;
        netwox__set_sarf(NETWIB_FALSE, NETWIB_TRUE, NETWIB_FALSE,
                         NETWIB_FALSE);
        psvt->spooftcpheader.seqnum = psvt->currentlocalseqnum;
        psvt->currentremoteseqnum++;
        psvt->spooftcpheader.acknum = psvt->currentremoteseqnum;
        psvt->pspoofbuf = NULL;
        psvt->currentstate = NETWOX_SOCKV_TCPSTATE_CLOSEWAIT_INIT;
      }
      if (netwox__get_sarf(NETWIB_FALSE, NETWIB_TRUE, NETWIB_FALSE,
                           NETWIB_FALSE) &&
          psvt->snifftcpheader.seqnum == psvt->currentremoteseqnum) {
        /* sniff sent us data */
        if (netwib__buf_ref_data_size(&psvt->sniffbuf)) {
          psvt->currentremoteseqnum += netwib__buf_ref_data_size(&psvt->sniffbuf);
          /* ack this data */
          psvt->actionspoof = NETWIB_TRUE;
          netwox__set_sarf(NETWIB_FALSE, NETWIB_TRUE, NETWIB_FALSE,
                           NETWIB_FALSE);
          psvt->spooftcpheader.seqnum = psvt->currentlocalseqnum;
          psvt->spooftcpheader.acknum = psvt->currentremoteseqnum;
          psvt->pspoofbuf = NULL;
          /* send it to user */
          psvt->actionusr = NETWIB_TRUE;
          psvt->pusrbuf = &psvt->sniffbuf;
        }
      }
      if (netwox__get_sarf(NETWIB_FALSE, NETWIB_TRUE, NETWIB_FALSE,
                           NETWIB_FALSE) &&
          psvt->snifftcpheader.seqnum == psvt->currentremoteseqnum-1 &&
          netwib__buf_ref_data_size(&psvt->sniffbuf) == 0 ) {
        /* sniff sent us a keep-alive */
        /* ack this data */
        psvt->actionspoof = NETWIB_TRUE;
        netwox__set_sarf(NETWIB_FALSE, NETWIB_TRUE, NETWIB_FALSE,
                         NETWIB_FALSE);
        psvt->spooftcpheader.seqnum = psvt->currentlocalseqnum;
        psvt->spooftcpheader.acknum = psvt->currentremoteseqnum;
        psvt->pspoofbuf = NULL;
        /* send it to user */
        psvt->actionusr = NETWIB_TRUE;
        psvt->pusrbuf = &psvt->sniffbuf;
      }
      if (netwox__get_sarf(NETWIB_FALSE, NETWIB_TRUE, NETWIB_FALSE,
                           NETWIB_TRUE) &&
          psvt->snifftcpheader.seqnum == psvt->currentremoteseqnum) {
        /* sniff sent us an ack-fin */
        psvt->currentremoteseqnum +=netwib__buf_ref_data_size(&psvt->sniffbuf);
        /* ack data */
        psvt->actionspoof = NETWIB_TRUE;
        netwox__set_sarf(NETWIB_FALSE, NETWIB_TRUE, NETWIB_FALSE,
                         NETWIB_FALSE);
        psvt->spooftcpheader.seqnum = psvt->currentlocalseqnum;
        psvt->currentremoteseqnum++;
        psvt->spooftcpheader.acknum = psvt->currentremoteseqnum;
        psvt->pspoofbuf = NULL;
        psvt->currentstate = NETWOX_SOCKV_TCPSTATE_CLOSEWAIT_INIT;
        /* send it to user */
        psvt->actionusr = NETWIB_TRUE;
        psvt->pusrbuf = &psvt->sniffbuf;
      }
      psvt->eventsniff = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_FINWAIT1_INIT :
      netwib_er(netwib_time_init_now(&psvt->abstime));
      netwib_er(netwib_time_plus_sec(&psvt->abstime, 1));
      psvt->pabstime = &psvt->abstime;
      psvt->currentstate = NETWOX_SOCKV_TCPSTATE_FINWAIT1;
      break;
    case NETWOX_SOCKV_TCPSTATE_FINWAIT1 :
      break;
    case NETWOX_SOCKV_TCPSTATE_FINWAIT1 | SOCKV_MASK_TIMEOUT :
      psvt->currentstate = NETWOX_SOCKV_TCPSTATE_CLOSED_INIT;
      psvt->eventtimeout = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_FINWAIT1 | SOCKV_MASK_USREND :
      psvt->continuetoloop = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_FINWAIT1 | SOCKV_MASK_USR :
      /* ignore user data now */
      psvt->eventusr = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_FINWAIT1 | SOCKV_MASK_SNIFF :
      if (netwox__get_sarf(NETWIB_FALSE, NETWIB_FALSE, NETWIB_TRUE,
                           NETWIB_FALSE) &&
          psvt->snifftcpheader.seqnum == psvt->currentremoteseqnum) {
        psvt->currentstate = NETWOX_SOCKV_TCPSTATE_CLOSED_INIT;
      }
      if (netwox__get_sarf(NETWIB_FALSE, NETWIB_TRUE, NETWIB_FALSE,
                           NETWIB_FALSE) &&
          psvt->snifftcpheader.acknum == psvt->currentlocalseqnum) {
        psvt->currentstate = NETWOX_SOCKV_TCPSTATE_FINWAIT2_INIT;
      }
      if (netwox__get_sarf(NETWIB_FALSE, NETWIB_FALSE, NETWIB_FALSE,
                           NETWIB_TRUE)) {
        psvt->actionspoof = NETWIB_TRUE;
        netwox__set_sarf(NETWIB_FALSE, NETWIB_TRUE, NETWIB_FALSE,
                         NETWIB_FALSE);
        psvt->spooftcpheader.seqnum = psvt->currentlocalseqnum;
        /* user asked to close so we do not give him data between
           psvt->currentremoteseqnum and psvt->snifftcpheader.seqnum */
        psvt->currentremoteseqnum = psvt->snifftcpheader.seqnum;
        psvt->spooftcpheader.acknum = psvt->currentremoteseqnum + 1;
        psvt->pspoofbuf = NULL;
        psvt->currentstate = NETWOX_SOCKV_TCPSTATE_CLOSING_INIT;
      }
      psvt->eventsniff = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_FINWAIT2_INIT :
      netwib_er(netwib_time_init_now(&psvt->abstime));
      netwib_er(netwib_time_plus_sec(&psvt->abstime, 1));
      psvt->pabstime = &psvt->abstime;
      psvt->currentstate = NETWOX_SOCKV_TCPSTATE_FINWAIT2;
      break;
    case NETWOX_SOCKV_TCPSTATE_FINWAIT2 :
      break;
    case NETWOX_SOCKV_TCPSTATE_FINWAIT2 | SOCKV_MASK_TIMEOUT :
      psvt->currentstate = NETWOX_SOCKV_TCPSTATE_CLOSED_INIT;
      psvt->eventtimeout = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_FINWAIT2 | SOCKV_MASK_USREND :
      psvt->continuetoloop = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_FINWAIT2 | SOCKV_MASK_USR :
      /* ignore user data now */
      psvt->eventusr = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_FINWAIT2 | SOCKV_MASK_SNIFF :
      if (netwox__get_sarf(NETWIB_FALSE, NETWIB_FALSE, NETWIB_TRUE,
                           NETWIB_FALSE) &&
          psvt->snifftcpheader.seqnum == psvt->currentremoteseqnum) {
        psvt->currentstate = NETWOX_SOCKV_TCPSTATE_CLOSED_INIT;
      }
      if (netwox__get_sarf(NETWIB_FALSE, NETWIB_TRUE, NETWIB_FALSE,
                           NETWIB_TRUE) ||
          netwox__get_sarf(NETWIB_FALSE, NETWIB_FALSE, NETWIB_FALSE,
                           NETWIB_TRUE)) {
        psvt->actionspoof = NETWIB_TRUE;
        netwox__set_sarf(NETWIB_FALSE, NETWIB_TRUE, NETWIB_FALSE,
                         NETWIB_FALSE);
        psvt->spooftcpheader.seqnum = psvt->currentlocalseqnum;
        /* user asked to close so we do not give him data between
           psvt->currentremoteseqnum and psvt->snifftcpheader.seqnum */
        psvt->currentremoteseqnum = psvt->snifftcpheader.seqnum;
        psvt->spooftcpheader.acknum = psvt->currentremoteseqnum + 1;
        psvt->pspoofbuf = NULL;
        psvt->currentstate = NETWOX_SOCKV_TCPSTATE_TIMEWAIT_INIT;
      }
      psvt->eventsniff = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_CLOSEWAIT_INIT :
      netwib_er(netwib_time_init_now(&psvt->abstime));
      netwib_er(netwib_time_plus_msec(&psvt->abstime, 100));
      psvt->pabstime = &psvt->abstime;
      psvt->currentstate = NETWOX_SOCKV_TCPSTATE_CLOSEWAIT;
      break;
    case NETWOX_SOCKV_TCPSTATE_CLOSEWAIT :
      break;
    case NETWOX_SOCKV_TCPSTATE_CLOSEWAIT | SOCKV_MASK_TIMEOUT :
    case NETWOX_SOCKV_TCPSTATE_CLOSEWAIT | SOCKV_MASK_USREND :
      /* send a fin */
      psvt->actionspoof = NETWIB_TRUE;
      netwox__set_sarf(NETWIB_FALSE, NETWIB_TRUE, NETWIB_FALSE, NETWIB_TRUE);
      psvt->spooftcpheader.seqnum = psvt->currentlocalseqnum;
      psvt->spooftcpheader.acknum = psvt->currentremoteseqnum;
      psvt->pspoofbuf = NULL;
      psvt->currentstate = NETWOX_SOCKV_TCPSTATE_LASTACK_INIT;
      psvt->eventtimeout = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_CLOSEWAIT | SOCKV_MASK_USR :
      /* continue to send user's data */
      psvt->actionspoof = NETWIB_TRUE;
      netwox__set_sarf(NETWIB_FALSE, NETWIB_TRUE, NETWIB_FALSE, NETWIB_FALSE);
      psvt->spooftcpheader.seqnum = psvt->currentlocalseqnum;
      psvt->spooftcpheader.acknum = psvt->currentremoteseqnum;
      psvt->pspoofbuf = &psvt->usrbuf;
      psvt->eventusr = NETWIB_FALSE;
      /* update for timeout */
      netwib_er(netwib_time_init_now(&psvt->abstime));
      netwib_er(netwib_time_plus_msec(&psvt->abstime, 100));
      psvt->eventtimeout = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_CLOSEWAIT | SOCKV_MASK_SNIFF :
      /* ignore sniff */
      psvt->eventsniff = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_CLOSING_INIT :
      netwib_er(netwib_time_init_now(&psvt->abstime));
      netwib_er(netwib_time_plus_sec(&psvt->abstime, 1));
      psvt->pabstime = &psvt->abstime;
      psvt->currentstate = NETWOX_SOCKV_TCPSTATE_CLOSING;
      break;
    case NETWOX_SOCKV_TCPSTATE_CLOSING :
      break;
    case NETWOX_SOCKV_TCPSTATE_CLOSING | SOCKV_MASK_TIMEOUT :
      psvt->currentstate = NETWOX_SOCKV_TCPSTATE_CLOSED_INIT;
      psvt->eventtimeout = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_CLOSING | SOCKV_MASK_USREND :
      psvt->continuetoloop = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_CLOSING | SOCKV_MASK_USR :
      /* ignore user data now */
      psvt->eventusr = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_CLOSING | SOCKV_MASK_SNIFF :
      if (netwox__get_sarf(NETWIB_FALSE, NETWIB_FALSE, NETWIB_TRUE,
                           NETWIB_FALSE) &&
          psvt->snifftcpheader.seqnum == psvt->currentremoteseqnum) {
        psvt->currentstate = NETWOX_SOCKV_TCPSTATE_CLOSED_INIT;
      }
      if (netwox__get_sarf(NETWIB_FALSE, NETWIB_TRUE, NETWIB_FALSE,
                           NETWIB_FALSE) &&
          psvt->snifftcpheader.acknum == psvt->currentlocalseqnum) {
        psvt->currentstate = NETWOX_SOCKV_TCPSTATE_TIMEWAIT_INIT;
      }
      psvt->eventsniff = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_LASTACK_INIT :
      netwib_er(netwib_time_init_now(&psvt->abstime));
      netwib_er(netwib_time_plus_sec(&psvt->abstime, 1));
      psvt->pabstime = &psvt->abstime;
      psvt->currentstate = NETWOX_SOCKV_TCPSTATE_LASTACK;
      break;
    case NETWOX_SOCKV_TCPSTATE_LASTACK :
      break;
    case NETWOX_SOCKV_TCPSTATE_LASTACK | SOCKV_MASK_TIMEOUT :
      psvt->currentstate = NETWOX_SOCKV_TCPSTATE_CLOSED;
      psvt->eventtimeout = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_LASTACK | SOCKV_MASK_USREND :
      psvt->continuetoloop = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_LASTACK | SOCKV_MASK_USR :
      /* ignore user data now */
      psvt->eventusr = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_LASTACK | SOCKV_MASK_SNIFF :
      if (netwox__get_sarf(NETWIB_FALSE, NETWIB_FALSE, NETWIB_TRUE,
                           NETWIB_FALSE) &&
          psvt->snifftcpheader.seqnum == psvt->currentremoteseqnum) {
        psvt->currentstate = NETWOX_SOCKV_TCPSTATE_CLOSED_INIT;
      }
      if (netwox__get_sarf(NETWIB_FALSE, NETWIB_TRUE, NETWIB_FALSE,
                           NETWIB_FALSE) &&
          psvt->snifftcpheader.acknum == psvt->currentlocalseqnum + 1) {
        psvt->currentstate = NETWOX_SOCKV_TCPSTATE_CLOSED_INIT;
      }
      psvt->eventsniff = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_TIMEWAIT_INIT :
      psvt->currentstate = NETWOX_SOCKV_TCPSTATE_TIMEWAIT;
      break;
    case NETWOX_SOCKV_TCPSTATE_TIMEWAIT :
      psvt->currentstate = NETWOX_SOCKV_TCPSTATE_CLOSED;
      break;
    case NETWOX_SOCKV_TCPSTATE_TIMEWAIT | SOCKV_MASK_TIMEOUT :
      psvt->currentstate = NETWOX_SOCKV_TCPSTATE_CLOSED;
      psvt->eventtimeout = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_TIMEWAIT | SOCKV_MASK_USREND :
      psvt->currentstate = NETWOX_SOCKV_TCPSTATE_CLOSED;
      psvt->continuetoloop = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_TIMEWAIT | SOCKV_MASK_USR :
      psvt->currentstate = NETWOX_SOCKV_TCPSTATE_CLOSED;
      psvt->eventusr = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_TIMEWAIT | SOCKV_MASK_SNIFF :
      psvt->currentstate = NETWOX_SOCKV_TCPSTATE_CLOSED;
      psvt->eventsniff = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_CLOSED_INIT :
      psvt->currentstate = NETWOX_SOCKV_TCPSTATE_CLOSED;
      break;
    case NETWOX_SOCKV_TCPSTATE_CLOSED :
    case NETWOX_SOCKV_TCPSTATE_CLOSED | SOCKV_MASK_TIMEOUT :
    case NETWOX_SOCKV_TCPSTATE_CLOSED | SOCKV_MASK_USREND :
    case NETWOX_SOCKV_TCPSTATE_CLOSED | SOCKV_MASK_USR :
    case NETWOX_SOCKV_TCPSTATE_CLOSED | SOCKV_MASK_SNIFF :
      psvt->continuetoloop = NETWIB_FALSE;
      break;
    case NETWOX_SOCKV_TCPSTATE_CLOSED_ONRESET_INIT :
      psvt->currentstate = NETWOX_SOCKV_TCPSTATE_CLOSED_ONRESET;
      break;
    case NETWOX_SOCKV_TCPSTATE_CLOSED_ONRESET :
    case NETWOX_SOCKV_TCPSTATE_CLOSED_ONRESET | SOCKV_MASK_TIMEOUT :
    case NETWOX_SOCKV_TCPSTATE_CLOSED_ONRESET | SOCKV_MASK_USREND :
    case NETWOX_SOCKV_TCPSTATE_CLOSED_ONRESET | SOCKV_MASK_USR :
    case NETWOX_SOCKV_TCPSTATE_CLOSED_ONRESET | SOCKV_MASK_SNIFF :
      return(NETWOX_ERR_SOCKVTCP_RST);
      break;
    default :
      return(NETWOX_ERR_INTERNALERROR);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
#if NETWOX_SOCKV_TCP_STATEMACHINE_DEBUG == 1
static netwib_err netwox_sockv_tcpstate_disp(netwox_sockv_tcpstate tcpstate)
{
  netwib_conststring pc=NULL;

  switch(tcpstate) {
    case NETWOX_SOCKV_TCPSTATE_LISTEN_INIT :
      pc = "listen_init";
      break;
    case NETWOX_SOCKV_TCPSTATE_SYNSENT_INIT :
      pc = "synsent_init";
      break;
    case NETWOX_SOCKV_TCPSTATE_SYNRECEIVED_INIT :
      pc = "synreceived_init";
      break;
    case NETWOX_SOCKV_TCPSTATE_ESTABLISHED_INIT :
      pc = "established_init";
      break;
    case NETWOX_SOCKV_TCPSTATE_FINWAIT1_INIT :
      pc = "finwait1_init";
      break;
    case NETWOX_SOCKV_TCPSTATE_FINWAIT2_INIT :
      pc = "finwait2_init";
      break;
    case NETWOX_SOCKV_TCPSTATE_CLOSEWAIT_INIT :
      pc = "closewait_init";
      break;
    case NETWOX_SOCKV_TCPSTATE_CLOSING_INIT :
      pc = "closing_init";
      break;
    case NETWOX_SOCKV_TCPSTATE_LASTACK_INIT :
      pc = "lastack_init";
      break;
    case NETWOX_SOCKV_TCPSTATE_TIMEWAIT_INIT :
      pc = "timewait_init";
      break;
    case NETWOX_SOCKV_TCPSTATE_CLOSED_INIT :
      pc = "closed_init";
      break;
    case NETWOX_SOCKV_TCPSTATE_CLOSED_ONRESET_INIT :
      pc = "closed_onreset_init";
      break;
    case NETWOX_SOCKV_TCPSTATE_ENDINIT :
      pc = "endinit";
      break;
    case NETWOX_SOCKV_TCPSTATE_LISTEN :
      pc = "listen";
      break;
    case NETWOX_SOCKV_TCPSTATE_SYNSENT :
      pc = "synsent";
      break;
    case NETWOX_SOCKV_TCPSTATE_SYNRECEIVED :
      pc = "synreceived";
      break;
    case NETWOX_SOCKV_TCPSTATE_ESTABLISHED :
      pc = "established";
      break;
    case NETWOX_SOCKV_TCPSTATE_FINWAIT1 :
      pc = "finwait1";
      break;
    case NETWOX_SOCKV_TCPSTATE_FINWAIT2 :
      pc = "finwait2";
      break;
    case NETWOX_SOCKV_TCPSTATE_CLOSEWAIT :
      pc = "closewait";
      break;
    case NETWOX_SOCKV_TCPSTATE_CLOSING :
      pc = "closing";
      break;
    case NETWOX_SOCKV_TCPSTATE_LASTACK :
      pc = "lastack";
      break;
    case NETWOX_SOCKV_TCPSTATE_TIMEWAIT :
      pc = "timewait";
      break;
    case NETWOX_SOCKV_TCPSTATE_CLOSED :
      pc = "closed";
      break;
    case NETWOX_SOCKV_TCPSTATE_CLOSED_ONRESET :
      pc = "closed_onreset";
      break;
  }
  netwib_er(netwib_fmt_display("%s", pc));

  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
static netwib_err netwox_sockv_tcp_statemachine_full(netwox_sockv_tcp *psvt)
{
  netwib_err ret;

  psvt->actionspoof = NETWIB_FALSE;
  psvt->actionusr = NETWIB_FALSE;

  psvt->stateold = psvt->currentstate;

#if NETWOX_SOCKV_TCP_STATEMACHINE_DEBUG == 1
  netwib_er(netwib_fmt_display("STATE:%{uint32}[", psvt->currentstate));
  netwib_er(netwox_sockv_tcpstate_disp(psvt->currentstate));
  netwib_er(netwib_fmt_display("] (to:%{bool:0}, ue:%{bool:0}, u:%{bool:0}, sn:%{bool:0}) --> ", psvt->eventtimeout, psvt->eventusrend, psvt->eventusr, psvt->eventsniff));
#endif

  ret = netwox_sockv_tcp_statemachine(psvt);

#if NETWOX_SOCKV_TCP_STATEMACHINE_DEBUG == 1
  netwib_er(netwib_fmt_display("STATE:%{uint32}[", psvt->currentstate));
  netwib_er(netwox_sockv_tcpstate_disp(psvt->currentstate));
  netwib_er(netwib_fmt_display("] (ret=%{uint32}, sp:%{bool:0}, u:%{bool:0})\n", ret, psvt->actionspoof, psvt->actionusr));
#endif

  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  if (psvt->actionspoof) {
    netwib_er(netwox_sockv_thread_tcp_actionspoof(psvt));
  }
  if (psvt->actionusr) {
    netwib_er(netwox_sockv_thread_tcp_actionusr(psvt));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_sockv_thread_tcp(netwib_ptr infosin,
                                   netwib_ptr *pinfosout)
{
  netwox_sockv_tcp svt;
  netwox_priv_io_sockv *pisv = (netwox_priv_io_sockv *)infosin;
  netwib_wait *pwaitusr, *pwaitusrend, *pwaitsniff;
  netwib_uint32 usrendval;
  netwib_bool supportip6;
  netwib_buf filter;
  netwib_err ret;

  /* initialize variables */
  svt.pisv = pisv;
  switch (pisv->socktype) {
  case NETWOX_SOCKTYPE_TCP_CLI :
    svt.remoteeth = pisv->remoteeth;
    svt.remoteip = pisv->remoteip;
    svt.remoteport = pisv->remoteport;
    svt.currentstate = NETWOX_SOCKV_TCPSTATE_SYNSENT_INIT;
    break;
  case NETWOX_SOCKTYPE_TCP_SER :
    svt.currentstate = NETWOX_SOCKV_TCPSTATE_LISTEN_INIT;
    break;
  default :
    break;
  }
  netwib_er(netwib_tcphdr_initdefault(&svt.spooftcpheader));
  svt.spooftcpheader.window = 16000;
  netwib_er(netwib_uint32_init_rand(1, 0xFFFFFFFFu, &svt.isn));
  svt.pabstime = NETWIB_TIME_INFINITE;
  netwib_er(netwib_buf_init_mallocdefault(&svt.usrbuf));
  svt.usrbuf.flags |= NETWIB_BUF_FLAGS_CANSLIDE;
  netwib_er(netwib_buf_init_mallocdefault(&svt.sniffbuf));
  svt.handshakedone = NETWIB_FALSE;

  /* the max chunk size depends on MTU */
  if (pisv->localip.iptype == NETWIB_IPTYPE_IP4) {
    svt.maxchunksize = NETWIB_MTU_MIN_IP4 - 20 -
      netwib__buf_ref_data_size(&pisv->ip4opts) - NETWIB_TCPHDR_MAXLEN;
  } else {
    svt.maxchunksize = NETWIB_MTU_MIN_IP6 - NETWIB_IPHDR_MINLEN -
      netwib__buf_ref_data_size(&pisv->ip6exts) - NETWIB_TCPHDR_MAXLEN;
  }

  svt.snifflastacknum = 0;
  svt.snifflastwindow = 0;
  svt.usrbufseqnumbegin = 0;
  svt.usrbufsentsize = 0;
  svt.numberofsynsent = 0;

  /* initialize sniff/spoof io */
  netwib_er(netwib_buf_init_mallocdefault(&filter));
  netwib_er(netwox_pcapfilt_supports_ip6(&pisv->device, &supportip6));
  if (pisv->localip.iptype == NETWIB_IPTYPE_IP4) {
    netwib_er(netwib_buf_append_fmt(&filter, "arp or (ether dst %{eth} and ip dst %{ip} and tcp and dst port %{port})", &pisv->localeth, &pisv->localip, pisv->localport));
  } else {
    if (supportip6) {
      netwib_er(netwib_buf_append_fmt(&filter, "icmp6 or (ether dst %{eth} and ip6 dst %{ip} and tcp and dst port %{port})", &pisv->localeth, &pisv->localip, pisv->localport));
    }
  }
  netwib_er(netwox_snispo_init_eth(&pisv->device, &filter, &svt.pio));
  netwib_er(netwib_buf_close(&filter));

  /* initialize waiting */
  netwib_er(netwib_wait_init_io_read(pisv->ptlvsysio, &pwaitusr));
  netwib_er(netwib_wait_init_thread_cond(pisv->pendcond, &usrendval,
                                         &pwaitusrend));
  netwib_er(netwib_wait_init_io_read(svt.pio, &pwaitsniff));

  /* initialize working buffer */
  netwib_er(netwib_bufpool_initdefault(&svt.pbufpool));

  /* initialize state machine */
  netwib_er(netwox_sockv_tcp_statemachine_full(&svt));

  /* main loop */
  ret = NETWIB_ERR_OK;
  svt.continuetoloop = NETWIB_TRUE;
  while (svt.continuetoloop) {
    netwib_er(netwib_wait_wait3(pwaitusr, pwaitusrend, pwaitsniff,
                                svt.pabstime, &svt.eventusr, &svt.eventusrend,
                                &svt.eventsniff));
    svt.eventtimeout = NETWIB_FALSE;
    if (!svt.eventusr && !svt.eventusrend && !svt.eventsniff) {
      svt.eventtimeout = NETWIB_TRUE;
    }
    if (svt.eventusr) {
      netwib_er(netwox_sockv_thread_tcp_eventusr(&svt));
      if (svt.eventusrend) {
        /* reset usrend because io_close was called while there is still
           user data to send. We will deal with this event in next loop
           iteration because svt.eventusrend will be set again */
        svt.eventusrend = NETWIB_FALSE;
      }
    }
    if (svt.eventsniff) {
      netwib_er(netwox_sockv_thread_tcp_eventsniff(&svt));
      if (netwib__buf_ref_data_size(&svt.usrbuf)) {
        /* because we can sniff an ack allowing to slide window, so
           to send stored data */
        svt.eventusr = NETWIB_TRUE;
      }
    }
    while (svt.eventusr || svt.eventusrend || svt.eventsniff ||
           svt.eventtimeout || svt.currentstate != svt.stateold) {
      ret = netwox_sockv_tcp_statemachine_full(&svt);
      if (ret != NETWIB_ERR_OK) {
        svt.continuetoloop = NETWIB_FALSE;
      }
      if (!svt.continuetoloop) {
        break;
      }
    }
  }

  /* indicates end */
  netwib_er(netwib_io_ctl_set_end_write(pisv->ptlvsysio));

  /* close resources */
  netwib_er(netwib_buf_close(&svt.usrbuf));
  netwib_er(netwib_buf_close(&svt.sniffbuf));
  netwib_er(netwib_bufpool_close(&svt.pbufpool));
  netwib_er(netwib_wait_close(&pwaitusr));
  netwib_er(netwib_wait_close(&pwaitusrend));
  netwib_er(netwib_wait_close(&pwaitsniff));
  netwib_er(netwib_io_close(&svt.pio));

  /* "father" waits for an error code, so give it */
  if (!svt.handshakedone) {
    netwib_er(netwox_sockv_info_write_err(svt.pisv, ret));
  }

  pinfosout = pinfosout; /* for compiler warning */
  return(ret);
}
