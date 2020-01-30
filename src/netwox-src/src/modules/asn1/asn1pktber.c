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
#define NETWOX_PKTBER_DECODE_PREVENTDOS 1

/*-------------------------------------------------------------*/
/* Encode an "unsigned integer" using ASN.1 "integer" type. So,
     0x1 -> 01
     0x93 -> 0093 (because 93 would be recognized as 0xFFFFFF93)
   The unsigned integer is only on 32 bits.
 */
static netwib_err netwox_priv_pktber_append_uint32(netwib_uint32 ui32,
                                                   netwib_buf *ppkt)
{
  netwib_data data;

  if (ui32 <= 0x7F) {
    netwib_er(netwib_buf_wantspace(ppkt, 1, &data));
    *data = (netwib_byte)ui32;
    ppkt->endoffset += 1;
  } else if (ui32 <= 0x7FFF) {
    netwib_er(netwib_buf_wantspace(ppkt, 2, &data));
    *data++ = netwib_c2_uint32_2(ui32);
    *data = netwib_c2_uint32_3(ui32);
    ppkt->endoffset += 2;
  } else if (ui32 <= 0x7FFFFF) {
    netwib_er(netwib_buf_wantspace(ppkt, 3, &data));
    *data++ = netwib_c2_uint32_1(ui32);
    *data++ = netwib_c2_uint32_2(ui32);
    *data = netwib_c2_uint32_3(ui32);
    ppkt->endoffset += 3;
  } else if (ui32 <= 0x7FFFFFFF) {
    netwib_er(netwib_buf_wantspace(ppkt, 4, &data));
    *data++ = netwib_c2_uint32_0(ui32);
    *data++ = netwib_c2_uint32_1(ui32);
    *data++ = netwib_c2_uint32_2(ui32);
    *data = netwib_c2_uint32_3(ui32);
    ppkt->endoffset += 4;
  } else {
    netwib_er(netwib_buf_wantspace(ppkt, 5, &data));
    *data++ = 0;
    *data++ = netwib_c2_uint32_0(ui32);
    *data++ = netwib_c2_uint32_1(ui32);
    *data++ = netwib_c2_uint32_2(ui32);
    *data = netwib_c2_uint32_3(ui32);
    ppkt->endoffset += 5;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* Encode an "unsigned integer" using ASN.1 "integer" type. So,
     0x1 -> 01
     0x93 -> 0093 (because 93 would be recognized as 0xFFFFFF93)
   The unsigned integer is on 64 bits.
 */
static netwib_err netwox_priv_pktber_append_uint64(netwib_uint64 ui64,
                                                   netwib_buf *ppkt)
{
  netwib_data data;
  netwib_uint32 high, low;

  high = netwib_c2_uint64_32high(ui64);
  low = netwib_c2_uint64_32low(ui64);

  if (high == 0) {
    netwib_er(netwox_priv_pktber_append_uint32(low, ppkt));
  } else {
    netwib_er(netwox_priv_pktber_append_uint32(high, ppkt));
    netwib_er(netwib_buf_wantspace(ppkt, 4, &data));
    netwib__data_append_uint32(data, low);
    ppkt->endoffset += 4;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* Encode an "integer" using ASN.1 "integer" type. So, there is
   nothing strange here.
   The integer can be 32 or 64 bits (netwib_intmax).
 */
static netwib_err netwox_priv_pktber_append_intmax(netwib_intmax imax,
                                                   netwib_buf *ppkt)
{
  netwib_data data;
  netwib_uintmax uimax, mask1, mask2;
  netwib_uint32 intbytes;

  /* compute useless leading 0x00 or 0xFF */
  uimax = (netwib_uintmax)imax;
#if NETWIB_INTMAX_BITS == 64
  intbytes = 8;
  mask1 = NETWIB_UINT_LL(0xFF80000000000000);
  mask2 = NETWIB_UINT_LL(0xFFFFFFFFFFFFFFFF);
#else
  intbytes = 4;
  mask1 = 0xFF800000u;
  mask2 = 0xFFFFFFFFu;
#endif
  while ((uimax & mask1) == 0 ||
         (uimax & mask1) == mask1) {
    mask2 >>= 8;
    uimax &= mask2;
    intbytes--;
    if (intbytes == 1) {
      break;
    }
    mask1 >>= 8;
  }

  netwib_er(netwib_buf_wantspace(ppkt, intbytes, &data));
  ppkt->endoffset += intbytes;
  while (intbytes--) {
    *data++ = (netwib_byte)((uimax >> 8*intbytes) & 0xFF);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* Encode an "identifier" (for now, suppose it's only on 32 bits).
 */
static netwib_err netwox_priv_pktber_append_id(netwib_uint32 ui32,
                                               netwib_buf *ppkt)
{
  netwib_data data;

  if (ui32 < 0x80) {
    netwib_er(netwib_buf_wantspace(ppkt, 1, &data));
    *data = (netwib_byte)ui32;
    ppkt->endoffset++;
  } else if (ui32 < 0x4000) {
    netwib_er(netwib_buf_wantspace(ppkt, 2, &data));
    *data++ = (netwib_byte)(0x80 | (ui32>>7));
    *data = (netwib_byte)(ui32 & 0x7F);
    ppkt->endoffset += 2;
  } else if (ui32 < 0x200000) {
    netwib_er(netwib_buf_wantspace(ppkt, 3, &data));
    *data++ = (netwib_byte)(0x80 | (ui32>>14));
    *data++ = (netwib_byte)(0x80 | ((ui32>>7) & 0x7F));
    *data = (netwib_byte)(ui32 & 0x7F);
    ppkt->endoffset += 3;
  } else if (ui32 < 0x10000000) {
    netwib_er(netwib_buf_wantspace(ppkt, 4, &data));
    *data++ = (netwib_byte)(0x80 | (ui32>>21));
    *data++ = (netwib_byte)(0x80 | ((ui32>>14) & 0x7F));
    *data++ = (netwib_byte)(0x80 | ((ui32>>7) & 0x7F));
    *data = (netwib_byte)(ui32 & 0x7F);
    ppkt->endoffset += 4;
  } else  {
    netwib_er(netwib_buf_wantspace(ppkt, 5, &data));
    *data++ = (netwib_byte)(0x80 | (ui32>>28));
    *data++ = (netwib_byte)(0x80 | ((ui32>>21) & 0x7F));
    *data++ = (netwib_byte)(0x80 | ((ui32>>14) & 0x7F));
    *data++ = (netwib_byte)(0x80 | ((ui32>>7) & 0x7F));
    *data = (netwib_byte)(ui32 & 0x7F);
    ppkt->endoffset += 5;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* Encode a "length" (for now, suppose it's only on 32 bits).
 */
static netwib_err netwox_priv_pktber_append_length(netwib_uint32 ui32,
                                                   netwib_buf *ppkt)
{
  netwib_data data;

  if (ui32 <= 0xFF) {
    netwib_er(netwib_buf_wantspace(ppkt, 1, &data));
    *data = (netwib_byte)ui32;
    ppkt->endoffset += 1;
  } else if (ui32 <= 0xFFFF) {
    netwib_er(netwib_buf_wantspace(ppkt, 2, &data));
    *data++ = netwib_c2_uint32_2(ui32);
    *data = netwib_c2_uint32_3(ui32);
    ppkt->endoffset += 2;
  } else if (ui32 <= 0xFFFFFF) {
    netwib_er(netwib_buf_wantspace(ppkt, 3, &data));
    *data++ = netwib_c2_uint32_1(ui32);
    *data++ = netwib_c2_uint32_2(ui32);
    *data = netwib_c2_uint32_3(ui32);
    ppkt->endoffset += 3;
  } else {
    netwib_er(netwib_buf_wantspace(ppkt, 4, &data));
    *data++ = netwib_c2_uint32_0(ui32);
    *data++ = netwib_c2_uint32_1(ui32);
    *data++ = netwib_c2_uint32_2(ui32);
    *data = netwib_c2_uint32_3(ui32);
    ppkt->endoffset += 4;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_pktber_append_asn1data(netwox_constasn1data *pasn1data,
                                                     netwib_buf *ppkt)
{
  netwib_data data;
  netwib_uint32 i, ui, numbytes, numunusedbits;

  switch (pasn1data->asn1datastore) {
  case NETWOX_ASN1DATASTORE_NOTHING :
    break;
  case NETWOX_ASN1DATASTORE_B :
    netwib_er(netwib_buf_append_byte((netwib_byte)(pasn1data->value.b?0xFF:0), ppkt));
    break;
  case NETWOX_ASN1DATASTORE_I :
    netwib_er(netwox_priv_pktber_append_intmax(pasn1data->value.i, ppkt));
    break;
  case NETWOX_ASN1DATASTORE_UI32 :
    netwib_er(netwox_priv_pktber_append_uint32(pasn1data->value.ui32, ppkt));
    break;
  case NETWOX_ASN1DATASTORE_UI64 :
    netwib_er(netwox_priv_pktber_append_uint64(pasn1data->value.ui64, ppkt));
    break;
  case NETWOX_ASN1DATASTORE_IPAD :
    if (pasn1data->value.ipad.iptype != NETWIB_IPTYPE_IP4) {
      return(NETWIB_ERR_PAIPTYPENOT4);
    }
    netwib_er(netwib_buf_wantspace(ppkt, 4, &data));
    netwib__data_append_uint32(data, pasn1data->value.ipad.ipvalue.ip4);
    ppkt->endoffset += 4;
    break;
  case NETWOX_ASN1DATASTORE_UNKNOWN :
    netwib_er(netwib_buf_append_buf(pasn1data->value.unknown.pbuf, ppkt));
    break;
  case NETWOX_ASN1DATASTORE_BITSTRING :
    numbytes = (pasn1data->value.bitstring.numbits + 7)/ 8;
    numunusedbits = 8*numbytes - pasn1data->value.bitstring.numbits;
    if (netwib__buf_ref_data_size(pasn1data->value.bitstring.pbuf) <numbytes) {
      return(NETWIB_ERR_PATOOHIGH);
    }
    netwib_er(netwib_buf_wantspace(ppkt, 1 + numbytes, &data));
    *data++ = (netwib_byte)numunusedbits;
    netwib_c_memcpy(data,
                    netwib__buf_ref_data_ptr(pasn1data->value.bitstring.pbuf),
                    numbytes);
    if (numunusedbits) {
      data += numbytes - 1;
      *data = (netwib_byte)(*data & (~((1 << numunusedbits) - 1)));
    }
    ppkt->endoffset += 1 + numbytes;
    break;
  case NETWOX_ASN1DATASTORE_BUF :
    netwib_er(netwib_buf_append_buf(pasn1data->value.pbuf, ppkt));
    break;
  case NETWOX_ASN1DATASTORE_OID :
    if (pasn1data->value.oid.id.size < 2) {
      return(NETWIB_ERR_PATOOLOW);
    }
    ui = *((netwib_uint32*)pasn1data->value.oid.id.p[0]);
    ui = 40*ui + *((netwib_uint32*)pasn1data->value.oid.id.p[1]);
    netwib_er(netwox_priv_pktber_append_id(ui, ppkt));
    for (i = 2; i < pasn1data->value.oid.id.size; i++) {
      ui = *((netwib_uint32*)pasn1data->value.oid.id.p[i]);
      netwib_er(netwox_priv_pktber_append_id(ui, ppkt));
    }
    break;
  case NETWOX_ASN1DATASTORE_SEQUENCE :
    for (i = 0; i < pasn1data->value.sequence.seq.size; i++) {
      netwib_er(netwox_pktber_append_asn1data((netwox_asn1data*)pasn1data->value.sequence.seq.p[i], ppkt));
    }
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pktber_append_asn1data(netwox_constasn1data *pasn1data,
                                         netwib_buf *ppkt)
{
  netwib_uint32 length;
  netwox_asn1tag_class cl=0;
  netwib_bool constructed=NETWIB_FALSE;
  netwox_asn1tag_num num=0;
  netwib_data data;
  netwib_buf *pbuf;
  netwib_byte b;

  /* obtain and write type */
  if (pasn1data->asn1tag == NETWOX_ASN1TAG_UNKNOWN) {
    cl = pasn1data->value.unknown.cl;
    constructed = pasn1data->value.unknown.constructed;
    num = pasn1data->value.unknown.num;
  } else {
    netwib_er(netwox_asn1tag_decode(pasn1data->asn1tag, &cl, &constructed,
                                    &num));
  }
  b = (netwib_byte)(cl << 6);
  if (constructed) {
    b |= 0x20;
  }
  if (num >= 31) {
    b |= 0x1F;
  } else {
    b |= num;
  }
  netwib_er(netwib_buf_wantspace(ppkt, 1, &data));
  *data = b;
  ppkt->endoffset++;
  if (num >= 31) {
    /* we have other bytes to set */
    netwib_er(netwox_priv_pktber_append_id(num, ppkt));
  }

  /* create value */
  netwib_er(netwib_bufpool_buf_init(pasn1data->pbufpool, &pbuf));
  netwib_er(netwox_priv_pktber_append_asn1data(pasn1data, pbuf));

  /* obtain and write length */
  length = netwib__buf_ref_data_size(pbuf);
  if (length < 0x80) {
    netwib_er(netwib_buf_wantspace(ppkt, 1, &data));
    *data = (netwib_byte)length;
    ppkt->endoffset++;
  } else {
    netwib_er(netwib_buf_wantspace(ppkt, 1, &data));
    if (length <= 0xFF) {
      *data++ = 0x80 | 1;
    } else if (length <= 0xFFFF) {
      *data++ = 0x80 | 2;
    } else if (length <= 0xFFFFFF) {
      *data++ = 0x80 | 3;
    } else {
      *data++ = 0x80 | 4;
    }
    ppkt->endoffset++;
    netwib_er(netwox_priv_pktber_append_length(length, ppkt));
  }

  /* write value */
  netwib_er(netwib_buf_append_buf(pbuf, ppkt));
  netwib_er(netwib_bufpool_buf_close(pasn1data->pbufpool, &pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_pktber_decode_uint32(netwib_buf *ppkt,
                                                   netwib_uint32 length,
                                                   netwib_uint32 *pui32)
{
  netwib_data data;
  netwib_uint32 datasize, ui32;

  data = netwib__buf_ref_data_ptr(ppkt);
  datasize = netwib__buf_ref_data_size(ppkt);

  if (datasize == 0 || datasize < length) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  if (length > 5 ||
      (length == 5 && data[0] != 0)) {
    return(NETWOX_ERR_NOTSUPPORTED);
  }

  ppkt->beginoffset += length;

  ui32 = 0;
  while(length--) {
    ui32 = (ui32<<8) | *data++;
  }
  *pui32 = ui32;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_pktber_decode_uint64(netwib_buf *ppkt,
                                                   netwib_uint32 length,
                                                   netwib_uint64 *pui64)
{
  netwib_data data;
  netwib_uint32 datasize;
  netwib_uint64 ui64, c64;

  data = netwib__buf_ref_data_ptr(ppkt);
  datasize = netwib__buf_ref_data_size(ppkt);

  if (datasize == 0 || datasize < length) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  if (length > 9 ||
      (length == 9 && data[0] != 0)) {
    return(NETWOX_ERR_NOTSUPPORTED);
  }

  ppkt->beginoffset += length;

  netwib__uint64_init_uint32(0, ui64);
  while (length--) {
    netwib__uint64_shl(ui64, 8, ui64);
    netwib__uint64_init_uint32(*data++, c64);
    netwib__uint64_or(ui64, c64, ui64);
  }
  *pui64 = ui64;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_pktber_decode_intmax(netwib_buf *ppkt,
                                                   netwib_uint32 length,
                                                   netwib_intmax *pimax)
{
  netwib_data data;
  netwib_uint32 datasize;
  netwib_uintmax uimax;

  data = netwib__buf_ref_data_ptr(ppkt);
  datasize = netwib__buf_ref_data_size(ppkt);

  if (datasize == 0 || datasize < length) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  if (length > NETWIBDEF_ARCH_BITS/8) {
    return(NETWOX_ERR_NOTSUPPORTED);
  }

  ppkt->beginoffset += length;

  uimax = 0;
  if (*data & 0x80) {
    uimax = (netwib_uintmax)(~0); /* 0xFFF... */
  }
  while(length--) {
    uimax = (uimax<<8) | *data++;
  }
  *pimax = (netwib_intmax)uimax;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_pktber_decode_id(netwib_buf *ppkt,
                                               netwib_uint32 *pui32)
{
  netwib_data data;
  netwib_uint32 datasize, ui32;
  netwib_byte b;

  data = netwib__buf_ref_data_ptr(ppkt);
  datasize = netwib__buf_ref_data_size(ppkt);

  if (datasize == 0) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  if (data[0] == 0x80) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  ui32 = 0;
  while (datasize--) {
    b = *data++;
    ui32 = (ui32<<7) | (b&0x7F);
    if (b & 0x80) {
      if (ui32 > 0x1FFFFFF) { /* only support 32 bits */
        return(NETWOX_ERR_NOTSUPPORTED);
      }
    } else {
      break;
    }
  }
  *pui32 = ui32;

  ppkt->beginoffset += data - netwib__buf_ref_data_ptr(ppkt);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* Currently, this function can also decode length */
#define netwox_priv_pktber_decode_length(ppkt,length,pui32) netwox_priv_pktber_decode_uint32(ppkt,length,pui32)

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_pktber_decode_asn1data(netwox_asn1tag_ext ext,
                                                     netwox_asn1tag_ctx *pctx,
                                                     netwib_buf *ppkt,
                                                     netwib_bool constructed,
                                                     netwox_asn1data *pasn1data)
{
  netwox_asn1data asn1data;
  netwib_data data;
  netwib_uint32 datasize, subid, subid2=0;
  netwib_bool firstdone;
  netwib_err ret;

  data = netwib__buf_ref_data_ptr(ppkt);
  datasize = netwib__buf_ref_data_size(ppkt);

  switch (pasn1data->asn1datastore) {
  case NETWOX_ASN1DATASTORE_NOTHING :
    if (constructed) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_ASN1DATASTORE_B :
    if (constructed) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize != 1) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    pasn1data->value.b = (*data)?NETWIB_TRUE:NETWIB_FALSE;
    break;
  case NETWOX_ASN1DATASTORE_I :
    if (constructed) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_priv_pktber_decode_intmax(ppkt, datasize,
                                               &pasn1data->value.i));
    break;
  case NETWOX_ASN1DATASTORE_UI32 :
    if (constructed) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_priv_pktber_decode_uint32(ppkt, datasize,
                                               &pasn1data->value.ui32));
    break;
    break;
  case NETWOX_ASN1DATASTORE_UI64 :
    if (constructed) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_priv_pktber_decode_uint64(ppkt, datasize,
                                               &pasn1data->value.ui64));
    break;
  case NETWOX_ASN1DATASTORE_IPAD :
    if (constructed) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize != 4) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    pasn1data->value.ipad.iptype = NETWIB_IPTYPE_IP4;
    pasn1data->value.ipad.ipvalue.ip4 = netwib_c2_uint32_4(data[0], data[1], data[2], data[3]);
    break;
  case NETWOX_ASN1DATASTORE_UNKNOWN :
    netwib_er(netwib_buf_append_buf(ppkt, pasn1data->value.unknown.pbuf));
    ppkt->beginoffset = ppkt->endoffset;
    break;
  case NETWOX_ASN1DATASTORE_BITSTRING :
    if (constructed) {
      return(NETWOX_ERR_NOTSUPPORTED);
    }
    if (datasize == 0) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (data[0] > 7) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize == 1 && data[0] != 0) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    ppkt->beginoffset++;
    pasn1data->value.bitstring.numbits = 8 * netwib__buf_ref_data_size(ppkt)
      - data[0];
    if (pasn1data->value.bitstring.numbits)
    netwib_er(netwib_buf_append_buf(ppkt, pasn1data->value.bitstring.pbuf));
    ppkt->beginoffset = ppkt->endoffset;
    break;
  case NETWOX_ASN1DATASTORE_BUF :
    if (constructed) {
      return(NETWOX_ERR_NOTSUPPORTED);
    }
    netwib_er(netwib_buf_append_buf(ppkt, pasn1data->value.pbuf));
    ppkt->beginoffset = ppkt->endoffset;
    break;
  case NETWOX_ASN1DATASTORE_OID :
    if (constructed) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    firstdone = NETWIB_FALSE;
    while(netwib__buf_ref_data_size(ppkt)) {
      /* decode */
      netwib_er(netwox_priv_pktber_decode_id(ppkt, &subid));
      /* add */
      if (!firstdone) {
        if (subid < 40) {
          subid2 = subid;
          subid = 0;
        } else if (subid < 80) {
          subid2 = subid - 40;
          subid = 1;
        } else {
          subid2 = subid - 80;
          subid = 2;
        }
        netwib_er(netwox_asn1data_add_oid(pasn1data, subid));
        netwib_er(netwox_asn1data_add_oid(pasn1data, subid2));
        firstdone = NETWIB_TRUE;
      } else {
        netwib_er(netwox_asn1data_add_oid(pasn1data, subid));
      }
    }
    if (!firstdone) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_ASN1DATASTORE_SEQUENCE :
    if (!constructed) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
#if NETWOX_PKTBER_DECODE_PREVENTDOS == 1
    /* prevent denial of services */
    if (pctx->tag.size > 10) {
      return(NETWOX_ERR_NOTSUPPORTED);
    }
#endif
    netwib_er(netwox_asn1tag_ctx_add(pctx, pasn1data->asn1tag));
    while(netwib__buf_ref_data_size(ppkt)) {
      /* decode */
      netwib_er(netwox_asn1data_init(pasn1data->pbufpool, &asn1data));
      ret = netwox_pktber_decode_asn1data(ext, pctx, ppkt, &asn1data);
      if (ret != NETWIB_ERR_OK) {
        netwib_er(netwox_asn1data_close(&asn1data));
        return(ret);
      }
      /* check for end (indefinite length) */
      if (asn1data.asn1tag == NETWOX_ASN1TAG_END) {
        break;
      }
#if NETWOX_PKTBER_DECODE_PREVENTDOS == 1
      /* prevent denial of services */
      if (pasn1data->value.sequence.seq.size > 200) {
        netwib_er(netwox_asn1data_close(&asn1data));
        return(NETWOX_ERR_NOTSUPPORTED);
      }
#endif
      /* add */
      netwib_er(netwox_asn1data_add_sequence(pasn1data, &asn1data));
    }
    netwib_er(netwox_asn1tag_ctx_del(pctx));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pktber_decode_asn1data(netwox_asn1tag_ext ext,
                                         netwox_asn1tag_ctx *pctx,
                                         netwib_buf *ppkt,
                                         netwox_asn1data *pasn1data)
{
  netwib_data data;
  netwib_uint32 length, lengthlength;
  netwox_asn1tag_class cl;
  netwox_asn1tag_num num;
  netwox_asn1tag asn1tag;
  netwib_buf pkt;
  netwib_bool constructed, indefinite;

#if 0
  netwib_er(netwib_buf_display(ppkt, NETWIB_ENCODETYPE_HEXA0));
  netwib_er(netwib_fmt_display("\n"));
#endif

  if (netwib__buf_ref_data_size(ppkt) == 0) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  data = netwib__buf_ref_data_ptr(ppkt);

  /* class */
  cl = (netwox_asn1tag_class)(data[0] >> 6);

  /* constructed */
  constructed = data[0] & 0x20;

  /* type */
  num = data[0] & 0x1F;
  pkt = *ppkt;
  pkt.beginoffset++;
  if (num == 0x1F) {
    netwib_er(netwox_priv_pktber_decode_id(&pkt, (netwib_uint32*)&num));
    if (num <= 30) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  /* tag value */
  netwib_er(netwox_asn1tag_init(ext, pctx, cl, num, &asn1tag));

  /* length */
  indefinite = NETWIB_FALSE;
  if (netwib__buf_ref_data_size(&pkt) == 0) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  data = netwib__buf_ref_data_ptr(&pkt);
  length = data[0];
  pkt.beginoffset++;
  if (length & 0x80) {
    lengthlength = length & 0x7F;
    if (lengthlength) { /* long definite form */
      netwib_er(netwox_priv_pktber_decode_length(&pkt, lengthlength, &length));
    } else { /* long indefinite form : will terminate on an end */
      if (!constructed) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      indefinite = NETWIB_TRUE;
    }
  }
  if (!indefinite) {
    if (length > netwib__buf_ref_data_size(&pkt)) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  /* set asn1 type */
  netwib_er(netwox_asn1data_set_type(pasn1data, asn1tag));
  if (asn1tag == NETWOX_ASN1TAG_UNKNOWN) {
    pasn1data->value.unknown.cl = cl;
    pasn1data->value.unknown.constructed = constructed;
    pasn1data->value.unknown.num = num;
  }

  /* value */
  if (!indefinite) {
    pkt.endoffset = pkt.beginoffset + length;
  }

  netwib_er(netwox_priv_pktber_decode_asn1data(ext, pctx, &pkt, constructed,
                                               pasn1data));

  /* now, it's ok so we can update ppkt */
  ppkt->beginoffset = pkt.endoffset;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pktber_decode_asn1data_snmp(netwib_buf *ppkt,
                                              netwox_asn1data *pasn1data)
{
  netwox_asn1tag_ctx asn1ctx;
  netwib_err ret;

  netwib_er(netwox_asn1tag_ctx_init(&asn1ctx));
  ret = netwox_pktber_decode_asn1data(NETWOX_ASN1TAG_EXT_SNMP, &asn1ctx,
                                      ppkt, pasn1data);
  netwib_er(netwox_asn1tag_ctx_close(&asn1ctx));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pktber_decode_asn1data_snmp3sp(netwib_buf *ppkt,
                                                 netwox_asn1data *pasn1data)
{
  netwox_asn1tag_ctx asn1ctx;
  netwib_err ret;

  netwib_er(netwox_asn1tag_ctx_init(&asn1ctx));
  ret = netwox_pktber_decode_asn1data(NETWOX_ASN1TAG_EXT_SNMP3SP, &asn1ctx,
                                      ppkt, pasn1data);
  netwib_er(netwox_asn1tag_ctx_close(&asn1ctx));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pktber_test(netwox_asn1tag_ext ext,
                              netwib_constbuf *ppkt)
{
  netwib_bufpool *pbufpool;
  netwox_asn1data asn1data;
  netwox_asn1tag_ctx ctx;
  netwib_buf *pbuf, pkt;
  netwib_cmp cmp;
  netwib_err ret;

  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  netwib_er(netwox_asn1data_init(pbufpool, &asn1data));
  netwib_er(netwox_asn1tag_ctx_init(&ctx));

  pkt = *ppkt;
  netwib_er(netwox_pkt_asn1_display(pbufpool, ext, &pkt, NULL,
                                    NETWIB_ENCODETYPE_ARRAY,
                                    NETWIB_ENCODETYPE_DUMP));

  pkt = *ppkt;
  ret = netwox_pktber_decode_asn1data(ext, &ctx, &pkt, &asn1data);
  if (ret == NETWIB_ERR_NOTCONVERTED) {
    netwib_er(netwib_fmt_display("ASN.1 not decoded\n"));
    netwib_er(netwox_asn1data_close(&asn1data));
    netwib_er(netwib_bufpool_close(&pbufpool));
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_buf_display(ppkt, NETWIB_ENCODETYPE_DUMP));
    return(ret);
  }

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
  netwib_er(netwox_asn1data_show(&asn1data, NETWIB_ENCODETYPE_ARRAY, pbuf));
  netwib_er(netwib_buf_display(pbuf, NETWIB_ENCODETYPE_DATA));

  netwib__buf_reinit(pbuf);
  netwib_er(netwox_pktber_append_asn1data(&asn1data, pbuf));
  netwib_er(netwib_buf_cmp(pbuf, ppkt, &cmp));
  ret = NETWIB_ERR_OK;
  if (cmp != NETWIB_CMP_EQ) {
    netwib_er(netwib_fmt_display("Mismatch :\n"));
    netwib_er(netwib_buf_display(pbuf, NETWIB_ENCODETYPE_DUMP));
    netwib_er(netwib_fmt_display("Wanted :\n"));
    netwib_er(netwib_buf_display(ppkt, NETWIB_ENCODETYPE_DUMP));
    ret = 12345;
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));
  netwib_er(netwox_asn1tag_ctx_close(&ctx));
  netwib_er(netwox_asn1data_close(&asn1data));
  netwib_er(netwib_bufpool_close(&pbufpool));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_pktber_test_static2(netwib_conststring pkt)
{
  netwib_buf buf1, buf2;

  netwib_er(netwib_buf_init_mallocdefault(&buf1));

  netwib_er(netwib_buf_init_ext_string(pkt, &buf2));
  netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_MIXED, &buf1));

  netwib_er(netwox_pktber_test(NETWOX_ASN1TAG_EXT_SNMP, &buf1));

  netwib_er(netwib_buf_close(&buf1));

  return(NETWIB_ERR_OK);
}
netwib_err netwox_pktber_test_static(void)
{

  netwib_er(netwox_pktber_test_static2("302902010004067075626c6963a01c0204158347c9020100020100300e300c06082b060102010104000500"));

  netwib_er(netwox_pktber_test_static2("302902010094067075626c6963a01c0204158347c9020100020100300e300c06082b060102010104000500"));

  netwib_er(netwox_pktber_test_static2("0101FF")); /* bool */

  netwib_er(netwox_pktber_test_static2("03020045")); /* bitstring */
  netwib_er(netwox_pktber_test_static2("03020144")); /* bitstring */
  netwib_er(netwox_pktber_test_static2("0303023444")); /* bitstring */
  netwib_er(netwox_pktber_test_static2("0303073480")); /* bitstring */
  netwib_er(netwox_pktber_test_static2("030100")); /* bitstring */

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_pktber_test_samples2(netwib_bufpool *pbufpool,
                                              netwib_constbuf *ppathfilename)
{
  netwib_io *pio;
  netwib_buf pkt;
  netwib_err ret;

  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_io_init_file_read(ppathfilename, &pio));
  while (NETWIB_TRUE) {
    ret = netwib_io_read(pio, &pkt);
    if (ret == NETWIB_ERR_DATAEND) {
      break;
    }
  }
  /*netwib_er(netwib_buf_display(&pkt, NETWIB_ENCODETYPE_DUMP));*/
  netwib_er(netwox_pkt_asn1_display(pbufpool, NETWOX_ASN1TAG_EXT_SNMP,
                                    &pkt, NULL, NETWIB_ENCODETYPE_ARRAY,
                                    NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_buf_close(&pkt));

  return(NETWIB_ERR_OK);
}
netwib_err netwox_pktber_test_samples(void)
{
  netwib_dir *pdir;
  netwib_bufpool *pbufpool;
  netwib_buf dirnamebuf, filename, pathfilename;
  netwib_err ret;

  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  if (0) {
    netwib_er(netwib_buf_init_ext_string("/file", &dirnamebuf));
    netwib_er(netwox_pktber_test_samples2(pbufpool, &dirnamebuf));
    netwib_er(netwib_bufpool_close(&pbufpool));
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_buf_init_ext_string("/file", &dirnamebuf));
  netwib_er(netwib_dir_init(&dirnamebuf, &pdir));
  netwib_er(netwib_buf_init_mallocdefault(&filename));
  netwib_er(netwib_buf_init_mallocdefault(&pathfilename));
  while (NETWIB_TRUE) {
    netwib__buf_reinit(&filename);
    ret = netwib_dir_next(pdir, &filename);
    if (ret == NETWIB_ERR_DATAEND) {
      break;
    }
    netwib_er(netwib_fmt_display("%{buf}\n", &filename));
    netwib__buf_reinit(&pathfilename);
    netwib_er(netwib_path_init_concat(&dirnamebuf, &filename, &pathfilename));
    netwib_er(netwox_pktber_test_samples2(pbufpool, &pathfilename));
  }

  netwib_er(netwib_buf_close(&pathfilename));
  netwib_er(netwib_buf_close(&filename));
  netwib_er(netwib_dir_close(&pdir));
  netwib_er(netwib_bufpool_close(&pbufpool));

  return(NETWIB_ERR_OK);
}
