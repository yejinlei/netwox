/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 devnum;
  netwib_buf device;
  netwib_uint32 mark;
} netwib_priv_conf_deduct_markitem;

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_deduct_markitem_init(netwib_priv_conf_deduct_markitem **ppitem)
{
  netwib_priv_conf_deduct_markitem *pitem;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_conf_deduct_markitem),
                              (netwib_ptr*)ppitem));
  pitem = *ppitem;
  pitem->devnum = 0;
  netwib_er(netwib_buf_init_mallocdefault(&pitem->device));
  pitem->mark = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_deduct_markitem_close(netwib_priv_conf_deduct_markitem **ppitem)
{
  netwib_priv_conf_deduct_markitem *pitem;

  pitem = *ppitem;
  netwib_er(netwib_buf_close(&pitem->device));
  netwib_er(netwib_ptr_free((netwib_ptr*)ppitem));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_deduct_markitem_erase(netwib_ptr pitem)
{
  netwib_er(netwib_priv_conf_deduct_markitem_close((netwib_priv_conf_deduct_markitem **)&pitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_deduct_mark_add(netwib_ring *pring,
                                                 netwib_uint32 devnum,
                                                 netwib_buf *pdevice,
                                                 netwib_uint32 mark)
{
  netwib_ring_index *pringindex;
  netwib_priv_conf_deduct_markitem *pmi=NULL;
  netwib_cmp cmp;
  netwib_err ret;

  netwib_er(netwib_ring_index_init(pring, &pringindex));
  ret = NETWIB_ERR_NOTFOUND;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pmi);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_NOTFOUND;
      break;
    }
    if (pmi->devnum == devnum) {
      netwib_er(netwib_buf_cmp(&pmi->device, pdevice, &cmp));
      if (cmp == NETWIB_CMP_EQ) {
        ret = NETWIB_ERR_OK;
        break;
      }
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));

  if (ret == NETWIB_ERR_NOTFOUND) {
    netwib_er(netwib_priv_conf_deduct_markitem_init(&pmi));
    pmi->devnum = devnum;
    netwib_er(netwib_buf_append_buf(pdevice, &pmi->device));
    pmi->mark = mark;
    netwib_er(netwib_ring_add_last(pring, pmi));
    ret = NETWIB_ERR_OK;
  } else if (ret == NETWIB_ERR_OK) {
    pmi->mark += mark;
  }

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_deduct_mark_debug(netwib_priv_confwork *pcw,
                                                   netwib_ring *pring,
                                                   netwib_conststring title)
{
  netwib_ring_index *pringindex;
  netwib_priv_conf_deduct_markitem *pmi;
  netwib_err ret;

  if (!pcw->debug) {
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_buf_append_fmt(pcw->pdebugbuf, "\n%s\n", title));

  netwib_er(netwib_ring_index_init(pring, &pringindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pmi);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_er(netwib_buf_append_fmt(pcw->pdebugbuf,
                                 "Mark %{uint32:04X} : %{uint32} <-> %{buf}\n",
                                  pmi->mark, pmi->devnum, &pmi->device));
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_deduct_mark_sort(netwib_constptr piteminf,
                                                  netwib_constptr pitemsup,
                                                  netwib_ptr pinfos,
                                                  netwib_cmp *pcmp)
{
  const netwib_priv_conf_deduct_markitem *pmiinf = piteminf;
  const netwib_priv_conf_deduct_markitem *pmisup = pitemsup;

  if (pmiinf->mark == pmisup->mark) {
    *pcmp = NETWIB_CMP_EQ;
  } else if (pmiinf->mark < pmisup->mark) {
    *pcmp = NETWIB_CMP_GT;
  } else {
    *pcmp = NETWIB_CMP_LT;
  }

  pinfos = pinfos; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_deduct_mark_dup(netwib_constptr piteminf,
                                                 netwib_constptr pitemsup,
                                                 netwib_ptr pinfos,
                                                 netwib_cmp *pcmp)
{
  const netwib_priv_conf_deduct_markitem *pmiinf = piteminf;
  const netwib_priv_conf_deduct_markitem *pmisup = pitemsup;

  if (pmiinf->devnum == pmisup->devnum) {
    *pcmp = NETWIB_CMP_EQ;
  } else {
    netwib_er(netwib_buf_cmp(&pmiinf->device, &pmisup->device, pcmp));
  }

  pinfos = pinfos; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_deduct_mark_setdev(netwib_ring *pring,
                                                    netwib_priv_confwork_devices *pcd)
{
  netwib_ring_index *pringindex;
  netwib_priv_conf_deduct_markitem *pmi=NULL;
  netwib_err ret;

  netwib_er(netwib_ring_index_init(pring, &pringindex));
  ret = NETWIB_ERR_NOTFOUND;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pmi);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_NOTFOUND;
      break;
    }
    if (pmi->devnum == pcd->devnum) {
      ret = NETWIB_ERR_OK;
      break;
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));

  if (ret == NETWIB_ERR_NOTFOUND) {
    if (netwib__buf_ref_data_size(&pcd->device) == 0) {
      netwib_er(netwib_buf_append_fmt(&pcd->device, "unknown_%{uint32}",
                                      pcd->devnum));
    }
    ret = NETWIB_ERR_OK;
  } else if (ret == NETWIB_ERR_OK) {
    netwib__buf_reinit(&pcd->device);
    netwib_er(netwib_buf_append_buf(&pmi->device, &pcd->device));
  }

  return(ret);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
#define NETWIB_CONF_DEDUCT_DEV_TYPE_ETH  0x8000
#define NETWIB_CONF_DEDUCT_DEV_TYPE      0x4000
#define NETWIB_CONF_DEDUCT_DEV_UNKNOWN   0x2000
#define NETWIB_CONF_DEDUCT_DEV_NOTHING   0x0000

#define NETWIB_CONF_DEDUCT_IP_IP         0x0800
#define NETWIB_CONF_DEDUCT_IP_NOTHING    0x0000

#define NETWIB_CONF_DEDUCT_POS           0x0100

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_deduct_dev(netwib_priv_confwork_devices *pcd,
                                              netwib_priv_confwork_devices *pcdlocal,
                                              netwib_uint32 numtype,
                                              netwib_uint32 numtypelocal,
                                              netwib_uint32 *pmark)
{
  netwib_cmp cmp;

  *pmark = NETWIB_CONF_DEDUCT_DEV_NOTHING;

  if (pcd->hwtype == NETWIB_DEVICE_HWTYPE_ETHER &&
      pcdlocal->hwtype == NETWIB_DEVICE_HWTYPE_ETHER) {
    netwib_er(netwib_eth_cmp(&pcd->eth, &pcdlocal->eth, &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      *pmark += NETWIB_CONF_DEDUCT_DEV_TYPE_ETH;
    } else {
      *pmark += NETWIB_CONF_DEDUCT_DEV_TYPE;
    }
  } else if (pcd->hwtype == pcdlocal->hwtype) {
    *pmark += NETWIB_CONF_DEDUCT_DEV_TYPE;
  } else if (pcd->hwtype == NETWIB_DEVICE_HWTYPE_UNKNOWN ||
             pcdlocal->hwtype == NETWIB_DEVICE_HWTYPE_UNKNOWN) {
    *pmark += NETWIB_CONF_DEDUCT_DEV_UNKNOWN;
  }

  *pmark += NETWIB_CONF_DEDUCT_POS;
  *pmark -= (numtype + numtypelocal);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_deduct_ip(netwib_priv_confwork_ip *pci,
                                             netwib_priv_confwork_ip *pcilocal,
                                             netwib_uint32 *pmark)
{
  netwib_cmp cmp;

  *pmark = NETWIB_CONF_DEDUCT_IP_NOTHING;

  netwib_er(netwib_ip_cmp(&pci->ip, &pcilocal->ip, &cmp));
  if (cmp == NETWIB_CMP_EQ) {
    *pmark += NETWIB_CONF_DEDUCT_IP_IP;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_deduct(netwib_priv_confwork *pcw,
                                          netwib_priv_confwork *pcwlocal)
{
  netwib_ring_index *pringindex, *pringindexlocal;
  netwib_ring *pmarkring;
  netwib_priv_confwork_devices *pcd, *pcdlocal;
  netwib_priv_confwork_ip *pci, *pcilocal;
  netwib_uint32 numtype[NETWIB_PRIV_CONFEASY_HWMAX];
  netwib_uint32 numtypelocal[NETWIB_PRIV_CONFEASY_HWMAX];
  netwib_uint32 mark, i;
  netwib_err ret;

  for (i = 0; i < NETWIB_PRIV_CONFEASY_HWMAX; i++) {
    numtype[i] = 0;
    numtypelocal[i] = 0;
  }

  netwib_er(netwib_ring_init(&netwib_priv_conf_deduct_markitem_erase, NULL,
                             &pmarkring));

  /* obtain mark using device rings */
  netwib_er(netwib_ring_index_init(pcwlocal->pdevices, &pringindexlocal));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindexlocal, (netwib_ptr*)&pcdlocal);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    if (pcdlocal->hwtype >= NETWIB_PRIV_CONFEASY_HWMAX) {
      return(NETWIB_ERR_LOINTERNALERROR);
    }
    numtypelocal[pcdlocal->hwtype]++;
    netwib_er(netwib_ring_index_init(pcw->pdevices, &pringindex));
    ret = NETWIB_ERR_OK;
    while (NETWIB_TRUE) {
      ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pcd);
      if (ret != NETWIB_ERR_OK) {
        if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
        break;
      }
      if (pcd->hwtype >= NETWIB_PRIV_CONFEASY_HWMAX) {
        return(NETWIB_ERR_LOINTERNALERROR);
      }
      numtype[pcd->hwtype]++;
      ret = netwib_priv_conf_deduct_dev(pcd, pcdlocal, numtype[pcd->hwtype],
                                        numtypelocal[pcdlocal->hwtype], &mark);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
      ret = netwib_priv_conf_deduct_mark_add(pmarkring, pcdlocal->devnum,
                                             &pcd->device, mark);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
    }
    netwib_er(netwib_ring_index_close(&pringindex));
  }
  netwib_er(netwib_ring_index_close(&pringindexlocal));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }
  netwib_er(netwib_priv_conf_deduct_mark_debug(pcw, pmarkring,
                                               "after mark dev"));

  /* obtain mark using ip rings */
  netwib_er(netwib_ring_index_init(pcwlocal->pip, &pringindexlocal));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindexlocal, (netwib_ptr*)&pcilocal);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_er(netwib_ring_index_init(pcw->pip, &pringindex));
    ret = NETWIB_ERR_OK;
    while (NETWIB_TRUE) {
      ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pci);
      if (ret != NETWIB_ERR_OK) {
        if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
        break;
      }
      ret = netwib_priv_conf_deduct_ip(pci, pcilocal, &mark);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
      ret = netwib_priv_conf_deduct_mark_add(pmarkring, pcilocal->devnum,
                                             &pci->device, mark);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
    }
    netwib_er(netwib_ring_index_close(&pringindex));
  }
  netwib_er(netwib_ring_index_close(&pringindexlocal));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }
  netwib_er(netwib_priv_conf_deduct_mark_debug(pcw, pmarkring,
                                               "after mark ip"));

  /* sort on mark */
  netwib_er(netwib_ring_sort(pmarkring, &netwib_priv_conf_deduct_mark_sort,
                             NULL));
  netwib_er(netwib_priv_conf_deduct_mark_debug(pcw, pmarkring,
                                               "after mark sort"));

  /* remove already set */
  netwib_er(netwib_ring_del_duplicate(pmarkring,
                                      &netwib_priv_conf_deduct_mark_dup, NULL,
                                      NETWIB_TRUE));
  netwib_er(netwib_priv_conf_deduct_mark_debug(pcw, pmarkring,
                                               "after mark dup"));

  /* set devices names */
  netwib_er(netwib_ring_index_init(pcwlocal->pdevices, &pringindexlocal));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindexlocal, (netwib_ptr*)&pcdlocal);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_er(netwib_priv_conf_deduct_mark_setdev(pmarkring, pcdlocal));
  }
  netwib_er(netwib_ring_index_close(&pringindexlocal));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  netwib_er(netwib_ring_close(&pmarkring, NETWIB_TRUE));
  return(NETWIB_ERR_OK);
}
