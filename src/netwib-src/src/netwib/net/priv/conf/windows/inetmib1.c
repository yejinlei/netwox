/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_inetmib1_devices(netwib_priv_confwork *pcw)
{
  netwib_priv_confwork_devices *pcd;
  netwib_priv_dll_snmpreq snmpdllreq;
  AsnObjectIdentifier oid0, oid1, oid2, oid3, oid4;
  netwib_uint32 ifnum, mactype, speed, i;

  /* get the number of interfaces */
  netwib_er(netwib_priv_dll_snmpoid_init(&oid0, 1, 3, 6, 1, 2, 1, 2, 1, -1));
  netwib_er(netwib_priv_dll_snmpreq_init(&snmpdllreq, &oid0, NULL, NULL,
                                         NULL, NULL));
  netwib_er(netwib_priv_dll_snmpoid_close(&oid0));
  netwib_er(netwib_priv_dll_snmpreq_getnext(&snmpdllreq));
  netwib_er(netwib_priv_dll_snmpreq__uint32(&snmpdllreq, 0, &ifnum));
  netwib_er(netwib_priv_dll_snmpreq_close(&snmpdllreq));
  if (ifnum == 0) {
    return(NETWIB_ERR_OK);
  }

  /* get devices info */
  netwib_er(netwib_priv_dll_snmpoid_init(&oid0, 1,3,6,1,2,1,2,2,1,1, -1));
  netwib_er(netwib_priv_dll_snmpoid_init(&oid1, 1,3,6,1,2,1,2,2,1,3, -1));
  netwib_er(netwib_priv_dll_snmpoid_init(&oid2, 1,3,6,1,2,1,2,2,1,4, -1));
  netwib_er(netwib_priv_dll_snmpoid_init(&oid3, 1,3,6,1,2,1,2,2,1,5, -1));
  netwib_er(netwib_priv_dll_snmpoid_init(&oid4, 1,3,6,1,2,1,2,2,1,6, -1));
  netwib_er(netwib_priv_dll_snmpreq_init(&snmpdllreq, &oid0, &oid1, &oid2,
                                         &oid3, &oid4));
  netwib_er(netwib_priv_dll_snmpoid_close(&oid0));
  netwib_er(netwib_priv_dll_snmpoid_close(&oid1));
  netwib_er(netwib_priv_dll_snmpoid_close(&oid2));
  netwib_er(netwib_priv_dll_snmpoid_close(&oid3));
  netwib_er(netwib_priv_dll_snmpoid_close(&oid4));
  for (i = 0; i < ifnum; i++) {
    netwib_er(netwib_priv_confwork_devices_init(&pcd));
    netwib_er(netwib_priv_dll_snmpreq_getnext(&snmpdllreq));
    netwib_er(netwib_priv_dll_snmpreq__uint32(&snmpdllreq, 0, &pcd->devnum));
    netwib_er(netwib_priv_dll_snmpreq__uint32(&snmpdllreq, 1, &mactype));
    netwib_er(netwib_priv_dll_snmpreq__uint32(&snmpdllreq, 2, &pcd->mtu));
    netwib_er(netwib_priv_dll_snmpreq__uint32(&snmpdllreq, 3, &speed));
    netwib_er(netwib_priv_conf_mib_hwtype(mactype, speed, &pcd->hwtype));
    if (pcd->hwtype == NETWIB_DEVICE_HWTYPE_ETHER) {
      netwib_er(netwib_priv_dll_snmpreq__eth(&snmpdllreq, 4, &pcd->eth));
    }
    netwib_er(netwib_priv_confwork_devices_add(pcw, pcd));
  }
  netwib_er(netwib_priv_dll_snmpreq_close(&snmpdllreq));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_inetmib1_ip(netwib_priv_confwork *pcw,
                                               netwib_ips *plocalips)
{
  netwib_priv_confwork_ip *pci;
  netwib_priv_dll_snmpreq snmpdllreq;
  AsnObjectIdentifier oid0, oid1, oid2;

  netwib_er(netwib_priv_dll_snmpoid_init(&oid0, 1,3,6,1,2,1,4,20,1,1, -1));
  netwib_er(netwib_priv_dll_snmpoid_init(&oid1, 1,3,6,1,2,1,4,20,1,2, -1));
  netwib_er(netwib_priv_dll_snmpoid_init(&oid2, 1,3,6,1,2,1,4,20,1,3, -1));
  netwib_er(netwib_priv_dll_snmpreq_init(&snmpdllreq, &oid0, &oid1, &oid2,
                                         NULL, NULL));
  netwib_er(netwib_priv_dll_snmpoid_close(&oid0));
  netwib_er(netwib_priv_dll_snmpoid_close(&oid1));
  netwib_er(netwib_priv_dll_snmpoid_close(&oid2));
  netwib_er(netwib_priv_dll_snmpreq_getnext(&snmpdllreq));
  while (snmpdllreq.varBindList.list[0].name.ids[7] == 20
         && snmpdllreq.varBindList.list[0].name.ids[9] == 1) {
    netwib_er(netwib_priv_confwork_ip_init(&pci));
    netwib_er(netwib_priv_dll_snmpreq__ip(&snmpdllreq, 0, &pci->ip));
    netwib_er(netwib_priv_dll_snmpreq__uint32(&snmpdllreq, 1, &pci->devnum));
    netwib_er(netwib_priv_dll_snmpreq__ip(&snmpdllreq, 2, &pci->mask));
    if (pci->ip.iptype == NETWIB_IPTYPE_IP4 && pci->ip.ipvalue.ip4 == 0) {
      netwib_er(netwib_priv_confwork_ip_close(&pci));
    } else {
      netwib_er(netwib_priv_confwork_ip_add(pcw, pci));
      netwib_er(netwib_ips_add_ip(plocalips, &pci->ip));
    }
    /* get next */
    netwib_er(netwib_priv_dll_snmpreq_getnext(&snmpdllreq));
  }
  netwib_er(netwib_priv_dll_snmpreq_close(&snmpdllreq));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_inetmib1_arpcache(netwib_priv_confwork *pcw)
{
  netwib_priv_confwork_arpcache *pca;
  netwib_priv_dll_snmpreq snmpdllreq;
  AsnObjectIdentifier oid0, oid1, oid2;

  /* also check here because it is called directly */
  if (!netwib_global_dll_snmp.isloaded) {
    return(NETWIB_ERR_LONOTSUPPORTED);
  }

  netwib_er(netwib_priv_dll_snmpoid_init(&oid0, 1,3,6,1,2,1,4,22,1,1, -1));
  netwib_er(netwib_priv_dll_snmpoid_init(&oid1, 1,3,6,1,2,1,4,22,1,2, -1));
  netwib_er(netwib_priv_dll_snmpoid_init(&oid2, 1,3,6,1,2,1,4,22,1,3, -1));
  netwib_er(netwib_priv_dll_snmpreq_init(&snmpdllreq, &oid0, &oid1, &oid2,
                                         NULL, NULL));
  netwib_er(netwib_priv_dll_snmpoid_close(&oid0));
  netwib_er(netwib_priv_dll_snmpoid_close(&oid1));
  netwib_er(netwib_priv_dll_snmpoid_close(&oid2));
  netwib_er(netwib_priv_dll_snmpreq_getnext(&snmpdllreq));
  while (snmpdllreq.varBindList.list[0].name.ids[7] == 22
         && snmpdllreq.varBindList.list[0].name.ids[9] == 1) {
    netwib_er(netwib_priv_confwork_arpcache_init(&pca));
    netwib_er(netwib_priv_dll_snmpreq__uint32(&snmpdllreq, 0, &pca->devnum));
    netwib_er(netwib_priv_dll_snmpreq__eth(&snmpdllreq, 1, &pca->eth));
    netwib_er(netwib_priv_dll_snmpreq__ip(&snmpdllreq, 2, &pca->ip));
    /* Check if the entry is valid before adding it. */
    if (pca->eth.b[0] == 0xFF &&
        pca->eth.b[1] == 0xFF &&
        pca->eth.b[2] == 0xFF &&
        pca->eth.b[3] == 0xFF &&
        pca->eth.b[4] == 0xFF &&
        pca->eth.b[5] == 0xFF) {
      /* Ignore broadcast addresses. */
      netwib_er(netwib_priv_confwork_arpcache_close(&pca));
    } else if (pca->eth.b[0] == 0x01 &&
               pca->eth.b[1] == 0x00 &&
               pca->eth.b[2] == 0x5E) {
      /* Ignore multicast addresses. */
      netwib_er(netwib_priv_confwork_arpcache_close(&pca));
    } else if (pca->eth.b[0] == 0x00 &&
               pca->eth.b[1] == 0x00 &&
               pca->eth.b[2] == 0x00 &&
               pca->eth.b[3] == 0x00 &&
               pca->eth.b[4] == 0x00 &&
               pca->eth.b[5] == 0x00) {
      /* Ignore null addresses. */
      netwib_er(netwib_priv_confwork_arpcache_close(&pca));
    } else {
      /* Add the entry. */
      netwib_er(netwib_priv_confwork_arpcache_add(pcw, pca));
    }
    /* Next. */
    netwib_er(netwib_priv_dll_snmpreq_getnext(&snmpdllreq));
  }
  netwib_er(netwib_priv_dll_snmpreq_close(&snmpdllreq));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_inetmib1_routes(netwib_priv_confwork *pcw,
                                                   netwib_ips *plocalips)
{
  netwib_priv_confwork_routes *pcr;
  netwib_priv_dll_snmpreq snmpdllreq;
  AsnObjectIdentifier oid0, oid1, oid2, oid3, oid4;
  netwib_bool yes;

  netwib_er(netwib_priv_dll_snmpoid_init(&oid0, 1,3,6,1,2,1,4,21,1,1, -1));
  netwib_er(netwib_priv_dll_snmpoid_init(&oid1, 1,3,6,1,2,1,4,21,1,2, -1));
  netwib_er(netwib_priv_dll_snmpoid_init(&oid2, 1,3,6,1,2,1,4,21,1,3, -1));
  netwib_er(netwib_priv_dll_snmpoid_init(&oid3, 1,3,6,1,2,1,4,21,1,7, -1));
  netwib_er(netwib_priv_dll_snmpoid_init(&oid4, 1,3,6,1,2,1,4,21,1,11, -1));
  netwib_er(netwib_priv_dll_snmpreq_init(&snmpdllreq, &oid0, &oid1, &oid2,
                                         &oid3, &oid4));
  netwib_er(netwib_priv_dll_snmpoid_close(&oid0));
  netwib_er(netwib_priv_dll_snmpoid_close(&oid1));
  netwib_er(netwib_priv_dll_snmpoid_close(&oid2));
  netwib_er(netwib_priv_dll_snmpoid_close(&oid3));
  netwib_er(netwib_priv_dll_snmpoid_close(&oid4));
  netwib_er(netwib_priv_dll_snmpreq_getnext(&snmpdllreq));
  while (snmpdllreq.varBindList.list[0].name.ids[7] == 21
         && snmpdllreq.varBindList.list[0].name.ids[9] == 1) {
    netwib_er(netwib_priv_confwork_routes_init(&pcr));
    netwib_er(netwib_priv_dll_snmpreq__ip(&snmpdllreq, 0, &pcr->dst));
    netwib_er(netwib_priv_dll_snmpreq__uint32(&snmpdllreq, 1, &pcr->devnum));
    netwib_er(netwib_priv_dll_snmpreq__uint32(&snmpdllreq, 2, &pcr->metric));
    netwib_er(netwib_priv_dll_snmpreq__ip(&snmpdllreq, 3, &pcr->gw));
    netwib_er(netwib_priv_dll_snmpreq__ip(&snmpdllreq, 4, &pcr->mask));
    pcr->gwset = NETWIB_TRUE;
    /* ignore gw set to one of the local addresses */
    netwib_er(netwib_ips_contains_ip(plocalips, &pcr->gw, &yes));
    if (yes) {
      netwib_er(netwib_priv_confwork_routes_close(&pcr));
    } else {
      netwib_er(netwib_priv_confwork_routes_add(pcw, pcr));
    }
    netwib_er(netwib_priv_dll_snmpreq_getnext(&snmpdllreq));
  }
  netwib_er(netwib_priv_dll_snmpreq_close(&snmpdllreq));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_inetmib1(netwib_priv_confwork *pcw)
{
  netwib_ips *plocalips;

  if (!netwib_global_dll_snmp.isloaded) {
    return(NETWIB_ERR_LONOTSUPPORTED);
  }

  netwib_er(netwib_ips_initdefault(&plocalips));

  netwib_er(netwib_priv_conf_inetmib1_devices(pcw));
  netwib_er(netwib_priv_conf_inetmib1_ip(pcw, plocalips));
  netwib_er(netwib_priv_conf_inetmib1_arpcache(pcw));
  netwib_er(netwib_priv_conf_inetmib1_routes(pcw, plocalips));

  netwib_er(netwib_ips_close(&plocalips));

  return(NETWIB_ERR_OK);
}
