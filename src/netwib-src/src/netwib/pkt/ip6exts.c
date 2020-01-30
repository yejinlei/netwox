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
netwib_err netwib_priv_ip6exts_skip_ip6ext(netwib_ipproto pktproto,
                                           netwib_constbuf *ppkt,
                                           netwib_ipproto *pnextproto,
                                           netwib_uint32 *pskipsize)
{
  netwib_data data;
  netwib_uint32 datasize, size=0;
  netwib_ipproto nextproto;

  datasize = netwib__buf_ref_data_size(ppkt);
  if (pskipsize != NULL) *pskipsize = datasize;
  if (datasize < 2) {
    return(NETWIB_ERR_DATAMISSING);
  }
  data = netwib__buf_ref_data_ptr(ppkt);

  /* next proto */
  nextproto = data[0];
  if (pnextproto != NULL) *pnextproto = nextproto;

  /* skip size */
  switch(pktproto) {
  case NETWIB_IPPROTO_FRAGMENT :
    /* rfc 2460 makes a particular case for fragment extension header */
    if (data[1] != 0) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    size = 8;
    break;
  case NETWIB_IPPROTO_AH :
    size = 4*data[1] + 8;
    break;
  default :
    size = 8*data[1];
  }
  if (size == 0) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  if (size > datasize) {
    return(NETWIB_ERR_DATAMISSING);
  }
  if (pskipsize != NULL) *pskipsize = size;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ip6exts_skip_notfrag(netwib_ipproto pktproto,
                                            netwib_constbuf *ppkt,
                                            netwib_uint32 *pskipsize)
{
  netwib_uint32 skipsize, totalskipsize;
  netwib_ipproto nextproto;
  netwib_buf pkt;

  pkt = *ppkt;
  totalskipsize = 0;
  while (NETWIB_TRUE) {
    if (!(pktproto == NETWIB_IPPROTO_HOPOPTS ||
          pktproto == NETWIB_IPPROTO_DSTOPTS ||
          pktproto == NETWIB_IPPROTO_ROUTING)) {
      break;
    }
    if (pkt.beginoffset >= pkt.endoffset) {
      break;
    }
    netwib_er(netwib_priv_ip6exts_skip_ip6ext(pktproto, &pkt,
                                              &nextproto, &skipsize));
    totalskipsize += skipsize;
    pkt.beginoffset += skipsize;
    if (pktproto == NETWIB_IPPROTO_ROUTING) {
      /* rfc says to stop after routing (or hopopts) */
      break;
    }
    pktproto = nextproto;
  }

  if (pskipsize != NULL) *pskipsize = totalskipsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_ip6exts(netwib_ipproto pktproto,
                                     netwib_constbuf *ppkt,
                                     netwib_ipproto *plastproto,
                                     netwib_uint32 *plastprotooffset,
                                     netwib_uint32 *pskipsize)
{
  netwib_uint32 skipsize, totalskipsize;
  netwib_ipproto lastproto;
  netwib_uint32 lastprotooffset;
  netwib_ip6ext ip6ext;
  netwib_buf pkt;
  netwib_err ret;

  pkt = *ppkt;
  totalskipsize = 0;
  lastproto = pktproto;
  lastprotooffset = (netwib_uint32)-1;
  while (NETWIB_TRUE) {
    if (!(pktproto == NETWIB_IPPROTO_HOPOPTS ||
          pktproto == NETWIB_IPPROTO_DSTOPTS ||
          pktproto == NETWIB_IPPROTO_ROUTING ||
          pktproto == NETWIB_IPPROTO_FRAGMENT ||
          pktproto == NETWIB_IPPROTO_AH)) {
      break;
    }
    if (pkt.beginoffset >= pkt.endoffset) {
      break;
    }
    ret = netwib_priv_ip6exts_skip_ip6ext(pktproto, &pkt,
                                          &lastproto, &skipsize);
    if (ret != NETWIB_ERR_OK) {
      lastproto = pktproto;
      break;
    }
    /* end on fragment */
    if (pktproto == NETWIB_IPPROTO_FRAGMENT) {
      netwib_er(netwib_pkt_decode_ip6ext(pktproto, &pkt, &ip6ext, NULL));
      if (ip6ext.ext.fragment.fragmentoffset) {
        lastprotooffset = totalskipsize;
        totalskipsize += skipsize;
        break;
      }
    }
    lastprotooffset = totalskipsize;
    totalskipsize += skipsize;
    pkt.beginoffset += skipsize;
    pktproto = lastproto;
  }

  if (plastproto != NULL) *plastproto = lastproto;
  if (plastprotooffset != NULL) *plastprotooffset = lastprotooffset;
  if (pskipsize != NULL) *pskipsize = totalskipsize;

  return(NETWIB_ERR_OK);
}
