/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_priv_conf_t netwib_priv_conf;
netwib_priv_mutex netwib_priv_conf_mut;
netwib_bool netwib_priv_conf_needtobeupdated = NETWIB_TRUE;

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conft_init(netwib_priv_conf_t *pct)
{
  netwib_er(netwib_ring_init(&netwib_conf_devices_erase, NULL, &pct->pdevices));
  netwib_er(netwib_ring_init(&netwib_conf_ip_erase, NULL, &pct->pip));
  netwib_er(netwib_ring_init(&netwib_conf_arpcache_erase, NULL, &pct->parpcache));
  netwib_er(netwib_ring_init(&netwib_conf_routes_erase, NULL, &pct->proutes));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conft_close(netwib_priv_conf_t *pct)
{
  netwib_er(netwib_ring_close(&pct->pdevices, NETWIB_TRUE));
  netwib_er(netwib_ring_close(&pct->pip, NETWIB_TRUE));
  netwib_er(netwib_ring_close(&pct->parpcache, NETWIB_TRUE));
  netwib_er(netwib_ring_close(&pct->proutes, NETWIB_TRUE));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conft_add_confwork(netwib_priv_confwork *pcw,
                                                 netwib_priv_conf_t *pc)
{
  netwib_priv_confwork_devices *pcwd;
  netwib_priv_confwork_ip *pcwi;
  netwib_priv_confwork_arpcache *pcwa;
  netwib_priv_confwork_routes *pcwr;
  netwib_conf_devices *pcd;
  netwib_conf_ip *pci;
  netwib_conf_arpcache *pca;
  netwib_conf_routes *pcr;
  netwib_ring_index *pringindex;
  netwib_err ret;

  netwib_er(netwib_ring_index_init(pcw->pdevices, &pringindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pcwd);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    ret = netwib_conf_devices_init(&pcd);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    pcd->devnum = pcwd->devnum;
    netwib_er(netwib_buf_append_buf(&pcwd->device, &pcd->device));
    netwib_er(netwib_buf_append_buf(&pcwd->deviceeasy, &pcd->deviceeasy));
    pcd->mtu = pcwd->mtu;
    pcd->hwtype = pcwd->hwtype;
    pcd->eth = pcwd->eth;
    ret = netwib_conf_devices_add(pc, pcd);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  netwib_er(netwib_ring_index_init(pcw->pip, &pringindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pcwi);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    ret = netwib_conf_ip_init(&pci);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    pci->devnum = pcwi->devnum;
    pci->ip = pcwi->ip;
    pci->mask = pcwi->mask;
    pci->prefix = pcwi->prefix;
    pci->ispointtopoint = pcwi->ispointtopoint;
    pci->pointtopointip = pcwi->pointtopointip;
    ret = netwib_conf_ip_add(pc, pci);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  netwib_er(netwib_ring_index_init(pcw->parpcache, &pringindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pcwa);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    ret = netwib_conf_arpcache_init(&pca);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    pca->devnum = pcwa->devnum;
    pca->eth = pcwa->eth;
    pca->ip = pcwa->ip;
    ret = netwib_conf_arpcache_addi(pc, pca);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  netwib_er(netwib_ring_index_init(pcw->proutes, &pringindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pcwr);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    ret = netwib_conf_routes_init(&pcr);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    pcr->devnum = pcwr->devnum;
    pcr->dst = pcwr->dst;
    pcr->mask = pcwr->mask;
    pcr->prefix = pcwr->prefix;
    pcr->srcset = pcwr->srcset;
    pcr->src = pcwr->src;
    pcr->gwset = pcwr->gwset;
    pcr->gw = pcwr->gw;
    pcr->metric = pcwr->metric;
    ret = netwib_conf_routes_add(pc, pcr);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conft_swap(netwib_priv_conf_t *pcsrc,
                                         netwib_priv_conf_t *pcdst)
{
  netwib_ring *p;

  p = pcdst->pdevices;
  pcdst->pdevices = pcsrc->pdevices;
  pcsrc->pdevices = p;
  p = pcdst->pip;
  pcdst->pip = pcsrc->pip;
  pcsrc->pip = p;
  p = pcdst->parpcache;
  pcdst->parpcache = pcsrc->parpcache;
  pcsrc->parpcache = p;
  p = pcdst->proutes;
  pcdst->proutes = pcsrc->proutes;
  pcsrc->proutes = p;

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_conf_rdlock(void)
{
  return(netwib_priv_mutex_rdlock(&netwib_priv_conf_mut));
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_conf_rdunlock(void)
{
  return(netwib_priv_mutex_rdunlock(&netwib_priv_conf_mut));
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_conf_wrlock(void)
{
  return(netwib_priv_mutex_wrlock(&netwib_priv_conf_mut));
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_conf_wrunlock(void)
{
  return(netwib_priv_mutex_wrunlock(&netwib_priv_conf_mut));
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_conf_init(void)
{
  netwib_priv_conf_needtobeupdated = NETWIB_TRUE;
  netwib_er(netwib_priv_conft_init(&netwib_priv_conf));
  netwib_er(netwib_priv_mutex_init(&netwib_priv_conf_mut));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_conf_close(void)
{
  netwib_priv_conf_needtobeupdated = NETWIB_TRUE;
  netwib_er(netwib_priv_conft_close(&netwib_priv_conf));
  netwib_er(netwib_priv_mutex_close(&netwib_priv_conf_mut));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_conf_update(void)
{
  netwib_priv_conf_t ct;
  netwib_priv_confwork cw;
  netwib_bool ctset;
  netwib_err ret;

  netwib_er(netwib_priv_confwork_init(&cw));

  ctset = NETWIB_FALSE;
  netwib_eg(netwib_priv_confwork_obtain(&cw, NETWIB_FALSE, NULL));
  netwib_eg(netwib_priv_conft_init(&ct));
  ctset = NETWIB_TRUE;
  netwib_eg(netwib_priv_conft_add_confwork(&cw, &ct));

  netwib_er(netwib_priv_conf_wrlock());
  ret = netwib_priv_conft_swap(&ct, &netwib_priv_conf);
  if (ret == NETWIB_ERR_OK) {
    netwib_priv_conf_needtobeupdated = NETWIB_FALSE;
  }
  netwib_er(netwib_priv_conf_wrunlock());

 netwib_gotolabel:
  if (ctset) {
    netwib_er(netwib_priv_conft_close(&ct));
  }
  netwib_er(netwib_priv_confwork_close(&cw));
  return(ret);
}
