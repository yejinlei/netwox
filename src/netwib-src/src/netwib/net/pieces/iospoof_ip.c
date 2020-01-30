/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
typedef struct {
  netwib_buf device;
  netwib_bool pioinittryed;
  netwib_bool canusepio;
  netwib_io *pio;
  netwib_device_dlttype dlttype;
} spoof_ip_iolink;

typedef struct {
  netwib_bool pioinittryed;
  netwib_bool canusepio;
  netwib_io *pio;
} spoof_ip_ioip;

typedef struct {
  netwib_bool initialized;
  netwib_uint32 arraysize; /* size of the array */
  spoof_ip_iolink *array; /* dynamic array of spoof_ip_iolink */
} spoof_ip_iolinks;

typedef struct {
  netwib_bool initialized;
  netwib_ip src; /* used for comparison */
  netwib_ip dst; /* used for comparison */
  netwib_bool uselink;
  /* used by link */
  spoof_ip_iolink *plink; /* used for storage */
  netwib_linkhdr linkhdr; /* used for storage */
  /* used by raw IP4/6 */
  spoof_ip_ioip *pip; /* used for storage */
} spoof_ip_last;

typedef struct {
  netwib_spoof_ip_inittype inittype;
  netwib_pkt_newfield newfield;
  spoof_ip_iolinks links;
  spoof_ip_ioip ip4;
  spoof_ip_ioip ip6;
  spoof_ip_last lastused;
  netwib_buf bufipfrag; /* temporary storage used for ip fragmentation */
} netwib_priv_io_spoof_ip;


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
static netwib_err spoof_ip_iolinks_init(spoof_ip_iolinks *piols)
{
  netwib_uint32 maxdevnum, i;
  netwib_conf_devices conf;
  netwib_conf_devices_index *pconfindex;
  spoof_ip_iolink *piol;
  netwib_err ret;

  /* first, obtain maximum device number (should be quite small) */
  maxdevnum = 0;
  netwib_er(netwib_conf_devices_index_init(&conf, &pconfindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_conf_devices_index_next(pconfindex);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    if (conf.devnum > maxdevnum) {
      maxdevnum = conf.devnum;
    }
  }
  netwib_er(netwib_conf_devices_index_close(&pconfindex));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  /* allocate and initialize the array */
  piols->initialized = NETWIB_TRUE;
  piols->arraysize = maxdevnum + 1;
  netwib_er(netwib_ptr_malloc(piols->arraysize*sizeof(spoof_ip_iolink),
                              (netwib_ptr*)&piols->array));
  for (i = 0; i < piols->arraysize; i++) {
    piol = &piols->array[i];
    netwib_er(netwib_buf_init_mallocdefault(&piol->device));
    piol->pioinittryed = NETWIB_FALSE;
  }

  /* now add info about devices */
  netwib_er(netwib_conf_devices_index_init(&conf, &pconfindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_conf_devices_index_next(pconfindex);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    if (conf.devnum >= piols->arraysize) {
      /* configuration changed : ignore this entry */
      continue;
    }
    piol = &piols->array[conf.devnum];
    netwib_er(netwib_buf_append_buf(&conf.device, &piol->device));
  }
  netwib_er(netwib_conf_devices_index_close(&pconfindex));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err spoof_ip_iolinks_close(spoof_ip_iolinks *piols)
{
  spoof_ip_iolink *piol;
  netwib_uint32 i;

  if (piols->initialized) {
    for (i = 0; i < piols->arraysize; i++) {
      piol = &piols->array[i];
      netwib_er(netwib_buf_close(&piol->device));
      if (piol->pioinittryed && piol->canusepio) {
        netwib_er(netwib_io_close(&piol->pio));
      }
    }
    netwib_er(netwib_ptr_free((netwib_ptr*)&piols->array));
  }

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
static netwib_err spoof_ip_last_update_raw(netwib_priv_io_spoof_ip *psi,
                                           netwib_iptype iptype)
{
  netwib_err ret;

  /* try to set info */
  switch(iptype) {
    case NETWIB_IPTYPE_IP4 :
      if (!psi->ip4.pioinittryed) {
        ret = netwib_io_init_spoof_ip4(&psi->ip4.pio);
        if (ret == NETWIB_ERR_OK) {
          psi->ip4.canusepio = NETWIB_TRUE;
        } else {
          psi->ip4.canusepio = NETWIB_FALSE;
        }
        psi->ip4.pioinittryed = NETWIB_TRUE;
      }
      if (!psi->ip4.canusepio) {
        return(NETWIB_ERR_LONOTSUPPORTED);
      }
      psi->lastused.pip = &psi->ip4;
      break;
    case NETWIB_IPTYPE_IP6 :
      if (!psi->ip6.pioinittryed) {
        ret = netwib_io_init_spoof_ip6(&psi->ip6.pio);
        if (ret == NETWIB_ERR_OK) {
          psi->ip6.canusepio = NETWIB_TRUE;
        } else {
          psi->ip6.canusepio = NETWIB_FALSE;
        }
        psi->ip6.pioinittryed = NETWIB_TRUE;
      }
      if (!psi->ip6.canusepio) {
        return(NETWIB_ERR_LONOTSUPPORTED);
      }
      psi->lastused.pip = &psi->ip6;
      break;
    default :
      return(NETWIB_ERR_LONOTSUPPORTED);
  }

  psi->lastused.uselink = NETWIB_FALSE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err spoof_ip_last_update_link(netwib_priv_io_spoof_ip *psi,
                                            netwib_constbuf *pbuf,
                                            netwib_constip *pdst)
{
  netwib_uint32 devnum;
  netwib_ip gw;
  spoof_ip_iolink *piol;
  netwib_err ret;

  /* eventually init links */
  if (!psi->links.initialized) {
    netwib_er(spoof_ip_iolinks_init(&psi->links));
  }

  /* loop through routes and obtain devicenumber and gateway */
  netwib_er(netwib_priv_conf_routes_gw(pdst, &gw, &devnum));

  /* obtain link info */
  if (devnum >= psi->links.arraysize) {
    /* configuration changed */
    return(NETWIB_ERR_LONOTSUPPORTED);
  }
  piol = &psi->links.array[devnum];

  /* eventually initialize the io */
  if (!piol->pioinittryed) {
    piol->canusepio = NETWIB_FALSE;
    ret = netwib_io_init_spoof_link(&piol->device, &piol->pio);
    if (ret == NETWIB_ERR_OK) {
      ret = netwib_spoof_ctl_get_dlt(piol->pio, &piol->dlttype);
      if (ret == NETWIB_ERR_OK) {
        piol->canusepio = NETWIB_TRUE;
      }
    }
    piol->pioinittryed = NETWIB_TRUE;
  }
  if (!piol->canusepio) {
    return(NETWIB_ERR_LONOTSUPPORTED);
  }

  /* try to construct linkhdr */
  netwib_er(netwib_priv_pkt_convert(NETWIB_DEVICE_DLTTYPE_RAW, pbuf,
                                    psi->newfield, NETWIB_PKT_NEWFIELD_FILL,
                                    &gw, piol->dlttype, &psi->lastused.linkhdr,
                                    NULL, NULL, NULL));

  psi->lastused.plink = piol;
  psi->lastused.uselink = NETWIB_TRUE;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err spoof_ip_last_update(netwib_priv_io_spoof_ip *psi,
                                       netwib_constbuf *pbuf,
                                       netwib_bool secondchance)
{
  netwib_ip src, dst;
  netwib_cmp cmp;
  netwib_err ret;

  /* obtain src and dst IP addresses from packet */
  netwib_er(netwib_priv_ippkt_decode_srcdst(pbuf, &src, &dst));

  /* if they are equal to last used, simply reuse last info to spoof */
  if (psi->lastused.initialized) {
    netwib_er(netwib_ip_cmp(&dst, &psi->lastused.dst, &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      netwib_er(netwib_ip_cmp(&src, &psi->lastused.src, &cmp));
      if (cmp == NETWIB_CMP_EQ) {
        return(NETWIB_ERR_OK);
      }
    }
  }

  /* we have to set last info */
  if (!secondchance) {
    switch(psi->inittype) {
    case NETWIB_SPOOF_IP_INITTYPE_RAW :
      netwib_er(spoof_ip_last_update_raw(psi, dst.iptype));
      break;
    case NETWIB_SPOOF_IP_INITTYPE_LINK :
      netwib_er(spoof_ip_last_update_link(psi, pbuf, &dst));
      break;
    case NETWIB_SPOOF_IP_INITTYPE_RAWLINK :
      ret = spoof_ip_last_update_raw(psi, dst.iptype);
      if (ret != NETWIB_ERR_OK) {
        netwib_er(spoof_ip_last_update_link(psi, pbuf, &dst));
      }
      break;
    case NETWIB_SPOOF_IP_INITTYPE_LINKRAW :
      ret = spoof_ip_last_update_link(psi, pbuf, &dst);
      if (ret != NETWIB_ERR_OK) {
        netwib_er(spoof_ip_last_update_raw(psi, dst.iptype));
      }
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
    }
  } else {
    switch(psi->inittype) {
    case NETWIB_SPOOF_IP_INITTYPE_RAW :
    case NETWIB_SPOOF_IP_INITTYPE_LINK :
      return(NETWIB_ERR_LONOTSUPPORTED);
      break;
    case NETWIB_SPOOF_IP_INITTYPE_RAWLINK :
      if (psi->lastused.uselink) {
        return(NETWIB_ERR_LONOTSUPPORTED);
      }
      netwib_er(spoof_ip_last_update_link(psi, pbuf, &dst));
      break;
    case NETWIB_SPOOF_IP_INITTYPE_LINKRAW :
      if (!psi->lastused.uselink) {
        return(NETWIB_ERR_LONOTSUPPORTED);
      }
      netwib_er(spoof_ip_last_update_raw(psi, dst.iptype));
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
    }
  }

  psi->lastused.src = src;
  psi->lastused.dst = dst;
  psi->lastused.initialized = NETWIB_TRUE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err spoof_ip_spoof_frag(netwib_constbuf *pfrag,
                                      netwib_ptr infos)
{
  netwib_priv_io_spoof_ip *psi = (netwib_priv_io_spoof_ip *)infos;
  spoof_ip_iolink *piol;

  piol = psi->lastused.plink;
  netwib_er(netwib_io_write(piol->pio, pfrag));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err spoof_ip_spoof(netwib_priv_io_spoof_ip *psi,
                                 netwib_constbuf *pbuf)
{
  spoof_ip_iolink *piol;
  netwib_uint32 mtu;
  netwib_iptype iptype;
  netwib_bool fragneeded;

  /* easy case */
  if (!psi->lastused.uselink) {
    netwib_er(netwib_io_write(psi->lastused.pip->pio, pbuf));
    return(NETWIB_ERR_OK);
  }

  /* spoof using specified link pio */
  piol = psi->lastused.plink;
  iptype = psi->lastused.dst.iptype;

  /* decide if fragmentation is needed (do not use device MTU because
     a smaller value can be found on the path) */
  mtu = (iptype == NETWIB_IPTYPE_IP4)?NETWIB_MTU_MIN_IP4:NETWIB_MTU_MIN_IP6;
  fragneeded = NETWIB_FALSE;
  if (netwib__buf_ref_data_size(pbuf) > mtu) {
    fragneeded = NETWIB_TRUE;
  }

  /* send packet(s) */
  if (fragneeded) {
    netwib_er(netwib_ip_frag(pbuf, 0, mtu, &spoof_ip_spoof_frag, psi));
  } else {
    netwib__buf_reinit(&psi->bufipfrag);
    netwib_er(netwib_pkt_append_linkdata(&psi->lastused.linkhdr, pbuf,
                                         &psi->bufipfrag));
    netwib_er(netwib_io_write(piol->pio, &psi->bufipfrag));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err spoof_ip_write(netwib_priv_io_spoof_ip *psi,
                                 netwib_constbuf *pbuf)
{
  netwib_err ret;

  /* eventually update last info */
  netwib_er(spoof_ip_last_update(psi, pbuf, NETWIB_FALSE));

  /* spoof using last info */
  ret = spoof_ip_spoof(psi, pbuf);
  if (ret == NETWIB_ERR_OK) {
    /* done */
    return(NETWIB_ERR_OK);
  }

  /* on error, check if we have a second chance (ie LINKRAW
     and only LINK tried : we also try RAW) */
  psi->lastused.initialized = NETWIB_FALSE;
  netwib_er(spoof_ip_last_update(psi, pbuf, NETWIB_TRUE));
  ret = spoof_ip_spoof(psi, pbuf);
  if (ret != NETWIB_ERR_OK) {
    psi->lastused.initialized = NETWIB_FALSE;
  }

  return(ret);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_spoof_ip_init(netwib_spoof_ip_inittype inittype,
                                               netwib_pkt_newfield newfield,
                                               netwib_bool *preadinitialized,
                                               netwib_bool *pwriteinitialized,
                                               netwib_priv_io_spoof_ip *ptr)
{

  /* initialize structures */
  ptr->inittype = inittype;
  ptr->newfield = newfield;
  ptr->links.initialized = NETWIB_FALSE;
  ptr->ip4.pioinittryed = NETWIB_FALSE;
  ptr->ip6.pioinittryed = NETWIB_FALSE;
  ptr->lastused.initialized = NETWIB_FALSE;
  netwib_er(netwib_buf_init_mallocdefault(&ptr->bufipfrag));

  /* set io values */
  *preadinitialized = NETWIB_FALSE;
  *pwriteinitialized = NETWIB_TRUE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_spoof_ip_close(netwib_priv_io_spoof_ip *ptr)
{

  /* close structures */
  netwib_er(netwib_buf_close(&ptr->bufipfrag));
  if (ptr->ip6.pioinittryed && ptr->ip6.canusepio) {
    netwib_er(netwib_io_close(&ptr->ip6.pio));
  }
  if (ptr->ip4.pioinittryed && ptr->ip4.canusepio) {
    netwib_er(netwib_io_close(&ptr->ip4.pio));
  }
  netwib_er(spoof_ip_iolinks_close(&ptr->links));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_spoof_ip_write(netwib_io *pio,
                                                netwib_constbuf *pbuf)
{
  netwib_priv_io_spoof_ip *ptr = (netwib_priv_io_spoof_ip *)pio->pcommon;

  netwib_er(spoof_ip_write(ptr, pbuf));

  return(NETWIB_ERR_OK);
}
/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_spoof_ip_wait(netwib_io *pio,
                                               netwib_io_waytype way,
                                               netwib_consttime *pabstime,
                                               netwib_bool *pevent)
{
  *pevent = NETWIB_TRUE;

  pio = pio; /* for compiler warning */
  way = way; /* for compiler warning */
  pabstime = pabstime; /* for compiler warning */
  pevent = pevent; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_spoof_ip_ctl_get(netwib_io *pio,
                                                  netwib_io_waytype way,
                                                  netwib_io_ctltype type,
                                                  netwib_ptr p,
                                                  netwib_uint32 *pui)
{

  switch(type) {
    case NETWIB_IO_CTLTYPE_SPOOF_DLT:
      if (pui != NULL) *pui = NETWIB_DEVICE_DLTTYPE_RAW;
      return(NETWIB_ERR_OK);
    default:
      return(NETWIB_ERR_PLEASETRYNEXT);
  }

  pio = pio; /* for compiler warning */
  way = way; /* for compiler warning */
  p = p; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_spoof_ip_fclose(netwib_io *pio)
{
  netwib_priv_io_spoof_ip *ptr = (netwib_priv_io_spoof_ip *)pio->pcommon;

  netwib_er(netwib_priv_io_spoof_ip_close(ptr));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_spoof_ip(netwib_spoof_ip_inittype inittype,
                                   netwib_io **ppio)
{
  netwib_pkt_newfield newfield;
  netwib_bool rdinit, wrinit;
  netwib_ptr pcommon;
  netwib_err ret;

  newfield = NETWIB_PKT_NEWFIELD_FILL;
  switch(inittype) {
  case NETWIB_SPOOF_IP_INITTYPE_UNKNOWN :
  case NETWIB_SPOOF_IP_INITTYPE_RAW :
    break;
  case NETWIB_SPOOF_IP_INITTYPE_LINKB :
    inittype = NETWIB_SPOOF_IP_INITTYPE_LINK;
    newfield = NETWIB_PKT_NEWFIELD_BLANK;
    break;
  case NETWIB_SPOOF_IP_INITTYPE_LINKF :
    inittype = NETWIB_SPOOF_IP_INITTYPE_LINK;
    break;
  case NETWIB_SPOOF_IP_INITTYPE_LINKFB :
    inittype = NETWIB_SPOOF_IP_INITTYPE_LINK;
    newfield = NETWIB_PKT_NEWFIELD_FILLBLANK;
    break;
  case NETWIB_SPOOF_IP_INITTYPE_RAWLINKB :
    inittype = NETWIB_SPOOF_IP_INITTYPE_RAWLINK;
    newfield = NETWIB_PKT_NEWFIELD_BLANK;
    break;
  case NETWIB_SPOOF_IP_INITTYPE_RAWLINKF :
    inittype = NETWIB_SPOOF_IP_INITTYPE_RAWLINK;
    break;
  case NETWIB_SPOOF_IP_INITTYPE_RAWLINKFB :
    inittype = NETWIB_SPOOF_IP_INITTYPE_RAWLINK;
    newfield = NETWIB_PKT_NEWFIELD_FILLBLANK;
    break;
  case NETWIB_SPOOF_IP_INITTYPE_LINKBRAW :
    inittype = NETWIB_SPOOF_IP_INITTYPE_LINKRAW;
    newfield = NETWIB_PKT_NEWFIELD_BLANK;
    break;
  case NETWIB_SPOOF_IP_INITTYPE_LINKFRAW :
    inittype = NETWIB_SPOOF_IP_INITTYPE_LINKRAW;
    break;
  case NETWIB_SPOOF_IP_INITTYPE_LINKFBRAW :
    inittype = NETWIB_SPOOF_IP_INITTYPE_LINKRAW;
    newfield = NETWIB_PKT_NEWFIELD_FILLBLANK;
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_spoof_ip), &pcommon));
  ret = netwib_priv_io_spoof_ip_init(inittype, newfield, &rdinit, &wrinit,
                                     (netwib_priv_io_spoof_ip *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdinit, wrinit,
                           pcommon,
                           NULL, /* read */
                           &netwib_priv_io_spoof_ip_write,
                           &netwib_priv_io_spoof_ip_wait,
                           NULL, /* unread */
                           NULL, /* set */
                           &netwib_priv_io_spoof_ip_ctl_get,
                           &netwib_priv_io_spoof_ip_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

