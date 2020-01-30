
/*-------------------------------------------------------------*/
#include "priv/conf.h"
#include "priv/confwork.h"
#include "priv/confdev.h"
#include "priv/confip.h"
#include "priv/confarp.h"
#include "priv/confrout.h"

/*-------------------------------------------------------------*/
#include "priv/ranges.h"
#include "priv/ips.h"

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_PRIV_DONT_WANT_SYS
 /* this works because priv/libnet.c does not use netwib_priv_sockaddr_len */
 #define netwib_priv_sockaddr_len int
#else
 #if NETWIBDEF_HAVETYPE_SOCKLEN_T==1
  #if defined NETWIBDEF_SYSNAME_HPUX
   /* under HP-UX 11.11, socklen_t is defined but not used in functions */
   #define netwib_priv_sockaddr_len int
  #else
   #define netwib_priv_sockaddr_len socklen_t
  #endif
 #else
  #define netwib_priv_sockaddr_len int
 #endif
#endif

/*-------------------------------------------------------------*/
#include "priv/sa.h"
#include "priv/ip.h"

/*-------------------------------------------------------------*/
#include "priv/libpcap.h"
#include "priv/libnet.h"

/*-------------------------------------------------------------*/
#if NETWIBDEF_ARCH_ENDIAN == 0
 /*i386*/
 #define netwib_priv_ntohl(x) ((netwib_uint32)((((netwib_uint32)(x) & 0xFFu) << 24) | (((netwib_uint32)(x) & 0xFF00u) <<  8) | (((netwib_uint32)(x) & 0xFF0000u) >>  8) | (((netwib_uint32)(x) & 0xFF000000u) >> 24)))
 #define netwib_priv_ntohs(x) ((netwib_uint16)((((netwib_uint16)(x) & 0xFF) << 8) | (((netwib_uint16)(x) & 0xFF00) >> 8)))
 #define netwib_priv_htonl(x) netwib_priv_ntohl(x)
 #define netwib_priv_htons(x) netwib_priv_ntohs(x)
#elif NETWIBDEF_ARCH_ENDIAN == 1
 /*68000*/
 #define netwib_priv_ntohl(x)        (x)
 #define netwib_priv_ntohs(x)        ((netwib_uint16)(x))
 #define netwib_priv_htonl(x)        (x)
 #define netwib_priv_htons(x)        ((netwib_uint16)(x))
#endif

/*-------------------------------------------------------------*/
#define NETWIB_FB_ROUNDUP(a) ((a)>0?(1+(((a)-1)|(sizeof(long)-1))):sizeof(long))

/*-------------------------------------------------------------*/
/* currently this one is the last : we detect it later */
#define NETWIB_PRIV_CONFEASY_HWMAX (NETWIB_DEVICE_HWTYPE_SLIP+1)

/*-------------------------------------------------------------*/
#if NETWIBDEF_HAVETYPE_SOCKLEN_T==1
 #define netwib_c_recv(a,b,c,d) recv(a,(netwib_data)(b),c,d)
 #define netwib_c_recvfrom(a,b,c,d,e,f) recvfrom(a,(netwib_data)(b),c,d,e,f)
 #define netwib_c_send(a,b,c,d) send(a,(netwib_constdata)(b),c,d)
 #define netwib_c_sendto(a,b,c,d,e,f) sendto(a,(netwib_constdata)(b),c,d,e,f)
 #define netwib_c_setsockopt(a,b,c,d,e) setsockopt(a,b,c,(netwib_constdata)(d),e)
#else
 #define netwib_c_recv(a,b,c,d) recv(a,(netwib_string)(b),c,d)
 #define netwib_c_recvfrom(a,b,c,d,e,f) recvfrom(a,(netwib_string)(b),c,d,e,f)
 #if defined NETWIBDEF_SYSNAME_HPUX
  /* because system's prototype does not have "const" */
  #define netwib_c_send(a,b,c,d) send(a,netwib_priv_caststring(b),c,d)
 #else
  #define netwib_c_send(a,b,c,d) send(a,(netwib_conststring)(b),c,d)
 #endif
 #define netwib_c_sendto(a,b,c,d,e,f) sendto(a,(netwib_conststring)(b),c,d,e,f)
 #define netwib_c_setsockopt(a,b,c,d,e) setsockopt(a,b,c,(netwib_conststring)(d),e)
#endif
#define netwib_c_getsockname(a,b,c) getsockname(a,b,c)
#define netwib_c_getpeername(a,b,c) getpeername(a,b,c)

/*-------------------------------------------------------------*/
netwib_err netwib_priv_conf_routes_gw(netwib_constip *pdst,
                                      netwib_ip *pgw,
                                      netwib_uint32 *pdevnum);
