/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_pcapfindalldevs_devhwtype(netwib_string dev,
                                                             netwib_device_hwtype *phwtype)
{
  netwib_string pc;

  if (!netwib_c_strncmp(dev, "PPPMAC", 6)) {
    *phwtype = NETWIB_DEVICE_HWTYPE_PPP;
    return(NETWIB_ERR_OK);
  }

  pc = netwib_c_strstr(dev, "NdisWan");
  if (pc != NULL) {
    *phwtype = NETWIB_DEVICE_HWTYPE_PPP;
    return(NETWIB_ERR_OK);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_pcapfindalldevs_hwtype(netwib_string dev,
                                                          netwib_device_hwtype *phwtype,
                                                          netwib_eth *peth)
{
  LPADAPTER padapter;
  netwib_priv_NetType nettype;
  BOOL b;
  netwib_err ret;

  *phwtype = NETWIB_DEVICE_HWTYPE_UNKNOWN;
  if (!netwib_global_dll_packet.isloaded) {
    return(NETWIB_ERR_OK);
  }
  ret = NETWIB_ERR_OK;

  netwib_er(netwib_priv_cs_PacketOpenAdapter(dev, &padapter));

  b = netwib_global_dll_packet.PacketGetNetType(padapter, &nettype);
  netwib_bg(b, NETWIB_ERR_FUPACKETGETNETTYPE);

  /* decide using linktype */
  if (netwib_global_dll_packet.versionmajor == 3 &&
    netwib_global_dll_packet.versionminor == 0) {
    if (nettype.v30.LinkType == NdisMedium802_3) {
      if (nettype.v30.LinkSpeed >= 1000000) {
        *phwtype = NETWIB_DEVICE_HWTYPE_ETHER;
      }
    }
  } else {
    if (nettype.v31.LinkType == NdisMedium802_3) {
      if (nettype.v31.LinkSpeed >= 1000000) {
        *phwtype = NETWIB_DEVICE_HWTYPE_ETHER;
      }
    }
  }

  /* also try with device name */
  if (*phwtype == NETWIB_DEVICE_HWTYPE_UNKNOWN) {
    netwib_eg(netwib_priv_conf_pcapfindalldevs_devhwtype(dev, phwtype));
  }

  /* if Ethernet, obtain its address */
  if (*phwtype == NETWIB_DEVICE_HWTYPE_ETHER) {
    PPACKET_OID_DATA OidData;
    netwib_eg(netwib_ptr_malloc(sizeof(PACKET_OID_DATA) + 128,
                                (netwib_ptr*)&OidData));
    OidData->Oid = OID_802_3_CURRENT_ADDRESS;
    OidData->Length = 128;
    b = netwib_global_dll_packet.PacketRequest(padapter, FALSE, OidData);
    if (b) {
      /* can not check size==NETWIB_ETH_LEN because Oid->Length is not set
         by PaquetRequest under Windows 9x */
      netwib_c_memcpy(peth->b, OidData->Data, NETWIB_ETH_LEN);
    } else {
      return(NETWIB_ERR_FUPACKETREQUEST);
    }
    netwib_er(netwib_ptr_free((netwib_ptr*)&OidData));
  }

netwib_gotolabel:
  netwib_er(netwib_priv_cs_PacketCloseAdapter(padapter));
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_pcapfindalldevs(netwib_priv_confwork *pcw)
{
  netwib_priv_confwork_devices *pcd;
  netwib_priv_confwork_ip *pci;
  pcap_if_t *alldevs;
  pcap_if_t *pcurdev;
  pcap_addr_t *pcurip;
  char errbuf[PCAP_ERRBUF_SIZE];
  int reti;
  netwib_err ret;

  if (!netwib_global_dll_winpcap.isloaded) {
    /* set nothing */
    return(NETWIB_ERR_OK);
  }

  reti = netwib_global_dll_winpcap.pcap_findalldevs(&alldevs, errbuf);
  netwib_ir(reti, NETWIB_ERR_FUPCAPFINDALLDEVS);

  for (pcurdev = alldevs; pcurdev != NULL; pcurdev = pcurdev->next) {
    /* set device */
    netwib_er(netwib_priv_confwork_devices_init(&pcd));
    netwib_er(netwib_buf_append_string(pcurdev->name, &pcd->device));
    if (pcurdev->flags & PCAP_IF_LOOPBACK) {
      pcd->hwtype = NETWIB_DEVICE_HWTYPE_LOOPBACK;
    } else {
      ret = netwib_priv_conf_pcapfindalldevs_hwtype(pcurdev->name,
                                                    &pcd->hwtype, &pcd->eth);
      if (ret != NETWIB_ERR_OK) {
        pcd->hwtype = NETWIB_DEVICE_HWTYPE_UNKNOWN;
      }
    }
    netwib_er(netwib_priv_confwork_devices_add(pcw, pcd));
    /* set ip */
    for (pcurip = pcurdev->addresses; pcurip != NULL; pcurip = pcurip->next) {
      if (pcurip->addr == NULL) {
        continue;
      }
      netwib_er(netwib_priv_confwork_ip_init(&pci));
      netwib_eg(netwib_buf_append_buf(&pcd->device, &pci->device));
      netwib_eg(netwib_priv_sa_ip_init_sa((netwib_priv_sockaddr_unalign*)pcurip->addr, &pci->ip));
      if (pci->ip.iptype == NETWIB_IPTYPE_IP4) {
        if (pci->ip.ipvalue.ip4 == 0) {
          netwib_goto(NETWIB_ERR_OK);
        }
      }
      if (pcurip->netmask != NULL) {
        netwib_eg(netwib_priv_sa_ip_init_sa((netwib_priv_sockaddr_unalign*)pcurip->netmask, &pci->mask));
      }
      if (pcurip->dstaddr != NULL) {
        netwib_eg(netwib_priv_sa_ip_init_sa((netwib_priv_sockaddr_unalign*)pcurip->dstaddr, &pci->pointtopointip));
        pci->ispointtopoint = NETWIB_TRUE;
      }
      netwib_eg(netwib_priv_confwork_ip_add(pcw, pci));
      continue;
netwib_gotolabel:
      netwib_er(netwib_priv_confwork_ip_close(&pci));
    }
  }

  netwib_global_dll_winpcap.pcap_freealldevs(alldevs);

  return(NETWIB_ERR_OK);
}
