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
netwib_err netwib_udphdr_initdefault(netwib_udphdr *pudphdr)
{

  if (pudphdr != NULL) {
    pudphdr->src = 0;
    pudphdr->dst = 0;
    pudphdr->len = NETWIB_UDPHDR_LEN;
    pudphdr->check = 0;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_udphdr(netwib_constudphdr *pudphdr,
                                    netwib_buf *ppkt)
{
  netwib_data data;

  if (pudphdr->src > 0xFFFF) {
    return(NETWIB_ERR_PATOOBIGFORHDR);
  }
  if (pudphdr->dst > 0xFFFF) {
    return(NETWIB_ERR_PATOOBIGFORHDR);
  }

  netwib_er(netwib_buf_wantspace(ppkt, NETWIB_UDPHDR_LEN, &data));

  netwib__data_append_uint16(data, pudphdr->src);
  netwib__data_append_uint16(data, pudphdr->dst);
  netwib__data_append_uint16(data, pudphdr->len);
  netwib__data_append_uint16(data, pudphdr->check);

  ppkt->endoffset += NETWIB_UDPHDR_LEN;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_prepend_udphdr(netwib_constudphdr *pudphdr,
                                     netwib_buf *ppkt)
{
  netwib_byte array[NETWIB_UDPHDR_LEN];
  netwib_buf buf;

  netwib_er(netwib_buf_init_ext_arrayempty(array, NETWIB_UDPHDR_LEN, &buf));
  netwib_er(netwib_pkt_append_udphdr(pudphdr, &buf));
  netwib_er(netwib_buf_prepend_buf(&buf, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_decode_udphdr(netwib_constbuf *ppkt,
                                    netwib_udphdr *pudphdr,
                                    netwib_uint32 *pskipsize)
{
  netwib_data data;
  netwib_uint32 datasize;

  if (pskipsize != NULL) *pskipsize = NETWIB_UDPHDR_LEN;

  datasize = netwib__buf_ref_data_size(ppkt);
  if (datasize < NETWIB_UDPHDR_LEN) {
    return(NETWIB_ERR_DATAMISSING);
  }
  data = netwib__buf_ref_data_ptr(ppkt);

  if (pudphdr != NULL) {
    netwib__data_decode_uint16(data, pudphdr->src);
    netwib__data_decode_uint16(data, pudphdr->dst);
    netwib__data_decode_uint16(data, pudphdr->len);
    netwib__data_decode_uint16(data, pudphdr->check);
  }

  return(NETWIB_ERR_OK);
}
