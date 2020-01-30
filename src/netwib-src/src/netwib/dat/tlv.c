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
static netwib_err netwib_priv_tlv_append_data(netwib_tlvtype type,
                                              netwib_constdata data,
                                              netwib_uint32 datasize,
                                              netwib_buf *ptlv)
{
  netwib_byte tl[8];
  netwib_err ret;

  /* append type length */
  tl[0] = netwib_c2_uint32_0(type);
  tl[1] = netwib_c2_uint32_1(type);
  tl[2] = netwib_c2_uint32_2(type);
  tl[3] = netwib_c2_uint32_3(type);
  tl[4] = netwib_c2_uint32_0(datasize);
  tl[5] = netwib_c2_uint32_1(datasize);
  tl[6] = netwib_c2_uint32_2(datasize);
  tl[7] = netwib_c2_uint32_3(datasize);
  netwib_er(netwib_buf_append_data(tl, 8, ptlv));
  netwib__localarray_ifbuf_wipe(ptlv, tl);

  /* append value */
  ret = netwib_buf_append_data(data, datasize, ptlv);
  if (ret != NETWIB_ERR_OK) {
    /* on error, rewind TL */
    ptlv->endoffset -= 8;
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_tlv_append_buf(netwib_constbuf *pbuf,
                                 netwib_buf *ptlv)
{
  netwib_data data;
  netwib_uint32 datasize;

  if (pbuf == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  netwib__buf_transfersensitive(pbuf, ptlv);

  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);
  netwib_er(netwib_priv_tlv_append_data(NETWIB_TLVTYPE_BUF, data, datasize,
                                        ptlv));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_tlv_append_uint32(netwib_uint32 ui,
                                    netwib_buf *ptlv)
{
  netwib_byte v[4];

  if ( ui & 0xFFFF0000u ) {
    v[0] = netwib_c2_uint32_0(ui);
    v[1] = netwib_c2_uint32_1(ui);
    v[2] = netwib_c2_uint32_2(ui);
    v[3] = netwib_c2_uint32_3(ui);
    netwib_er(netwib_priv_tlv_append_data(NETWIB_TLVTYPE_UINT, v, 4, ptlv));
  } else if ( ui & 0xFFFFFF00u ) {
    v[0] = netwib_c2_uint16_0(ui);
    v[1] = netwib_c2_uint16_1(ui);
    netwib_er(netwib_priv_tlv_append_data(NETWIB_TLVTYPE_UINT, v, 2, ptlv));
  } else {
    v[0] = (netwib_byte)ui;
    netwib_er(netwib_priv_tlv_append_data(NETWIB_TLVTYPE_UINT, v, 1, ptlv));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_tlv_append_uint64(netwib_uint64 ui,
                                    netwib_buf *ptlv)
{
  netwib_byte v[8];
  netwib_uint32 high, low;

  high = netwib_c2_uint64_32high(ui);
  low = netwib_c2_uint64_32low(ui);

  if (high != 0) {
    v[0] = netwib_c2_uint32_0(high);
    v[1] = netwib_c2_uint32_1(high);
    v[2] = netwib_c2_uint32_2(high);
    v[3] = netwib_c2_uint32_3(high);
    v[4] = netwib_c2_uint32_0(low);
    v[5] = netwib_c2_uint32_1(low);
    v[6] = netwib_c2_uint32_2(low);
    v[7] = netwib_c2_uint32_3(low);
    netwib_er(netwib_priv_tlv_append_data(NETWIB_TLVTYPE_UINT, v, 8, ptlv));
  } else {
    netwib_er(netwib_tlv_append_uint32(low, ptlv));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_tlv_append_eth(netwib_consteth *peth,
                                 netwib_buf *ptlv)
{
  netwib_er(netwib_priv_tlv_append_data(NETWIB_TLVTYPE_ETH, peth->b,
                                      NETWIB_ETH_LEN, ptlv));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_tlv_append_ip(netwib_constip *pip,
                                netwib_buf *ptlv)
{
  netwib_byte v[4];

  switch(pip->iptype) {
    case NETWIB_IPTYPE_IP4 :
      v[0] = netwib_c2_uint32_0(pip->ipvalue.ip4);
      v[1] = netwib_c2_uint32_1(pip->ipvalue.ip4);
      v[2] = netwib_c2_uint32_2(pip->ipvalue.ip4);
      v[3] = netwib_c2_uint32_3(pip->ipvalue.ip4);
      netwib_er(netwib_priv_tlv_append_data(NETWIB_TLVTYPE_IP, v, 4, ptlv));
      break;
    case NETWIB_IPTYPE_IP6 :
      netwib_er(netwib_priv_tlv_append_data(NETWIB_TLVTYPE_IP,
                                          pip->ipvalue.ip6.b, NETWIB_IP6_LEN,
                                          ptlv));
      break;
    default :
      return(NETWIB_ERR_PAIPTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_tlv_append_end(netwib_buf *ptlv)
{
  netwib_er(netwib_priv_tlv_append_data(NETWIB_TLVTYPE_END, NULL, 0, ptlv));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_tlv_append_newtype(netwib_tlvtype type,
                                     netwib_constbuf *pvalue,
                                     netwib_buf *ptlv)
{
  netwib_data data=NULL;
  netwib_uint32 datasize=0;

  if (pvalue != NULL) {
    data = netwib__buf_ref_data_ptr(pvalue);
    datasize = netwib__buf_ref_data_size(pvalue);
  }

  netwib_er(netwib_priv_tlv_append_data(type, data, datasize, ptlv));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_tlv_append_tlv(netwib_constbuf *pnewtlv,
                                 netwib_buf *ptlv)
{
  netwib_er(netwib_buf_append_buf(pnewtlv, ptlv));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_tlv_prepend_tlv(netwib_constbuf *pnewtlv,
                                  netwib_buf *ptlv)
{
  netwib_er(netwib_buf_prepend_buf(pnewtlv, ptlv));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_tlv_entry_typelenval(netwib_constbuf *ptlv,
                                                   netwib_tlvtype *ptype,
                                                   netwib_uint32 *plength,
                                                   netwib_data *pdata,
                                                   netwib_uint32 *pskipsize)
{
  netwib_data data;
  netwib_uint32 datasize;
  netwib_uint32 length;

  datasize = netwib__buf_ref_data_size(ptlv);
  if (datasize == 0) {
    return(NETWIB_ERR_DATAEND);
  }
  if (datasize < 8) {
    return(NETWIB_ERR_PATLVINVALID);
  }

  data = netwib__buf_ref_data_ptr(ptlv);
  length = netwib_c2_uint32_4(data[4], data[5], data[6], data[7]);
  if (datasize < length + 8) {
    return(NETWIB_ERR_PATLVINVALID);
  }

  if (ptype != NULL) {
    *ptype = (netwib_tlvtype)netwib_c2_uint32_4(data[0], data[1],
                                              data[2], data[3]);
  }

  if (plength != NULL) {
    *plength = length;
  }

  if (pdata != NULL) {
    *pdata = data + 8;
  }

  if (pskipsize != NULL) {
    *pskipsize = 8 + length;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_tlv_entry_typelen(netwib_constbuf *ptlv,
                                    netwib_tlvtype *ptype,
                                    netwib_uint32 *plength,
                                    netwib_uint32 *pskipsize)
{
  if (ptlv == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  netwib_er(netwib_priv_tlv_entry_typelenval(ptlv, ptype, plength, NULL,
                                           pskipsize));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_tlv_decode_buf(netwib_constbuf *ptlv,
                                 netwib_bufext *pbuf,
                                 netwib_uint32 *pskipsize)
{
  netwib_tlvtype type;
  netwib_data data;
  netwib_uint32 datasize;

  netwib_er(netwib_priv_tlv_entry_typelenval(ptlv, &type, &datasize, &data,
                                             pskipsize));
  switch(type) {
    case NETWIB_TLVTYPE_BUF :
      netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, pbuf));
      netwib__buf_transfersensitive(ptlv, pbuf);
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_TLVTYPE_END :
      return(NETWIB_ERR_DATAEND);
      break;
    default :
      return(NETWIB_ERR_DATAOTHERTYPE);
      break;
  }

  return(NETWIB_ERR_LOINTERNALERROR);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_tlv_decode_uint(netwib_constbuf *ptlv,
                                         netwib_uint32 *puihigh,
                                         netwib_uint32 *puilow,
                                         netwib_uint32 *pskipsize)
{
  netwib_tlvtype type;
  netwib_data data;
  netwib_uint32 datasize;

  netwib_er(netwib_priv_tlv_entry_typelenval(ptlv, &type, &datasize, &data,
                                             pskipsize));
  switch(type) {
    case NETWIB_TLVTYPE_UINT :
      switch(datasize) {
        case 1 :
          *puihigh = 0;
          *puilow = data[0];
          break;
        case 2 :
          *puihigh = 0;
          *puilow = netwib_c2_uint16_2(data[0], data[1]);
          break;
        case 4 :
          *puihigh = 0;
          *puilow = netwib_c2_uint32_4(data[0], data[1], data[2], data[3]);
          break;
        case 8 :
          *puihigh = netwib_c2_uint32_4(data[0], data[1], data[2], data[3]);
          *puilow = netwib_c2_uint32_4(data[4], data[5], data[6], data[7]);
          break;
        default :
          return(NETWIB_ERR_PATLVINVALID);
          break;
      }
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_TLVTYPE_END :
      return(NETWIB_ERR_DATAEND);
      break;
    default :
      return(NETWIB_ERR_DATAOTHERTYPE);
      break;
  }

  return(NETWIB_ERR_LOINTERNALERROR);
}

/*-------------------------------------------------------------*/
netwib_err netwib_tlv_decode_uint32(netwib_constbuf *ptlv,
                                    netwib_uint32 *pui,
                                    netwib_uint32 *pskipsize)
{
  netwib_uint32 high, low;

  netwib_er(netwib_tlv_decode_uint(ptlv, &high, &low, pskipsize));

  if (high) {
    return(NETWIB_ERR_PATLVINVALID);
  }
  if (pui != NULL) {
    *pui = low;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_tlv_decode_uint64(netwib_constbuf *ptlv,
                                    netwib_uint64 *pui,
                                    netwib_uint32 *pskipsize)
{
  netwib_uint32 high, low;

  netwib_er(netwib_tlv_decode_uint(ptlv, &high, &low, pskipsize));

  if (pui != NULL) {
    netwib__uint64_init_32(high, low, *pui);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_tlv_decode_eth(netwib_constbuf *ptlv,
                                 netwib_eth *peth,
                                 netwib_uint32 *pskipsize)
{
  netwib_tlvtype type;
  netwib_data data;
  netwib_uint32 datasize;

  netwib_er(netwib_priv_tlv_entry_typelenval(ptlv, &type, &datasize, &data,
                                           pskipsize));
  switch(type) {
    case NETWIB_TLVTYPE_ETH :
      if (datasize != NETWIB_ETH_LEN) {
        return(NETWIB_ERR_PATLVINVALID);
      }
      if (peth != NULL) {
        netwib_c_memcpy(peth->b, data, NETWIB_ETH_LEN);
      }
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_TLVTYPE_END :
      return(NETWIB_ERR_DATAEND);
      break;
    default :
      return(NETWIB_ERR_DATAOTHERTYPE);
      break;
  }

  return(NETWIB_ERR_LOINTERNALERROR);
}

/*-------------------------------------------------------------*/
netwib_err netwib_tlv_decode_ip(netwib_constbuf *ptlv,
                                netwib_ip *pip,
                                netwib_uint32 *pskipsize)
{
  netwib_tlvtype type;
  netwib_data data;
  netwib_uint32 datasize;

  netwib_er(netwib_priv_tlv_entry_typelenval(ptlv, &type, &datasize, &data,
                                           pskipsize));
  switch(type) {
    case NETWIB_TLVTYPE_IP :
      switch(datasize) {
        case 4 :
          if (pip != NULL) {
            pip->iptype = NETWIB_IPTYPE_IP4;
            pip->ipvalue.ip4 = netwib_c2_uint32_4(data[0], data[1],
                                                 data[2], data[3]);
          }
          break;
        case NETWIB_IP6_LEN :
          if (pip != NULL) {
            pip->iptype = NETWIB_IPTYPE_IP6;
            netwib_c_memcpy(pip->ipvalue.ip6.b, data, NETWIB_IPTYPE_IP6);
          }
          break;
        default :
          return(NETWIB_ERR_PATLVINVALID);
      }
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_TLVTYPE_END :
      return(NETWIB_ERR_DATAEND);
      break;
    default :
      return(NETWIB_ERR_DATAOTHERTYPE);
      break;
  }

  return(NETWIB_ERR_LOINTERNALERROR);
}

/*-------------------------------------------------------------*/
netwib_err netwib_tlv_decode_newtype(netwib_constbuf *ptlv,
                                     netwib_tlvtype *ptype,
                                     netwib_uint32 *plength,
                                     netwib_bufext *pvalue,
                                     netwib_uint32 *pskipsize)
{
  netwib_tlvtype type;
  netwib_data data;
  netwib_uint32 datasize;

  netwib_er(netwib_priv_tlv_entry_typelenval(ptlv, &type, &datasize, &data,
                                           pskipsize));

  if (ptype != NULL) {
    *ptype = type;
  }

  if (plength != NULL) {
    *plength = datasize;
  }

  if (type == NETWIB_TLVTYPE_END) {
    /* in this case, stop here and do not set value */
    return(NETWIB_ERR_DATAEND);
  }

  netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, pvalue));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_tlv_decode_tlv(netwib_constbuf *ptlv,
                                 netwib_bufext *pfirsttlv,
                                 netwib_uint32 *pskipsize)
{
  netwib_tlvtype type;
  netwib_data data;
  netwib_uint32 datasize;

  netwib_er(netwib_priv_tlv_entry_typelenval(ptlv, &type, &datasize, &data,
                                             pskipsize));

  netwib_er(netwib_buf_init_ext_arrayfilled(netwib__buf_ref_data_ptr(ptlv),
                                            8+datasize, pfirsttlv));
  netwib__buf_transfersensitive(ptlv, pfirsttlv);

  return(NETWIB_ERR_OK);
}
