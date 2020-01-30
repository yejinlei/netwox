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
netwib_err netwox_asn1data_init(netwib_bufpool *pbufpool,
                                netwox_asn1data *pasn1data)
{
  pasn1data->asn1tag = NETWOX_ASN1TAG_UNSET;
  pasn1data->pbufpool = pbufpool;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1data_close(netwox_asn1data *pasn1data)
{
  netwib_uint32 i;
  netwox_asn1data *pasn1datai;

  if (pasn1data->asn1tag == NETWOX_ASN1TAG_UNSET) {
    return(NETWIB_ERR_OK);
  }

  switch (pasn1data->asn1datastore) {
  case NETWOX_ASN1DATASTORE_NOTHING :
  case NETWOX_ASN1DATASTORE_B :
  case NETWOX_ASN1DATASTORE_I :
  case NETWOX_ASN1DATASTORE_UI32 :
  case NETWOX_ASN1DATASTORE_UI64 :
  case NETWOX_ASN1DATASTORE_IPAD :
    /* nothing to close */
    break;
  case NETWOX_ASN1DATASTORE_UNKNOWN :
    netwib_er(netwib_bufpool_buf_close(pasn1data->pbufpool,
                                       &pasn1data->value.unknown.pbuf));
    break;
  case NETWOX_ASN1DATASTORE_BITSTRING :
    netwib_er(netwib_bufpool_buf_close(pasn1data->pbufpool,
                                       &pasn1data->value.bitstring.pbuf));
    break;
  case NETWOX_ASN1DATASTORE_BUF :
    netwib_er(netwib_bufpool_buf_close(pasn1data->pbufpool,
                                       &pasn1data->value.pbuf));
    break;
  case NETWOX_ASN1DATASTORE_OID :
    netwib_er(netwox_asn1oid_close(&pasn1data->value.oid));
    break;
  case NETWOX_ASN1DATASTORE_SEQUENCE :
    for (i = 0; i < pasn1data->value.sequence.seq.size; i++) {
      pasn1datai = (netwox_asn1data *)pasn1data->value.sequence.seq.p[i];
      netwib_er(netwox_asn1data_close(pasn1datai));
    }
    netwib_er(netwib_array_close(&pasn1data->value.sequence.seq));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1data_set_type(netwox_asn1data *pasn1data,
                                    netwox_asn1tag asn1tag)
{

  if (pasn1data->asn1tag != NETWOX_ASN1TAG_UNSET) {
    netwib_er(netwox_asn1data_close(pasn1data));
  }

  switch (asn1tag) {
  case NETWOX_ASN1TAG_END :
  case NETWOX_ASN1TAG_NULL :
  case NETWOX_ASN1TAG_SNMPVAR_NOSUCHOBJECT :
  case NETWOX_ASN1TAG_SNMPVAR_NOSUCHINSTANCE :
  case NETWOX_ASN1TAG_SNMPVAR_ENDOFMIBVIEW :
    pasn1data->asn1datastore = NETWOX_ASN1DATASTORE_NOTHING;
    break;
  case NETWOX_ASN1TAG_BOOL :
    pasn1data->asn1datastore = NETWOX_ASN1DATASTORE_B;
    break;
  case NETWOX_ASN1TAG_INTEGER :
    pasn1data->asn1datastore = NETWOX_ASN1DATASTORE_I;
    break;
  case NETWOX_ASN1TAG_COUNTER32 :
  case NETWOX_ASN1TAG_GAUGE32 :
  case NETWOX_ASN1TAG_TIMETICKS :
  case NETWOX_ASN1TAG_UINT32 :
  case NETWOX_ASN1TAG_UINT32OLD :
    pasn1data->asn1datastore = NETWOX_ASN1DATASTORE_UI32;
    break;
  case NETWOX_ASN1TAG_COUNTER64 :
    pasn1data->asn1datastore = NETWOX_ASN1DATASTORE_UI64;
    break;
  case NETWOX_ASN1TAG_OCTETSTRING :
  case NETWOX_ASN1TAG_OPAQUE :
  case NETWOX_ASN1TAG_NSAP :
    pasn1data->asn1datastore = NETWOX_ASN1DATASTORE_BUF;
    break;
  case NETWOX_ASN1TAG_UNKNOWN :
    pasn1data->asn1datastore = NETWOX_ASN1DATASTORE_UNKNOWN;
    break;
  case NETWOX_ASN1TAG_IP4 :
    pasn1data->asn1datastore = NETWOX_ASN1DATASTORE_IPAD;
    break;
  case NETWOX_ASN1TAG_BITSTRING :
    pasn1data->asn1datastore = NETWOX_ASN1DATASTORE_BITSTRING;
    break;
  case NETWOX_ASN1TAG_OID :
    pasn1data->asn1datastore = NETWOX_ASN1DATASTORE_OID;
    break;
  case NETWOX_ASN1TAG_SEQUENCE :
  case NETWOX_ASN1TAG_SNMPPDU_GETREQUEST :
  case NETWOX_ASN1TAG_SNMPPDU_GETNEXTREQUEST :
  case NETWOX_ASN1TAG_SNMPPDU_RESPONSE :
  case NETWOX_ASN1TAG_SNMPPDU_SETREQUEST :
  case NETWOX_ASN1TAG_SNMPPDU_TRAP :
  case NETWOX_ASN1TAG_SNMPPDU_GETBULKREQUEST :
  case NETWOX_ASN1TAG_SNMPPDU_INFORMREQUEST :
  case NETWOX_ASN1TAG_SNMPPDU_TRAP2 :
  case NETWOX_ASN1TAG_SNMPPDU_REPORT :
    pasn1data->asn1datastore = NETWOX_ASN1DATASTORE_SEQUENCE;
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }
  pasn1data->asn1tag = asn1tag;

  switch (pasn1data->asn1datastore) {
  case NETWOX_ASN1DATASTORE_NOTHING :
  case NETWOX_ASN1DATASTORE_B :
  case NETWOX_ASN1DATASTORE_I :
  case NETWOX_ASN1DATASTORE_UI32 :
  case NETWOX_ASN1DATASTORE_UI64 :
    /* nothing to initialize */
    break;
  case NETWOX_ASN1DATASTORE_IPAD :
    netwib_er(netwib_ip_init_ip4(0, &pasn1data->value.ipad));
    break;
  case NETWOX_ASN1DATASTORE_UNKNOWN :
    /* values
         pasn1data->value.unknown.cl
         pasn1data->value.unknown.constructed
         pasn1data->value.unknown.num
       must be set by user
    */
    netwib_er(netwib_bufpool_buf_init(pasn1data->pbufpool,
                                      &pasn1data->value.unknown.pbuf));
    break;
  case NETWOX_ASN1DATASTORE_BITSTRING :
    pasn1data->value.bitstring.numbits = 0;
    netwib_er(netwib_bufpool_buf_init(pasn1data->pbufpool,
                                      &pasn1data->value.bitstring.pbuf));
    break;
  case NETWOX_ASN1DATASTORE_BUF :
    netwib_er(netwib_bufpool_buf_init(pasn1data->pbufpool,
                                      &pasn1data->value.pbuf));
    break;
  case NETWOX_ASN1DATASTORE_OID :
    netwib_er(netwox_asn1oid_init(pasn1data->pbufpool, &pasn1data->value.oid));
    break;
  case NETWOX_ASN1DATASTORE_SEQUENCE :
    netwib_er(netwib_array_init(sizeof(netwox_asn1data), 0,
                                &pasn1data->value.sequence.seq));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1data_add_oid(netwox_asn1data *pasn1data,
                                   netwib_uint32 subid)
{

  if (pasn1data->asn1datastore != NETWOX_ASN1DATASTORE_OID) {
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  netwib_er(netwox_asn1oid_add_id(&pasn1data->value.oid, subid));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1data_add_oid_buf(netwox_asn1data *pasn1data,
                                       netwib_constbuf *pbuf)
{

  if (pasn1data->asn1datastore != NETWOX_ASN1DATASTORE_OID) {
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  netwib_er(netwox_buf_decode_asn1oid(pbuf, &pasn1data->value.oid));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1data_add_sequence(netwox_asn1data *pasn1data,
                                        netwox_constasn1data *pasn1datatoadd)
{
  netwox_asn1data *pasn1datai;

  if (pasn1data->asn1datastore != NETWOX_ASN1DATASTORE_SEQUENCE) {
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  netwib_er(netwib_array_ctl_set_size(&pasn1data->value.sequence.seq,
                                      pasn1data->value.sequence.seq.size+1));
  pasn1datai = (netwox_asn1data*)pasn1data->value.sequence.seq.p[pasn1data->value.sequence.seq.size-1];
  *pasn1datai = *pasn1datatoadd;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1data_dup(netwox_constasn1data *pasn1datasrc,
                               netwox_asn1data *pasn1datadst)
{
  netwox_asn1data asn1data;
  netwib_uint32 i;
  netwib_err ret;

  netwib_er(netwox_asn1data_set_type(pasn1datadst, pasn1datasrc->asn1tag));

  switch (pasn1datasrc->asn1datastore) {
  case NETWOX_ASN1DATASTORE_NOTHING :
    break;
  case NETWOX_ASN1DATASTORE_B :
    pasn1datadst->value.b = pasn1datasrc->value.b;
    break;
  case NETWOX_ASN1DATASTORE_I :
    pasn1datadst->value.i = pasn1datasrc->value.i;
    break;
  case NETWOX_ASN1DATASTORE_UI32 :
    pasn1datadst->value.ui32 = pasn1datasrc->value.ui32;
    break;
  case NETWOX_ASN1DATASTORE_UI64 :
    pasn1datadst->value.ui64 = pasn1datasrc->value.ui64;
    break;
  case NETWOX_ASN1DATASTORE_IPAD :
    pasn1datadst->value.ipad= pasn1datasrc->value.ipad;
    break;
  case NETWOX_ASN1DATASTORE_UNKNOWN :
    pasn1datadst->value.unknown.cl = pasn1datasrc->value.unknown.cl;
    pasn1datadst->value.unknown.constructed = pasn1datasrc->value.unknown.constructed;
    pasn1datadst->value.unknown.num = pasn1datasrc->value.unknown.num;
    netwib_er(netwib_buf_append_buf(pasn1datasrc->value.unknown.pbuf,
                                    pasn1datadst->value.unknown.pbuf));
    break;
  case NETWOX_ASN1DATASTORE_BITSTRING :
    pasn1datadst->value.bitstring.numbits = pasn1datasrc->value.bitstring.numbits;
    netwib_er(netwib_buf_append_buf(pasn1datasrc->value.bitstring.pbuf,
                                    pasn1datadst->value.bitstring.pbuf));
    break;
  case NETWOX_ASN1DATASTORE_BUF :
    netwib_er(netwib_buf_append_buf(pasn1datasrc->value.pbuf,
                                    pasn1datadst->value.pbuf));
    break;
  case NETWOX_ASN1DATASTORE_OID :
    netwib_er(netwox_asn1oid_dup(&pasn1datasrc->value.oid,
                                 &pasn1datadst->value.oid));
    break;
  case NETWOX_ASN1DATASTORE_SEQUENCE :
    for (i = 0; i < pasn1datasrc->value.sequence.seq.size; i++) {
      netwib_er(netwox_asn1data_init(pasn1datasrc->pbufpool, &asn1data));
      ret = netwox_asn1data_dup((netwox_asn1data*)pasn1datasrc->value.sequence.seq.p[i], &asn1data);
      if (ret != NETWIB_ERR_OK) {
        netwib_er(netwox_asn1data_close(&asn1data));
        return(ret);
      }
      netwib_er(netwox_asn1data_add_sequence(pasn1datadst, &asn1data));
    }
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1data_init_arg(netwib_bufpool *pbufpool,
                                    netwox_arg *parg,
                                    netwib_char key1,
                                    netwib_char key2,
                                    netwox_asn1data *pasn1data)
{
  netwib_buf buf1, buf2;
  netwib_char type;

  netwib_er(netwox_asn1data_init(pbufpool, pasn1data));

  netwib_er(netwox_arg_buf(parg, key1, &buf1));
  netwib_er(netwox_arg_buf(parg, key2, &buf2));

  type = '-';
  if (netwib__buf_ref_data_size(&buf1) == 1) {
    type = *(netwib_char*)netwib__buf_ref_data_ptr(&buf1);
  }

  switch(type) {
  case 'i' :
    netwib_er(netwox_asn1data_set_type(pasn1data, NETWOX_ASN1TAG_INTEGER));
    netwib_er(netwib_buf_decode_fmt(&buf2, "%{uintmax}%$",
                                    &pasn1data->value.i));
    break;
  case 's' :
    netwib_er(netwox_asn1data_set_type(pasn1data, NETWOX_ASN1TAG_OCTETSTRING));
    netwib_er(netwib_buf_append_buf(&buf2, pasn1data->value.pbuf));
    break;
  case 'm' :
    netwib_er(netwox_asn1data_set_type(pasn1data, NETWOX_ASN1TAG_OCTETSTRING));
    netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_MIXED,
                                pasn1data->value.pbuf));
    break;
  case 'n' :
    netwib_er(netwox_asn1data_set_type(pasn1data, NETWOX_ASN1TAG_NULL));
    break;
  case 'o' :
    netwib_er(netwox_asn1data_set_type(pasn1data, NETWOX_ASN1TAG_OID));
    netwib_er(netwox_buf_decode_asn1oid(&buf2, &pasn1data->value.oid));
    break;
  case 'a' :
    netwib_er(netwox_asn1data_set_type(pasn1data, NETWOX_ASN1TAG_IP4));
    netwib_er(netwib_ip_init_buf(&buf2, NETWIB_IP_DECODETYPE_BEST,
                                 &pasn1data->value.ipad));
    break;
  case 'c' :
    netwib_er(netwox_asn1data_set_type(pasn1data, NETWOX_ASN1TAG_COUNTER32));
    netwib_er(netwib_buf_decode_fmt(&buf2, "%{uint32}%$",
                                    &pasn1data->value.ui32));
    break;
  case 'g' :
    netwib_er(netwox_asn1data_set_type(pasn1data, NETWOX_ASN1TAG_GAUGE32));
    netwib_er(netwib_buf_decode_fmt(&buf2, "%{uint32}%$",
                                    &pasn1data->value.ui32));
    break;
  case 't' :
    netwib_er(netwox_asn1data_set_type(pasn1data, NETWOX_ASN1TAG_TIMETICKS));
    netwib_er(netwib_buf_decode_fmt(&buf2, "%{uint32}%$",
                                    &pasn1data->value.ui32));
    break;
  case 'M' :
    netwib_er(netwox_asn1data_set_type(pasn1data, NETWOX_ASN1TAG_OPAQUE));
    netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_MIXED,
                                pasn1data->value.pbuf));
    break;
  case 'u' :
    netwib_er(netwox_asn1data_set_type(pasn1data, NETWOX_ASN1TAG_UINT32));
    netwib_er(netwib_buf_decode_fmt(&buf2, "%{uint32}%$",
                                    &pasn1data->value.ui32));
    break;
  case 'U' :
    netwib_er(netwox_asn1data_set_type(pasn1data, NETWOX_ASN1TAG_COUNTER64));
    netwib_er(netwib_buf_decode_fmt(&buf2, "%{uint64}%$",
                                    &pasn1data->value.ui64));
    break;
  default :
    netwib_er(netwib_fmt_display("Supported ASN.1 types are: i=integer, s=octetstring, m=mixedoctetstring, n=null, o=oid, a=ipaddress, c=counter32, g=gauge32, t=timeticks, M=mixedopaque, u=uint32, U=uint64\n"));
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}
