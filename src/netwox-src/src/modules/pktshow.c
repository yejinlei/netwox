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
#include "../netwox.h"

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_link_show(netwib_bufpool *pbufpool,
                                netwib_device_dlttype dlttype,
                                netwib_constbuf *ppkt,
                                netwib_encodetype_context *pctx,
                                netwib_encodetype hdrencodetype,
                                netwib_encodetype dataencodetype,
                                netwib_buf *pbuf)
{
  netwib_linkhdr linkhdr;
  netwib_arphdr arpheader;
  netwib_linkhdrproto linkhdrproto;
  netwib_buf pkt;
  netwib_encodetype_context *pctxtouse, ctx;
  netwib_err ret;

  pctxtouse = pctx;
  if (pctx == NULL) {
    netwib_er(netwib_buf_encode_transition_init(&ctx));
    pctxtouse = &ctx;
  }

  pkt = *ppkt;
  ret = netwib_pkt_decode_layer_link(dlttype, &pkt, &linkhdr);
  if (ret == NETWIB_ERR_NOTCONVERTED || ret == NETWIB_ERR_DATAMISSING
      || ret == NETWIB_ERR_LONOTIMPLEMENTED) {
    netwib_er(netwib_pkt_data_show(&pkt, pctxtouse, dataencodetype,
                                         pbuf));
    netwib_er(netwib_buf_encode_transition_end(pctxtouse, pbuf));
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  if (dlttype != NETWIB_DEVICE_DLTTYPE_RAW &&
      dlttype != NETWIB_DEVICE_DLTTYPE_RAW4 &&
      dlttype != NETWIB_DEVICE_DLTTYPE_RAW6) {
    netwib_er(netwib_buf_encode_transition(pctxtouse, hdrencodetype, pbuf));
    netwib_er(netwib_linkhdr_show(&linkhdr, hdrencodetype, pbuf));
  }

  netwib_er(netwib_linkhdr_get_proto(&linkhdr, &linkhdrproto));
  switch(linkhdrproto) {
    case NETWIB_LINKHDRPROTO_ARP :
    case NETWIB_LINKHDRPROTO_RARP :
      ret = netwib_pkt_decode_layer_arp(&pkt, &arpheader);
      if (ret == NETWIB_ERR_NOTCONVERTED || ret == NETWIB_ERR_DATAMISSING
          || ret == NETWIB_ERR_LONOTIMPLEMENTED) {
        netwib_er(netwib_pkt_data_show(&pkt, pctxtouse, dataencodetype, pbuf));
        netwib_er(netwib_buf_encode_transition_end(pctxtouse, pbuf));
        return(NETWIB_ERR_OK);
      } else if (ret != NETWIB_ERR_OK) {
        return(ret);
      }
      netwib_er(netwib_buf_encode_transition(pctxtouse, hdrencodetype, pbuf));
      netwib_er(netwib_arphdr_show(&arpheader, hdrencodetype, pbuf));
      netwib_er(netwib_buf_encode_transition(pctxtouse,
                                             NETWIB_ENCODETYPE_TRANSITION_END,
                                             pbuf));
      break;
    case NETWIB_LINKHDRPROTO_IP4 :
    case NETWIB_LINKHDRPROTO_IP6 :
      netwib_er(netwox_pkt_ip_show(pbufpool, &pkt, pctxtouse, hdrencodetype,
                                   dataencodetype, pbuf));
      break;
    default :
      netwib_er(netwib_pkt_data_show(&pkt, pctxtouse, dataencodetype, pbuf));
      netwib_er(netwib_buf_encode_transition_end(pctxtouse, pbuf));
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_link_display(netwib_bufpool *pbufpool,
                                   netwib_device_dlttype dlttype,
                                   netwib_constbuf *ppkt,
                                   netwib_encodetype_context *pctx,
                                   netwib_encodetype hdrencodetype,
                                   netwib_encodetype dataencodetype)
{
  netwib_err ret;
  netwib_string pc;
  netwib_buf *pbuf;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
  ret = netwox_pkt_link_show(pbufpool, dlttype, ppkt, pctx, hdrencodetype,
                             dataencodetype, pbuf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_ref_string(pbuf, &pc));
    fprintf(stdout, "%s", pc);
    fflush(stdout);
  }
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_ip_show(netwib_bufpool *pbufpool,
                              netwib_constbuf *ppkt,
                              netwib_encodetype_context *pctx,
                              netwib_encodetype hdrencodetype,
                              netwib_encodetype dataencodetype,
                              netwib_buf *pbuf)
{
  netwib_iphdr iphdr;
  netwib_buf pkt;
  netwib_encodetype_context *pctxtouse, ctx;
  netwib_bool isfragment;
  netwib_ipproto ipproto;
  netwib_err ret;

  pctxtouse = pctx;
  if (pctx == NULL) {
    netwib_er(netwib_buf_encode_transition_init(&ctx));
    pctxtouse = &ctx;
  }

  pkt = *ppkt;
  ret = netwib_pkt_decode_layer_ip(&pkt, &iphdr);
  if (ret == NETWIB_ERR_NOTCONVERTED || ret == NETWIB_ERR_DATAMISSING
      || ret == NETWIB_ERR_LONOTIMPLEMENTED) {
    netwib_er(netwib_pkt_data_show(&pkt, pctxtouse, dataencodetype, pbuf));
    netwib_er(netwib_buf_encode_transition_end(pctxtouse, pbuf));
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  netwib_er(netwib_buf_encode_transition(pctxtouse, hdrencodetype, pbuf));
  netwib_er(netwib_iphdr_show(&iphdr, hdrencodetype, pbuf));

  /* fragments cannot be displayed */
  netwib_er(netwox_iphdr_isfrag(&iphdr, &isfragment));
  if (isfragment) {
    netwib_er(netwib_pkt_data_show(&pkt, pctxtouse, dataencodetype, pbuf));
    netwib_er(netwib_buf_encode_transition_end(pctxtouse, pbuf));
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_iphdr_get_proto(&iphdr, &ipproto));
  switch(ipproto) {
    case NETWIB_IPPROTO_UDP :
      netwib_er(netwox_pkt_udp_show(pbufpool, &pkt, pctxtouse, hdrencodetype,
                                    dataencodetype, pbuf));
      break;
    case NETWIB_IPPROTO_TCP :
      netwib_er(netwox_pkt_tcp_show(pbufpool, &pkt, pctxtouse, hdrencodetype,
                                    dataencodetype, pbuf));
      break;
    case NETWIB_IPPROTO_ICMP4 :
      netwib_er(netwib_pkt_icmp4_show(&pkt, pctxtouse, hdrencodetype,
                                      dataencodetype, pbuf));
      break;
    case NETWIB_IPPROTO_ICMP6 :
      netwib_er(netwib_pkt_icmp6_show(&pkt, pctxtouse, hdrencodetype,
                                      dataencodetype, pbuf));
      break;
    default :
      netwib_er(netwib_pkt_data_show(&pkt, pctxtouse, dataencodetype,
                                     pbuf));
      netwib_er(netwib_buf_encode_transition_end(pctxtouse, pbuf));
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_ip_display(netwib_bufpool *pbufpool,
                                 netwib_constbuf *ppkt,
                                 netwib_encodetype_context *pctx,
                                 netwib_encodetype hdrencodetype,
                                 netwib_encodetype dataencodetype)
{
  netwib_err ret;
  netwib_string pc;
  netwib_buf *pbuf;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
  ret = netwox_pkt_ip_show(pbufpool, ppkt, pctx, hdrencodetype, dataencodetype,
                           pbuf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_ref_string(pbuf, &pc));
    fprintf(stdout, "%s", pc);
    fflush(stdout);
  }
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_udp_show(netwib_bufpool *pbufpool,
                               netwib_constbuf *ppkt,
                               netwib_encodetype_context *pctx,
                               netwib_encodetype hdrencodetype,
                               netwib_encodetype dataencodetype,
                               netwib_buf *pbuf)
{
  netwib_udphdr udpheader;
  netwib_buf pkt;
  netwib_encodetype_context *pctxtouse, ctx;
  netwib_err ret;

  pctxtouse = pctx;
  if (pctx == NULL) {
    netwib_er(netwib_buf_encode_transition_init(&ctx));
    pctxtouse = &ctx;
  }

  pkt = *ppkt;
  ret = netwib_pkt_decode_layer_udp(&pkt, &udpheader);
  if (ret == NETWIB_ERR_NOTCONVERTED || ret == NETWIB_ERR_DATAMISSING
      || ret == NETWIB_ERR_LONOTIMPLEMENTED) {
    netwib_er(netwib_pkt_data_show(&pkt, pctxtouse, dataencodetype, pbuf));
    netwib_er(netwib_buf_encode_transition_end(pctxtouse, pbuf));
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  netwib_er(netwib_buf_encode_transition(pctxtouse, hdrencodetype, pbuf));
  netwib_er(netwib_udphdr_show(&udpheader, hdrencodetype, pbuf));

  if (udpheader.src == NETWOX_SERVICES_DOMAIN ||
      udpheader.dst == NETWOX_SERVICES_DOMAIN) {
    netwib_er(netwox_pkt_dns_show(pbufpool, &pkt, pctxtouse, hdrencodetype,
                                  dataencodetype, pbuf));
  } else if (udpheader.src == NETWOX_SERVICES_SNMP ||
             udpheader.dst == NETWOX_SERVICES_SNMP ||
             udpheader.src == NETWOX_SERVICES_SNMPTRAP ||
             udpheader.dst == NETWOX_SERVICES_SNMPTRAP) {
    netwib_er(netwox_pkt_snmp_show(pbufpool, &pkt, pctxtouse, hdrencodetype,
                                   dataencodetype, pbuf));
  } else if (udpheader.src == NETWOX_SERVICES_TFTP ||
             udpheader.dst == NETWOX_SERVICES_TFTP) {
    netwib_er(netwox_pkt_tftp_show(pbufpool, &pkt, pctxtouse, hdrencodetype,
                                   dataencodetype, pbuf));
  } else if (udpheader.dst == NETWOX_SERVICES_DHCP4S ||
             udpheader.dst == NETWOX_SERVICES_DHCP4C) {
    netwib_er(netwox_pkt_dhcp4_show(&pkt, pctxtouse, hdrencodetype,
                                    dataencodetype, pbuf));
  } else if (udpheader.src == NETWOX_SERVICES_NTP ||
             udpheader.dst == NETWOX_SERVICES_NTP) {
    netwib_er(netwox_pkt_ntp_show(&pkt, pctxtouse, hdrencodetype,
                                  dataencodetype, pbuf));
  } else {
    netwib_er(netwib_pkt_data_show(&pkt, pctxtouse, dataencodetype, pbuf));
  }
  netwib_er(netwib_buf_encode_transition_end(pctxtouse, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_tcp_show(netwib_bufpool *pbufpool,
                               netwib_constbuf *ppkt,
                               netwib_encodetype_context *pctx,
                               netwib_encodetype hdrencodetype,
                               netwib_encodetype dataencodetype,
                               netwib_buf *pbuf)
{
  netwib_tcphdr tcpheader;
  netwib_buf pkt, dnspkt;
  netwib_encodetype_context *pctxtouse, ctx;
  netwib_bool dnspktdecoded;
  netwib_err ret;

  pctxtouse = pctx;
  if (pctx == NULL) {
    netwib_er(netwib_buf_encode_transition_init(&ctx));
    pctxtouse = &ctx;
  }

  pkt = *ppkt;
  ret = netwib_pkt_decode_layer_tcp(&pkt, &tcpheader);
  if (ret == NETWIB_ERR_NOTCONVERTED || ret == NETWIB_ERR_DATAMISSING
      || ret == NETWIB_ERR_LONOTIMPLEMENTED) {
    netwib_er(netwib_pkt_data_show(&pkt, pctxtouse, dataencodetype, pbuf));
    netwib_er(netwib_buf_encode_transition_end(pctxtouse, pbuf));
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  netwib_er(netwib_buf_encode_transition(pctxtouse, hdrencodetype, pbuf));
  netwib_er(netwib_tcphdr_show(&tcpheader, hdrencodetype, pbuf));

  dnspktdecoded = NETWIB_FALSE;
  if (tcpheader.src == NETWOX_SERVICES_DOMAIN ||
      tcpheader.dst == NETWOX_SERVICES_DOMAIN) {
    ret = netwox_pkt_decode_tcpdns(&pkt, &dnspkt, NULL);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwox_pkt_dns_show(pbufpool, &dnspkt, pctxtouse,
                                    hdrencodetype, dataencodetype, pbuf));
      dnspktdecoded = NETWIB_TRUE;
    }
  }
  if (!dnspktdecoded) {
    netwib_er(netwib_pkt_data_show(&pkt, pctxtouse, dataencodetype, pbuf));
  }
  netwib_er(netwib_buf_encode_transition_end(pctxtouse, pbuf));

  return(NETWIB_ERR_OK);
}
