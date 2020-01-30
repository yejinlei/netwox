/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Linux
#define NETWIB_IPINITETH_IOCTLRARP 1
static netwib_err netwib_priv_ip_init_eth_ioctlrarp(netwib_consteth *peth,
                                                    netwib_ip *pip)
{
  netwib_byte *pb;
  netwib_ip4 ip4;
  struct arpreq ar;
  int fd, reti;

  netwib_c_memset(&ar, 0, sizeof(ar));
  ar.arp_pa.sa_family = AF_INET;
  netwib_c_memcpy(ar.arp_ha.sa_data, peth->b, NETWIB_ETH_LEN);

  fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd == -1) {
    return(NETWIB_ERR_FUSOCKET);
  }

  reti = ioctl(fd, SIOCGRARP, (char *)&ar);
  close(fd);
  if (reti == -1) {
    if (errno == EINVAL || errno == ENXIO ||
        errno == ENOTTY /* Confirmed this strange name from http://lkml.indiana.edu/hypermail/linux/kernel/1201.0/01582.html */) {
      /* RARP is not supported by kernel */
      return(NETWIB_ERR_NOTCONVERTED);
    }
    return(NETWIB_ERR_FUIOCTL);
  }

  pb = (netwib_data) (ar.arp_pa.sa_data + 2);
  ip4 = (*pb++) << 24;
  ip4 |= (*pb++) << 16;
  ip4 |= (*pb++) << 8;
  ip4 |= *pb;
  if (ip4 == 0) {
    /* SuSE 5.1 indicates a 0 IP address instead of returning an error
       in ioctl(fd, SIOCGRARP, ...) */
    return(NETWIB_ERR_NOTCONVERTED);
  }
  if (pip != NULL) {
    pip->iptype = NETWIB_IPTYPE_IP4;
    pip->ipvalue.ip4 = ip4;
  }

  return(NETWIB_ERR_OK);
}
#else
 #define NETWIB_IPINITETH_IOCTLRARP 0
#endif

/*-------------------------------------------------------------*/
#if NETWIBDEF_HAVEFUNC_ETHER_NTOHOST==1
#define NETWIB_IPINITETH_ETCETHERS 1
static netwib_err netwib_priv_ip_init_eth_etcethers(netwib_consteth *peth,
                                                    netwib_ip *pip)
{
  netwib_char array[1024];
  netwib_err ret;
  struct ether_addr ea;
  int reti;

#if defined NETWIBDEF_SYSNAME_FreeBSD || defined NETWIBDEF_SYSNAME_MacOS
  netwib_c_memcpy(ea.octet, peth->b, NETWIB_ETH_LEN);
#else
  netwib_c_memcpy(ea.ether_addr_octet, peth->b, NETWIB_ETH_LEN);
#endif
  reti = ether_ntohost(array, &ea);
  if (reti) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  ret = netwib_priv_ip_init_sip4(array, pip);
  if (ret == NETWIB_ERR_OK) return(NETWIB_ERR_OK);

  /* because the function can return a hostname */
  netwib_er(netwib_priv_ip_init_hn4(array, pip));

  return(NETWIB_ERR_OK);
}
#else
 #define NETWIB_IPINITETH_ETCETHERS 0
#endif

/*-------------------------------------------------------------*/
static netwib_err netwib_ip_init_eth2(netwib_consteth *peth,
                                      netwib_ip *pip)
{
  netwib_err ret = NETWIB_ERR_NOTCONVERTED;

 #if NETWIB_IPINITETH_IOCTLRARP == 1
  ret = netwib_priv_ip_init_eth_ioctlrarp(peth, pip);
  if (ret == NETWIB_ERR_OK) return(NETWIB_ERR_OK);
  if (ret != NETWIB_ERR_NOTCONVERTED) return(ret);
 #endif

 #if NETWIB_IPINITETH_ETCETHERS == 1
  ret = netwib_priv_ip_init_eth_etcethers(peth, pip);
  if (ret == NETWIB_ERR_OK) return(NETWIB_ERR_OK);
  if (ret != NETWIB_ERR_NOTCONVERTED) return(ret);
 #endif

  ret = netwib_priv_confrel_arpcache_ip(peth, pip);
  if (ret == NETWIB_ERR_OK) return(NETWIB_ERR_OK);
  if (ret != NETWIB_ERR_NOTCONVERTED) return(ret);

  /* we could also send an RARP request, but nowadays, nobody answers */

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ip_init_eth(netwib_consteth *peth,
                              netwib_ip *pip)
{
  netwib_ip ipa;
  netwib_err ret;

  ret = netwib_priv_confglo_arpcache_ip(peth, pip);
  if (ret == NETWIB_ERR_NOTCONVERTED) {
    ret = netwib_ip_init_eth2(peth, &ipa);
    if (ret == NETWIB_ERR_OK) {
      /* add value in ARP cache */
      netwib_er(netwib_priv_confglo_arpcache_add(NULL, peth, pip));
      if (pip != NULL) *pip = ipa;
    }
  }
  return(ret);
}
