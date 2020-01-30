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
netwib_err netwib_ip64bits_init_ippkt(netwib_constbuf *pippkt,
                                      netwib_bufext *pip64bits)
{
  netwib_data data;
  netwib_uint32 datasize, badlen;
  netwib_iptype iptype;
  netwib_iphdr ipheader;
  netwib_buf ippkt;
  netwib_err ret;

  datasize = netwib__buf_ref_data_size(pippkt);
  if (datasize == 0) {
    netwib_er(netwib_buf_init_ext_empty(pip64bits));
    return(NETWIB_ERR_OK);
  }
  data = netwib__buf_ref_data_ptr(pippkt);

  netwib_er(netwib_priv_ippkt_decode_iptype(pippkt, &iptype));

  /* for IP4, can be fast */
  if (iptype == NETWIB_IPTYPE_IP4) {
    badlen = 4*(data[0] & 0x0F) + 8;
    if (badlen > datasize) badlen = datasize;
    netwib_er(netwib_buf_init_ext_arrayfilled(data, badlen, pip64bits));
    return(NETWIB_ERR_OK);
  }

  /* for IP6, we have to decode the header */
  ippkt = *pippkt;
  ret = netwib_pkt_decode_layer_ip(&ippkt, &ipheader);
  if (ret != NETWIB_ERR_OK) {
    /* a size of 500 will be sufficient, because it is not valid, so less
       or more ... */
    badlen = (datasize > 500)?500:datasize;
    netwib_er(netwib_buf_init_ext_arrayfilled(data, badlen, pip64bits));
    return(NETWIB_ERR_OK);
  }
  badlen = datasize - netwib__buf_ref_data_size(&ippkt) + 8;
  if (badlen > datasize) badlen = datasize;
  netwib_er(netwib_buf_init_ext_arrayfilled(data, badlen, pip64bits));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ip64bits_decode_ipudp(netwib_constbuf *pip64bits,
                                        netwib_iphdr *piphdr,
                                        netwib_port *pudpsrc,
                                        netwib_port *pudpdst)
{
  netwib_buf pkt;
  netwib_iphdr iphdr, *piphdrtouse;
  netwib_ipproto ipproto;
  netwib_data data;
  netwib_uint32 datasize;

  pkt = *pip64bits;
  piphdrtouse = (piphdr==NULL)?&iphdr:piphdr;
  netwib_er(netwib_pkt_decode_layer_ip(&pkt, piphdrtouse));
  netwib_er(netwib_iphdr_get_proto(piphdrtouse, &ipproto));
  if (ipproto != NETWIB_IPPROTO_UDP) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  datasize = netwib__buf_ref_data_size(&pkt);
  if (datasize < 4) {
    return(NETWIB_ERR_DATAMISSING);
  }
  data = netwib__buf_ref_data_ptr(&pkt);

  if (pudpsrc != NULL) {
    *pudpsrc = netwib_c2_uint16_2(data[0], data[1]);
  }
  if (pudpdst != NULL) {
    *pudpdst = netwib_c2_uint16_2(data[2], data[3]);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ip64bits_decode_iptcp(netwib_constbuf *pip64bits,
                                        netwib_iphdr *piphdr,
                                        netwib_port *ptcpsrc,
                                        netwib_port *ptcpdst)
{
  netwib_buf pkt;
  netwib_iphdr iphdr, *piphdrtouse;
  netwib_ipproto ipproto;
  netwib_data data;
  netwib_uint32 datasize;

  pkt = *pip64bits;
  piphdrtouse = (piphdr==NULL)?&iphdr:piphdr;
  netwib_er(netwib_pkt_decode_layer_ip(&pkt, piphdrtouse));
  netwib_er(netwib_iphdr_get_proto(piphdrtouse, &ipproto));
  if (ipproto != NETWIB_IPPROTO_TCP) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  datasize = netwib__buf_ref_data_size(&pkt);
  if (datasize < 4) {
    return(NETWIB_ERR_DATAMISSING);
  }
  data = netwib__buf_ref_data_ptr(&pkt);

  if (ptcpsrc != NULL) {
    *ptcpsrc = netwib_c2_uint16_2(data[0], data[1]);
  }
  if (ptcpdst != NULL) {
    *ptcpdst = netwib_c2_uint16_2(data[2], data[3]);
  }

  return(NETWIB_ERR_OK);
}
