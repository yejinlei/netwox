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
static netwib_err netwib_checksum_update_pseudo(netwib_constiphdr *piphdr,
                                                netwib_ipproto ipproto,
                                                netwib_uint32 datasize,
                                                netwib_uint32 *ptmpchecksum)
{
  netwib_byte array[40];
  netwib_data data;
  netwib_ip4 ip4;

  data = array;
  switch(piphdr->iptype) {
  case NETWIB_IPTYPE_IP4 :
    netwib_er(netwib_ip4_init_ip(&piphdr->src, &ip4));
    netwib__data_append_uint32(data, ip4);
    /* if a source routing is present, the last address should be
       used instead of dst. To be implemented later. */
    netwib_er(netwib_ip4_init_ip(&piphdr->dst, &ip4));
    netwib__data_append_uint32(data, ip4);
    netwib__data_append_uint8(data, 0);
    netwib__data_append_uint8(data, ipproto);
    netwib__data_append_uint16(data, datasize);
    netwib_er(netwib_checksum_update_data(array, 12, ptmpchecksum));
    break;
  case NETWIB_IPTYPE_IP6 :
    netwib_c_memcpy(data, piphdr->src.ipvalue.ip6.b, NETWIB_IP6_LEN);
    data += NETWIB_IP6_LEN;
    /* if a routing extension is present, the last address should be
       used instead of dst. To be implemented later. */
    netwib_c_memcpy(data, piphdr->dst.ipvalue.ip6.b, NETWIB_IP6_LEN);
    data += NETWIB_IP6_LEN;
    netwib__data_append_uint32(data, datasize);
    netwib__data_append_uint8(data, 0);
    netwib__data_append_uint8(data, 0);
    netwib__data_append_uint8(data, 0);
    netwib__data_append_uint8(data, ipproto);
    netwib_er(netwib_checksum_update_data(array, 40, ptmpchecksum));
    break;
  default :
    return(NETWIB_ERR_PAIPTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_layer_data(netwib_constbuf *pdata,
                                        netwib_buf *ppkt)
{
  return(netwib_buf_append_buf(pdata, ppkt));
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_layer_data(netwib_buf *ppkt,
                                        netwib_bufext *pdata)
{
  netwib_er(netwib_buf_init_ext_buf(ppkt, pdata));
  ppkt->beginoffset = ppkt->endoffset;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_layer_link(netwib_constlinkhdr *plinkhdr,
                                        netwib_buf *ppkt)
{
  return(netwib_pkt_append_linkhdr(plinkhdr, ppkt));
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_layer_link(netwib_device_dlttype dlttype,
                                        netwib_buf *ppkt,
                                        netwib_linkhdr *plinkhdr)
{
  netwib_uint32 skipsize;

  netwib_er(netwib_pkt_decode_linkhdr(dlttype, ppkt, plinkhdr, &skipsize));
  ppkt->beginoffset += skipsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_layer_ip(netwib_constiphdr *piphdr,
                                      netwib_uint32 datasize,
                                      netwib_buf *ppkt)
{
  netwib_uint32 tmpchecksum, ihl4;
  netwib_iphdr iphdr;
  netwib_byte hdrarray[NETWIB_PRIV_IP4HDR_MAXLEN];
  netwib_buf bufhdr;

  switch(piphdr->iptype) {
  case NETWIB_IPTYPE_IP4 :
    /* update header fields */
    iphdr = *piphdr;
    ihl4 = NETWIB_PRIV_IP4HDR_MINLEN +
      netwib__buf_ref_data_size(&iphdr.header.ip4.opts);
    iphdr.header.ip4.ihl = (netwib_uint8)(ihl4/4);
    iphdr.header.ip4.totlen = (netwib_uint16)(ihl4 + datasize);
    /* compute checksum */
    iphdr.header.ip4.check = 0;
    netwib_er(netwib_checksum_init(&tmpchecksum));
    netwib_er(netwib_buf_init_ext_arraysizeofempty(hdrarray, &bufhdr));
    netwib_er(netwib_pkt_append_iphdr(&iphdr, &bufhdr));
    netwib_er(netwib_checksum_update_buf(&bufhdr, &tmpchecksum));
    netwib_er(netwib_checksum_close(tmpchecksum, &iphdr.header.ip4.check));
    break;
  case NETWIB_IPTYPE_IP6 :
    /* update header fields */
    iphdr = *piphdr;
    iphdr.header.ip6.payloadlength = (netwib_uint16)(netwib__buf_ref_data_size(&iphdr.header.ip6.exts) + datasize);
    break;
  default :
    return(NETWIB_ERR_PAIPTYPE);
  }

  /* construct packet */
  netwib_er(netwib_pkt_append_iphdr(&iphdr, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_layer_ip(netwib_buf *ppkt,
                                      netwib_iphdr *piphdr)
{
  netwib_uint32 datasize, skipsize;
  netwib_iphdr iphdr, *piphdrtouse;

  piphdrtouse = (piphdr==NULL)?&iphdr:piphdr;
  netwib_er(netwib_pkt_decode_iphdr(ppkt, piphdrtouse, &skipsize));
  ppkt->beginoffset += skipsize;

  datasize = netwib__buf_ref_data_size(ppkt);
  switch(piphdrtouse->iptype) {
  case NETWIB_IPTYPE_IP4 :
    if (piphdrtouse->header.ip4.totlen > skipsize) {
      if (skipsize + datasize > piphdrtouse->header.ip4.totlen) {
        /* sniffed packet contains padding bytes : ignore them */
        ppkt->endoffset = ppkt->beginoffset + piphdrtouse->header.ip4.totlen
          - skipsize;
      }
    }
    break;
  case NETWIB_IPTYPE_IP6 :
    if (datasize > piphdrtouse->header.ip6.payloadlength) {
      /* sniffed packet contains padding bytes : ignore them */
      ppkt->endoffset = ppkt->beginoffset
        + piphdrtouse->header.ip6.payloadlength;
    }
    break;
  default :
    return(NETWIB_ERR_PAIPTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_layer_udp(netwib_constiphdr *piphdr,
                                       netwib_constudphdr *pudphdr,
                                       netwib_constbuf *pudpdata,
                                       netwib_buf *ppkt)
{
  netwib_uint32 tmpchecksum, datasize;
  netwib_udphdr udpheader;
  netwib_byte hdrarray[NETWIB_UDPHDR_LEN];
  netwib_buf bufhdr;

  /* update header fields */
  udpheader = *pudphdr;
  datasize = netwib__buf_ref_data_sizenull(pudpdata);
  udpheader.len = (netwib_uint16)(NETWIB_UDPHDR_LEN + datasize);

  /* compute checksum */
  udpheader.check = 0;
  netwib_er(netwib_checksum_init(&tmpchecksum));
  netwib_er(netwib_checksum_update_pseudo(piphdr, NETWIB_IPPROTO_UDP,
                                          udpheader.len, &tmpchecksum));
  netwib_er(netwib_buf_init_ext_arraysizeofempty(hdrarray, &bufhdr));
  netwib_er(netwib_pkt_append_udphdr(&udpheader, &bufhdr));
  netwib_er(netwib_checksum_update_buf(&bufhdr, &tmpchecksum));
  netwib_er(netwib_checksum_update_buf(pudpdata, &tmpchecksum));
  netwib_er(netwib_checksum_close(tmpchecksum, &udpheader.check));

  /* construct packet */
  netwib_er(netwib_pkt_append_udphdr(&udpheader, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_layer_udp(netwib_buf *ppkt,
                                       netwib_udphdr *pudphdr)
{
  netwib_uint32 skipsize;

  netwib_er(netwib_pkt_decode_udphdr(ppkt, pudphdr, &skipsize));
  ppkt->beginoffset += skipsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_layer_tcp(netwib_constiphdr *piphdr,
                                       netwib_consttcphdr *ptcphdr,
                                       netwib_constbuf *ptcpdata,
                                       netwib_buf *ppkt)
{
  netwib_uint32 tmpchecksum, datasize, tcpoptssize;
  netwib_tcphdr tcpheader;
  netwib_byte hdrarray[NETWIB_TCPHDR_MAXLEN];
  netwib_buf bufhdr;

  /* update header fields */
  tcpheader = *ptcphdr;
  tcpoptssize = netwib__buf_ref_data_size(&tcpheader.opts);
  tcpheader.doff = (netwib_uint8)((NETWIB_TCPHDR_MINLEN + tcpoptssize) / 4);

  /* compute checksum */
  tcpheader.check = 0;
  netwib_er(netwib_checksum_init(&tmpchecksum));
  datasize = NETWIB_TCPHDR_MINLEN + netwib__buf_ref_data_size(&ptcphdr->opts) +
    netwib__buf_ref_data_sizenull(ptcpdata);
  netwib_er(netwib_checksum_update_pseudo(piphdr, NETWIB_IPPROTO_TCP,
                                          datasize, &tmpchecksum));
  netwib_er(netwib_buf_init_ext_arraysizeofempty(hdrarray, &bufhdr));
  netwib_er(netwib_pkt_append_tcphdr(&tcpheader, &bufhdr));
  netwib_er(netwib_checksum_update_buf(&bufhdr, &tmpchecksum));
  netwib_er(netwib_checksum_update_buf(ptcpdata, &tmpchecksum));
  netwib_er(netwib_checksum_close(tmpchecksum, &tcpheader.check));

  /* construct packet */
  netwib_er(netwib_pkt_append_tcphdr(&tcpheader, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_layer_tcp(netwib_buf *ppkt,
                                       netwib_tcphdr *ptcphdr)
{
  netwib_uint32 skipsize;

  netwib_er(netwib_pkt_decode_tcphdr(ppkt, ptcphdr, &skipsize));

  ppkt->beginoffset += skipsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_layer_icmp4(netwib_consticmp4 *picmp4,
                                         netwib_buf *ppkt)
{
  netwib_uint32 tmpchecksum, originalsize;
  netwib_uint16 check;
  netwib_data data;
  netwib_icmp4 icmp4;
  netwib_buf buf;

  /* no checksum */
  icmp4 = *picmp4;
  icmp4.check = 0;

  /* construct packet */
  originalsize = netwib__buf_ref_data_size(ppkt);
  netwib_er(netwib_pkt_append_icmp4(&icmp4, ppkt));

  /* compute checksum */
  buf = *ppkt;
  buf.beginoffset += originalsize;
  netwib_er(netwib_checksum_init(&tmpchecksum));
  netwib_er(netwib_checksum_update_buf(&buf, &tmpchecksum));
  netwib_er(netwib_checksum_close(tmpchecksum, &check));

  /* change checksum */
  data = netwib__buf_ref_data_ptr(ppkt);
  data += originalsize;
  data[2] = netwib_c2_uint16_0(check);
  data[3] = netwib_c2_uint16_1(check);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_layer_icmp4(netwib_buf *ppkt,
                                         netwib_icmp4 *picmp4)
{
  netwib_uint32 skipsize;

  netwib_er(netwib_pkt_decode_icmp4(ppkt, picmp4, &skipsize));
  ppkt->beginoffset += skipsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_layer_icmp6(netwib_constiphdr *piphdr,
                                         netwib_consticmp6 *picmp6,
                                         netwib_buf *ppkt)
{
  netwib_uint32 tmpchecksum, originalsize;
  netwib_uint16 check;
  netwib_data data;
  netwib_icmp6 icmp6;
  netwib_buf buf;

  /* no checksum */
  icmp6 = *picmp6;
  icmp6.check = 0;

  /* construct packet */
  originalsize = netwib__buf_ref_data_size(ppkt);
  netwib_er(netwib_pkt_append_icmp6(&icmp6, ppkt));

  /* compute checksum */
  buf = *ppkt;
  buf.beginoffset += originalsize;
  netwib_er(netwib_checksum_init(&tmpchecksum));
  netwib_er(netwib_checksum_update_pseudo(piphdr, NETWIB_IPPROTO_ICMP6,
                                          netwib__buf_ref_data_size(&buf),
                                          &tmpchecksum));
  netwib_er(netwib_checksum_update_buf(&buf, &tmpchecksum));
  netwib_er(netwib_checksum_close(tmpchecksum, &check));

  /* change checksum */
  data = netwib__buf_ref_data_ptr(ppkt);
  data += originalsize;
  data[2] = netwib_c2_uint16_0(check);
  data[3] = netwib_c2_uint16_1(check);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_layer_icmp6(netwib_buf *ppkt,
                                         netwib_icmp6 *picmp6)
{
  netwib_uint32 skipsize;

  netwib_er(netwib_pkt_decode_icmp6(ppkt, picmp6, &skipsize));
  ppkt->beginoffset += skipsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_layer_arp(netwib_constarphdr *parphdr,
                                       netwib_buf *ppkt)
{
  return(netwib_pkt_append_arphdr(parphdr, ppkt));
}


/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_layer_arp(netwib_buf *ppkt,
                                       netwib_arphdr *parphdr)
{
  netwib_uint32 skipsize;

  netwib_er(netwib_pkt_decode_arphdr(ppkt, parphdr, &skipsize));
  ppkt->beginoffset += skipsize;

  return(NETWIB_ERR_OK);
}

