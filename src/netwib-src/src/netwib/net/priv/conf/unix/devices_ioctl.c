/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
/***
  Used by :
    Linux
    Solaris
 ***/

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_devices_ioctl_hw(int fd,
                                                    netwib_conststring device,
                                                    netwib_device_hwtype *phwtype,
                                                    netwib_eth *peth)
#if defined NETWIBDEF_SYSNAME_Linux
{
  struct ifreq ifr;
  int reti;

  netwib_c_memset(&ifr, 0, sizeof(ifr));
  netwib_c_strcpy(ifr.ifr_name, device);
  ifr.ifr_addr.sa_family = AF_INET;
  reti = ioctl(fd, SIOCGIFHWADDR, (char *)&ifr);
  if (reti < 0) {
    return(NETWIB_ERR_FUIOCTL);
  }

  switch(ifr.ifr_hwaddr.sa_family) {
  case ARPHRD_ETHER :
    *phwtype = NETWIB_DEVICE_HWTYPE_ETHER;
    netwib_c_memcpy(peth->b, &ifr.ifr_hwaddr.sa_data, NETWIB_ETH_LEN);
    break;
  case ARPHRD_LOOPBACK :
    *phwtype = NETWIB_DEVICE_HWTYPE_LOOPBACK;
    break;
  case ARPHRD_PPP :
    *phwtype = NETWIB_DEVICE_HWTYPE_PPP;
    break;
  default :
    *phwtype = NETWIB_DEVICE_HWTYPE_UNKNOWN;
    break;
  }

  return(NETWIB_ERR_OK);
}
#elif defined NETWIBDEF_SYSNAME_Solaris
{
  netwib_ptr buf;
  netwib_data pbufeth;
  dl_info_req_t ireq;
  dl_phys_addr_req_t pareq;
  int fd2;
  netwib_err ret;

  if (!netwib_c_strcmp(device, "lo0")) {
    *phwtype = NETWIB_DEVICE_HWTYPE_LOOPBACK;
    return(NETWIB_ERR_OK);
  }

  ret = netwib_priv_dlpi_open(device, &fd2);
  if (ret != NETWIB_ERR_OK) {
    /* if we are not root, we cannot retrieve device type and
       address. */
    *phwtype = NETWIB_DEVICE_HWTYPE_UNKNOWN;
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_ptr_malloc(2048, &buf));
  ret = NETWIB_ERR_OK;

  /* ask type */
  ireq.dl_primitive = DL_INFO_REQ;
  netwib_eg(netwib_priv_dlpi_putmsg(fd2, &ireq, sizeof(ireq), 0));
  netwib_eg(netwib_priv_dlpi_getmsg(fd2, DL_INFO_ACK_SIZE, buf));
  switch((int)((dl_info_ack_t *)buf)->dl_mac_type) {
  case DL_ETHER :
    *phwtype = NETWIB_DEVICE_HWTYPE_ETHER;
    break;
  default :
    *phwtype = NETWIB_DEVICE_HWTYPE_UNKNOWN;
    break;
  }

  /* obtain Ethernet address */
  if (*phwtype == NETWIB_DEVICE_HWTYPE_ETHER) {
    pareq.dl_primitive = DL_PHYS_ADDR_REQ;
    pareq.dl_addr_type = DL_CURR_PHYS_ADDR;
    netwib_eg(netwib_priv_dlpi_putmsg(fd2, &pareq, sizeof(pareq), 0));
    netwib_eg(netwib_priv_dlpi_getmsg(fd2, DL_PHYS_ADDR_ACK_SIZE, buf));
    pbufeth = (netwib_data)buf + ((dl_phys_addr_ack_t*)buf)->dl_addr_offset;
    if (pbufeth[0] == 0 && pbufeth[1] == 0 && pbufeth[2] == 0 &&
        pbufeth[3] == 0 && pbufeth[4] == 0 && pbufeth[5] == 0) {
      *phwtype = NETWIB_DEVICE_HWTYPE_UNKNOWN;
    } else {
      netwib_c_memcpy(peth->b, pbufeth, NETWIB_ETH_LEN);
    }
  }

 netwib_gotolabel:
  close(fd2);
  netwib_er(netwib_ptr_free(&buf));
  fd = fd; /* for compiler warning */
  return(ret);
}
#endif

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_devices_ioctl_dev(netwib_priv_confwork *pcw,
                                                     int fd,
                                                     netwib_conststring device)
{
  netwib_priv_confwork_devices *pcd;
  netwib_bool pcdset;
  netwib_err ret;
  struct ifreq ifr;
  int reti;

  netwib_er(netwib_priv_confwork_devices_init(&pcd));

  /* prepare for error handling */
  pcdset = NETWIB_FALSE;
  ret = NETWIB_ERR_OK;

  /* set device */
  netwib_eg(netwib_buf_append_string(device, &pcd->device));
  pcdset = NETWIB_TRUE;

  /* get hwtype */
  netwib_eg(netwib_priv_conf_devices_ioctl_hw(fd, device, &pcd->hwtype,
                                              &pcd->eth));

  /* get mtu */
  netwib_c_memset(&ifr, 0, sizeof(ifr));
  netwib_c_strcpy(ifr.ifr_name, device);
  ifr.ifr_addr.sa_family = AF_INET;
  reti = ioctl(fd, SIOCGIFMTU, (char *)&ifr);
  netwib_ig(reti, NETWIB_ERR_FUIOCTL);
#if defined NETWIBDEF_SYSNAME_Solaris
  pcd->mtu = ifr.ifr_metric;
#else
  pcd->mtu = ifr.ifr_mtu;
#endif

  /* save value */
  netwib_eg(netwib_priv_confwork_devices_add(pcw, pcd));
  pcdset = NETWIB_FALSE;

  /* clean and leave */
 netwib_gotolabel:
  if (pcdset) {
    netwib_er(netwib_priv_confwork_devices_close(&pcd));
  }
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_devices_ioctl(netwib_priv_confwork *pcw)
{
  netwib_priv_confwork_ip *pci;
  netwib_uint32 numif, i;
  netwib_bool pciset;
  netwib_err ret;
  struct ifconf ifc;
  struct ifreq ireqbuf[100];
  struct ifreq ifr;
  int fd, reti;

  /* prepare for error handling */
  pciset = NETWIB_FALSE;
  ret = NETWIB_ERR_OK;

  /* work on this socket */
  fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
    return(NETWIB_ERR_FUSOCKET);
  }

#if defined NETWIBDEF_SYSNAME_Linux
 #if defined ifr_ifindex
  /* first obtain all devices (a device with IP address 0.0.0.0 is not
     retrieved by SIOCGIFCONF). However, we still have to do the job in
     SIOCGIFCONF loop, because aliases are not retrieved here.
     This only works for kernels >= 2.1.50. For previous versions, we
     could read /proc/net/dev (not implemented in netwib). */
  for (i = 1; ; i++) {
    netwib_c_memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_ifindex = i;
    reti = ioctl(fd, SIOCGIFNAME, (char *)&ifr);
    if (reti < 0) break;
    netwib_eg(netwib_priv_conf_devices_ioctl_dev(pcw, fd, ifr.ifr_name));
  }
 #endif
#endif

  /* store config in buffer */
  ifc.ifc_len = sizeof(ireqbuf);
  ifc.ifc_ifcu.ifcu_req = ireqbuf;
  netwib_c_memset((char *)ireqbuf, 0, sizeof(ireqbuf));
  reti = ioctl(fd, SIOCGIFCONF, (char *)&ifc);
  netwib_ig(reti, NETWIB_ERR_FUIOCTL);

  /* loop for each device */
  numif = ifc.ifc_len / sizeof(struct ifreq);
  for (i = 0; i < numif; i++) {
    /** DEVICE **/
    netwib_eg(netwib_priv_conf_devices_ioctl_dev(pcw, fd,
                                                 ireqbuf[i].ifr_name));
    /** IP **/
    /* get flags */
    netwib_c_memset(&ifr, 0, sizeof(ifr));
    netwib_c_strcpy(ifr.ifr_name, ireqbuf[i].ifr_name);
#if defined NETWIBDEF_SYSNAME_Solaris
    /* Solaris 8 does not support alias (elx0:1) for SIOCGIFFLAGS */
    {
      netwib_string pc;
      pc = netwib_c_strchr(ifr.ifr_name, ':');
      if (pc != NULL) {
        *pc = '\0';
      }
    }
#endif
    ifr.ifr_addr.sa_family = AF_INET;
    reti = ioctl(fd, SIOCGIFFLAGS, (char *)&ifr);
    netwib_ig(reti, NETWIB_ERR_FUIOCTL);
    if ( ! (ifr.ifr_flags & IFF_UP) ) {
      continue;
    }
    netwib_eg(netwib_priv_confwork_ip_init(&pci));
    pciset = NETWIB_TRUE;
    /* get ptp */
    if (ifr.ifr_flags & IFF_POINTOPOINT) {
      pci->ispointtopoint = NETWIB_TRUE;
    }
    /* get device */
    netwib_eg(netwib_buf_append_string(ireqbuf[i].ifr_name, &pci->device));
    /* get ip */
    netwib_c_memset(&ifr, 0, sizeof(ifr));
    netwib_c_strcpy(ifr.ifr_name, ireqbuf[i].ifr_name);
    ifr.ifr_addr.sa_family = AF_INET;
    reti = ioctl(fd, SIOCGIFADDR, (char *)&ifr);
    netwib_ig(reti, NETWIB_ERR_FUIOCTL);
    netwib_eg(netwib_priv_sa_ip_init_sa((const netwib_priv_sockaddr_unalign *)&ifr.ifr_addr, &pci->ip));
    /* get mask */
    netwib_c_memset(&ifr, 0, sizeof(ifr));
    netwib_c_strcpy(ifr.ifr_name, ireqbuf[i].ifr_name);
    ifr.ifr_addr.sa_family = AF_INET;
    reti = ioctl(fd, SIOCGIFNETMASK, (char *)&ifr);
    netwib_ig(reti, NETWIB_ERR_FUIOCTL);
    netwib_eg(netwib_priv_sa_ip_init_sa((const netwib_priv_sockaddr_unalign *)&ifr.ifr_addr, &pci->mask));
    /* get ptpip */
    if (pci->ispointtopoint) {
      netwib_c_memset(&ifr, 0, sizeof(ifr));
      netwib_c_strcpy(ifr.ifr_name, ireqbuf[i].ifr_name);
      ifr.ifr_addr.sa_family = AF_INET;
      reti = ioctl(fd, SIOCGIFDSTADDR, (char *)&ifr);
      netwib_ig(reti, NETWIB_ERR_FUIOCTL);
      netwib_eg(netwib_priv_sa_ip_init_sa((const netwib_priv_sockaddr_unalign *)&ifr.ifr_addr, &pci->pointtopointip));
    }
    /* save value */
    netwib_eg(netwib_priv_confwork_ip_add(pcw, pci));
    pciset = NETWIB_FALSE;
  }

  /* clean and leave */
 netwib_gotolabel:
  close(fd);
  if (pciset) {
    netwib_er(netwib_priv_confwork_ip_close(&pci));
  }
  return(ret);
}

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Solaris
static netwib_err netwib_priv_conf_devices_ioctl6(netwib_priv_confwork *pcw);
#if NETWIBDEF_HAVEVAR_SIOCGLIFCONF == 1
/* we have to redefine this struct because system ones only work
   on 64 bit platforms... This is an adapted copy of net/if.h.
   Anyone knows why Sun made this stupid choice ? */
struct  netwib_lifreq {
  char    lifr_name[LIFNAMSIZ];           /* if name, e.g. "en0" */
  union {
    int     lifru_addrlen;          /* for subnet/token etc */
    uint_t  lifru_ppa;              /* SIOCSLIFNAME */
  } lifr_lifru1;
  /* padding because ia32 "long long"s are only 4-byte aligned. */
  int     lifr_pad0;
  union {
    struct  sockaddr_storage lifru_addr;
    struct  sockaddr_storage lifru_dstaddr;
    struct  sockaddr_storage lifru_broadaddr;
    struct  sockaddr_storage lifru_token;   /* With lifr_addrlen */
    struct  sockaddr_storage lifru_subnet;  /* With lifr_addrlen */
    int     lifru_index;            /* interface index */
    uint32_t lifru_flags1;           /* Flags for SIOC?LIFFLAGS */
    uint32_t lifru_flags2;           /* Flags for SIOC?LIFFLAGS */
    int     lifru_metric;
    uint_t  lifru_mtu;
    char    lifru_data[1];          /* interface dependent data */
    char    lifru_enaddr[6];
    int     lif_muxid[2];           /* mux id's for arp and ip */
    struct lif_nd_req       lifru_nd_req;
    struct lif_ifinfo_req   lifru_ifinfo_req;
  } lifr_lifru;
};
struct  netwib_lifconf {
  sa_family_t     lifc_family;
  int             lifc_flags;     /* request specific interfaces */
  int             lifc_len;       /* size of associated buffer */
  union {
    caddr_t lifcu_buf;
    struct  netwib_lifreq *lifcu_req;
  } lifc_lifcu;
};
#define NETWIB_SIOCGLIFADDR    _IOWR('i', 113, struct netwib_lifreq)
#define NETWIB_SIOCGLIFDSTADDR _IOWR('i', 115, struct netwib_lifreq)
#define NETWIB_SIOCGLIFFLAGS   _IOWR('i', 117, struct netwib_lifreq)
#define NETWIB_SIOCGLIFMTU     _IOWR('i', 122, struct netwib_lifreq)
#define NETWIB_SIOCGLIFNETMASK _IOWR('i', 125, struct netwib_lifreq)
#define NETWIB_SIOCGLIFSUBNET  _IOWR('i', 138, struct netwib_lifreq)
/* end of copy */
static netwib_err netwib_priv_conf_devices_ioctl6(netwib_priv_confwork *pcw)
{
  netwib_priv_confwork_devices *pcd;
  netwib_priv_confwork_ip *pci;
  netwib_uint32 numif, i;
  netwib_bool pcdset, pciset;
  netwib_err ret;
  struct netwib_lifconf lifc;
  struct netwib_lifreq lireqbuf[100];
  struct netwib_lifreq lifr;
  netwib_priv_sockaddr_unalign *psa;
  int fd, reti;

  /* prepare for error handling */
  pcdset = NETWIB_FALSE;
  pciset = NETWIB_FALSE;
  ret = NETWIB_ERR_OK;

  /* work on this socket */
  fd = socket(AF_INET6, SOCK_DGRAM, 0);
  if (fd < 0) {
    return(NETWIB_ERR_FUSOCKET);
  }

  /* store config in buffer */
  lifc.lifc_len = sizeof(lireqbuf);
  lifc.lifc_lifcu.lifcu_req = lireqbuf;
  lifc.lifc_family = AF_INET6;
  netwib_c_memset((char *)lireqbuf, 0, sizeof(lireqbuf));
  reti = ioctl(fd, SIOCGLIFCONF, (char *)&lifc);
  netwib_ig(reti, NETWIB_ERR_FUIOCTL);

  /* loop for each device */
  numif = lifc.lifc_len / sizeof(struct netwib_lifreq);
  for (i = 0; i < numif; i++) {
    /** DEVICE **/
    netwib_eg(netwib_priv_confwork_devices_init(&pcd));
    pcdset = NETWIB_TRUE;
    /* get device */
    netwib_eg(netwib_buf_append_string(lireqbuf[i].lifr_name, &pcd->device));
    /* get hwtype */
    netwib_eg(netwib_priv_conf_devices_ioctl_hw(fd, lireqbuf[i].lifr_name,
                                                &pcd->hwtype, &pcd->eth));
    /* get mtu */
    netwib_c_memset(&lifr, 0, sizeof(lifr));
    netwib_c_strcpy(lifr.lifr_name, lireqbuf[i].lifr_name);
    lifr.lifr_lifru.lifru_addr.ss_family = AF_INET6;
    reti = ioctl(fd, NETWIB_SIOCGLIFMTU, (char *)&lifr);
    netwib_ig(reti, NETWIB_ERR_FUIOCTL);
    pcd->mtu = lifr.lifr_lifru.lifru_mtu;
    /* save value */
    netwib_eg(netwib_priv_confwork_devices_add(pcw, pcd));
    pcdset = NETWIB_FALSE;
    /** IP **/
    /* get flags */
    netwib_c_memset(&lifr, 0, sizeof(lifr));
    netwib_c_strcpy(lifr.lifr_name, lireqbuf[i].lifr_name);
#if defined NETWIBDEF_SYSNAME_Solaris
    /* Solaris 8 does not support alias (elx0:1) for SIOCGIFFLAGS */
    {
      netwib_string pc;
      pc = netwib_c_strchr(lifr.lifr_name, ':');
      if (pc != NULL) {
        *pc = '\0';
      }
    }
#endif
    lifr.lifr_lifru.lifru_addr.ss_family = AF_INET6;
    reti = ioctl(fd, NETWIB_SIOCGLIFFLAGS, (char *)&lifr);
    netwib_ig(reti, NETWIB_ERR_FUIOCTL);
    if ( ! (lifr.lifr_lifru.lifru_flags1 & IFF_UP) ) {
      continue;
    }
    netwib_eg(netwib_priv_confwork_ip_init(&pci));
    pciset = NETWIB_TRUE;
    /* get ptp */
    if (lifr.lifr_lifru.lifru_flags1 & IFF_POINTOPOINT) {
      pci->ispointtopoint = NETWIB_TRUE;
    }
    /* get device */
    netwib_eg(netwib_buf_append_string(lireqbuf[i].lifr_name, &pci->device));
    /* get ip */
    netwib_c_memset(&lifr, 0, sizeof(lifr));
    netwib_c_strcpy(lifr.lifr_name, lireqbuf[i].lifr_name);
    lifr.lifr_lifru.lifru_addr.ss_family = AF_INET6;
    reti = ioctl(fd, NETWIB_SIOCGLIFADDR, (char *)&lifr);
    netwib_ig(reti, NETWIB_ERR_FUIOCTL);
    psa = (netwib_priv_sockaddr_unalign*)&lifr.lifr_lifru.lifru_addr;
    netwib_eg(netwib_priv_sa_ip_init_sa(psa, &pci->ip));
    /* get prefix */
    netwib_c_memset(&lifr, 0, sizeof(lifr));
    netwib_c_strcpy(lifr.lifr_name, lireqbuf[i].lifr_name);
    lifr.lifr_lifru.lifru_addr.ss_family = AF_INET6;
    reti = ioctl(fd, NETWIB_SIOCGLIFNETMASK, (char *)&lifr);
    netwib_ig(reti, NETWIB_ERR_FUIOCTL);
    psa = (netwib_priv_sockaddr_unalign*)&lifr.lifr_lifru.lifru_addr;
    netwib_eg(netwib_priv_ip_maskprefix_init_sa(psa, NULL, &pci->prefix));
    /* get ptpip */
    if (pci->ispointtopoint) {
      netwib_c_memset(&lifr, 0, sizeof(lifr));
      netwib_c_strcpy(lifr.lifr_name, lireqbuf[i].lifr_name);
      lifr.lifr_lifru.lifru_addr.ss_family = AF_INET6;
      reti = ioctl(fd, NETWIB_SIOCGLIFDSTADDR, (char *)&lifr);
      netwib_ig(reti, NETWIB_ERR_FUIOCTL);
      psa = (netwib_priv_sockaddr_unalign*)&lifr.lifr_lifru.lifru_addr;
      netwib_eg(netwib_priv_sa_ip_init_sa(psa, &pci->pointtopointip));
    }
    /* save value */
    netwib_eg(netwib_priv_confwork_ip_add(pcw, pci));
    pciset = NETWIB_FALSE;
  }

  /* clean and leave */
 netwib_gotolabel:
  close(fd);
  if (pcdset) {
    netwib_er(netwib_priv_confwork_devices_close(&pcd));
  }
  if (pciset) {
    netwib_er(netwib_priv_confwork_ip_close(&pci));
  }
  return(ret);
}
#else
static netwib_err netwib_priv_conf_devices_ioctl6(netwib_priv_confwork *pcw)
{
  pcw = pcw; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
#endif
#endif


