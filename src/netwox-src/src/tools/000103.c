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
netwib_conststring t000103_description[] = {
  "This tool requests the version of a Bind DNS server. Bind is a common",
  "Unix DNS server. It provides a method to request its versions.",
  "Please note, some server administrator change this field to hide their",
  "DNS server type.",
  "",
  NETWOX_DESC_dns_tcp,
  "",
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000103_args[] = {
  NETWOX_SOCK_ARG_TCP_CLIPORT("53"),
  NETWOX_TOOLARG_OPT_BOOL('C', "tcp", "if use TCP instead of UDP", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in milliseconds",
                             "60000"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000103_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_UDP_DNS,
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_DNS,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000103_info = {
  "Obtain version of a Bind DNS server",
  t000103_description,
  NULL,
  t000103_args,
  t000103_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000103_display(netwib_bufpool *pbufpool,
                                  netwib_constbuf *pdnspkt,
                                  netwib_ptr infos)
{
  netwox_dnshdr dnsheader;
  netwib_ring *prrring;
  netwib_ring_index *pringindex;
  netwib_ptr pitem;
  netwox_dnsrr *pdnsrr;
  netwib_buf buf;
  netwib_err ret=NETWIB_ERR_OK;

  /*netwib_er(netwox_pkt_dns_display(pdnspkt, NULL,
                                   NETWIB_ENCODETYPE_ARRAY,
                                   NETWIB_ENCODETYPE_DUMP));*/


  netwib_er(netwox_dnsrrring_init(&prrring));
  netwib_er(netwox_pkt_decode_dns(pbufpool, pdnspkt, &dnsheader, prrring));

  if (dnsheader.rcode != NETWOX_DNSHDR_RCODE_OK) {
    netwib_er(netwox_dnsrrring_close(&prrring));
    return(NETWIB_ERR_OK);
  }

  /* do not check if answer is for question (only dns id is checked) */

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_ring_index_init(prrring, &pringindex));
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, &pitem);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    pdnsrr = (netwox_dnsrr *)pitem;
    if (pdnsrr->kind == NETWOX_DNSRR_KIND_ANSWER) {
      if (pdnsrr->type == NETWOX_DNSRR_TYPE_TXT) {
        netwib_er(netwox_dnsrdata_decode_txt(&pdnsrr->rdata, &buf));
        netwib_er(netwib_fmt_display("%{buf}\n", &buf));
        break;
      }
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwox_dnsrrring_close(&prrring));

  infos = infos; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err t000103_query(netwib_bufpool *pbufpool,
                                netwib_io *pio,
                                netwib_bool tcppacket,
                                netwib_uint32 maxwaitms)
{
  netwib_buf bufquestion, name;
  netwox_dnshdr dnsheader;
  netwib_time t;

  /* prepare question */
  netwib_er(netwib_buf_init_mallocdefault(&bufquestion));
  netwib_er(netwox_dnshdr_initdefault(&dnsheader));
  netwib_er(netwib_buf_init_ext_string("version.bind.", &name));
  netwib_er(netwox_pkt_append_dns_question(pbufpool, &dnsheader, &name,
                                           NETWOX_DNSRR_TYPE_TXT,
                                           NETWOX_DNSRR_CLASS_CH,
                                           &bufquestion));
  netwib_er(netwib_buf_close(&name));
  /*netwib_er(netwox_pkt_dns_display(&bufquestion, NULL,
                                   NETWIB_ENCODETYPE_ARRAY,
                                   NETWIB_ENCODETYPE_DUMP));*/

  /* send question and expect answer */
  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_plus_msec(&t, maxwaitms));
  netwib_er(netwox_dns_query(pbufpool, pio, tcppacket, &bufquestion, &t,
                             &t000103_display, NULL));

  netwib_er(netwib_buf_close(&bufquestion));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err t000103_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwib_io *pio;
  netwib_bufpool *pbufpool;
  netwib_bool usetcp;
  netwib_uint32 maxwaitms;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000103_info, &parg));
  netwib_er(netwox_arg_bool(parg, 'C', &usetcp));
  if (usetcp) {
    netwib_er(netwox_sockinfo_init_arg_tcp_cli(parg, &sockinfo));
  } else {
    netwib_er(netwox_sockinfo_init_arg_udp_cli(parg, &sockinfo));
  }
  netwib_er(netwox_arg_uint32(parg, 'T', &maxwaitms));

  /* query */
  netwib_er(netwox_sock_init(&sockinfo, &pio));
  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  netwib_er(t000103_query(pbufpool, pio, usetcp, maxwaitms));
  netwib_er(netwib_bufpool_close(&pbufpool));

  /* close */
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
