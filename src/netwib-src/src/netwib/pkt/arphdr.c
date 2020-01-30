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
netwib_err netwib_arphdr_initdefault(netwib_arphdr *parphdr)
{

  if (parphdr != NULL) {
    parphdr->op = (netwib_arphdrop)0;
    netwib_er(netwib_eth_init_fields(0,0,0,0,0,0, &parphdr->ethsrc));
    netwib_er(netwib_ip_init_ip4(0, &parphdr->ipsrc));
    netwib_er(netwib_eth_init_fields(0,0,0,0,0,0, &parphdr->ethdst));
    netwib_er(netwib_ip_init_ip4(0, &parphdr->ipdst));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_arphdr(netwib_constarphdr *parphdr,
                                    netwib_buf *ppkt)
{
  netwib_data data;

  if (parphdr->ipsrc.iptype != NETWIB_IPTYPE_IP4)
    return(NETWIB_ERR_PAIPTYPENOT4);
  if (parphdr->ipdst.iptype != NETWIB_IPTYPE_IP4)
    return(NETWIB_ERR_PAIPTYPENOT4);

  netwib_er(netwib_buf_wantspace(ppkt, NETWIB_ARPHDR_LEN, &data));

  netwib__data_append_uint16(data, 1);
  netwib__data_append_uint16(data, NETWIB_ETHERHDRTYPE_IP4);
  netwib__data_append_uint8(data, NETWIB_ETH_LEN);
  netwib__data_append_uint8(data, NETWIB_IP4_LEN);
  netwib__data_append_uint16(data, parphdr->op);
  netwib_c_memcpy(data, parphdr->ethsrc.b, NETWIB_ETH_LEN);
  data += NETWIB_ETH_LEN;
  netwib__data_append_uint32(data, parphdr->ipsrc.ipvalue.ip4);
  netwib_c_memcpy(data, parphdr->ethdst.b, NETWIB_ETH_LEN);
  data += NETWIB_ETH_LEN;
  netwib__data_append_uint32(data, parphdr->ipdst.ipvalue.ip4);

  ppkt->endoffset += NETWIB_ARPHDR_LEN;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_prepend_arphdr(netwib_constarphdr *parphdr,
                                     netwib_buf *ppkt)
{
  netwib_byte array[NETWIB_ARPHDR_LEN];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_arrayempty(array, NETWIB_ARPHDR_LEN, &buf));
  ret = netwib_pkt_append_arphdr(parphdr, &buf);
  if (ret == NETWIB_ERR_OK) {
    ret = netwib_buf_prepend_buf(&buf, ppkt);
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_arphdr(netwib_constbuf *ppkt,
                                    netwib_arphdr *parphdr,
                                    netwib_uint32 *pskipsize)
{
  netwib_data data;
  netwib_uint32 datasize, ui;

  if (pskipsize != NULL) *pskipsize = NETWIB_ARPHDR_LEN;

  datasize = netwib__buf_ref_data_size(ppkt);
  if (datasize < NETWIB_ARPHDR_LEN) {
    return(NETWIB_ERR_DATAMISSING);
  }

  if (parphdr != NULL) {
    data = netwib__buf_ref_data_ptr(ppkt);
    netwib__data_decode_uint16(data, ui);
    if (ui != 1) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16(data, ui);
    if (ui != NETWIB_ETHERHDRTYPE_IP4) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint8(data, ui);
    if (ui != NETWIB_ETH_LEN) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint8(data, ui);
    if (ui != NETWIB_IP4_LEN) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16t(data, parphdr->op, netwib_arphdrop);
    netwib_c_memcpy(parphdr->ethsrc.b, data, NETWIB_ETH_LEN);
    data += NETWIB_ETH_LEN;
    netwib__data_decode_uint32(data, parphdr->ipsrc.ipvalue.ip4);
    parphdr->ipsrc.iptype = NETWIB_IPTYPE_IP4;
    netwib_c_memcpy(parphdr->ethdst.b, data, NETWIB_ETH_LEN);
    data += NETWIB_ETH_LEN;
    netwib__data_decode_uint32(data, parphdr->ipdst.ipvalue.ip4);
    parphdr->ipdst.iptype = NETWIB_IPTYPE_IP4;
  }

  return(NETWIB_ERR_OK);
}

