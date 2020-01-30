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
netwib_conststring t000102_description[] = {
  "This tool queries a DNS server. For example, it can retrieve IP",
  "address associated to a hostname.",
  "Parameter --dst-ip defines the DNS server address. This server has to",
  "be authoritative for the query requested (or allow recursion, if",
  "--recurdesired is set).",
  "Parameter --name contains the name to query.",
  "Parameter --type indicates the requested information type.",
  "",
  "Example: 'netwox 102 server --name www.example.com --type a'",
  "Example: 'netwox 102 server --name 1.2.3.4 --type ptr'",
  "Example: 'netwox 102 server --name example.com --type mx'",
  "",
  NETWOX_DESC_dns_tcp,
  "",
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000102_args[] = {
  NETWOX_SOCK_ARG_TCP_CLIPORT("53"),
  NETWOX_TOOLARG_REQ_BUF_HOSTNAME('n', "name", "name to query", NULL),
  NETWOX_TOOLARG_REQ_BUF('y', "type", "type (a, ptr, ns, etc.)", "a"),
  NETWOX_TOOLARG_OPTA_BUF('c', "class", "class (in, ch, etc.)", "in"),
  NETWOX_TOOLARG_OPTA_BOOL('u', "recurdesired", "recursion desired", NULL),
  NETWOX_TOOLARG_OPTA_BOOL('C', "tcp", "if use TCP instead of UDP", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in milliseconds",
                             "60000"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000102_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_UDP_DNS,
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_DNS,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000102_info = {
  "Query a DNS server",
  t000102_description,
  "dig, nslookup",
  t000102_args,
  t000102_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000102_display(netwib_bufpool *pbufpool,
                                  netwib_constbuf *pdnspkt,
                                  netwib_ptr infos)
{
  netwib_er(netwox_pkt_dns_display(pbufpool, pdnspkt, NULL,
                                   NETWIB_ENCODETYPE_ARRAY,
                                   NETWIB_ENCODETYPE_DUMP));
  infos = infos; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000102_query(netwib_bufpool *pbufpool,
                                netwib_io *pio,
                                netwib_constbuf *pname,
                                netwox_dnsrr_type type,
                                netwox_dnsrr_class rclass,
                                netwib_bool recurdesired,
                                netwib_bool tcppacket,
                                netwib_uint32 maxwaitms)
{
  netwib_buf bufquestion, name;
  netwox_dnshdr dnsheader;
  netwib_ip ipad;
  netwib_time t;
  netwib_err ret;

  /* prepare question */
  netwib_er(netwib_buf_init_mallocdefault(&name));
  netwib_er(netwib_buf_append_buf(pname, &name));
  if (type == NETWOX_DNSRR_TYPE_PTR) {
    ret = netwib_ip_init_buf(pname, NETWIB_IP_DECODETYPE_IP, &ipad);
    if (ret == NETWIB_ERR_OK) {
      netwib__buf_reinit(&name);
      netwib_er(netwox_dns_inaddrarpa(&ipad, &name));
    }
  }
  netwib_er(netwib_buf_init_mallocdefault(&bufquestion));
  netwib_er(netwox_dnshdr_initdefault(&dnsheader));
  dnsheader.recurdesired = recurdesired;
  netwib_er(netwox_pkt_append_dns_question(pbufpool, &dnsheader, &name, type,
                                           rclass, &bufquestion));
  netwib_er(netwib_buf_close(&name));
  netwib_er(netwox_pkt_dns_display(pbufpool, &bufquestion, NULL,
                                   NETWIB_ENCODETYPE_ARRAY,
                                   NETWIB_ENCODETYPE_DUMP));

  /* send question and expect answer */
  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_plus_msec(&t, maxwaitms));
  netwib_er(netwox_dns_query(pbufpool, pio, tcppacket, &bufquestion, &t,
                             &t000102_display, NULL));

  netwib_er(netwib_buf_close(&bufquestion));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err t000102_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_io *pio;
  netwib_bufpool *pbufpool;
  netwib_buf bufname, buftype, bufclass;
  netwox_dnsrr_type type;
  netwox_dnsrr_class rclass;
  netwib_bool recurdesired, usetcp;
  netwib_uint32 maxwaitms;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000102_info, &parg));
  netwib_er(netwox_arg_bool(parg, 'C', &usetcp));
  if (usetcp) {
    netwib_er(netwox_sockinfo_init_arg_tcp_cli(parg, &sockinfo));
  } else {
    netwib_er(netwox_sockinfo_init_arg_udp_cli(parg, &sockinfo));
  }
  netwib_er(netwox_arg_buf(parg, 'n', &bufname));
  netwib_er(netwox_arg_buf(parg, 'y', &buftype));
  netwib_er(netwox_arg_buf(parg, 'c', &bufclass));
  netwib_er(netwox_arg_bool(parg, 'u', &recurdesired));
  netwib_er(netwox_arg_uint32(parg, 'T', &maxwaitms));

  /* query */
  netwib_er(netwox_dnsrr_type_init_buf(&buftype, &type));
  netwib_er(netwox_dnsrr_class_init_buf(&bufclass, &rclass));
  netwib_er(netwox_sock_init(&sockinfo, &pio));
  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  netwib_er(t000102_query(pbufpool, pio, &bufname, type, rclass, recurdesired,
                          usetcp, maxwaitms));
  netwib_er(netwib_bufpool_close(&pbufpool));

  /* close */
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
