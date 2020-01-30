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
netwib_err netwib_linkhdr_initdefault(netwib_device_dlttype type,
                                      netwib_linkhdr *plinkhdr)
{

  if (plinkhdr != NULL) {
    plinkhdr->type = type;
    switch(type) {
    case NETWIB_DEVICE_DLTTYPE_ETHER :
    case NETWIB_DEVICE_DLTTYPE_NULL :
    case NETWIB_DEVICE_DLTTYPE_LOOP :
    case NETWIB_DEVICE_DLTTYPE_RAW :
    case NETWIB_DEVICE_DLTTYPE_RAW4 :
    case NETWIB_DEVICE_DLTTYPE_RAW6 :
      break;
    case NETWIB_DEVICE_DLTTYPE_PPP :
      /* is this correct to set nothing ??? */
      break;
    case NETWIB_DEVICE_DLTTYPE_LINUX_SLL :
      plinkhdr->hdr.linuxsll.pkttype = NETWIB_LINUXSLLHDRPKTTYPE_HOST;
      plinkhdr->hdr.linuxsll.hatype = NETWIB_LINUXSLLHDRHATYPE_ETHER;
      plinkhdr->hdr.linuxsll.halen = NETWIB_ETH_LEN;
      break;
    default :
      return(NETWIB_ERR_LONOTIMPLEMENTED);
      break;
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_etherhdr(netwib_constetherhdr *petherhdr,
                                             netwib_buf *ppkt)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(ppkt, NETWIB_ETHERHDR_LEN, &data));

  netwib_c_memcpy(data, petherhdr->dst.b, NETWIB_ETH_LEN);
  data += NETWIB_ETH_LEN;
  netwib_c_memcpy(data, petherhdr->src.b, NETWIB_ETH_LEN);
  data += NETWIB_ETH_LEN;
  netwib__data_append_uint16(data, petherhdr->type);

  ppkt->endoffset += NETWIB_ETHERHDR_LEN;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_etherhdr(netwib_constbuf *ppkt,
                                             netwib_etherhdr *petherhdr,
                                             netwib_uint32 *pskipsize)
{
  netwib_data data;
  netwib_uint32 datasize;

  if (pskipsize != NULL) *pskipsize = NETWIB_ETHERHDR_LEN;

  datasize = netwib__buf_ref_data_size(ppkt);
  if (datasize < NETWIB_ETHERHDR_LEN) {
    return(NETWIB_ERR_DATAMISSING);
  }

  if (petherhdr != NULL) {
    data = netwib__buf_ref_data_ptr(ppkt);
    netwib_c_memcpy(petherhdr->dst.b, data, NETWIB_ETH_LEN);
    data += NETWIB_ETH_LEN;
    netwib_c_memcpy(petherhdr->src.b, data, NETWIB_ETH_LEN);
    data += NETWIB_ETH_LEN;
    netwib__data_decode_uint16t(data, petherhdr->type, netwib_etherhdrtype);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_nullhdr(netwib_constnullhdr *pnullhdr,
                                            netwib_buf *ppkt)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(ppkt, NETWIB_NULLHDR_LEN, &data));

  netwib__data_append_uint32(data, pnullhdr->type);

  ppkt->endoffset += NETWIB_NULLHDR_LEN;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_nullhdr(netwib_constbuf *ppkt,
                                            netwib_nullhdr *pnullhdr,
                                            netwib_uint32 *pskipsize)
{
  netwib_data data;
  netwib_uint32 datasize;

  if (pskipsize != NULL) *pskipsize = NETWIB_NULLHDR_LEN;

  datasize = netwib__buf_ref_data_size(ppkt);
  if (datasize < NETWIB_NULLHDR_LEN) {
    return(NETWIB_ERR_DATAMISSING);
  }

  if (pnullhdr != NULL) {
    data = netwib__buf_ref_data_ptr(ppkt);
    netwib__data_decode_uint32t(data, pnullhdr->type, netwib_etherhdrtype);
    if ((pnullhdr->type & 0xFFFF) == 0) {
      /* swap */
      pnullhdr->type = (netwib_etherhdrtype)(((pnullhdr->type>>8)&0xFFFF) |
                                             (pnullhdr->type>>24));
    }
    /* under BSD, this field can also contain AF_xyz */
    switch((int)pnullhdr->type) {
      case AF_INET :
        pnullhdr->type = NETWIB_ETHERHDRTYPE_IP4;
        break;
#if NETWIBDEF_HAVEVAR_AF_INET6 == 1
      case AF_INET6 :
        pnullhdr->type = NETWIB_ETHERHDRTYPE_IP6;
        break;
#endif
      default :
        /* do not change */
        break;
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_loophdr(netwib_constloophdr *ploophdr,
                                            netwib_buf *ppkt)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(ppkt, NETWIB_LOOPHDR_LEN, &data));

  netwib__data_append_uint32(data, ploophdr->type);

  ppkt->endoffset += NETWIB_LOOPHDR_LEN;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_loophdr(netwib_constbuf *ppkt,
                                            netwib_loophdr *ploophdr,
                                            netwib_uint32 *pskipsize)
{
  netwib_data data;
  netwib_uint32 datasize;

  if (pskipsize != NULL) *pskipsize = NETWIB_LOOPHDR_LEN;

  datasize = netwib__buf_ref_data_size(ppkt);
  if (datasize < NETWIB_LOOPHDR_LEN) {
    return(NETWIB_ERR_DATAMISSING);
  }

  if (ploophdr != NULL) {
    data = netwib__buf_ref_data_ptr(ppkt);
    netwib__data_decode_uint32t(data, ploophdr->type, netwib_etherhdrtype);
    /* under BSD, this field can contain AF_xyz */
    switch((int)ploophdr->type) {
      case AF_INET :
        ploophdr->type = NETWIB_ETHERHDRTYPE_IP4;
        break;
#if NETWIBDEF_HAVEVAR_AF_INET6 == 1
      case AF_INET6 :
        ploophdr->type = NETWIB_ETHERHDRTYPE_IP6;
        break;
#endif
      default :
        /* do not change */
        break;
    }
  }

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_ppphdr(netwib_constppphdr *pppphdr,
                                           netwib_buf *ppkt)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(ppkt, NETWIB_PPPHDR_LEN, &data));

  netwib__data_append_uint8(data, pppphdr->address);
  netwib__data_append_uint8(data, pppphdr->control);
  netwib__data_append_uint16(data, pppphdr->protocol);

  ppkt->endoffset += NETWIB_PPPHDR_LEN;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_ppphdr(netwib_constbuf *ppkt,
                                           netwib_ppphdr *pppphdr,
                                           netwib_uint32 *pskipsize)
{
  netwib_data data;
  netwib_uint32 datasize;

  if (pskipsize != NULL) *pskipsize = NETWIB_PPPHDR_LEN;

  datasize = netwib__buf_ref_data_size(ppkt);
  if (datasize < NETWIB_PPPHDR_LEN) {
    return(NETWIB_ERR_DATAMISSING);
  }

  if (pppphdr != NULL) {
    data = netwib__buf_ref_data_ptr(ppkt);
    netwib__data_decode_uint8(data, pppphdr->address);
    netwib__data_decode_uint8(data, pppphdr->control);
    netwib__data_decode_uint16t(data, pppphdr->protocol, netwib_ppphdrproto);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_linuxsllhdr(netwib_constlinuxsllhdr *plinuxsllhdr,
                                                netwib_buf *ppkt)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(ppkt, NETWIB_LINUXSLLHDR_LEN, &data));

  netwib__data_append_uint16(data, plinuxsllhdr->pkttype);
  netwib__data_append_uint16(data, plinuxsllhdr->hatype);
  netwib__data_append_uint16(data, plinuxsllhdr->halen);
  if (plinuxsllhdr->halen >= 8 ) {
    netwib_c_memcpy(data, plinuxsllhdr->srcaddr, 8);
    data += 8;
  } else {
    netwib_c_memcpy(data, plinuxsllhdr->srcaddr, plinuxsllhdr->halen);
    data += plinuxsllhdr->halen;
    netwib_c_memset(data, 0, 8-plinuxsllhdr->halen);
    data += 8-plinuxsllhdr->halen;
  }
  netwib__data_append_uint16(data, plinuxsllhdr->protocol);

  ppkt->endoffset += NETWIB_LINUXSLLHDR_LEN;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_linuxsllhdr(netwib_constbuf *ppkt,
                                                netwib_linuxsllhdr *plinuxsllhdr,
                                                netwib_uint32 *pskipsize)
{
  netwib_data data;
  netwib_uint32 datasize;

  if (pskipsize != NULL) *pskipsize = NETWIB_LINUXSLLHDR_LEN;

  datasize = netwib__buf_ref_data_size(ppkt);
  if (datasize < NETWIB_LINUXSLLHDR_LEN) {
    return(NETWIB_ERR_DATAMISSING);
  }

  if (plinuxsllhdr != NULL) {
    data = netwib__buf_ref_data_ptr(ppkt);
    netwib__data_decode_uint16t(data, plinuxsllhdr->pkttype,
                              netwib_linuxsllhdrpkttype);
    netwib__data_decode_uint16t(data, plinuxsllhdr->hatype,
                              netwib_linuxsllhdrhatype);
    netwib__data_decode_uint16(data, plinuxsllhdr->halen);
    if (plinuxsllhdr->halen >= 8 ) {
      netwib_c_memcpy(plinuxsllhdr->srcaddr, data, 8);
    } else {
      netwib_c_memcpy(plinuxsllhdr->srcaddr, data, plinuxsllhdr->halen);
      netwib_c_memset(plinuxsllhdr->srcaddr+plinuxsllhdr->halen, 0,
                      8-plinuxsllhdr->halen);
    }
    data += 8;
    netwib__data_decode_uint16t(data, plinuxsllhdr->protocol,
                                netwib_etherhdrtype);
  }

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_linkhdr(netwib_constlinkhdr *plinkhdr,
                                     netwib_buf *ppkt)
{
  switch(plinkhdr->type) {
    case NETWIB_DEVICE_DLTTYPE_EN10MB :
      netwib_er(netwib_pkt_append_etherhdr(&plinkhdr->hdr.ether, ppkt));
      break;
    case NETWIB_DEVICE_DLTTYPE_NULL :
      netwib_er(netwib_pkt_append_nullhdr(&plinkhdr->hdr.null, ppkt));
      break;
    case NETWIB_DEVICE_DLTTYPE_LOOP :
      netwib_er(netwib_pkt_append_loophdr(&plinkhdr->hdr.loop, ppkt));
      break;
    case NETWIB_DEVICE_DLTTYPE_RAW :
    case NETWIB_DEVICE_DLTTYPE_RAW4 :
    case NETWIB_DEVICE_DLTTYPE_RAW6 :
      /* nothing to append */
      break;
    case NETWIB_DEVICE_DLTTYPE_PPP :
      netwib_er(netwib_pkt_append_ppphdr(&plinkhdr->hdr.ppp, ppkt));
      break;
    case NETWIB_DEVICE_DLTTYPE_LINUX_SLL :
      netwib_er(netwib_pkt_append_linuxsllhdr(&plinkhdr->hdr.linuxsll, ppkt));
      break;
    default :
      return(NETWIB_ERR_LONOTIMPLEMENTED);
      break;
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_prepend_linkhdr(netwib_constlinkhdr *plinkhdr,
                                      netwib_buf *ppkt)
{
  netwib_byte array[NETWIB_LINKHDR_MAXLEN];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_arrayempty(array, NETWIB_LINKHDR_MAXLEN,
                                           &buf));
  ret = netwib_pkt_append_linkhdr(plinkhdr, &buf);
  if (ret == NETWIB_ERR_OK) {
    ret = netwib_buf_prepend_buf(&buf, ppkt);
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_linkhdr(netwib_device_dlttype dlttype,
                                     netwib_constbuf *ppkt,
                                     netwib_linkhdr *plinkhdr,
                                     netwib_uint32 *pskipsize)
{
  netwib_iptype iptype;

  switch(dlttype) {
    case NETWIB_DEVICE_DLTTYPE_EN10MB :
      netwib_er(netwib_pkt_decode_etherhdr(ppkt,
                                 (plinkhdr==NULL)?NULL:&plinkhdr->hdr.ether,
                                           pskipsize));
      break;
    case NETWIB_DEVICE_DLTTYPE_NULL :
      netwib_er(netwib_pkt_decode_nullhdr(ppkt,
                                 (plinkhdr==NULL)?NULL:&plinkhdr->hdr.null,
                                          pskipsize));
      break;
    case NETWIB_DEVICE_DLTTYPE_LOOP :
      netwib_er(netwib_pkt_decode_loophdr(ppkt,
                                 (plinkhdr==NULL)?NULL:&plinkhdr->hdr.loop,
                                          pskipsize));
      break;
    case NETWIB_DEVICE_DLTTYPE_RAW :
      netwib_er(netwib_priv_ippkt_decode_iptype(ppkt, &iptype));
      switch(iptype) {
      case NETWIB_IPTYPE_IP4 :
        dlttype = NETWIB_DEVICE_DLTTYPE_RAW4;
        break;
      case NETWIB_IPTYPE_IP6 :
        dlttype = NETWIB_DEVICE_DLTTYPE_RAW6;
        break;
      default :
        break;
      }
      /* no break */
    case NETWIB_DEVICE_DLTTYPE_RAW4 :
    case NETWIB_DEVICE_DLTTYPE_RAW6 :
      if (pskipsize != NULL) *pskipsize = NETWIB_RAWHDR_LEN;
      break;
    case NETWIB_DEVICE_DLTTYPE_PPP :
      netwib_er(netwib_pkt_decode_ppphdr(ppkt,
                                 (plinkhdr==NULL)?NULL:&plinkhdr->hdr.ppp,
                                         pskipsize));
      break;
    case NETWIB_DEVICE_DLTTYPE_LINUX_SLL :
      netwib_er(netwib_pkt_decode_linuxsllhdr(ppkt,
                                 (plinkhdr==NULL)?NULL:&plinkhdr->hdr.linuxsll,
                                              pskipsize));
      break;
    default :
      return(NETWIB_ERR_LONOTIMPLEMENTED);
      break;
  }

  if (plinkhdr != NULL) plinkhdr->type = dlttype;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_linkhdr_set_proto(netwib_linkhdr *plinkhdr,
                                    netwib_linkhdrproto linkhdrproto)
{
  netwib_etherhdrtype etherhdrtype = (netwib_etherhdrtype)0;
  netwib_ppphdrproto ppphdrproto = (netwib_ppphdrproto)0;

  switch(linkhdrproto) {
    case NETWIB_LINKHDRPROTO_IP4 :
      etherhdrtype = NETWIB_ETHERHDRTYPE_IP4;
      ppphdrproto = NETWIB_PPPHDRPROTO_IP4;
      break;
    case NETWIB_LINKHDRPROTO_IP6 :
      etherhdrtype = NETWIB_ETHERHDRTYPE_IP6;
      ppphdrproto = NETWIB_PPPHDRPROTO_IP6;
      break;
    case NETWIB_LINKHDRPROTO_ARP :
      etherhdrtype = NETWIB_ETHERHDRTYPE_ARP;
      ppphdrproto = (netwib_ppphdrproto)0;
      break;
    case NETWIB_LINKHDRPROTO_RARP :
      etherhdrtype = NETWIB_ETHERHDRTYPE_RARP;
      ppphdrproto = (netwib_ppphdrproto)0;
      break;
    case NETWIB_LINKHDRPROTO_IPX :
      etherhdrtype = NETWIB_ETHERHDRTYPE_IPX;
      ppphdrproto = NETWIB_PPPHDRPROTO_IPX;
      break;
    default :
      return(NETWIB_ERR_LONOTIMPLEMENTED);
      break;
  }

  switch(plinkhdr->type) {
    case NETWIB_DEVICE_DLTTYPE_EN10MB :
      plinkhdr->hdr.ether.type = etherhdrtype;
      break;
    case NETWIB_DEVICE_DLTTYPE_NULL :
      plinkhdr->hdr.null.type = etherhdrtype;
      break;
    case NETWIB_DEVICE_DLTTYPE_LOOP :
      plinkhdr->hdr.loop.type = etherhdrtype;
      break;
    case NETWIB_DEVICE_DLTTYPE_RAW :
    case NETWIB_DEVICE_DLTTYPE_RAW4 :
    case NETWIB_DEVICE_DLTTYPE_RAW6 :
      /* nothing to store */
      break;
    case NETWIB_DEVICE_DLTTYPE_PPP :
      if (ppphdrproto == 0) {
        return(NETWIB_ERR_LONOTIMPLEMENTED);
      }
      plinkhdr->hdr.ppp.protocol = ppphdrproto;
      break;
    case NETWIB_DEVICE_DLTTYPE_LINUX_SLL :
      plinkhdr->hdr.linuxsll.protocol = etherhdrtype;
      break;
    default :
      return(NETWIB_ERR_LONOTIMPLEMENTED);
      break;
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_linkhdr_get_proto(netwib_constlinkhdr *plinkhdr,
                                    netwib_linkhdrproto *plinkhdrproto)
{
  netwib_etherhdrtype etherhdrtype = (netwib_etherhdrtype)0;
  netwib_ppphdrproto ppphdrproto = (netwib_ppphdrproto)0;
  netwib_linkhdrproto linkhdrproto = (netwib_linkhdrproto)0;
  netwib_bool etherset;

  etherset = NETWIB_TRUE;

  switch(plinkhdr->type) {
    case NETWIB_DEVICE_DLTTYPE_EN10MB :
      etherhdrtype = plinkhdr->hdr.ether.type;
      break;
    case NETWIB_DEVICE_DLTTYPE_NULL :
      etherhdrtype = plinkhdr->hdr.null.type;
      break;
    case NETWIB_DEVICE_DLTTYPE_LOOP :
      etherhdrtype = plinkhdr->hdr.loop.type;
      break;
    case NETWIB_DEVICE_DLTTYPE_RAW :
      /* IPv4 or IPv6, but don't know which one */
      return(NETWIB_ERR_NOTCONVERTED);
      break;
    case NETWIB_DEVICE_DLTTYPE_RAW4 :
      etherhdrtype = NETWIB_ETHERHDRTYPE_IP4;
      break;
    case NETWIB_DEVICE_DLTTYPE_RAW6 :
      etherhdrtype = NETWIB_ETHERHDRTYPE_IP6;
      break;
    case NETWIB_DEVICE_DLTTYPE_PPP :
      ppphdrproto = plinkhdr->hdr.ppp.protocol;
      etherset = NETWIB_FALSE;
      break;
    case NETWIB_DEVICE_DLTTYPE_LINUX_SLL :
      etherhdrtype = plinkhdr->hdr.linuxsll.protocol;
      break;
    default :
      return(NETWIB_ERR_LONOTIMPLEMENTED);
      break;
  }

  if (etherset) {
    switch(etherhdrtype) {
      case NETWIB_ETHERHDRTYPE_IP4 :
        linkhdrproto = NETWIB_LINKHDRPROTO_IP4;
        break;
      case NETWIB_ETHERHDRTYPE_IP6 :
        linkhdrproto = NETWIB_LINKHDRPROTO_IP6;
        break;
      case NETWIB_ETHERHDRTYPE_ARP :
        linkhdrproto = NETWIB_LINKHDRPROTO_ARP;
        break;
      case NETWIB_ETHERHDRTYPE_RARP :
        linkhdrproto = NETWIB_LINKHDRPROTO_RARP;
        break;
      case NETWIB_ETHERHDRTYPE_IPX :
        linkhdrproto = NETWIB_LINKHDRPROTO_IPX;
        break;
      default :
        linkhdrproto = NETWIB_LINKHDRPROTO_UNKNOWN;
        break;
    }
  } else {
    switch(ppphdrproto) {
      case NETWIB_PPPHDRPROTO_IP4 :
        linkhdrproto = NETWIB_LINKHDRPROTO_IP4;
      case NETWIB_PPPHDRPROTO_IP6 :
        linkhdrproto = NETWIB_LINKHDRPROTO_IP6;
        break;
      case NETWIB_PPPHDRPROTO_IPX :
        linkhdrproto = NETWIB_LINKHDRPROTO_IPX;
        break;
      default :
        linkhdrproto = NETWIB_LINKHDRPROTO_UNKNOWN;
        break;
    }
  }

  if (plinkhdrproto != NULL) *plinkhdrproto = linkhdrproto;
  return(NETWIB_ERR_OK);
}
