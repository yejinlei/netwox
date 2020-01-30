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
netwib_err netwib_ip4opts_show(netwib_constbuf *ppkt,
                               netwib_encodetype encodetype,
                               netwib_buf *pbuf)
{
  netwib_buf pkt, badopt;
  netwib_ip4opt ip4opt;
  netwib_uint32 skipsize;
  netwib_err ret;

  switch(encodetype) {
    case NETWIB_ENCODETYPE_SYNTH :
      netwib_er(netwib_buf_append_string("ip4opts", pbuf));
      break;
    case NETWIB_ENCODETYPE_ARRAY :
      netwib_er(netwib_show_array_head("IP4OPTS", pbuf));
      pkt = *ppkt;
      while (NETWIB_TRUE) {
        if (pkt.beginoffset >= pkt.endoffset) {
          break;
        }
        ret = netwib_pkt_decode_ip4opt(&pkt, &ip4opt, &skipsize);
        if (ret == NETWIB_ERR_OK) {
          netwib_er(netwib_ip4opt_show(&ip4opt, encodetype, pbuf));
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
