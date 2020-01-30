/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confwork_easy_dev(netwib_priv_confwork *pcw)
{
  netwib_ring_index *pringindex;
  netwib_priv_confwork_devices *pcd;
  netwib_uint32 nextnum[NETWIB_PRIV_CONFEASY_HWMAX];
  netwib_uint32 devnum, i;
  netwib_err ret;

  for (i = 0; i < NETWIB_PRIV_CONFEASY_HWMAX; i++) {
    nextnum[i] = 0;
  }

  netwib_er(netwib_ring_index_init(pcw->pdevices, &pringindex));
  ret = NETWIB_ERR_OK;
  devnum = 1;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pcd);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    if (pcd->hwtype >= NETWIB_PRIV_CONFEASY_HWMAX) {
      /* array nextnum is too small */
      return(NETWIB_ERR_LOINTERNALERROR);
    }
    pcd->devnum = devnum++;
    i = nextnum[pcd->hwtype]++;
    switch (pcd->hwtype) {
    case NETWIB_DEVICE_HWTYPE_UNKNOWN :
      ret = netwib_buf_append_fmt(&pcd->deviceeasy, "Unk%{uint32}", i);
      break;
    case NETWIB_DEVICE_HWTYPE_ETHER :
      ret = netwib_buf_append_fmt(&pcd->deviceeasy, "Eth%{uint32}", i);
      break;
    case NETWIB_DEVICE_HWTYPE_LOOPBACK :
      ret = netwib_buf_append_fmt(&pcd->deviceeasy, "Lo%{uint32}", i);
      break;
    case NETWIB_DEVICE_HWTYPE_PPP :
      ret = netwib_buf_append_fmt(&pcd->deviceeasy, "Ppp%{uint32}", i);
      break;
    case NETWIB_DEVICE_HWTYPE_SLIP :
      ret = netwib_buf_append_fmt(&pcd->deviceeasy, "Sli%{uint32}", i);
      break;
    case NETWIB_DEVICE_HWTYPE_PLIP :
      ret = netwib_buf_append_fmt(&pcd->deviceeasy, "Pli%{uint32}", i);
      break;
    }
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confwork_easy_devnum(netwib_priv_confwork *pcw,
                                                   netwib_constbuf *pdevice,
                                                   netwib_uint32 *pdevnum)
{
  netwib_ring_index *pringindex;
  netwib_priv_confwork_devices *pcd;
  netwib_uint32 nextunknown, nextdevnum;
  netwib_cmp cmp;
  netwib_err ret;

  netwib_er(netwib_ring_index_init(pcw->pdevices, &pringindex));
  nextunknown = 0;
  nextdevnum = 1;
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pcd);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    nextdevnum++;
    if (pcd->hwtype == NETWIB_DEVICE_HWTYPE_UNKNOWN) {
      nextunknown++;
    }
    ret = netwib_buf_cmp(pdevice, &pcd->device, &cmp);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    if (cmp == NETWIB_CMP_EQ) {
      *pdevnum = pcd->devnum;
      break;
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));

  if (ret == NETWIB_ERR_DATAEND) {
    /* we need to add it */
    netwib_er(netwib_priv_confwork_devices_init(&pcd));
    pcd->devnum = nextdevnum;
    netwib_er(netwib_buf_append_buf(pdevice, &pcd->device));
    netwib_er(netwib_buf_append_fmt(&pcd->deviceeasy, "Unk%{uint32}",
                                  nextunknown));
    pcd->mtu = 0;
    pcd->hwtype = NETWIB_DEVICE_HWTYPE_UNKNOWN;
    netwib_er(netwib_priv_confwork_devices_add(pcw, pcd));
    *pdevnum = pcd->devnum;
    ret = NETWIB_ERR_OK;
  }

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confwork_easy(netwib_priv_confwork *pcw)
{
  netwib_ring_index *pringindex;
  netwib_priv_confwork_ip *pci;
  netwib_priv_confwork_arpcache *pca;
  netwib_priv_confwork_routes *pcr;
  netwib_err ret;

  netwib_er(netwib_priv_confwork_easy_dev(pcw));
  netwib_er(netwib_priv_confwork_debug(pcw, "After easy_dev"));

  netwib_er(netwib_ring_index_init(pcw->pip, &pringindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pci);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    ret = netwib_priv_confwork_easy_devnum(pcw, &pci->device, &pci->devnum);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }
  netwib_er(netwib_priv_confwork_debug(pcw, "After easy_devnum for ip"));

  netwib_er(netwib_ring_index_init(pcw->parpcache, &pringindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pca);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    ret = netwib_priv_confwork_easy_devnum(pcw, &pca->device, &pca->devnum);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }
  netwib_er(netwib_priv_confwork_debug(pcw, "After easy_devnum for arpcache"));

  netwib_er(netwib_ring_index_init(pcw->proutes, &pringindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pcr);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    ret = netwib_priv_confwork_easy_devnum(pcw, &pcr->device, &pcr->devnum);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }
  netwib_er(netwib_priv_confwork_debug(pcw, "After easy_devnum for routes"));

  return(NETWIB_ERR_OK);
}
