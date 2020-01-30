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
netwib_err netwox_buf_append_snmppdutype(netwox_snmppdutype type,
                                         netwib_buf *pbuf)
{
  netwib_conststring pc;

  pc = NULL;
  switch(type) {
  case NETWOX_SNMPPDUTYPE_GETREQUEST :
    pc = "GetRequest";
    break;
  case NETWOX_SNMPPDUTYPE_GETNEXTREQUEST :
    pc = "GetNextRequest";
    break;
  case NETWOX_SNMPPDUTYPE_RESPONSE :
    pc = "Response";
    break;
  case NETWOX_SNMPPDUTYPE_SETREQUEST :
    pc = "SetRequest";
    break;
  case NETWOX_SNMPPDUTYPE_TRAP :
    pc = "Trap";
    break;
  case NETWOX_SNMPPDUTYPE_GETBULKREQUEST :
    pc = "GetBulkRequest";
    break;
  case NETWOX_SNMPPDUTYPE_INFORMREQUEST :
    pc = "InformRequest";
    break;
  case NETWOX_SNMPPDUTYPE_TRAP2 :
    pc = "Trap2";
    break;
  case NETWOX_SNMPPDUTYPE_REPORT :
    pc = "Report";
    break;
  default :
    pc = "Unknown";
    break;
  }

  if (pc != NULL) {
    netwib_er(netwib_buf_append_string(pc, pbuf));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_buf_append_snmppdu_pdu_errorstatus(netwox_snmppdu_pdu_errorstatus errorstatus,
                                                     netwib_buf *pbuf)
{
  netwib_conststring pc;

  pc = NULL;
  switch(errorstatus) {
  case NETWOX_SNMPPDU_PDU_ERRORSTATUS_NOERROR :
    pc = "no error";
    break;
  case NETWOX_SNMPPDU_PDU_ERRORSTATUS_TOOBIG :
    pc = "too big";
    break;
  case NETWOX_SNMPPDU_PDU_ERRORSTATUS_NOSUCHNAME :
    pc = "no such name";
    break;
  case NETWOX_SNMPPDU_PDU_ERRORSTATUS_BADVALUE :
    pc = "bad value";
    break;
  case NETWOX_SNMPPDU_PDU_ERRORSTATUS_READONLY :
    pc = "read only";
    break;
  case NETWOX_SNMPPDU_PDU_ERRORSTATUS_GENERR :
    pc = "gen err";
    break;
  case NETWOX_SNMPPDU_PDU_ERRORSTATUS_NOACCESS :
    pc = "no access";
    break;
  case NETWOX_SNMPPDU_PDU_ERRORSTATUS_WRONGTYPE :
    pc = "wrong type";
    break;
  case NETWOX_SNMPPDU_PDU_ERRORSTATUS_WRONGLENGTH :
    pc = "wrong length";
    break;
  case NETWOX_SNMPPDU_PDU_ERRORSTATUS_WRONGENCODING :
    pc = "wrong encoding";
    break;
  case NETWOX_SNMPPDU_PDU_ERRORSTATUS_WRONGVALUE :
    pc = "wrong value";
    break;
  case NETWOX_SNMPPDU_PDU_ERRORSTATUS_NOCREATION :
    pc = "no creation";
    break;
  case NETWOX_SNMPPDU_PDU_ERRORSTATUS_INCONSISTENTVALUE :
    pc = "inconsistent value";
    break;
  case NETWOX_SNMPPDU_PDU_ERRORSTATUS_RESOURCEUNAVAILABLE :
    pc = "resource unavailable";
    break;
  case NETWOX_SNMPPDU_PDU_ERRORSTATUS_COMMITFAILED :
    pc = "commit failed";
    break;
  case NETWOX_SNMPPDU_PDU_ERRORSTATUS_UNDOFAILED :
    pc = "undo failed";
    break;
  case NETWOX_SNMPPDU_PDU_ERRORSTATUS_AUTHORIZATIONERROR :
    pc = "authorization error";
    break;
  case NETWOX_SNMPPDU_PDU_ERRORSTATUS_NOTWRITABLE :
    pc = "not writable";
    break;
  case NETWOX_SNMPPDU_PDU_ERRORSTATUS_INCONSISTENTNAME :
    pc = "inconsistent name";
    break;
  default :
    pc = "unknown";
    break;
  }

  if (pc != NULL) {
    netwib_er(netwib_buf_append_string(pc, pbuf));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_buf_append_snmppdu_trappdu_type(netwox_snmppdu_trappdu_type type,
                                                  netwib_buf *pbuf)
{
  netwib_conststring pc;

  pc = NULL;
  switch(type) {
  case NETWOX_SNMPPDU_TRAPPDU_TYPE_COLDSTART :
    pc = "cold start";
    break;
  case NETWOX_SNMPPDU_TRAPPDU_TYPE_WARMSTART :
    pc = "warn start";
    break;
  case NETWOX_SNMPPDU_TRAPPDU_TYPE_LINKDOWN :
    pc = "link down";
    break;
  case NETWOX_SNMPPDU_TRAPPDU_TYPE_LINKUP :
    pc = "link up";
    break;
  case NETWOX_SNMPPDU_TRAPPDU_TYPE_AUTHENTICATIONFAILURE :
    pc = "authentication failure";
    break;
  case NETWOX_SNMPPDU_TRAPPDU_TYPE_EGPNEIGHBORLOSS :
    pc = "egp neighbor loss";
    break;
  case NETWOX_SNMPPDU_TRAPPDU_TYPE_ENTERPRISESPECIFIC :
    pc = "enterprise specific";
    break;
  default :
    pc = "unknown";
    break;
  }

  if (pc != NULL) {
    netwib_er(netwib_buf_append_string(pc, pbuf));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmppdu_init(netwib_bufpool *pbufpool,
                               netwox_snmppdu *psnmppdu)
{
  psnmppdu->pbufpool = pbufpool;
  psnmppdu->type = NETWOX_SNMPPDUTYPE_UNSET;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmppdu_close(netwox_snmppdu *psnmppdu)
{
  if (psnmppdu->type == NETWOX_SNMPPDUTYPE_UNSET) {
    return(NETWIB_ERR_OK);
  }

  switch(psnmppdu->type) {
  case NETWOX_SNMPPDUTYPE_GETREQUEST :
  case NETWOX_SNMPPDUTYPE_GETNEXTREQUEST :
  case NETWOX_SNMPPDUTYPE_RESPONSE :
  case NETWOX_SNMPPDUTYPE_SETREQUEST :
  case NETWOX_SNMPPDUTYPE_INFORMREQUEST :
  case NETWOX_SNMPPDUTYPE_TRAP2 :
  case NETWOX_SNMPPDUTYPE_REPORT :
    netwib_er(netwox_snmpvbl_close(&psnmppdu->pdu.pdu.vbl));
    break;
  case NETWOX_SNMPPDUTYPE_TRAP :
    netwib_er(netwox_asn1oid_close(&psnmppdu->pdu.trappdu.enterprise));
    netwib_er(netwox_snmpvbl_close(&psnmppdu->pdu.trappdu.vbl));
    break;
  case NETWOX_SNMPPDUTYPE_GETBULKREQUEST :
    netwib_er(netwox_snmpvbl_close(&psnmppdu->pdu.bulkpdu.vbl));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmppdu_set_type(netwox_snmppdu *psnmppdu,
                                   netwox_snmppdutype type)
{
  if (psnmppdu->type != NETWOX_SNMPPDUTYPE_UNSET) {
    netwib_er(netwox_snmppdu_close(psnmppdu));
  }

  switch(type) {
  case NETWOX_SNMPPDUTYPE_GETREQUEST :
  case NETWOX_SNMPPDUTYPE_GETNEXTREQUEST :
  case NETWOX_SNMPPDUTYPE_RESPONSE :
  case NETWOX_SNMPPDUTYPE_SETREQUEST :
  case NETWOX_SNMPPDUTYPE_INFORMREQUEST :
  case NETWOX_SNMPPDUTYPE_TRAP2 :
  case NETWOX_SNMPPDUTYPE_REPORT :
    netwib_er(netwib_uint32_init_rand_all(&psnmppdu->pdu.pdu.requestid));
    psnmppdu->pdu.pdu.errorstatus = NETWOX_SNMPPDU_PDU_ERRORSTATUS_NOERROR;
    psnmppdu->pdu.pdu.errorindex = 0;
    netwib_er(netwox_snmpvbl_init(psnmppdu->pbufpool, &psnmppdu->pdu.pdu.vbl));
    break;
  case NETWOX_SNMPPDUTYPE_TRAP :
    netwib_er(netwox_asn1oid_init(psnmppdu->pbufpool,
                                  &psnmppdu->pdu.trappdu.enterprise));
    netwib_er(netwib_ip_init_ip4(0, &psnmppdu->pdu.trappdu.agentaddr));
    psnmppdu->pdu.trappdu.type = NETWOX_SNMPPDU_TRAPPDU_TYPE_COLDSTART;
    psnmppdu->pdu.trappdu.specifictrap = 0;
    psnmppdu->pdu.trappdu.timestamp = 0;
    netwib_er(netwox_snmpvbl_init(psnmppdu->pbufpool,
                                  &psnmppdu->pdu.trappdu.vbl));
    break;
  case NETWOX_SNMPPDUTYPE_GETBULKREQUEST :
    netwib_er(netwib_uint32_init_rand_all(&psnmppdu->pdu.bulkpdu.requestid));
    psnmppdu->pdu.bulkpdu.nonrepeaters = 0;
    psnmppdu->pdu.bulkpdu.maxrepetitions = 0;
    netwib_er(netwox_snmpvbl_init(psnmppdu->pbufpool,
                                  &psnmppdu->pdu.bulkpdu.vbl));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }
  psnmppdu->type = type;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1data_append_snmppdu(netwox_constsnmppdu *psnmppdu,
                                          netwox_asn1data *pasn1data)
{
  netwox_asn1data asn1data, asn1data1;
  netwox_asn1tag asn1tag;

  switch(psnmppdu->type) {
  case NETWOX_SNMPPDUTYPE_GETREQUEST :
    asn1tag = NETWOX_ASN1TAG_SNMPPDU_GETREQUEST;
    break;
  case NETWOX_SNMPPDUTYPE_GETNEXTREQUEST :
    asn1tag = NETWOX_ASN1TAG_SNMPPDU_GETNEXTREQUEST;
    break;
  case NETWOX_SNMPPDUTYPE_RESPONSE :
    asn1tag = NETWOX_ASN1TAG_SNMPPDU_RESPONSE;
    break;
  case NETWOX_SNMPPDUTYPE_SETREQUEST :
    asn1tag = NETWOX_ASN1TAG_SNMPPDU_SETREQUEST;
    break;
  case NETWOX_SNMPPDUTYPE_INFORMREQUEST :
    asn1tag = NETWOX_ASN1TAG_SNMPPDU_INFORMREQUEST;
    break;
  case NETWOX_SNMPPDUTYPE_TRAP2 :
    asn1tag = NETWOX_ASN1TAG_SNMPPDU_TRAP2;
    break;
  case NETWOX_SNMPPDUTYPE_REPORT :
    asn1tag = NETWOX_ASN1TAG_SNMPPDU_REPORT;
    break;
  case NETWOX_SNMPPDUTYPE_TRAP :
    asn1tag = NETWOX_ASN1TAG_SNMPPDU_TRAP;
    break;
  case NETWOX_SNMPPDUTYPE_GETBULKREQUEST :
    asn1tag = NETWOX_ASN1TAG_SNMPPDU_GETBULKREQUEST;
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }
  netwib_er(netwox_asn1data_set_type(pasn1data, asn1tag));

  switch(psnmppdu->type) {
  case NETWOX_SNMPPDUTYPE_GETREQUEST :
  case NETWOX_SNMPPDUTYPE_GETNEXTREQUEST :
  case NETWOX_SNMPPDUTYPE_RESPONSE :
  case NETWOX_SNMPPDUTYPE_SETREQUEST :
  case NETWOX_SNMPPDUTYPE_INFORMREQUEST :
  case NETWOX_SNMPPDUTYPE_TRAP2 :
  case NETWOX_SNMPPDUTYPE_REPORT :
    netwox__asn1data_append_integer(pasn1data, psnmppdu->pdu.pdu.requestid);
    netwox__asn1data_append_integer(pasn1data, psnmppdu->pdu.pdu.errorstatus);
    netwox__asn1data_append_integer(pasn1data, psnmppdu->pdu.pdu.errorindex);
    netwib_er(netwox_asn1data_init(pasn1data->pbufpool, &asn1data1));
    netwib_er(netwox_asn1data_append_snmpvbl(&psnmppdu->pdu.pdu.vbl,
                                             &asn1data1));
    netwib_er(netwox_asn1data_add_sequence(pasn1data, &asn1data1));
    break;
  case NETWOX_SNMPPDUTYPE_TRAP :
    netwox__asn1data_append_oid_dup(pasn1data,
                                    &psnmppdu->pdu.trappdu.enterprise);
    netwox__asn1data_append_ip(pasn1data, psnmppdu->pdu.trappdu.agentaddr);
    netwox__asn1data_append_integer(pasn1data,
                                    psnmppdu->pdu.trappdu.type);
    netwox__asn1data_append_integer(pasn1data,
                                    psnmppdu->pdu.trappdu.specifictrap);
    netwox__asn1data_append_timeticks(pasn1data,
                                      psnmppdu->pdu.trappdu.timestamp);
    netwib_er(netwox_asn1data_init(pasn1data->pbufpool, &asn1data1));
    netwib_er(netwox_asn1data_append_snmpvbl(&psnmppdu->pdu.trappdu.vbl,
                                             &asn1data1));
    netwib_er(netwox_asn1data_add_sequence(pasn1data, &asn1data1));
    break;
  case NETWOX_SNMPPDUTYPE_GETBULKREQUEST :
    netwox__asn1data_append_integer(pasn1data,
                                    psnmppdu->pdu.bulkpdu.requestid);
    netwox__asn1data_append_integer(pasn1data,
                                    psnmppdu->pdu.bulkpdu.nonrepeaters);
    netwox__asn1data_append_integer(pasn1data,
                                    psnmppdu->pdu.bulkpdu.maxrepetitions);
    netwib_er(netwox_asn1data_init(pasn1data->pbufpool, &asn1data1));
    netwib_er(netwox_asn1data_append_snmpvbl(&psnmppdu->pdu.pdu.vbl,
                                             &asn1data1));
    netwib_er(netwox_asn1data_add_sequence(pasn1data, &asn1data1));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1data_decode_snmppdu(netwox_constasn1data *pasn1data,
                                          netwox_snmppdu *psnmppdu)
{
  netwox_asn1data **plevel1;
  netwox_snmppdutype snmppdutype;

  switch(pasn1data->asn1tag) {
  case NETWOX_ASN1TAG_SNMPPDU_GETREQUEST :
    snmppdutype = NETWOX_SNMPPDUTYPE_GETREQUEST;
    break;
  case NETWOX_ASN1TAG_SNMPPDU_GETNEXTREQUEST :
    snmppdutype = NETWOX_SNMPPDUTYPE_GETNEXTREQUEST;
    break;
  case NETWOX_ASN1TAG_SNMPPDU_RESPONSE :
    snmppdutype = NETWOX_SNMPPDUTYPE_RESPONSE;
    break;
  case NETWOX_ASN1TAG_SNMPPDU_SETREQUEST :
    snmppdutype = NETWOX_SNMPPDUTYPE_SETREQUEST;
    break;
  case NETWOX_ASN1TAG_SNMPPDU_TRAP :
    snmppdutype = NETWOX_SNMPPDUTYPE_TRAP;
    break;
  case NETWOX_ASN1TAG_SNMPPDU_GETBULKREQUEST :
    snmppdutype = NETWOX_SNMPPDUTYPE_GETBULKREQUEST;
    break;
  case NETWOX_ASN1TAG_SNMPPDU_INFORMREQUEST :
    snmppdutype = NETWOX_SNMPPDUTYPE_INFORMREQUEST;
    break;
  case NETWOX_ASN1TAG_SNMPPDU_TRAP2 :
    snmppdutype = NETWOX_SNMPPDUTYPE_TRAP2;
    break;
  case NETWOX_ASN1TAG_SNMPPDU_REPORT :
    snmppdutype = NETWOX_SNMPPDUTYPE_REPORT;
    break;
  default :
    return(NETWIB_ERR_NOTCONVERTED);
  }

  netwib_er(netwox_snmppdu_set_type(psnmppdu, snmppdutype));
  netwox__asn1data_decode_sequencestore(pasn1data, plevel1);
  switch(psnmppdu->type) {
  case NETWOX_SNMPPDUTYPE_GETREQUEST :
  case NETWOX_SNMPPDUTYPE_GETNEXTREQUEST :
  case NETWOX_SNMPPDUTYPE_RESPONSE :
  case NETWOX_SNMPPDUTYPE_SETREQUEST :
  case NETWOX_SNMPPDUTYPE_INFORMREQUEST :
  case NETWOX_SNMPPDUTYPE_TRAP2 :
  case NETWOX_SNMPPDUTYPE_REPORT :
    netwox__asn1data_check_sequence_size(pasn1data, 4);
    netwox__asn1data_decode_integert(plevel1[0], netwib_uint32,
                                     psnmppdu->pdu.pdu.requestid);
    netwox__asn1data_decode_integert(plevel1[1],
                                     netwox_snmppdu_pdu_errorstatus,
                                     psnmppdu->pdu.pdu.errorstatus);
    netwox__asn1data_decode_integert(plevel1[2], netwib_uint32,
                                     psnmppdu->pdu.pdu.errorindex);
    netwib_er(netwox_asn1data_decode_snmpvbl(plevel1[3],
                                             &psnmppdu->pdu.pdu.vbl));
    break;
  case NETWOX_SNMPPDUTYPE_TRAP :
    netwox__asn1data_check_sequence_size(pasn1data, 6);
    netwox__asn1data_decode_oid_dup(plevel1[0],
                                    &psnmppdu->pdu.trappdu.enterprise);
    netwox__asn1data_decode_ip(plevel1[1], psnmppdu->pdu.trappdu.agentaddr);
    netwox__asn1data_decode_integert(plevel1[2],
                                     netwox_snmppdu_trappdu_type,
                                     psnmppdu->pdu.trappdu.type);
    netwox__asn1data_decode_integert(plevel1[3],
                                     netwib_uint32,
                                     psnmppdu->pdu.trappdu.specifictrap);
    netwox__asn1data_decode_timeticks(plevel1[4],
                                      psnmppdu->pdu.trappdu.timestamp);
    netwib_er(netwox_asn1data_decode_snmpvbl(plevel1[5],
                                             &psnmppdu->pdu.trappdu.vbl));
    break;
  case NETWOX_SNMPPDUTYPE_GETBULKREQUEST :
    netwox__asn1data_check_sequence_size(pasn1data, 4);
    netwox__asn1data_decode_integert(plevel1[0], netwib_uint32,
                                     psnmppdu->pdu.bulkpdu.requestid);
    netwox__asn1data_decode_integert(plevel1[1], netwib_uint32,
                                     psnmppdu->pdu.bulkpdu.nonrepeaters);
    netwox__asn1data_decode_integert(plevel1[2], netwib_uint32,
                                     psnmppdu->pdu.bulkpdu.maxrepetitions);
    netwib_er(netwox_asn1data_decode_snmpvbl(plevel1[3],
                                             &psnmppdu->pdu.bulkpdu.vbl));
    break;
  default :
    return(NETWIB_ERR_NOTCONVERTED);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmppdu_show(netwox_constsnmppdu *psnmppdu,
                               netwib_buf *pbuf)
{
  netwib_byte array[80];
  netwib_buf buf;

  netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));

  netwib_er(netwox_buf_append_snmppdutype(psnmppdu->type, &buf));
  netwib_er(netwib_show_array_fmt32(pbuf, " %{buf}:", &buf));

  switch(psnmppdu->type) {
  case NETWOX_SNMPPDUTYPE_GETREQUEST :
  case NETWOX_SNMPPDUTYPE_GETNEXTREQUEST :
  case NETWOX_SNMPPDUTYPE_RESPONSE :
  case NETWOX_SNMPPDUTYPE_SETREQUEST :
  case NETWOX_SNMPPDUTYPE_INFORMREQUEST :
  case NETWOX_SNMPPDUTYPE_TRAP2 :
  case NETWOX_SNMPPDUTYPE_REPORT :
    netwib_er(netwib_show_array_fmt32(pbuf, "  requestid: %{uint32}",
                                      psnmppdu->pdu.pdu.requestid));
    netwib__buf_reinit(&buf);
    netwib_er(netwox_buf_append_snmppdu_pdu_errorstatus(psnmppdu->pdu.pdu.errorstatus, &buf));
    netwib_er(netwib_show_array_fmt32(pbuf,
                                      "  errorstatus: %{uint32} (%{buf})",
                                      psnmppdu->pdu.pdu.errorstatus, &buf));
    netwib_er(netwib_show_array_fmt32(pbuf, "  errorindex: %{uint32}",
                                      psnmppdu->pdu.pdu.errorindex));
    netwib_er(netwox_snmpvbl_show(&psnmppdu->pdu.pdu.vbl, pbuf));
    break;
  case NETWOX_SNMPPDUTYPE_TRAP :
    netwib__buf_reinit(&buf);
    buf.flags |= NETWIB_BUF_FLAGS_CANALLOC;
    netwib_er(netwox_buf_append_asn1oid(&psnmppdu->pdu.trappdu.enterprise,
                                        &buf));
    netwib_er(netwib_show_array_fmt32(pbuf, "  enterprise: %{buf}", &buf));
    netwib_er(netwib_show_array_fmt32(pbuf, "  agent: %{ip}",
                                    &psnmppdu->pdu.trappdu.agentaddr));
    netwib__buf_reinit(&buf);
    netwib_er(netwox_buf_append_snmppdu_trappdu_type(psnmppdu->pdu.trappdu.type, &buf));
    netwib_er(netwib_show_array_fmt32(pbuf, "  traptype: %{uint32} (%{buf})",
                                      psnmppdu->pdu.trappdu.type, &buf));
    netwib_er(netwib_show_array_fmt32(pbuf, "  specifictrap: %{uint32}",
                                      psnmppdu->pdu.trappdu.specifictrap));
    netwib_er(netwib_show_array_fmt32(pbuf, "  timestamp: %{uint32}",
                                      psnmppdu->pdu.trappdu.timestamp));
    netwib_er(netwox_snmpvbl_show(&psnmppdu->pdu.trappdu.vbl, pbuf));
    netwib_er(netwib_buf_close(&buf));
    break;
  case NETWOX_SNMPPDUTYPE_GETBULKREQUEST :
    netwib_er(netwib_show_array_fmt32(pbuf, "  requestid: %{uint32}",
                                      psnmppdu->pdu.bulkpdu.requestid));
    netwib_er(netwib_show_array_fmt32(pbuf, "  nonrepeaters: %{uint32}",
                                      psnmppdu->pdu.bulkpdu.nonrepeaters));
    netwib_er(netwib_show_array_fmt32(pbuf, "  maxrepetitions: %{uint32}",
                                      psnmppdu->pdu.bulkpdu.maxrepetitions));
    netwib_er(netwox_snmpvbl_show(&psnmppdu->pdu.bulkpdu.vbl, pbuf));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

