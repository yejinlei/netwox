/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_devices_init(netwib_priv_confwork_devices **ppitem)
{
  netwib_priv_confwork_devices *pitem;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_confwork_devices),
                              (netwib_ptr*)ppitem));
  pitem = *ppitem;
  pitem->devnum = 0;
  netwib_er(netwib_buf_init_mallocdefault(&pitem->device));
  netwib_er(netwib_buf_init_mallocdefault(&pitem->deviceeasy));
  pitem->mtu = 0;
  pitem->hwtype = NETWIB_DEVICE_HWTYPE_UNKNOWN;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_devices_close(netwib_priv_confwork_devices **ppitem)
{
  netwib_priv_confwork_devices *pitem;

  pitem = *ppitem;
  netwib_er(netwib_buf_close(&pitem->device));
  netwib_er(netwib_buf_close(&pitem->deviceeasy));
  netwib_er(netwib_ptr_free((netwib_ptr*)ppitem));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confwork_dev_noalias(netwib_buf *pbuf)
{
  netwib_string str, pc;

  netwib_er(netwib_buf_ref_string(pbuf, &str));
  pc = netwib_c_strchr(str, ':');
  if (pc != NULL) {
    pbuf->endoffset = pbuf->beginoffset + pc - str;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_devices_add(netwib_priv_confwork *pcw,
                                            netwib_priv_confwork_devices *pitem)
{
  netwib_er(netwib_priv_confwork_dev_noalias(&pitem->device));
  netwib_er(netwib_ring_add_last(pcw->pdevices, pitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_devices_erase(netwib_ptr pitem)
{
  netwib_er(netwib_priv_confwork_devices_close((netwib_priv_confwork_devices **)&pitem));
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
netwib_err netwib_conf_devices_init(netwib_conf_devices **ppitem)
{
  netwib_conf_devices *pitem;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_conf_devices),
                              (netwib_ptr*)ppitem));
  pitem = *ppitem;
  netwib_er(netwib_buf_init_mallocdefault(&pitem->device));
  netwib_er(netwib_buf_init_mallocdefault(&pitem->deviceeasy));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_conf_devices_close(netwib_conf_devices **ppitem)
{
  netwib_conf_devices *pitem;

  pitem = *ppitem;
  netwib_er(netwib_buf_close(&pitem->device));
  netwib_er(netwib_buf_close(&pitem->deviceeasy));
  netwib_er(netwib_ptr_free((netwib_ptr*)ppitem));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_conf_devices_add(netwib_priv_conf_t *pc,
                                   netwib_conf_devices *pitem)
{
  netwib_er(netwib_ring_add_last(pc->pdevices, pitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_conf_devices_erase(netwib_ptr pitem)
{
  netwib_er(netwib_conf_devices_close((netwib_conf_devices **)&pitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_conf_device_info(netwib_constbuf *pdevice,
                                        netwib_buf *prealdevice,
                                        netwib_uint32 *pmtu,
                                        netwib_device_hwtype *phwtype,
                                        netwib_eth *peth)
{
  netwib_conf_devices conf;
  netwib_conf_devices_index *pconfindex;
  netwib_cmp cmp;
  netwib_err ret;

  netwib_er(netwib_conf_devices_index_init(&conf, &pconfindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_conf_devices_index_next(pconfindex);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_NOTFOUND;
      break;
    }
    netwib_er(netwib_buf_cmp(&conf.device, pdevice, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      netwib_er(netwib_buf_cmp(&conf.deviceeasy, pdevice, &cmp));
    }
    if (cmp == NETWIB_CMP_EQ) {
      netwib_er(netwib_buf_append_buf(&conf.device, prealdevice));
      if (pmtu != NULL) *pmtu = conf.mtu;
      if (phwtype != NULL) *phwtype = conf.hwtype;
      if (peth != NULL) *peth = conf.eth;
      break;
    }
  }
  netwib_er(netwib_conf_devices_index_close(&pconfindex));

  return(ret);
}
