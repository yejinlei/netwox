
/*-------------------------------------------------------------*/
#ifndef NETWIB_MAININC_INCLUDED
#define NETWIB_MAININC_INCLUDED 1

/*-------------------------------------------------------------*/
#ifdef WIN32
  #ifdef __MINGW32__
    /* mingw32 and Dev-cpp */
    #define NETWIBDEF_PROGCC_MINGW32 1
  #else
    /* Visual C++ */
    #define NETWIBDEF_PROGCC_VISUALC 1
    /* disable warning "conditional expression is constant" */
    #pragma warning(disable : 4127)
    /* decide which kind of code to generate */
    #define _WIN32_WINNT 0x0400
  #endif
#endif

/*-------------------------------------------------------------*/
#include <netwib/inc/debuga.h>

/*-------------------------------------------------------------*/
/* For the moment, we only need NETWIBDEF_SYSNAME_... */
#define NETWIB_DEF_ONLY 1
#include <netwib.h>

/*-------------------------------------------------------------*/
#ifdef NETWIBDEF_SYSNAME_Unix
 /* every Unix need it*/
 #define _REENTRANT 1
#endif

/*-------------------------------------------------------------*/
#ifdef NETWIBDEF_SYSNAME_Linux
 #define _GNU_SOURCE 1
 /* should not be necessary, but SuSE 5.1 need those three defines */
 #define _POSIX_SOURCE 1
 #define _BSD_SOURCE 1
 /* large file support */
 #define _LARGEFILE_SOURCE 1
 #define _FILE_OFFSET_BITS 64
#elif defined NETWIBDEF_SYSNAME_FreeBSD
#elif defined NETWIBDEF_SYSNAME_MacOS
#elif defined NETWIBDEF_SYSNAME_NetBSD
#elif defined NETWIBDEF_SYSNAME_OpenBSD
#elif defined NETWIBDEF_SYSNAME_Solaris
 #define __EXTENSIONS__ 1
 #define _POSIX_PTHREAD_SEMANTICS 1
 /* large file support */
 #define _LARGEFILE_SOURCE 1
 #define _FILE_OFFSET_BITS 64
#elif defined NETWIBDEF_SYSNAME_HPUX
 #define _HPUX_SOURCE 1
 /* large file support */
 #define _LARGEFILE_SOURCE 1
 #define _FILE_OFFSET_BITS 64
#elif defined NETWIBDEF_SYSNAME_Tru64UNIX
 #define _POSIX_PII 1
 #define _POSIX_C_SOURCE 1
 /* under Tru64 version 5.1, if _POSIX_C_SOURCE is defined, _XOPEN_SOURCE
    is required because of a bug in includes */
 #define _XOPEN_SOURCE 500
#elif defined NETWIBDEF_SYSNAME_AIX
#elif defined NETWIBDEF_SYSNAME_Unsupported
  /* YOU have to take a decision... You may need to put defines.
     Then, you can remove NETWIB_UNSUPPORTED_TAKEDECISION.
  */
  NETWIB_UNSUPPORTED_TAKEDECISION
  #define _GNU_SOURCE 1
  #define _POSIX_SOURCE 1
  #define _BSD_SOURCE 1
  #define _LARGEFILE_SOURCE 1
  #define _FILE_OFFSET_BITS 64
  #define __EXTENSIONS__ 1
  #define _POSIX_PTHREAD_SEMANTICS 1
  #define _HPUX_SOURCE 1
  #define _POSIX_PII 1
  #define _POSIX_C_SOURCE 1
  #define _XOPEN_SOURCE 500
#elif defined NETWIBDEF_SYSNAME_Windows
 /* nothing to include for the moment */
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
 #include <stdlib.h>
 #include <stdio.h>
 #include <stdarg.h>
 #include <unistd.h>
 #include <string.h>
 #include <ctype.h>
 #include <signal.h>
 #include <errno.h>
 #include <regex.h>
 #include <dirent.h>
 #include <time.h>
 #include <netdb.h>
 #include <termios.h>
 #include <fcntl.h>
 #include <pwd.h>
 #include <grp.h>
 #include <limits.h>
#elif defined NETWIBDEF_SYSNAME_Windows
 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>
 #include <stdarg.h>
 #include <windows.h>
 #include <limits.h>
 #include <io.h>
 #include <memory.h>
 #include <ctype.h>
 #include <conio.h>
 #include <signal.h>
 #include <errno.h>
 #include <fcntl.h>
 #include <time.h>
 #include <process.h>
 #include <direct.h>
 #include <winsock.h>
 #include "port/windows/regex/regex.h"
 #include <iphlpapi.h>
 #include <winioctl.h>
 #include <snmp.h>
 #include <ws2tcpip.h>
 #include "port/windows/ndis/ntddndis.h"
 #if 0
  /* currently unused */
  #include <ras.h>
 #endif
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <sys/wait.h>
 #include <sys/socket.h>
 #include <sys/resource.h>
 #include <sys/time.h>
#elif defined NETWIBDEF_SYSNAME_Windows
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <sys/timeb.h>
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
 #include <net/if.h>
 #include <net/route.h>
 #include <netinet/in.h>
#elif defined NETWIBDEF_SYSNAME_Windows
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
#ifdef NETWIBDEF_SYSNAME_Unix
 #if NETWIBDEF_HAVEINCL_PTHREAD==1
  #include <pthread.h>
 #endif
 #if NETWIBDEF_HAVEINCL_STROPTS==1
  #include <stropts.h>
 #endif
 #if NETWIBDEF_HAVEINCL_SCHED==1
  #include <sched.h>
 #endif
 #if NETWIBDEF_HAVEINCL_PTY==1
  #include <pty.h>
 #endif
 #if NETWIBDEF_HAVEINCL_SYS_POLL==1
  #include <sys/poll.h>
 #endif
 #if NETWIBDEF_HAVEINCL_SYS_PARAM==1
  #include <sys/param.h>
 #endif
 #if NETWIBDEF_HAVEINCL_SYS_SYSCTL==1
  #include <sys/sysctl.h>
 #endif
 #if NETWIBDEF_HAVEINCL_SYS_IOCTL==1
  #include <sys/ioctl.h>
 #endif
 #if NETWIBDEF_HAVEINCL_SYS_SOCKIO==1
  #include <sys/sockio.h>
 #endif
 #if NETWIBDEF_HAVEINCL_SYS_DLPI==1
  #include <sys/dlpi.h>
 #endif
 #if NETWIBDEF_HAVEINCL_SYS_BUFMOD==1
  #include <sys/bufmod.h>
 #endif
 #if NETWIBDEF_HAVEINCL_NET_ETHERNET==1
  #include <net/ethernet.h>
 #endif
 #if NETWIBDEF_HAVEINCL_NET_IF_ARP==1
  #include <net/if_arp.h>
 #endif
 #if NETWIBDEF_HAVEINCL_NET_IF_TYPES==1
  #include <net/if_types.h>
 #endif
 #if NETWIBDEF_HAVEINCL_NET_IF_DL==1
  #include <net/if_dl.h>
 #endif
 #if NETWIBDEF_HAVEINCL_NET_BPF==1
  #include <net/bpf.h>
 #endif
 #if NETWIBDEF_HAVEINCL_NETINET_IF_ETHER==1
  #include <netinet/if_ether.h>
 #endif
 #if NETWIBDEF_HAVEINCL_NETINET_ETHER==1
  #include <netinet/ether.h>
 #endif
 #if NETWIBDEF_HAVEINCL_NETPACKET_PACKET==1
  #include <netpacket/packet.h>
 #endif
#elif defined NETWIBDEF_SYSNAME_Windows
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
#include <netwib/inc/missing.h>

/*-------------------------------------------------------------*/
#if NETWIBDEF_LIBPCAPINSTALLED == 1
 #ifdef NETWIBDEF_SYSNAME_Unix
  #ifdef __cplusplus
   extern "C" {
  #endif
  #include <pcap.h>
  #ifdef __cplusplus
   }
  #endif
 #endif
#endif

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_FreeBSD || defined NETWIBDEF_SYSNAME_NetBSD || defined NETWIBDEF_SYSNAME_OpenBSD || defined NETWIBDEF_SYSNAME_MacOS
 #define NETWIBDEF_SYSNAME_BSD 1
#endif

/*-------------------------------------------------------------*/
/* now, we can fully include netwib */
#define NETWIB_EXT_FILE 1
#if defined NETWIBDEF_SYSNAME_Windows
 #define NETWIB_EXT_HANDLE 1
#endif
#include <netwib.h>

/*-------------------------------------------------------------*/
#include <netwib/inc/debugb.h>

/*-------------------------------------------------------------*/
#include <netwib/dat/priv.h>
#include <netwib/dat/debug.h>

#include <netwib/sys/priv.h>
#include <netwib/sys/debug.h>

#include <netwib/net/priv.h>
#include <netwib/net/debug.h>

#include <netwib/pkt/priv.h>
#include <netwib/pkt/debug.h>

/*-------------------------------------------------------------*/
#endif
