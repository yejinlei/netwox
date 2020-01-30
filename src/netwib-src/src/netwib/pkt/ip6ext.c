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
netwib_err netwib_ip6ext_initdefault(netwib_ipproto proto,
                                     netwib_ip6ext *pip6ext)
{

  pip6ext->proto = proto;
  pip6ext->nextproto = NETWIB_IPPROTO_NONE;

  switch(proto) {
  case NETWIB_IPPROTO_HOPOPTS :
    netwib_er(netwib_buf_init_ext_empty(&pip6ext->ext.hopopts.options));
    break;
  case NETWIB_IPPROTO_DSTOPTS :
    netwib_er(netwib_buf_init_ext_empty(&pip6ext->ext.dstopts.options));
    break;
  case NETWIB_IPPROTO_ROUTING :
    pip6ext->ext.routing.routingtype = 0;
    pip6ext->ext.routing.segmentsleft = 0;
    netwib_er(netwib_buf_init_ext_empty(&pip6ext->ext.routing.data));
    break;
  case NETWIB_IPPROTO_FRAGMENT :
    pip6ext->ext.fragment.fragmentoffset = 0;
    pip6ext->ext.fragment.reservedb1 = NETWIB_FALSE;
    pip6ext->ext.fragment.reservedb2 = NETWIB_FALSE;
    pip6ext->ext.fragment.morefrag = NETWIB_FALSE;
    netwib_er(netwib_uint32_init_rand(1, 0xFFFFFFFFu,
                                      &pip6ext->ext.fragment.id));
    break;
  case NETWIB_IPPROTO_AH :
    pip6ext->ext.ah.reserved = 0;
    pip6ext->ext.ah.spi = 0;
    pip6ext->ext.ah.seqnum = 0;
    netwib_er(netwib_buf_init_ext_empty(&pip6ext->ext.ah.data));
    break;
  default :
    return(NETWIB_ERR_LONOTIMPLEMENTED);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_ip6ext_pad(netwib_uint32 padsize,
                                               netwib_buf *ppkt)
{
  netwib_uint32 i;

  switch(padsize) {
  case 0 :
    break;
  case 1 :
    netwib_er(netwib_buf_append_byte(0, ppkt));
    break;
  default :
    netwib_er(netwib_buf_append_byte(1, ppkt));
    padsize -= 2;
    netwib_er(netwib_buf_append_byte((netwib_byte)padsize, ppkt));
    for (i = 0 ; i < padsize; i++) {
      netwib_er(netwib_buf_append_byte(0, ppkt));
    }
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_ip6ext(netwib_constip6ext *pip6ext,
                                    netwib_buf *ppkt)
{
  netwib_data data;
  netwib_uint32 pktsize=0, hdrextlen, paddingsize, fragoff;

  switch(pip6ext->proto) {
  case NETWIB_IPPROTO_HOPOPTS :
    pktsize = 2 + netwib__buf_ref_data_size(&pip6ext->ext.hopopts.options);
    hdrextlen = (pktsize + 7)/8;
    paddingsize = 8*hdrextlen - pktsize;
    netwib_er(netwib_buf_wantspace(ppkt, 8*hdrextlen, &data));
    netwib__data_append_uint8(data, pip6ext->nextproto);
    netwib__data_append_uint8(data, hdrextlen);
    ppkt->endoffset += 2;
    netwib_er(netwib_buf_append_buf(&pip6ext->ext.hopopts.options, ppkt));
    netwib_er(netwib_pkt_append_ip6ext_pad(paddingsize, ppkt));
    break;
  case NETWIB_IPPROTO_DSTOPTS :
    pktsize = 2 + netwib__buf_ref_data_size(&pip6ext->ext.dstopts.options);
    hdrextlen = (pktsize + 7)/8;
    paddingsize = 8*hdrextlen - pktsize;
    netwib_er(netwib_buf_wantspace(ppkt, 8*hdrextlen, &data));
    netwib__data_append_uint8(data, pip6ext->nextproto);
    netwib__data_append_uint8(data, hdrextlen);
    ppkt->endoffset += 2;
    netwib_er(netwib_buf_append_buf(&pip6ext->ext.dstopts.options, ppkt));
    netwib_er(netwib_pkt_append_ip6ext_pad(paddingsize, ppkt));
    break;
  case NETWIB_IPPROTO_ROUTING :
    pktsize = 4 + netwib__buf_ref_data_size(&pip6ext->ext.routing.data);
    if ((pktsize%8) && (pktsize!=4)) {
      /* data must be aligned on 64 bits */
      return(NETWIB_ERR_PATOOLOW);
    }
    netwib_er(netwib_buf_wantspace(ppkt, pktsize, &data));
    netwib__data_append_uint8(data, pip6ext->nextproto);
    netwib__data_append_uint8(data, pktsize/8);
    netwib__data_append_uint8(data, pip6ext->ext.routing.routingtype);
    netwib__data_append_uint8(data, pip6ext->ext.routing.segmentsleft);
    ppkt->endoffset += 4;
    if (pktsize == 4) {
      netwib_er(netwib_buf_wantspace(ppkt, 4, &data));
      netwib__data_append_uint32(data, 0);
      ppkt->endoffset += 4;
    } else {
      netwib_er(netwib_buf_append_buf(&pip6ext->ext.routing.data, ppkt));
    }
    break;
  case NETWIB_IPPROTO_FRAGMENT :
    netwib_er(netwib_buf_wantspace(ppkt, 8, &data));
    netwib__data_append_uint8(data, pip6ext->nextproto);
    netwib__data_append_uint8(data, 0);
    fragoff = pip6ext->ext.fragment.fragmentoffset<<3;
    if (pip6ext->ext.fragment.reservedb1) {
      fragoff |= 4;
    }
    if (pip6ext->ext.fragment.reservedb2) {
      fragoff |= 2;
    }
    if (pip6ext->ext.fragment.morefrag) {
      fragoff |= 1;
    }
    netwib__data_append_uint16(data, fragoff);
    netwib__data_append_uint32(data, pip6ext->ext.fragment.id);
    ppkt->endoffset += 8;
    break;
  case NETWIB_IPPROTO_AH :
    pktsize = 12 + netwib__buf_ref_data_size(&pip6ext->ext.ah.data);
    if (pktsize % 4) {
      /* data must be aligned on 32 bits */
      return(NETWIB_ERR_PATOOLOW);
    }
    netwib_er(netwib_buf_wantspace(ppkt, pktsize, &data));
    netwib__data_append_uint8(data, pip6ext->nextproto);
    netwib__data_append_uint8(data, pktsize/4 - 2);
    netwib__data_append_uint16(data, pip6ext->ext.ah.reserved);
    netwib__data_append_uint32(data, pip6ext->ext.ah.spi);
    netwib__data_append_uint32(data, pip6ext->ext.ah.seqnum);
    ppkt->endoffset += 12;
    netwib_er(netwib_buf_append_buf(&pip6ext->ext.ah.data, ppkt));
    break;
  default :
    return(NETWIB_ERR_LONOTIMPLEMENTED);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_prepend_ip6ext(netwib_constip6ext *pip6ext,
                                     netwib_buf *ppkt)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));
  ret = netwib_pkt_append_ip6ext(pip6ext, &buf);
  if (ret == NETWIB_ERR_OK) {
    ret = netwib_buf_prepend_buf(&buf, ppkt);
  }
  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_ip6ext(netwib_ipproto pktproto,
                                    netwib_constbuf *ppkt,
                                    netwib_ip6ext *pip6ext,
                                    netwib_uint32 *pskipsize)
{
  netwib_data data;
  netwib_uint32 ui16, ui32, skipsize;

  netwib_er(netwib_priv_ip6exts_skip_ip6ext(pktproto, ppkt,
                                            &pip6ext->nextproto, &skipsize));
  if (pskipsize != NULL) *pskipsize = skipsize;

  data = netwib__buf_ref_data_ptr(ppkt);

  pip6ext->proto = pktproto;
  switch(pktproto) {
  case NETWIB_IPPROTO_HOPOPTS :
    netwib_er(netwib_buf_init_ext_arrayfilled(data+2, skipsize-2,
                                              &pip6ext->ext.hopopts.options));
    break;
  case NETWIB_IPPROTO_DSTOPTS :
    netwib_er(netwib_buf_init_ext_arrayfilled(data+2, skipsize-2,
                                              &pip6ext->ext.dstopts.options));
    break;
  case NETWIB_IPPROTO_ROUTING :
    pip6ext->ext.routing.routingtype = data[2];
    pip6ext->ext.routing.segmentsleft = data[3];
    netwib_er(netwib_buf_init_ext_arrayfilled(data+4, skipsize-4,
                                              &pip6ext->ext.routing.data));
    break;
  case NETWIB_IPPROTO_FRAGMENT :
    ui16 = netwib_c2_uint16_2(data[2], data[3]);
    pip6ext->ext.fragment.fragmentoffset = (netwib_uint16)(ui16 >> 3);
    pip6ext->ext.fragment.reservedb1 = (ui16&4)?NETWIB_TRUE:NETWIB_FALSE;
    pip6ext->ext.fragment.reservedb2 = (ui16&2)?NETWIB_TRUE:NETWIB_FALSE;
    pip6ext->ext.fragment.morefrag = (ui16&1)?NETWIB_TRUE:NETWIB_FALSE;
    pip6ext->ext.fragment.id = netwib_c2_uint32_4(data[4], data[5], data[6],
                                                  data[7]);
    break;
  case NETWIB_IPPROTO_AH :
    ui16 = netwib_c2_uint16_2(data[2], data[3]);
    pip6ext->ext.ah.reserved = (netwib_uint16)(ui16 >> 3);
    ui32 = netwib_c2_uint32_4(data[4], data[5], data[6], data[7]);
    pip6ext->ext.ah.spi = ui32;
    ui32 = netwib_c2_uint32_4(data[8], data[9], data[10], data[11]);
    pip6ext->ext.ah.seqnum = ui32;
    netwib_er(netwib_buf_init_ext_arrayfilled(data+12, skipsize-12,
                                              &pip6ext->ext.ah.data));
    break;
  default :
    return(NETWIB_ERR_LONOTIMPLEMENTED);
  }

  return(NETWIB_ERR_OK);
}
