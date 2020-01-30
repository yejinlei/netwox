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
netwib_err netwib_priv_ippkt_decode_iptype(netwib_constbuf *ppkt,
                                           netwib_iptype *piptype)
{
  netwib_data data;
  netwib_uint32 datasize, version;

  datasize = netwib__buf_ref_data_size(ppkt);
  if (datasize == 0) {
    return(NETWIB_ERR_DATAMISSING);
  }
  data = netwib__buf_ref_data_ptr(ppkt);

  version = data[0] >> 4;

  switch(version) {
    case 4 :
      if (piptype != NULL) *piptype = NETWIB_IPTYPE_IP4;
      break;
    case 6 :
      if (piptype != NULL) *piptype = NETWIB_IPTYPE_IP6;
      break;
    default :
      return(NETWIB_ERR_NOTCONVERTED);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ippkt_decode_srcdst(netwib_constbuf *ppkt,
                                           netwib_ip *psrc,
                                           netwib_ip *pdst)
{
  netwib_data data;
  netwib_uint32 datasize, version;

  datasize = netwib__buf_ref_data_size(ppkt);
  if (datasize == 0) {
    return(NETWIB_ERR_DATAMISSING);
  }
  data = netwib__buf_ref_data_ptr(ppkt);

  version = data[0] >> 4;

  switch(version) {
    case 4 :
      if (datasize < NETWIB_PRIV_IP4HDR_MINLEN) {
        return(NETWIB_ERR_DATAMISSING);
      }
      psrc->iptype = NETWIB_IPTYPE_IP4;
      psrc->ipvalue.ip4 = netwib_c2_uint32_4(data[12], data[13],
                                             data[14], data[15]);
      pdst->iptype = NETWIB_IPTYPE_IP4;
      pdst->ipvalue.ip4 = netwib_c2_uint32_4(data[16], data[17],
                                             data[18], data[19]);
      break;
    case 6 :
      if (datasize < NETWIB_PRIV_IP6HDR_MINLEN) {
        return(NETWIB_ERR_DATAMISSING);
      }
      psrc->iptype = NETWIB_IPTYPE_IP6;
      netwib_c_memcpy(psrc->ipvalue.ip6.b, data+8, NETWIB_IP6_LEN);
      pdst->iptype = NETWIB_IPTYPE_IP6;
      netwib_c_memcpy(pdst->ipvalue.ip6.b, data+24, NETWIB_IP6_LEN);
      break;
    default :
      return(NETWIB_ERR_NOTCONVERTED);
  }

  return(NETWIB_ERR_OK);
}
