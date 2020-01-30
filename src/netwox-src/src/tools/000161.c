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
netwib_conststring t000161_description[] = {
  "This tool sends a TRAP to a SNMP server. It permits to alert a server.",
  "",
  NETWOX_DESC_snmp_common,
  "",
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000161_args[] = {
  NETWOX_SOCK_ARG_UDP_CLIPORT("162"),
  NETWOX_SNMP_ARG,
  NETWOX_TOOLARG_REQ_BUF_OID('r', "enterprise", "enterprise", ".1.1"),
  NETWOX_TOOLARG_REQ_IP('a', "agent", "agent", NULL),
  NETWOX_TOOLARG_REQ_UINT32('s', "traptype", "traptype", "3"),
  NETWOX_TOOLARG_OPT_UINT32('S', "specifictrap", "specifictrap", NULL),
  NETWOX_TOOLARG_REQ_UINT32('z', "timestamp", "timestamp", NULL),
  NETWOX_TOOLARG_REQ_BUF_OID('n', "oidname", "oid name", ".1.2"),
  NETWOX_TOOLARG_REQ_BUF_OIDTYPE('t', "oidtype",
                                 "oid type (i,s,m,n,o,a,c,g,t,M,u,U)", "s"),
  NETWOX_TOOLARG_REQ_BUF('V', "oidvalue", "oid value", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000161_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_UDP_SNMP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000161_info = {
  "SNMP Trap",
  t000161_description,
  "snmptrap",
  t000161_args,
  t000161_nodes,
};

/*-------------------------------------------------------------*/
typedef struct {
  netwox_asn1oid enterprise;
  netwib_ip agent;
  netwib_uint32 traptype;
  netwib_uint32 specifictrap;
  netwib_uint32 timestamp;
  netwox_asn1oid oidname;
  netwox_asn1data oidvalue;
} t000161_infos;

/*-------------------------------------------------------------*/
static netwib_err t000161_req(netwox_snmparg *psnmparg,
                              netwox_snmpmsg *psnmpmsg,
                              netwox_snmp3usmsp *psnmp3usmsp,
                              netwox_snmppdu *psnmppdu,
                              netwib_ptr infos)
{
  t000161_infos *pinfos = (t000161_infos *)infos;
  netwox_snmpvb snmpvb;

  netwib_er(netwox_snmppdu_set_type(psnmppdu, NETWOX_SNMPPDUTYPE_TRAP));
  netwib_er(netwox_asn1oid_dup(&pinfos->enterprise,
                               &psnmppdu->pdu.trappdu.enterprise));
  psnmppdu->pdu.trappdu.agentaddr = pinfos->agent;
  psnmppdu->pdu.trappdu.type = pinfos->traptype;
  psnmppdu->pdu.trappdu.specifictrap = pinfos->specifictrap;
  psnmppdu->pdu.trappdu.timestamp = pinfos->timestamp;
  netwib_er(netwox_snmpvb_init(psnmparg->pbufpool, &snmpvb));
  netwib_er(netwox_asn1oid_dup(&pinfos->oidname, &snmpvb.oid));
  netwib_er(netwox_asn1data_dup(&pinfos->oidvalue, &snmpvb.data));
  netwib_er(netwox_snmpvbl_add(&psnmppdu->pdu.trappdu.vbl, &snmpvb));

  psnmpmsg = psnmpmsg; /* for compiler warning */
  psnmp3usmsp = psnmp3usmsp; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err t000161_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_io *pio;
  netwox_snmparg snmparg;
  t000161_infos infos;
  netwib_err ret;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000161_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_udp_cli(parg, &sockinfo));
  netwib_er(netwox_sock_init(&sockinfo, &pio));
  netwib_er(netwox_snmparg_init(parg, pio, &snmparg));

  netwib_er(netwox_asn1oid_init_arg(snmparg.pbufpool, parg, 'r',
                                    &infos.enterprise));
  netwib_er(netwox_arg_ip(parg, 'a', &infos.agent));
  netwib_er(netwox_arg_uint32(parg, 's', &infos.traptype));
  netwib_er(netwox_arg_uint32(parg, 'S', &infos.specifictrap));
  netwib_er(netwox_arg_uint32(parg, 'z', &infos.timestamp));
  netwib_er(netwox_asn1oid_init_arg(snmparg.pbufpool, parg, 'n',
                                    &infos.oidname));
  netwib_er(netwox_asn1data_init_arg(snmparg.pbufpool, parg, 't', 'V',
                                     &infos.oidvalue));

  /* main loop */
  ret = netwox_snmpex_session(&snmparg, pio, t000161_req, NULL, &infos);

  /* close */
  netwib_er(netwox_asn1data_close(&infos.oidvalue));
  netwib_er(netwox_asn1oid_close(&infos.oidname));
  netwib_er(netwox_asn1oid_close(&infos.enterprise));
  netwib_er(netwox_snmparg_close(&snmparg));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(ret);
}

