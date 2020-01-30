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
#include "../netwox.h"

/*-------------------------------------------------------------*/
#define NETWOX_PERFMSG_HELLO_V1SIZE (5*sizeof(netwib_uint32))
#define NETWOX_PERFMSG_DATA_V1SIZE (6*sizeof(netwib_uint32))
#define NETWOX_PERFMSG_BYE_V1SIZE (3*sizeof(netwib_uint32))
#define NETWOX_PERFMSG_BYEACK_V1SIZE (2*sizeof(netwib_uint32))

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_perfmsg(netwox_constperfmsg *pmsg,
                                     netwib_buf *ppkt)
{
  netwib_data data;
  netwib_uint32 msgsize=0;

  switch(pmsg->perfmsgtype) {
  case NETWOX_PERFMSGTYPE_HELLO :
    msgsize = NETWOX_PERFMSG_HELLO_V1SIZE;
    break;
  case NETWOX_PERFMSGTYPE_DATA :
    if (pmsg->size < NETWOX_PERFMSG_DATA_V1SIZE) {
      return(NETWIB_ERR_PATOOLOW);
    }
    msgsize = pmsg->size;
    break;
  case NETWOX_PERFMSGTYPE_BYE :
    msgsize = NETWOX_PERFMSG_BYE_V1SIZE;
    break;
  case NETWOX_PERFMSGTYPE_BYEACK :
    msgsize = NETWOX_PERFMSG_BYEACK_V1SIZE;
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  }

  netwib_er(netwib_buf_wantspace(ppkt, msgsize, &data));
  netwib__data_append_uint32(data, msgsize);
  netwib__data_append_uint32(data, pmsg->perfmsgtype);

  switch(pmsg->perfmsgtype) {
  case NETWOX_PERFMSGTYPE_HELLO :
    if (pmsg->msg.hello.version != 1) {
      return(NETWOX_ERR_NOTSUPPORTED);
    }
    netwib__data_append_uint32(data, pmsg->msg.hello.version);
    netwib__data_append_uint32(data, pmsg->msg.hello.durationms);
    netwib__data_append_uint32(data, pmsg->msg.hello.datasize);
    break;
  case NETWOX_PERFMSGTYPE_DATA :
    netwib__data_append_uint32(data, pmsg->msg.data.sendtime.sec);
    netwib__data_append_uint32(data, pmsg->msg.data.sendtime.nsec);
    netwib__data_append_uint32(data, pmsg->msg.data.recvtime.sec);
    netwib__data_append_uint32(data, pmsg->msg.data.recvtime.nsec);
    break;
  case NETWOX_PERFMSGTYPE_BYE :
    netwib__data_append_uint32(data, pmsg->msg.bye.numdatasent);
    break;
  case NETWOX_PERFMSGTYPE_BYEACK :
    break;
  }

  ppkt->endoffset += msgsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_decode_perfmsg(netwib_buf *ppkt,
                                     netwox_perfmsg *pmsg)
{
  netwib_data data;
  netwib_uint32 msgsize=0;

  if (netwib__buf_ref_data_size(ppkt) < 8) {
    return(NETWIB_ERR_DATAMISSING);
  }
  data = netwib__buf_ref_data_ptr(ppkt);

  netwib__data_decode_uint32(data, pmsg->size);
  if (netwib__buf_ref_data_size(ppkt) < pmsg->size) {
    return(NETWIB_ERR_DATAMISSING);
  }

  netwib__data_decode_uint32(data, pmsg->perfmsgtype);
  switch(pmsg->perfmsgtype) {
  case NETWOX_PERFMSGTYPE_HELLO :
    msgsize = NETWOX_PERFMSG_HELLO_V1SIZE;
    break;
  case NETWOX_PERFMSGTYPE_DATA :
    if (pmsg->size < NETWOX_PERFMSG_DATA_V1SIZE) {
      ppkt->beginoffset += pmsg->size;
      return(NETWOX_ERR_NOTSUPPORTED);
    }
    msgsize = pmsg->size;
    break;
  case NETWOX_PERFMSGTYPE_BYE :
    msgsize = NETWOX_PERFMSG_BYE_V1SIZE;
    break;
  case NETWOX_PERFMSGTYPE_BYEACK :
    msgsize = NETWOX_PERFMSG_BYEACK_V1SIZE;
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  }
  if (pmsg->size != msgsize) {
    ppkt->beginoffset += pmsg->size;
    return(NETWOX_ERR_NOTSUPPORTED);
  }

  switch(pmsg->perfmsgtype) {
  case NETWOX_PERFMSGTYPE_HELLO :
    netwib__data_decode_uint32(data, pmsg->msg.hello.version);
    if (pmsg->msg.hello.version != 1) {
      ppkt->beginoffset += pmsg->size;
      return(NETWOX_ERR_NOTSUPPORTED);
    }
    netwib__data_decode_uint32(data, pmsg->msg.hello.durationms);
    netwib__data_decode_uint32(data, pmsg->msg.hello.datasize);
    break;
  case NETWOX_PERFMSGTYPE_DATA :
    netwib__data_decode_uint32(data, pmsg->msg.data.sendtime.sec);
    netwib__data_decode_uint32(data, pmsg->msg.data.sendtime.nsec);
    netwib__data_decode_uint32(data, pmsg->msg.data.recvtime.sec);
    netwib__data_decode_uint32(data, pmsg->msg.data.recvtime.nsec);
    break;
  case NETWOX_PERFMSGTYPE_BYE :
    netwib__data_decode_uint32(data, pmsg->msg.bye.numdatasent);
    break;
  case NETWOX_PERFMSGTYPE_BYEACK :
    break;
  }

  ppkt->beginoffset += pmsg->size;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_perf_recv(netwib_io *pio,
                                   netwib_time *pabstime,
                                   netwib_buf *pbuf,
                                   netwox_perfmsg *pperfmsg)
{
  netwib_bool event;
  netwib_err ret;

  pbuf->flags |= NETWIB_BUF_FLAGS_CANSLIDE;

  /* check if an old chunk is ready (do not check if time has elapsed) */
  ret = netwox_pkt_decode_perfmsg(pbuf, pperfmsg);
  if (ret == NETWIB_ERR_OK) {
    if (pperfmsg->perfmsgtype == NETWOX_PERFMSGTYPE_DATA) {
      netwib_er(netwib_time_init_now(&pperfmsg->msg.data.recvtime));
    }
    return(NETWIB_ERR_OK);
  }

  while(NETWIB_TRUE) {
    /* wait */
    netwib_er(netwib_io_wait_read(pio, pabstime, &event));
    if (!event) {
      ret = NETWOX_ERR_TIMEOUT;
      break;
    }
    /* read */
    ret = netwib_io_read(pio, pbuf);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* analyze */
    ret = netwox_pkt_decode_perfmsg(pbuf, pperfmsg);
    if (ret != NETWIB_ERR_OK) {
      continue;
    }
    break;
  }

  if (pperfmsg->perfmsgtype == NETWOX_PERFMSGTYPE_DATA) {
    netwib_er(netwib_time_init_now(&pperfmsg->msg.data.recvtime));
  }

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_perf_send(netwib_io *pio,
                                   netwib_buf *pbuf,
                                   netwox_perfmsg *pperfmsg)
{
  netwib__buf_reinit(pbuf);

  if (pperfmsg->perfmsgtype == NETWOX_PERFMSGTYPE_DATA) {
    netwib_er(netwib_time_init_now(&pperfmsg->msg.data.sendtime));
  }
  netwib_er(netwox_pkt_append_perfmsg(pperfmsg, pbuf));

  netwib_er(netwib_io_write(pio, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_perf_ser(netwib_io *pio)
{
  netwib_buf buf;
  netwox_perfmsg perfmsg;
  netwib_uint32 datasize=0, durationms=0, numdatasent;
  netwib_time endtime;
  netwib_bool haselapsed;
  netwib_err ret;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  /* read hello sent by client */
  while(NETWIB_TRUE) {
    /* read message */
    ret = netwox_perf_recv(pio, NETWIB_TIME_INFINITE, &buf, &perfmsg);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      netwib_er(netwib_buf_close(&buf));
      return(ret);
    }
    /* analyze */
    if (perfmsg.perfmsgtype == NETWOX_PERFMSGTYPE_HELLO) {
      datasize = perfmsg.msg.hello.datasize;
      durationms = perfmsg.msg.hello.durationms;
      break;
    }
  }

  /* send data messages */
  perfmsg.perfmsgtype = NETWOX_PERFMSGTYPE_DATA;
  perfmsg.size = datasize;
  perfmsg.msg.data.recvtime.sec = 0;
  perfmsg.msg.data.recvtime.nsec = 0;
  netwib_er(netwib_time_init_now(&endtime));
  netwib_er(netwib_time_plus_msec(&endtime, durationms));
  numdatasent = 0;
  while(NETWIB_TRUE) {
    /* send */
    ret = netwox_perf_send(pio, &buf, &perfmsg);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwib_buf_close(&buf));
      return(ret);
    }
    numdatasent++;
    /* eventually check time */
    if (durationms) {
      netwib_er(netwib_time_iselapsed(&endtime, &haselapsed));
      if (haselapsed) {
        break;
      }
    }
  }

  /* send bye */
  perfmsg.perfmsgtype = NETWOX_PERFMSGTYPE_BYE;
  perfmsg.msg.bye.numdatasent = numdatasent;
  ret = netwox_perf_send(pio, &buf, &perfmsg);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_buf_close(&buf));
    return(ret);
  }

  /* wait byeack for 10 seconds */
  netwib_er(netwib_time_init_now(&endtime));
  netwib_er(netwib_time_plus_sec(&endtime, 10));
  while(NETWIB_TRUE) {
    /* read message */
    ret = netwox_perf_recv(pio, &endtime, &buf, &perfmsg);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      netwib_er(netwib_buf_close(&buf));
      return(ret);
    }
    /* analyze */
    if (perfmsg.perfmsgtype == NETWOX_PERFMSGTYPE_BYEACK) {
      break;
    }
  }

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_perf_diff_usec(netwib_consttime *psend,
                                        netwib_consttime *precv,
                                        netwib_int32 *pdelay)
{
  *pdelay = 0;
  if (precv->sec != psend->sec) {
    *pdelay += 1000000 * (netwib_int32)(precv->sec - psend->sec);
  }
  if (precv->nsec != psend->nsec) {
    *pdelay += (netwib_int32)(precv->nsec - psend->nsec)/1000;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_perf_cli(netwox_perfinfo *pperfinfo)
{
  netwib_buf buf;
  netwox_perfmsg perfmsg;
  netwib_uint32 numdatarecv, numdatasent, totalrecv, jitter, msec, bps;
  netwib_int32 delay, lastdelay, diff;
  netwib_time displaytime, duration, endtime;
  netwib_io_socktype socktype;
  netwib_bool display;
  netwib_err ret;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  /* send hello */
  perfmsg.perfmsgtype = NETWOX_PERFMSGTYPE_HELLO;
  perfmsg.msg.hello.version = 1;
  /* add 200 to have 1000->1200 which will display once at 1 second */
  perfmsg.msg.hello.durationms = pperfinfo->durationms + 200;
  perfmsg.msg.hello.datasize = pperfinfo->size;
  netwib_er(netwox_perf_send(pperfinfo->pio, &buf, &perfmsg));

  /* read data */
  numdatarecv = 0;
  numdatasent = 0;
  totalrecv = 0;
  lastdelay = 0;
  jitter = 0;
  netwib_er(netwib_time_init_now(&displaytime));
  netwib_er(netwib_time_plus_sec(&displaytime, 1));
  netwib_er(netwib_time_init_now(&endtime));
  netwib_er(netwib_time_plus_msec(&endtime, pperfinfo->durationms));
  netwib_er(netwib_time_plus_sec(&endtime, 1)); /* timeout */
  while(NETWIB_TRUE) {
    /* read messages */
    ret = netwox_perf_recv(pperfinfo->pio, &endtime, &buf, &perfmsg);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND || ret == NETWOX_ERR_TIMEOUT) {
        numdatasent = 0xFFFFFFFFu;
        break;
      }
      netwib_er(netwib_buf_close(&buf));
      return(ret);
    }
    /* analyze bye messages */
    if (perfmsg.perfmsgtype == NETWOX_PERFMSGTYPE_BYE) {
      numdatasent = perfmsg.msg.bye.numdatasent;
      perfmsg.perfmsgtype = NETWOX_PERFMSGTYPE_BYEACK;
      ret = netwox_perf_send(pperfinfo->pio, &buf, &perfmsg);
      if (ret != NETWIB_ERR_OK) {
        netwib_er(netwib_buf_close(&buf));
        return(ret);
      }
      break;
    }
    /* analyze data messages */
    if (perfmsg.perfmsgtype != NETWOX_PERFMSGTYPE_DATA) {
      continue;
    }
    /* jitter : rfc 1889
         D(i,j)=(Rj-Ri)-(Sj-Si)=(Rj-Sj)-(Ri-Si)
         J=J+(|D(i-1,i)|-J)/16
       we will compute it in microseconds
    */
    netwib_er(netwox_perf_diff_usec(&perfmsg.msg.data.sendtime,
                                    &perfmsg.msg.data.recvtime, &delay));
    if (numdatarecv) {
      diff = delay-lastdelay;
      if (diff < 0) diff = -diff;
      jitter = jitter + (netwib_int32)(diff-jitter)/16;
    }
    lastdelay = delay;
    numdatarecv++;
    totalrecv += perfmsg.size;
    /* display statistics */
    netwib_er(netwib_time_iselapsed(&displaytime, &display));
    if (display) {
      netwib_er(netwib_time_init_now(&duration));
      netwib_er(netwib_time_minus_time(&duration, &displaytime));
      netwib_er(netwib_time_decode_msec(&duration, &msec));
      msec += 1000;
      bps = (netwib_uint32)((float)(1000.0*totalrecv)/msec);
      totalrecv = 0;
      netwib_er(netwib_fmt_display("%{r 10;uint32} Bytes/sec ", bps));
      /* Real value should be 8*bps/1024 + EthIpTCP + TCPAck + etc.
         but we use an approximate value. Formula "19bps/2120" comes
         from my own measure (Ethernet sniff of a 10Mb LAN).
         This is not really accurate, but it's better than 8*bps/1024 */
      netwib_er(netwib_fmt_display("[=~= %{r 7;uint32} kbit/sec]    ",
                                   19*bps/2120));
      netwib_er(netwib_fmt_display("jitter=%{r 8;uint32} usec\n", jitter));
      netwib_er(netwib_time_init_now(&displaytime));
      netwib_er(netwib_time_plus_sec(&displaytime, 1));
    }
  }

  if (numdatarecv == 0) {
    netwib_er(netwib_fmt_display("No packet was received (did you setup the server side ?).\n"));
  } else {
    netwib_er(netwib_sock_ctl_get_socktype(pperfinfo->pio, &socktype));
    if (numdatasent == 0xFFFFFFFFu) {
      netwib_er(netwib_fmt_display("No packet received during last second: "));
      if (socktype == NETWIB_IO_SOCKTYPE_UDP_CLI) {
        netwib_er(netwib_fmt_display("the BYE packet was perhaps lost.\n"));
      } else {
        netwib_er(netwib_fmt_display("connection was perhaps lost.\n"));
      }
    }
    if (socktype == NETWIB_IO_SOCKTYPE_UDP_CLI) {
      netwib_er(netwib_fmt_display("Packets sent by server: "));
      if (numdatasent == 0xFFFFFFFFu) {
        netwib_er(netwib_fmt_display("unknown (no BYE packet)\n"));
      } else {
        netwib_er(netwib_fmt_display("%{uint32}\n", numdatasent));
      }
      netwib_er(netwib_fmt_display("Packets recv by client: %{uint32}\n",
                                   numdatarecv));
    }
  }

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

