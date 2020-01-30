/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_arpcache_init(netwib_priv_confwork_arpcache **ppitem)
{
  netwib_priv_confwork_arpcache *pitem;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_confwork_arpcache),
                              (netwib_ptr*)ppitem));
  pitem = *ppitem;
  pitem->devnum = 0;
  netwib_er(netwib_buf_init_mallocdefault(&pitem->device));

  /* otherwise if forgot, they'll crash debug function */
  netwib_er(netwib_ip_init_ip4_fields(0xFF,0xFF,0xFF,0xFF, &pitem->ip));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_arpcache_close(netwib_priv_confwork_arpcache **ppitem)
{
  netwib_priv_confwork_arpcache *pitem;

  pitem = *ppitem;
  netwib_er(netwib_buf_close(&pitem->device));
  netwib_er(netwib_ptr_free((netwib_ptr*)ppitem));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_arpcache_add(netwib_priv_confwork *pcw,
                                             netwib_priv_confwork_arpcache *pitem)
{
  netwib_er(netwib_priv_confwork_dev_noalias(&pitem->device));
  netwib_er(netwib_ring_add_last(pcw->parpcache, pitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_arpcache_erase(netwib_ptr pitem)
{
  netwib_er(netwib_priv_confwork_arpcache_close((netwib_priv_confwork_arpcache **)&pitem));
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
netwib_err netwib_conf_arpcache_init(netwib_conf_arpcache **ppitem)
{

  netwib_er(netwib_ptr_malloc(sizeof(netwib_conf_arpcache),
                              (netwib_ptr*)ppitem));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_conf_arpcache_close(netwib_conf_arpcache **ppitem)
{

  netwib_er(netwib_ptr_free((netwib_ptr*)ppitem));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_conf_arpcache_addi(netwib_priv_conf_t *pc,
                                     netwib_conf_arpcache *pitem)
{
  netwib_er(netwib_ring_add_last(pc->parpcache, pitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_conf_arpcache_erase(netwib_ptr pitem)
{
  netwib_er(netwib_conf_arpcache_close((netwib_conf_arpcache **)&pitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_arpcacheglo_add_dev(netwib_constbuf *pdevice,
                                                       netwib_uint32 *pdevnum)
{
  netwib_ring_index *pringindex;
  netwib_conf_devices *pcd;
  netwib_cmp cmp;
  netwib_err ret;

  netwib_er(netwib_ring_index_init(netwib_priv_conf.pdevices, &pringindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pcd);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_NOTFOUND;
      break;
    }
    ret = netwib_buf_cmp(pdevice, &pcd->device, &cmp);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    if (cmp == NETWIB_CMP_EQ) {
      *pdevnum = pcd->devnum;
      break;
    }
    ret = netwib_buf_cmp(pdevice, &pcd->deviceeasy, &cmp);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    if (cmp == NETWIB_CMP_EQ) {
      *pdevnum = pcd->devnum;
      break;
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_arpcacheglo_add_ip(netwib_constip *pip,
                                                      netwib_uint32 *pdevnum)
{
  netwib_ring_index *pringindex;
  netwib_conf_ip *pci;
  netwib_cmp cmp;
  netwib_err ret;

  netwib_er(netwib_ring_index_init(netwib_priv_conf.pip, &pringindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pci);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_NOTFOUND;
      break;
    }
    ret = netwib_priv_ip_net_cmp_ipmaskprefix(&pci->ip, pip, &pci->mask,
                                              pci->prefix, &cmp);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    if (cmp == NETWIB_CMP_EQ) {
      *pdevnum = pci->devnum;
      break;
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confglo_arpcache_add(netwib_constbuf *pdevice,
                                            netwib_consteth *peth,
                                            netwib_constip *pip)
{
  netwib_conf_arpcache *pitem;
  netwib_uint32 devnum=0;
  netwib_err ret;

  netwib_er(netwib_priv_conf_rdlock());
  ret = NETWIB_ERR_NOTFOUND;
  if (pdevice != NULL) {
    ret = netwib_priv_conf_arpcacheglo_add_dev(pdevice, &devnum);
  }
  if (ret == NETWIB_ERR_NOTFOUND) {
    ret = netwib_priv_conf_arpcacheglo_add_ip(pip, &devnum);
  }
  if (ret == NETWIB_ERR_NOTFOUND) {
    devnum = 0;
    ret = NETWIB_ERR_OK;
  }
  netwib_er(netwib_priv_conf_rdunlock());
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  netwib_er(netwib_conf_arpcache_init(&pitem));
  pitem->devnum = devnum;
  pitem->eth = *peth;
  pitem->ip = *pip;
  netwib_er(netwib_priv_conf_wrlock());
  netwib_er(netwib_ring_add_last(netwib_priv_conf.parpcache, pitem));
  netwib_er(netwib_priv_conf_wrunlock());

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confglo_arpcache_ip2(netwib_consteth *peth,
                                                   netwib_ip *pip)
{
  netwib_ring_index *pringindex;
  netwib_conf_arpcache *pentry;
  netwib_cmp cmp;
  netwib_err ret;

  netwib_er(netwib_ring_index_init(netwib_priv_conf.parpcache, &pringindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pentry);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_NOTCONVERTED;
      break;
    }
    netwib_er(netwib_eth_cmp(peth, &pentry->eth, &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      *pip = pentry->ip;
      break;
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  return(ret);
}
netwib_err netwib_priv_confglo_arpcache_ip(netwib_consteth *peth,
                                           netwib_ip *pip)
{
  netwib_err ret;

  netwib__priv_conf_eventuallyupdate();

  netwib_er(netwib_priv_conf_rdlock());
  ret = netwib_priv_confglo_arpcache_ip2(peth, pip);
  netwib_er(netwib_priv_conf_rdunlock());
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confglo_arpcache_eth2(netwib_constip *pip,
                                                    netwib_eth *peth)
{
  netwib_ring_index *pringindex;
  netwib_conf_arpcache *pentry;
  netwib_cmp cmp;
  netwib_err ret;

  netwib_er(netwib_ring_index_init(netwib_priv_conf.parpcache, &pringindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pentry);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_NOTCONVERTED;
      break;
    }
    ret = netwib_ip_cmp(pip, &pentry->ip, &cmp);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    if (cmp == NETWIB_CMP_EQ) {
      *peth = pentry->eth;
      break;
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  return(ret);
}
netwib_err netwib_priv_confglo_arpcache_eth(netwib_constip *pip,
                                            netwib_eth *peth)
{
  netwib_err ret;

  netwib__priv_conf_eventuallyupdate();

  netwib_er(netwib_priv_conf_rdlock());
  ret = netwib_priv_confglo_arpcache_eth2(pip, peth);
  netwib_er(netwib_priv_conf_rdunlock());
  return(ret);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confrel_arpcache_ip(netwib_consteth *peth,
                                           netwib_ip *pip)
{
  netwib_priv_confwork cw;
  netwib_ring_index *pringindex;
  netwib_priv_confwork_arpcache *pentry;
  netwib_cmp cmp;
  netwib_bool rindindexset;
  netwib_err ret;

  ret = NETWIB_ERR_OK;
  rindindexset = NETWIB_FALSE;
  netwib_er(netwib_priv_confwork_init(&cw));
  netwib_eg(netwib_priv_confwork_obtain_arpcache(&cw));

  netwib_eg(netwib_ring_index_init(cw.parpcache, &pringindex));
  rindindexset = NETWIB_TRUE;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pentry);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_NOTCONVERTED;
      break;
    }
    netwib_eg(netwib_eth_cmp(peth, &pentry->eth, &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      *pip = pentry->ip;
      break;
    }
  }

 netwib_gotolabel:
  if (rindindexset) {
    netwib_er(netwib_ring_index_close(&pringindex));
  }
  netwib_er(netwib_priv_confwork_close(&cw));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confrel_arpcache_eth(netwib_constip *pip,
                                            netwib_eth *peth)
{
  netwib_priv_confwork cw;
  netwib_ring_index *pringindex;
  netwib_priv_confwork_arpcache *pentry;
  netwib_cmp cmp;
  netwib_bool rindindexset;
  netwib_err ret;

  ret = NETWIB_ERR_OK;
  rindindexset = NETWIB_FALSE;
  netwib_er(netwib_priv_confwork_init(&cw));
  netwib_eg(netwib_priv_confwork_obtain_arpcache(&cw));

  netwib_eg(netwib_ring_index_init(cw.parpcache, &pringindex));
  rindindexset = NETWIB_TRUE;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pentry);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_NOTCONVERTED;
      break;
    }
    netwib_eg(netwib_ip_cmp(pip, &pentry->ip, &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      *peth = pentry->eth;
      break;
    }
  }

 netwib_gotolabel:
  if (rindindexset) {
    netwib_er(netwib_ring_index_close(&pringindex));
  }
  netwib_er(netwib_priv_confwork_close(&cw));
  return(ret);
}
