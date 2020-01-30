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
netwib_conststring t000073_description[] = {
  "This tool simulates the presence of a computer. It will answer to ARP",
  "requests and ping.",
  "Other hosts can 'see' this virtual computer, but it will not really",
  "exist. This tool is frequently used to answer ARP when a tool is",
  "sending packets with a spoofed IP address.",
  "Parameter --ips defines the fake IP address of this computer.",
  "Parameter --eths defines the fake Ethernet address of this computer.",
  "If --no-answer-arp or --no-answer-ping is set, tool does not answers",
  "to ARP/IP6NeighborDiscovery and ping.",
  "It works only on a Ethernet device.",
  NETWOX_DESC_toolpriv_snsp,
  NULL
};
netwox_toolarg t000073_args[] = {
  NETWOX_TOOLARG_OPT_BUF_DEVICE('d', NULL, NULL, NULL),
  NETWOX_TOOLARG_REQ_IPS_U('i', NULL, NULL, "1.2.3.4"),
  NETWOX_TOOLARG_REQ_ETHS_U('e', NULL, NULL, "0:a:b:c:d:e"),
  NETWOX_TOOLARG_OPT_BOOL('a', "answer-arp", "answer ARP", "1"),
  NETWOX_TOOLARG_OPT_BOOL('p', "answer-ping", "answer ICMP Echo Request", "1"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000073_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SERVER_ARP,
  NETWOX_TOOLTREENODETYPE_SERVER_ICMP,
  NETWOX_TOOLTREENODETYPE_NETAUDIT_ARP,
  NETWOX_TOOLTREENODETYPE_NETAUDIT_ICMP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000073_info = {
  "Simulate presence of a/several computer/s (arp and ping)",
  t000073_description,
  "reply, virtual",
  t000073_args,
  t000073_nodes,
};

/*-------------------------------------------------------------*/
typedef enum {
  T000073_TYPE_ONEONE = 1,
  T000073_TYPE_SEVONE, /* severalIP -> oneEth */
  T000073_TYPE_SEVSEV  /* severalIP -> severalEth */
} t000073_type;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_buf device;
  netwib_ips *pips;
  netwib_eths *peths;
  netwib_bool answerarp;
  netwib_bool answerping;
  /* internal */
  t000073_type type;
  netwib_ips_index *pipsindex;
  netwib_eths_index *pethsindex;
  netwib_io *pio;
  netwib_eth srceth;
  netwib_ip srcip;
} t000073_param;

/*-------------------------------------------------------------*/
static netwib_err t000073_init(t000073_param *pparam)
{
  netwib_err ret1, ret2;

  /* initialize sniff/spoof io */
  netwib_er(netwox_snispo_init_eth(&pparam->device, NULL, &pparam->pio));

  /* initialize indexes */
  netwib_er(netwib_ips_index_init(pparam->pips, &pparam->pipsindex));
  netwib_er(netwib_eths_index_init(pparam->peths, &pparam->pethsindex));

  /* determine type of action */
  netwib_er(netwib_ips_index_next_ip(pparam->pipsindex, &pparam->srcip));
  netwib_er(netwib_eths_index_next_eth(pparam->pethsindex, &pparam->srceth));
  ret1 = netwib_ips_index_next_ip(pparam->pipsindex, NULL);
  ret2 = netwib_eths_index_next_eth(pparam->pethsindex, NULL);
  if (ret1 == NETWIB_ERR_OK && ret2 == NETWIB_ERR_OK) {
    /* do not check if numbers are equal */
    pparam->type = T000073_TYPE_SEVSEV;
  } else if (ret1 == NETWIB_ERR_OK) {
    pparam->type = T000073_TYPE_SEVONE;
  } else if (ret2 == NETWIB_ERR_OK) {
    netwib_er(netwib_fmt_display("There are more Ethernet addresses than IP addresses\n"));
    return(NETWOX_ERR_TOOLARG_INVALID);
  } else {
    pparam->type = T000073_TYPE_ONEONE;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000073_close(t000073_param *pparam)
{

  netwib_er(netwib_io_close(&pparam->pio));
  netwib_er(netwib_ips_index_close(&pparam->pipsindex));
  netwib_er(netwib_eths_index_close(&pparam->pethsindex));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000073_eth_init_ip(t000073_param *pparam,
                                      netwib_constip *pip,
                                      netwib_bool *pfound,
                                      netwib_eth *pethtouse,
                                      netwib_ip *piptouse)
{
  netwib_ip ipa, wantedip;
  netwib_eth eth;
  netwib_cmp cmp;
  netwib_uint32 number;
  netwib_err ret;

  *pfound = NETWIB_FALSE;

  /* search if candidate is in the list */
  netwib_er(netwib_ips_index_ctl_set_rewind(pparam->pipsindex));
  number = 0;
  while (NETWIB_TRUE) {
    /* get ipa/eth */
    ret = netwib_ips_index_next_ip(pparam->pipsindex, &ipa);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      return(ret);
    }
    number++;
    /* check it corresponds */
    netwib_er(netwib_ip_cmp(pip, &ipa, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      netwib_er(netwib_ip_init_ip6_fields(0xFF020000u, 0, 1, 0xFF000000u,
                                          &wantedip));
      wantedip.ipvalue.ip6.b[13] = ipa.ipvalue.ip6.b[13];
      wantedip.ipvalue.ip6.b[14] = ipa.ipvalue.ip6.b[14];
      wantedip.ipvalue.ip6.b[15] = ipa.ipvalue.ip6.b[15];
      netwib_er(netwib_ip_cmp(pip, &wantedip, &cmp));
      if (cmp != NETWIB_CMP_EQ) {
        continue;
      }
    }
    /* found */
    *pfound = NETWIB_TRUE;
    *piptouse = ipa;
    break;
  }

  /* easy case : only one Ethernet address */
  if (pparam->type == T000073_TYPE_SEVONE) {
    *pethtouse = pparam->srceth;
    return(NETWIB_ERR_OK);
  }

  /* search associated Ethernet address */
  eth.b[0] = 0; /* compiler warning */
  netwib_er(netwib_eths_index_ctl_set_rewind(pparam->pethsindex));
  while (number--) {
    ret = netwib_eths_index_next_eth(pparam->pethsindex, &eth);
    if (ret == NETWIB_ERR_DATAEND) {
      netwib_er(netwib_fmt_display("There are less Ethernet addresses than IP addresses\n"));
      return(NETWOX_ERR_TOOLARG_INVALID);
    }
    else if (ret != NETWIB_ERR_OK) return(ret);
  }
  *pethtouse = eth;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000073_answer(t000073_param *pparam,
                                 netwib_constbuf *ppkt)
{
  netwib_linkhdr linkheader;
  netwib_arphdr arpheader;
  netwib_iphdr ipheader;
  netwib_icmp4 icmp4;
  netwib_icmp6 icmp6;
  netwib_buf pkt;
  netwib_ipproto ipproto;
  netwib_eth ethtouse;
  netwib_ip iptouse;
  netwib_bool found;
  netwib_err ret;

  pkt = *ppkt;

  ret = netwib_pkt_decode_layer_link(NETWIB_DEVICE_DLTTYPE_ETHER,
                                     &pkt, &linkheader);
  if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);

  switch(linkheader.hdr.ether.type) {
  case NETWIB_ETHERHDRTYPE_IP4 :
    if (!pparam->answerping) return(NETWIB_ERR_OK);
    ret = netwib_pkt_decode_layer_ip(&pkt, &ipheader);
    if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
    netwib_er(netwib_iphdr_get_proto(&ipheader, &ipproto));
    switch(ipproto) {
    case NETWIB_IPPROTO_ICMP4 :
      ret = netwib_pkt_decode_layer_icmp4(&pkt, &icmp4);
      if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
      netwib_er(t000073_eth_init_ip(pparam, &ipheader.dst, &found,
                                    &ethtouse, &iptouse));
      if (found) {
        netwib_er(netwox_alive_linkipicmp4ping(pparam->pio,
                                               &ethtouse, &iptouse,
                                               &linkheader, &ipheader,
                                               &icmp4, NULL));
      }
      break;
    default :
      break;
    }
    break;
  case NETWIB_ETHERHDRTYPE_IP6 :
    ret = netwib_pkt_decode_layer_ip(&pkt, &ipheader);
    if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
    netwib_er(netwib_iphdr_get_proto(&ipheader, &ipproto));
    switch(ipproto) {
    case NETWIB_IPPROTO_ICMP6 :
      ret = netwib_pkt_decode_layer_icmp6(&pkt, &icmp6);
      if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
      netwib_er(t000073_eth_init_ip(pparam, &ipheader.dst, &found,
                                    &ethtouse, &iptouse));
      if (found) {
        if (pparam->answerarp) {
          netwib_er(netwox_alive_linkipicmp6nd(pparam->pio,
                                               &ethtouse, &iptouse,
                                               &linkheader, &ipheader,
                                               &icmp6, NULL));
        }
        if (pparam->answerping) {
          netwib_er(netwox_alive_linkipicmp6ping(pparam->pio,
                                                 &ethtouse, &iptouse,
                                                 &linkheader, &ipheader,
                                                 &icmp6, NULL));
        }
      }
      break;
    default :
      break;
    }
    break;
  case NETWIB_ETHERHDRTYPE_ARP :
    if (pparam->answerarp) {
      ret = netwib_pkt_decode_layer_arp(&pkt, &arpheader);
      if (ret != NETWIB_ERR_OK) return(NETWIB_ERR_OK);
      netwib_er(t000073_eth_init_ip(pparam, &arpheader.ipdst, &found,
                                    &ethtouse, &iptouse));
      if (found) {
        netwib_er(netwox_alive_linkarp(pparam->pio,
                                       &ethtouse, &iptouse,
                                       &linkheader, &arpheader, NULL));
      }
    }
    break;
  default :
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000073_loop(t000073_param *pparam)
{
  netwib_buf pkt;
  netwib_err ret;

  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  while (NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();
    /* sniff */
    netwib__buf_reinit(&pkt);
    ret = netwib_io_read(pparam->pio, &pkt);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    }
    if (ret != NETWIB_ERR_OK) return(ret);
    /* eventually answer */
    if (pparam->type == T000073_TYPE_ONEONE) {
      /* easy case */
      netwib_er(netwox_alive_ether(pparam->pio, &pparam->srceth,
                                   &pparam->srcip, &pkt, pparam->answerarp,
                                   pparam->answerping, NETWIB_FALSE,
                                   NULL));
    } else {
      netwib_er(t000073_answer(pparam, &pkt));
    }
  }
  netwib_er(netwib_buf_close(&pkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err t000073_core(int argc, char *argv[])
{
  netwox_arg *parg;
  t000073_param param;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000073_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'd', &param.device));
  netwib_er(netwox_arg_ips(parg, 'i', &param.pips));
  netwib_er(netwox_arg_eths(parg, 'e', &param.peths));
  netwib_er(netwox_arg_bool(parg, 'a', &param.answerarp));
  netwib_er(netwox_arg_bool(parg, 'p', &param.answerping));

  /* initialize */
  netwib_er(t000073_init(&param));

  /* main loop */
  netwib_er(t000073_loop(&param));

  /* close */
  netwib_er(t000073_close(&param));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
