/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confclean_devices_dup(netwib_constptr piteminf,
                                                    netwib_constptr pitemsup,
                                                    netwib_ptr pinfos,
                                                    netwib_cmp *pcmp)
{
  netwib_priv_confwork_devices *pcd1 =
    (netwib_priv_confwork_devices *)(netwib_uintptr)piteminf;
  netwib_priv_confwork_devices *pcd2 =
    (netwib_priv_confwork_devices *)(netwib_uintptr)pitemsup;
  netwib_cmp cmp;

  netwib_er(netwib_buf_cmp(&pcd1->device, &pcd2->device, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    *pcmp = cmp;
    return(NETWIB_ERR_OK);
  }

  /* now, devices are equal, so they must have the same hwtype.
     We eventually update unknown values */
  if (pcd1->hwtype != pcd2->hwtype) {
    if (pcd1->hwtype == NETWIB_DEVICE_HWTYPE_UNKNOWN) {
      pcd1->hwtype = pcd2->hwtype;
      pcd1->eth = pcd2->eth;
    } else if (pcd2->hwtype == NETWIB_DEVICE_HWTYPE_UNKNOWN) {
      pcd2->hwtype = pcd1->hwtype;
      pcd2->eth = pcd1->eth;
    } else {
      /* they are different */
      *pcmp = NETWIB_CMP_LT;
      return(NETWIB_ERR_OK);
    }
  }

  /* eventually update null Ethernet */
  if (pcd1->hwtype == NETWIB_DEVICE_HWTYPE_ETHER) {
    netwib_er(netwib_eth_cmp(&pcd1->eth, &pcd2->eth, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      if (pcd1->eth.b[0] == 0 && pcd1->eth.b[1] == 0 &&
          pcd1->eth.b[2] == 0 && pcd1->eth.b[3] == 0 &&
          pcd1->eth.b[4] == 0 && pcd1->eth.b[5] == 0 ) {
        pcd1->eth = pcd2->eth;
      } else if (pcd2->eth.b[0] == 0 && pcd2->eth.b[1] == 0 &&
                 pcd2->eth.b[2] == 0 && pcd2->eth.b[3] == 0 &&
                 pcd2->eth.b[4] == 0 && pcd2->eth.b[5] == 0 ) {
        pcd2->eth = pcd1->eth;
      } else {
        /* they are different */
        *pcmp = cmp;
        return(NETWIB_ERR_OK);
      }
    }
  }

  /* ignore differences in MTU : only update or choose the min */
  if (pcd1->mtu != pcd2->mtu) {
    if (pcd1->mtu == 0) {
      pcd1->mtu = pcd2->mtu;
    } else if (pcd2->mtu == 0) {
      pcd2->mtu = pcd1->mtu;
    } else {
      if (pcd1->mtu > pcd2->mtu) {
        pcd1->mtu = pcd2->mtu;
      }
    }
  }

  /* if we are here, they are equal, so the second will be removed */
  *pcmp = NETWIB_CMP_EQ;
  pinfos = pinfos; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confclean_ip_dup(netwib_constptr piteminf,
                                               netwib_constptr pitemsup,
                                               netwib_ptr pinfos,
                                               netwib_cmp *pcmp)
{
  netwib_priv_confwork_ip *pci1 =
    (netwib_priv_confwork_ip *)(netwib_uintptr)piteminf;
  netwib_priv_confwork_ip *pci2 =
    (netwib_priv_confwork_ip *)(netwib_uintptr)pitemsup;
  netwib_string str;
  netwib_cmp cmp;

  netwib_er(netwib_ip_cmp(&pci1->ip, &pci2->ip, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    *pcmp = cmp;
    return(NETWIB_ERR_OK);
  }

  /* now, ip are equal, so they must have the same mask */
  if (pci1->ip.iptype == NETWIB_IPTYPE_IP4) {
    netwib_er(netwib_ip_cmp(&pci1->mask, &pci2->mask, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      *pcmp = cmp;
      return(NETWIB_ERR_OK);
    }
  } else {
    if (pci1->prefix != pci2->prefix) {
      *pcmp = (pci1->prefix < pci2->prefix)?NETWIB_CMP_LT:NETWIB_CMP_GT;
      return(NETWIB_ERR_OK);
    }
  }

  /* now, ip/mask are equal, so they must have the same ppp.
     We eventually update unknown values */
  if (pci1->ispointtopoint && pci2->ispointtopoint) {
    netwib_er(netwib_ip_cmp(&pci1->pointtopointip, &pci2->pointtopointip,
                            &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      *pcmp = cmp;
      return(NETWIB_ERR_OK);
    }
  } else if (pci1->ispointtopoint) {
    pci2->ispointtopoint = NETWIB_TRUE;
    pci2->pointtopointip = pci1->pointtopointip;
  } else if (pci2->ispointtopoint) {
    pci1->ispointtopoint = NETWIB_TRUE;
    pci1->pointtopointip = pci2->pointtopointip;
  }

  /* deal with devices */
  netwib_er(netwib_buf_cmp(&pci1->device, &pci2->device, &cmp));
  if (cmp == NETWIB_CMP_EQ) {
    *pcmp = NETWIB_CMP_EQ;
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_buf_ref_string(&pci1->device, &str));
  if (str[0] == '\0') {
    netwib_er(netwib_buf_append_buf(&pci2->device, &pci1->device));
    *pcmp = NETWIB_CMP_EQ;
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_buf_ref_string(&pci2->device, &str));
  if (str[0] == '\0') {
    netwib_er(netwib_buf_append_buf(&pci1->device, &pci2->device));
    *pcmp = NETWIB_CMP_EQ;
    return(NETWIB_ERR_OK);
  }

  *pcmp = cmp;
  pinfos = pinfos; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confclean_arpcache_setdev(netwib_priv_confwork *pcw,
                                                        netwib_priv_confwork_arpcache *pca)
{
  netwib_ring_index *pringindex;
  netwib_priv_confwork_ip *pci;
  netwib_cmp cmp;
  netwib_err ret;

  netwib_er(netwib_ring_index_init(pcw->pip, &pringindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pci);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_eg(netwib_priv_ip_net_cmp_ipmaskprefix(&pci->ip, &pca->ip,
                                                  &pci->mask,
                                                  pci->prefix, &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      netwib_eg(netwib_buf_append_buf(&pci->device, &pca->device));
    }
  }

 netwib_gotolabel:
  netwib_er(netwib_ring_index_close(&pringindex));
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confclean_routes_setdevsrc(netwib_priv_confwork *pcw,
                                                         netwib_priv_confwork_routes *pcr)
{
  netwib_ring_index *pringindex;
  netwib_priv_confwork_ip *pci;
  netwib_ip possiblesrc, possiblegw;
  netwib_buf possibledevice;
  netwib_cmp cmp;
  netwib_bool possiblesrcset, possiblegwset, possibledeviceset;
  netwib_bool pcrdeviceset, foundsure;
  netwib_uint32 foundpossiblelevel;
  netwib_err ret;

  pcrdeviceset = NETWIB_TRUE;
  if (pcr->device.beginoffset == pcr->device.endoffset) {
    pcrdeviceset = NETWIB_FALSE;
  }

  possiblesrc.iptype = NETWIB_IPTYPE_IP4; /* compiler warning */
  possiblegw.iptype = NETWIB_IPTYPE_IP4; /* compiler warning */
#define NETWIB_FOUNDPOSSIBLELEVEL_NOTFOUND     0
#define NETWIB_FOUNDPOSSIBLELEVEL_FOUNDDEVNOIP 1
#define NETWIB_FOUNDPOSSIBLELEVEL_FOUNDDEVIP   2
#define NETWIB_FOUNDPOSSIBLELEVEL_FOUNDLAN     3
  foundpossiblelevel = NETWIB_FOUNDPOSSIBLELEVEL_NOTFOUND;

  netwib_er(netwib_buf_init_mallocdefault(&possibledevice));
  netwib_er(netwib_ring_index_init(pcw->pip, &pringindex));
  foundsure = NETWIB_FALSE;
  possiblesrcset = NETWIB_FALSE;
  possiblegwset = NETWIB_FALSE;
  possibledeviceset = NETWIB_FALSE;
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pci);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }

    /* if dst is a local device */
    netwib_eg(netwib_ip_cmp(&pcr->dst, &pci->ip, &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      if (!pcrdeviceset) {
        netwib_eg(netwib_buf_append_buf(&pci->device, &pcr->device));
      }
      pcr->srcset = NETWIB_FALSE;
      foundsure = NETWIB_TRUE;
      break;
    }

    /* if src is a local device */
    if (pcr->srcset) {
      netwib_eg(netwib_ip_cmp(&pcr->src, &pci->ip, &cmp));
      if (cmp == NETWIB_CMP_EQ) {
        if (!pcrdeviceset) {
          netwib_eg(netwib_buf_append_buf(&pci->device, &pcr->device));
        }
        if (pci->ispointtopoint) {
          if (!pcr->gwset) {
            pcr->gwset = NETWIB_TRUE;
            pcr->gw = pci->pointtopointip;
          }
        }
        foundsure = NETWIB_TRUE;
        break;
      }
    }

    /* if gw or dst is on the LAN */
    netwib_eg(netwib_priv_ip_net_cmp_ipmaskprefix(&pci->ip, &pcr->dst,
                                                  &pci->mask,
                                                  pci->prefix, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      if (pcr->gwset) {
        netwib_eg(netwib_priv_ip_net_cmp_ipmaskprefix(&pci->ip, &pcr->gw,
                                                      &pci->mask,
                                                      pci->prefix, &cmp));
      }
    }
    if (cmp == NETWIB_CMP_EQ) {
      if (pcrdeviceset) {
        netwib_eg(netwib_buf_cmp(&pci->device, &pcr->device, &cmp));
        if (cmp == NETWIB_CMP_EQ) {
          if (!pcr->srcset) {
            pcr->srcset = NETWIB_TRUE;
            pcr->src = pci->ip;
          }
          foundsure = NETWIB_TRUE;
          break;
        }
      }
      if (foundpossiblelevel != NETWIB_FOUNDPOSSIBLELEVEL_FOUNDLAN) {
        foundpossiblelevel = NETWIB_FOUNDPOSSIBLELEVEL_FOUNDLAN;
        possibledeviceset = NETWIB_TRUE;
        netwib__buf_reinit(&possibledevice);
        netwib_eg(netwib_buf_append_buf(&pci->device, &possibledevice));
        possiblesrcset = NETWIB_TRUE;
        possiblesrc = pci->ip;
        if (pci->ispointtopoint) {
          possiblegwset = NETWIB_TRUE;
          possiblegw = pci->pointtopointip;
        } else {
          possiblegwset = NETWIB_FALSE;
        }
      }
    }

    /* we might also only have device set */
    if (pcrdeviceset) {
      netwib_eg(netwib_buf_cmp(&pci->device, &pcr->device, &cmp));
      if (cmp == NETWIB_CMP_EQ) {
        netwib_bool setposs = NETWIB_FALSE;
        if (foundpossiblelevel == NETWIB_FOUNDPOSSIBLELEVEL_NOTFOUND) {
          if (pci->ip.iptype == pcr->dst.iptype) {
            foundpossiblelevel = NETWIB_FOUNDPOSSIBLELEVEL_FOUNDDEVIP;
          } else {
            foundpossiblelevel = NETWIB_FOUNDPOSSIBLELEVEL_FOUNDDEVNOIP;
          }
          setposs = NETWIB_TRUE;
        } else if (foundpossiblelevel==NETWIB_FOUNDPOSSIBLELEVEL_FOUNDDEVNOIP) {
          if (pci->ip.iptype == pcr->dst.iptype) {
            foundpossiblelevel = NETWIB_FOUNDPOSSIBLELEVEL_FOUNDDEVIP;
            setposs = NETWIB_TRUE;
          }
        }
        if (setposs) {
          possiblesrcset = NETWIB_TRUE;
          possiblesrc = pci->ip;
          if (pci->ispointtopoint) {
            possiblegwset = NETWIB_TRUE;
            possiblegw = pci->pointtopointip;
          }
        }
      }
    }

  }
  if (!foundsure) {
    if (foundpossiblelevel != NETWIB_FOUNDPOSSIBLELEVEL_NOTFOUND) {
      if (possibledeviceset) {
        if (!pcrdeviceset) {
          netwib_eg(netwib_buf_append_buf(&possibledevice, &pcr->device));
        }
      }
      if (possiblesrcset) {
        if (!pcr->srcset) {
          pcr->srcset = NETWIB_TRUE;
          pcr->src = possiblesrc;
        }
      }
      if (possiblegwset) {
        if (!pcr->gwset) {
          pcr->gwset = NETWIB_TRUE;
          pcr->gw = possiblegw;
        }
      }
    }
  }

 netwib_gotolabel:
  netwib_er(netwib_ring_index_close(&pringindex));
  netwib_er(netwib_buf_close(&possibledevice));
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confclean_devip_addsys(netwib_priv_confwork *pcw,
                                                     netwib_priv_confwork_devices *pcd,
                                                     netwib_priv_confwork_ip *pci)
{
  netwib_priv_confwork_arpcache *pca;
  netwib_priv_confwork_routes *pcr;
  netwib_ip net;
  netwib_bool addnet;

  /* add it in the ARP cache */
  if (pcd->hwtype == NETWIB_DEVICE_HWTYPE_ETHER) {
    netwib_er(netwib_priv_confwork_arpcache_init(&pca));
    netwib_er(netwib_buf_append_buf(&pcd->device, &pca->device));
    pca->eth = pcd->eth;
    pca->ip = pci->ip;
    netwib_er(netwib_priv_confwork_arpcache_add(pcw, pca));
  }

  /* add it in the routing table */
  netwib_er(netwib_priv_confwork_routes_init(&pcr));
  netwib_er(netwib_buf_append_buf(&pcd->device, &pcr->device));
  pcr->dst = pci->ip;
  netwib_er(netwib_ip_init_ip4(0xFFFFFFFFu, &pcr->mask));
  pcr->prefix = 128;
  pcr->srcset = NETWIB_FALSE;
  pcr->gwset = NETWIB_FALSE;
  pcr->metric = 0;
  netwib_er(netwib_priv_confwork_routes_add(pcw, pcr));

  /* also add net */
  addnet = NETWIB_TRUE;
  if (pci->ip.iptype == NETWIB_IPTYPE_IP4 &&
      pci->mask.ipvalue.ip4 == 0xFFFFFFFFu) {
    /* otherwise, add twice */
    addnet = NETWIB_FALSE;
  }
  if (pci->ip.iptype == NETWIB_IPTYPE_IP6 &&
      pci->prefix == 128) {
    addnet = NETWIB_FALSE;
  }
  if (addnet) {
    netwib_er(netwib_priv_ip_net_init_ipmaskprefix(&pci->ip, &pci->mask,
                                                   pci->prefix, &net));
    netwib_er(netwib_priv_confwork_routes_init(&pcr));
    netwib_er(netwib_buf_append_buf(&pcd->device, &pcr->device));
    pcr->dst = net;
    pcr->mask = pci->mask;
    pcr->prefix = pci->prefix;
    if (pci->ispointtopoint) {
      pcr->srcset = NETWIB_FALSE;
      pcr->gwset = NETWIB_TRUE;
      pcr->gw = pci->pointtopointip;
    } else {
      pcr->srcset = NETWIB_TRUE;
      pcr->src = pci->ip;
      pcr->gwset = NETWIB_FALSE;
    }
    pcr->metric = 0;
    netwib_er(netwib_priv_confwork_routes_add(pcw, pcr));
  }

  /* say how to reach the peer */
  if (pci->ispointtopoint) {
    netwib_er(netwib_priv_confwork_routes_init(&pcr));
    netwib_er(netwib_buf_append_buf(&pcd->device, &pcr->device));
    pcr->dst = pci->pointtopointip;
    netwib_er(netwib_ip_init_ip4(0xFFFFFFFFu, &pcr->mask));
    pcr->prefix = 128;
    pcr->srcset = NETWIB_TRUE;
    pcr->src = pci->ip;
    pcr->gwset = NETWIB_FALSE;
    pcr->metric = 0;
    netwib_er(netwib_priv_confwork_routes_add(pcw, pcr));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confclean_dev_addsys(netwib_priv_confwork *pcw,
                                                   netwib_priv_confwork_devices *pcd)
{
  netwib_ring_index *pringindex;
  netwib_priv_confwork_ip *pci;
  netwib_cmp cmp;
  netwib_err ret;

  netwib_er(netwib_ring_index_init(pcw->pip, &pringindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pci);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_eg(netwib_buf_cmp(&pcd->device, &pci->device, &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      netwib_eg(netwib_priv_confclean_devip_addsys(pcw, pcd, pci));
    }
  }

 netwib_gotolabel:
  netwib_er(netwib_ring_index_close(&pringindex));
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confclean_arpcache_dup(netwib_constptr piteminf,
                                                     netwib_constptr pitemsup,
                                                     netwib_ptr pinfos,
                                                     netwib_cmp *pcmp)
{
  netwib_priv_confwork_arpcache *pca1 = (netwib_priv_confwork_arpcache *)(netwib_uintptr)piteminf;
  netwib_priv_confwork_arpcache *pca2 = (netwib_priv_confwork_arpcache *)(netwib_uintptr)pitemsup;
  netwib_cmp cmpeth, cmpip;

  netwib_er(netwib_eth_cmp(&pca1->eth, &pca2->eth, &cmpeth));
  netwib_er(netwib_ip_cmp(&pca1->ip, &pca2->ip, &cmpip));

  if (cmpeth == NETWIB_CMP_EQ && cmpip == NETWIB_CMP_EQ) {
    *pcmp = NETWIB_CMP_EQ;
  } else if (cmpeth == NETWIB_CMP_EQ) {
    *pcmp = cmpip;
  } else if (cmpip == NETWIB_CMP_EQ) {
    /* new overwrite old */
    netwib__buf_reinit(&pca1->device);
    netwib_er(netwib_buf_append_buf(&pca2->device, &pca1->device));
    pca1->eth = pca2->eth;
    *pcmp = NETWIB_CMP_EQ;
  } else {
    *pcmp = NETWIB_CMP_LT;
  }

  pinfos = pinfos; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confclean_routes_dup(netwib_constptr piteminf,
                                                   netwib_constptr pitemsup,
                                                   netwib_ptr pinfos,
                                                   netwib_cmp *pcmp)
{
  netwib_priv_confwork_routes *pcr1 = (netwib_priv_confwork_routes *)(netwib_uintptr)piteminf;
  netwib_priv_confwork_routes *pcr2 = (netwib_priv_confwork_routes *)(netwib_uintptr)pitemsup;
  netwib_cmp cmp;

  netwib_er(netwib_buf_cmp(&pcr1->device, &pcr2->device, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    *pcmp = cmp;
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_ip_cmp(&pcr1->dst, &pcr2->dst, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    *pcmp = cmp;
    return(NETWIB_ERR_OK);
  }

  if (pcr1->dst.iptype == NETWIB_IPTYPE_IP4) {
    netwib_er(netwib_ip_cmp(&pcr1->mask, &pcr2->mask, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      *pcmp = cmp;
      return(NETWIB_ERR_OK);
    }
  } else {
    if (pcr1->prefix != pcr2->prefix) {
      *pcmp = (pcr1->prefix<pcr2->prefix)?NETWIB_CMP_LT:NETWIB_CMP_GT;
      return(NETWIB_ERR_OK);
    }
  }

  if (pcr1->srcset != pcr2->srcset) {
    *pcmp = NETWIB_CMP_LT;
    return(NETWIB_ERR_OK);
  }

  if (pcr1->srcset) {
    netwib_er(netwib_ip_cmp(&pcr1->src, &pcr2->src, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      *pcmp = cmp;
      return(NETWIB_ERR_OK);
    }
  }

  if (pcr1->gwset != pcr2->gwset) {
    *pcmp = NETWIB_CMP_LT;
    return(NETWIB_ERR_OK);
  }

  if (pcr1->gwset) {
    netwib_er(netwib_ip_cmp(&pcr1->gw, &pcr2->gw, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      *pcmp = cmp;
      return(NETWIB_ERR_OK);
    }
  }

  /* if we are here, they are equal, so the second will be removed */
  *pcmp = NETWIB_CMP_EQ;
  pinfos = pinfos; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confwork_clean(netwib_priv_confwork *pcw)
{
  netwib_ring_index *pringindex;
  netwib_priv_confwork_devices *pcd;
  netwib_priv_confwork_arpcache *pca;
  netwib_priv_confwork_routes *pcr;
  netwib_err ret;

  /* DEVICES : remove duplicates and eventually update unknown fields */
  netwib_er(netwib_ring_del_duplicate(pcw->pdevices,
                                    &netwib_priv_confclean_devices_dup, NULL,
                                    NETWIB_TRUE));
  netwib_er(netwib_priv_confwork_debug(pcw, "After device_dup"));

  /* IP : remove duplicates and eventually update unknown fields */
  netwib_er(netwib_ring_del_duplicate(pcw->pip,
                                      &netwib_priv_confclean_ip_dup, NULL,
                                      NETWIB_TRUE));
  netwib_er(netwib_priv_confwork_debug(pcw, "After ip_dup"));

  /* ARPCACHE : set a device for empty ones */
  netwib_er(netwib_ring_index_init(pcw->parpcache, &pringindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pca);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    if (pca->device.beginoffset == pca->device.endoffset) {
      ret = netwib_priv_confclean_arpcache_setdev(pcw, pca);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }
  netwib_er(netwib_priv_confwork_debug(pcw, "After arpcache_setdev"));

  /* ROUTES : set a device/src for empty ones */
  netwib_er(netwib_ring_index_init(pcw->proutes, &pringindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pcr);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    ret = netwib_priv_confclean_routes_setdevsrc(pcw, pcr);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }
  netwib_er(netwib_priv_confwork_debug(pcw, "After routes_setdevsrc"));

  /* ARPCACHE/ROUTES : reading devices and ip, we can update arp and
     routes, to ensure they contain everything */
  netwib_er(netwib_ring_index_init(pcw->pdevices, &pringindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pcd);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    ret = netwib_priv_confclean_dev_addsys(pcw, pcd);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }
  netwib_er(netwib_priv_confwork_debug(pcw, "After dev_addsys"));

  /* ARPCACHE : only remove duplicates */
  netwib_er(netwib_ring_del_duplicate(pcw->parpcache,
                                      &netwib_priv_confclean_arpcache_dup,
                                      NULL,
                                      NETWIB_TRUE));
  netwib_er(netwib_priv_confwork_debug(pcw, "After arpcache_dup"));

  /* ROUTES : only remove duplicates */
  netwib_er(netwib_ring_del_duplicate(pcw->proutes,
                                      &netwib_priv_confclean_routes_dup, NULL,
                                      NETWIB_TRUE));
  netwib_er(netwib_priv_confwork_debug(pcw, "After routes_dup"));

  return(NETWIB_ERR_OK);
}
