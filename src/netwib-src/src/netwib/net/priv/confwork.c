/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_init(netwib_priv_confwork *pcw)
{
  netwib_er(netwib_ring_init(&netwib_priv_confwork_devices_erase, NULL,
                             &pcw->pdevices));
  netwib_er(netwib_ring_init(&netwib_priv_confwork_arpcache_erase, NULL,
                             &pcw->parpcache));
  netwib_er(netwib_ring_init(&netwib_priv_confwork_ip_erase, NULL,
                             &pcw->pip));
  netwib_er(netwib_ring_init(&netwib_priv_confwork_routes_erase, NULL,
                             &pcw->proutes));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_close(netwib_priv_confwork *pcw)
{
  netwib_er(netwib_ring_close(&pcw->pdevices, NETWIB_TRUE));
  netwib_er(netwib_ring_close(&pcw->pip, NETWIB_TRUE));
  netwib_er(netwib_ring_close(&pcw->parpcache, NETWIB_TRUE));
  netwib_er(netwib_ring_close(&pcw->proutes, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confwork_obtain_sys(netwib_priv_confwork *pcw);
#if NETWIB_DEBUG_CONF_FAKE == 1
 #include "conf/confdebug.c"
#else
 #if defined NETWIBDEF_SYSNAME_Linux
  #include "conf/conflinux.c"
 #elif defined NETWIBDEF_SYSNAME_Solaris
  #include "conf/confsolaris.c"
 #elif defined NETWIBDEF_SYSNAME_BSD
  #include "conf/confbsd.c"
 #elif defined NETWIBDEF_SYSNAME_HPUX
  #include "conf/confhpux.c"
 #elif defined NETWIBDEF_SYSNAME_Tru64UNIX
  #include "conf/conftru64unix.c"
 #elif defined NETWIBDEF_SYSNAME_AIX
  #include "conf/confaix.c"
 #elif defined NETWIBDEF_SYSNAME_Windows
  #include "conf/confwindows.c"
 #elif defined NETWIBDEF_SYSNAME_Unsupported
  #include "conf/confunsupported.c"
 #else
  #error "Unknown value for NETWIBDEF_SYSNAME"
 #endif
#endif

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confwork_clean(netwib_priv_confwork *pcw);
#include "conf/clean.c"

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confwork_easy(netwib_priv_confwork *pcw);
#include "conf/easy.c"

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confwork_reorder(netwib_priv_confwork *pcw);
#include "conf/reorder.c"

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_obtain(netwib_priv_confwork *pcw,
                                       netwib_bool debug,
                                       netwib_buf *pdebugbuf)
{
  pcw->debug = debug;
  pcw->pdebugbuf = pdebugbuf;

  netwib_er(netwib_priv_confwork_obtain_sys(pcw));
  netwib_er(netwib_priv_confwork_debug(pcw, "After obtain_sys"));

  netwib_er(netwib_priv_confwork_clean(pcw));
  netwib_er(netwib_priv_confwork_debug(pcw, "After clean"));

  netwib_er(netwib_priv_confwork_easy(pcw));
  netwib_er(netwib_priv_confwork_debug(pcw, "After easy"));

  netwib_er(netwib_priv_confwork_reorder(pcw));
  netwib_er(netwib_priv_confwork_debug(pcw, "After reorder"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_debug(netwib_priv_confwork *pcw,
                                      netwib_conststring title)
{
  netwib_ring_index *pringindex;
  netwib_priv_confwork_devices *pcd;
  netwib_priv_confwork_ip *pci;
  netwib_priv_confwork_arpcache *pca;
  netwib_priv_confwork_routes *pcr;
  netwib_buf *pbuf;
  netwib_err ret;

  if (!pcw->debug) {
    return(NETWIB_ERR_OK);
  }
  pbuf = pcw->pdebugbuf;

  netwib_er(netwib_buf_append_fmt(pbuf, "\n:::: %s ::::\n", title));

  netwib_er(netwib_buf_append_string(" $$$ devices $$$\n", pbuf));
  netwib_er(netwib_ring_index_init(pcw->pdevices, &pringindex));
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pcd);
    if (ret == NETWIB_ERR_DATAEND) break;
    netwib_er(netwib_buf_append_fmt(pbuf, "  d=%{uint32},%{buf},%{buf}",
                                    pcd->devnum,
                                    &pcd->device,&pcd->deviceeasy));
    netwib_er(netwib_buf_append_fmt(pbuf, " m=%{uint32} t=", pcd->mtu));
    netwib_er(netwib_buf_append_device_hwtype(pcd->hwtype, pbuf));
    if (pcd->hwtype == NETWIB_DEVICE_HWTYPE_ETHER) {
      netwib_er(netwib_buf_append_fmt(pbuf, ">%{eth}", &pcd->eth));
    }
    netwib_er(netwib_buf_append_string("\n", pbuf));
  }
  netwib_er(netwib_ring_index_close(&pringindex));

  netwib_er(netwib_buf_append_string("\n $$$ ip $$$\n", pbuf));
  netwib_er(netwib_ring_index_init(pcw->pip, &pringindex));
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pci);
    if (ret == NETWIB_ERR_DATAEND) break;
    netwib_er(netwib_buf_append_fmt(pbuf, "  d=%{uint32},%{buf}",
                                  pci->devnum, &pci->device));
    netwib_er(netwib_buf_append_fmt(pbuf, " i=%{ip}/", &pci->ip));
    if (pci->ip.iptype == NETWIB_IPTYPE_IP4) {
      netwib_er(netwib_buf_append_fmt(pbuf, "%{ip}", &pci->mask));
    } else {
      netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32}", pci->prefix));
    }
    netwib_er(netwib_buf_append_fmt(pbuf, " p=%{bool}", pci->ispointtopoint));
    if (pci->ispointtopoint) {
      netwib_er(netwib_buf_append_fmt(pbuf, ",%{ip}", &pci->pointtopointip));
    }
    netwib_er(netwib_buf_append_string("\n", pbuf));
  }
  netwib_er(netwib_ring_index_close(&pringindex));

  netwib_er(netwib_buf_append_string("\n $$$ arpcache $$$\n", pbuf));
  netwib_er(netwib_ring_index_init(pcw->parpcache, &pringindex));
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pca);
    if (ret == NETWIB_ERR_DATAEND) break;
    netwib_er(netwib_buf_append_fmt(pbuf, "  d=%{uint32},%{buf}",
                                  pca->devnum, &pca->device));
    netwib_er(netwib_buf_append_fmt(pbuf, " e=%{eth}", &pca->eth));
    netwib_er(netwib_buf_append_fmt(pbuf, " i=%{ip}\n", &pca->ip));
  }
  netwib_er(netwib_ring_index_close(&pringindex));

  netwib_er(netwib_buf_append_string("\n $$$ routes $$$\n", pbuf));
  netwib_er(netwib_ring_index_init(pcw->proutes, &pringindex));
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pcr);
    if (ret == NETWIB_ERR_DATAEND) break;
    netwib_er(netwib_buf_append_fmt(pbuf, "  d=%{uint32},%{buf}",
                                  pcr->devnum, &pcr->device));
    netwib_er(netwib_buf_append_fmt(pbuf, " i=%{ip}/", &pcr->dst));
    if (pcr->dst.iptype == NETWIB_IPTYPE_IP4) {
      netwib_er(netwib_buf_append_fmt(pbuf, "%{ip}", &pcr->mask));
    } else {
      netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32}", pcr->prefix));
    }
    netwib_er(netwib_buf_append_fmt(pbuf, " s=%{bool}", pcr->srcset));
    if (pcr->srcset) {
      netwib_er(netwib_buf_append_fmt(pbuf, ",%{ip}",  &pcr->src));
    }
    netwib_er(netwib_buf_append_fmt(pbuf, " g=%{bool}", pcr->gwset));
    if (pcr->gwset) {
      netwib_er(netwib_buf_append_fmt(pbuf, ",%{ip}",  &pcr->gw));
    }
    netwib_er(netwib_buf_append_fmt(pbuf, " m=%{uint32}", pcr->metric));
    netwib_er(netwib_buf_append_string("\n", pbuf));
  }
  netwib_er(netwib_ring_index_close(&pringindex));

  return(NETWIB_ERR_OK);
}

