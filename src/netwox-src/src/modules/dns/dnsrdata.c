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
#include "../../netwox.h"

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_init_a(netwib_constip *paddress,
                                  netwox_dnsrr_type *ptype,
                                  netwib_buf *prdata)
{
  netwib_data data;

  switch(paddress->iptype) {
  case NETWIB_IPTYPE_IP4 :
    if (ptype != NULL) *ptype = NETWOX_DNSRR_TYPE_A;
    netwib_er(netwib_buf_wantspace(prdata, NETWIB_IP4_LEN, &data));
    netwib__data_append_uint32(data, paddress->ipvalue.ip4);
    prdata->endoffset += NETWIB_IP4_LEN;
    break;
  case NETWIB_IPTYPE_IP6 :
    if (ptype != NULL) *ptype = NETWOX_DNSRR_TYPE_AAAA;
    netwib_er(netwib_buf_wantspace(prdata, NETWIB_IP6_LEN, &data));
    netwib_c_memcpy(data, paddress->ipvalue.ip6.b, NETWIB_IP6_LEN);
    prdata->endoffset += NETWIB_IP6_LEN;
    break;
  default :
    return(NETWIB_ERR_PAIPTYPE);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_decode_a(netwox_dnsrr_type type,
                                    netwib_constbuf *prdata,
                                    netwib_ip *paddress)
{
  netwib_data data;
  netwib_uint32 datasize;

  data = netwib__buf_ref_data_ptr(prdata);
  datasize = netwib__buf_ref_data_size(prdata);
  switch(type) {
  case NETWOX_DNSRR_TYPE_A :
    if (datasize != NETWIB_IP4_LEN) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (paddress != NULL) {
      paddress->iptype = NETWIB_IPTYPE_IP4;
      netwib__data_decode_uint32(data, paddress->ipvalue.ip4);
    }
    break;
  case NETWOX_DNSRR_TYPE_AAAA :
    if (datasize != NETWIB_IP6_LEN) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (paddress != NULL) {
      paddress->iptype = NETWIB_IPTYPE_IP6;
      netwib_c_memcpy(paddress->ipvalue.ip6.b, data, NETWIB_IP6_LEN);
    }
    break;
  default :
    return(NETWIB_ERR_NOTCONVERTED);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_init_a6(netwib_uint32 prefixlen,
                                   netwib_constip *paddress,
                                   netwib_constbuf *pprefixname,
                                   netwib_buf *prdata)
{
  netwib_data data;
  netwib_uint32 i, starti;

  if (prefixlen > 128) {
    return(NETWIB_ERR_PATOOHIGH);
  }
  if (paddress->iptype != NETWIB_IPTYPE_IP6) {
    return(NETWIB_ERR_PAIPTYPENOT6);
  }

  /* write prefix length */
  netwib_er(netwib_buf_wantspace(prdata, 17, &data));
  *data++ = (netwib_byte)prefixlen;
  prdata->endoffset++;

  /* write ip address (address suffix) */
  if (prefixlen != 128) {
    starti = prefixlen / 8;
    *data = paddress->ipvalue.ip6.b[starti];
    if (prefixlen % 8) {
      *data &= (1 << (8 - (prefixlen % 8))) - 1;
    }
    data++;
    for (i = starti+1; i < 16; i++) {
      *data++ = paddress->ipvalue.ip6.b[i];
    }
    prdata->endoffset += 16 - starti;
  }

  /* write name without compression (as described in rfc) */
  if (prefixlen != 0) {
    netwib_er(netwox_dnsfield_host_initcomp(NULL, pprefixname, prdata));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_decode_a6(netwib_constbuf *prdata,
                                     netwib_uint32 *pprefixlen,
                                     netwib_ip *paddress,
                                     netwib_buf *pprefixname)
{
  netwib_data data;
  netwib_uint32 datasize, prefixlen, starti, i;
  netwib_buf rdata;

  data = netwib__buf_ref_data_ptr(prdata);
  datasize = netwib__buf_ref_data_size(prdata);
  if (datasize == 0) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  /* prefix length */
  prefixlen = *data++;
  if (pprefixlen != NULL) *pprefixlen = prefixlen;
  datasize--;

  /* IP address */
  if (prefixlen != 128) {
    starti = prefixlen / 8;
    if (datasize < 16 - starti) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (paddress != NULL) {
      paddress->iptype = NETWIB_IPTYPE_IP6;
      for (i = 0; i < starti; i++) {
        paddress->ipvalue.ip6.b[i] = 0;
      }
      for (i = starti; i < 16; i++) {
        paddress->ipvalue.ip6.b[i] = *data++;
      }
    }
    datasize -= 16 - starti;
  }

  /* name without compression (as described in rfc) */
  if (prefixlen != 0) {
    rdata = *prdata;
    rdata.beginoffset += netwib__buf_ref_data_size(prdata) - datasize;
    netwib_er(netwox_dnsfield_host_uncdecode(NULL, &rdata, pprefixname));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_initcomp_ns(netwib_constbuf *pfullpkt,
                                       netwib_constbuf *pnsdname,
                                       netwib_buf *prdata)
{
  netwib_er(netwox_dnsfield_host_initcomp(pfullpkt, pnsdname, prdata));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_uncdecode_ns(netwib_constbuf *pfullpkt,
                                        netwib_constbuf *prdata,
                                        netwib_buf *pnsdname)
{
  netwib_buf rdata;

  rdata = *prdata;
  netwib_er(netwox_dnsfield_host_uncdecode(pfullpkt, &rdata, pnsdname));

  if (netwib__buf_ref_data_size(&rdata)) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_initcomp_cname(netwib_constbuf *pfullpkt,
                                          netwib_constbuf *pcname,
                                          netwib_buf *prdata)
{
  netwib_er(netwox_dnsfield_host_initcomp(pfullpkt, pcname, prdata));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_uncdecode_cname(netwib_constbuf *pfullpkt,
                                           netwib_constbuf *prdata,
                                           netwib_buf *pcname)
{
  netwib_buf rdata;

  rdata = *prdata;
  netwib_er(netwox_dnsfield_host_uncdecode(pfullpkt, &rdata, pcname));

  if (netwib__buf_ref_data_size(&rdata)) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_initcomp_soa(netwib_constbuf *pfullpkt,
                                        netwib_constbuf *pdomainname,
                                        netwib_constbuf *pemail,
                                        netwib_uint32 serial,
                                        netwib_uint32 refresh,
                                        netwib_uint32 retry,
                                        netwib_uint32 expire,
                                        netwib_uint32 minimum,
                                        netwib_buf *prdata)
{
  netwib_data data;

  netwib_er(netwox_dnsfield_host_initcomp(pfullpkt, pdomainname, prdata));
  netwib_er(netwox_dnsfield_email_initcomp(pfullpkt, pemail, prdata));

  netwib_er(netwib_buf_wantspace(prdata, 20, &data));
  netwib__data_append_uint32(data, serial);
  netwib__data_append_uint32(data, refresh);
  netwib__data_append_uint32(data, retry);
  netwib__data_append_uint32(data, expire);
  netwib__data_append_uint32(data, minimum);
  prdata->endoffset += 20;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_uncdecode_soa(netwib_constbuf *pfullpkt,
                                         netwib_constbuf *prdata,
                                         netwib_buf *pdomainname,
                                         netwib_buf *pemail,
                                         netwib_uint32 *pserial,
                                         netwib_uint32 *prefresh,
                                         netwib_uint32 *pretry,
                                         netwib_uint32 *pexpire,
                                         netwib_uint32 *pminimum)
{
  netwib_buf rdata;
  netwib_data data;
  netwib_uint32 datasize;

  rdata = *prdata;
  netwib_er(netwox_dnsfield_host_uncdecode(pfullpkt, &rdata, pdomainname));
  netwib_er(netwox_dnsfield_email_uncdecode(pfullpkt, &rdata, pemail));

  data = netwib__buf_ref_data_ptr(&rdata);
  datasize = netwib__buf_ref_data_size(&rdata);
  if (datasize != 20) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  if (pserial != NULL) {
    *pserial = netwib_c2_uint32_4(data[0], data[1], data[2], data[3]);
  }
  if (prefresh != NULL) {
    *prefresh = netwib_c2_uint32_4(data[4], data[5], data[6], data[7]);
  }
  if (pretry != NULL) {
    *pretry = netwib_c2_uint32_4(data[8], data[9], data[10], data[11]);
  }
  if (pexpire != NULL) {
    *pexpire = netwib_c2_uint32_4(data[12], data[13], data[14], data[15]);
  }
  if (pminimum != NULL) {
    *pminimum = netwib_c2_uint32_4(data[16], data[17], data[18], data[19]);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_initcomp_ptr(netwib_constbuf *pfullpkt,
                                        netwib_constbuf *pptrdname,
                                        netwib_buf *prdata)
{
  netwib_er(netwox_dnsfield_host_initcomp(pfullpkt, pptrdname, prdata));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_uncdecode_ptr(netwib_constbuf *pfullpkt,
                                         netwib_constbuf *prdata,
                                         netwib_buf *pptrdname)
{
  netwib_buf rdata;

  rdata = *prdata;
  netwib_er(netwox_dnsfield_host_uncdecode(pfullpkt, &rdata, pptrdname));

  if (netwib__buf_ref_data_size(&rdata)) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_init_hinfo(netwib_constbuf *pcpu,
                                      netwib_constbuf *pos,
                                      netwib_buf *prdata)
{
  netwib_er(netwox_dnsfield_text_init(pcpu, prdata));
  netwib_er(netwox_dnsfield_text_init(pos, prdata));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_decode_hinfo(netwib_constbuf *prdata,
                                        netwib_buf *pcpu,
                                        netwib_buf *pos)
{
  netwib_buf rdata;

  rdata = *prdata;
  netwib_er(netwox_dnsfield_text_decode(&rdata, pcpu));
  netwib_er(netwox_dnsfield_text_decode(&rdata, pos));

  if (netwib__buf_ref_data_size(&rdata)) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_initcomp_mx(netwib_constbuf *pfullpkt,
                                       netwib_uint16 preference,
                                       netwib_constbuf *pexchange,
                                       netwib_buf *prdata)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(prdata, 2, &data));
  netwib__data_append_uint16(data, preference);
  prdata->endoffset += 2;

  netwib_er(netwox_dnsfield_host_initcomp(pfullpkt, pexchange, prdata));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_uncdecode_mx(netwib_constbuf *pfullpkt,
                                        netwib_constbuf *prdata,
                                        netwib_uint16 *ppreference,
                                        netwib_buf *pexchange)
{
  netwib_buf rdata;
  netwib_data data;
  netwib_uint32 datasize;

  rdata = *prdata;

  datasize = netwib__buf_ref_data_size(&rdata);
  if (datasize < 2) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  data = netwib__buf_ref_data_ptr(&rdata);
  if (ppreference != NULL) {
    netwib__data_decode_uint16(data, *ppreference);
  }
  rdata.beginoffset += 2;

  netwib_er(netwox_dnsfield_host_uncdecode(pfullpkt, &rdata, pexchange));

  if (netwib__buf_ref_data_size(&rdata)) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_init_txt(netwib_constbuf *ptext,
                                    netwib_buf *prdata)
{
  netwib_er(netwox_dnsfield_text_init(ptext, prdata));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_decode_txt(netwib_constbuf *prdata,
                                      netwib_buf *ptext)
{
  netwib_buf rdata;

  rdata = *prdata;
  netwib_er(netwox_dnsfield_text_decode(&rdata, ptext));

  if (netwib__buf_ref_data_size(&rdata)) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_init_pkt(netwib_bufpool *pbufpool,
                                    netwox_dnsrr_type type,
                                    netwib_constbuf *pfullpkt,
                                    netwib_constbuf *ppkt,
                                    netwib_buf *prdata)
{
  netwib_buf *pbuf1, *pbuf2;
  netwib_uint32 ui1, ui2, ui3, ui4, ui5;
  netwib_uint16 ui16;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf1));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf2));

  switch(type) {
    case NETWOX_DNSRR_TYPE_NS :
      ret = netwox_dnsrdata_uncdecode_ns(pfullpkt, ppkt, pbuf1);
      if (ret == NETWIB_ERR_OK) {
        ret = netwox_dnsrdata_init_ns(pbuf1, prdata);
      }
      break;
    case NETWOX_DNSRR_TYPE_CNAME :
      ret = netwox_dnsrdata_uncdecode_cname(pfullpkt, ppkt, pbuf1);
      if (ret == NETWIB_ERR_OK) {
        ret = netwox_dnsrdata_init_cname(pbuf1, prdata);
      }
      break;
    case NETWOX_DNSRR_TYPE_SOA :
      ret = netwox_dnsrdata_uncdecode_soa(pfullpkt, ppkt, pbuf1, pbuf2, &ui1,
                                          &ui2, &ui3, &ui4, &ui5);
      if (ret == NETWIB_ERR_OK) {
        ret = netwox_dnsrdata_init_soa(pbuf1, pbuf2, ui1,
                                       ui2, ui3, ui4, ui5, prdata);
      }
      break;
    case NETWOX_DNSRR_TYPE_PTR :
      ret = netwox_dnsrdata_uncdecode_ptr(pfullpkt, ppkt, pbuf1);
      if (ret == NETWIB_ERR_OK) {
        ret = netwox_dnsrdata_init_ptr(pbuf1, prdata);
      }
      break;
    case NETWOX_DNSRR_TYPE_MX :
      ret = netwox_dnsrdata_uncdecode_mx(pfullpkt, ppkt, &ui16, pbuf1);
      if (ret == NETWIB_ERR_OK) {
        ret = netwox_dnsrdata_init_mx(ui16, pbuf1, prdata);
      }
      break;
    default :
      /* an unrecognized type or a type not needing compression */
      ret = netwib_buf_append_buf(ppkt, prdata);
      break;
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf2));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf1));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_decode_pkt(netwib_bufpool *pbufpool,
                                      netwox_dnsrr_type type,
                                      netwib_constbuf *pfullpkt,
                                      netwib_constbuf *prdata,
                                      netwib_buf *ppkt)
{
  netwib_buf *pbuf1, *pbuf2;
  netwib_uint32 ui1, ui2, ui3, ui4, ui5;
  netwib_uint16 ui16;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf1));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf2));

  switch(type) {
    case NETWOX_DNSRR_TYPE_NS :
      ret = netwox_dnsrdata_decode_ns(prdata, pbuf1);
      if (ret == NETWIB_ERR_OK) {
        ret = netwox_dnsrdata_initcomp_ns(pfullpkt, pbuf1, ppkt);
      }
      break;
    case NETWOX_DNSRR_TYPE_CNAME :
      ret = netwox_dnsrdata_decode_cname(prdata, pbuf1);
      if (ret == NETWIB_ERR_OK) {
        ret = netwox_dnsrdata_initcomp_cname(pfullpkt, pbuf1, ppkt);
      }
      break;
    case NETWOX_DNSRR_TYPE_SOA :
      ret = netwox_dnsrdata_decode_soa(prdata, pbuf1, pbuf2, &ui1,
                                       &ui2, &ui3, &ui4, &ui5);
      if (ret == NETWIB_ERR_OK) {
        ret = netwox_dnsrdata_initcomp_soa(pfullpkt, pbuf1, pbuf2, ui1,
                                           ui2, ui3, ui4, ui5, ppkt);
      }
      break;
    case NETWOX_DNSRR_TYPE_PTR :
      ret = netwox_dnsrdata_decode_ptr(prdata, pbuf1);
      if (ret == NETWIB_ERR_OK) {
        ret = netwox_dnsrdata_initcomp_ptr(pfullpkt, pbuf1, ppkt);
      }
      break;
    case NETWOX_DNSRR_TYPE_MX :
      ret = netwox_dnsrdata_decode_mx(prdata, &ui16, pbuf1);
      if (ret == NETWIB_ERR_OK) {
        ret = netwox_dnsrdata_initcomp_mx(pfullpkt, ui16, pbuf1, ppkt);
      }
      break;
    default :
      /* an unrecognized type or a type not needing compression */
      ret = netwib_buf_append_buf(prdata, ppkt);
      break;
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf2));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf1));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_buf_append_dnsrdata(netwib_bufpool *pbufpool,
                                      netwox_dnsrr_type type,
                                      netwib_constbuf *prdata,
                                      netwib_buf *pbuf)
{
  netwib_buf *pbuf1, *pbuf2;
  netwib_uint32 ui1, ui2, ui3, ui4, ui5;
  netwib_uint16 ui16;
  netwib_ip ipad;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf1));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf2));

  switch(type) {
    case NETWOX_DNSRR_TYPE_A :
    case NETWOX_DNSRR_TYPE_AAAA :
      ret = netwox_dnsrdata_decode_a(type, prdata, &ipad);
      if (ret == NETWIB_ERR_OK) {
        ret = netwib_buf_append_ip(&ipad, NETWIB_IP_ENCODETYPE_IP, pbuf);
      }
      break;
    case NETWOX_DNSRR_TYPE_A6 :
      ret = netwox_dnsrdata_decode_a6(prdata, &ui1, &ipad, pbuf1);
      if (ret == NETWIB_ERR_OK) {
        ret = netwib_buf_append_fmt(pbuf, "%{uint32} %{ip} %{buf}", ui1,
                                    &ipad, pbuf1);
      }
      break;
    case NETWOX_DNSRR_TYPE_NS :
      ret = netwox_dnsrdata_decode_ns(prdata, pbuf1);
      if (ret == NETWIB_ERR_OK) {
        ret = netwib_buf_append_buf(pbuf1, pbuf);
      }
      break;
    case NETWOX_DNSRR_TYPE_CNAME :
      ret = netwox_dnsrdata_decode_cname(prdata, pbuf1);
      if (ret == NETWIB_ERR_OK) {
        ret = netwib_buf_append_buf(pbuf1, pbuf);
      }
      break;
    case NETWOX_DNSRR_TYPE_SOA :
      ret = netwox_dnsrdata_decode_soa(prdata, pbuf1, pbuf2, &ui1,
                                       &ui2, &ui3, &ui4, &ui5);
      if (ret == NETWIB_ERR_OK) {
        ret = netwib_buf_append_fmt(pbuf, "%{buf} ...", pbuf1);
      }
      break;
    case NETWOX_DNSRR_TYPE_PTR :
      ret = netwox_dnsrdata_decode_ptr(prdata, pbuf1);
      if (ret == NETWIB_ERR_OK) {
        ret = netwib_buf_append_buf(pbuf1, pbuf);
      }
      break;
    case NETWOX_DNSRR_TYPE_HINFO :
      ret = netwox_dnsrdata_decode_hinfo(prdata, pbuf1, pbuf2);
      if (ret == NETWIB_ERR_OK) {
        ret = netwib_buf_append_fmt(pbuf, "%{buf} %{buf}", pbuf1, pbuf2);
      }
      break;
    case NETWOX_DNSRR_TYPE_MX :
      ret = netwox_dnsrdata_decode_mx(prdata, &ui16, pbuf1);
      if (ret == NETWIB_ERR_OK) {
        ret = netwib_buf_append_fmt(pbuf, "%{uint16} %{buf}", ui16,
                                    pbuf1);
      }
      break;
    case NETWOX_DNSRR_TYPE_TXT :
      ret = netwox_dnsrdata_decode_txt(prdata, pbuf1);
      if (ret == NETWIB_ERR_OK) {
        ret = netwib_buf_append_buf(pbuf1, pbuf);
      }
      break;
    default :
      ret = netwib_buf_encode(prdata, NETWIB_ENCODETYPE_MIXED, pbuf);
      break;
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf2));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf1));

  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_buf_encode(prdata, NETWIB_ENCODETYPE_MIXED, pbuf));
  }

  return(NETWIB_ERR_OK);
}
