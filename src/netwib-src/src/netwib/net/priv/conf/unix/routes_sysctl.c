/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
/***
  Used by :
    BSD
 ***/

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_routes_sysctl_dev(netwib_uint32 ifnum,
                                                     netwib_buf *pdevice,
                                                     netwib_bool *pisloopback)
{
  netwib_buf bufiflist;
  netwib_data iflist;
  netwib_bool bufiflistset;
  netwib_c_sizet len;
  netwib_err ret;
  struct if_msghdr *pifm;
  int mib[6], reti;

  ret = NETWIB_ERR_OK;
  bufiflistset = NETWIB_FALSE;
  *pisloopback = NETWIB_FALSE;

  /* obtain list of interfaces */
  mib[0] = CTL_NET;
  mib[1] = PF_ROUTE;
  mib[2] = 0;
  mib[3] = AF_UNSPEC;
  mib[4] = NET_RT_IFLIST;
  mib[5] = 0;
  reti = sysctl(mib, 6, NULL, &len, NULL, 0);
  netwib_ir(reti, NETWIB_ERR_FUSYSCTL);
  netwib_er(netwib_buf_init_malloc(len, &bufiflist));
  bufiflistset = NETWIB_TRUE;
  iflist = netwib__buf_ref_data_ptr(&bufiflist);
  reti = sysctl(mib, 6, iflist, &len, NULL, 0);
  netwib_ig(reti, NETWIB_ERR_FUSYSCTL);
  bufiflist.endoffset = bufiflist.beginoffset + len;
  pifm = (struct if_msghdr *)(void*)iflist;

  /* loop on each interface */
  while (NETWIB_TRUE) {
    /* check ending condition */
    if (bufiflist.beginoffset >= bufiflist.endoffset) {
      break;
    }

    /** DEVICE **/
    if (pifm->ifm_type == RTM_IFINFO) {
      if (pifm->ifm_index == ifnum) {
        struct sockaddr_dl *psdl;
        psdl = (struct sockaddr_dl *)(pifm + 1);
        netwib_eg(netwib_buf_append_data((netwib_constdata)psdl->sdl_data,
                                         psdl->sdl_nlen,
                                         pdevice));
        if (pifm->ifm_flags & IFT_LOOP) {
          *pisloopback = NETWIB_TRUE;
        }
        netwib_goto(NETWIB_ERR_OK);
      }
    }

    /** NEXT **/
    /* using shift ensures pointer is always aligned */
    netwib_eg(netwib_buf_shift(&bufiflist, -pifm->ifm_msglen, NETWIB_TRUE));
  }
  netwib_eg(netwib_buf_append_fmt(pdevice, "unk%{uint32}", ifnum));

 netwib_gotolabel:
  if (bufiflistset) {
    netwib_er(netwib_buf_close(&bufiflist));
  }
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_routes_sysctl(netwib_priv_confwork *pcw)
{
  netwib_priv_confwork_routes *pcr;
  netwib_buf bufrtlist;
  netwib_data rtlist;
  netwib_uint32 i;
  netwib_bool bufrtlistset, pcrset, dstset, netmaskset, isloopback;
  netwib_c_sizet len;
  netwib_iptype iptype;
  netwib_err ret;
  struct rt_msghdr *prtm;
  netwib_priv_sockaddr_unalign *psa;
  int mib[6], reti;

  ret = NETWIB_ERR_OK;
  pcrset = NETWIB_FALSE;
  bufrtlistset = NETWIB_FALSE;

  /* obtain list of interfaces */
  mib[0] = CTL_NET;
  mib[1] = PF_ROUTE;
  mib[2] = 0;
  mib[3] = AF_UNSPEC;
  mib[4] = NET_RT_DUMP;
  mib[5] = 0;
  reti = sysctl(mib, 6, NULL, &len, NULL, 0);
  netwib_ir(reti, NETWIB_ERR_FUSYSCTL);
  netwib_er(netwib_buf_init_malloc(len, &bufrtlist));
  bufrtlistset = NETWIB_TRUE;
  rtlist = netwib__buf_ref_data_ptr(&bufrtlist);
  reti = sysctl(mib, 6, rtlist, &len, NULL, 0);
  netwib_ig(reti, NETWIB_ERR_FUSYSCTL);
  bufrtlist.endoffset = bufrtlist.beginoffset + len;
  prtm = (struct rt_msghdr *)(void*)rtlist;

  /* loop on each interface */
  while (NETWIB_TRUE) {
    /* check ending condition */
    if (bufrtlist.beginoffset >= bufrtlist.endoffset) {
      break;
    }
    /* skip not up */
    if ( ! (prtm->rtm_flags & RTF_UP) ) {
      goto netwib_trynextentry;
    }
    if ( ! (prtm->rtm_flags & RTF_GATEWAY) ) {
      goto netwib_trynextentry;
    }
    /* prepare storage */
    netwib_eg(netwib_priv_confwork_routes_init(&pcr));
    pcrset = NETWIB_TRUE;
    /* obtain all ip info */
    psa = (netwib_priv_sockaddr_unalign *)(prtm + 1);
    dstset = NETWIB_FALSE;
    netmaskset = NETWIB_FALSE;
    iptype = NETWIB_IPTYPE_UNKNOWN;
    for (i = 0; i < RTAX_MAX; i++) {
      if ( ! (prtm->rtm_addrs & (1 << i)) ) {
        continue;
      }
      ret = NETWIB_ERR_OK;
      if (i == RTAX_DST) {
        ret = netwib_priv_sa_ip_init_sai(psa, iptype, &pcr->dst);
        if (ret == NETWIB_ERR_OK) iptype = pcr->dst.iptype;
        dstset = NETWIB_TRUE;
      } else if (i == RTAX_NETMASK) {
        ret = netwib_priv_ip_maskprefix_init_sai(psa, iptype, &pcr->mask,
                                                 &pcr->prefix);
        netmaskset = NETWIB_TRUE;
      } else if (i == RTAX_GATEWAY) {
        ret = netwib_priv_sa_ip_init_sai(psa, iptype, &pcr->gw);
        if (ret == NETWIB_ERR_OK) iptype = pcr->gw.iptype;
        pcr->gwset = NETWIB_TRUE;
      }
      if (ret == NETWIB_ERR_PAINVALIDTYPE ||
          ret == NETWIB_ERR_PAIPTYPE) {
        ret = NETWIB_ERR_OK;
        goto netwib_trynextentry;
      }
      netwib_eg(ret);
      /* psa points to the next item */
      psa = (netwib_priv_sockaddr_unalign *)(((char *)psa) + NETWIB_FB_ROUNDUP(((struct sockaddr *)psa)->sa_len));
    }
    /* in case no destination was set: bad entry */
    if (!dstset) {
      goto netwib_trynextentry;
    }
    /* if netmask is not set, it is full or none */
    if (!netmaskset) {
      if (prtm->rtm_flags & RTF_HOST) {
        netwib_eg(netwib_ip_init_ip4(0xFFFFFFFFu, &pcr->mask));
        pcr->prefix = 128;
      } else {
        netwib_eg(netwib_ip_init_ip4(0, &pcr->mask));
        pcr->prefix = 0;
      }
    }
    /* get device */
    netwib_eg(netwib_priv_conf_routes_sysctl_dev(prtm->rtm_index,
                                                 &pcr->device,
                                                 &isloopback));
    if (isloopback) {
      /* FreeBSD 4.0 adds useless routes for IPv6 under loopback.
         OpenBSD 2.7 adds useless routes for IPv4 under loopback. */
      goto netwib_trynextentry;
    }
    /* get metric (I am not sure this is the correct parameter to use) */
    pcr->metric = prtm->rtm_rmx.rmx_hopcount;
    /* save value */
    netwib_eg(netwib_priv_confwork_routes_add(pcw, pcr));
    pcrset = NETWIB_FALSE;

    /** NEXT **/
  netwib_trynextentry:
    if (pcrset) {
      netwib_eg(netwib_priv_confwork_routes_close(&pcr));
      pcrset = NETWIB_FALSE;
    }
    netwib_eg(netwib_buf_shift(&bufrtlist, -prtm->rtm_msglen, NETWIB_TRUE));
  }

 netwib_gotolabel:
  if (bufrtlistset) {
    netwib_er(netwib_buf_close(&bufrtlist));
  }
  if (pcrset) {
    netwib_eg(netwib_priv_confwork_routes_close(&pcr));
  }
  return(ret);
}
