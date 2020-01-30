/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_updtdev2(netwib_priv_confwork *pcw,
                                            netwib_uint32 devnum,
                                            netwib_buf *pdevice)
{
  netwib_ring_index *pringindex;
  netwib_priv_confwork_devices *pcd;
  netwib_err ret;

  netwib_er(netwib_ring_index_init(pcw->pdevices, &pringindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pcd);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    if (pcd->devnum == devnum) {
      ret = netwib_buf_append_buf(&pcd->device, pdevice);
      break;
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_updtdev(netwib_priv_confwork *pcw)
{
  netwib_ring_index *pringindex;
  netwib_priv_confwork_ip *pci;
  netwib_priv_confwork_arpcache *pca;
  netwib_priv_confwork_routes *pcr;
  netwib_err ret;

  /* update ip ring */
  netwib_er(netwib_ring_index_init(pcw->pip, &pringindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pci);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    ret = netwib_priv_conf_updtdev2(pcw, pci->devnum, &pci->device);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  /* update arp cache ring */
  netwib_er(netwib_ring_index_init(pcw->parpcache, &pringindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pca);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    ret = netwib_priv_conf_updtdev2(pcw, pca->devnum, &pca->device);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  /* update route ring */
  netwib_er(netwib_ring_index_init(pcw->proutes, &pringindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pcr);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    ret = netwib_priv_conf_updtdev2(pcw, pcr->devnum, &pcr->device);
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
