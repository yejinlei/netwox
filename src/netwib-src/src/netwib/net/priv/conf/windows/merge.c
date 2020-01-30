/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_merge(netwib_priv_confwork *pcw,
                                         netwib_priv_confwork *pcwlocal)
{
  netwib_ring_index *pringindex;
  netwib_priv_confwork_devices *pcd, *pcdlocal;
  netwib_priv_confwork_ip *pci, *pcilocal;
  netwib_priv_confwork_arpcache *pca, *pcalocal;
  netwib_priv_confwork_routes *pcr, *pcrlocal;
  netwib_err ret;

  /* devices ring */
  netwib_er(netwib_ring_index_init(pcwlocal->pdevices, &pringindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pcdlocal);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_er(netwib_priv_confwork_devices_init(&pcd));
    netwib_er(netwib_buf_append_buf(&pcdlocal->device, &pcd->device));
    pcd->mtu = pcdlocal->mtu;
    pcd->hwtype = pcdlocal->hwtype;
    pcd->eth = pcdlocal->eth;
    netwib_er(netwib_priv_confwork_devices_add(pcw, pcd));
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  /* ip ring */
  netwib_er(netwib_ring_index_init(pcwlocal->pip, &pringindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pcilocal);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_er(netwib_priv_confwork_ip_init(&pci));
    netwib_er(netwib_buf_append_buf(&pcilocal->device, &pci->device));
    pci->ip = pcilocal->ip;
    pci->mask = pcilocal->mask;
    pci->prefix = pcilocal->prefix;
    pci->ispointtopoint = pcilocal->ispointtopoint;
    pci->pointtopointip = pcilocal->pointtopointip;
    netwib_er(netwib_priv_confwork_ip_add(pcw, pci));
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  /* arp ring */
  netwib_er(netwib_ring_index_init(pcwlocal->parpcache, &pringindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pcalocal);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_er(netwib_priv_confwork_arpcache_init(&pca));
    netwib_er(netwib_buf_append_buf(&pcalocal->device, &pca->device));
    pca->eth = pcalocal->eth;
    pca->ip = pcalocal->ip;
    netwib_er(netwib_priv_confwork_arpcache_add(pcw, pca));
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  /* routes ring */
  netwib_er(netwib_ring_index_init(pcwlocal->proutes, &pringindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pcrlocal);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_er(netwib_priv_confwork_routes_init(&pcr));
    netwib_er(netwib_buf_append_buf(&pcrlocal->device, &pcr->device));
    pcr->dst = pcrlocal->dst;
    pcr->mask = pcrlocal->mask;
    pcr->prefix = pcrlocal->prefix;
    pcr->srcset = pcrlocal->srcset;
    pcr->src = pcrlocal->src;
    pcr->gwset = pcrlocal->gwset;
    pcr->gw = pcrlocal->gw;
    pcr->metric = pcrlocal->metric;
    netwib_er(netwib_priv_confwork_routes_add(pcw, pcr));
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  return(NETWIB_ERR_OK);
}
