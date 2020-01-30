/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
/***
  Used by :
    BSD
 ***/

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_arpcache_sysctl(netwib_priv_confwork *pcw)
{
  netwib_priv_confwork_arpcache *pca;
  netwib_buf bufrtlist;
  netwib_data rtlist;
  netwib_bool bufrtlistset, pcaset;
  netwib_device_hwtype hwtype;
  netwib_c_sizet len;
  netwib_err ret;
  struct rt_msghdr *prtm;
  struct sockaddr_inarp *psia;
  struct sockaddr_dl *psdl;
  int mib[6], reti;

  ret = NETWIB_ERR_OK;
  pcaset = NETWIB_FALSE;
  bufrtlistset = NETWIB_FALSE;

  /* obtain list of interfaces */
  mib[0] = CTL_NET;
  mib[1] = PF_ROUTE;
  mib[2] = 0;
  mib[3] = AF_UNSPEC;
  mib[4] = NET_RT_FLAGS;
  mib[5] = RTF_LLINFO;
  reti = sysctl(mib, 6, NULL, &len, NULL, 0);
  netwib_ir(reti, NETWIB_ERR_FUSYSCTL);
  netwib_er(netwib_buf_init_malloc(len, &bufrtlist));
  bufrtlistset = NETWIB_TRUE;
  rtlist = netwib__buf_ref_data_ptr(&bufrtlist);
  reti = sysctl(mib, 6, rtlist, &len, NULL, 0);
  /* when the ARP cache is empty, we got this error ; so ignore it */
  netwib_ig(reti, NETWIB_ERR_OK);
  bufrtlist.endoffset = bufrtlist.beginoffset + len;
  prtm = (struct rt_msghdr *)(void*)rtlist;

  /* loop on each item */
  while (NETWIB_TRUE) {
    /* check ending condition */
    if (bufrtlist.beginoffset >= bufrtlist.endoffset) {
      break;
    }
    /* set position */
    psia = (struct sockaddr_inarp *)(prtm + 1);
    psdl = (struct sockaddr_dl *)(psia + 1);
    netwib_eg(netwib_priv_confwork_arpcache_init(&pca));
    pcaset = NETWIB_TRUE;
    /* device */
    netwib_eg(netwib_buf_append_data((netwib_constdata)psdl->sdl_data,
                                     psdl->sdl_nlen,
                                     &pca->device));
    /* hwtype */
    netwib_eg(netwib_priv_conf_hwtype_init_sys(psdl->sdl_type, &hwtype));
    if (hwtype != NETWIB_DEVICE_HWTYPE_ETHER) {
      goto netwib_trynextentry;
    }
    /* Ethernet (expired values are bad) */
    netwib_c_memcpy(pca->eth.b, LLADDR(psdl), NETWIB_ETH_LEN);
    if (pca->eth.b[0] == 0 && pca->eth.b[1] == 0 && pca->eth.b[2] == 0 &&
        pca->eth.b[3] == 0 && pca->eth.b[4] == 0 && pca->eth.b[5] == 0) {
      goto netwib_trynextentry;
    }
    if (pca->eth.b[0] == 0xFF && pca->eth.b[1]==0xFF && pca->eth.b[2]==0xFF &&
        pca->eth.b[3] == 0xFF && pca->eth.b[4]==0xFF && pca->eth.b[5]==0xFF) {
      goto netwib_trynextentry;
    }
    /* IP */
    netwib_eg(netwib_priv_sa_ip_init_sa((netwib_priv_sockaddr_unalign *)psia,
                                        &pca->ip));
    /* save value */
    netwib_eg(netwib_priv_confwork_arpcache_add(pcw, pca));
    pcaset = NETWIB_FALSE;

    /** NEXT **/
  netwib_trynextentry:
    if (pcaset) {
      netwib_eg(netwib_priv_confwork_arpcache_close(&pca));
      pcaset = NETWIB_FALSE;
    }
    /* using shift ensures pointer is always aligned */
    netwib_eg(netwib_buf_shift(&bufrtlist, -prtm->rtm_msglen, NETWIB_TRUE));
  }

 netwib_gotolabel:
  if (bufrtlistset) {
    netwib_er(netwib_buf_close(&bufrtlist));
  }
  if (pcaset) {
    netwib_eg(netwib_priv_confwork_arpcache_close(&pca));
  }
  return(ret);
}

