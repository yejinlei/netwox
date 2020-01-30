/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confwork_obtain_sys(netwib_priv_confwork *pcw)
{
  netwib_priv_confwork_devices *pcd;
  netwib_priv_confwork_ip *pci;
  netwib_priv_confwork_arpcache *pca;
  netwib_priv_confwork_routes *pcr;

  netwib_er(netwib_priv_confwork_devices_init(&pcd));
  netwib_er(netwib_buf_append_string("lo", &pcd->device));
  pcd->hwtype = NETWIB_DEVICE_HWTYPE_LOOPBACK;
  pcd->mtu = 15000;
  netwib_er(netwib_priv_confwork_devices_add(pcw, pcd));

  netwib_er(netwib_priv_confwork_devices_init(&pcd));
  netwib_er(netwib_buf_append_string("eth0", &pcd->device));
  pcd->hwtype = NETWIB_DEVICE_HWTYPE_ETHER;
  netwib_er(netwib_eth_init_fields(1,2,3,4,5,6, &pcd->eth));
  pcd->mtu = 1500;
  netwib_er(netwib_priv_confwork_devices_add(pcw, pcd));

  netwib_er(netwib_priv_confwork_devices_init(&pcd));
  netwib_er(netwib_buf_append_string("eth0:1", &pcd->device));
  pcd->hwtype = NETWIB_DEVICE_HWTYPE_ETHER;
  netwib_er(netwib_eth_init_fields(1,2,3,4,5,6, &pcd->eth));
  pcd->mtu = 1500;
  netwib_er(netwib_priv_confwork_devices_add(pcw, pcd));

  netwib_er(netwib_priv_confwork_devices_init(&pcd));
  netwib_er(netwib_buf_append_string("eth1", &pcd->device));
  pcd->hwtype = NETWIB_DEVICE_HWTYPE_ETHER;
  netwib_er(netwib_eth_init_fields(1,1,1,1,1,1, &pcd->eth));
  pcd->mtu = 1500;
  netwib_er(netwib_priv_confwork_devices_add(pcw, pcd));

  netwib_er(netwib_priv_confwork_devices_init(&pcd));
  netwib_er(netwib_buf_append_string("ppp0", &pcd->device));
  pcd->hwtype = NETWIB_DEVICE_HWTYPE_PPP;
  pcd->mtu = 1500;
  netwib_er(netwib_priv_confwork_devices_add(pcw, pcd));

  netwib_er(netwib_priv_confwork_devices_init(&pcd));
  netwib_er(netwib_buf_append_string("eth2", &pcd->device));
  pcd->hwtype = NETWIB_DEVICE_HWTYPE_ETHER;
  netwib_er(netwib_eth_init_fields(0xae,0xae,0xae,0xae,0xae,0xae, &pcd->eth));
  pcd->mtu = 1500;
  netwib_er(netwib_priv_confwork_devices_add(pcw, pcd));

  netwib_er(netwib_priv_confwork_devices_init(&pcd));
  netwib_er(netwib_buf_append_string("ppp1", &pcd->device));
  pcd->hwtype = NETWIB_DEVICE_HWTYPE_PPP;
  pcd->mtu = 1500;
  netwib_er(netwib_priv_confwork_devices_add(pcw, pcd));

  netwib_er(netwib_priv_confwork_devices_init(&pcd));
  netwib_er(netwib_buf_append_string("verylonglonglongdevicename", &pcd->device));
  pcd->hwtype = NETWIB_DEVICE_HWTYPE_ETHER;
  netwib_er(netwib_eth_init_fields(2,2,2,2,2,2, &pcd->eth));
  pcd->mtu = 1500;
  netwib_er(netwib_priv_confwork_devices_add(pcw, pcd));

  netwib_er(netwib_priv_confwork_devices_init(&pcd));
  netwib_er(netwib_buf_append_string("eth0", &pcd->device));
  pcd->hwtype = NETWIB_DEVICE_HWTYPE_UNKNOWN;
  netwib_er(netwib_eth_init_fields(0,0,0,0,0,0, &pcd->eth));
  pcd->mtu = 0;
  netwib_er(netwib_priv_confwork_devices_add(pcw, pcd));

  /**/
  netwib_er(netwib_priv_confwork_ip_init(&pci));
  netwib_er(netwib_buf_append_string("lo", &pci->device));
  netwib_er(netwib_ip_init_ip4(0x7F000001, &pci->ip));
  netwib_er(netwib_ip_init_ip4(0xFF000000u, &pci->mask));
  pci->ispointtopoint = NETWIB_FALSE;
  netwib_er(netwib_priv_confwork_ip_add(pcw, pci));

  netwib_er(netwib_priv_confwork_ip_init(&pci));
  netwib_er(netwib_buf_append_string("eth0", &pci->device));
  netwib_er(netwib_ip_init_ip4(0x0A010101, &pci->ip));
  netwib_er(netwib_ip_init_ip4(0xFF000000u, &pci->mask));
  pci->ispointtopoint = NETWIB_FALSE;
  netwib_er(netwib_priv_confwork_ip_add(pcw, pci));

  netwib_er(netwib_priv_confwork_ip_init(&pci));
  netwib_er(netwib_buf_append_string("eth0:1", &pci->device));
  netwib_er(netwib_ip_init_ip4(0xC0A80101u, &pci->ip));
  netwib_er(netwib_ip_init_ip4(0xFFFFFF00u, &pci->mask));
  pci->ispointtopoint = NETWIB_FALSE;
  netwib_er(netwib_priv_confwork_ip_add(pcw, pci));

  netwib_er(netwib_priv_confwork_ip_init(&pci));
  netwib_er(netwib_buf_append_string("eth1", &pci->device));
  netwib_er(netwib_ip_init_ip4(0xC0A80501u, &pci->ip));
  netwib_er(netwib_ip_init_ip4(0xFFFFFF00u, &pci->mask));
  pci->ispointtopoint = NETWIB_FALSE;
  netwib_er(netwib_priv_confwork_ip_add(pcw, pci));

  netwib_er(netwib_priv_confwork_ip_init(&pci));
  netwib_er(netwib_buf_append_string("ppp0", &pci->device));
  netwib_er(netwib_ip_init_ip4(0x0B010101, &pci->ip));
  netwib_er(netwib_ip_init_ip4(0xFFFF0000u, &pci->mask));
  pci->ispointtopoint = NETWIB_TRUE;
  netwib_er(netwib_ip_init_ip4(0x0B010102, &pci->pointtopointip));
  netwib_er(netwib_priv_confwork_ip_add(pcw, pci));

  netwib_er(netwib_priv_confwork_ip_init(&pci));
  netwib_er(netwib_buf_append_string("eth2", &pci->device));
  netwib_er(netwib_ip_init_ip6_fields(0xfec00000u,0x1234,0,1, &pci->ip));
  pci->prefix = 64;
  pci->ispointtopoint = NETWIB_FALSE;
  netwib_er(netwib_priv_confwork_ip_add(pcw, pci));

  netwib_er(netwib_priv_confwork_ip_init(&pci));
  netwib_er(netwib_buf_append_string("ppp1", &pci->device));
  netwib_er(netwib_ip_init_ip6_fields(0xfe80000, 0, 0, 0x9999, &pci->ip));
  pci->prefix = 10;
  pci->ispointtopoint = NETWIB_TRUE;
  netwib_er(netwib_ip_init_ip6_fields(0xfe80000, 0, 0, 0x8888,
                                    &pci->pointtopointip));
  netwib_er(netwib_priv_confwork_ip_add(pcw, pci));

  netwib_er(netwib_priv_confwork_ip_init(&pci));
  netwib_er(netwib_buf_append_string("verylonglonglongdevicename", &pci->device));
  netwib_er(netwib_ip_init_ip6_fields(0x12345678, 0x12345678, 0x12345678,
                                    0x12345678, &pci->ip));
  pci->prefix = 20;
  pci->ispointtopoint = NETWIB_FALSE;
  netwib_er(netwib_priv_confwork_ip_add(pcw, pci));

  netwib_er(netwib_priv_confwork_ip_init(&pci));
  netwib_er(netwib_buf_append_string("eth0", &pci->device));
  netwib_er(netwib_ip_init_ip4(0x0A010101, &pci->ip));
  netwib_er(netwib_ip_init_ip4(0xFF000000u, &pci->mask));
  pci->ispointtopoint = NETWIB_FALSE;
  netwib_er(netwib_priv_confwork_ip_add(pcw, pci));

  netwib_er(netwib_priv_confwork_ip_init(&pci));
  netwib_er(netwib_buf_append_string("ett", &pci->device));
  netwib_er(netwib_ip_init_ip4(0x0A0B0C0D, &pci->ip));
  netwib_er(netwib_ip_init_ip4(0xFF000000u, &pci->mask));
  pci->ispointtopoint = NETWIB_FALSE;
  netwib_er(netwib_priv_confwork_ip_add(pcw, pci));

  /**/
  netwib_er(netwib_priv_confwork_arpcache_init(&pca));
  netwib_er(netwib_buf_append_string("eth0", &pca->device));
  netwib_er(netwib_eth_init_fields(1,0,0,3,0,0, &pca->eth));
  netwib_er(netwib_ip_init_ip4(0x0A020202, &pca->ip));
  netwib_er(netwib_priv_confwork_arpcache_add(pcw, pca));

  netwib_er(netwib_priv_confwork_arpcache_init(&pca));
  netwib_er(netwib_buf_append_string("eth1", &pca->device));
  netwib_er(netwib_eth_init_fields(1,0,2,3,0,0, &pca->eth));
  netwib_er(netwib_ip_init_ip4(0xC0A80502u, &pca->ip));
  netwib_er(netwib_priv_confwork_arpcache_add(pcw, pca));

  netwib_er(netwib_priv_confwork_arpcache_init(&pca));
  netwib_er(netwib_buf_append_string("eth2", &pca->device));
  netwib_er(netwib_eth_init_fields(1,0,2,3,0,0, &pca->eth));
  netwib_er(netwib_ip_init_ip6_fields(0xfec00000u, 0x1234, 0, 2, &pca->ip));
  netwib_er(netwib_priv_confwork_arpcache_add(pcw, pca));

  netwib_er(netwib_priv_confwork_arpcache_init(&pca));
  netwib_er(netwib_buf_append_string("verylonglonglongdevicename", &pca->device));
  netwib_er(netwib_eth_init_fields(1,0,3,3,0,0, &pca->eth));
  netwib_er(netwib_ip_init_ip6_fields(0x12345678, 0x12345678, 0x12345678,
                                    0x12345678, &pca->ip));
  netwib_er(netwib_priv_confwork_arpcache_add(pcw, pca));

  netwib_er(netwib_priv_confwork_arpcache_init(&pca));
  netwib_er(netwib_buf_append_string("", &pca->device));
  netwib_er(netwib_eth_init_fields(1,0,0,3,0,0, &pca->eth));
  netwib_er(netwib_ip_init_ip4(0x0A020202, &pca->ip));
  netwib_er(netwib_priv_confwork_arpcache_add(pcw, pca));

  netwib_er(netwib_priv_confwork_arpcache_init(&pca));
  netwib_er(netwib_buf_append_string("", &pca->device));
  netwib_er(netwib_eth_init_fields(0,1,2,3,0,0, &pca->eth));
  netwib_er(netwib_ip_init_ip4(0x0C0C0C0C, &pca->ip));
  netwib_er(netwib_priv_confwork_arpcache_add(pcw, pca));

  /**/
  netwib_er(netwib_priv_confwork_routes_init(&pcr));
  netwib_er(netwib_buf_append_string("eth1", &pcr->device));
  netwib_er(netwib_ip_init_ip4(0xC0A80501u, &pcr->dst));
  netwib_er(netwib_ip_init_ip4(0xFFFFFFFFu, &pcr->mask));
  pcr->srcset = NETWIB_FALSE;
  pcr->gwset = NETWIB_FALSE;
  pcr->metric = 0;
  netwib_er(netwib_priv_confwork_routes_add(pcw, pcr));

  netwib_er(netwib_priv_confwork_routes_init(&pcr));
  netwib_er(netwib_buf_append_string("eth1", &pcr->device));
  netwib_er(netwib_ip_init_ip4(0xC0A80500u, &pcr->dst));
  netwib_er(netwib_ip_init_ip4(0xFFFFFF00u, &pcr->mask));
  pcr->srcset = NETWIB_FALSE;
  pcr->gwset = NETWIB_FALSE;
  pcr->metric = 15;
  netwib_er(netwib_priv_confwork_routes_add(pcw, pcr));

  netwib_er(netwib_priv_confwork_routes_init(&pcr));
  netwib_er(netwib_buf_append_string("eth1", &pcr->device));
  netwib_er(netwib_ip_init_ip4(0x22345600, &pcr->dst));
  netwib_er(netwib_ip_init_ip4(0xFFFFFF00u, &pcr->mask));
  pcr->srcset = NETWIB_TRUE;
  netwib_er(netwib_ip_init_ip4(0xC0A80501u, &pcr->src));
  pcr->gwset = NETWIB_TRUE;
  netwib_er(netwib_ip_init_ip4(0xC0A805FEu, &pcr->gw));
  pcr->metric = 1;
  netwib_er(netwib_priv_confwork_routes_add(pcw, pcr));

  netwib_er(netwib_priv_confwork_routes_init(&pcr));
  netwib_er(netwib_buf_append_string("eth2", &pcr->device));
  netwib_er(netwib_ip_init_ip6_fields(0xfec00000u,0x1234,0,1, &pcr->dst));
  pcr->prefix = 128;
  pcr->srcset = NETWIB_FALSE;
  pcr->gwset = NETWIB_FALSE;
  pcr->metric = 5;
  netwib_er(netwib_priv_confwork_routes_add(pcw, pcr));

  netwib_er(netwib_priv_confwork_routes_init(&pcr));
  netwib_er(netwib_buf_append_string("eth2", &pcr->device));
  netwib_er(netwib_ip_init_ip6_fields(0xfec00000u,0x1234,0,0, &pcr->dst));
  pcr->prefix = 64;
  pcr->srcset = NETWIB_TRUE;
  netwib_er(netwib_ip_init_ip6_fields(0xfec00000u,0x1234,0,1, &pcr->src));
  pcr->gwset = NETWIB_TRUE;
  netwib_er(netwib_ip_init_ip6_fields(0xfec00000u,0x1234,0,0xFE, &pcr->gw));
  pcr->metric = 0;
  netwib_er(netwib_priv_confwork_routes_add(pcw, pcr));

  netwib_er(netwib_priv_confwork_routes_init(&pcr));
  netwib_er(netwib_buf_append_string("verylonglonglongdevicename", &pcr->device));
  netwib_er(netwib_ip_init_ip6_fields(0x12345678, 0x12345678, 0x12345678,
                                      0x12345678, &pcr->dst));
  pcr->prefix = 64;
  pcr->srcset = NETWIB_TRUE;
  netwib_er(netwib_ip_init_ip6_fields(0x12345678, 0x12345678, 0x12345678,
                                      0x12345678, &pcr->src));
  pcr->gwset = NETWIB_TRUE;
  netwib_er(netwib_ip_init_ip6_fields(0x12345678, 0x12345678, 0x12345678,
                                      0x12345678, &pcr->gw));
  pcr->metric = 0;
  netwib_er(netwib_priv_confwork_routes_add(pcw, pcr));

  netwib_er(netwib_priv_confwork_routes_init(&pcr));
  netwib_er(netwib_buf_append_string("ppp0", &pcr->device));
  netwib_er(netwib_ip_init_ip4(0, &pcr->dst));
  netwib_er(netwib_ip_init_ip4(0, &pcr->mask));
  pcr->srcset = NETWIB_FALSE;
  pcr->gwset = NETWIB_FALSE;
  pcr->metric = 0;
  netwib_er(netwib_priv_confwork_routes_add(pcw, pcr));

  netwib_er(netwib_priv_confwork_routes_init(&pcr));
  netwib_er(netwib_buf_append_string("eth1", &pcr->device));
  netwib_er(netwib_ip_init_ip4(0, &pcr->dst));
  netwib_er(netwib_ip_init_ip4(0, &pcr->mask));
  pcr->srcset = NETWIB_FALSE;
  pcr->gwset = NETWIB_TRUE;
  netwib_er(netwib_ip_init_ip4(0xC0A805FDu, &pcr->gw));
  pcr->metric = 0;
  netwib_er(netwib_priv_confwork_routes_add(pcw, pcr));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_obtain_arpcache(netwib_priv_confwork *pcw)
{
  netwib_priv_confwork_arpcache *pca;

  netwib_er(netwib_priv_confwork_arpcache_init(&pca));
  netwib_er(netwib_buf_append_string("eth0", &pca->device));
  netwib_er(netwib_eth_init_fields(1,0,0,3,0,0, &pca->eth));
  netwib_er(netwib_ip_init_ip4(0x0A020202, &pca->ip));
  netwib_er(netwib_priv_confwork_arpcache_add(pcw, pca));

  netwib_er(netwib_priv_confwork_arpcache_init(&pca));
  netwib_er(netwib_buf_append_string("eth1", &pca->device));
  netwib_er(netwib_eth_init_fields(1,0,2,3,0,0, &pca->eth));
  netwib_er(netwib_ip_init_ip4(0xC0A80502u, &pca->ip));
  netwib_er(netwib_priv_confwork_arpcache_add(pcw, pca));

  return(NETWIB_ERR_OK);
}
