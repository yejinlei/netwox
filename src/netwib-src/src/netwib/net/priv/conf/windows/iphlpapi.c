/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_iphlpapi_devices(netwib_priv_confwork *pcw)
{
  netwib_priv_confwork_devices *pcd;
  PMIB_IFTABLE piftable;
  PMIB_IFROW pifrow;
  DWORD err, bufsize, i;

  /* get the IP table size */
  bufsize = 0;
  err = (netwib_global_dll_iphlpapi.GetIfTable)(NULL, &bufsize, FALSE);
  if (err == ERROR_NO_DATA) {
    return(NETWIB_ERR_OK);
  }
  if (err != ERROR_INSUFFICIENT_BUFFER) {
    return(NETWIB_ERR_FUGETIFTABLE);
  }
  if (bufsize == 0) {
    return(NETWIB_ERR_OK);
  }

  /* allocate data */
  netwib_er(netwib_ptr_malloc(bufsize, (netwib_ptr*)&piftable));

  /* get the interface table */
  err = netwib_global_dll_iphlpapi.GetIfTable(piftable, &bufsize, TRUE);
  if (err != NO_ERROR) {
    netwib_er(netwib_ptr_free((netwib_ptr*)&piftable));
    return(NETWIB_ERR_FUGETIFTABLE);
  }

  /* loop through each entry */
  for (i = 0; i < piftable->dwNumEntries; i++) {
    netwib_er(netwib_priv_confwork_devices_init(&pcd));
    pifrow = &piftable->table[i];
    pcd->devnum = pifrow->dwIndex;
    /* On a modern computer, there are so many devices, that we use
       the description instead of the Unicode name (wszName). */
    netwib_er(netwib_buf_append_string("Description>", &pcd->device));
    netwib_er(netwib_buf_append_data(pifrow->bDescr, pifrow->dwDescrLen, &pcd->device));
    /* I'm not sure if there is always a '\0' at the end of the bDescr buffer. */
    if (netwib__buf_ref_data_size(&pcd->device) != 0) {
      char *pc = netwib__buf_ref_data_ptr(&pcd->device);
      pc += netwib__buf_ref_data_size(&pcd->device) - 1;
      if (*pc == '\0') {
        pcd->device.endoffset--; /* Suppress the '\0'. */
      }
    }
    /* Determine the Hardware type. */
    netwib_er(netwib_priv_conf_mib_hwtype(pifrow->dwType, pifrow->dwSpeed,
                                          &pcd->hwtype));
    if (pcd->hwtype == NETWIB_DEVICE_HWTYPE_ETHER) {
      netwib_er(netwib_eth_init_fields(pifrow->bPhysAddr[0],
                                       pifrow->bPhysAddr[1],
                                       pifrow->bPhysAddr[2],
                                       pifrow->bPhysAddr[3],
                                       pifrow->bPhysAddr[4],
                                       pifrow->bPhysAddr[5], &pcd->eth));
    }
    pcd->mtu = pifrow->dwMtu;
    netwib_er(netwib_priv_confwork_devices_add(pcw, pcd));
  }

  netwib_er(netwib_ptr_free((netwib_ptr*)&piftable));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_iphlpapi_ip(netwib_priv_confwork *pcw,
                                               netwib_ips *plocalips)
{
  netwib_priv_confwork_ip *pci;
  PMIB_IPADDRTABLE pipaddrtable;
  PMIB_IPADDRROW pipaddrrow;
  DWORD err, bufsize, i;

  /* get the IP table size */
  bufsize = 0;
  err = (netwib_global_dll_iphlpapi.GetIpAddrTable)(NULL, &bufsize, FALSE);
  if (err == ERROR_NO_DATA) {
    return(NETWIB_ERR_OK);
  }
  if (err != ERROR_INSUFFICIENT_BUFFER) {
    return(NETWIB_ERR_FUGETIPADDRTABLE);
  }
  if (bufsize == 0) {
    return(NETWIB_ERR_OK);
  }

  /* allocate data */
  netwib_er(netwib_ptr_malloc(bufsize, (netwib_ptr*)&pipaddrtable));

  /* get the IP table */
  err = netwib_global_dll_iphlpapi.GetIpAddrTable(pipaddrtable, &bufsize,
                                                 FALSE);
  if (err != NO_ERROR) {
    netwib_er(netwib_ptr_free((netwib_ptr*)&pipaddrtable));
    return(NETWIB_ERR_FUGETIPADDRTABLE);
  }

  /* loop through each entry */
  for (i = 0; i < pipaddrtable->dwNumEntries; i++) {
    netwib_er(netwib_priv_confwork_ip_init(&pci));
    pipaddrrow = &pipaddrtable->table[i];
    pci->devnum = pipaddrrow->dwIndex;
    netwib_er(netwib_ip_init_ip4(netwib_priv_ntohl(pipaddrrow->dwAddr),
                                 &pci->ip));
    netwib_er(netwib_ip_init_ip4(netwib_priv_ntohl(pipaddrrow->dwMask),
                                 &pci->mask));
    if (pci->ip.iptype == NETWIB_IPTYPE_IP4 && pci->ip.ipvalue.ip4 == 0) {
      netwib_er(netwib_priv_confwork_ip_close(&pci));
    } else {
      netwib_er(netwib_priv_confwork_ip_add(pcw, pci));
      netwib_er(netwib_ips_add_ip(plocalips, &pci->ip));
    }
  }

  netwib_er(netwib_ptr_free((netwib_ptr*)&pipaddrtable));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_iphlpapi_arpcache(netwib_priv_confwork *pcw)
{
  netwib_priv_confwork_arpcache *pca;
  PMIB_IPNETTABLE pipnettable;
  PMIB_IPNETROW pipnetrow;
  DWORD err, bufsize, i;

  /* also check here because it is called directly */
  if (!netwib_global_dll_iphlpapi.isloaded) {
    return(NETWIB_ERR_LONOTSUPPORTED);
  }

  /* get arp table size */
  bufsize = 0;
  err = (netwib_global_dll_iphlpapi.GetIpNetTable)(NULL, &bufsize, FALSE);
  if (err == ERROR_NO_DATA) {
    return(NETWIB_ERR_OK);
  }
  if (err != ERROR_INSUFFICIENT_BUFFER) {
    return(NETWIB_ERR_FUGETIPNETTABLE);
  }
  if (bufsize == 0) {
    return(NETWIB_ERR_OK);
  }

  /* allocate data */
  netwib_er(netwib_ptr_malloc(bufsize, (netwib_ptr*)&pipnettable));

  /* get arp table */
  err = netwib_global_dll_iphlpapi.GetIpNetTable(pipnettable, &bufsize, FALSE);
  if (err != NO_ERROR) {
    netwib_er(netwib_ptr_free((netwib_ptr*)&pipnettable));
    return(NETWIB_ERR_FUGETIPNETTABLE);
  }

  /* loop through each entry */
  for (i = 0; i < pipnettable->dwNumEntries; i++) {
    /* Check if the entry is valid. */
    pipnetrow = &pipnettable->table[i];
    if (pipnetrow->dwPhysAddrLen != NETWIB_ETH_LEN) {
      continue;
    }
    /* Ignore broadcast addresses. */
    if (pipnetrow->bPhysAddr[0] == 0xFF &&
        pipnetrow->bPhysAddr[1] == 0xFF &&
        pipnetrow->bPhysAddr[2] == 0xFF &&
        pipnetrow->bPhysAddr[3] == 0xFF &&
        pipnetrow->bPhysAddr[4] == 0xFF &&
        pipnetrow->bPhysAddr[5] == 0xFF) {
      continue;
    }
    /* Ignore multicast addresses. */
    if (pipnetrow->bPhysAddr[0] == 0x01 &&
        pipnetrow->bPhysAddr[1] == 0x00 &&
        pipnetrow->bPhysAddr[2] == 0x5E) {
      continue;
    }
    /* Add the entry. */
    netwib_er(netwib_priv_confwork_arpcache_init(&pca));
    pca->devnum = pipnetrow->dwIndex;
    netwib_er(netwib_ip_init_ip4(netwib_priv_ntohl(pipnetrow->dwAddr),
                                 &pca->ip));
    netwib_er(netwib_eth_init_fields(pipnetrow->bPhysAddr[0],
                                     pipnetrow->bPhysAddr[1],
                                     pipnetrow->bPhysAddr[2],
                                     pipnetrow->bPhysAddr[3],
                                     pipnetrow->bPhysAddr[4],
                                     pipnetrow->bPhysAddr[5],
                                     &pca->eth));
    netwib_er(netwib_priv_confwork_arpcache_add(pcw, pca));
  }

  netwib_er(netwib_ptr_free((netwib_ptr*)&pipnettable));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_iphlpapi_routes(netwib_priv_confwork *pcw,
                                                   netwib_ips *plocalips)
{
  netwib_priv_confwork_routes *pcr;
  PMIB_IPFORWARDTABLE pipforwardtable;
  PMIB_IPFORWARDROW pipforwardrow;
  netwib_bool yes;
  DWORD err, bufsize, i;

  /* get arp table size */
  bufsize = 0;
  err = (netwib_global_dll_iphlpapi.GetIpForwardTable)(NULL, &bufsize, FALSE);
  if (err == ERROR_NO_DATA) {
    return(NETWIB_ERR_OK);
  }
  if (err != ERROR_INSUFFICIENT_BUFFER) {
    return(NETWIB_ERR_FUGETIPFORWARDTABLE);
  }
  if (bufsize == 0) {
    return(NETWIB_ERR_OK);
  }

  /* allocate data */
  netwib_er(netwib_ptr_malloc(bufsize, (netwib_ptr*)&pipforwardtable));

  /* get arp table */
  err = netwib_global_dll_iphlpapi.GetIpForwardTable(pipforwardtable,
                                                     &bufsize, FALSE);
  if (err != NO_ERROR) {
    netwib_er(netwib_ptr_free((netwib_ptr*)&pipforwardtable));
    return(NETWIB_ERR_FUGETIPFORWARDTABLE);
  }

  /* loop through each entry */
  for (i = 0; i < pipforwardtable->dwNumEntries; i++) {
    netwib_er(netwib_priv_confwork_routes_init(&pcr));
    pipforwardrow = &pipforwardtable->table[i];
    pcr->devnum = pipforwardrow->dwForwardIfIndex;
    netwib_er(netwib_ip_init_ip4(netwib_priv_ntohl(pipforwardrow->dwForwardDest),
                                 &pcr->dst));
    netwib_er(netwib_ip_init_ip4(netwib_priv_ntohl(pipforwardrow->dwForwardMask),
                                 &pcr->mask));
    netwib_er(netwib_ip_init_ip4(netwib_priv_ntohl(pipforwardrow->dwForwardNextHop), &pcr->gw));
    pcr->metric = pipforwardrow->dwForwardMetric1;
    pcr->gwset = NETWIB_TRUE;

    /* ignore gw set to one of the local addresses */
    netwib_er(netwib_ips_contains_ip(plocalips, &pcr->gw, &yes));
    if (yes) {
      netwib_er(netwib_priv_confwork_routes_close(&pcr));
    } else {
      netwib_er(netwib_priv_confwork_routes_add(pcw, pcr));
    }
  }

  netwib_er(netwib_ptr_free((netwib_ptr*)&pipforwardtable));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_iphlpapi(netwib_priv_confwork *pcw)
{
  netwib_ips *plocalips;

  if (!netwib_global_dll_iphlpapi.isloaded) {
    return(NETWIB_ERR_LONOTSUPPORTED);
  }

  netwib_er(netwib_ips_initdefault(&plocalips));

  netwib_er(netwib_priv_conf_iphlpapi_devices(pcw));
  netwib_er(netwib_priv_conf_iphlpapi_ip(pcw, plocalips));
  netwib_er(netwib_priv_conf_iphlpapi_arpcache(pcw));
  netwib_er(netwib_priv_conf_iphlpapi_routes(pcw, plocalips));

  netwib_er(netwib_ips_close(&plocalips));

  return(NETWIB_ERR_OK);
}


