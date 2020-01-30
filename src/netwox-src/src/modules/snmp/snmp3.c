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
netwib_err netwox_buf_append_snmp3_flags(netwib_uint32 flags,
                                         netwib_buf *pbuf)
{
  if (flags & NETWOX_SNMP3_FLAGS_REPORTABLE) {
    netwib_er(netwib_buf_append_string("reportable,", pbuf));
  } else {
    netwib_er(netwib_buf_append_string("!reportable,", pbuf));
  }
  if (flags & NETWOX_SNMP3_FLAGS_PRIV) {
    netwib_er(netwib_buf_append_string("priv,", pbuf));
  } else {
    netwib_er(netwib_buf_append_string("!priv,", pbuf));
  }
  if (flags & NETWOX_SNMP3_FLAGS_AUTH) {
    netwib_er(netwib_buf_append_string("auth", pbuf));
  } else {
    netwib_er(netwib_buf_append_string("!auth", pbuf));
  }


  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_buf_append_snmp3_securitymodel(netwox_snmp3_securitymodel sm,
                                                 netwib_buf *pbuf)
{
  netwib_conststring pc;

  pc = NULL;
  switch(sm) {
  case NETWOX_SNMP3_SECURITYMODEL_ANY :
    pc = "any";
    break;
  case NETWOX_SNMP3_SECURITYMODEL_1 :
    pc = "1";
    break;
  case NETWOX_SNMP3_SECURITYMODEL_2C :
    pc = "2c";
    break;
  case NETWOX_SNMP3_SECURITYMODEL_USM :
    pc = "usm";
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
netwib_err netwox_snmp3usmsp_init(netwib_bufpool *pbufpool,
                                  netwox_snmp3usmsp *pusmsp)
{
  pusmsp->pbufpool = pbufpool;
  netwib_er(netwib_bufpool_buf_init(pusmsp->pbufpool,
                                    &pusmsp->pauthoritativeengineid));
  pusmsp->authoritativeengineboots = 0;
  pusmsp->authoritativeenginetime = 0;
  netwib_er(netwib_bufpool_buf_init(pusmsp->pbufpool,
                                    &pusmsp->pusername));
  netwib_er(netwib_bufpool_buf_init(pusmsp->pbufpool,
                                    &pusmsp->pauthenticationparameters));
  netwib_er(netwib_bufpool_buf_init(pusmsp->pbufpool,
                                    &pusmsp->pprivacyparameters));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmp3usmsp_close(netwox_snmp3usmsp *pusmsp)
{
  netwib_er(netwib_bufpool_buf_close(pusmsp->pbufpool,
                                     &pusmsp->pauthoritativeengineid));
  netwib_er(netwib_bufpool_buf_close(pusmsp->pbufpool,
                                     &pusmsp->pusername));
  netwib_er(netwib_bufpool_buf_close(pusmsp->pbufpool,
                                     &pusmsp->pauthenticationparameters));
  netwib_er(netwib_bufpool_buf_close(pusmsp->pbufpool,
                                     &pusmsp->pprivacyparameters));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1data_append_snmp3usmsp(netwox_constsnmp3usmsp *pusmsp,
                                             netwox_asn1data *pasn1data)
{
  netwox_asn1data asn1data;

  netwib_er(netwox_asn1data_set_type(pasn1data, NETWOX_ASN1TAG_SEQUENCE));

  netwox__asn1data_append_octetstring_dup(pasn1data,
                                          pusmsp->pauthoritativeengineid);
  netwox__asn1data_append_integer(pasn1data, pusmsp->authoritativeengineboots);
  netwox__asn1data_append_integer(pasn1data, pusmsp->authoritativeenginetime);
  netwox__asn1data_append_octetstring_dup(pasn1data,
                                          pusmsp->pusername);
  netwox__asn1data_append_octetstring_dup(pasn1data,
                                          pusmsp->pauthenticationparameters);
  netwox__asn1data_append_octetstring_dup(pasn1data,
                                          pusmsp->pprivacyparameters);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1data_decode_snmp3usmsp(netwox_constasn1data *pasn1data,
                                             netwox_snmp3usmsp *pusmsp)
{
  netwox_asn1data **plevel1;

  netwox__asn1data_decode_sequence(pasn1data, plevel1);
  netwox__asn1data_check_sequence_size(pasn1data, 6);

  netwox__asn1data_decode_octetstring_dup(plevel1[0],
                                          pusmsp->pauthoritativeengineid);
  netwox__asn1data_decode_integert(plevel1[1], netwib_uint32,
                                   pusmsp->authoritativeengineboots);
  netwox__asn1data_decode_integert(plevel1[2], netwib_uint32,
                                   pusmsp->authoritativeenginetime);
  netwox__asn1data_decode_octetstring_dup(plevel1[3],
                                          pusmsp->pusername);
  netwox__asn1data_decode_octetstring_dup(plevel1[4],
                                          pusmsp->pauthenticationparameters);
  netwox__asn1data_decode_octetstring_dup(plevel1[5],
                                          pusmsp->pprivacyparameters);
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmp3usmsp_show(netwox_constsnmp3usmsp *pusmsp,
                                   netwib_buf *pbuf)
{
  netwib_er(netwib_show_array_fmt32(pbuf, " securityparameters:"));

  netwib_er(netwib_show_array_data("  authoritativeengineid",
                                   pusmsp->pauthoritativeengineid,
                                   NETWIB_ENCODETYPE_HEXA0, ' ', pbuf));
  netwib_er(netwib_show_array_fmt32(pbuf,
                                    "  authoritativeengineboots: %{uint32}",
                                    pusmsp->authoritativeengineboots));
  netwib_er(netwib_show_array_fmt32(pbuf,
                                    "  authoritativeenginetime: %{uint32}",
                                    pusmsp->authoritativeenginetime));
  netwib_er(netwib_show_array_data("  username", pusmsp->pusername,
                                   NETWIB_ENCODETYPE_MIXED, ' ', pbuf));
  netwib_er(netwib_show_array_data("  authenticationparameters",
                                   pusmsp->pauthenticationparameters,
                                   NETWIB_ENCODETYPE_HEXA0, ' ', pbuf));
  netwib_er(netwib_show_array_data("  privacyparameters",
                                   pusmsp->pprivacyparameters,
                                   NETWIB_ENCODETYPE_HEXA0, ' ', pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmp3scopedpdu_init(netwib_bufpool *pbufpool,
                                      netwox_snmp3scopedpdu *pusmsp)
{
  pusmsp->pbufpool = pbufpool;
  netwib_er(netwib_bufpool_buf_init(pusmsp->pbufpool,
                                    &pusmsp->pcontextengineid));
  netwib_er(netwib_bufpool_buf_init(pusmsp->pbufpool, &pusmsp->pcontextname));
  netwib_er(netwox_snmppdu_init(pusmsp->pbufpool, &pusmsp->snmppdu));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmp3scopedpdu_close(netwox_snmp3scopedpdu *pusmsp)
{
  netwib_er(netwib_bufpool_buf_close(pusmsp->pbufpool,
                                     &pusmsp->pcontextengineid));
  netwib_er(netwib_bufpool_buf_close(pusmsp->pbufpool, &pusmsp->pcontextname));
  netwib_er(netwox_snmppdu_close(&pusmsp->snmppdu));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1data_append_snmp3scopedpdu(netwox_constsnmp3scopedpdu *pusmsp,
                                                 netwox_asn1data *pasn1data)
{
  netwox_asn1data asn1data, asn1data1;

  netwib_er(netwox_asn1data_set_type(pasn1data, NETWOX_ASN1TAG_SEQUENCE));
  netwox__asn1data_append_octetstring_dup(pasn1data, pusmsp->pcontextengineid);
  netwox__asn1data_append_octetstring_dup(pasn1data, pusmsp->pcontextname);
  netwib_er(netwox_asn1data_init(pasn1data->pbufpool, &asn1data1));
  netwib_er(netwox_asn1data_append_snmppdu(&pusmsp->snmppdu, &asn1data1));
  netwib_er(netwox_asn1data_add_sequence(pasn1data, &asn1data1));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1data_decode_snmp3scopedpdu(netwox_constasn1data *pasn1data,
                                                netwox_snmp3scopedpdu *pusmsp)
{
  netwox_asn1data **plevel1;

  netwox__asn1data_decode_sequence(pasn1data, plevel1);
  netwox__asn1data_check_sequence_size(pasn1data, 3);
  netwox__asn1data_decode_octetstring_dup(plevel1[0],
                                          pusmsp->pcontextengineid);
  netwox__asn1data_decode_octetstring_dup(plevel1[1], pusmsp->pcontextname);
  netwib_er(netwox_asn1data_decode_snmppdu(plevel1[2], &pusmsp->snmppdu));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmp3scopedpdu_show(netwox_constsnmp3scopedpdu *pusmsp,
                                      netwib_buf *pbuf)
{
  netwib_er(netwib_show_array_data(" contextengineid",
                                   pusmsp->pcontextengineid,
                                   NETWIB_ENCODETYPE_HEXA0,
                                   ' ', pbuf));
  netwib_er(netwib_show_array_data(" contextname", pusmsp->pcontextname,
                                   NETWIB_ENCODETYPE_MIXED,
                                   ' ', pbuf));
  netwib_er(netwox_snmppdu_show(&pusmsp->snmppdu, pbuf));
  return(NETWIB_ERR_OK);
}
