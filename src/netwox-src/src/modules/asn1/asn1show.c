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
static netwib_err netwox_priv_show_asn1data_synth(netwox_constasn1data *pasn1data,
                                                  netwib_buf *pbuf)
{
  netwib_byte array[80];
  netwib_buf buf;
  netwib_string fmt;
  netwib_data data;
  netwib_byte b;
  netwib_uint32 i, j;

  switch (pasn1data->asn1datastore) {
  case NETWOX_ASN1DATASTORE_NOTHING :
    netwib_er(netwib_buf_append_string("nothing", pbuf));
    break;
  case NETWOX_ASN1DATASTORE_B :
    netwib_er(netwib_buf_append_fmt(pbuf, "%{bool}", pasn1data->value.b));
    break;
  case NETWOX_ASN1DATASTORE_I :
    netwib_er(netwib_buf_append_fmt(pbuf, "%{intmax}", pasn1data->value.i));
    break;
  case NETWOX_ASN1DATASTORE_UI32 :
    netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32}", pasn1data->value.ui32));
    break;
  case NETWOX_ASN1DATASTORE_UI64 :
    netwib_er(netwib_buf_append_fmt(pbuf, "%{uint64}", pasn1data->value.ui64));
    break;
  case NETWOX_ASN1DATASTORE_IPAD :
    netwib_er(netwib_buf_append_fmt(pbuf, "%{ip}",
                                    &pasn1data->value.ipad));
    break;
  case NETWOX_ASN1DATASTORE_UNKNOWN :
    netwib_er(netwib_buf_append_fmt(pbuf, "cl=%{uint32} num=%{uint32} ",
                                    pasn1data->value.unknown.cl,
                                    pasn1data->value.unknown.num));
    netwib_er(netwib_buf_encode(pasn1data->value.unknown.pbuf,
                                NETWIB_ENCODETYPE_HEXA0, pbuf));
    break;
  case NETWOX_ASN1DATASTORE_BITSTRING :
    data = netwib__buf_ref_data_ptr(pasn1data->value.bitstring.pbuf);
    j = pasn1data->value.bitstring.numbits / 8;
    for (i = 0; i < j; i++) {
      netwib_er(netwib_buf_append_fmt(pbuf, "%{byte:8b}", *data++));
    }
    j = pasn1data->value.bitstring.numbits - 8*j;
    if (j) {
      b = *data;
      b = (netwib_byte)(b >> (8 - j));
      netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));
      netwib_er(netwib_buf_append_fmt(&buf, "%%{byte:%{uint32}b}", j));
      netwib_er(netwib_buf_ref_string(&buf, &fmt));
      netwib_er(netwib_buf_append_fmt(pbuf, fmt, b));
    }
    break;
  case NETWOX_ASN1DATASTORE_BUF :
    if (pasn1data->asn1tag == NETWOX_ASN1TAG_OCTETSTRING) {
      netwib_er(netwib_buf_encode(pasn1data->value.pbuf,
                                  NETWIB_ENCODETYPE_MIXED0, pbuf));
    } else {
      netwib_er(netwib_buf_encode(pasn1data->value.pbuf,
                                  NETWIB_ENCODETYPE_HEXA0, pbuf));
    }
    break;
  case NETWOX_ASN1DATASTORE_OID :
    netwib_er(netwox_buf_append_asn1oid(&pasn1data->value.oid, pbuf));
    break;
  case NETWOX_ASN1DATASTORE_SEQUENCE :
    netwib_er(netwib_buf_append_string("sequence", pbuf));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_show_asn1data_array(netwox_constasn1data *pasn1data,
                                                  netwib_buf *pleadingspaces,
                                                  netwib_buf *pbuf)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_uint32 i, leadingspaces;
  netwib_string leading;
  netwib_conststring pc;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));
  ret = netwox_priv_show_asn1data_synth(pasn1data, &buf);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_buf_close(&buf));
    return(ret);
  }

  leadingspaces = netwib__buf_ref_data_size(pleadingspaces);
  netwib_er(netwib_buf_append_byte(' ', pleadingspaces));
  pc = NULL;
  switch (pasn1data->asn1tag) {
    /* sequences */
  case NETWOX_ASN1TAG_SEQUENCE :
    pc = "sequence";
    break;
  case NETWOX_ASN1TAG_SNMPPDU_GETREQUEST :
    pc = "snmp_pdu_getrequest";
    break;
  case NETWOX_ASN1TAG_SNMPPDU_GETNEXTREQUEST :
    pc = "snmp_pdu_getnextrequest";
    break;
  case NETWOX_ASN1TAG_SNMPPDU_RESPONSE :
    pc = "snmp_pdu_response";
    break;
  case NETWOX_ASN1TAG_SNMPPDU_SETREQUEST :
    pc = "snmp_pdu_setrequest";
    break;
  case NETWOX_ASN1TAG_SNMPPDU_TRAP :
    pc = "snmp_pdu_trap";
    break;
  case NETWOX_ASN1TAG_SNMPPDU_GETBULKREQUEST :
    pc = "snmp_pdu_getbulkrequest";
    break;
  case NETWOX_ASN1TAG_SNMPPDU_INFORMREQUEST :
    pc = "snmp_pdu_informrequest";
    break;
  case NETWOX_ASN1TAG_SNMPPDU_TRAP2 :
    pc = "snmp_pdu_trap2";
    break;
  case NETWOX_ASN1TAG_SNMPPDU_REPORT :
    pc = "snmp_pdu_report";
    break;
    /* not sequences */
  case NETWOX_ASN1TAG_END :
    netwib_er(netwib_buf_append_string("end", pleadingspaces));
    break;
  case NETWOX_ASN1TAG_BOOL :
    netwib_er(netwib_buf_append_string("bool", pleadingspaces));
    break;
  case NETWOX_ASN1TAG_INTEGER :
    netwib_er(netwib_buf_append_string("integer", pleadingspaces));
    break;
  case NETWOX_ASN1TAG_UNKNOWN :
    netwib_er(netwib_buf_append_string("unknown", pleadingspaces));
    break;
  case NETWOX_ASN1TAG_BITSTRING :
    netwib_er(netwib_buf_append_string("bitstring", pleadingspaces));
    break;
  case NETWOX_ASN1TAG_OCTETSTRING :
    netwib_er(netwib_buf_append_string("octetstring", pleadingspaces));
    break;
  case NETWOX_ASN1TAG_NULL :
    netwib_er(netwib_buf_append_string("null", pleadingspaces));
    break;
  case NETWOX_ASN1TAG_OID :
    netwib_er(netwib_buf_append_string("oid", pleadingspaces));
    break;
  case NETWOX_ASN1TAG_IP4 :
    netwib_er(netwib_buf_append_string("ip", pleadingspaces));
    break;
  case NETWOX_ASN1TAG_COUNTER32 :
    netwib_er(netwib_buf_append_string("counter32", pleadingspaces));
    break;
  case NETWOX_ASN1TAG_GAUGE32 :
    netwib_er(netwib_buf_append_string("gauge32", pleadingspaces));
    break;
  case NETWOX_ASN1TAG_TIMETICKS :
    netwib_er(netwib_buf_append_string("timeticks", pleadingspaces));
    break;
  case NETWOX_ASN1TAG_OPAQUE :
    netwib_er(netwib_buf_append_string("opaque", pleadingspaces));
    break;
  case NETWOX_ASN1TAG_NSAP :
    netwib_er(netwib_buf_append_string("nsap", pleadingspaces));
    break;
  case NETWOX_ASN1TAG_UINT32OLD :
    netwib_er(netwib_buf_append_string("uint32old", pleadingspaces));
    break;
  case NETWOX_ASN1TAG_UINT32 :
    netwib_er(netwib_buf_append_string("uint32", pleadingspaces));
    break;
  case NETWOX_ASN1TAG_COUNTER64 :
    netwib_er(netwib_buf_append_string("counter64", pleadingspaces));
    break;
  case NETWOX_ASN1TAG_SNMPVAR_NOSUCHOBJECT :
    netwib_er(netwib_buf_append_string("nosuchobject", pleadingspaces));
    break;
  case NETWOX_ASN1TAG_SNMPVAR_NOSUCHINSTANCE :
    netwib_er(netwib_buf_append_string("nosuchinstance", pleadingspaces));
    break;
  case NETWOX_ASN1TAG_SNMPVAR_ENDOFMIBVIEW :
    netwib_er(netwib_buf_append_string("endofmibview", pleadingspaces));
    break;
  default :
    netwib_er(netwib_buf_close(&buf));
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  if (pc != NULL) {
    netwib_er(netwib_show_array_fmt32(pbuf, "%{buf}%s_begin",
                                      pleadingspaces, pc));
    for (i = 0; i < pasn1data->value.sequence.seq.size; i++) {
      netwib_er(netwox_priv_show_asn1data_array((netwox_asn1data*)pasn1data->value.sequence.seq.p[i], pleadingspaces, pbuf));
    }
    netwib_er(netwib_show_array_fmt32(pbuf, "%{buf}%s_end",
                                      pleadingspaces, pc));
  } else {
    if (pasn1data->asn1datastore == NETWOX_ASN1DATASTORE_NOTHING) {
      netwib_er(netwib_show_array_fmt32(pbuf, "%{buf}", pleadingspaces));
    } else {
      netwib_er(netwib_buf_ref_string(pleadingspaces, &leading));
      netwib_er(netwib_show_array_data(leading, &buf,
                                       NETWIB_ENCODETYPE_DATA,
                                       ' ', pbuf));
    }
  }

  pleadingspaces->endoffset = pleadingspaces->beginoffset + leadingspaces;
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1data_show(netwox_constasn1data *pasn1data,
                                netwib_encodetype encodetype,
                                netwib_buf *pbuf)
{
  netwib_byte array[512];
  netwib_buf leadingspaces;
  netwib_err ret=NETWIB_ERR_OK;

  switch(encodetype) {
  case NETWIB_ENCODETYPE_SYNTH :
    ret = netwox_priv_show_asn1data_synth(pasn1data, pbuf);
    break;
  case NETWIB_ENCODETYPE_ARRAY :
    netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &leadingspaces));
    ret = netwox_priv_show_asn1data_array(pasn1data, &leadingspaces, pbuf);
    netwib_er(netwib_buf_close(&leadingspaces));
    break;
  default :
    ret = NETWIB_ERR_PAINVALIDTYPE;
    break;
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_asn1_show(netwib_bufpool *pbufpool,
                                netwox_asn1tag_ext ext,
                                netwib_constbuf *ppkt,
                                netwib_encodetype_context *pctx,
                                netwib_encodetype asn1encodetype,
                                netwib_encodetype dataencodetype,
                                netwib_buf *pbuf)
{
  netwox_asn1data asn1data;
  netwox_asn1tag_ctx asn1ctx;
  netwib_buf pkt;
  netwib_encodetype_context *pctxtouse, ctx;
  netwib_err ret;

  pctxtouse = pctx;
  if (pctx == NULL) {
    netwib_er(netwib_buf_encode_transition_init(&ctx));
    pctxtouse = &ctx;
  }

  switch(asn1encodetype) {
  case NETWIB_ENCODETYPE_SYNTH :
    netwib_er(netwib_buf_encode_transition(pctxtouse, asn1encodetype, pbuf));
    netwib_er(netwib_buf_append_string("asn1", pbuf));
    return(NETWIB_ERR_OK);
  case NETWIB_ENCODETYPE_ARRAY :
    break;
  default :
    netwib_er(netwib_pkt_data_show(ppkt, pctxtouse, asn1encodetype, pbuf));
    return(NETWIB_ERR_OK);
    break;
  }

  pkt = *ppkt;
  netwib_er(netwox_asn1data_init(pbufpool, &asn1data));
  netwib_er(netwox_asn1tag_ctx_init(&asn1ctx));
  ret = netwox_pktber_decode_asn1data(ext, &asn1ctx, &pkt, &asn1data);
  if (ret == NETWIB_ERR_NOTCONVERTED || ret == NETWOX_ERR_NOTSUPPORTED
      || ret == NETWIB_ERR_DATAMISSING || ret == NETWIB_ERR_LONOTIMPLEMENTED) {
    netwib_er(netwib_pkt_data_show(ppkt, pctxtouse, dataencodetype, pbuf));
    netwib_er(netwox_asn1tag_ctx_close(&asn1ctx));
    netwib_er(netwox_asn1data_close(&asn1data));
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_asn1tag_ctx_close(&asn1ctx));
    netwib_er(netwox_asn1data_close(&asn1data));
    return(ret);
  }

  netwib_er(netwib_buf_encode_transition(pctxtouse, asn1encodetype, pbuf));
  netwib_er(netwib_show_array_head("ASN.1", pbuf));
  netwib_er(netwox_asn1data_show(&asn1data, asn1encodetype, pbuf));
  netwib_er(netwib_show_array_tail(pbuf));

  if (netwib__buf_ref_data_size(&pkt)) {
    netwib_er(netwib_pkt_data_show(&pkt, pctxtouse, dataencodetype, pbuf));
  }

  netwib_er(netwox_asn1tag_ctx_close(&asn1ctx));
  netwib_er(netwox_asn1data_close(&asn1data));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_asn1_display(netwib_bufpool *pbufpool,
                                   netwox_asn1tag_ext ext,
                                   netwib_constbuf *ppkt,
                                   netwib_encodetype_context *pctx,
                                   netwib_encodetype asn1encodetype,
                                   netwib_encodetype dataencodetype)
{
  netwib_buf *pbuf;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
  ret = netwox_pkt_asn1_show(pbufpool, ext, ppkt, pctx, asn1encodetype,
                             dataencodetype, pbuf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_display(pbuf, NETWIB_ENCODETYPE_DATA));
  }
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(ret);
}
