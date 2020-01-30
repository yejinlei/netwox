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
netwib_err netwib_icmp6nd_initdefault(netwib_icmp6ndtype type,
                                      netwib_icmp6nd *picmp6nd)
{

  picmp6nd->type = type;
  switch(type) {
  case NETWIB_ICMP6NDTYPE_SRCLINK :
  case NETWIB_ICMP6NDTYPE_DSTLINK :
    netwib_er(netwib_eth_init_fields(0,0,0,0,0,0,
                                     &picmp6nd->opt.link.linkad));
    break;
  case NETWIB_ICMP6NDTYPE_PREFIX :
    picmp6nd->opt.prefix.prefixlength = 0;
    picmp6nd->opt.prefix.onlink = NETWIB_FALSE;
    picmp6nd->opt.prefix.autonomous = NETWIB_FALSE;
    picmp6nd->opt.prefix.reserved1 = 0;
    picmp6nd->opt.prefix.validlifetime = 0;
    picmp6nd->opt.prefix.preferredlifetime = 0;
    picmp6nd->opt.prefix.reserved2 = 0;
    netwib_er(netwib_ip_init_ip6_fields(0,0,0,0,
                                        &picmp6nd->opt.prefix.prefix));
    break;
  case NETWIB_ICMP6NDTYPE_REDIR :
    picmp6nd->opt.redir.reserved1 = 0;
    picmp6nd->opt.redir.reserved2 = 0;
    netwib_er(netwib_buf_init_ext_empty(&picmp6nd->opt.redir.badippacket));
    break;
  case NETWIB_ICMP6NDTYPE_MTU :
    picmp6nd->opt.mtu.reserved = 0;
    picmp6nd->opt.mtu.mtu = 0;
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_icmp6nd(netwib_consticmp6nd *picmp6nd,
                                     netwib_buf *ppkt)
{
  netwib_buf badippkt;
  netwib_data data;
  netwib_uint32 length, tmpval;

  switch(picmp6nd->type) {
  case NETWIB_ICMP6NDTYPE_SRCLINK :
  case NETWIB_ICMP6NDTYPE_DSTLINK :
    length = 8;
    netwib_er(netwib_buf_wantspace(ppkt, length, &data));
    netwib__data_append_uint8(data, picmp6nd->type);
    netwib__data_append_uint8(data, length/8);
    netwib_c_memcpy(data, picmp6nd->opt.link.linkad.b, NETWIB_ETH_LEN);
    ppkt->endoffset += length;
    break;
  case NETWIB_ICMP6NDTYPE_PREFIX :
    if (picmp6nd->opt.prefix.prefix.iptype != NETWIB_IPTYPE_IP6) {
      return(NETWIB_ERR_PAINVALIDTYPE);
    }
    length = 32;
    netwib_er(netwib_buf_wantspace(ppkt, length, &data));
    netwib__data_append_uint8(data, picmp6nd->type);
    netwib__data_append_uint8(data, length/8);
    netwib__data_append_uint8(data, picmp6nd->opt.prefix.prefixlength);
    tmpval = picmp6nd->opt.prefix.reserved1;
    if (picmp6nd->opt.prefix.onlink) {
      tmpval |= 1 << 7;
    }
    if (picmp6nd->opt.prefix.autonomous) {
      tmpval |= 1 << 6;
    }
    netwib__data_append_uint8(data, tmpval);
    netwib__data_append_uint32(data, picmp6nd->opt.prefix.validlifetime);
    netwib__data_append_uint32(data, picmp6nd->opt.prefix.preferredlifetime);
    netwib__data_append_uint32(data, picmp6nd->opt.prefix.reserved2);
    netwib_c_memcpy(data, picmp6nd->opt.prefix.prefix.ipvalue.ip6.b,
                    NETWIB_IP6_LEN);
    ppkt->endoffset += length;
    break;
  case NETWIB_ICMP6NDTYPE_REDIR :
    netwib_er(netwib_ip64bits_init_ippkt(&picmp6nd->opt.redir.badippacket,
                                         &badippkt));
    tmpval = netwib__buf_ref_data_size(&badippkt);
    length = 8 * ( (tmpval + 7) / 8 );
    tmpval = netwib__buf_ref_data_size(&picmp6nd->opt.redir.badippacket);
    if (length > tmpval) {
      length = 8 * (tmpval / 8);
    }
    badippkt.endoffset = badippkt.beginoffset + length;
    length = 8 + length;
    netwib_er(netwib_buf_wantspace(ppkt, length, &data));
    netwib__data_append_uint8(data, picmp6nd->type);
    netwib__data_append_uint8(data, length/8);
    netwib__data_append_uint16(data, picmp6nd->opt.redir.reserved1);
    netwib__data_append_uint32(data, picmp6nd->opt.redir.reserved2);
    ppkt->endoffset += 8;
    netwib_er(netwib_buf_append_buf(&badippkt, ppkt));
    break;
  case NETWIB_ICMP6NDTYPE_MTU :
    length = 8;
    netwib_er(netwib_buf_wantspace(ppkt, length, &data));
    netwib__data_append_uint8(data, picmp6nd->type);
    netwib__data_append_uint8(data, length/8);
    netwib__data_append_uint16(data, picmp6nd->opt.mtu.reserved);
    netwib__data_append_uint32(data, picmp6nd->opt.mtu.mtu);
    ppkt->endoffset += length;
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_icmp6nd(netwib_constbuf *ppkt,
                                     netwib_icmp6nd *picmp6nd,
                                     netwib_uint32 *pskipsize)
{
  netwib_data data;
  netwib_uint32 datasize, length, tmpval;

  datasize = netwib__buf_ref_data_size(ppkt);
  if (pskipsize != NULL) *pskipsize = datasize;
  if (datasize < 2) {
    return(NETWIB_ERR_DATAMISSING);
  }
  data = netwib__buf_ref_data_ptr(ppkt);

  netwib__data_decode_uint8t(data, picmp6nd->type, netwib_icmp6ndtype);
  netwib__data_decode_uint8(data, length);
  if (length == 0) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  if (8*length > datasize) {
    return(NETWIB_ERR_DATAMISSING);
  }
  datasize = 8*length;
  if (pskipsize != NULL) *pskipsize = datasize;

  switch(picmp6nd->type) {
  case NETWIB_ICMP6NDTYPE_SRCLINK :
  case NETWIB_ICMP6NDTYPE_DSTLINK :
    if (datasize != 8) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_c_memcpy(picmp6nd->opt.link.linkad.b, data, NETWIB_ETH_LEN);
    break;
  case NETWIB_ICMP6NDTYPE_PREFIX :
    if (datasize != 32) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint8(data, picmp6nd->opt.prefix.prefixlength);
    netwib__data_decode_uint8(data, tmpval);
    picmp6nd->opt.prefix.onlink = (tmpval&(1<<7))?NETWIB_TRUE:NETWIB_FALSE;
    picmp6nd->opt.prefix.autonomous = (tmpval&(1<<6))?NETWIB_TRUE:NETWIB_FALSE;
    picmp6nd->opt.prefix.reserved1 = (netwib_uint8)(tmpval & 0x3F);
    netwib__data_decode_uint32(data, picmp6nd->opt.prefix.validlifetime);
    netwib__data_decode_uint32(data, picmp6nd->opt.prefix.preferredlifetime);
    netwib__data_decode_uint32(data, picmp6nd->opt.prefix.reserved2);
    picmp6nd->opt.prefix.prefix.iptype = NETWIB_IPTYPE_IP6;
    netwib_c_memcpy(picmp6nd->opt.prefix.prefix.ipvalue.ip6.b, data,
                    NETWIB_IP6_LEN);
    break;
  case NETWIB_ICMP6NDTYPE_REDIR :
    if (datasize < 8) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16(data, picmp6nd->opt.redir.reserved1);
    netwib__data_decode_uint32(data, picmp6nd->opt.redir.reserved2);
    datasize -= 8;
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, &picmp6nd->opt.redir.badippacket));
    break;
  case NETWIB_ICMP6NDTYPE_MTU :
    if (datasize != 8) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16(data, picmp6nd->opt.mtu.reserved);
    netwib__data_decode_uint32(data, picmp6nd->opt.mtu.mtu);
    break;
  default :
    return(NETWIB_ERR_LONOTIMPLEMENTED);
    break;
  }

  return(NETWIB_ERR_OK);
}
