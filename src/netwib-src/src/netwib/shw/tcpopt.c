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
netwib_err netwib_tcpopt_show(netwib_consttcpopt *ptcpopt,
                              netwib_encodetype encodetype,
                              netwib_buf *pbuf)
{
  netwib_buf buf;
  netwib_uint32 i;

  if (encodetype == NETWIB_ENCODETYPE_SYNTH) {
    switch(ptcpopt->type) {
    case NETWIB_TCPOPTTYPE_END :
      netwib_er(netwib_buf_append_string("end", pbuf));
      break;
    case NETWIB_TCPOPTTYPE_NOOP :
      netwib_er(netwib_buf_append_string("noop", pbuf));
      break;
    case NETWIB_TCPOPTTYPE_MSS :
      netwib_er(netwib_buf_append_string("mss", pbuf));
      break;
    case NETWIB_TCPOPTTYPE_WINDOWSCALE :
      netwib_er(netwib_buf_append_string("windowscale", pbuf));
      break;
    case NETWIB_TCPOPTTYPE_SACKPERMITTED :
      netwib_er(netwib_buf_append_string("sackpermitted", pbuf));
      break;
    case NETWIB_TCPOPTTYPE_SACK :
      netwib_er(netwib_buf_append_string("sack", pbuf));
      break;
    case NETWIB_TCPOPTTYPE_ECHOREQUEST :
      netwib_er(netwib_buf_append_string("echorequest", pbuf));
      break;
    case NETWIB_TCPOPTTYPE_ECHOREPLY :
      netwib_er(netwib_buf_append_string("echoreply", pbuf));
      break;
    case NETWIB_TCPOPTTYPE_TIMESTAMP :
      netwib_er(netwib_buf_append_string("timestamp", pbuf));
      break;
    case NETWIB_TCPOPTTYPE_CC :
      netwib_er(netwib_buf_append_string("cc", pbuf));
      break;
    case NETWIB_TCPOPTTYPE_CCNEW :
      netwib_er(netwib_buf_append_string("ccnew", pbuf));
      break;
    case NETWIB_TCPOPTTYPE_CCECHO :
      netwib_er(netwib_buf_append_string("ccecho", pbuf));
      break;
    default :
      return(NETWIB_ERR_LONOTIMPLEMENTED);
      break;
    }
  } else if (encodetype == NETWIB_ENCODETYPE_ARRAY) {
    switch(ptcpopt->type) {
    case NETWIB_TCPOPTTYPE_END :
      netwib_er(netwib_show_array_fmt32(pbuf, " end"));
      break;
    case NETWIB_TCPOPTTYPE_NOOP :
      netwib_er(netwib_show_array_fmt32(pbuf, " noop"));
      break;
    case NETWIB_TCPOPTTYPE_MSS :
      netwib_er(netwib_show_array_fmt32(pbuf, " mss=%{uint16}", ptcpopt->opt.mss.maxsegsize));
      break;
    case NETWIB_TCPOPTTYPE_WINDOWSCALE :
      netwib_er(netwib_show_array_fmt32(pbuf, " windowscale=%{uint8}", ptcpopt->opt.windowscale.windowscale));
      break;
    case NETWIB_TCPOPTTYPE_SACKPERMITTED :
      netwib_er(netwib_show_array_fmt32(pbuf, " sackpermitted"));
      break;
    case NETWIB_TCPOPTTYPE_SACK :
      netwib_er(netwib_show_array_fmt32(pbuf, " sack (storedvalues=%{uint32}) :", ptcpopt->opt.sack.storedvalues));
      for (i = 0; i < ptcpopt->opt.sack.storedvalues; i++) {
        netwib_er(netwib_show_array_fmt32(pbuf, "   %{uint32} -> %{uint32}",
                                          ptcpopt->opt.sack.leftedge[i],
                                          ptcpopt->opt.sack.rightedge[i]));
      }
      break;
    case NETWIB_TCPOPTTYPE_ECHOREQUEST :
      netwib_er(netwib_show_array_fmt32(pbuf, " echorequest : data=%{uint32}", ptcpopt->opt.echo.data));
      break;
    case NETWIB_TCPOPTTYPE_ECHOREPLY :
      netwib_er(netwib_show_array_fmt32(pbuf, " echoreply : data=%{uint32}", ptcpopt->opt.echo.data));
      break;
    case NETWIB_TCPOPTTYPE_TIMESTAMP :
      netwib_er(netwib_show_array_fmt32(pbuf, " timestamp : val=%{uint32} echoreply=%{uint32}", ptcpopt->opt.timestamp.val, ptcpopt->opt.timestamp.echoreply));
      break;
    case NETWIB_TCPOPTTYPE_CC :
      netwib_er(netwib_show_array_fmt32(pbuf, " cc : connectioncount=%{uint32}", ptcpopt->opt.cc.connectioncount));
      break;
    case NETWIB_TCPOPTTYPE_CCNEW :
      netwib_er(netwib_show_array_fmt32(pbuf, " ccnew : connectioncount=%{uint32}", ptcpopt->opt.cc.connectioncount));
      break;
    case NETWIB_TCPOPTTYPE_CCECHO :
      netwib_er(netwib_show_array_fmt32(pbuf, " ccecho : connectioncount=%{uint32}", ptcpopt->opt.cc.connectioncount));
      break;
    default :
      return(NETWIB_ERR_LONOTIMPLEMENTED);
      break;
    }
  } else {
    netwib_er(netwib_buf_init_mallocdefault(&buf));
    netwib_er(netwib_pkt_append_tcpopt(ptcpopt, &buf));
    netwib_er(netwib_buf_encode(&buf, encodetype, pbuf));
    netwib_er(netwib_buf_close(&buf));
  }

  return(NETWIB_ERR_OK);
}
