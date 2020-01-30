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
netwib_err netwox_buf_append_snmpversion(netwox_snmpversion version,
                                         netwib_buf *pbuf)
{
  netwib_conststring pc;

  pc = NULL;
  switch(version) {
  case NETWOX_SNMPVERSION_1 :
    pc = "1";
    break;
  case NETWOX_SNMPVERSION_2C :
    pc = "2c";
    break;
  case NETWOX_SNMPVERSION_2U :
    pc = "2u";
    break;
  case NETWOX_SNMPVERSION_3 :
    pc = "3";
    break;
  default :
    pc = "?";
    break;
  }

  if (pc != NULL) {
    netwib_er(netwib_buf_append_string(pc, pbuf));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmpmsg_init(netwib_bufpool *pbufpool,
                               netwox_snmpmsg *psnmpmsg)
{
  psnmpmsg->pbufpool = pbufpool;
  psnmpmsg->version = NETWOX_SNMPVERSION_UNSET;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmpmsg_close(netwox_snmpmsg *psnmpmsg)
{
  if (psnmpmsg->version == NETWOX_SNMPVERSION_UNSET) {
    return(NETWIB_ERR_OK);
  }

  switch(psnmpmsg->version) {
  case NETWOX_SNMPVERSION_1 :
  case NETWOX_SNMPVERSION_2C :
    netwib_er(netwib_bufpool_buf_close(psnmpmsg->pbufpool,
                                       &psnmpmsg->v.v12.pcommunity));
    netwib_er(netwox_snmppdu_close(&psnmpmsg->v.v12.snmppdu));
    break;
  case NETWOX_SNMPVERSION_3 :
    netwib_er(netwib_bufpool_buf_close(psnmpmsg->pbufpool,
                                       &psnmpmsg->v.v3.psecurityparameters));
    netwib_er(netwox_snmp3scopedpdu_close(&psnmpmsg->v.v3.clearpdu));
    netwib_er(netwib_bufpool_buf_close(psnmpmsg->pbufpool,
                                       &psnmpmsg->v.v3.pencryptedpdu));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmpmsg_set_type(netwox_snmpmsg *psnmpmsg,
                                   netwox_snmpversion version)
{
  if (psnmpmsg->version != NETWOX_SNMPVERSION_UNSET) {
    netwib_er(netwox_snmpmsg_close(psnmpmsg));
  }

  switch(version) {
  case NETWOX_SNMPVERSION_1 :
  case NETWOX_SNMPVERSION_2C :
    netwib_er(netwib_bufpool_buf_init(psnmpmsg->pbufpool,
                                      &psnmpmsg->v.v12.pcommunity));
    netwib_er(netwox_snmppdu_init(psnmpmsg->pbufpool,
                                  &psnmpmsg->v.v12.snmppdu));
    break;
  case NETWOX_SNMPVERSION_3 :
    netwib_er(netwib_uint32_init_rand_all(&psnmpmsg->v.v3.globalheader.id));
    psnmpmsg->v.v3.globalheader.maxsize = 1472; /* suppose MTU of 1500 */
    psnmpmsg->v.v3.globalheader.flags = 0;
    psnmpmsg->v.v3.globalheader.securitymodel = NETWOX_SNMP3_SECURITYMODEL_USM;
    netwib_er(netwib_bufpool_buf_init(psnmpmsg->pbufpool,
                                      &psnmpmsg->v.v3.psecurityparameters));
    netwib_er(netwox_snmp3scopedpdu_init(psnmpmsg->pbufpool,
                                         &psnmpmsg->v.v3.clearpdu));
    netwib_er(netwib_bufpool_buf_init(psnmpmsg->pbufpool,
                                      &psnmpmsg->v.v3.pencryptedpdu));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }
  psnmpmsg->version = version;


  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1data_append_snmpmsg(netwox_constsnmpmsg *psnmpmsg,
                                          netwox_asn1data *pasn1data)
{
  netwox_asn1data asn1data, asn1data1;
  netwib_buf buf;
  netwib_byte array[1];

  switch(psnmpmsg->version) {
  case NETWOX_SNMPVERSION_1 :
  case NETWOX_SNMPVERSION_2C :
  case NETWOX_SNMPVERSION_3 :
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  netwib_er(netwox_asn1data_set_type(pasn1data, NETWOX_ASN1TAG_SEQUENCE));
  netwox__asn1data_append_integer(pasn1data, psnmpmsg->version);

  switch(psnmpmsg->version) {
  case NETWOX_SNMPVERSION_1 :
  case NETWOX_SNMPVERSION_2C :
    netwox__asn1data_append_octetstring_dup(pasn1data,
                                            psnmpmsg->v.v12.pcommunity);
    netwib_er(netwox_asn1data_init(pasn1data->pbufpool, &asn1data1));
    netwib_er(netwox_asn1data_append_snmppdu(&psnmpmsg->v.v12.snmppdu,
                                             &asn1data1));
    netwib_er(netwox_asn1data_add_sequence(pasn1data, &asn1data1));
    break;
  case NETWOX_SNMPVERSION_3 :
    netwib_er(netwox_asn1data_init(pasn1data->pbufpool, &asn1data1));
    netwib_er(netwox_asn1data_set_type(&asn1data1, NETWOX_ASN1TAG_SEQUENCE));
    netwox__asn1data_append_integer(&asn1data1,
                                    psnmpmsg->v.v3.globalheader.id);
    netwox__asn1data_append_integer(&asn1data1,
                                    psnmpmsg->v.v3.globalheader.maxsize);
    netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));
    array[0] = (netwib_byte)psnmpmsg->v.v3.globalheader.flags;
    buf.endoffset++;
    netwox__asn1data_append_octetstring_dup(&asn1data1, &buf);
    netwox__asn1data_append_integer(&asn1data1,
                                    psnmpmsg->v.v3.globalheader.securitymodel);
    netwib_er(netwox_asn1data_add_sequence(pasn1data, &asn1data1));
    netwox__asn1data_append_octetstring_dup(pasn1data,
                                           psnmpmsg->v.v3.psecurityparameters);
    if (psnmpmsg->v.v3.globalheader.flags & NETWOX_SNMP3_FLAGS_PRIV) {
      netwox__asn1data_append_octetstring_dup(pasn1data,
                                              psnmpmsg->v.v3.pencryptedpdu);
    } else {
      netwib_er(netwox_asn1data_init(pasn1data->pbufpool, &asn1data1));
      netwib_er(netwox_asn1data_append_snmp3scopedpdu(&psnmpmsg->v.v3.clearpdu,
                                                      &asn1data1));
      netwib_er(netwox_asn1data_add_sequence(pasn1data, &asn1data1));
    }
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1data_decode_snmpmsg(netwox_constasn1data *pasn1data,
                                          netwox_snmpmsg *psnmpmsg)
{
  netwox_asn1data **plevel1, **plevel2;
  netwox_snmpversion version;
  netwib_buf *pbuf;
  netwib_byte *pc;
  netwib_err ret;

  netwox__asn1data_decode_sequence(pasn1data, plevel1);

  netwox__asn1data_check_sequence_minsize(pasn1data, 1);
  netwox__asn1data_decode_integert(plevel1[0], netwox_snmpversion, version);
  ret = netwox_snmpmsg_set_type(psnmpmsg, version);
  if (ret != NETWIB_ERR_OK) {
    if (ret == NETWIB_ERR_PAINVALIDTYPE) ret = NETWIB_ERR_NOTCONVERTED;
    return(ret);
  }

  switch(version) {
  case NETWOX_SNMPVERSION_1 :
  case NETWOX_SNMPVERSION_2C :
    netwox__asn1data_check_sequence_size(pasn1data, 3);
    netwox__asn1data_decode_octetstring_dup(plevel1[1],
                                            psnmpmsg->v.v12.pcommunity);
    netwib_er(netwox_asn1data_decode_snmppdu(plevel1[2],
                                             &psnmpmsg->v.v12.snmppdu));
    break;
  case NETWOX_SNMPVERSION_3 :
    netwox__asn1data_check_sequence_size(pasn1data, 4);
    netwox__asn1data_decode_sequence(plevel1[1], plevel2);
    netwox__asn1data_check_sequence_size(plevel1[1], 4);
    netwox__asn1data_decode_integert(plevel2[0], netwib_uint32,
                                     psnmpmsg->v.v3.globalheader.id);
    netwox__asn1data_decode_integert(plevel2[1], netwib_uint32,
                                     psnmpmsg->v.v3.globalheader.maxsize);
    netwox__asn1data_decode_octetstring(plevel2[2], pbuf);
    if (netwib__buf_ref_data_size(pbuf) != 1) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    pc = netwib__buf_ref_data_ptr(pbuf);
    psnmpmsg->v.v3.globalheader.flags = pc[0];
    netwox__asn1data_decode_integert(plevel2[3], netwib_uint32,
                                     psnmpmsg->v.v3.globalheader.securitymodel);
    netwox__asn1data_decode_octetstring_dup(plevel1[2],
                                       psnmpmsg->v.v3.psecurityparameters);
    if (psnmpmsg->v.v3.globalheader.flags & NETWOX_SNMP3_FLAGS_PRIV) {
      netwox__asn1data_decode_octetstring_dup(plevel1[3],
                                              psnmpmsg->v.v3.pencryptedpdu);
    } else {
      netwib_er(netwox_asn1data_decode_snmp3scopedpdu(plevel1[3],
                                                    &psnmpmsg->v.v3.clearpdu));
    }
    break;
  default :
    return(NETWIB_ERR_NOTCONVERTED);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmpmsg_show(netwox_constsnmpmsg *psnmpmsg,
                               netwib_encodetype encodetype,
                               netwib_buf *pbuf)
{
  netwox_snmp3usmsp usmsp;
  netwox_asn1data asn1data;
  netwib_byte array[80];
  netwib_buf buf, usp;
  netwib_err ret;

  if (encodetype == NETWIB_ENCODETYPE_SYNTH) {
    switch(psnmpmsg->version) {
    case NETWOX_SNMPVERSION_1 :
    case NETWOX_SNMPVERSION_2C :
      netwib_er(netwox_buf_append_snmppdutype(psnmpmsg->v.v12.snmppdu.type,
                                              pbuf));
      break;
    case NETWOX_SNMPVERSION_3 :
      if (psnmpmsg->v.v3.globalheader.flags & NETWOX_SNMP3_FLAGS_PRIV) {
        netwib_er(netwib_buf_append_string("SnmpEncrypted", pbuf));
      } else {
        netwib_er(netwox_buf_append_snmppdutype(psnmpmsg->v.v3.clearpdu.snmppdu.type, pbuf));
      }
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
    }
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));

  netwib_er(netwib_show_array_head("SNMP", pbuf));

  netwib_er(netwox_buf_append_snmpversion(psnmpmsg->version, &buf));
  netwib_er(netwib_show_array_fmt32(pbuf, " version: %{uint32} (SNMPv%{buf})",
                                    psnmpmsg->version, &buf));
  switch(psnmpmsg->version) {
  case NETWOX_SNMPVERSION_1 :
  case NETWOX_SNMPVERSION_2C :
    netwib_er(netwib_show_array_data(" community", psnmpmsg->v.v12.pcommunity,
                                     NETWIB_ENCODETYPE_MIXED,
                                     ' ', pbuf));
    netwib_er(netwox_snmppdu_show(&psnmpmsg->v.v12.snmppdu, pbuf));
    break;
  case NETWOX_SNMPVERSION_3 :
    netwib_er(netwib_show_array_fmt32(pbuf, " id: %{uint32}",
                                      psnmpmsg->v.v3.globalheader.id));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxsize: %{uint32}",
                                      psnmpmsg->v.v3.globalheader.maxsize));
    netwib__buf_reinit(&buf);
    netwib_er(netwox_buf_append_snmp3_flags(psnmpmsg->v.v3.globalheader.flags,
                                            &buf));
    netwib_er(netwib_show_array_fmt32(pbuf, " flags: %{uint32:08b} (%{buf})",
                                      psnmpmsg->v.v3.globalheader.flags,
                                      &buf));
    netwib__buf_reinit(&buf);
    netwib_er(netwox_buf_append_snmp3_securitymodel(psnmpmsg->v.v3.globalheader.securitymodel, &buf));
    netwib_er(netwib_show_array_fmt32(pbuf,
                                      " securitymodel: %{uint32} (%{buf})",
                                      psnmpmsg->v.v3.globalheader.securitymodel,
                                      &buf));
    ret = NETWIB_ERR_NOTCONVERTED;
    if (psnmpmsg->v.v3.globalheader.securitymodel == NETWOX_SNMP3_SECURITYMODEL_USM) {
      netwib_er(netwox_asn1data_init(psnmpmsg->pbufpool, &asn1data));
      netwib_er(netwox_snmp3usmsp_init(psnmpmsg->pbufpool, &usmsp));
      usp = *psnmpmsg->v.v3.psecurityparameters;
      ret = netwox_pktber_decode_asn1data_snmp3sp(&usp, &asn1data);
      if (ret == NETWIB_ERR_OK) {
        ret = netwox_asn1data_decode_snmp3usmsp(&asn1data, &usmsp);
        if (ret == NETWIB_ERR_OK) {
          netwib_er(netwox_snmp3usmsp_show(&usmsp, pbuf));
        }
      }
      netwib_er(netwox_snmp3usmsp_close(&usmsp));
      netwib_er(netwox_asn1data_close(&asn1data));
    }
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwib_show_array_data(" securityparameters",
                                       psnmpmsg->v.v3.psecurityparameters,
                                       NETWIB_ENCODETYPE_MIXED,
                                       ' ', pbuf));
    }
    if (psnmpmsg->v.v3.globalheader.flags & NETWOX_SNMP3_FLAGS_PRIV) {
      netwib_er(netwib_show_array_data(" encryptedpdu",
                                       psnmpmsg->v.v3.pencryptedpdu,
                                       NETWIB_ENCODETYPE_HEXA0,
                                       ' ', pbuf));
    } else {
      netwib_er(netwox_snmp3scopedpdu_show(&psnmpmsg->v.v3.clearpdu, pbuf));
    }
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  }

  netwib_er(netwib_show_array_tail(pbuf));

  return(NETWIB_ERR_OK);
}
