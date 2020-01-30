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
netwib_err netwib_ip6exts_show(netwib_ipproto pktproto,
                               netwib_constbuf *ppkt,
                               netwib_encodetype encodetype,
                               netwib_buf *pbuf)
{
  netwib_buf pkt;
  netwib_ip6ext ip6ext;
  netwib_uint32 skipsize;
  netwib_err ret;

  switch(encodetype) {
    case NETWIB_ENCODETYPE_SYNTH :
      netwib_er(netwib_buf_append_string("ip6exts", pbuf));
      break;
    case NETWIB_ENCODETYPE_ARRAY :
      pkt = *ppkt;
      while (NETWIB_TRUE) {
        if (pkt.beginoffset >= pkt.endoffset) {
          break;
        }
        ret = netwib_pkt_decode_ip6ext(pktproto, &pkt, &ip6ext, &skipsize);
        if (ret == NETWIB_ERR_OK) {
          netwib_er(netwib_ip6ext_show(&ip6ext, encodetype, pbuf));
        } else if (ret == NETWIB_ERR_LONOTIMPLEMENTED) {
          netwib_er(netwib_show_array_head("IP6 Extension", pbuf));
          netwib_er(netwib_show_array_fmt32(pbuf,
                                            " unknown extension (%{uint32})",
                                            pktproto));
          netwib_er(netwib_show_array_line_end(pbuf));
        } else {
          return(ret);
        }
        pkt.beginoffset += skipsize;
        pktproto = ip6ext.nextproto;
      }
      break;
    default:
      netwib_er(netwib_buf_encode(ppkt, encodetype, pbuf));
      return(NETWIB_ERR_OK);
  }

  return(NETWIB_ERR_OK);
}
