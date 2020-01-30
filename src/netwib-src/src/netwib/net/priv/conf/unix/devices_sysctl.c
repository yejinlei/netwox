/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
/***
  Used by :
    BSD
 ***/

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_hwtype_init_sys(netwib_uint32 systype,
                                                   netwib_device_hwtype *phwtype)
{

  switch(systype) {
  case IFT_ETHER :
    *phwtype = NETWIB_DEVICE_HWTYPE_ETHER;
    break;
  case IFT_LOOP :
    *phwtype = NETWIB_DEVICE_HWTYPE_LOOPBACK;
    break;
  case IFT_PPP :
    *phwtype = NETWIB_DEVICE_HWTYPE_PPP;
    break;
  case IFT_PARA :
    *phwtype = NETWIB_DEVICE_HWTYPE_PLIP;
    break;
  case IFT_SLIP :
    *phwtype = NETWIB_DEVICE_HWTYPE_SLIP;
    break;
  default :
    *phwtype = NETWIB_DEVICE_HWTYPE_UNKNOWN;
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_devices_sysctl(netwib_priv_confwork *pcw)
{
  netwib_priv_confwork_devices *pcd;
  netwib_priv_confwork_ip *pci;
  netwib_buf bufiflist;
  netwib_data iflist;
  netwib_uint32 i;
  netwib_bool bufiflistset, pcdset, pciset;
  netwib_bool isup, ispointtopoint;
  netwib_c_sizet len;
  netwib_iptype iptype;
  netwib_err ret;
  struct if_msghdr *pifm;
  int mib[6], reti;

  ret = NETWIB_ERR_OK;
  pcdset = NETWIB_FALSE;
  pciset = NETWIB_FALSE;
  bufiflistset = NETWIB_FALSE;
  isup = NETWIB_FALSE;
  ispointtopoint = NETWIB_FALSE;

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
      struct sockaddr_dl *psdl;
      /* it contains one sockaddr_dl */
      psdl = (struct sockaddr_dl *)(pifm + 1);
      netwib_eg(netwib_priv_confwork_devices_init(&pcd));
      pcdset = NETWIB_TRUE;
      /* device */
      netwib_eg(netwib_buf_append_data((netwib_constdata)psdl->sdl_data,
                                       psdl->sdl_nlen, &pcd->device));
      /* mtu */
      pcd->mtu = (pifm->ifm_data).ifi_mtu;
      /* hwtype */
      netwib_eg(netwib_priv_conf_hwtype_init_sys(psdl->sdl_type,
                                                 &pcd->hwtype));
      /* Ethernet */
      if (pcd->hwtype == NETWIB_DEVICE_HWTYPE_ETHER) {
        netwib_c_memcpy(pcd->eth.b, LLADDR(psdl), NETWIB_ETH_LEN);
      }
      /* save flags */
      isup = (pifm->ifm_flags & IFF_UP)?NETWIB_TRUE:NETWIB_FALSE;
      ispointtopoint =(pifm->ifm_flags&IFF_POINTOPOINT)?NETWIB_TRUE:NETWIB_FALSE;
      /* save value */
      netwib_eg(netwib_priv_confwork_devices_add(pcw, pcd));
      pcdset = NETWIB_FALSE;
    }

    /** IP **/
    if (pifm->ifm_type == RTM_NEWADDR && isup) {
      struct ifa_msghdr *pifam;
      netwib_priv_sockaddr_unalign *psa;
      /* it contains ifa_msghdr followed by sockaddrs */
      pifam = (struct ifa_msghdr *)pifm;
      /* initialize the pointer to the first sockaddr */
      psa = (netwib_priv_sockaddr_unalign *)(pifam + 1);
      /* initialize storage */
      netwib_eg(netwib_priv_confwork_ip_init(&pci));
      pciset = NETWIB_TRUE;
      netwib_eg(netwib_buf_append_buf(&pcd->device, &pci->device));
      pci->ispointtopoint = ispointtopoint;
      iptype = NETWIB_IPTYPE_UNKNOWN;
      /* we look if one of the sockaddr contains ip or netmask */
      for (i = 0; i < RTAX_MAX; i++) {
        if ( ! (pifam->ifam_addrs & (1 << i)) ) {
          continue;
        }
        ret = NETWIB_ERR_OK;
        if (i == RTAX_IFA) {
          ret = netwib_priv_sa_ip_init_sai(psa, iptype, &pci->ip);
          if (ret == NETWIB_ERR_OK) iptype = pci->ip.iptype;
        } else if (i == RTAX_NETMASK) {
          ret = netwib_priv_ip_maskprefix_init_sai(psa, iptype, &pci->mask,
                                                  &pci->prefix);
        } else if (i == RTAX_BRD) {
          if (ispointtopoint) {
            ret = netwib_priv_sa_ip_init_sai(psa, iptype, &pci->pointtopointip);
          }
        }
        if (ret == NETWIB_ERR_PAINVALIDTYPE ||
            ret == NETWIB_ERR_PAIPTYPE) {
          ret = NETWIB_ERR_OK;
          goto netwib_trynextentry;
        }
        netwib_eg(ret);
        /* psa points to the next item */
        psa = (netwib_priv_sockaddr_unalign *)(((char *)psa) + NETWIB_FB_ROUNDUP(((struct sockaddr*)psa)->sa_len));
      }
      /* FreeBSD 4.0 sets 0.0.0.0 address for lp0 : skip */
      if (pci->ip.iptype == NETWIB_IPTYPE_IP4 && pci->ip.ipvalue.ip4 == 0) {
        goto netwib_trynextentry;
      }
      /* save value */
      netwib_eg(netwib_priv_confwork_ip_add(pcw, pci));
      pciset = NETWIB_FALSE;
    }

    /** NEXT **/
  netwib_trynextentry:
    if (pcdset) {
      netwib_eg(netwib_priv_confwork_devices_close(&pcd));
    }
    if (pciset) {
      netwib_eg(netwib_priv_confwork_ip_close(&pci));
      pciset = NETWIB_FALSE;
    }
    /* using shift ensures pointer is always aligned */
    netwib_eg(netwib_buf_shift(&bufiflist, -pifm->ifm_msglen, NETWIB_TRUE));
  }

 netwib_gotolabel:
  if (bufiflistset) {
    netwib_er(netwib_buf_close(&bufiflist));
  }
  if (pcdset) {
    netwib_eg(netwib_priv_confwork_devices_close(&pcd));
  }
  if (pciset) {
    netwib_eg(netwib_priv_confwork_ip_close(&pci));
  }
  return(ret);
}
