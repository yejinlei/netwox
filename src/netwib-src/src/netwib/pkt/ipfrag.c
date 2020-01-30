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
static netwib_err netwib_ip_frag_ip4opts(netwib_constbuf *pip4opts,
                                         netwib_buf *pip4opts2)
{
  netwib_data data;
  netwib_uint32 datasize, optsize;
  netwib_bool continuetoloop;

  data = netwib__buf_ref_data_ptr(pip4opts);
  datasize = netwib__buf_ref_data_size(pip4opts);

  continuetoloop = NETWIB_TRUE;
  while(continuetoloop) {
    if (datasize == 0) {
      break;
    }
    switch(data[0]) {
    case NETWIB_IP4OPTTYPE_END :
      /* not copied, and force end */
      datasize = 0;
      break;
    case NETWIB_IP4OPTTYPE_NOOP :
      /* copied, and shift */
      netwib_er(netwib_buf_append_byte(NETWIB_IP4OPTTYPE_NOOP, pip4opts2));
      data ++;
      datasize --;
      break;
    default :
      /* From rfc 791, when MSB is 0, option is not copied */
      if (datasize < 2) {
        continuetoloop = NETWIB_FALSE;
        break;
      }
      optsize = data[1];
      if (optsize > datasize) {
        continuetoloop = NETWIB_FALSE;
        break;
      }
      if (data[0] & 0x80) {
        netwib_er(netwib_buf_append_data(data, optsize, pip4opts2));
      }
      data += optsize;
      datasize -= optsize;
      break;
    }
  }

  /* now, check if we need to add an END */
  if (netwib__buf_ref_data_size(pip4opts2) & 0x3) {
    netwib_er(netwib_buf_append_byte(NETWIB_IP4OPTTYPE_END, pip4opts2));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ip_frag_ip6exts(netwib_iphdr *piphdr,
                                         netwib_buf *pip6exts,
                                         netwib_ip6ext *pip6extfragment)
{

  netwib_uint32 lastprotooffset;
  netwib_ipproto ipproto;
  netwib_data data;

  netwib_er(netwib_ip6ext_initdefault(NETWIB_IPPROTO_FRAGMENT,
                                      pip6extfragment));

  if (netwib__buf_ref_data_size(&piphdr->header.ip6.exts) == 0) {
    ipproto = piphdr->protocol;
    piphdr->protocol = NETWIB_IPPROTO_FRAGMENT;
    pip6extfragment->nextproto = ipproto;
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_buf_append_buf(&piphdr->header.ip6.exts, pip6exts));

  netwib_er(netwib_pkt_decode_ip6exts(piphdr->protocol, pip6exts,
                                      &ipproto, &lastprotooffset, NULL));
  data = netwib__buf_ref_data_ptr(pip6exts);
  data[lastprotooffset] = NETWIB_IPPROTO_FRAGMENT;
  pip6extfragment->nextproto = ipproto;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ip_frag_ip(netwib_constbuf *pippkt,
                                    netwib_iphdr *piphdr,
                                    netwib_buf *pipdata,
                                    netwib_uint32 fragdatasize,
                                    netwib_uint32 fragtotalsize,
                                    netwib_ip_frag_pf pfunc,
                                    netwib_ptr infos)
{
  netwib_uint32 leftipdatasize, fragipdatasize, sentipdatasize;
  netwib_byte ip4optsarray[NETWIB_IP4OPT_MAXLEN];
  netwib_buf ip4opts, ip6exts, bufipfrag;
  netwib_ip6ext ip6extfragment;
  netwib_uint32 ip6extssize=0, iphdrsize;
  netwib_bool isip4;
  netwib_err ret=NETWIB_ERR_OK;

  /* determine type */
  isip4 = NETWIB_FALSE;
  if (piphdr->iptype == NETWIB_IPTYPE_IP4) {
    isip4 = NETWIB_TRUE;
  }

  /* allocate working memory */
  netwib_er(netwib_buf_init_mallocdefault(&bufipfrag));

  /* obtain IP6 extensions */
  ip6exts.beginoffset = 0; /* compiler warning */
  if (!isip4) {
    netwib_er(netwib_buf_init_mallocdefault(&ip6exts));
    netwib_eg(netwib_ip_frag_ip6exts(piphdr, &ip6exts, &ip6extfragment));
    ip6extssize = netwib__buf_ref_data_size(&ip6exts);
  }

  /* send first fragment */
  leftipdatasize = netwib__buf_ref_data_size(pipdata);
  if (isip4) {
    piphdr->header.ip4.dontfrag = NETWIB_FALSE;
    piphdr->header.ip4.morefrag = NETWIB_TRUE;
    piphdr->header.ip4.offsetfrag = 0;
    iphdrsize = NETWIB_PRIV_IP4HDR_MINLEN +
      netwib__buf_ref_data_size(&piphdr->header.ip4.opts);
  } else {
    ip6extfragment.ext.fragment.morefrag = NETWIB_TRUE;
    ip6extfragment.ext.fragment.fragmentoffset = 0;
    netwib_eg(netwib_pkt_append_ip6ext(&ip6extfragment, &ip6exts));
    piphdr->header.ip6.exts = ip6exts;
    iphdrsize = NETWIB_PRIV_IP6HDR_MINLEN +
      netwib__buf_ref_data_size(&piphdr->header.ip6.exts) + 8/*fragment*/;
  }
  if (fragtotalsize) {
    if (fragtotalsize <= iphdrsize) {
      return(NETWIB_ERR_PATOOLOW);
    }
    fragipdatasize = 8*((fragtotalsize-iphdrsize)/8);
  } else {
    fragipdatasize = 0xFFFFFFFFu;
  }
  if (fragdatasize) {
    fragdatasize = 8*((fragdatasize+7)/8);
    if (fragdatasize < fragipdatasize) {
      fragipdatasize = fragdatasize;
    }
  }

  /* special case where fragmentation is finally not needed */
  if (fragipdatasize >= leftipdatasize) {
    ret = (*pfunc)(pippkt, infos);
    netwib_goto(ret);
  }

  /* send first fragment */
  pipdata->endoffset = pipdata->beginoffset + fragipdatasize;
  netwib__buf_reinit(&bufipfrag);
  netwib_eg(netwib_pkt_append_ipdata(piphdr, pipdata, &bufipfrag));
  netwib_eg((*pfunc)(&bufipfrag, infos));
  sentipdatasize = fragipdatasize;
  leftipdatasize -= fragipdatasize;
  pipdata->beginoffset += fragipdatasize;

  /* decide which IP4 options have to be copied over fragments */
  if (isip4) {
    netwib_eg(netwib_buf_init_ext_arraysizeofempty(ip4optsarray, &ip4opts));
    netwib_eg(netwib_ip_frag_ip4opts(&piphdr->header.ip4.opts, &ip4opts));
    piphdr->header.ip4.opts = ip4opts;
    iphdrsize = NETWIB_PRIV_IP4HDR_MINLEN +
      netwib__buf_ref_data_size(&piphdr->header.ip4.opts);
    if (fragtotalsize) {
      /* need to recompute fragipdatasize */
      fragipdatasize = 8*((fragtotalsize-iphdrsize)/8);
      if (fragdatasize) {
        if (fragdatasize < fragipdatasize) {
          fragipdatasize = fragdatasize;
        }
      }
    }
  }

  /* send complete fragments */
  while(leftipdatasize > fragipdatasize) {
    pipdata->endoffset = pipdata->beginoffset + fragipdatasize;
    if (isip4) {
      piphdr->header.ip4.offsetfrag = (netwib_uint16)(sentipdatasize/8);
    } else {
      ip6exts.endoffset = ip6exts.beginoffset + ip6extssize;
      ip6extfragment.ext.fragment.fragmentoffset =
        (netwib_uint16)(sentipdatasize/8);
      netwib_eg(netwib_pkt_append_ip6ext(&ip6extfragment, &ip6exts));
      piphdr->header.ip6.exts = ip6exts;
    }
    netwib__buf_reinit(&bufipfrag);
    netwib_eg(netwib_pkt_append_ipdata(piphdr, pipdata, &bufipfrag));
    netwib_eg((*pfunc)(&bufipfrag, infos));
    sentipdatasize += fragipdatasize;
    leftipdatasize -= fragipdatasize;
    pipdata->beginoffset += fragipdatasize;
  }

  /* send last fragment */
  pipdata->endoffset = pipdata->beginoffset + leftipdatasize;
  if (isip4) {
    piphdr->header.ip4.morefrag = NETWIB_FALSE;
    piphdr->header.ip4.offsetfrag = (netwib_uint16)(sentipdatasize/8);
  } else {
    ip6exts.endoffset = ip6exts.beginoffset + ip6extssize;
    ip6extfragment.ext.fragment.morefrag = NETWIB_FALSE;
    ip6extfragment.ext.fragment.fragmentoffset =
      (netwib_uint16)(sentipdatasize/8);
    netwib_eg(netwib_pkt_append_ip6ext(&ip6extfragment, &ip6exts));
    piphdr->header.ip6.exts = ip6exts;
  }
  netwib__buf_reinit(&bufipfrag);
  netwib_eg(netwib_pkt_append_ipdata(piphdr, pipdata, &bufipfrag));
  ret = (*pfunc)(&bufipfrag, infos);

 netwib_gotolabel:
  netwib_er(netwib_buf_close(&bufipfrag));
  if (!isip4) {
    netwib_er(netwib_buf_close(&ip6exts));
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ip_frag(netwib_constbuf *pippkt,
                          netwib_uint32 fragdatasize,
                          netwib_uint32 fragtotalsize,
                          netwib_ip_frag_pf pfunc,
                          netwib_ptr infos)
{
  netwib_buf ippkt;
  netwib_iphdr iphdr;
  netwib_uint32 skipsize, shiftsize;

  if (netwib__buf_ref_data_size(pippkt) > 0xFFFF) {
    return(NETWIB_ERR_PATOOHIGH);
  }

  /* special cases where fragmentation are not needed */
  if (fragtotalsize > netwib__buf_ref_data_size(pippkt)) {
    fragtotalsize = 0;
  }
  if (fragdatasize > netwib__buf_ref_data_size(pippkt)) {
    fragdatasize = 0;
  }
  if (fragdatasize == 0 && fragtotalsize == 0) {
    netwib_er((*pfunc)(pippkt, infos));
    return(NETWIB_ERR_OK);
  }

  /* decode IP header */
  ippkt = *pippkt;
  netwib_er(netwib_pkt_decode_layer_ip(&ippkt, &iphdr));
  /* For IPv6, some extensions are not fragmented, while others are.
     The unfragmented extensions are HOPOPTS, DSTOPTS(1) and ROUTING.
     So, iphdr.header.ip6.exts and ippkt are eventually adjusted. */
  if (iphdr.iptype == NETWIB_IPTYPE_IP6) {
    if (netwib__buf_ref_data_size(&iphdr.header.ip6.exts)) {
      netwib_er(netwib_priv_ip6exts_skip_notfrag(iphdr.protocol,
                                                 &iphdr.header.ip6.exts,
                                                 &skipsize));
      shiftsize = netwib__buf_ref_data_size(&iphdr.header.ip6.exts) - skipsize;
      if (shiftsize) {
        iphdr.header.ip6.exts.endoffset -= shiftsize;
        ippkt.beginoffset -= shiftsize;
      }
    }
  }

  /* special case where fragmentation is not needed */
  if (fragdatasize > netwib__buf_ref_data_size(&ippkt)) {
    if (fragtotalsize == 0) {
      netwib_er((*pfunc)(pippkt, infos));
      return(NETWIB_ERR_OK);
    }
    fragdatasize = 0;
  }

  netwib_er(netwib_ip_frag_ip(pippkt, &iphdr, &ippkt, fragdatasize,
                              fragtotalsize, pfunc, infos));

  return(NETWIB_ERR_OK);
}
