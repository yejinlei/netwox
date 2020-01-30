/*
                                  NETWIB
                             Network library
                Copyright(c) 1999-2012 Laurent CONSTANTIN
                      http://ntwib.sourceforge.net/
                        laurentconstantin@free.fr
                                  -----
  This file is part of Netwib.

  Netwib is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3
  as published by the Free Software Foundation.

  Netwib is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details (http://www.gnu.org/).

------------------------------------------------------------------------
*/

#include <netwib/inc/maininc.h>

/*-------------------------------------------------------------*/
netwib_err netwib_tcpopt_initdefault(netwib_tcpopttype tcpopttype,
                                     netwib_tcpopt *ptcpopt)
{
  netwib_uint32 i;

  ptcpopt->type = tcpopttype;
  switch(tcpopttype) {
  case NETWIB_TCPOPTTYPE_END :
    break;
  case NETWIB_TCPOPTTYPE_NOOP :
    break;
  case NETWIB_TCPOPTTYPE_MSS :
    ptcpopt->opt.mss.maxsegsize = 0;
    break;
  case NETWIB_TCPOPTTYPE_WINDOWSCALE :
    ptcpopt->opt.windowscale.windowscale = 1;
    break;
  case NETWIB_TCPOPTTYPE_SACKPERMITTED :
    break;
  case NETWIB_TCPOPTTYPE_SACK :
    ptcpopt->opt.sack.storedvalues = 0;
    for (i = 0; i < 4; i++) {
      ptcpopt->opt.sack.leftedge[i] = 0;
      ptcpopt->opt.sack.rightedge[i] = 0;
    }
    break;
  case NETWIB_TCPOPTTYPE_ECHOREQUEST :
    netwib_er(netwib_uint32_init_rand(1, 0xFFFF, &ptcpopt->opt.echo.data));
    break;
  case NETWIB_TCPOPTTYPE_ECHOREPLY :
    ptcpopt->opt.echo.data = 0;
    break;
  case NETWIB_TCPOPTTYPE_TIMESTAMP :
    ptcpopt->opt.timestamp.val = 0;
    ptcpopt->opt.timestamp.echoreply = 0;
    break;
  case NETWIB_TCPOPTTYPE_CC :
  case NETWIB_TCPOPTTYPE_CCNEW :
  case NETWIB_TCPOPTTYPE_CCECHO :
    ptcpopt->opt.cc.connectioncount = 0;
    break;
  default :
    return(NETWIB_ERR_LONOTIMPLEMENTED);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_tcpopt(netwib_consttcpopt *ptcpopt,
                                    netwib_buf *ppkt)
{
  netwib_data data;
  netwib_uint32 i, length;

  switch(ptcpopt->type) {
  case NETWIB_TCPOPTTYPE_END :
  case NETWIB_TCPOPTTYPE_NOOP :
    length = 1;
    netwib_er(netwib_buf_wantspace(ppkt, length, &data));
    netwib__data_append_uint8(data, ptcpopt->type);
    ppkt->endoffset += length;
    break;
    break;
  case NETWIB_TCPOPTTYPE_MSS :
    length = 4;
    netwib_er(netwib_buf_wantspace(ppkt, length, &data));
    netwib__data_append_uint8(data, ptcpopt->type);
    netwib__data_append_uint8(data, length);
    netwib__data_append_uint16(data, ptcpopt->opt.mss.maxsegsize);
    ppkt->endoffset += length;
    break;
  case NETWIB_TCPOPTTYPE_WINDOWSCALE :
    length = 3;
    netwib_er(netwib_buf_wantspace(ppkt, length, &data));
    netwib__data_append_uint8(data, ptcpopt->type);
    netwib__data_append_uint8(data, length);
    netwib__data_append_uint8(data, ptcpopt->opt.windowscale.windowscale);
    ppkt->endoffset += length;
    break;
  case NETWIB_TCPOPTTYPE_SACKPERMITTED :
    length = 2;
    netwib_er(netwib_buf_wantspace(ppkt, length, &data));
    netwib__data_append_uint8(data, ptcpopt->type);
    netwib__data_append_uint8(data, length);
    ppkt->endoffset += length;
    break;
  case NETWIB_TCPOPTTYPE_SACK :
    if (ptcpopt->opt.sack.storedvalues > 4) {
      return(NETWIB_ERR_PATOOBIGFORHDR);
    }
    length = 2 + 8*ptcpopt->opt.sack.storedvalues;
    netwib_er(netwib_buf_wantspace(ppkt, length, &data));
    netwib__data_append_uint8(data, ptcpopt->type);
    netwib__data_append_uint8(data, length);
    for (i = 0; i < ptcpopt->opt.sack.storedvalues; i++) {
      netwib__data_append_uint32(data, ptcpopt->opt.sack.leftedge[i]);
      netwib__data_append_uint32(data, ptcpopt->opt.sack.rightedge[i]);
    }
    ppkt->endoffset += length;
    break;
  case NETWIB_TCPOPTTYPE_ECHOREQUEST :
  case NETWIB_TCPOPTTYPE_ECHOREPLY :
    length = 6;
    netwib_er(netwib_buf_wantspace(ppkt, length, &data));
    netwib__data_append_uint8(data, ptcpopt->type);
    netwib__data_append_uint8(data, length);
    netwib__data_append_uint32(data, ptcpopt->opt.echo.data);
    ppkt->endoffset += length;
    break;
  case NETWIB_TCPOPTTYPE_TIMESTAMP :
    length = 10;
    netwib_er(netwib_buf_wantspace(ppkt, length, &data));
    netwib__data_append_uint8(data, ptcpopt->type);
    netwib__data_append_uint8(data, length);
    netwib__data_append_uint32(data, ptcpopt->opt.timestamp.val);
    netwib__data_append_uint32(data, ptcpopt->opt.timestamp.echoreply);
    ppkt->endoffset += length;
    break;
  case NETWIB_TCPOPTTYPE_CC :
  case NETWIB_TCPOPTTYPE_CCNEW :
  case NETWIB_TCPOPTTYPE_CCECHO :
    length = 6;
    netwib_er(netwib_buf_wantspace(ppkt, length, &data));
    netwib__data_append_uint8(data, ptcpopt->type);
    netwib__data_append_uint8(data, length);
    netwib__data_append_uint32(data, ptcpopt->opt.cc.connectioncount);
    ppkt->endoffset += length;
    break;
  default :
    return(NETWIB_ERR_LONOTIMPLEMENTED);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_prepend_tcpopt(netwib_consttcpopt *ptcpopt,
                                     netwib_buf *ppkt)
{
  netwib_byte array[NETWIB_TCPOPT_MAXLEN];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));
  ret = netwib_pkt_append_tcpopt(ptcpopt, &buf);
  if (ret == NETWIB_ERR_OK) {
    ret = netwib_buf_prepend_buf(&buf, ppkt);
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_tcpopt(netwib_constbuf *ppkt,
                                    netwib_tcpopt *ptcpopt,
                                    netwib_uint32 *pskipsize)
{
  netwib_data data;
  netwib_uint32 datasize, length, i;

  datasize = netwib__buf_ref_data_size(ppkt);
  if (pskipsize != NULL) *pskipsize = datasize;
  if (datasize == 0) {
    return(NETWIB_ERR_DATAMISSING);
  }
  data = netwib__buf_ref_data_ptr(ppkt);
  netwib__data_decode_uint8(data, ptcpopt->type);

  if (ptcpopt->type == NETWIB_TCPOPTTYPE_END ||
      ptcpopt->type == NETWIB_TCPOPTTYPE_NOOP) {
    if (pskipsize != NULL) *pskipsize = 1;
    return(NETWIB_ERR_OK);
  }

  if (datasize == 1) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwib__data_decode_uint8(data, length);
  if (length > datasize) {
    return(NETWIB_ERR_DATAMISSING);
  }
  if (pskipsize != NULL) *pskipsize = length;

  switch(ptcpopt->type) {
  case NETWIB_TCPOPTTYPE_END :
  case NETWIB_TCPOPTTYPE_NOOP :
    return(NETWIB_ERR_LOINTERNALERROR);
    break;
  case NETWIB_TCPOPTTYPE_MSS :
    if (length != 4) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16(data, ptcpopt->opt.mss.maxsegsize);
    break;
  case NETWIB_TCPOPTTYPE_WINDOWSCALE :
    if (length != 3) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint8(data, ptcpopt->opt.windowscale.windowscale);
    break;
  case NETWIB_TCPOPTTYPE_SACKPERMITTED :
    if (length != 2) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWIB_TCPOPTTYPE_SACK :
    length -= 2;
    if (length % 8) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    ptcpopt->opt.sack.storedvalues = length / 8;
    if (ptcpopt->opt.sack.storedvalues > 4) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    for (i = 0; i < ptcpopt->opt.sack.storedvalues; i++) {
      netwib__data_decode_uint32(data, ptcpopt->opt.sack.leftedge[i]);
      netwib__data_decode_uint32(data, ptcpopt->opt.sack.rightedge[i]);
    }
    break;
  case NETWIB_TCPOPTTYPE_ECHOREQUEST :
  case NETWIB_TCPOPTTYPE_ECHOREPLY :
    if (length != 6) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint32(data, ptcpopt->opt.echo.data);
    break;
  case NETWIB_TCPOPTTYPE_TIMESTAMP :
    if (length != 10) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint32(data, ptcpopt->opt.timestamp.val);
    netwib__data_decode_uint32(data, ptcpopt->opt.timestamp.echoreply);
    break;
  case NETWIB_TCPOPTTYPE_CC :
  case NETWIB_TCPOPTTYPE_CCNEW :
  case NETWIB_TCPOPTTYPE_CCECHO :
    if (length != 6) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint32(data, ptcpopt->opt.cc.connectioncount);
    break;
  default :
    return(NETWIB_ERR_LONOTIMPLEMENTED);
    break;
  }

  return(NETWIB_ERR_OK);
}
