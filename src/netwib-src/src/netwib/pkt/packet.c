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
netwib_err netwib_pkt_append_linkdata(netwib_constlinkhdr *plinkhdr,
                                      netwib_constbuf *pdata,
                                      netwib_buf *ppkt)
{
  netwib_er(netwib_pkt_append_layer_link(plinkhdr, ppkt));
  netwib_er(netwib_pkt_append_layer_data(pdata, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_linkdata(netwib_device_dlttype dlttype,
                                      netwib_constbuf *ppkt,
                                      netwib_linkhdr *plinkhdr,
                                      netwib_bufext *pdata)
{
  netwib_buf pkt;

  pkt = *ppkt;
  netwib_er(netwib_pkt_decode_layer_link(dlttype, &pkt, plinkhdr));
  netwib_er(netwib_buf_init_ext_buf(&pkt, pdata));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_ipdata(netwib_constiphdr *piphdr,
                                    netwib_constbuf *pdata,
                                    netwib_buf *ppkt)
{
  netwib_uint32 bufdatasize;

  bufdatasize = netwib__buf_ref_data_sizenull(pdata);
  netwib_er(netwib_pkt_append_layer_ip(piphdr, bufdatasize, ppkt));

  netwib_er(netwib_pkt_append_layer_data(pdata, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_ipdata(netwib_constbuf *ppkt,
                                    netwib_iphdr *piphdr,
                                    netwib_bufext *pdata)
{
  netwib_buf pkt;

  pkt = *ppkt;
  netwib_er(netwib_pkt_decode_layer_ip(&pkt, piphdr));
  netwib_er(netwib_buf_init_ext_buf(&pkt, pdata));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_linkipdata(netwib_constlinkhdr *plinkhdr,
                                        netwib_constiphdr *piphdr,
                                        netwib_constbuf *pdata,
                                        netwib_buf *ppkt)
{
  netwib_linkhdr linkhdr;

  linkhdr = *plinkhdr;
  netwib_er(netwib_linkhdr_set_proto(&linkhdr, piphdr->iptype));
  netwib_er(netwib_pkt_append_layer_link(&linkhdr, ppkt));

  netwib_er(netwib_pkt_append_ipdata(piphdr, pdata, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_linkipdata(netwib_device_dlttype dlttype,
                                        netwib_constbuf *ppkt,
                                        netwib_linkhdr *plinkhdr,
                                        netwib_iphdr *piphdr,
                                        netwib_bufext *pdata)
{
  netwib_buf pkt;
  netwib_linkhdr linkhdr, *plinkhdrtouse;
  netwib_linkhdrproto linkhdrproto;
  netwib_iptype iptype;

  pkt = *ppkt;
  plinkhdrtouse = (plinkhdr==NULL)?&linkhdr:plinkhdr;
  netwib_er(netwib_pkt_decode_layer_link(dlttype, &pkt, plinkhdrtouse));
  netwib_er(netwib_linkhdr_get_proto(plinkhdrtouse, &linkhdrproto));
  if (linkhdrproto == NETWIB_LINKHDRPROTO_IP4) {
    netwib_er(netwib_priv_ippkt_decode_iptype(&pkt, &iptype));
    if (iptype != NETWIB_IPTYPE_IP4) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else if (linkhdrproto == NETWIB_LINKHDRPROTO_IP6) {
    netwib_er(netwib_priv_ippkt_decode_iptype(&pkt, &iptype));
    if (iptype != NETWIB_IPTYPE_IP6) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  netwib_er(netwib_pkt_decode_ipdata(&pkt, piphdr, pdata));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_ipudpdata(netwib_constiphdr *piphdr,
                                       netwib_constudphdr *pudphdr,
                                       netwib_constbuf *pdata,
                                       netwib_buf *ppkt)
{
  netwib_uint32 ipdatasize;
  netwib_iphdr iphdr;

  ipdatasize = NETWIB_UDPHDR_LEN + netwib__buf_ref_data_sizenull(pdata);
  iphdr = *piphdr;
  netwib_er(netwib_iphdr_set_proto(&iphdr, NETWIB_IPPROTO_UDP));
  netwib_er(netwib_pkt_append_layer_ip(&iphdr, ipdatasize, ppkt));

  netwib_er(netwib_pkt_append_layer_udp(&iphdr, pudphdr, pdata, ppkt));

  netwib_er(netwib_pkt_append_layer_data(pdata, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_ipudpdata(netwib_constbuf *ppkt,
                                       netwib_iphdr *piphdr,
                                       netwib_udphdr *pudphdr,
                                       netwib_bufext *pdata)
{
  netwib_buf pkt;
  netwib_iphdr iphdr, *piphdrtouse;
  netwib_ipproto ipproto;

  pkt = *ppkt;
  piphdrtouse = (piphdr==NULL)?&iphdr:piphdr;
  netwib_er(netwib_pkt_decode_layer_ip(&pkt, piphdrtouse));
  netwib_er(netwib_iphdr_get_proto(piphdrtouse, &ipproto));
  if (ipproto != NETWIB_IPPROTO_UDP) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  netwib_er(netwib_pkt_decode_layer_udp(&pkt, pudphdr));

  netwib_er(netwib_pkt_decode_layer_data(&pkt, pdata));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_linkipudpdata(netwib_constlinkhdr *plinkhdr,
                                           netwib_constiphdr *piphdr,
                                           netwib_constudphdr *pudphdr,
                                           netwib_constbuf *pdata,
                                           netwib_buf *ppkt)
{
  netwib_linkhdr linkhdr;

  linkhdr = *plinkhdr;
  netwib_er(netwib_linkhdr_set_proto(&linkhdr, piphdr->iptype));
  netwib_er(netwib_pkt_append_layer_link(&linkhdr, ppkt));

  netwib_er(netwib_pkt_append_ipudpdata(piphdr, pudphdr, pdata, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_linkipudpdata(netwib_device_dlttype dlttype,
                                           netwib_constbuf *ppkt,
                                           netwib_linkhdr *plinkhdr,
                                           netwib_iphdr *piphdr,
                                           netwib_udphdr *pudphdr,
                                           netwib_bufext *pdata)
{
  netwib_buf pkt;
  netwib_linkhdr linkhdr, *plinkhdrtouse;
  netwib_linkhdrproto linkhdrproto;
  netwib_iptype iptype;

  pkt = *ppkt;
  plinkhdrtouse = (plinkhdr==NULL)?&linkhdr:plinkhdr;
  netwib_er(netwib_pkt_decode_layer_link(dlttype, &pkt, plinkhdrtouse));
  netwib_er(netwib_linkhdr_get_proto(plinkhdrtouse, &linkhdrproto));
  if (linkhdrproto == NETWIB_LINKHDRPROTO_IP4) {
    netwib_er(netwib_priv_ippkt_decode_iptype(&pkt, &iptype));
    if (iptype != NETWIB_IPTYPE_IP4) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else if (linkhdrproto == NETWIB_LINKHDRPROTO_IP6) {
    netwib_er(netwib_priv_ippkt_decode_iptype(&pkt, &iptype));
    if (iptype != NETWIB_IPTYPE_IP6) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  netwib_er(netwib_pkt_decode_ipudpdata(&pkt, piphdr, pudphdr, pdata));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_iptcpdata(netwib_constiphdr *piphdr,
                                       netwib_consttcphdr *ptcphdr,
                                       netwib_constbuf *pdata,
                                       netwib_buf *ppkt)
{
  netwib_uint32 ipdatasize;
  netwib_iphdr iphdr;

  iphdr = *piphdr;
  netwib_er(netwib_iphdr_set_proto(&iphdr, NETWIB_IPPROTO_TCP));
  ipdatasize = NETWIB_TCPHDR_MINLEN +
    netwib__buf_ref_data_size(&ptcphdr->opts) +
    netwib__buf_ref_data_sizenull(pdata);
  netwib_er(netwib_pkt_append_layer_ip(&iphdr, ipdatasize, ppkt));

  netwib_er(netwib_pkt_append_layer_tcp(&iphdr, ptcphdr, pdata, ppkt));

  netwib_er(netwib_pkt_append_layer_data(pdata, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_iptcpdata(netwib_constbuf *ppkt,
                                       netwib_iphdr *piphdr,
                                       netwib_tcphdr *ptcphdr,
                                       netwib_bufext *pdata)
{
  netwib_buf pkt;
  netwib_iphdr iphdr, *piphdrtouse;
  netwib_ipproto ipproto;

  pkt = *ppkt;
  piphdrtouse = (piphdr==NULL)?&iphdr:piphdr;
  netwib_er(netwib_pkt_decode_layer_ip(&pkt, piphdrtouse));
  netwib_er(netwib_iphdr_get_proto(piphdrtouse, &ipproto));
  if (ipproto != NETWIB_IPPROTO_TCP) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  netwib_er(netwib_pkt_decode_layer_tcp(&pkt, ptcphdr));

  netwib_er(netwib_pkt_decode_layer_data(&pkt, pdata));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_linkiptcpdata(netwib_constlinkhdr *plinkhdr,
                                           netwib_constiphdr *piphdr,
                                           netwib_consttcphdr *ptcphdr,
                                           netwib_constbuf *pdata,
                                           netwib_buf *ppkt)
{
  netwib_linkhdr linkhdr;

  linkhdr = *plinkhdr;
  netwib_er(netwib_linkhdr_set_proto(&linkhdr, piphdr->iptype));
  netwib_er(netwib_pkt_append_layer_link(&linkhdr, ppkt));

  netwib_er(netwib_pkt_append_iptcpdata(piphdr, ptcphdr, pdata, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_linkiptcpdata(netwib_device_dlttype dlttype,
                                           netwib_constbuf *ppkt,
                                           netwib_linkhdr *plinkhdr,
                                           netwib_iphdr *piphdr,
                                           netwib_tcphdr *ptcphdr,
                                           netwib_bufext *pdata)
{
  netwib_buf pkt;
  netwib_linkhdr linkhdr, *plinkhdrtouse;
  netwib_linkhdrproto linkhdrproto;
  netwib_iptype iptype;

  pkt = *ppkt;
  plinkhdrtouse = (plinkhdr==NULL)?&linkhdr:plinkhdr;
  netwib_er(netwib_pkt_decode_layer_link(dlttype, &pkt, plinkhdrtouse));
  netwib_er(netwib_linkhdr_get_proto(plinkhdrtouse, &linkhdrproto));
  if (linkhdrproto == NETWIB_LINKHDRPROTO_IP4) {
    netwib_er(netwib_priv_ippkt_decode_iptype(&pkt, &iptype));
    if (iptype != NETWIB_IPTYPE_IP4) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else if (linkhdrproto == NETWIB_LINKHDRPROTO_IP6) {
    netwib_er(netwib_priv_ippkt_decode_iptype(&pkt, &iptype));
    if (iptype != NETWIB_IPTYPE_IP6) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  netwib_er(netwib_pkt_decode_iptcpdata(&pkt, piphdr, ptcphdr, pdata));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_ipicmp4(netwib_constiphdr *piphdr,
                                     netwib_consticmp4 *picmp4,
                                     netwib_buf *ppkt)
{
  netwib_uint32 ipdatasize;
  netwib_iphdr iphdr;

  iphdr = *piphdr;
  netwib_er(netwib_iphdr_set_proto(&iphdr, NETWIB_IPPROTO_ICMP4));
  netwib_er(netwib_priv_icmp4_size(picmp4, &ipdatasize));
  netwib_er(netwib_pkt_append_layer_ip(&iphdr, ipdatasize, ppkt));

  netwib_er(netwib_pkt_append_layer_icmp4(picmp4, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_ipicmp4(netwib_constbuf *ppkt,
                                     netwib_iphdr *piphdr,
                                     netwib_icmp4 *picmp4)
{
  netwib_buf pkt;
  netwib_iphdr iphdr, *piphdrtouse;
  netwib_ipproto ipproto;

  pkt = *ppkt;
  piphdrtouse = (piphdr==NULL)?&iphdr:piphdr;
  netwib_er(netwib_pkt_decode_layer_ip(&pkt, piphdrtouse));
  netwib_er(netwib_iphdr_get_proto(piphdrtouse, &ipproto));
  if (ipproto != NETWIB_IPPROTO_ICMP4) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  netwib_er(netwib_pkt_decode_layer_icmp4(&pkt, picmp4));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_linkipicmp4(netwib_constlinkhdr *plinkhdr,
                                         netwib_constiphdr *piphdr,
                                         netwib_consticmp4 *picmp4,
                                         netwib_buf *ppkt)
{
  netwib_linkhdr linkhdr;

  linkhdr = *plinkhdr;
  netwib_er(netwib_linkhdr_set_proto(&linkhdr, piphdr->iptype));
  netwib_er(netwib_pkt_append_layer_link(&linkhdr, ppkt));

  netwib_er(netwib_pkt_append_ipicmp4(piphdr, picmp4, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_linkipicmp4(netwib_device_dlttype dlttype,
                                         netwib_constbuf *ppkt,
                                         netwib_linkhdr *plinkhdr,
                                         netwib_iphdr *piphdr,
                                         netwib_icmp4 *picmp4)
{
  netwib_buf pkt;
  netwib_linkhdr linkhdr, *plinkhdrtouse;
  netwib_linkhdrproto linkhdrproto;

  pkt = *ppkt;
  plinkhdrtouse = (plinkhdr==NULL)?&linkhdr:plinkhdr;
  netwib_er(netwib_pkt_decode_layer_link(dlttype, &pkt, plinkhdrtouse));
  netwib_er(netwib_linkhdr_get_proto(plinkhdrtouse, &linkhdrproto));
  if (linkhdrproto != NETWIB_LINKHDRPROTO_IP4 &&
      linkhdrproto != NETWIB_LINKHDRPROTO_IP6) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  netwib_er(netwib_pkt_decode_ipicmp4(&pkt, piphdr, picmp4));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_ipicmp6(netwib_constiphdr *piphdr,
                                     netwib_consticmp6 *picmp6,
                                     netwib_buf *ppkt)
{
  netwib_uint32 ipdatasize;
  netwib_iphdr iphdr;

  iphdr = *piphdr;
  netwib_er(netwib_iphdr_set_proto(&iphdr, NETWIB_IPPROTO_ICMP6));
  netwib_er(netwib_priv_icmp6_size(picmp6, &ipdatasize));
  netwib_er(netwib_pkt_append_layer_ip(&iphdr, ipdatasize, ppkt));

  netwib_er(netwib_pkt_append_layer_icmp6(&iphdr, picmp6, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_ipicmp6(netwib_constbuf *ppkt,
                                     netwib_iphdr *piphdr,
                                     netwib_icmp6 *picmp6)
{
  netwib_buf pkt;
  netwib_iphdr iphdr, *piphdrtouse;
  netwib_ipproto ipproto;

  pkt = *ppkt;
  piphdrtouse = (piphdr==NULL)?&iphdr:piphdr;
  netwib_er(netwib_pkt_decode_layer_ip(&pkt, piphdrtouse));
  netwib_er(netwib_iphdr_get_proto(piphdrtouse, &ipproto));
  if (ipproto != NETWIB_IPPROTO_ICMP6) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  netwib_er(netwib_pkt_decode_layer_icmp6(&pkt, picmp6));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_linkipicmp6(netwib_constlinkhdr *plinkhdr,
                                         netwib_constiphdr *piphdr,
                                         netwib_consticmp6 *picmp6,
                                         netwib_buf *ppkt)
{
  netwib_linkhdr linkhdr;

  linkhdr = *plinkhdr;
  netwib_er(netwib_linkhdr_set_proto(&linkhdr, piphdr->iptype));
  netwib_er(netwib_pkt_append_layer_link(&linkhdr, ppkt));

  netwib_er(netwib_pkt_append_ipicmp6(piphdr, picmp6, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_linkipicmp6(netwib_device_dlttype dlttype,
                                         netwib_constbuf *ppkt,
                                         netwib_linkhdr *plinkhdr,
                                         netwib_iphdr *piphdr,
                                         netwib_icmp6 *picmp6)
{
  netwib_buf pkt;
  netwib_linkhdr linkhdr, *plinkhdrtouse;
  netwib_linkhdrproto linkhdrproto;

  pkt = *ppkt;
  plinkhdrtouse = (plinkhdr==NULL)?&linkhdr:plinkhdr;
  netwib_er(netwib_pkt_decode_layer_link(dlttype, &pkt, plinkhdrtouse));
  netwib_er(netwib_linkhdr_get_proto(plinkhdrtouse, &linkhdrproto));
  if (linkhdrproto != NETWIB_LINKHDRPROTO_IP4 &&
      linkhdrproto != NETWIB_LINKHDRPROTO_IP6) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  netwib_er(netwib_pkt_decode_ipicmp6(&pkt, piphdr, picmp6));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_linkarp(netwib_constlinkhdr *plinkhdr,
                                     netwib_constarphdr *parphdr,
                                     netwib_buf *ppkt)
{
  /* before calling this function, user has to set plinkhdr->type
     to ARP or RARP */

  netwib_er(netwib_pkt_append_layer_link(plinkhdr, ppkt));

  netwib_er(netwib_pkt_append_layer_arp(parphdr, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_linkarp(netwib_device_dlttype dlttype,
                                     netwib_constbuf *ppkt,
                                     netwib_linkhdr *plinkhdr,
                                     netwib_arphdr *parphdr)
{
  netwib_buf pkt;
  netwib_linkhdr linkhdr, *plinkhdrtouse;
  netwib_linkhdrproto linkhdrproto;

  pkt = *ppkt;
  plinkhdrtouse = (plinkhdr==NULL)?&linkhdr:plinkhdr;
  netwib_er(netwib_pkt_decode_layer_link(dlttype, &pkt, plinkhdrtouse));
  netwib_er(netwib_linkhdr_get_proto(plinkhdrtouse, &linkhdrproto));
  if (linkhdrproto != NETWIB_LINKHDRPROTO_ARP &&
      linkhdrproto != NETWIB_LINKHDRPROTO_RARP) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  netwib_er(netwib_pkt_decode_layer_arp(&pkt, parphdr));

  return(NETWIB_ERR_OK);
}

