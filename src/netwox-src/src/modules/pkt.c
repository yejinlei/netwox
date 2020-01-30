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
netwib_err netwox_pkt_iptype_init_ippkt(netwib_constbuf *pippkt,
                                        netwib_iptype *piptype)
{
  netwib_data data;
  netwib_uint32 datasize;

  datasize = netwib__buf_ref_data_size(pippkt);
  if (datasize == 0) {
    *piptype = NETWIB_IPTYPE_UNKNOWN;
    return(NETWIB_ERR_OK);
  }
  data = netwib__buf_ref_data_ptr(pippkt);

  switch(data[0] >> 4) {
  case 4 :
    *piptype = NETWIB_IPTYPE_IP4;
    break;
  case 6 :
    *piptype = NETWIB_IPTYPE_IP6;
    break;
  default :
    *piptype = NETWIB_IPTYPE_UNKNOWN;
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_iphdr_isfrag(netwib_constiphdr *piphdr,
                               netwib_bool *pisfrag)
{
  netwib_uint32 skipsize;
  netwib_ipproto pktproto;
  netwib_ip6ext ip6ext;
  netwib_buf pkt;
  netwib_err ret;

  *pisfrag = NETWIB_FALSE;

  ret = NETWIB_ERR_OK;
  switch(piphdr->iptype) {
  case NETWIB_IPTYPE_IP4 :
    if (piphdr->header.ip4.offsetfrag) {
      *pisfrag = NETWIB_TRUE;
    }
    break;
  case NETWIB_IPTYPE_IP6 :
    if (netwib__buf_ref_data_size(&piphdr->header.ip6.exts) != 0) {
      pktproto = piphdr->protocol;
      pkt = piphdr->header.ip6.exts;
      while (NETWIB_TRUE) {
        ret = netwib_pkt_decode_ip6ext(pktproto, &pkt, &ip6ext, &skipsize);
        if (ret != NETWIB_ERR_OK) {
          if (ret == NETWIB_ERR_NOTCONVERTED) ret = NETWIB_ERR_OK;
          if (ret == NETWIB_ERR_DATAMISSING) ret = NETWIB_ERR_OK;
          if (ret == NETWIB_ERR_LONOTIMPLEMENTED) ret = NETWIB_ERR_OK;
          break;
        }
        if (pktproto == NETWIB_IPPROTO_FRAGMENT) {
          if (ip6ext.ext.fragment.fragmentoffset) {
            *pisfrag = NETWIB_TRUE;
          }
          break;
        }
        pkt.beginoffset += skipsize;
      }
    }
    break;
  default :
    return(NETWIB_ERR_PAIPTYPE);
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ip6exts_decode_para(netwib_constbuf *pip6extsin,
                                      netwib_ipproto *pipproto,
                                      netwib_bufext *pip6extsout)
{
  netwib_data data;
  netwib_uint32 datasize;

  /* protocol is stored in first byte */
  datasize = netwib__buf_ref_data_size(pip6extsin);
  if (datasize == 0) {
    return(NETWIB_ERR_PATOOLOW);
  }
  data = netwib__buf_ref_data_ptr(pip6extsin);

  if (pipproto != NULL) *pipproto = data[0];
  netwib_er(netwib_buf_init_ext_arrayfilled(data+1, datasize-1, pip6extsout));

  return(NETWIB_ERR_OK);
}
