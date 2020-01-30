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
static netwib_err netwox_icmp6nd_addr(netwib_constip *pip,
                                      netwib_eth *pwantedeth,
                                      netwib_ip *pwantedip)
{

  /* rfc 2373 :
         ip=1234:5678:90ab:cdef:1122:3344:5566:7788 -> ip=FF02::1:FF66:7788
     rfc 2464 :
         ip=1234:5678:90ab:cdef:1122:3344:5566:7788 -> eth=33:33:55:66:77:88
     example :
         pip = 1234:5678:90ab:cdef:1122:3344:5566:7788
         ip6.dst = FF02::1:FF66:7788
         eth.dst = 33:33:FF:66:77:88 (computed from ip6.dst and not pip)
  */
  if (pwantedip != NULL) {
    netwib_er(netwib_ip_init_ip6_fields(0xFF020000u, 0, 1, 0xFF000000u,
                                        pwantedip));
    pwantedip->ipvalue.ip6.b[13] = pip->ipvalue.ip6.b[13];
    pwantedip->ipvalue.ip6.b[14] = pip->ipvalue.ip6.b[14];
    pwantedip->ipvalue.ip6.b[15] = pip->ipvalue.ip6.b[15];
  }

  if (pwantedeth != NULL) {
    netwib_er(netwib_eth_init_fields(0x33, 0x33, 0xFF,
                                     pip->ipvalue.ip6.b[13],
                                     pip->ipvalue.ip6.b[14],
                                     pip->ipvalue.ip6.b[15],
                                     pwantedeth));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_icmp6nd_alive(netwib_consteth *plocaleth,
                                netwib_constip *plocalip,
                                netwib_constlinkhdr *psniffedlinkhdr,
                                netwib_constiphdr *psniffediphdr,
                                netwib_consticmp6 *psniffedicmp6,
                                netwib_bool *ppktset,
                                netwib_buf *ppkt)
{
  netwib_linkhdr linkheader;
  netwib_iphdr ipheader;
  netwib_icmp6 icmp6;
  netwib_icmp6nd icmp6nd;
  netwib_byte array[80];
  netwib_buf buf;
  netwib_eth wantedeth;
  netwib_ip wantedip;
  netwib_cmp cmp;

  *ppktset = NETWIB_FALSE;
  if (plocalip->iptype != NETWIB_IPTYPE_IP6) return(NETWIB_ERR_OK);

  /* check it's for us */
  netwib_er(netwox_icmp6nd_addr(plocalip, &wantedeth, &wantedip));
  if (psniffedlinkhdr->type != NETWIB_DEVICE_DLTTYPE_ETHER)
    return(NETWIB_ERR_OK);
  netwib_er(netwib_eth_cmp(&psniffedlinkhdr->hdr.ether.dst, &wantedeth, &cmp));
  if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
  netwib_er(netwib_ip_cmp(&psniffediphdr->dst, &wantedip, &cmp));
  if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);
  if (psniffediphdr->ttl != 255) return(NETWIB_ERR_OK);
  if (psniffedicmp6->type != NETWIB_ICMP6TYPE_NEIGHBORSOLICIT)
    return(NETWIB_ERR_OK);
  if (psniffedicmp6->code != NETWIB_ICMP6CODE_NEIGHBORSOLICIT)
    return(NETWIB_ERR_OK);
  netwib_er(netwib_ip_cmp(&psniffedicmp6->msg.neighborsolicit.target, plocalip,
                          &cmp));
  if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);

  /* answer */
  netwib_er(netwib_linkhdr_initdefault(NETWIB_DEVICE_DLTTYPE_ETHER,
                                       &linkheader));
  linkheader.hdr.ether.src = *plocaleth;
  linkheader.hdr.ether.dst = psniffedlinkhdr->hdr.ether.src;

  netwib_er(netwib_iphdr_initdefault(psniffediphdr->iptype, &ipheader));
  ipheader.src = *plocalip;
  ipheader.dst = psniffediphdr->src;
  ipheader.ttl = 255;

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_NEIGHBORADVERT, &icmp6));
  icmp6.msg.neighboradvert.solicited = NETWIB_TRUE;
  icmp6.msg.neighboradvert.override = NETWIB_TRUE;
  icmp6.msg.neighboradvert.target = *plocalip;

  netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));
  netwib_er(netwib_icmp6nd_initdefault(NETWIB_ICMP6NDTYPE_DSTLINK,
                                       &icmp6nd));
  icmp6nd.opt.link.linkad = *plocaleth;
  netwib_er(netwib_pkt_append_icmp6nd(&icmp6nd, &buf));
  icmp6.msg.neighboradvert.options = buf;

  netwib_er(netwib_pkt_append_linkipicmp6(&linkheader, &ipheader, &icmp6,
                                          ppkt));
  *ppktset = NETWIB_TRUE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_icmp6nd_ns(netwib_consteth *plocaleth,
                             netwib_constip *plocalip,
                             netwib_constip *pqueriedip,
                             netwib_buf *ppkt)
{
  netwib_linkhdr linkhdr;
  netwib_iphdr ipheader;
  netwib_icmp6 icmp6;
  netwib_icmp6nd icmp6nd;
  netwib_byte array[80];
  netwib_buf buf;

  netwib_er(netwib_linkhdr_initdefault(NETWIB_DEVICE_DLTTYPE_ETHER, &linkhdr));
  linkhdr.hdr.ether.src = *plocaleth;

  netwib_er(netwib_iphdr_initdefault(NETWIB_IPTYPE_IP6, &ipheader));
  ipheader.src = *plocalip;
  ipheader.ttl = 255;

  netwib_er(netwox_icmp6nd_addr(pqueriedip, &linkhdr.hdr.ether.dst,
                                &ipheader.dst));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_NEIGHBORSOLICIT,
                                     &icmp6));
  icmp6.msg.neighborsolicit.target = *pqueriedip;
  netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));
  netwib_er(netwib_icmp6nd_initdefault(NETWIB_ICMP6NDTYPE_SRCLINK,
                                       &icmp6nd));
  icmp6nd.opt.link.linkad = *plocaleth;
  netwib_er(netwib_pkt_append_icmp6nd(&icmp6nd, &buf));

  icmp6.msg.neighborsolicit.options = buf;
  netwib_er(netwib_pkt_append_linkipicmp6(&linkhdr, &ipheader, &icmp6, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_icmp6nd_na(netwib_constip *pwantedip,
                             netwib_constlinkhdr *psniffedlinkhdr,
                             netwib_constiphdr *psniffediphdr,
                             netwib_consticmp6 *psniffedicmp6,
                             netwib_bool *pobtainedethset,
                             netwib_eth *pobtainedeth)
{
  netwib_icmp6nd icmp6nd;
  netwib_uint32 skipsize;
  netwib_buf opts;
  netwib_cmp cmp;
  netwib_err ret;

  *pobtainedethset= NETWIB_FALSE;

  if (psniffediphdr->iptype != NETWIB_IPTYPE_IP6) return(NETWIB_ERR_OK);
  if (psniffediphdr->ttl != 255) return(NETWIB_ERR_OK);

  if (psniffedicmp6->type != NETWIB_ICMP6TYPE_NEIGHBORADVERT ||
      psniffedicmp6->code != NETWIB_ICMP6CODE_NEIGHBORADVERT) {
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_ip_cmp(&psniffedicmp6->msg.neighboradvert.target,
                          pwantedip, &cmp));
  if (cmp != NETWIB_CMP_EQ) return(NETWIB_ERR_OK);

  opts = psniffedicmp6->msg.neighboradvert.options;
  while (NETWIB_TRUE) {
    if (opts.beginoffset >= opts.endoffset) {
      break;
    }
    ret = netwib_pkt_decode_icmp6nd(&opts, &icmp6nd, &skipsize);
    if (ret == NETWIB_ERR_OK) {
      if (icmp6nd.type == NETWIB_ICMP6NDTYPE_DSTLINK) {
        if (pobtainedeth != NULL) {
          *pobtainedeth = icmp6nd.opt.link.linkad;
        }
        *pobtainedethset= NETWIB_TRUE;
        return(NETWIB_ERR_OK);
      }
    }
    opts.beginoffset += skipsize;
  }

  psniffedlinkhdr = psniffedlinkhdr; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
