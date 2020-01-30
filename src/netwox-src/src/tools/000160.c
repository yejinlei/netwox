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
netwib_conststring t000160_description[] = {
  "This tool requests a WALK on a SNMP server. It permits to retrieve the",
  "value of one item, and of successive ones.",
  "",
  NETWOX_DESC_snmp_oid,
  "",
  NETWOX_DESC_snmp_common,
  "",
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000160_args[] = {
  NETWOX_SOCK_ARG_UDP_CLIPORT("161"),
  NETWOX_SNMP_ARG,
  NETWOX_TOOLARG_REQ_BUF_OID('q', "oid", "OID to query", ".0.1"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000160_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_UDP_SNMP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000160_info = {
  "SNMP Walk",
  t000160_description,
  "snmpwalk",
  t000160_args,
  t000160_nodes,
};

/*-------------------------------------------------------------*/
typedef struct {
  netwox_asn1oid oid;
  netwib_uint32 id;
} t000160_infos;

/*-------------------------------------------------------------*/
static netwib_err t000160_req(netwox_snmparg *psnmparg,
                              netwox_snmpmsg *psnmpmsg,
                              netwox_snmp3usmsp *psnmp3usmsp,
                              netwox_snmppdu *psnmppdu,
                              netwib_ptr infos)
{
  t000160_infos *pinfos = (t000160_infos *)infos;
  netwox_snmpvb snmpvb;

  netwib_er(netwox_snmppdu_set_type(psnmppdu,
                                    NETWOX_SNMPPDUTYPE_GETNEXTREQUEST));
  pinfos->id = psnmppdu->pdu.pdu.requestid;
  netwib_er(netwox_snmpvb_init(psnmparg->pbufpool, &snmpvb));
  netwib_er(netwox_asn1oid_dup(&pinfos->oid, &snmpvb.oid));
  netwib_er(netwox_asn1data_set_type(&snmpvb.data, NETWOX_ASN1TAG_NULL));
  netwib_er(netwox_snmpvbl_add(&psnmppdu->pdu.pdu.vbl, &snmpvb));

  psnmpmsg = psnmpmsg; /* for compiler warning */
  psnmp3usmsp = psnmp3usmsp; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000160_rep(netwox_snmparg *psnmparg,
                              netwox_snmpmsg *psnmpmsg,
                              netwox_snmp3usmsp *psnmp3usmsp,
                              netwox_snmppdu *psnmppdu,
                              netwib_ptr infos,
                              netwib_bool *pstoploop)
{
  t000160_infos *pinfos = (t000160_infos *)infos;
  netwox_snmpvbl *psnmpvbl;
  netwox_snmpvb *psnmpvb;
  netwib_buf buf;
  netwib_cmp cmp;

  if (psnmppdu->pdu.pdu.requestid != pinfos->id) {
    return(NETWIB_ERR_OK);
  }

  if (psnmppdu->type != NETWOX_SNMPPDUTYPE_RESPONSE) {
    return(NETWOX_ERR_SNMP_BADREPLY);
  }

  if (psnmppdu->pdu.pdu.errorstatus != NETWOX_SNMPPDU_PDU_ERRORSTATUS_NOERROR){
    /* SNMPv1 ends like this */
    return(NETWIB_ERR_DATAEND);
  }

  psnmpvbl = &psnmppdu->pdu.pdu.vbl;
  if (psnmpvbl->vb.size != 1) {
    return(NETWOX_ERR_SNMP_BADREPLY);
  }

  psnmpvb = (netwox_snmpvb *)psnmpvbl->vb.p[0];
  netwib_er(netwox_asn1oid_cmp(&psnmpvb->oid, &pinfos->oid, &cmp));
  if (cmp == NETWIB_CMP_EQ) {
    /* SNMPv2 ends like this */
    return(NETWIB_ERR_DATAEND);
  }

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwox_buf_append_asn1oid(&psnmpvb->oid, &buf));
  netwib_er(netwib_buf_append_string(": ", &buf));
  netwib_er(netwox_asn1data_show(&psnmpvb->data,
                                 NETWIB_ENCODETYPE_SYNTH, &buf));
  netwib_er(netwib_fmt_display("%{buf}\n", &buf));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwox_asn1oid_close(&pinfos->oid));
  netwib_er(netwox_asn1oid_init(psnmpmsg->pbufpool, &pinfos->oid));
  netwib_er(netwox_asn1oid_dup(&psnmpvb->oid, &pinfos->oid));

  *pstoploop = NETWIB_TRUE;
  psnmparg = psnmparg; /* for compiler warning */
  psnmpmsg = psnmpmsg; /* for compiler warning */
  psnmp3usmsp = psnmp3usmsp; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err t000160_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_io *pio;
  netwox_snmparg snmparg;
  t000160_infos infos;
  netwib_err ret;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000160_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_udp_cli(parg, &sockinfo));
  netwib_er(netwox_sock_init(&sockinfo, &pio));
  netwib_er(netwox_snmparg_init(parg, pio, &snmparg));

  netwib_er(netwox_asn1oid_init_arg(snmparg.pbufpool, parg, 'q', &infos.oid));

  /* main loop */
  do {
    ret = netwox_snmpex_session(&snmparg, pio, t000160_req, t000160_rep,
                                &infos);
  } while (ret == NETWIB_ERR_OK);
  if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;

  /* close */
  netwib_er(netwox_asn1oid_close(&infos.oid));
  netwib_er(netwox_snmparg_close(&snmparg));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(ret);
}

