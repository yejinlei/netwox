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
static netwib_err netwib_priv_icmp6nd_show(netwib_consticmp6nd *picmp6nd,
                                           netwib_buf *pbuf)
{

  switch(picmp6nd->type) {
  case NETWIB_ICMP6NDTYPE_SRCLINK :
    netwib_er(netwib_show_array_fmt32(pbuf, " src link : linkad=%{eth}", &picmp6nd->opt.link.linkad));
    break;
  case NETWIB_ICMP6NDTYPE_DSTLINK :
    netwib_er(netwib_show_array_fmt32(pbuf, " dst link : linkad=%{eth}", &picmp6nd->opt.link.linkad));
    break;
  case NETWIB_ICMP6NDTYPE_PREFIX :
    netwib_er(netwib_show_array_fmt32(pbuf, " prefix : prefixlength=%{uint8} onlink=%{bool} autonomous=%{bool}", picmp6nd->opt.prefix.prefixlength, picmp6nd->opt.prefix.onlink, picmp6nd->opt.prefix.autonomous));
    netwib_er(netwib_show_array_fmt32(pbuf, "          reserved1=%{uint8} reserved2=%{uint32}", picmp6nd->opt.prefix.reserved1, picmp6nd->opt.prefix.reserved2));
    netwib_er(netwib_show_array_fmt32(pbuf, "          validlifetime=%{uint32} preferredlifetime=%{uint32}", picmp6nd->opt.prefix.validlifetime, picmp6nd->opt.prefix.preferredlifetime));
    netwib_er(netwib_show_array_fmt32(pbuf, "          prefix=%{ip}", &picmp6nd->opt.prefix.prefix));
    break;
  case NETWIB_ICMP6NDTYPE_REDIR :
    netwib_er(netwib_show_array_fmt32(pbuf, " redir : reserved1=%{uint16} reserved2=%{uint32}", picmp6nd->opt.redir.reserved1, picmp6nd->opt.redir.reserved2));
    netwib_er(netwib_show_array_fmt32(pbuf, "         bad IP packet :"));
    netwib_er(netwib_pkt_ip_show(&picmp6nd->opt.redir.badippacket,
                                 NULL, NETWIB_ENCODETYPE_ARRAY,
                                 NETWIB_ENCODETYPE_DUMP, pbuf));
    break;
  case NETWIB_ICMP6NDTYPE_MTU :
    netwib_er(netwib_show_array_fmt32(pbuf, " mtu : reserved=%{uint16} mtu=%{uint32}", picmp6nd->opt.mtu.reserved, picmp6nd->opt.mtu.mtu));
    break;
  default :
    return(NETWIB_ERR_LONOTIMPLEMENTED);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_icmp6nd_show(netwib_consticmp6nd *picmp6nd,
                               netwib_encodetype encodetype,
                               netwib_buf *pbuf)
{
  netwib_buf buf;

  switch(encodetype) {
    case NETWIB_ENCODETYPE_SYNTH :
      netwib_er(netwib_buf_append_fmt(pbuf, "icmp6nd:%{uint32}",
                                      picmp6nd->type));
      break;
    case NETWIB_ENCODETYPE_ARRAY :
      netwib_er(netwib_priv_icmp6nd_show(picmp6nd, pbuf));
      break;
    default :
      netwib_er(netwib_buf_init_mallocdefault(&buf));
      netwib_er(netwib_pkt_append_icmp6nd(picmp6nd, &buf));
      netwib_er(netwib_buf_encode(&buf, encodetype, pbuf));
      netwib_er(netwib_buf_close(&buf));
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_icmp6nds_show(netwib_constbuf *ppkt,
                                netwib_encodetype encodetype,
                                netwib_buf *pbuf)
{
  netwib_buf pkt, badopt;
  netwib_icmp6nd icmp6nd;
  netwib_uint32 skipsize;
  netwib_err ret;

  switch(encodetype) {
    case NETWIB_ENCODETYPE_SYNTH :
      netwib_er(netwib_buf_append_string("icmp6nds", pbuf));
      break;
    case NETWIB_ENCODETYPE_ARRAY :
      netwib_er(netwib_show_array_head("ICMP6NDS", pbuf));
      pkt = *ppkt;
      while (NETWIB_TRUE) {
        if (pkt.beginoffset >= pkt.endoffset) {
          break;
        }
        ret = netwib_pkt_decode_icmp6nd(&pkt, &icmp6nd, &skipsize);
        if (ret == NETWIB_ERR_OK) {
          netwib_er(netwib_icmp6nd_show(&icmp6nd, encodetype, pbuf));
        } else if (ret == NETWIB_ERR_NOTCONVERTED ||
                   ret == NETWIB_ERR_DATAMISSING ||
                   ret == NETWIB_ERR_LONOTIMPLEMENTED) {
          badopt = pkt;
          badopt.endoffset = badopt.beginoffset + skipsize;
          netwib_er(netwib_show_array_data(" undecoded option", &badopt,
                                           NETWIB_ENCODETYPE_HEXA0, ' ',
                                           pbuf));
        } else {
          return(ret);
        }
        pkt.beginoffset += skipsize;
      }
      netwib_er(netwib_show_array_tail(pbuf));
      break;
    default:
      netwib_er(netwib_buf_encode(ppkt, encodetype, pbuf));
      return(NETWIB_ERR_OK);
  }

  return(NETWIB_ERR_OK);
}
