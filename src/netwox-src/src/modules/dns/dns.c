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
netwib_err netwox_dns_inaddrarpa(netwib_constip *pip,
                                 netwib_buf *pbuf)
{
  netwib_data data;
  netwib_uint32 quartet;
  netwib_int32 i;

  switch(pip->iptype) {
  case NETWIB_IPTYPE_IP4 :
    netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32}.%{uint32}.%{uint32}.%{uint32}.in-addr.arpa.", netwib_c2_uint32_3(pip->ipvalue.ip4), netwib_c2_uint32_2(pip->ipvalue.ip4), netwib_c2_uint32_1(pip->ipvalue.ip4), netwib_c2_uint32_0(pip->ipvalue.ip4)));
    break;
  case NETWIB_IPTYPE_IP6 :
    netwib_er(netwib_buf_wantspace(pbuf, 64, &data));
    for (i = 15; i >= 0; i--) {
      quartet = pip->ipvalue.ip6.b[i] & 0xF;
      *data++ = netwib_c2_16toc(quartet);
      *data++ = '.';
      quartet = pip->ipvalue.ip6.b[i] >> 4;
      *data++ = netwib_c2_16toc(quartet);
      *data++ = '.';
    }
    pbuf->endoffset += 64;
    netwib_er(netwib_buf_append_string("ip6.int.", pbuf));
    break;
  default :
    return(NETWIB_ERR_PAIPTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_dns_query_udp(netwib_bufpool *pbufpool,
                                       netwib_io *pio,
                                       netwib_constbuf *pquestion,
                                       netwib_consttime *pabstime,
                                       netwox_dns_query_pf pfunc,
                                       netwib_ptr infos)
{
  netwib_buf *pbuf;
  netwib_bool event;
  netwib_uint16 questionid, answerid;
  netwib_err ret=NETWIB_ERR_OK;

  /* send question */
  netwib_er(netwox_pkt_decode_dnsid(pquestion, &questionid));
  netwib_er(netwib_io_write(pio, pquestion));

  /* expect answer */
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
  pbuf->flags |= NETWIB_BUF_FLAGS_CANSLIDE;
  while (NETWIB_TRUE) {
    /* read */
    netwib_er(netwib_io_wait_read(pio, pabstime, &event));
    if (!event) break;
    ret = netwib_io_read(pio, pbuf);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    }
    else if (ret != NETWIB_ERR_OK) break;
    /* check this packet */
    ret = netwox_pkt_decode_dnsid(pbuf, &answerid);
    if (ret == NETWIB_ERR_OK) {
      if (answerid == questionid) {
        /* in UDP, there is only one answer */
        ret = (*pfunc)(pbufpool, pbuf, infos);
        break;
      }
    }
    netwib__buf_reinit(pbuf);
  }
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_dns_query_tcp(netwib_bufpool *pbufpool,
                                       netwib_io *pio,
                                       netwib_constbuf *pquestion,
                                       netwib_consttime *pabstime,
                                       netwox_dns_query_pf pfunc,
                                       netwib_ptr infos)
{
  netwib_buf *pbuf, dnspkt;
  netwib_bool event, needtoreadmore;
  netwib_uint32 previoussize, skipsize;
  netwib_uint16 questionid, answerid;
  netwib_time endtime;
  netwib_consttime *ptime;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));

  /* send question */
  netwib_er(netwox_pkt_decode_dnsid(pquestion, &questionid));
  netwib_er(netwox_pkt_append_tcpdns_prepare(pbuf, &previoussize));
  ret = netwib_buf_append_buf(pquestion, pbuf);
  netwib_er(netwox_pkt_append_tcpdns_finish(pbuf, previoussize, ret));
  netwib_er(netwib_io_write(pio, pbuf));

  /* expect answer */
  netwib__buf_reinit(pbuf);
  pbuf->flags |= NETWIB_BUF_FLAGS_CANSLIDE;
  needtoreadmore = NETWIB_TRUE;
  ptime = pabstime;
  while (NETWIB_TRUE) {
    /* read */
    if (needtoreadmore) {
      ret = NETWIB_ERR_OK;
      netwib_er(netwib_io_wait_read(pio, ptime, &event));
      if (!event) break;
      ret = netwib_io_read(pio, pbuf);
      if (ret == NETWIB_ERR_DATANOTAVAIL) {
        ret = NETWIB_ERR_OK;
        continue;
      }
      else if (ret != NETWIB_ERR_OK) break;
      needtoreadmore = NETWIB_FALSE;
    }
    /* check this packet */
    ret = netwox_pkt_decode_tcpdns(pbuf, &dnspkt, &skipsize);
    if (ret == NETWIB_ERR_OK) {
      ret = netwox_pkt_decode_dnsid(&dnspkt, &answerid);
      if (ret == NETWIB_ERR_OK) {
        if (answerid == questionid) {
          ret = (*pfunc)(pbufpool, &dnspkt, infos);
          if (ret != NETWIB_ERR_OK) break;
        }
      }
      pbuf->beginoffset += skipsize;
    } else {
      needtoreadmore = NETWIB_TRUE;
      /* apparently, using TCP, client has no way to determine if
         server has sent all its data. so wait wait for 100ms. */
      netwib_er(netwib_time_init_now(&endtime));
      netwib_er(netwib_time_plus_msec(&endtime, 100));
      ptime = &endtime;
    }
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dns_query(netwib_bufpool *pbufpool,
                            netwib_io *pio,
                            netwib_bool ioistcp,
                            netwib_constbuf *pquestion,
                            netwib_consttime *pabstime,
                            netwox_dns_query_pf pfunc,
                            netwib_ptr infos)
{

  if (ioistcp) {
    netwib_er(netwox_dns_query_tcp(pbufpool, pio, pquestion, pabstime, pfunc,
                                   infos));
  } else {
    netwib_er(netwox_dns_query_udp(pbufpool, pio, pquestion, pabstime, pfunc,
                                   infos));
  }

  return(NETWIB_ERR_OK);
}

