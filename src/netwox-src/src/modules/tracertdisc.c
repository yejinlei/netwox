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
netwib_err netwox_tracertdisc_hoplist_hop_create(netwib_ptr *ppitem)
{
  netwib_er(netwib_ptr_malloc(sizeof(netwox_tracertdisc_hoplist_hop), ppitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_tracertdisc_hoplist_hop_erase(netwib_ptr pitem)
{
  netwib_er(netwib_ptr_free(&pitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_tracertdisc_hoplist_ring_add(netwib_ring *pring,
                                               netwib_uint32 hopcount,
                                               netwib_constip *phopip,
                                               netwib_bool isdst,
                                               netwib_bool portisopen,
                                               netwib_bool isfake)
{
  netwox_tracertdisc_hoplist_hop *pitem;

  netwib_er(netwox_tracertdisc_hoplist_hop_create((netwib_ptr *)&pitem));

  pitem->hopcount = hopcount;
  pitem->hopip = *phopip;
  pitem->isdst = isdst;
  pitem->portisopen = portisopen;
  pitem->isfake = isfake;

  netwib_er(netwib_ring_add_last(pring, (netwib_constptr)pitem));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_tracertdisc_hoplist_ring_add_txt(netwib_ring *pring,
                                                   netwib_uint32 hopcount,
                                                   netwib_conststring hopip,
                                                   netwib_bool isdst,
                                                   netwib_bool portisopen,
                                                   netwib_bool isfake)
{
  netwib_buf buf;
  netwib_ip ipad;

  netwib_er(netwib_buf_init_ext_string(hopip, &buf));
  netwib_er(netwib_ip_init_buf(&buf, NETWIB_IP_DECODETYPE_IP, &ipad));

  netwib_er(netwox_tracertdisc_hoplist_ring_add(pring, hopcount, &ipad,
                                                isdst, portisopen, isfake));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_tracertdisc_hoplist_ring_display(netwib_ring *pring,
                                                   netwox_tracertdisc_method method)
{
  netwib_ring_index *pringindex;
  netwox_tracertdisc_hoplist_hop *pitem;
  netwib_err ret;

  netwib_er(netwib_ring_index_init(pring, &pringindex));

  ret = NETWIB_ERR_OK;
  while(NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pitem);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_er(netwib_fmt_display(" "));
    if (pitem->isfake) {
      netwib_er(netwib_fmt_display("after_"));
    }
    netwib_er(netwib_fmt_display("hop%{uint32}=%{ip}", pitem->hopcount,
                                 &pitem->hopip));
    if (pitem->isdst) {
      netwib_er(netwib_fmt_display("-destination"));
      switch(method) {
      case NETWOX_TRACERTDISC_METHOD_TCP :
      case NETWOX_TRACERTDISC_METHOD_UDP :
        if (pitem->portisopen) {
          netwib_er(netwib_fmt_display("-open"));
        } else {
          netwib_er(netwib_fmt_display("-closed"));
        }
        break;
      default :
        break;
      }
    }
  }
  netwib_er(netwib_fmt_display("\n"));

  netwib_er(netwib_ring_index_close(&pringindex));

  return(ret);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err netwox_tracertdisc_loop_ip(netwox_tracertdisc *ptracertdisc,
                                             netwox_tracertdisc_graph *pgraph,
                                             netwib_ip *pip)
{
  netwib_ports_index *pportsindex;
  netwox_tracert_method method;
  netwib_ring *phoplist;
  netwib_port port;
  netwib_err ret=NETWIB_ERR_OK;

  /* TCP */
  netwib_er(netwox_tracertdisc_hoplist_ring_init(&phoplist));
  netwib_er(netwib_ports_index_init(ptracertdisc->pdsttcpports, &pportsindex));
  method = NETWOX_TRACERT_METHOD_TCP;
  if (ptracertdisc->ethspoof) {
    method = NETWOX_TRACERT_METHOD_TCP_ETHIPSPOOF;
  }
  while (NETWIB_TRUE) {
    ret = netwib_ports_index_next_port(pportsindex, &port);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    ptracertdisc->addinfo.dstport = port;
    netwib_er(netwib_fmt_display("\nTCP traceroute to port %{port} of %{ip} in progress...\n", port, pip));
    ret = netwox_tracert_loop(method, pip, &ptracertdisc->addinfo, phoplist);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwib_fmt_display("Note: port %{port} of %{ip} generated an error\n", port, pip));
    } else {
      netwib_er(netwox_tracertdisc_graph_add(pgraph, phoplist,
                                             NETWOX_TRACERTDISC_METHOD_TCP,
                                             port));
    }
    netwib_er(netwox_tracertdisc_hoplist_ring_delall(phoplist));
  }
  netwib_er(netwox_tracertdisc_hoplist_ring_close(&phoplist));
  netwib_er(netwib_ports_index_close(&pportsindex));
  netwib_er(ret);

  /* UDP */
  netwib_er(netwox_tracertdisc_hoplist_ring_init(&phoplist));
  netwib_er(netwib_ports_index_init(ptracertdisc->pdstudpports, &pportsindex));
  method = NETWOX_TRACERT_METHOD_UDP;
  if (ptracertdisc->ethspoof) {
    method = NETWOX_TRACERT_METHOD_UDP_ETHIPSPOOF;
  }
  while (NETWIB_TRUE) {
    ret = netwib_ports_index_next_port(pportsindex, &port);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    ptracertdisc->addinfo.dstport = port;
    netwib_er(netwib_fmt_display("\nUDP traceroute to port %{port} of %{ip} in progress...\n", port, pip));
    ret = netwox_tracert_loop(method, pip, &ptracertdisc->addinfo, phoplist);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwib_fmt_display("Note: port %{port} of %{ip} generated an error\n", port, pip));
    } else {
      netwib_er(netwox_tracertdisc_graph_add(pgraph, phoplist,
                                             NETWOX_TRACERTDISC_METHOD_UDP,
                                             port));
    }
    netwib_er(netwox_tracertdisc_hoplist_ring_delall(phoplist));
  }
  netwib_er(netwox_tracertdisc_hoplist_ring_close(&phoplist));
  netwib_er(netwib_ports_index_close(&pportsindex));
  netwib_er(ret);

  /* ICMP */
  if (ptracertdisc->useicmp) {
    netwib_er(netwox_tracertdisc_hoplist_ring_init(&phoplist));
    method = NETWOX_TRACERT_METHOD_PING;
    if (ptracertdisc->ethspoof) {
      method = NETWOX_TRACERT_METHOD_PING_ETHIPSPOOF;
    }
    netwib_er(netwox_tracertdisc_hoplist_ring_delall(phoplist));
    netwib_er(netwib_fmt_display("\nICMP traceroute to %{ip} in progress...\n", pip));
    ret = netwox_tracert_loop(method, pip, &ptracertdisc->addinfo, phoplist);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwib_fmt_display("Note: traceroute to %{ip} generated an error\n",
                                   pip));
    } else {
      netwib_er(netwox_tracertdisc_graph_add(pgraph, phoplist,
                                             NETWOX_TRACERTDISC_METHOD_ICMP,
                                             0));
    }
    netwib_er(netwox_tracertdisc_hoplist_ring_close(&phoplist));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_tracertdisc_loop(netwox_tracertdisc *ptracertdisc)
{
  netwox_tracertdisc_graph graph;
  netwib_ips_index *pipsindex;
  netwib_ip ipad;
  netwib_err ret=NETWIB_ERR_OK;

  /* initialize graph */
  netwib_er(netwox_tracertdisc_graph_init(ptracertdisc, &graph));
  ptracertdisc->addinfo.resolve = NETWIB_FALSE; /* will be done with cache
                                                   in the graph */

  /* retrieve information */
  netwib_er(netwib_ips_index_init(ptracertdisc->pdstips, &pipsindex));
  while (NETWIB_TRUE) {
    ret = netwib_ips_index_next_ip(pipsindex, &ipad);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_er(netwib_fmt_display("\n\n-----------------------------------------------------------\n"));
    netwib_er(netwib_fmt_display("Traceroute to %{ip} in progress...\n", &ipad));
    ret = netwox_tracertdisc_loop_ip(ptracertdisc, &graph, &ipad);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwib_fmt_display("Note: traceroute to %{ip} generated an error\n", &ipad));
    }
    if (ptracertdisc->verbose) {
      netwib_er(netwib_fmt_display("\n\n-----------------------------------------------------------\n"));
      netwib_er(netwib_fmt_display("Intermediary report:\n"));
      netwib_er(netwox_tracertdisc_graph_display(&graph));
    }
  }
  netwib_er(netwib_ips_index_close(&pipsindex));
  netwib_er(ret);

  /* display graph */
  netwib_er(netwib_fmt_display("\n\n-----------------------------------------------------------\n"));
  netwib_er(netwib_fmt_display("Final report:\n"));
  netwib_er(netwox_tracertdisc_graph_display(&graph));

  /* close graph */
  netwib_er(netwox_tracertdisc_graph_close(&graph));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
typedef struct {
  netwib_ip hostip;
  netwib_ring *pchilds;
  netwib_ring *phostinfo;
  netwib_bool isfake;
  /* internal */
  netwib_uint32 level;
  netwib_uint32 displaynumber;
} netwox_hostgraph_host;
static netwib_err netwox_hostgraph_host_create(netwib_ptr *ppitem)
{
  netwox_hostgraph_host *phost;

  netwib_er(netwib_ptr_malloc(sizeof(netwox_hostgraph_host), ppitem));

  phost = (netwox_hostgraph_host *)*ppitem;
  netwib_er(netwib_ring_init(NULL, NULL, &phost->pchilds));
  netwib_er(netwib_ring_init(NULL, NULL, &phost->phostinfo));

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_hostgraph_host_erase(netwib_ptr pitem)
{
  netwox_hostgraph_host *phost;

  phost = (netwox_hostgraph_host *)pitem;
  netwib_er(netwib_ring_close(&phost->pchilds, NETWIB_FALSE));
  netwib_er(netwib_ring_close(&phost->phostinfo, NETWIB_FALSE));

  netwib_er(netwib_ptr_free(&pitem));

  return(NETWIB_ERR_OK);
}
#define netwox_hostgraph_ring_init(ppring) netwib_ring_init(&netwox_hostgraph_host_erase,NULL,ppring)
#define netwox_hostgraph_ring_close(ppring) netwib_ring_close(ppring,NETWIB_TRUE)
static netwib_err netwox_hostgraph_ring_add(netwib_ring *pring,
                                            netwib_constip *phostip,
                                            netwib_bool isfake,
                                            netwib_uint32 level,
                                            netwib_uint32 displaynumber,
                                            netwox_hostgraph_host **pphost)
{
  netwox_hostgraph_host *phost;

  netwib_er(netwox_hostgraph_host_create((netwib_ptr *)&phost));
  netwib_er(netwib_ring_add_last(pring, (netwib_constptr)phost));
  *pphost = phost;

  phost->hostip = *phostip;
  phost->isfake = isfake;
  phost->level = level;
  phost->displaynumber = displaynumber;

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_hostgraph_ring_search_f(netwib_constptr pitem,
                                                 netwib_ptr pinfos,
                                                 netwib_bool *pbool)
{
  const netwox_hostgraph_host *phost = (const netwox_hostgraph_host *)pitem;
  netwib_ip *phostip = (netwib_ip *)pinfos;
  netwib_cmp cmp;

  netwib_er(netwib_ip_cmp(&phost->hostip, phostip, &cmp));
  if (cmp == NETWIB_CMP_EQ) {
    *pbool = NETWIB_TRUE;
  } else {
    *pbool = NETWIB_FALSE;
  }

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_hostgraph_ring_search(netwib_ring *pring,
                                               netwib_ip *phostip,
                                               netwox_hostgraph_host **pphost)
{
  netwib_ring_index *pringindex;
  netwib_ptr pitem;
  netwib_err ret;

  netwib_er(netwib_ring_index_init(pring, &pringindex));

  ret = netwib_ring_index_next_criteria(pringindex,
                                        &netwox_hostgraph_ring_search_f,
                                        phostip, &pitem);
  if (ret == NETWIB_ERR_OK) {
    *pphost = (netwox_hostgraph_host *)pitem;
  } else {
    *pphost = NULL;
  }

  netwib_er(netwib_ring_index_close(&pringindex));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_tracertdisc_graph_init(netwox_tracertdisc *ptracertdisc,
                                         netwox_tracertdisc_graph *pgraph)
{
  netwib_uint32 i;

  pgraph->resolve = ptracertdisc->addinfo.resolve;
  if (pgraph->resolve) {
    netwib_er(netwox_dnscache_init(NETWIB_FALSE, NETWIB_TRUE,
                                   &pgraph->dnscache));
  }
  pgraph->verbose = ptracertdisc->verbose;

  for (i = 0; i < NETWOX_TRACERTDISC_GRAPH_MAXLEVEL; i++) {
    pgraph->levels[i] = NULL;
  }
  pgraph->lastdisplaynumber = 0;

  netwib_er(netwib_buf_init_mallocdefault(&pgraph->pad));

  netwib_er(netwib_ips_initdefault(&pgraph->pipsreached));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_tracertdisc_graph_close(netwox_tracertdisc_graph *pgraph)
{
  netwib_uint32 i;

  netwib_er(netwib_ips_close(&pgraph->pipsreached));

  netwib_er(netwib_buf_close(&pgraph->pad));

  for (i = 0; i < NETWOX_TRACERTDISC_GRAPH_MAXLEVEL; i++) {
    if (pgraph->levels[i] != NULL) {
      netwib_er(netwox_hostgraph_ring_close(&pgraph->levels[i]));
    }
  }

  if (pgraph->resolve) {
    netwib_er(netwox_dnscache_close(&pgraph->dnscache));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_tracertdisc_graph_add(netwox_tracertdisc_graph *pgraph,
                                        netwib_ring *phoplist,
                                        netwox_tracertdisc_method method,
                                        netwib_port port)
{
  netwib_ring_index *pringindex, *pringindex2;
  netwox_tracertdisc_hoplist_hop *phop;
  netwox_hostgraph_host *phost, *phostparent, *phosttmp;
  netwib_uint32 ui;
  netwib_ptr pitem;
  netwib_err ret;

  if (pgraph->verbose) {
    netwib_er(netwib_fmt_display("Adding to graph "));
    switch(method) {
    case NETWOX_TRACERTDISC_METHOD_TCP :
      netwib_er(netwib_fmt_display("%{port}/tcp:\n", port));
      break;
    case NETWOX_TRACERTDISC_METHOD_UDP :
      netwib_er(netwib_fmt_display("%{port}/udp:\n", port));
      break;
    case NETWOX_TRACERTDISC_METHOD_ICMP :
      netwib_er(netwib_fmt_display("icmp:\n"));
      break;
    default :
      return(NETWOX_ERR_INTERNALERROR);
    }
    netwib_er(netwox_tracertdisc_hoplist_ring_display(phoplist, method));
  }

  netwib_er(netwib_ring_index_init(phoplist, &pringindex));
  ret = NETWIB_ERR_OK;
  phostparent = NULL;
  while(NETWIB_TRUE) {
    /* obtain next hop */
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&phop);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    /* add it to the list */
    if (!phop->isfake) {
      netwib_er(netwib_ips_add_ip(pgraph->pipsreached, &phop->hopip));
    }
    /* add it to the graph */
    if (phop->hopcount > NETWOX_TRACERTDISC_GRAPH_MAXLEVEL-1) {
      netwib_er(netwib_fmt_display("This hopcount is too big: %{uint32}\n",
                                   phop->hopcount));
      continue;
    }
    phost = NULL;
    if (pgraph->levels[phop->hopcount] == NULL) {
      netwib_er(netwox_hostgraph_ring_init(&pgraph->levels[phop->hopcount]));
    } else {
      netwib_er(netwox_hostgraph_ring_search(pgraph->levels[phop->hopcount],
                                             &phop->hopip, &phost));
    }
    if (phost == NULL) {
      netwib_er(netwox_hostgraph_ring_add(pgraph->levels[phop->hopcount],
                                          &phop->hopip, phop->isfake,
                                          phop->hopcount,
                                          pgraph->lastdisplaynumber,
                                          &phost));
    }
    /* add its information */
    if (phop->isdst) {
      ui = (netwib_uint32)((method<<24) | (phop->portisopen<<16) | port);
      netwib_er(netwib_ring_add_last(phost->phostinfo, (netwib_constptr)(netwib_uintptr)ui));
    }
    /* link with its parent (search its childs to create only one link) */
    if (phostparent != NULL) {
      netwib_er(netwib_ring_index_init(phostparent->pchilds, &pringindex2));
      while (NETWIB_TRUE) {
        ret = netwib_ring_index_next_criteria(pringindex2,
                                              &netwox_hostgraph_ring_search_f,
                                              &phop->hopip, &pitem);
        if (ret != NETWIB_ERR_OK) {
          netwib_er(netwib_ring_add_last(phostparent->pchilds, phost));
          break;
        }
        phosttmp = (netwox_hostgraph_host *)pitem;
        if (phosttmp->level == phop->hopcount) {
          /* already linked */
          break;
        }
      }
      netwib_er(netwib_ring_index_close(&pringindex2));
    }
    phostparent = phost;
  }
  netwib_er(netwib_ring_index_close(&pringindex));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_tracertdisc_graph_display_host(netwox_tracertdisc_graph *pgraph,
                                                 netwox_hostgraph_host *phost,
                                                        netwib_uint32 skipsizeadded)
{
  netwib_ring_index *pringindex;
  netwox_hostgraph_host *phostchild;
  netwib_ptr ptr;
  netwib_buf hn;
  netwox_tracertdisc_method method;
  netwib_port port;
  netwib_bool portisopen, containsip;
  netwib_data data;
  netwib_uint32 datasize, ui, count, totalcount, skipsize;
  netwib_err ret;

  /* there is no need to display a fake entry when a real one is available */
  if (phost->isfake) {
    netwib_er(netwib_ips_contains_ip(pgraph->pipsreached, &phost->hostip,
                                     &containsip));
    if (containsip) {
      return(NETWIB_ERR_OK);
    }
  }

  if (!pgraph->titledisplayed) {
    netwib_er(netwib_fmt_display("Tree starting at level %{uint32}:\n",
                                 phost->level));
    pgraph->titledisplayed = NETWIB_TRUE;
  }

  /* display host information */
  netwib_er(netwib_fmt_display("%{buf}-%{ip}", &pgraph->pad, &phost->hostip));
  /* replace "-?" by "  " ; also replace "`" by " " */
  pgraph->pad.endoffset -= 2*skipsizeadded;
  datasize = netwib__buf_ref_data_size(&pgraph->pad);
  if (datasize) {
    data = netwib__buf_ref_data_ptr(&pgraph->pad);
    if (data[datasize-1] == '`') {
      data[datasize-1] = ' ';
    }
  }
  for (ui = 0; ui < skipsizeadded; ui++) {
    netwib_er(netwib_buf_append_string("  ", &pgraph->pad));
  }
  if (phost->displaynumber >= pgraph->lastdisplaynumber) {
    netwib_er(netwib_fmt_display(" ...(branch already displayed)\n"));
    return(NETWIB_ERR_OK);
  } else if (pgraph->resolve) {
    netwib_er(netwib_buf_init_mallocdefault(&hn));
    ret = netwib_buf_append_ip(&phost->hostip, NETWIB_IP_ENCODETYPE_HNS, &hn);
    if (ret != NETWIB_ERR_OK) {
      netwib__buf_reinit(&hn);
      netwib_er(netwib_buf_append_string("unresolved", &hn));
    }
    netwib_er(netwib_fmt_display(" (%{buf})", &hn));
    netwib_er(netwib_buf_close(&hn));
  }
  netwib_er(netwib_fmt_display("\n"));
  phost->displaynumber = pgraph->lastdisplaynumber;

  /* display stored information */
  if (phost->isfake) {
    netwib_er(netwib_fmt_display("%{buf} nothing indicates this host is up\n",
                                 &pgraph->pad));
  } else {
    netwib_ports *ptcpopen, *ptcpclosed, *pudpclosed;
    netwib_bool tcpopen, tcpclosed, udpclosed, icmpopen;
    netwib_er(netwib_ports_initdefault(&ptcpopen));
    netwib_er(netwib_ports_initdefault(&ptcpclosed));
    netwib_er(netwib_ports_initdefault(&pudpclosed));
    tcpopen = NETWIB_FALSE; tcpclosed = NETWIB_FALSE;
    udpclosed = NETWIB_FALSE; icmpopen = NETWIB_FALSE;
    netwib_er(netwib_ring_index_init(phost->phostinfo, &pringindex));
    ret = NETWIB_ERR_OK;
    while(NETWIB_TRUE) {
      ret = netwib_ring_index_next(pringindex, &ptr);
      if (ret != NETWIB_ERR_OK) {
        if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
        break;
      }
      ui = (netwib_uint32)(netwib_uintptr)ptr;
      method = (netwox_tracertdisc_method)(ui>>24);
      portisopen = (netwib_bool)((ui>>16)&0xFF);
      port = (netwib_port)(ui&0xFFFF);
      switch(method) {
      case NETWOX_TRACERTDISC_METHOD_TCP :
        if (portisopen) {
          netwib_er(netwib_ports_add_port(ptcpopen, port));
          tcpopen = NETWIB_TRUE;
        } else {
          netwib_er(netwib_ports_add_port(ptcpclosed, port));
          tcpclosed = NETWIB_TRUE;
        }
        break;
      case NETWOX_TRACERTDISC_METHOD_UDP :
        /* udp is always closed */
        netwib_er(netwib_ports_add_port(pudpclosed, port));
        udpclosed = NETWIB_TRUE;
        break;
      case NETWOX_TRACERTDISC_METHOD_ICMP :
        icmpopen = NETWIB_TRUE;
        break;
      default :
        return(NETWOX_ERR_INTERNALERROR);
      }
    }
    if (tcpopen) {
      netwib_er(netwib_fmt_display("%{buf} tcp_open: %{ports}\n",
                                   &pgraph->pad, ptcpopen));
    }
    if (tcpclosed) {
      netwib_er(netwib_fmt_display("%{buf} tcp_closed: %{ports}\n",
                                   &pgraph->pad, ptcpclosed));
    }
    if (udpclosed) {
      netwib_er(netwib_fmt_display("%{buf} udp_closed: %{ports}\n",
                                   &pgraph->pad, pudpclosed));
    }
    if (icmpopen) {
      netwib_er(netwib_fmt_display("%{buf} icmp: reached\n", &pgraph->pad));
    }
    netwib_er(netwib_ring_index_close(&pringindex));
    netwib_er(netwib_ports_close(&ptcpopen));
    netwib_er(netwib_ports_close(&ptcpclosed));
    netwib_er(netwib_ports_close(&pudpclosed));
    netwib_er(ret);
  }

  /* skip into childs */
  netwib_er(netwib_buf_append_string(" |", &pgraph->pad));
  netwib_er(netwib_ring_index_init(phost->pchilds, &pringindex));
  netwib_er(netwib_ring_ctl_get_count(phost->pchilds, &totalcount));
  ret = NETWIB_ERR_OK;
  count = 0;
  while(NETWIB_TRUE) {
    /* obtain next host */
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&phostchild);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    /* change pad buffer */
    count++;
    if (count == totalcount) {
      pgraph->pad.endoffset--;
      netwib_er(netwib_buf_append_byte('`', &pgraph->pad));
    }
    skipsize = 0;
    if (phostchild->level > phost->level+1) {
      skipsize = phostchild->level - phost->level - 1;
      for (ui = 0; ui < skipsize; ui++) {
        netwib_er(netwib_buf_append_string("-?", &pgraph->pad));
      }
    }
    /* display */
    netwib_er(netwox_tracertdisc_graph_display_host(pgraph, phostchild,
                                                    skipsize));
    pgraph->pad.endoffset -= 2*skipsize;
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  pgraph->pad.endoffset -= 2;

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_tracertdisc_graph_display(netwox_tracertdisc_graph *pgraph)
{
  netwib_ring_index *pringindex;
  netwox_hostgraph_host *phost;
  netwib_uint32 i;
  netwib_err ret;

  pgraph->lastdisplaynumber++;
  ret = NETWIB_ERR_OK;
  for (i = 0; i < NETWOX_TRACERTDISC_GRAPH_MAXLEVEL; i++) {
    if (pgraph->levels[i] != NULL) {
      pgraph->titledisplayed = NETWIB_FALSE;
      netwib_er(netwib_ring_index_init(pgraph->levels[i], &pringindex));
      while(NETWIB_TRUE) {
        /* obtain next host */
        ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&phost);
        if (ret != NETWIB_ERR_OK) {
          if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
          break;
        }
        /* check if it's a new start */
        if (phost->displaynumber < pgraph->lastdisplaynumber) {
          netwib__buf_reinit(&pgraph->pad);
          netwib_er(netwox_tracertdisc_graph_display_host(pgraph, phost, 0));
        }
      }
      netwib_er(netwib_ring_index_close(&pringindex));
    }
  }

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_tracertdisc_graph_test2(netwox_tracertdisc_graph *pgraph,
                                          netwib_ring *phoplist,
                                          netwox_tracertdisc_method method,
                                          netwib_port port)
{
  netwib_er(netwox_tracertdisc_graph_add(pgraph, phoplist, method, port));
  netwib_er(netwox_tracertdisc_graph_display(pgraph));
  netwib_er(netwox_tracertdisc_hoplist_ring_delall(phoplist));
  return(NETWIB_ERR_OK);
}
netwib_err netwox_tracertdisc_graph_test(netwox_tracertdisc *ptracertdisc)
{
  netwox_tracertdisc_graph graph;
  netwib_ring *phoplist;

  /* init */
  netwib_er(netwox_tracertdisc_graph_init(ptracertdisc, &graph));
  graph.verbose = NETWIB_TRUE;
  netwib_er(netwox_tracertdisc_hoplist_ring_init(&phoplist));

  /**/
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 1, "1.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 2, "2.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 5, "3.2.3.4",
                                                    1, 1, 0));
  netwib_er(netwox_tracertdisc_graph_test2(&graph, phoplist,
                                           NETWOX_TRACERTDISC_METHOD_TCP, 21));

  /**/
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 1, "1.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 2, "2.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 3, "3.2.3.5",
                                                    1, 1, 0));
  netwib_er(netwox_tracertdisc_graph_test2(&graph, phoplist,
                                           NETWOX_TRACERTDISC_METHOD_TCP, 11));

  /**/
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 1, "1.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 2, "2.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 1, "3.2.3.6",
                                                    1, 1, 0));
  netwib_er(netwox_tracertdisc_graph_test2(&graph, phoplist,
                                           NETWOX_TRACERTDISC_METHOD_TCP, 10));

  /**/
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 1, "1.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 2, "2.2.3.4",
                                                    1, 0, 0));
  netwib_er(netwox_tracertdisc_graph_test2(&graph, phoplist,
                                           NETWOX_TRACERTDISC_METHOD_TCP, 99));

  /**/
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 1, "1.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 2, "4.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 6, "5.2.3.4",
                                                    0, 0, 1));
  netwib_er(netwox_tracertdisc_graph_test2(&graph, phoplist,
                                           NETWOX_TRACERTDISC_METHOD_TCP, 26));

  /**/
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 1, "1.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 2, "4.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 3, "6.2.3.4",
                                                    1, 1, 0));
  netwib_er(netwox_tracertdisc_graph_test2(&graph, phoplist,
                                           NETWOX_TRACERTDISC_METHOD_TCP, 22));

  /**/
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 1, "1.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 2, "4.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 3, "6.2.3.4",
                                                    1, 0, 0));
  netwib_er(netwox_tracertdisc_graph_test2(&graph, phoplist,
                                           NETWOX_TRACERTDISC_METHOD_TCP, 21));

  /**/
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 1, "1.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 2, "4.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 3, "6.2.3.4",
                                                    1, 0, 0));
  netwib_er(netwox_tracertdisc_graph_test2(&graph, phoplist,
                                           NETWOX_TRACERTDISC_METHOD_TCP, 23));

  /**/
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 1, "1.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 2, "4.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 3, "6.2.3.4",
                                                    1, 0, 0));
  netwib_er(netwox_tracertdisc_graph_test2(&graph, phoplist,
                                           NETWOX_TRACERTDISC_METHOD_UDP, 21));

  /**/
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 1, "1.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 2, "4.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 3, "6.2.3.4",
                                                    1, 0, 0));
  netwib_er(netwox_tracertdisc_graph_test2(&graph, phoplist,
                                           NETWOX_TRACERTDISC_METHOD_ICMP, 0));

  /**/
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 1, "1.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 2, "4.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 3, "6.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 4, "9.2.3.4",
                                                    1, 0, 0));
  netwib_er(netwox_tracertdisc_graph_test2(&graph, phoplist,
                                           NETWOX_TRACERTDISC_METHOD_ICMP, 0));

  /**/
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 1, "1.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 2, "4.6.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 3, "6.2.3.4",
                                                    1, 0, 0));
  netwib_er(netwox_tracertdisc_graph_test2(&graph, phoplist,
                                           NETWOX_TRACERTDISC_METHOD_UDP, 2));

  /**/
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 1, "1.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 2, "4.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 5, "7.2.3.4",
                                                    0, 0, 1));
  netwib_er(netwox_tracertdisc_graph_test2(&graph, phoplist,
                                           NETWOX_TRACERTDISC_METHOD_TCP, 25));

  /**/
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 1, "1.2.3.1",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 2, "7.2.3.1",
                                                    1, 0, 0));
  netwib_er(netwox_tracertdisc_graph_test2(&graph, phoplist,
                                           NETWOX_TRACERTDISC_METHOD_TCP, 25));

  /**/
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 3, "1.2.3.1",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 4, "7.2.3.1",
                                                    1, 0, 0));
  netwib_er(netwox_tracertdisc_graph_test2(&graph, phoplist,
                                           NETWOX_TRACERTDISC_METHOD_TCP, 25));

  /**/
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 1, "1.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 2, "4.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 5, "5.2.3.4",
                                                    0, 0, 1));
  netwib_er(netwox_tracertdisc_graph_test2(&graph, phoplist,
                                           NETWOX_TRACERTDISC_METHOD_TCP, 27));

  /**/
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 1, "1.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 2, "4.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 3, "5.2.3.4",
                                                    1, 1, 0));
  netwib_er(netwox_tracertdisc_graph_test2(&graph, phoplist,
                                           NETWOX_TRACERTDISC_METHOD_TCP, 22));

  /**/
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 1, "1.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 2, "4.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 4, "7.2.3.4",
                                                    1, 1, 0));
  netwib_er(netwox_tracertdisc_graph_test2(&graph, phoplist,
                                           NETWOX_TRACERTDISC_METHOD_TCP, 22));

  /**/
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 1, "1.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 3, "4.3.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 4, "7.2.3.4",
                                                    1, 1, 0));
  netwib_er(netwox_tracertdisc_graph_test2(&graph, phoplist,
                                           NETWOX_TRACERTDISC_METHOD_UDP, 28));

  /**/
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 1, "1.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 2, "4.33.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 3, "4.3.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 4, "7.2.3.4",
                                                    1, 1, 0));
  netwib_er(netwox_tracertdisc_graph_test2(&graph, phoplist,
                                           NETWOX_TRACERTDISC_METHOD_UDP, 29));

  /**/
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 1, "1.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 2, "4.33.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 3, "1.2.3.1",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 4, "7.2.3.4",
                                                    1, 1, 0));
  netwib_er(netwox_tracertdisc_graph_test2(&graph, phoplist,
                                           NETWOX_TRACERTDISC_METHOD_UDP, 29));

  /**/
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 1, "1.2.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 2, "4.33.3.4",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 3, "1.2.3.1",
                                                    0, 0, 0));
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 7, "10.2.3.4",
                                                    0, 0, 1));
  netwib_er(netwox_tracertdisc_graph_test2(&graph, phoplist,
                                           NETWOX_TRACERTDISC_METHOD_UDP, 56));

  /**/
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 7, "7.2.3.6",
                                                    0, 0, 1));
  netwib_er(netwox_tracertdisc_graph_test2(&graph, phoplist,
                                           NETWOX_TRACERTDISC_METHOD_UDP, 29));

  /**/
  netwib_er(netwox_tracertdisc_hoplist_ring_add_txt(phoplist, 7, "7.2.3.4",
                                                    0, 0, 1));
  netwib_er(netwox_tracertdisc_graph_test2(&graph, phoplist,
                                           NETWOX_TRACERTDISC_METHOD_UDP, 29));

  /* close */
  netwib_er(netwox_tracertdisc_hoplist_ring_close(&phoplist));
  netwib_er(netwox_tracertdisc_graph_close(&graph));

  return(NETWIB_ERR_OK);
}

