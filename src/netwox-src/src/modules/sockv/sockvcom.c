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
netwib_err netwox_sockv_info_write_err(netwox_priv_io_sockv *pisv,
                                       netwib_err err)
{
  netwib_err ret;

  netwib_er(netwib_thread_rwlock_wrlock(pisv->pinfolockrd,
                                        NETWIB_TIME_INFINITE, NULL));

  ret = netwib_tlv_append_uint32(err, &pisv->infobufrd);

  netwib_er(netwib_thread_rwlock_wrunlock(pisv->pinfolockrd));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_sockv_info_read_err(netwox_priv_io_sockv *pisv,
                                      netwib_err *perr)
{
  netwib_uint32 skipsize;
  netwib_err ret;

  ret = NETWIB_ERR_DATAEND;
  while (ret == NETWIB_ERR_DATAEND) {
    netwib_er(netwib_thread_rwlock_rdlock(pisv->pinfolockrd,
                                          NETWIB_TIME_INFINITE, NULL));
    ret = netwib_tlv_decode_uint32(&pisv->infobufrd, (netwib_uint32*)perr,
                                   &skipsize);
    if (ret == NETWIB_ERR_OK) {
      pisv->infobufrd.beginoffset += skipsize;
    }
    netwib_er(netwib_thread_rwlock_rdunlock(pisv->pinfolockrd));
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_sockv_info_read_err_avail(netwox_priv_io_sockv *pisv,
                                            netwib_bool *pevent)
{
  netwib_err ret;

  netwib_er(netwib_thread_rwlock_rdlock(pisv->pinfolockrd,
                                        NETWIB_TIME_INFINITE, NULL));
  ret = netwib_tlv_decode_uint32(&pisv->infobufrd, NULL, NULL);
  netwib_er(netwib_thread_rwlock_rdunlock(pisv->pinfolockrd));
  if (ret == NETWIB_ERR_OK) {
    if (pevent != NULL) *pevent = NETWIB_TRUE;
  } else {
    if (pevent != NULL) *pevent = NETWIB_FALSE;
    ret = NETWIB_ERR_PLEASELOOPTIME;
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_sockv_info_write_cli(netwox_priv_io_sockv *pisv,
                                       netwib_bool fromchildtofather,
                                       netwib_consteth *pclieth,
                                       netwib_constip *pcliip,
                                       netwib_port cliport)
{
  netwib_thread_rwlock *prwlock;
  netwib_buf *pbuf;
  netwib_err ret;

  prwlock = pisv->pinfolockwr;
  pbuf = &pisv->infobufwr;
  if (fromchildtofather) {
    prwlock = pisv->pinfolockrd;
    pbuf = &pisv->infobufrd;
  }

  netwib_er(netwib_thread_rwlock_wrlock(prwlock, NETWIB_TIME_INFINITE, NULL));

  ret = netwib_tlv_append_eth(pclieth, pbuf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_tlv_append_ip(pcliip, pbuf));
    netwib_er(netwib_tlv_append_uint32(cliport, pbuf));
  }

  netwib_er(netwib_thread_rwlock_wrunlock(prwlock));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_sockv_info_read_cli(netwox_priv_io_sockv *pisv,
                                      netwib_bool fromchildtofather,
                                      netwib_eth *pclieth,
                                      netwib_ip *pcliip,
                                      netwib_port *pcliport)
{
  netwib_uint32 skipsize;
  netwib_thread_rwlock *prwlock;
  netwib_buf *pbuf;
  netwib_err ret;

  prwlock = pisv->pinfolockwr;
  pbuf = &pisv->infobufwr;
  if (fromchildtofather) {
    prwlock = pisv->pinfolockrd;
    pbuf = &pisv->infobufrd;
  }

  ret = NETWIB_ERR_DATAEND;
  while (ret == NETWIB_ERR_DATAEND) {
    netwib_er(netwib_thread_rwlock_rdlock(prwlock, NETWIB_TIME_INFINITE,
                                          NULL));
    ret = netwib_tlv_decode_eth(pbuf, pclieth, &skipsize);
    if (ret == NETWIB_ERR_OK) {
      pbuf->beginoffset += skipsize;
      netwib_er(netwib_tlv_decode_ip(pbuf, pcliip, &skipsize));
      pbuf->beginoffset += skipsize;
      netwib_er(netwib_tlv_decode_uint32(pbuf, pcliport, &skipsize));
      pbuf->beginoffset += skipsize;
    }
    netwib_er(netwib_thread_rwlock_rdunlock(prwlock));
  }

  return(ret);
}
