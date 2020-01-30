/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
#ifndef LIBNET_ERRBUF_SIZE
 /* because it's not defined in old versions of libnet */
 #define LIBNET_ERRBUF_SIZE 256
#endif

/*-------------------------------------------------------------*/
#define NETWIB_PRIV_LIBNET_USE_LIBNET10   1
#define NETWIB_PRIV_LIBNET_USE_LIBNET11   2
#define NETWIB_PRIV_LIBNET_USE_PACKETDLL  3
#define NETWIB_PRIV_LIBNET_USE_WINSOCK2   4
#define NETWIB_PRIV_LIBNET_USE_NOTSUP     5
#if defined NETWIBDEF_SYSNAME_Unix
 #if NETWIBDEF_LIBNETINSTALLED == 1
  #if NETWIBDEF_LIBNETVERSION11 == 1
   #define NETWIB_PRIV_LIBNET_USE_LINK NETWIB_PRIV_LIBNET_USE_LIBNET11
   #define NETWIB_PRIV_LIBNET_USE_IP4 NETWIB_PRIV_LIBNET_USE_LIBNET11
   #define NETWIB_PRIV_LIBNET_USE_IP6 NETWIB_PRIV_LIBNET_USE_LIBNET11
  #else
   #define NETWIB_PRIV_LIBNET_USE_LINK NETWIB_PRIV_LIBNET_USE_LIBNET10
   #define NETWIB_PRIV_LIBNET_USE_IP4 NETWIB_PRIV_LIBNET_USE_LIBNET10
   #define NETWIB_PRIV_LIBNET_USE_IP6 NETWIB_PRIV_LIBNET_USE_NOTSUP
   #define NETWIB_PRIV_LIBNET_ERR_IP6 "libnet 1.0 does not support IPv6 spoof"
  #endif
 #else
  #define NETWIB_PRIV_LIBNET_USE_LINK NETWIB_PRIV_LIBNET_USE_NOTSUP
  #define NETWIB_PRIV_LIBNET_USE_IP4 NETWIB_PRIV_LIBNET_USE_NOTSUP
  #define NETWIB_PRIV_LIBNET_USE_IP6 NETWIB_PRIV_LIBNET_USE_NOTSUP
  #define NETWIB_PRIV_LIBNET_ERR_LINK "netwib wasn't compiled with libnet support"
  #define NETWIB_PRIV_LIBNET_ERR_IP4 NETWIB_PRIV_LIBNET_ERR_LINK
  #define NETWIB_PRIV_LIBNET_ERR_IP6 NETWIB_PRIV_LIBNET_ERR_LINK
 #endif
#elif defined NETWIBDEF_SYSNAME_Windows
 #if NETWIBDEF_LIBPCAPINSTALLED == 1
  #define NETWIB_PRIV_LIBNET_USE_LINK NETWIB_PRIV_LIBNET_USE_PACKETDLL
 #else
  #define NETWIB_PRIV_LIBNET_USE_LINK NETWIB_PRIV_LIBNET_USE_NOTSUP
  #define NETWIB_PRIV_LIBNET_ERR_LINK "netwib wasn't compiled with WinPCAP support"
 #endif
 #define NETWIB_PRIV_LIBNET_USE_IP4 NETWIB_PRIV_LIBNET_USE_WINSOCK2
 #define NETWIB_PRIV_LIBNET_USE_IP6 NETWIB_PRIV_LIBNET_USE_NOTSUP
 #define NETWIB_PRIV_LIBNET_ERR_IP6 "Windows support for IPv6 spoof is not implemented"
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_priv_libnet_notsupported(netwib_conststring msg);
netwib_err netwib_priv_libnet_notsupported(netwib_conststring msg)
{
  netwib_er(netwib_priv_errmsg_string(msg));
  return(NETWIB_ERR_LONOTSUPPORTED);
}

/*-------------------------------------------------------------*/
#if NETWIB_PRIV_LIBNET_USE_LINK == NETWIB_PRIV_LIBNET_USE_NOTSUP
 #define netwib_priv_libnet_supports_link(d,h,b) netwib_priv_libnet_notsupported(NETWIB_PRIV_LIBNET_ERR_LINK)
 #define netwib_priv_libnet_init_link(d,l) netwib_priv_libnet_notsupported(NETWIB_PRIV_LIBNET_ERR_LINK)
 #define netwib_priv_libnet_close_link(l) netwib_priv_libnet_notsupported(NETWIB_PRIV_LIBNET_ERR_LINK)
 #define netwib_priv_libnet_write_link(l,d,s) netwib_priv_libnet_notsupported(NETWIB_PRIV_LIBNET_ERR_LINK)
 #define netwib_priv_libnet_wait_link(l,t,e) netwib_priv_libnet_notsupported(NETWIB_PRIV_LIBNET_ERR_LINK)
#else
static netwib_err netwib_priv_libnet_supports_link(netwib_conststring device,
                                                   netwib_device_hwtype hwtype,
                                                   netwib_bool *pyes);
static netwib_err netwib_priv_libnet_init_link(netwib_conststring device,
                                               netwib_priv_libnet *plib);
static netwib_err netwib_priv_libnet_close_link(netwib_priv_libnet *plib);
static netwib_err netwib_priv_libnet_write_link(netwib_priv_libnet *plib,
                                                netwib_constdata data,
                                                netwib_uint32 datasize);
static netwib_err netwib_priv_libnet_wait_link(netwib_priv_libnet *plib,
                                               netwib_consttime *pabstime,
                                               netwib_bool *pevent);
#endif

/*-------------------------------------------------------------*/
#if NETWIB_PRIV_LIBNET_USE_IP4 == NETWIB_PRIV_LIBNET_USE_NOTSUP
 #define netwib_priv_libnet_supports_ip4(b) netwib_priv_libnet_notsupported(NETWIB_PRIV_LIBNET_ERR_IP4)
 #define netwib_priv_libnet_init_ip4(l) netwib_priv_libnet_notsupported(NETWIB_PRIV_LIBNET_ERR_IP4)
 #define netwib_priv_libnet_close_ip4(l) netwib_priv_libnet_notsupported(NETWIB_PRIV_LIBNET_ERR_IP4)
 #define netwib_priv_libnet_write_ip4(l,d,s) netwib_priv_libnet_notsupported(NETWIB_PRIV_LIBNET_ERR_IP4)
 #define netwib_priv_libnet_wait_ip4(l,t,e) netwib_priv_libnet_notsupported(NETWIB_PRIV_LIBNET_ERR_IP4)
#else
static netwib_err netwib_priv_libnet_supports_ip4(netwib_bool *pyes);
static netwib_err netwib_priv_libnet_init_ip4(netwib_priv_libnet *plib);
static netwib_err netwib_priv_libnet_close_ip4(netwib_priv_libnet *plib);
static netwib_err netwib_priv_libnet_write_ip4(netwib_priv_libnet *plib,
                                               netwib_constdata data,
                                               netwib_uint32 datasize);
static netwib_err netwib_priv_libnet_wait_ip4(netwib_priv_libnet *plib,
                                              netwib_consttime *pabstime,
                                              netwib_bool *pevent);
#endif

/*-------------------------------------------------------------*/
#if NETWIB_PRIV_LIBNET_USE_IP6 == NETWIB_PRIV_LIBNET_USE_NOTSUP
 #define netwib_priv_libnet_supports_ip6(b) netwib_priv_libnet_notsupported(NETWIB_PRIV_LIBNET_ERR_IP6)
 #define netwib_priv_libnet_init_ip6(l) netwib_priv_libnet_notsupported(NETWIB_PRIV_LIBNET_ERR_IP6)
 #define netwib_priv_libnet_close_ip6(l) netwib_priv_libnet_notsupported(NETWIB_PRIV_LIBNET_ERR_IP6)
 #define netwib_priv_libnet_write_ip6(l,d,s) netwib_priv_libnet_notsupported(NETWIB_PRIV_LIBNET_ERR_IP6)
 #define netwib_priv_libnet_wait_ip6(l,t,e) netwib_priv_libnet_notsupported(NETWIB_PRIV_LIBNET_ERR_IP6)
#else
static netwib_err netwib_priv_libnet_supports_ip6(netwib_bool *pyes);
static netwib_err netwib_priv_libnet_init_ip6(netwib_priv_libnet *plib);
static netwib_err netwib_priv_libnet_close_ip6(netwib_priv_libnet *plib);
static netwib_err netwib_priv_libnet_write_ip6(netwib_priv_libnet *plib,
                                               netwib_constdata data,
                                               netwib_uint32 datasize);
static netwib_err netwib_priv_libnet_wait_ip6(netwib_priv_libnet *plib,
                                              netwib_consttime *pabstime,
                                              netwib_bool *pevent);
#endif

/*-------------------------------------------------------------*/
#if NETWIB_PRIV_LIBNET_USE_LINK == NETWIB_PRIV_LIBNET_USE_LIBNET10
static netwib_err netwib_priv_libnet_supports_link(netwib_conststring device,
                                                   netwib_device_hwtype hwtype,
                                                   netwib_bool *pyes)
{
  /* do not check if device/hwtype is correct */
  if (pyes != NULL) *pyes = NETWIB_TRUE;
  device = device; /* for compiler warning */
  hwtype = hwtype; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_init_link(netwib_conststring device,
                                               netwib_priv_libnet *plib)
{
  char errbuf[LIBNET_ERRBUF_SIZE];
  struct libnet_link_int *plli;
  /* use cast because libnet prototype is not "const" */
  plli = libnet_open_link_interface(netwib_priv_caststring(device), errbuf);
  if (plli == NULL) {
    netwib_er(netwib_priv_errmsg_string(errbuf));
    return(NETWIB_ERR_LONOTSUPPORTED);
  }
  plib->plli = plli;
  plib->fd = plli->fd;
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_close_link(netwib_priv_libnet *plib)
{
  int reti;
  reti = libnet_close_link_interface((struct libnet_link_int*)plib->plli);
  netwib_ir(reti, NETWIB_ERR_FULIBNETCLOSELINKINT);
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_write_link(netwib_priv_libnet *plib,
                                                netwib_constdata data,
                                                netwib_uint32 datasize)
{
  netwib_data device;
  int reti;
  device = (netwib_data)plib->device.totalptr; /* '\0' is already added */
  reti = libnet_write_link_layer((struct libnet_link_int*)plib->plli,
#ifndef __cplusplus
 /* there is void* because libnet_write_link_layer prototype requires
    device to be a char* or byte* depending on version */
                                 (void*)device,
#else
 /* g++ forbids void*, so we have to cast by hand */
                                 (char*)device,
#endif
                                 netwib_priv_castdata(data), datasize);
  netwib_ir(reti, NETWIB_ERR_FULIBNETWRITELL);
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_wait_link(netwib_priv_libnet *plib,
                                               netwib_consttime *pabstime,
                                               netwib_bool *pevent)
{
  netwib_er(netwib_priv_fd_wait(plib->fd, NETWIB_IO_WAYTYPE_WRITE,
                              pabstime, pevent));
  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
#if NETWIB_PRIV_LIBNET_USE_IP4 == NETWIB_PRIV_LIBNET_USE_LIBNET10
static netwib_err netwib_priv_libnet_supports_ip4(netwib_bool *pyes)
{
  if (pyes != NULL) *pyes = NETWIB_TRUE;
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_init_ip4(netwib_priv_libnet *plib)
{
  plib->fd = libnet_open_raw_sock(IPPROTO_RAW);
  netwib_ir(plib->fd, NETWIB_ERR_FULIBNETOPENRAWSOCK);
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_close_ip4(netwib_priv_libnet *plib)
{
  int reti;
  reti = libnet_close_raw_sock(plib->fd);
  netwib_ir(reti, NETWIB_ERR_FULIBNETCLOSERAWSOCK);
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_write_ip4(netwib_priv_libnet *plib,
                                               netwib_constdata data,
                                               netwib_uint32 datasize)
{
  int reti;
  reti = libnet_write_ip(plib->fd, netwib_priv_castdata(data), datasize);
  netwib_ir(reti, NETWIB_ERR_FULIBNETWRITEIP);
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_wait_ip4(netwib_priv_libnet *plib,
                                              netwib_consttime *pabstime,
                                              netwib_bool *pevent)
{
  netwib_er(netwib_priv_fd_wait(plib->fd, NETWIB_IO_WAYTYPE_WRITE,
                              pabstime, pevent));
  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
#if NETWIB_PRIV_LIBNET_USE_LINK == NETWIB_PRIV_LIBNET_USE_LIBNET11
static netwib_err netwib_priv_libnet_supports_link(netwib_conststring device,
                                                   netwib_device_hwtype hwtype,
                                                   netwib_bool *pyes)
{
  /* there is a bug in libnet 1.1.0 : fd 0 is closed when opening
     an unsupported device. However, it's not easy to determine
     what's supported or not. So I choose to support everything. */
  if (pyes != NULL) *pyes = NETWIB_TRUE;
  device = device; /* for compiler warning */
  hwtype = hwtype; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_init_link(netwib_conststring device,
                                               netwib_priv_libnet *plib)
{
  char errbuf[LIBNET_ERRBUF_SIZE];
  plib->plibnett = libnet_init(LIBNET_LINK, netwib_priv_caststring(device),
                               errbuf);
  if (plib->plibnett == NULL) {
    netwib_er(netwib_priv_errmsg_string(errbuf));
    return(NETWIB_ERR_FULIBNETINIT);
  }
  plib->fd = libnet_getfd((libnet_t*)plib->plibnett);
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_close_link(netwib_priv_libnet *plib)
{
  libnet_destroy((libnet_t*)plib->plibnett);
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_write_link(netwib_priv_libnet *plib,
                                                netwib_constdata data,
                                                netwib_uint32 datasize)
{
  int reti;
  reti = libnet_write_link((libnet_t*)plib->plibnett,
                           netwib_priv_castdata(data), datasize);
  netwib_ir(reti, NETWIB_ERR_FULIBNETWRITELINK);
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_wait_link(netwib_priv_libnet *plib,
                                               netwib_consttime *pabstime,
                                               netwib_bool *pevent)
{
  netwib_er(netwib_priv_fd_wait(plib->fd, NETWIB_IO_WAYTYPE_WRITE,
                                pabstime, pevent));
  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
#if NETWIB_PRIV_LIBNET_USE_IP4 == NETWIB_PRIV_LIBNET_USE_LIBNET11
static netwib_err netwib_priv_libnet_supports_ip4(netwib_bool *pyes)
{
  if (pyes != NULL) *pyes = NETWIB_TRUE;
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_init_ip4(netwib_priv_libnet *plib)
{
  char errbuf[LIBNET_ERRBUF_SIZE];
  plib->plibnett = libnet_init(LIBNET_RAW4, NULL, errbuf);
  if (plib->plibnett == NULL) {
    netwib_er(netwib_priv_errmsg_string(errbuf));
    return(NETWIB_ERR_FULIBNETINIT);
  }
  plib->fd = libnet_getfd((libnet_t*)plib->plibnett);
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_close_ip4(netwib_priv_libnet *plib)
{
  libnet_destroy((libnet_t*)plib->plibnett);
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_write_ip4(netwib_priv_libnet *plib,
                                               netwib_constdata data,
                                               netwib_uint32 datasize)
{
  int reti;
  reti = libnet_write_raw_ipv4((libnet_t*)plib->plibnett,
                               netwib_priv_castdata(data), datasize);
  netwib_ir(reti, NETWIB_ERR_FULIBNETWRITERAWIPV4);
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_wait_ip4(netwib_priv_libnet *plib,
                                              netwib_consttime *pabstime,
                                              netwib_bool *pevent)
{
  netwib_er(netwib_priv_fd_wait(plib->fd, NETWIB_IO_WAYTYPE_WRITE,
                                pabstime, pevent));
  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
#if NETWIB_PRIV_LIBNET_USE_IP6 == NETWIB_PRIV_LIBNET_USE_LIBNET11
static netwib_err netwib_priv_libnet_supports_ip6(netwib_bool *pyes)
{
  char errbuf[LIBNET_ERRBUF_SIZE];
  libnet_t *plibnett;
  plibnett = libnet_init(LIBNET_RAW6, NULL, errbuf);
  if (plibnett == NULL) {
    netwib_er(netwib_priv_errmsg_string("libnet does not support raw IPv6"));
    if (pyes != NULL) *pyes = NETWIB_FALSE;
  } else {
    libnet_destroy(plibnett);
    if (pyes != NULL) *pyes = NETWIB_TRUE;
  }
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_init_ip6(netwib_priv_libnet *plib)
{
  char errbuf[LIBNET_ERRBUF_SIZE];
  plib->plibnett = libnet_init(LIBNET_RAW6, NULL, errbuf);
  if (plib->plibnett == NULL) {
    netwib_er(netwib_priv_errmsg_string(errbuf));
    return(NETWIB_ERR_FULIBNETINIT);
  }
  plib->fd = libnet_getfd((libnet_t*)plib->plibnett);
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_close_ip6(netwib_priv_libnet *plib)
{
  libnet_destroy((libnet_t*)plib->plibnett);
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_write_ip6(netwib_priv_libnet *plib,
                                               netwib_constdata data,
                                               netwib_uint32 datasize)
{
  int reti;
  reti = libnet_write_raw_ipv6((libnet_t*)plib->plibnett,
                               netwib_priv_castdata(data), datasize);
  netwib_ir(reti, NETWIB_ERR_FULIBNETWRITERAWIPV6);
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_wait_ip6(netwib_priv_libnet *plib,
                                              netwib_consttime *pabstime,
                                              netwib_bool *pevent)
{
  netwib_er(netwib_priv_fd_wait(plib->fd, NETWIB_IO_WAYTYPE_WRITE,
                                pabstime, pevent));
  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
#if NETWIB_PRIV_LIBNET_USE_LINK == NETWIB_PRIV_LIBNET_USE_PACKETDLL
static netwib_err netwib_priv_libnet_supports_link(netwib_conststring device,
                                                   netwib_device_hwtype hwtype,
                                                   netwib_bool *pyes)
{
  if (netwib_global_dll_packet.isloaded) {
    if (hwtype == NETWIB_DEVICE_HWTYPE_ETHER) {
      if (pyes != NULL) *pyes = NETWIB_TRUE;
    } else {
      netwib_er(netwib_priv_errmsg_string("device is not of Ethernet type"));
      if (pyes != NULL) *pyes = NETWIB_FALSE;
    }
  } else {
    netwib_er(netwib_priv_errmsg_string("WinPCAP >= 3.0 is not found"));
    if (pyes != NULL) *pyes = NETWIB_FALSE;
  }
  device = device; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_init_link(netwib_conststring device,
                                               netwib_priv_libnet *plib)
{
  BOOL err;
  netwib_er(netwib_priv_cs_PacketOpenAdapter(netwib_priv_caststring(device),
                                             (LPADAPTER *)&(plib->padapter)));
  if (plib->padapter == NULL) {
    return(NETWIB_ERR_FUPACKETOPENADAPTER);
  }
  err = netwib_global_dll_packet.PacketSetBuff(plib->padapter, 512000);
  if (!err) {
    return(NETWIB_ERR_FUPACKETSETBUFF);
  }
  plib->ppacket = netwib_global_dll_packet.PacketAllocatePacket();
  if (plib->ppacket == NULL) {
    return(NETWIB_ERR_FUPACKETALLOCATEPACKET);
  }
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_close_link(netwib_priv_libnet *plib)
{
  netwib_global_dll_packet.PacketFreePacket(plib->ppacket);
  netwib_priv_cs_PacketCloseAdapter(plib->padapter);
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_write_link(netwib_priv_libnet *plib,
                                                netwib_constdata data,
                                                netwib_uint32 datasize)
{
  BOOL err;
  netwib_global_dll_packet.PacketInitPacket(plib->ppacket, netwib_priv_castdata(data), datasize);
  err = netwib_global_dll_packet.PacketSendPacket(plib->padapter, plib->ppacket, TRUE);
  if (!err) {
    return(NETWIB_ERR_FUPACKETSENDPACKET);
  }
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_wait_link(netwib_priv_libnet *plib,
                                               netwib_consttime *pabstime,
                                               netwib_bool *pevent)
{
  if (pevent != NULL) *pevent = NETWIB_TRUE;
  plib = plib; /* for compiler warning */
  pabstime = pabstime; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
#if NETWIB_PRIV_LIBNET_USE_IP4 == NETWIB_PRIV_LIBNET_USE_WINSOCK2
static netwib_err netwib_priv_libnet_supports_ip4(netwib_bool *pyes)
{
  netwib_windowstype windowstype;
  if (!netwib_global_dll_winsock2.isloaded) {
    netwib_er(netwib_priv_errmsg_string("Winsock2 not found"));
    if (pyes != NULL) *pyes = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  }
  /* unfortunately, Winsock 2.2 exists under WinNT, but IP spoofing
     is only supported starting from Win2000 */
  netwib_er(netwib_windowstype_init(&windowstype));
  switch(windowstype) {
    case NETWIB_WINDOWSTYPE_31 :
    case NETWIB_WINDOWSTYPE_95 :
    case NETWIB_WINDOWSTYPE_98 :
    case NETWIB_WINDOWSTYPE_ME :
    case NETWIB_WINDOWSTYPE_NT350 :
    case NETWIB_WINDOWSTYPE_NT351 :
    case NETWIB_WINDOWSTYPE_NT4 :
      netwib_er(netwib_priv_errmsg_string("Winsock2 version is too old"));
      if (pyes != NULL) *pyes = NETWIB_FALSE;
      break;
    case NETWIB_WINDOWSTYPE_2000 :
    case NETWIB_WINDOWSTYPE_XP :
    case NETWIB_WINDOWSTYPE_2003 :
    case NETWIB_WINDOWSTYPE_2008 :
    case NETWIB_WINDOWSTYPE_7 :
    case NETWIB_WINDOWSTYPE_UNKNOWN :
      if (pyes != NULL) *pyes = NETWIB_TRUE;
      break;
  }
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_init_ip4(netwib_priv_libnet *plib)
{
  plib->fd = netwib_global_dll_winsock2.socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
  if (plib->fd == INVALID_SOCKET) {
    return(NETWIB_ERR_FUSOCKET);
  }
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_close_ip4(netwib_priv_libnet *plib)
{
  int reti;
  reti = netwib_global_dll_winsock2.closesocket(plib->fd);
  netwib_ir(reti, NETWIB_ERR_FUCLOSE);
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_write_ip4(netwib_priv_libnet *plib,
                                             netwib_constdata data,
                                             netwib_uint32 datasize)
{
  struct sockaddr_in sain;
  netwib_ip4 ip4;
  int one, reti;
  memset(&sain, 0, sizeof(struct sockaddr_in));
  sain.sin_family = AF_INET;
  if (datasize > 19) {
    ip4 = netwib_c2_uint32_4(data[16], data[17], data[18], data[19]);
    sain.sin_addr.s_addr = netwib_priv_htonl(ip4);
  }
  /* IP header is included */
  one = 1;
  reti = netwib_global_dll_winsock2.setsockopt(plib->fd, IPPROTO_IP,
                                               IP_HDRINCL,
                                               (netwib_conststring)&one,
                                               sizeof(one));
  netwib_ir(reti, NETWIB_ERR_FUSETSOCKOPT);
  /* to be able to send broadcast packets */
  one = 1;
  reti = netwib_global_dll_winsock2.setsockopt(plib->fd, SOL_SOCKET,
                                               SO_BROADCAST,
                                               (netwib_conststring)&one,
                                               sizeof(one));
  netwib_ir(reti, NETWIB_ERR_FUSETSOCKOPT);
  /* send */
  reti = netwib_global_dll_winsock2.sendto(plib->fd,
                                           (netwib_conststring)data, datasize,
                                           0, (struct sockaddr *)&sain,
                                           sizeof(struct sockaddr));
  netwib_ir(reti, NETWIB_ERR_FUSENDTO);
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_libnet_wait_ip4(netwib_priv_libnet *plib,
                                              netwib_consttime *pabstime,
                                              netwib_bool *pevent)
{
  if (pevent != NULL) *pevent = NETWIB_TRUE;
  plib = plib; /* for compiler warning */
  pabstime = pabstime; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_priv_libnet_supports(netwib_constbuf *pdevice,
                                       netwib_spoof_inittype inittype,
                                       netwib_bool *pyes)
{
  netwib_buf bufdevice;
  netwib_string device;
  netwib_device_hwtype hwtype;
  netwib_err ret;

  ret = NETWIB_ERR_OK;
  switch(inittype) {
  case NETWIB_SPOOF_INITTYPE_LINK :
    netwib_er(netwib_buf_init_mallocdefault(&bufdevice));
    ret = netwib_priv_conf_device_info(pdevice, &bufdevice, NULL, &hwtype,
                                       NULL);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_buf_ref_string(&bufdevice, &device));
      ret = netwib_priv_libnet_supports_link(device, hwtype, pyes);
    }
    netwib_er(netwib_buf_close(&bufdevice));
    break;
  case NETWIB_SPOOF_INITTYPE_IP4 :
    ret = netwib_priv_libnet_supports_ip4(pyes);
    break;
  case NETWIB_SPOOF_INITTYPE_IP6 :
    ret = netwib_priv_libnet_supports_ip6(pyes);
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  }
  if (ret == NETWIB_ERR_LONOTSUPPORTED) {
    if (pyes != NULL) *pyes = NETWIB_FALSE;
    ret = NETWIB_ERR_OK;
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_libnet_init(netwib_constbuf *pdevice,
                                   netwib_spoof_inittype inittype,
                                   netwib_priv_libnet *plib)
{
  netwib_string device;
  netwib_err ret;

  plib->inittype = inittype;

  switch(inittype) {
  case NETWIB_SPOOF_INITTYPE_LINK :
    netwib_er(netwib_buf_init_mallocdefault(&plib->device));
    ret = netwib_priv_conf_device_info(pdevice, &plib->device, NULL,
                                       &plib->hwtype, NULL);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_buf_ref_string(&plib->device, &device));
      ret = netwib_priv_libnet_init_link(device, plib);
    }
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwib_buf_close(&plib->device));
      return(ret);
    }
    break;
  case NETWIB_SPOOF_INITTYPE_IP4 :
    netwib_er(netwib_priv_libnet_init_ip4(plib));
    break;
  case NETWIB_SPOOF_INITTYPE_IP6 :
    netwib_er(netwib_priv_libnet_init_ip6(plib));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_libnet_close(netwib_priv_libnet *plib)
{
  switch(plib->inittype) {
  case NETWIB_SPOOF_INITTYPE_LINK :
    netwib_er(netwib_priv_libnet_close_link(plib));
    netwib_er(netwib_buf_close(&plib->device));
    break;
  case NETWIB_SPOOF_INITTYPE_IP4 :
    netwib_er(netwib_priv_libnet_close_ip4(plib));
    break;
  case NETWIB_SPOOF_INITTYPE_IP6 :
    netwib_er(netwib_priv_libnet_close_ip6(plib));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_libnet_write(netwib_priv_libnet *plib,
                                    netwib_constbuf *pbuf)
{
  netwib_data data;
  netwib_uint32 datasize;

  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);

  switch(plib->inittype) {
  case NETWIB_SPOOF_INITTYPE_LINK :
    netwib_er(netwib_priv_libnet_write_link(plib, data, datasize));
    break;
  case NETWIB_SPOOF_INITTYPE_IP4 :
    netwib_er(netwib_priv_libnet_write_ip4(plib, data, datasize));
    break;
  case NETWIB_SPOOF_INITTYPE_IP6 :
    netwib_er(netwib_priv_libnet_write_ip6(plib, data, datasize));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_libnet_wait(netwib_priv_libnet *plib,
                                   netwib_io_waytype way,
                                   netwib_consttime *pabstime,
                                   netwib_bool *pevent)
{

  switch(plib->inittype) {
  case NETWIB_SPOOF_INITTYPE_LINK :
    netwib_er(netwib_priv_libnet_wait_link(plib, pabstime, pevent));
    break;
  case NETWIB_SPOOF_INITTYPE_IP4 :
    netwib_er(netwib_priv_libnet_wait_ip4(plib, pabstime, pevent));
    break;
  case NETWIB_SPOOF_INITTYPE_IP6 :
    netwib_er(netwib_priv_libnet_wait_ip6(plib, pabstime, pevent));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  }

  way = way; /* for compiler warning */
  pabstime = pabstime; /* for compiler warning */
  pevent = pevent; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_libnet_get_dlt(netwib_priv_libnet *plib)
{

  switch(plib->inittype) {
  case NETWIB_SPOOF_INITTYPE_LINK :
    break;
  case NETWIB_SPOOF_INITTYPE_IP4 :
  case NETWIB_SPOOF_INITTYPE_IP6 :
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  }

  switch(plib->hwtype) {
    case NETWIB_DEVICE_HWTYPE_ETHER :
      plib->dlttype = NETWIB_DEVICE_DLTTYPE_EN10MB;
      break;
    case NETWIB_DEVICE_HWTYPE_LOOPBACK :
#if defined NETWIBDEF_SYSNAME_Linux
      plib->dlttype = NETWIB_DEVICE_DLTTYPE_EN10MB;
#else
      plib->dlttype = NETWIB_DEVICE_DLTTYPE_NULL;
#endif
      break;
    case NETWIB_DEVICE_HWTYPE_PPP :
      plib->dlttype = NETWIB_DEVICE_DLTTYPE_NULL;
      break;
    default :
      /* this is surely incorrect, but I do not have network cards
         other than Ethernet... */
      plib->dlttype = NETWIB_DEVICE_DLTTYPE_NULL;
      break;
  }

  return(NETWIB_ERR_OK);
}
