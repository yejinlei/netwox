/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confclean_devnum_cmp(netwib_uint32 devnum1,
                                                   netwib_uint32 devnum2,
                                                   netwib_cmp *pcmp)
{
  if (devnum1 == devnum2) {
    *pcmp = NETWIB_CMP_EQ;
  } else if (devnum1 < devnum2) {
    *pcmp = NETWIB_CMP_LT;
  } else {
    *pcmp = NETWIB_CMP_GT;
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confclean_ip_group(netwib_constptr piteminf,
                                                 netwib_constptr pitemsup,
                                                 netwib_ptr pinfos,
                                                 netwib_cmp *pcmp)
{
  const netwib_priv_confwork_ip *pci1 = (const netwib_priv_confwork_ip *)piteminf;
  const netwib_priv_confwork_ip *pci2 = (const netwib_priv_confwork_ip *)pitemsup;
  netwib_er(netwib_priv_confclean_devnum_cmp(pci1->devnum, pci2->devnum, pcmp));

  pinfos = pinfos; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confclean_arpcache_sort(netwib_constptr piteminf,
                                                      netwib_constptr pitemsup,
                                                      netwib_ptr pinfos,
                                                      netwib_cmp *pcmp)
{
  const netwib_priv_confwork_arpcache *pca1 =
    (const netwib_priv_confwork_arpcache *)piteminf;
  const netwib_priv_confwork_arpcache *pca2 =
    (const netwib_priv_confwork_arpcache *)pitemsup;
  netwib_cmp cmp;

  netwib_er(netwib_priv_confclean_devnum_cmp(pca1->devnum, pca2->devnum, &cmp));
  if (cmp == NETWIB_CMP_EQ) {
    netwib_er(netwib_eth_cmp(&pca1->eth, &pca2->eth, &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      netwib_er(netwib_ip_cmp(&pca1->ip, &pca2->ip, &cmp));
    }
  }

  *pcmp = cmp;
  pinfos = pinfos; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confclean_routes_sort(netwib_constptr piteminf,
                                                    netwib_constptr pitemsup,
                                                    netwib_ptr pinfos,
                                                    netwib_cmp *pcmp)
{
  const netwib_priv_confwork_routes *pcr1 =
    (const netwib_priv_confwork_routes *)piteminf;
  const netwib_priv_confwork_routes *pcr2 =
    (const netwib_priv_confwork_routes *)pitemsup;
  netwib_ip4 net1, net2;

  switch(pcr1->dst.iptype) {
    case NETWIB_IPTYPE_IP4:
      switch(pcr2->dst.iptype) {
        case NETWIB_IPTYPE_IP4:
          net1 = ~pcr1->mask.ipvalue.ip4;
          net2 = ~pcr2->mask.ipvalue.ip4;
          if (net1 == net2) {
            *pcmp = NETWIB_CMP_EQ;
          } else if (net1 < net2) {
            *pcmp = NETWIB_CMP_LT;
          } else {
            *pcmp = NETWIB_CMP_GT;
          }
          break;
        case NETWIB_IPTYPE_IP6:
          *pcmp = NETWIB_CMP_LT;
          break;
        default:
          return(NETWIB_ERR_PAIPTYPE);
      }
      break;
    case NETWIB_IPTYPE_IP6:
      switch(pcr2->dst.iptype) {
        case NETWIB_IPTYPE_IP4:
          *pcmp = NETWIB_CMP_GT;
          break;
        case NETWIB_IPTYPE_IP6:
          if (pcr1->prefix == pcr2->prefix) {
            *pcmp = NETWIB_CMP_EQ;
          } else if (pcr1->prefix < pcr2->prefix) {
            *pcmp = NETWIB_CMP_GT;
          } else {
            *pcmp = NETWIB_CMP_LT;
          }
          break;
        default:
          return(NETWIB_ERR_PAINVALIDTYPE);
      }
      break;
    default:
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  if (*pcmp == NETWIB_CMP_EQ) {
    if (pcr1->metric == pcr2->metric) {
      *pcmp = NETWIB_CMP_EQ;
    } else if (pcr1->metric < pcr2->metric) {
      *pcmp = NETWIB_CMP_LT;
    } else {
      *pcmp = NETWIB_CMP_GT;
    }
  }

  if (*pcmp == NETWIB_CMP_EQ) {
    netwib_er(netwib_priv_confclean_devnum_cmp(pcr1->devnum, pcr2->devnum,
                                               pcmp));
  }

  pinfos = pinfos; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confwork_reorder(netwib_priv_confwork *pcw)
{

  /* group ip (for example, Linux need it because IPv6 configuration
     if retrieved after IPv4) */
  netwib_er(netwib_ring_group(pcw->pip,
                              &netwib_priv_confclean_ip_group, NULL));

  /* sort arp cache (currently, only because it's easier to read) */
  netwib_er(netwib_ring_sort(pcw->parpcache,
                             &netwib_priv_confclean_arpcache_sort, NULL));

  /* sort routes (for example, FreeBSD need it) */
  netwib_er(netwib_ring_sort(pcw->proutes, &netwib_priv_confclean_routes_sort,
                             NULL));

  return(NETWIB_ERR_OK);
}
