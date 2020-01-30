/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
#if NETWIBDEF_HAVEFUNC_ETHER_HOSTTON==1
#define NETWIB_ETHINITIP_ETCETHERS 1
static netwib_err netwib_priv_eth_init_ip_etcethers(netwib_constip *pip,
                                                    netwib_eth *peth)
{
  netwib_buf buf;
  netwib_eth eth;
  netwib_char ips[16];
  netwib_string str;
  struct ether_addr ea;
  int reti;

  netwib_er(netwib_buf_init_ext_arrayempty(ips, sizeof(ips), &buf));
  netwib_er(netwib_priv_ip_buf_append_ip4(pip, &buf));
  netwib_er(netwib_buf_ref_string(&buf, &str));
  reti = ether_hostton(str, &ea);
  if (reti) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

#if defined NETWIBDEF_SYSNAME_FreeBSD || defined NETWIBDEF_SYSNAME_MacOS
  netwib_c_memcpy(eth.b, ea.octet, NETWIB_ETH_LEN);
#else
  netwib_c_memcpy(eth.b, ea.ether_addr_octet, NETWIB_ETH_LEN);
#endif

  if (peth != NULL) {
    *peth = eth;
  }

  return(NETWIB_ERR_OK);
}
#else
 #define NETWIB_ETHINITIP_ETCETHERS 0
#endif

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Windows
#define NETWIB_ETHINITIP_IPHLPAPI 1
static netwib_err netwib_priv_eth_init_ip_iphlpapi(netwib_constip *pip,
                                                   netwib_eth *peth)
{
  ULONG ethlocal[NETWIB_ETH_LEN], bufsize;
  DWORD err;

  if (!netwib_global_dll_iphlpapi.sendarp_avail) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  if (pip->iptype != NETWIB_IPTYPE_IP4) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  bufsize = NETWIB_ETH_LEN;
  err = netwib_global_dll_iphlpapi.SendARP(netwib_priv_htonl(pip->ipvalue.ip4),
                                           0, ethlocal, &bufsize);
  if (err != NO_ERROR) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  if (bufsize != NETWIB_ETH_LEN) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  if (peth != NULL) {
    netwib_c_memcpy(peth->b, ethlocal, NETWIB_ETH_LEN);
  }

  return(NETWIB_ERR_OK);
}
#else
 #define NETWIB_ETHINITIP_IPHLPAPI 0
#endif

/*-------------------------------------------------------------*/
#if NETWIBDEF_SUPPORT_SNIFF == 1 && NETWIBDEF_SUPPORT_SPOOF == 1
 #define NETWIB_ETHINITIP_SPOOFSNIFF 1
#else
 #define NETWIB_ETHINITIP_SPOOFSNIFF 0
#endif
#if NETWIB_ETHINITIP_SPOOFSNIFF == 1
static netwib_err netwib_priv_eth_init_ip_spoofsniff_rep6(netwib_buf *popts,
                                                          netwib_eth *peth)
{
  netwib_icmp6nd icmp6nd;
  netwib_uint32 skipsize;
  netwib_err ret;

  while (NETWIB_TRUE) {
    if (popts->beginoffset >= popts->endoffset) {
      break;
    }
    ret = netwib_pkt_decode_icmp6nd(popts, &icmp6nd, &skipsize);
    if (ret == NETWIB_ERR_OK) {
      if (icmp6nd.type == NETWIB_ICMP6NDTYPE_DSTLINK) {
        if (peth != NULL) {
          *peth = icmp6nd.opt.link.linkad;
        }
        return(NETWIB_ERR_OK);
      }
    }
    popts->beginoffset += skipsize;
  }

  return(NETWIB_ERR_NOTCONVERTED);
}
static netwib_err netwib_priv_eth_init_ip_spoofsniff_rep(netwib_constbuf *ppkt,
                                                         netwib_constip *pip,
                                                         netwib_eth *peth)
{
  netwib_arphdr arpheader;
  netwib_iphdr ipheader;
  netwib_icmp6 icmp6;
  netwib_cmp cmp;
  netwib_err ret;

  /*
  netwib_er(netwib_pkt_link_display(NETWIB_DEVICE_DLTTYPE_ETHER, ppkt, NULL,
                                    NETWIB_ENCODETYPE_ARRAY,
                                    NETWIB_ENCODETYPE_DUMP));
  */

  /* IPv4 */
  if (pip->iptype == NETWIB_IPTYPE_IP4) {
    ret = netwib_pkt_decode_linkarp(NETWIB_DEVICE_DLTTYPE_ETHER, ppkt,
                                    NULL, &arpheader);
    if (ret == NETWIB_ERR_OK) {
      if (arpheader.op == NETWIB_ARPHDROP_ARPREP) {
        netwib_er(netwib_ip_cmp(&arpheader.ipsrc, pip, &cmp));
        if (cmp == NETWIB_CMP_EQ) {
          if (peth != NULL) {
            *peth = arpheader.ethsrc;
          }
          return(NETWIB_ERR_OK);
        }
      }
    }
    return(NETWIB_ERR_NOTCONVERTED);
  }

  /* IPv6 */
  ret = netwib_pkt_decode_linkipicmp6(NETWIB_DEVICE_DLTTYPE_ETHER, ppkt,
                                      NULL, &ipheader, &icmp6);
  if (ret == NETWIB_ERR_OK) {
    if (ipheader.iptype == NETWIB_IPTYPE_IP6 && ipheader.ttl == 255) {
      if (icmp6.type == NETWIB_ICMP6TYPE_NEIGHBORADVERT &&
          icmp6.code == NETWIB_ICMP6CODE_NEIGHBORADVERT) {
        netwib_er(netwib_ip_cmp(&icmp6.msg.neighboradvert.target, pip, &cmp));
        if (cmp == NETWIB_CMP_EQ) {
          return(netwib_priv_eth_init_ip_spoofsniff_rep6(&icmp6.msg.neighboradvert.options, peth));
        }
      }
    }
  }

  return(NETWIB_ERR_NOTCONVERTED);
}
static netwib_err netwib_priv_eth_init_ip_spoofsniff4(netwib_io *pio,
                                                      netwib_buf *ppkt,
                                                      netwib_constip *pip,
                                                      netwib_eth *peth)
{
  netwib_time endtime;
  netwib_bool event;
  netwib_err ret;

  netwib_er(netwib_time_init_now(&endtime));
  netwib_er(netwib_time_plus_msec(&endtime, 300));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    /* wait */
    netwib_er(netwib_io_wait(pio, NETWIB_IO_WAYTYPE_READ, &endtime, &event));
    if (!event) {
      ret = NETWIB_ERR_NOTCONVERTED;
      break;
    }
    /* read */
    netwib__buf_reinit(ppkt);
    ret = netwib_io_read(pio, ppkt);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      /* simply ignore, and try next */
    } else if (ret == NETWIB_ERR_OK) {
      ret = netwib_priv_eth_init_ip_spoofsniff_rep(ppkt, pip, peth);
      if (ret != NETWIB_ERR_NOTCONVERTED) {
        break;
      }
    } else {
      break;
    }
  }

  return(ret);
}
static netwib_err netwib_priv_eth_init_ip_spoofsniff_req(netwib_io *piospoof,
                                                         netwib_buf *ppkt,
                                                      netwib_consteth *psrceth,
                                                      netwib_constip *psrcip,
                                                         netwib_constip *pip)
{
  netwib_linkhdr linkhdr;
  netwib_arphdr arpheader;
  netwib_iphdr ipheader;
  netwib_icmp6 icmp6;
  netwib_icmp6nd icmp6nd;
  netwib_byte array[80];
  netwib_buf buf;

  /* construct packet */
  netwib_er(netwib_linkhdr_initdefault(NETWIB_DEVICE_DLTTYPE_ETHER, &linkhdr));
  linkhdr.hdr.ether.src = *psrceth;
  if (pip->iptype == NETWIB_IPTYPE_IP4) {
    netwib_er(netwib_eth_init_fields(0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                                     &linkhdr.hdr.ether.dst));
    linkhdr.hdr.ether.type = NETWIB_ETHERHDRTYPE_ARP;
    netwib_er(netwib_arphdr_initdefault(&arpheader));
    arpheader.op = NETWIB_ARPHDROP_ARPREQ;
    arpheader.ethsrc = linkhdr.hdr.ether.src;
    arpheader.ipsrc = *psrcip;
    netwib_er(netwib_eth_init_fields(0,0,0,0,0,0, &arpheader.ethdst));
    arpheader.ipdst = *pip;
    netwib_er(netwib_pkt_append_linkarp(&linkhdr, &arpheader, ppkt));
  } else {
    /* rfc 2373 :
         ip=1234:5678:90ab:cdef:1122:3344:5566:7788 -> ip=FF02::1:FF66:7788
       rfc 2464 :
         ip=1234:5678:90ab:cdef:1122:3344:5566:7788 -> eth=33:33:55:66:77:88
       example :
         pip = 1234:5678:90ab:cdef:1122:3344:5566:7788
         ip6.dst = FF02::1:FF66:7788
         eth.dst = 33:33:FF:66:77:88 (computed from ip6.dst and not pip)
    */
    netwib_er(netwib_eth_init_fields(0x33, 0x33, 0xFF,
                                     pip->ipvalue.ip6.b[13],
                                     pip->ipvalue.ip6.b[14],
                                     pip->ipvalue.ip6.b[15],
                                     &linkhdr.hdr.ether.dst));
    netwib_er(netwib_iphdr_initdefault(NETWIB_IPTYPE_IP6, &ipheader));
    ipheader.src = *psrcip;
    netwib_er(netwib_ip_init_ip6_fields(0xFF020000u, 0, 1, 0xFF000000u,
                                        &ipheader.dst));
    ipheader.dst.ipvalue.ip6.b[13] = pip->ipvalue.ip6.b[13];
    ipheader.dst.ipvalue.ip6.b[14] = pip->ipvalue.ip6.b[14];
    ipheader.dst.ipvalue.ip6.b[15] = pip->ipvalue.ip6.b[15];
    ipheader.ttl = 255;
    netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_NEIGHBORSOLICIT,
                                       &icmp6));
    icmp6.msg.neighborsolicit.target = *pip;
    netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));
    netwib_er(netwib_icmp6nd_initdefault(NETWIB_ICMP6NDTYPE_SRCLINK,
                                         &icmp6nd));
    icmp6nd.opt.link.linkad = *psrceth;
    netwib_er(netwib_pkt_append_icmp6nd(&icmp6nd, &buf));
    icmp6.msg.neighborsolicit.options = buf;
    netwib_er(netwib_pkt_append_linkipicmp6(&linkhdr, &ipheader, &icmp6,
                                            ppkt));
  }

  /* send packet */
  netwib_er(netwib_io_write(piospoof, ppkt));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_eth_init_ip_spoofsniff3(netwib_constbuf *pdevice,
                                                      netwib_consteth *psrceth,
                                                      netwib_constip *psrcip,
                                                      netwib_constip *pip,
                                                      netwib_eth *peth)
{
  netwib_io *piosniff, *piospoof;
  netwib_buf filter, pkt;
  netwib_bool sniffset, spoofset;
  netwib_err ret;

  /* initialize resources */
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_buf_init_ext_string("", &filter));
  if (pip->iptype == NETWIB_IPTYPE_IP4) {
    netwib_er(netwib_buf_init_ext_string("arp", &filter));
  }
  ret = NETWIB_ERR_OK;
  sniffset = NETWIB_FALSE;
  spoofset = NETWIB_FALSE;
  netwib_eg(netwib_io_init_sniff(pdevice, &filter, &piosniff));
  sniffset = NETWIB_TRUE;
  netwib_eg(netwib_io_init_spoof_link(pdevice, &piospoof));
  spoofset = NETWIB_TRUE;

  /* send request */
  netwib_eg(netwib_priv_eth_init_ip_spoofsniff_req(piospoof, &pkt, psrceth,
                                                   psrcip, pip));

  /* sniff during less than 300 milli-second */
  ret = netwib_priv_eth_init_ip_spoofsniff4(piosniff, &pkt, pip, peth);

  /* end */
 netwib_gotolabel:
  netwib_er(netwib_buf_close(&pkt));
  if (sniffset) {
    netwib_eg(netwib_io_close(&piosniff));
  }
  if (spoofset) {
    netwib_eg(netwib_io_close(&piospoof));
  }
  return(ret);
}
static netwib_err netwib_priv_eth_init_ip_spoofsniff2(netwib_constbuf *pdevice,
                                                      netwib_uint32 devnum,
                                                      netwib_consteth *psrceth,
                                                      netwib_constip *pip,
                                                      netwib_eth *peth)
{
  netwib_conf_ip conf;
  netwib_conf_ip_index *pconfindex;
  netwib_ip fakeip;
  netwib_uint32 high=0, low=0;
  netwib_err ret;

  netwib_er(netwib_conf_ip_index_init(&conf, &pconfindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_conf_ip_index_next(pconfindex);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) {
        /* no address found: try using a fake address, avoiding
           broadcast addresses */
        fakeip = *pip;
        if (fakeip.iptype == NETWIB_IPTYPE_IP4) {
          high = fakeip.ipvalue.ip4 & 0xFFFFFFFCu;
          low = fakeip.ipvalue.ip4 & 3;
        } else {
          high = fakeip.ipvalue.ip6.b[NETWIB_IP6_LEN-1] & 0xFFFFFFFCu;
          low = fakeip.ipvalue.ip6.b[NETWIB_IP6_LEN-1] & 3;
        }
        switch(low) {
        case 0: low = 1; break;
        case 1: low = 2; break;
        case 2: low = 1; break;
        case 3: low = 2; break;
        }
        if (fakeip.iptype == NETWIB_IPTYPE_IP4) {
          fakeip.ipvalue.ip4 = high | low;
        } else {
          fakeip.ipvalue.ip6.b[NETWIB_IP6_LEN-1] = (netwib_byte)(high | low);
        }
        ret = netwib_priv_eth_init_ip_spoofsniff3(pdevice, psrceth, &fakeip,
                                                  pip, peth);
      }
      break;
    }
    if (conf.devnum == devnum) {
      if (conf.ip.iptype == pip->iptype) {
        ret = netwib_priv_eth_init_ip_spoofsniff3(pdevice, psrceth, &conf.ip,
                                                  pip, peth);
        break;
      }
    }
  }
  netwib_er(netwib_conf_ip_index_close(&pconfindex));

  return(ret);
}
static netwib_err netwib_priv_eth_init_ip_spoofsniff(netwib_constip *pip,
                                                     netwib_eth *peth)
{
  netwib_conf_devices conf;
  netwib_conf_devices_index *pconfindex;
  netwib_err ret;

  netwib_er(netwib_conf_devices_index_init(&conf, &pconfindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_conf_devices_index_next(pconfindex);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_NOTCONVERTED;
      break;
    }
    if (conf.hwtype == NETWIB_DEVICE_HWTYPE_ETHER) {
      ret = netwib_priv_eth_init_ip_spoofsniff2(&conf.device, conf.devnum,
                                                &conf.eth, pip, peth);
      if (ret == NETWIB_ERR_OK || ret != NETWIB_ERR_NOTCONVERTED) {
        break;
      }
    }
  }
  netwib_er(netwib_conf_devices_index_close(&pconfindex));

  return(ret);
}
#endif

/*-------------------------------------------------------------*/
static netwib_err netwib_eth_init_ip2(netwib_constip *pip,
                                      netwib_eth *peth)
{
  netwib_err ret = NETWIB_ERR_NOTCONVERTED;

 #if NETWIB_ETHINITIP_ETCETHERS == 1
  ret = netwib_priv_eth_init_ip_etcethers(pip, peth);
  if (ret == NETWIB_ERR_OK) return(NETWIB_ERR_OK);
  if (ret != NETWIB_ERR_NOTCONVERTED) return(ret);
 #endif

 #if NETWIB_ETHINITIP_IPHLPAPI == 1
  ret = netwib_priv_eth_init_ip_iphlpapi(pip, peth);
  if (ret == NETWIB_ERR_OK) return(NETWIB_ERR_OK);
  if (ret != NETWIB_ERR_NOTCONVERTED) return(ret);
 #endif

  ret = netwib_priv_confrel_arpcache_eth(pip, peth);
  if (ret == NETWIB_ERR_OK) return(NETWIB_ERR_OK);
  if (ret != NETWIB_ERR_NOTCONVERTED) return(ret);

  /* do it last because this is very long (n*300ms) */
 #if NETWIB_ETHINITIP_SPOOFSNIFF == 1
  ret = netwib_priv_eth_init_ip_spoofsniff(pip, peth);
  if (ret == NETWIB_ERR_OK) return(NETWIB_ERR_OK);
  if (ret != NETWIB_ERR_NOTCONVERTED) return(ret);
 #endif

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_eth_init_ip(netwib_constip *pip,
                              netwib_eth *peth)
{
  netwib_eth eth;
  netwib_err ret;

  ret = netwib_priv_confglo_arpcache_eth(pip, peth);
  if (ret == NETWIB_ERR_NOTCONVERTED) {
    ret = netwib_eth_init_ip2(pip, &eth);
    if (ret == NETWIB_ERR_OK) {
      /* add value in ARP cache */
      netwib_er(netwib_priv_confglo_arpcache_add(NULL, &eth, pip));
      if (peth != NULL) *peth = eth;
    }
  }
  return(ret);
}
