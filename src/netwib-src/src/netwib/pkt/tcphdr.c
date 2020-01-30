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
netwib_err netwib_tcphdr_initdefault(netwib_tcphdr *ptcphdr)
{
  if (ptcphdr != NULL) {
    ptcphdr->src = 0;
    ptcphdr->dst = 0;
    netwib_er(netwib_uint32_init_rand(1, 0xFFFFFFFFu, &ptcphdr->seqnum));
    ptcphdr->acknum = 0;
    ptcphdr->doff = NETWIB_TCPHDR_MINLEN/4;
    ptcphdr->reserved1 = NETWIB_FALSE;
    ptcphdr->reserved2 = NETWIB_FALSE;
    ptcphdr->reserved3 = NETWIB_FALSE;
    ptcphdr->reserved4 = NETWIB_FALSE;
    ptcphdr->cwr = NETWIB_FALSE;
    ptcphdr->ece = NETWIB_FALSE;
    ptcphdr->urg = NETWIB_FALSE;
    ptcphdr->ack = NETWIB_FALSE;
    ptcphdr->psh = NETWIB_FALSE;
    ptcphdr->rst = NETWIB_FALSE;
    ptcphdr->syn = NETWIB_FALSE;
    ptcphdr->fin = NETWIB_FALSE;
    ptcphdr->window = 1500;
    ptcphdr->check = 0;
    ptcphdr->urgptr = 0;
    netwib_er(netwib_buf_init_ext_empty(&ptcphdr->opts));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_tcphdr(netwib_consttcphdr *ptcphdr,
                                    netwib_buf *ppkt)
{
  netwib_data data;
  netwib_byte b;
  netwib_uint32 tcpoptssize;

  if (ptcphdr->src > 0xFFFF) {
    return(NETWIB_ERR_PATOOBIGFORHDR);
  }
  if (ptcphdr->dst > 0xFFFF) {
    return(NETWIB_ERR_PATOOBIGFORHDR);
  }
  if (ptcphdr->doff > 0xF) {
    return(NETWIB_ERR_PATOOBIGFORHDR);
  }
  tcpoptssize = netwib__buf_ref_data_size(&ptcphdr->opts);
  if (tcpoptssize != 0) {
    if (tcpoptssize % 4) {
      return(NETWIB_ERR_PATCPOPTSNOTX4);
    }
    if (tcpoptssize > 40) {
      return(NETWIB_ERR_PATCPOPTSMAX10);
    }
  }

  netwib_er(netwib_buf_wantspace(ppkt, NETWIB_TCPHDR_MINLEN, &data));

  netwib__data_append_uint16(data, ptcphdr->src);
  netwib__data_append_uint16(data, ptcphdr->dst);
  netwib__data_append_uint32(data, ptcphdr->seqnum);
  netwib__data_append_uint32(data, ptcphdr->acknum);
  b = (netwib_byte)(ptcphdr->doff << 4);
  if (ptcphdr->reserved1) b |= 0x8;
  if (ptcphdr->reserved2) b |= 0x4;
  if (ptcphdr->reserved3) b |= 0x2;
  if (ptcphdr->reserved4) b |= 0x1;
  netwib__data_append_uint8(data, b);
  b = 0;
  if (ptcphdr->cwr) b |= 0x80;
  if (ptcphdr->ece) b |= 0x40;
  if (ptcphdr->urg) b |= 0x20;
  if (ptcphdr->ack) b |= 0x10;
  if (ptcphdr->psh) b |= 0x08;
  if (ptcphdr->rst) b |= 0x04;
  if (ptcphdr->syn) b |= 0x02;
  if (ptcphdr->fin) b |= 0x01;
  netwib__data_append_uint8(data, b);
  netwib__data_append_uint16(data, ptcphdr->window);
  netwib__data_append_uint16(data, ptcphdr->check);
  netwib__data_append_uint16(data, ptcphdr->urgptr);

  ppkt->endoffset += NETWIB_TCPHDR_MINLEN;

  if (tcpoptssize != 0) {
    netwib_er(netwib_buf_append_buf(&ptcphdr->opts, ppkt));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_prepend_tcphdr(netwib_consttcphdr *ptcphdr,
                                     netwib_buf *ppkt)
{
  netwib_byte array[NETWIB_TCPHDR_MAXLEN];
  netwib_buf buf;

  netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));
  netwib_er(netwib_pkt_append_tcphdr(ptcphdr, &buf));
  netwib_er(netwib_buf_prepend_buf(&buf, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_tcphdr(netwib_constbuf *ppkt,
                                    netwib_tcphdr *ptcphdr,
                                    netwib_uint32 *pskipsize)
{
  netwib_data data;
  netwib_uint32 datasize, doff4;
  netwib_byte b;

  datasize = netwib__buf_ref_data_size(ppkt);
  if (datasize < NETWIB_TCPHDR_MINLEN) {
    return(NETWIB_ERR_DATAMISSING);
  }
  data = netwib__buf_ref_data_ptr(ppkt);
  doff4 = 4 * (data[12]>>4);
  if (datasize < doff4) {
    return(NETWIB_ERR_DATAMISSING);
  }

  if (ptcphdr != NULL) {
    netwib__data_decode_uint16(data, ptcphdr->src);
    netwib__data_decode_uint16(data, ptcphdr->dst);
    netwib__data_decode_uint32(data, ptcphdr->seqnum);
    netwib__data_decode_uint32(data, ptcphdr->acknum);
    netwib__data_decode_uint8(data, b);
    ptcphdr->doff = (netwib_byte)(b >> 4);
    ptcphdr->reserved1 = (b&0x8)?NETWIB_TRUE:NETWIB_FALSE;
    ptcphdr->reserved2 = (b&0x4)?NETWIB_TRUE:NETWIB_FALSE;
    ptcphdr->reserved3 = (b&0x2)?NETWIB_TRUE:NETWIB_FALSE;
    ptcphdr->reserved4 = (b&0x1)?NETWIB_TRUE:NETWIB_FALSE;
    netwib__data_decode_uint8(data, b);
    ptcphdr->cwr = (b&0x80)?NETWIB_TRUE:NETWIB_FALSE;
    ptcphdr->ece = (b&0x40)?NETWIB_TRUE:NETWIB_FALSE;
    ptcphdr->urg = (b&0x20)?NETWIB_TRUE:NETWIB_FALSE;
    ptcphdr->ack = (b&0x10)?NETWIB_TRUE:NETWIB_FALSE;
    ptcphdr->psh = (b&0x8)?NETWIB_TRUE:NETWIB_FALSE;
    ptcphdr->rst = (b&0x4)?NETWIB_TRUE:NETWIB_FALSE;
    ptcphdr->syn = (b&0x2)?NETWIB_TRUE:NETWIB_FALSE;
    ptcphdr->fin = (b&0x1)?NETWIB_TRUE:NETWIB_FALSE;
    netwib__data_decode_uint16(data, ptcphdr->window);
    netwib__data_decode_uint16(data, ptcphdr->check);
    netwib__data_decode_uint16(data, ptcphdr->urgptr);
    if (doff4 > NETWIB_TCPHDR_MINLEN) {
      netwib_er(netwib_buf_init_ext_arrayfilled(data,
                                                doff4 - NETWIB_TCPHDR_MINLEN,
                                                &ptcphdr->opts));
    } else {
      netwib_er(netwib_buf_init_ext_empty(&ptcphdr->opts));
    }
  }

  if (pskipsize != NULL) {
    if (doff4 < NETWIB_TCPHDR_MINLEN) {
      /* invalid, but more logical */
      *pskipsize = NETWIB_TCPHDR_MINLEN;
    } else {
      *pskipsize = doff4;
    }
  }

  return(NETWIB_ERR_OK);
}
