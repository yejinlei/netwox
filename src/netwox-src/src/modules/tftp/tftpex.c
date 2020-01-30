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
netwib_err netwox_tftp_cli2ser(netwox_sockinfo *psockinfo,
                               netwib_io **ppio)
{
  netwox_sockinfo newsockinfo;
  netwib_port localport;
  netwib_err ret;

  netwib_er(netwib_sock_ctl_get_local(*ppio, NULL, &localport));
  netwib_er(netwox_sockinfo_init_listen(psockinfo, *ppio, localport,
                                        &newsockinfo));

  netwib_er(netwib_io_close(ppio));

  /* if server's answer is fast, and before this point, the TFTP client
     won't work */
  ret = netwox_sock_init(&newsockinfo, ppio);

  netwib_er(netwox_sockinfo_close(&newsockinfo));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_tftp_send(netwox_tftpinfos *ptftpinfos,
                            netwox_consttftppkt *ptftppktsend)
{
  netwib_buf *ppkt;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(ptftpinfos->pbufpool, &ppkt));
  netwib_er(netwox_pkt_append_tftppkt(ptftppktsend, ppkt));
  ret = netwib_io_write(ptftpinfos->pio, ppkt);
  netwib_er(netwib_bufpool_buf_close(ptftpinfos->pbufpool, &ppkt));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_tftp_recv(netwox_tftpinfos *ptftpinfos,
                            netwox_tftppkt *ptftppktrecv)
{
  netwib_buf *ppkt;
  netwib_time t;
  netwib_bool event;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_bufpool_buf_init(ptftpinfos->pbufpool, &ppkt));

  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_plus_msec(&t, ptftpinfos->maxwaitms));
  while(NETWIB_TRUE) {
    netwib__buf_reinit(ppkt);
    ret = netwib_io_wait_read(ptftpinfos->pio, &t, &event);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    if (!event) {
      ret = NETWOX_ERR_TIMEOUT;
      break;
    }
    ret = netwib_io_read(ptftpinfos->pio, ppkt);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATANOTAVAIL) {
        ret = NETWIB_ERR_OK;
        continue;
      }
      if (ret == NETWIB_ERR_DATAEND) break;
      break;
    }
    ret = netwox_pkt_decode_tftppkt(ppkt, ptftppktrecv);
    if (ret == NETWIB_ERR_OK) {
      break;
    }
  }

  netwib_er(netwib_bufpool_buf_close(ptftpinfos->pbufpool, &ppkt));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_tftp_write(netwox_tftpinfos *ptftpinfos,
                             netwib_io *plocalio,
                             netwib_bool server)
{
  netwox_tftppkt tftppktsend, tftppktrecv;
  netwib_uint32 numtry;
  netwib_uint16 wantedblock;
  netwib_io *pio;
  netwib_buf *pbuf;
  netwib_bool canrecv, lastsent;
  netwib_err ret=NETWIB_ERR_OK;

  if (ptftpinfos->retry == 0) {
    ptftpinfos->retry = 1;
  }

  netwib_er(netwib_bufpool_buf_init(ptftpinfos->pbufpool, &pbuf));
  netwib_er(netwib_io_init_data(NETWIB_IO_INIT_DATA_TYPE_FIXED,
                                NETWIB_IO_INIT_DATA_TYPE_TRANSPARENT, &pio));
  netwib_er(netwib_io_ctl_set_data_fixed_size(pio, NETWIB_IO_WAYTYPE_READ,
                                              512));
  netwib_er(netwib_io_plug_read(pio, plocalio));

  canrecv = NETWIB_TRUE;
  wantedblock = 0;
  if (server) {
    canrecv = NETWIB_FALSE;
    tftppktrecv.opcode = NETWOX_TFTPOPCODE_ACK;
    tftppktrecv.pkt.data.block = 0;
  }

  lastsent = NETWIB_FALSE;
  tftppktrecv.opcode = NETWOX_TFTPOPCODE_ACK; /* compiler warning */
  tftppktrecv.pkt.error.code = 0; /* compiler warning */
  while(NETWIB_TRUE) {
    if (canrecv) {
      for (numtry = 0; numtry < ptftpinfos->retry; numtry++) {
        ret = netwox_tftp_recv(ptftpinfos, &tftppktrecv);
        if (ret == NETWOX_ERR_TIMEOUT) {
          netwib_eg(netwox_tftp_send(ptftpinfos, &tftppktsend));
        } else if (ret == NETWIB_ERR_OK) {
          break;
        } else {
          netwib_eg(ret);
        }
      }
    }
    if (tftppktrecv.opcode == NETWOX_TFTPOPCODE_ACK) {
      if (tftppktrecv.pkt.data.block == wantedblock) {
        if (lastsent) {
          break;
        }
        netwib__buf_reinit(pbuf);
        do {
          ret = netwib_io_read(pio, pbuf);
        } while (ret == NETWIB_ERR_DATANOTAVAIL);
        if (ret == NETWIB_ERR_DATAEND) {
          netwib__buf_reinit(pbuf);
          ret = NETWIB_ERR_OK;
        } else if (ret != NETWIB_ERR_OK) {
          netwib_eg(ret);
        }
        tftppktsend.opcode = NETWOX_TFTPOPCODE_DATA;
        wantedblock++;
        tftppktsend.pkt.data.block = wantedblock;
        netwib_eg(netwib_buf_init_ext_buf(pbuf, &tftppktsend.pkt.data.data));
        netwib_eg(netwox_tftp_send(ptftpinfos, &tftppktsend));
        if (netwib__buf_ref_data_size(pbuf) < 512) {
          lastsent = NETWIB_TRUE;
        }
        canrecv = NETWIB_TRUE;
      }
    } else if (tftppktrecv.opcode == NETWOX_TFTPOPCODE_ERROR) {
      netwib_eg(netwib_fmt_display("Received error %{uint16}: %{buf}\n",
                                   tftppktrecv.pkt.error.code,
                                   &tftppktrecv.pkt.error.msg));
      netwib_eg(NETWOX_ERR_PROTOCOL);
    } else {
      netwib_eg(NETWOX_ERR_PROTOCOL);
    }
  }

 netwib_gotolabel:
  netwib_er(netwib_io_unplug_next(pio, NETWIB_IO_WAYTYPE_READ, NULL));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_bufpool_buf_close(ptftpinfos->pbufpool, &pbuf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_tftp_read(netwox_tftpinfos *ptftpinfos,
                            netwib_io *plocalio,
                            netwib_bool server)
{
  netwox_tftppkt tftppktsend, tftppktrecv;
  netwib_uint32 numtry;
  netwib_uint16 wantedblock;
  netwib_bool canresend;
  netwib_err ret=NETWIB_ERR_OK;

  if (ptftpinfos->retry == 0) {
    ptftpinfos->retry = 1;
  }

  canresend = NETWIB_FALSE;
  if (server) {
    tftppktsend.opcode = NETWOX_TFTPOPCODE_ACK;
    tftppktsend.pkt.ack.block = 0;
    netwib_er(netwox_tftp_send(ptftpinfos, &tftppktsend));
    canresend = NETWIB_TRUE;
  }
  wantedblock = 1;

  tftppktrecv.opcode = NETWOX_TFTPOPCODE_ACK; /* compiler warning */
  tftppktrecv.pkt.error.code = 0; /* compiler warning */
  while(NETWIB_TRUE) {
    for (numtry = 0; numtry < ptftpinfos->retry; numtry++) {
      ret = netwox_tftp_recv(ptftpinfos, &tftppktrecv);
      if (ret == NETWOX_ERR_TIMEOUT) {
        if (!canresend) {
          return(ret);
        }
        netwib_er(netwox_tftp_send(ptftpinfos, &tftppktsend));
      } else if (ret == NETWIB_ERR_OK) {
        break;
      } else {
        return(ret);
      }
    }
    if (tftppktrecv.opcode == NETWOX_TFTPOPCODE_DATA) {
      if (tftppktrecv.pkt.data.block == wantedblock) {
        netwib_er(netwib_io_write(plocalio, &tftppktrecv.pkt.data.data));
        tftppktsend.opcode = NETWOX_TFTPOPCODE_ACK;
        tftppktsend.pkt.ack.block = tftppktrecv.pkt.data.block;
        netwib_er(netwox_tftp_send(ptftpinfos, &tftppktsend));
        if (netwib__buf_ref_data_size(&tftppktrecv.pkt.data.data) < 512) {
          break;
        }
        wantedblock++;
        canresend = NETWIB_TRUE;
      }
    } else if (tftppktrecv.opcode == NETWOX_TFTPOPCODE_ERROR) {
      netwib_er(netwib_fmt_display("Received error %{uint16}: %{buf}\n",
                                   tftppktrecv.pkt.error.code,
                                   &tftppktrecv.pkt.error.msg));
      return(NETWOX_ERR_PROTOCOL);
    } else {
      return(NETWOX_ERR_PROTOCOL);
    }
  }

  return(ret);
}
