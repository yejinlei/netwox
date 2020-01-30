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
netwib_conststring t000159_description[] = {
  "This tool requests a GET on a SNMP server. It permits to retrieve the",
  "value of one item.",
  "",
  NETWOX_DESC_snmp_oid,
  "",
  NETWOX_DESC_snmp_common,
  "",
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000159_args[] = {
  NETWOX_SOCK_ARG_UDP_CLIPORT("161"),
  NETWOX_SNMP_ARG,
  NETWOX_TOOLARG_REQ_BUF_OID('q', "oid", "OID to query", ".1.3.6.1.2.1.1.4.0"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000159_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_UDP_SNMP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000159_info = {
  "SNMP Get",
  t000159_description,
  "snmpget",
  t000159_args,
  t000159_nodes,
};

/*-------------------------------------------------------------*/
typedef struct {
  netwox_asn1oid oid;
  netwib_uint32 id;
} t000159_infos;

/*-------------------------------------------------------------*/
static netwib_err t000159_req(netwox_snmparg *psnmparg,
                              netwox_snmpmsg *psnmpmsg,
                              netwox_snmp3usmsp *psnmp3usmsp,
                              netwox_snmppdu *psnmppdu,
                              netwib_ptr infos)
{
  t000159_infos *pinfos = (t000159_infos *)infos;
  netwox_snmpvb snmpvb;

  netwib_er(netwox_snmppdu_set_type(psnmppdu, NETWOX_SNMPPDUTYPE_GETREQUEST));
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
static netwib_err t000159_rep(netwox_snmparg *psnmparg,
                              netwox_snmpmsg *psnmpmsg,
                              netwox_snmp3usmsp *psnmp3usmsp,
                              netwox_snmppdu *psnmppdu,
                              netwib_ptr infos,
                              netwib_bool *pstoploop)
{
  t000159_infos *pinfos = (t000159_infos *)infos;
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
    return(NETWOX_ERR_SNMP_BADREPLY);
  }

  psnmpvbl = &psnmppdu->pdu.pdu.vbl;
  if (psnmpvbl->vb.size != 1) {
    return(NETWOX_ERR_SNMP_BADREPLY);
  }

  psnmpvb = (netwox_snmpvb *)psnmpvbl->vb.p[0];
  netwib_er(netwox_asn1oid_cmp(&psnmpvb->oid, &pinfos->oid, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    return(NETWOX_ERR_SNMP_BADREPLY);
  }

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwox_asn1data_show(&psnmpvb->data, NETWIB_ENCODETYPE_SYNTH,
                                 &buf));
  netwib_er(netwib_fmt_display("%{buf}\n", &buf));
  netwib_er(netwib_buf_close(&buf));

  *pstoploop = NETWIB_TRUE;
  psnmparg = psnmparg; /* for compiler warning */
  psnmpmsg = psnmpmsg; /* for compiler warning */
  psnmp3usmsp = psnmp3usmsp; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err t000159_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_io *pio;
  netwox_snmparg snmparg;
  t000159_infos infos;
  netwib_err ret;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000159_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_udp_cli(parg, &sockinfo));
  netwib_er(netwox_sock_init(&sockinfo, &pio));
  netwib_er(netwox_snmparg_init(parg, pio, &snmparg));

  netwib_er(netwox_asn1oid_init_arg(snmparg.pbufpool, parg, 'q', &infos.oid));

  /* main loop */
  ret = netwox_snmpex_session(&snmparg, pio, t000159_req, t000159_rep, &infos);

  /* close */
  netwib_er(netwox_asn1oid_close(&infos.oid));
  netwib_er(netwox_snmparg_close(&snmparg));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(ret);
}

