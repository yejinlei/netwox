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
netwib_err netwib_pkt_data_show(netwib_constbuf *ppkt,
                                netwib_encodetype_context *pctx,
                                netwib_encodetype encodetype,
                                netwib_buf *pbuf)
{
  netwib_er(netwib_buf_encode_transition(pctx, encodetype, pbuf));
  netwib_er(netwib_buf_encode(ppkt, encodetype, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_data_display(netwib_constbuf *ppkt,
                                   netwib_encodetype_context *pctx,
                                   netwib_encodetype encodetype)
{
  netwib_err ret;
  netwib_string pc;
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  ret = netwib_pkt_data_show(ppkt, pctx, encodetype, &buf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_encode_transition_end(pctx, &buf));
    netwib_er(netwib_buf_ref_string(&buf, &pc));
    fprintf(stdout, "%s", pc);
    fflush(stdout);
  }
  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_link_show(netwib_device_dlttype dlttype,
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
  if (ret == NETWIB_ERR_NOTCONVERTED || ret == NETWIB_ERR_DATAMISSING) {
    netwib_er(netwib_pkt_data_show(&pkt, pctxtouse, dataencodetype, pbuf));
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
      if (ret == NETWIB_ERR_NOTCONVERTED || ret == NETWIB_ERR_DATAMISSING) {
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
      netwib_er(netwib_pkt_ip_show(&pkt, pctxtouse, hdrencodetype,
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
netwib_err netwib_pkt_link_display(netwib_device_dlttype dlttype,
                                   netwib_constbuf *ppkt,
                                   netwib_encodetype_context *pctx,
                                   netwib_encodetype hdrencodetype,
                                   netwib_encodetype dataencodetype)
{
  netwib_err ret;
  netwib_string pc;
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  ret = netwib_pkt_link_show(dlttype, ppkt, pctx, hdrencodetype,
                             dataencodetype, &buf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_ref_string(&buf, &pc));
    fprintf(stdout, "%s", pc);
    fflush(stdout);
  }
  netwib_er(netwib_buf_close(&buf));

  return(ret);
}


/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_ip_show_isfrag(netwib_constiphdr *piphdr,
                                            netwib_bool *pisfrag)
{
  netwib_uint32 skipsize;
  netwib_ipproto pktproto;
  netwib_ip6ext ip6ext;
  netwib_buf pkt;
  netwib_err ret;

  *pisfrag = NETWIB_FALSE;

  ret = NETWIB_ERR_OK;
  switch(piphdr->iptype) {
  case NETWIB_IPTYPE_IP4 :
    if (piphdr->header.ip4.offsetfrag) {
      *pisfrag = NETWIB_TRUE;
    }
    break;
  case NETWIB_IPTYPE_IP6 :
    if (netwib__buf_ref_data_size(&piphdr->header.ip6.exts) != 0) {
      pktproto = piphdr->protocol;
      pkt = piphdr->header.ip6.exts;
      while (NETWIB_TRUE) {
        if (pktproto == NETWIB_IPPROTO_FRAGMENT) {
          ret = netwib_pkt_decode_ip6ext(pktproto, &pkt, &ip6ext, NULL);
          if (ret == NETWIB_ERR_OK) {
            if (ip6ext.ext.fragment.fragmentoffset) {
              *pisfrag = NETWIB_TRUE;
            }
          }
          break;
        }
        ret = netwib_priv_ip6exts_skip_ip6ext(pktproto, &pkt,
                                              &pktproto, &skipsize);
        if (ret != NETWIB_ERR_OK) {
          if (ret == NETWIB_ERR_NOTCONVERTED) ret = NETWIB_ERR_OK;
          if (ret == NETWIB_ERR_DATAMISSING) ret = NETWIB_ERR_OK;
          break;
        }
        pkt.beginoffset += skipsize;
      }
    }
    break;
  default :
    return(NETWIB_ERR_PAIPTYPE);
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_ip_show(netwib_constbuf *ppkt,
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
  if (ret == NETWIB_ERR_NOTCONVERTED || ret == NETWIB_ERR_DATAMISSING) {
    netwib_er(netwib_pkt_data_show(&pkt, pctxtouse, dataencodetype, pbuf));
    netwib_er(netwib_buf_encode_transition_end(pctxtouse, pbuf));
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  netwib_er(netwib_buf_encode_transition(pctxtouse, hdrencodetype, pbuf));
  netwib_er(netwib_iphdr_show(&iphdr, hdrencodetype, pbuf));

  /* fragments cannot be displayed */
  netwib_er(netwib_pkt_ip_show_isfrag(&iphdr, &isfragment));
  if (isfragment) {
    netwib_er(netwib_pkt_data_show(&pkt, pctxtouse, dataencodetype, pbuf));
    netwib_er(netwib_buf_encode_transition_end(pctxtouse, pbuf));
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_iphdr_get_proto(&iphdr, &ipproto));
  switch(ipproto) {
    case NETWIB_IPPROTO_UDP :
      netwib_er(netwib_pkt_udp_show(&pkt, pctxtouse, hdrencodetype,
                                    dataencodetype, pbuf));
      break;
    case NETWIB_IPPROTO_TCP :
      netwib_er(netwib_pkt_tcp_show(&pkt, pctxtouse, hdrencodetype,
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
netwib_err netwib_pkt_ip_display(netwib_constbuf *ppkt,
                                 netwib_encodetype_context *pctx,
                                 netwib_encodetype hdrencodetype,
                                 netwib_encodetype dataencodetype)
{
  netwib_err ret;
  netwib_string pc;
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  ret = netwib_pkt_ip_show(ppkt, pctx, hdrencodetype, dataencodetype,
                                 &buf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_ref_string(&buf, &pc));
    fprintf(stdout, "%s", pc);
    fflush(stdout);
  }
  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_udp_show(netwib_constbuf *ppkt,
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
  if (ret == NETWIB_ERR_NOTCONVERTED || ret == NETWIB_ERR_DATAMISSING) {
    netwib_er(netwib_pkt_data_show(&pkt, pctxtouse, dataencodetype, pbuf));
    netwib_er(netwib_buf_encode_transition_end(pctxtouse, pbuf));
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  netwib_er(netwib_buf_encode_transition(pctxtouse, hdrencodetype, pbuf));
  netwib_er(netwib_udphdr_show(&udpheader, hdrencodetype, pbuf));

  netwib_er(netwib_pkt_data_show(&pkt, pctxtouse, dataencodetype, pbuf));
  netwib_er(netwib_buf_encode_transition_end(pctxtouse, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_udp_display(netwib_constbuf *ppkt,
                                  netwib_encodetype_context *pctx,
                                  netwib_encodetype hdrencodetype,
                                  netwib_encodetype dataencodetype)
{
  netwib_err ret;
  netwib_string pc;
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  ret = netwib_pkt_udp_show(ppkt, pctx, hdrencodetype, dataencodetype, &buf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_ref_string(&buf, &pc));
    fprintf(stdout, "%s", pc);
    fflush(stdout);
  }
  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_tcp_show(netwib_constbuf *ppkt,
                               netwib_encodetype_context *pctx,
                               netwib_encodetype hdrencodetype,
                               netwib_encodetype dataencodetype,
                               netwib_buf *pbuf)
{
  netwib_tcphdr tcpheader;
  netwib_buf pkt;
  netwib_encodetype_context *pctxtouse, ctx;
  netwib_err ret;

  pctxtouse = pctx;
  if (pctx == NULL) {
    netwib_er(netwib_buf_encode_transition_init(&ctx));
    pctxtouse = &ctx;
  }

  pkt = *ppkt;
  ret = netwib_pkt_decode_layer_tcp(&pkt, &tcpheader);
  if (ret == NETWIB_ERR_NOTCONVERTED || ret == NETWIB_ERR_DATAMISSING) {
    netwib_er(netwib_pkt_data_show(&pkt, pctxtouse, dataencodetype, pbuf));
    netwib_er(netwib_buf_encode_transition_end(pctxtouse, pbuf));
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  netwib_er(netwib_buf_encode_transition(pctxtouse, hdrencodetype, pbuf));
  netwib_er(netwib_tcphdr_show(&tcpheader, hdrencodetype, pbuf));

  netwib_er(netwib_pkt_data_show(&pkt, pctxtouse, dataencodetype, pbuf));
  netwib_er(netwib_buf_encode_transition_end(pctxtouse, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_tcp_display(netwib_constbuf *ppkt,
                                  netwib_encodetype_context *pctx,
                                  netwib_encodetype hdrencodetype,
                                  netwib_encodetype dataencodetype)
{
  netwib_err ret;
  netwib_string pc;
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  ret = netwib_pkt_tcp_show(ppkt, pctx, hdrencodetype, dataencodetype, &buf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_ref_string(&buf, &pc));
    fprintf(stdout, "%s", pc);
    fflush(stdout);
  }
  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_icmp4_show(netwib_constbuf *ppkt,
                                 netwib_encodetype_context *pctx,
                                 netwib_encodetype hdrencodetype,
                                 netwib_encodetype dataencodetype,
                                 netwib_buf *pbuf)
{
  netwib_icmp4 icmp4;
  netwib_buf pkt;
  netwib_encodetype_context *pctxtouse, ctx;
  netwib_err ret;

  pctxtouse = pctx;
  if (pctx == NULL) {
    netwib_er(netwib_buf_encode_transition_init(&ctx));
    pctxtouse = &ctx;
  }

  pkt = *ppkt;
  ret = netwib_pkt_decode_layer_icmp4(&pkt, &icmp4);
  if (ret == NETWIB_ERR_NOTCONVERTED || ret == NETWIB_ERR_DATAMISSING) {
    netwib_er(netwib_pkt_data_show(&pkt, pctxtouse, dataencodetype, pbuf));
    netwib_er(netwib_buf_encode_transition_end(pctxtouse, pbuf));
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  netwib_er(netwib_buf_encode_transition(pctxtouse, hdrencodetype, pbuf));
  netwib_er(netwib_icmp4_show(&icmp4, hdrencodetype, pbuf));
  netwib_er(netwib_buf_encode_transition_end(pctxtouse, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_icmp4_display(netwib_constbuf *ppkt,
                                    netwib_encodetype_context *pctx,
                                    netwib_encodetype hdrencodetype,
                                    netwib_encodetype dataencodetype)
{
  netwib_err ret;
  netwib_string pc;
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  ret = netwib_pkt_icmp4_show(ppkt, pctx, hdrencodetype, dataencodetype, &buf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_ref_string(&buf, &pc));
    fprintf(stdout, "%s", pc);
    fflush(stdout);
  }
  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_icmp6_show(netwib_constbuf *ppkt,
                                 netwib_encodetype_context *pctx,
                                 netwib_encodetype hdrencodetype,
                                 netwib_encodetype dataencodetype,
                                 netwib_buf *pbuf)
{
  netwib_icmp6 icmp6;
  netwib_buf pkt;
  netwib_encodetype_context *pctxtouse, ctx;
  netwib_err ret;

  pctxtouse = pctx;
  if (pctx == NULL) {
    netwib_er(netwib_buf_encode_transition_init(&ctx));
    pctxtouse = &ctx;
  }

  pkt = *ppkt;
  ret = netwib_pkt_decode_layer_icmp6(&pkt, &icmp6);
  if (ret == NETWIB_ERR_NOTCONVERTED || ret == NETWIB_ERR_DATAMISSING) {
    netwib_er(netwib_pkt_data_show(&pkt, pctxtouse, dataencodetype, pbuf));
    netwib_er(netwib_buf_encode_transition_end(pctxtouse, pbuf));
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  netwib_er(netwib_buf_encode_transition(pctxtouse, hdrencodetype, pbuf));
  netwib_er(netwib_icmp6_show(&icmp6, hdrencodetype, pbuf));
  netwib_er(netwib_buf_encode_transition_end(pctxtouse, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_icmp6_display(netwib_constbuf *ppkt,
                                    netwib_encodetype_context *pctx,
                                    netwib_encodetype hdrencodetype,
                                    netwib_encodetype dataencodetype)
{
  netwib_err ret;
  netwib_string pc;
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  ret = netwib_pkt_icmp6_show(ppkt, pctx, hdrencodetype, dataencodetype, &buf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_ref_string(&buf, &pc));
    fprintf(stdout, "%s", pc);
    fflush(stdout);
  }
  netwib_er(netwib_buf_close(&buf));

  return(ret);
}


