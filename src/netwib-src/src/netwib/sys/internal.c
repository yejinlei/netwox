/*
                                  NETWIB
                             Network library
                Copyright(c) 1999-2012 Laurent CONSTANTIN
                      http://ntwib.sourceforge.net/
                        laurentconstantin@free.fr
                                  -----
  This file is part of Netwib.

  Netwib is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3
  as published by the Free Software Foundation.

  Netwib is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details (http://www.gnu.org/).

------------------------------------------------------------------------
*/

#include <netwib/inc/maininc.h>

/*-------------------------------------------------------------*/
netwib_err netwib_internal_version(netwib_uint32 *pversionmajor,
                                   netwib_uint32 *pversionminor,
                                   netwib_uint32 *pversionmicro)
{

  if (pversionmajor != NULL) *pversionmajor = NETWIBDEF_VERSIONMAJOR;
  if (pversionminor != NULL) *pversionminor = NETWIBDEF_VERSIONMINOR;
  if (pversionmicro != NULL) *pversionmicro = NETWIBDEF_VERSIONMICRO;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_internal2(netwib_buf *poutbuf)
{

#define netwib_co_s(n,st) netwib_er(netwib_buf_append_fmt(poutbuf, "%s=\"%s\"\n", n, st));
#define netwib_co_i(n,i) netwib_er(netwib_buf_append_fmt(poutbuf, "%s=%{uint32}\n", n, i));

  netwib_co_s("NETWIBDEF_SYSNAME",
              NETWIBDEF_SYSNAME);
  netwib_co_s("NETWIBDEF_SYSARCH",
              NETWIBDEF_SYSARCH);
  netwib_co_i("NETWIBDEF_ARCH_ENDIAN",
              NETWIBDEF_ARCH_ENDIAN);
  netwib_co_i("NETWIBDEF_ARCH_BITS",
              NETWIBDEF_ARCH_BITS);
  netwib_co_i("NETWIBDEF_ARCH_ALIGN",
              NETWIBDEF_ARCH_ALIGN);
  netwib_co_s("NETWIBDEF_VERSION",
              NETWIBDEF_VERSION);
  netwib_co_i("NETWIBDEF_VERSIONMAJOR",
              NETWIBDEF_VERSIONMAJOR);
  netwib_co_i("NETWIBDEF_VERSIONMINOR",
              NETWIBDEF_VERSIONMINOR);
  netwib_co_i("NETWIBDEF_VERSIONMICRO",
              NETWIBDEF_VERSIONMICRO);
  netwib_co_i("NETWIBDEF_LIBPTHREADINSTALLED",
              NETWIBDEF_LIBPTHREADINSTALLED);
  netwib_co_i("NETWIBDEF_LIBPTHREADFROMDCE",
              NETWIBDEF_LIBPTHREADFROMDCE);
  netwib_co_i("NETWIBDEF_LIBPTHREADFROMMIT",
              NETWIBDEF_LIBPTHREADFROMMIT);
  netwib_co_i("NETWIBDEF_LIBPCAPINSTALLED",
              NETWIBDEF_LIBPCAPINSTALLED);
  netwib_co_i("NETWIBDEF_LIBPCAPFROMREDHAT",
              NETWIBDEF_LIBPCAPFROMREDHAT);
  netwib_co_i("NETWIBDEF_LIBNETINSTALLED",
              NETWIBDEF_LIBNETINSTALLED);
  netwib_co_i("NETWIBDEF_LIBNETVERSION11",
              NETWIBDEF_LIBNETVERSION11);
  netwib_co_i("NETWIBDEF_LIBUTILINSTALLED",
              NETWIBDEF_LIBUTILINSTALLED);
  netwib_co_s("NETWIBDEF_PROGCC",
              NETWIBDEF_PROGCC);
  netwib_co_i("NETWIBDEF_PROGCCALIAS",
              NETWIBDEF_PROGCCALIAS);
  netwib_co_i("NETWIBDEF_PROGCCWUNDEF",
              NETWIBDEF_PROGCCWUNDEF);
  netwib_co_i("NETWIBDEF_PROGCCWSIGNC",
              NETWIBDEF_PROGCCWSIGNC);
  netwib_co_i("NETWIBDEF_PROGCCWEXTRA",
              NETWIBDEF_PROGCCWEXTRA);
  netwib_co_s("NETWIBDEF_PROGAR",
              NETWIBDEF_PROGAR);
  netwib_co_s("NETWIBDEF_PROGRANLIB",
              NETWIBDEF_PROGRANLIB);
  netwib_co_s("NETWIBDEF_PROGGREP",
              NETWIBDEF_PROGGREP);
  netwib_co_s("NETWIBDEF_INSTPREFIX",
              NETWIBDEF_INSTPREFIX);
  netwib_co_s("NETWIBDEF_INSTINCLUDE",
              NETWIBDEF_INSTINCLUDE);
  netwib_co_s("NETWIBDEF_INSTLIB",
              NETWIBDEF_INSTLIB);
  netwib_co_s("NETWIBDEF_INSTBIN",
              NETWIBDEF_INSTBIN);
  netwib_co_s("NETWIBDEF_INSTMAN",
              NETWIBDEF_INSTMAN);
  netwib_co_s("NETWIBDEF_INSTMAN3",
              NETWIBDEF_INSTMAN3);
  netwib_co_i("NETWIBDEF_SUPPORT_SNIFF",
              NETWIBDEF_SUPPORT_SNIFF);
  netwib_co_i("NETWIBDEF_SUPPORT_SPOOF",
              NETWIBDEF_SUPPORT_SPOOF);
  netwib_co_i("NETWIBDEF_SUPPORT_ENCODETYPE_PCAP",
              NETWIBDEF_SUPPORT_ENCODETYPE_PCAP);
  netwib_co_i("NETWIBDEF_HAVEINCL_PTHREAD",
              NETWIBDEF_HAVEINCL_PTHREAD);
  netwib_co_i("NETWIBDEF_HAVEINCL_STROPTS",
              NETWIBDEF_HAVEINCL_STROPTS);
  netwib_co_i("NETWIBDEF_HAVEINCL_SCHED",
              NETWIBDEF_HAVEINCL_SCHED);
  netwib_co_i("NETWIBDEF_HAVEINCL_PTY",
              NETWIBDEF_HAVEINCL_PTY);
  netwib_co_i("NETWIBDEF_HAVEINCL_SYS_POLL",
              NETWIBDEF_HAVEINCL_SYS_POLL);
  netwib_co_i("NETWIBDEF_HAVEINCL_SYS_PARAM",
              NETWIBDEF_HAVEINCL_SYS_PARAM);
  netwib_co_i("NETWIBDEF_HAVEINCL_SYS_SYSCTL",
              NETWIBDEF_HAVEINCL_SYS_SYSCTL);
  netwib_co_i("NETWIBDEF_HAVEINCL_SYS_IOCTL",
              NETWIBDEF_HAVEINCL_SYS_IOCTL);
  netwib_co_i("NETWIBDEF_HAVEINCL_SYS_SOCKIO",
              NETWIBDEF_HAVEINCL_SYS_SOCKIO);
  netwib_co_i("NETWIBDEF_HAVEINCL_SYS_DLPI",
              NETWIBDEF_HAVEINCL_SYS_DLPI);
  netwib_co_i("NETWIBDEF_HAVEINCL_SYS_BUFMOD",
              NETWIBDEF_HAVEINCL_SYS_BUFMOD);
  netwib_co_i("NETWIBDEF_HAVEINCL_SYS_MMAN",
              NETWIBDEF_HAVEINCL_SYS_MMAN);
  netwib_co_i("NETWIBDEF_HAVEINCL_NET_ETHERNET",
              NETWIBDEF_HAVEINCL_NET_ETHERNET);
  netwib_co_i("NETWIBDEF_HAVEINCL_NET_IF_ARP",
              NETWIBDEF_HAVEINCL_NET_IF_ARP);
  netwib_co_i("NETWIBDEF_HAVEINCL_NET_IF_TYPES",
              NETWIBDEF_HAVEINCL_NET_IF_TYPES);
  netwib_co_i("NETWIBDEF_HAVEINCL_NET_IF_DL",
              NETWIBDEF_HAVEINCL_NET_IF_DL);
  netwib_co_i("NETWIBDEF_HAVEINCL_NET_BPF",
              NETWIBDEF_HAVEINCL_NET_BPF);
  netwib_co_i("NETWIBDEF_HAVEINCL_NETINET_IF_ETHER",
              NETWIBDEF_HAVEINCL_NETINET_IF_ETHER);
  netwib_co_i("NETWIBDEF_HAVEINCL_NETINET_ETHER",
              NETWIBDEF_HAVEINCL_NETINET_ETHER);
  netwib_co_i("NETWIBDEF_HAVEINCL_NETPACKET_PACKET",
              NETWIBDEF_HAVEINCL_NETPACKET_PACKET);
  netwib_co_i("NETWIBDEF_HAVEINCL_ASM_PAGE",
              NETWIBDEF_HAVEINCL_ASM_PAGE);
  netwib_co_i("NETWIBDEF_HAVEINCL_MACH_MACHINE_VMPARAM",
              NETWIBDEF_HAVEINCL_MACH_MACHINE_VMPARAM);
  netwib_co_i("NETWIBDEF_HAVEINCL_MACHINE_PARAM",
              NETWIBDEF_HAVEINCL_MACHINE_PARAM);
  netwib_co_i("NETWIBDEF_HAVETYPE_SIZE_T",
              NETWIBDEF_HAVETYPE_SIZE_T);
  netwib_co_i("NETWIBDEF_HAVETYPE_SOCKLEN_T",
              NETWIBDEF_HAVETYPE_SOCKLEN_T);
  netwib_co_i("NETWIBDEF_HAVETYPE_SIGHANDLER_T",
              NETWIBDEF_HAVETYPE_SIGHANDLER_T);
  netwib_co_i("NETWIBDEF_HAVETYPE_SIG_T",
              NETWIBDEF_HAVETYPE_SIG_T);
  netwib_co_i("NETWIBDEF_HAVETYPE_OFF_T",
              NETWIBDEF_HAVETYPE_OFF_T);
  netwib_co_i("NETWIBDEF_HAVETYPE_PTHREAD_RWLOCK_T",
              NETWIBDEF_HAVETYPE_PTHREAD_RWLOCK_T);
  netwib_co_i("NETWIBDEF_HAVETYPE_SOCKADDR_IN6",
              NETWIBDEF_HAVETYPE_SOCKADDR_IN6);
  netwib_co_i("NETWIBDEF_HAVEFIELD_TM_GMTOFF",
              NETWIBDEF_HAVEFIELD_TM_GMTOFF);
  netwib_co_i("NETWIBDEF_HAVEFIELD_SOCKADDR_SALEN",
              NETWIBDEF_HAVEFIELD_SOCKADDR_SALEN);
  netwib_co_i("NETWIBDEF_HAVEFIELD_SOCKADDRIN_SINLEN",
              NETWIBDEF_HAVEFIELD_SOCKADDRIN_SINLEN);
  netwib_co_i("NETWIBDEF_HAVEFIELD_SOCKADDRIN6_SIN6LEN",
              NETWIBDEF_HAVEFIELD_SOCKADDRIN6_SIN6LEN);
  netwib_co_i("NETWIBDEF_HAVEFUNC_ETHER_NTOHOST",
              NETWIBDEF_HAVEFUNC_ETHER_NTOHOST);
  netwib_co_i("NETWIBDEF_HAVEFUNC_ETHER_HOSTTON",
              NETWIBDEF_HAVEFUNC_ETHER_HOSTTON);
  netwib_co_i("NETWIBDEF_HAVEFUNC_HSTRERROR",
              NETWIBDEF_HAVEFUNC_HSTRERROR);
  netwib_co_i("NETWIBDEF_HAVEFUNC_SNPRINTF",
              NETWIBDEF_HAVEFUNC_SNPRINTF);
  netwib_co_i("NETWIBDEF_HAVEFUNC_PCAP_FREECODE",
              NETWIBDEF_HAVEFUNC_PCAP_FREECODE);
  netwib_co_i("NETWIBDEF_HAVEFUNC_PCAP_FREECODE2",
              NETWIBDEF_HAVEFUNC_PCAP_FREECODE2);
  netwib_co_i("NETWIBDEF_HAVEFUNC_PCAP_OPENDEAD",
              NETWIBDEF_HAVEFUNC_PCAP_OPENDEAD);
  netwib_co_i("NETWIBDEF_HAVEFUNC_POLL",
              NETWIBDEF_HAVEFUNC_POLL);
  netwib_co_i("NETWIBDEF_HAVEFUNC_DIRFD",
              NETWIBDEF_HAVEFUNC_DIRFD);
  netwib_co_i("NETWIBDEF_HAVEFUNC_FPATHCONF",
              NETWIBDEF_HAVEFUNC_FPATHCONF);
  netwib_co_i("NETWIBDEF_HAVEFUNC_PATHCONF",
              NETWIBDEF_HAVEFUNC_PATHCONF);
  netwib_co_i("NETWIBDEF_HAVEFUNC_SYSCONF",
              NETWIBDEF_HAVEFUNC_SYSCONF);
  netwib_co_i("NETWIBDEF_HAVEFUNC_GETPAGESIZE",
              NETWIBDEF_HAVEFUNC_GETPAGESIZE);
  netwib_co_i("NETWIBDEF_HAVEFUNC_GETRLIMIT",
              NETWIBDEF_HAVEFUNC_GETRLIMIT);
  netwib_co_i("NETWIBDEF_HAVEFUNC_NANOSLEEP",
              NETWIBDEF_HAVEFUNC_NANOSLEEP);
  netwib_co_i("NETWIBDEF_HAVEFUNC_USLEEP",
              NETWIBDEF_HAVEFUNC_USLEEP);
  netwib_co_i("NETWIBDEF_HAVEFUNC_GETTIMEOFDAY",
              NETWIBDEF_HAVEFUNC_GETTIMEOFDAY);
  netwib_co_i("NETWIBDEF_HAVEFUNC_CLOCK_GETTIME",
              NETWIBDEF_HAVEFUNC_CLOCK_GETTIME);
  netwib_co_i("NETWIBDEF_HAVEFUNC_PTHREAD_DELAY_NP",
              NETWIBDEF_HAVEFUNC_PTHREAD_DELAY_NP);
  netwib_co_i("NETWIBDEF_HAVEFUNC_PTHREAD_MUTEX_TIMEDLOCK",
              NETWIBDEF_HAVEFUNC_PTHREAD_MUTEX_TIMEDLOCK);
  netwib_co_i("NETWIBDEF_HAVEFUNC_PTHREAD_RWLOCK_TIMEDRDLOCK",
              NETWIBDEF_HAVEFUNC_PTHREAD_RWLOCK_TIMEDRDLOCK);
  netwib_co_i("NETWIBDEF_HAVEFUNC_PTHREAD_RWLOCK_TIMEDWRLOCK",
              NETWIBDEF_HAVEFUNC_PTHREAD_RWLOCK_TIMEDWRLOCK);
  netwib_co_i("NETWIBDEF_HAVEFUNC_PTHREAD_YIELD",
              NETWIBDEF_HAVEFUNC_PTHREAD_YIELD);
  netwib_co_i("NETWIBDEF_HAVEFUNC_SCHED_YIELD",
              NETWIBDEF_HAVEFUNC_SCHED_YIELD);
  netwib_co_i("NETWIBDEF_HAVEFUNC_GETPWNAM_R",
              NETWIBDEF_HAVEFUNC_GETPWNAM_R);
  netwib_co_i("NETWIBDEF_HAVEFUNC_GETGRNAM_R",
              NETWIBDEF_HAVEFUNC_GETGRNAM_R);
  netwib_co_i("NETWIBDEF_HAVEFUNC_GETGRGID_R",
              NETWIBDEF_HAVEFUNC_GETGRGID_R);
  netwib_co_i("NETWIBDEF_HAVEFUNC_READDIR_R",
              NETWIBDEF_HAVEFUNC_READDIR_R);
  netwib_co_i("NETWIBDEF_HAVEFUNC_GETHOSTBYNAME_R",
              NETWIBDEF_HAVEFUNC_GETHOSTBYNAME_R);
  netwib_co_i("NETWIBDEF_HAVEFUNC_GETHOSTBYADDR_R",
              NETWIBDEF_HAVEFUNC_GETHOSTBYADDR_R);
  netwib_co_i("NETWIBDEF_HAVEFUNC_GETNAMEINFO",
              NETWIBDEF_HAVEFUNC_GETNAMEINFO);
  netwib_co_i("NETWIBDEF_HAVEFUNC_GETADDRINFO",
              NETWIBDEF_HAVEFUNC_GETADDRINFO);
  netwib_co_i("NETWIBDEF_HAVEFUNC_GETIPNODEBYNAME",
              NETWIBDEF_HAVEFUNC_GETIPNODEBYNAME);
  netwib_co_i("NETWIBDEF_HAVEFUNC_GETIPNODEBYADDR",
              NETWIBDEF_HAVEFUNC_GETIPNODEBYADDR);
  netwib_co_i("NETWIBDEF_HAVEFUNC_LOCALTIME_R",
              NETWIBDEF_HAVEFUNC_LOCALTIME_R);
  netwib_co_i("NETWIBDEF_HAVEVAR_HERRNO",
              NETWIBDEF_HAVEVAR_HERRNO);
  netwib_co_i("NETWIBDEF_HAVEVAR_PATH_MAX",
              NETWIBDEF_HAVEVAR_PATH_MAX);
  netwib_co_i("NETWIBDEF_HAVEVAR_NAME_MAX",
              NETWIBDEF_HAVEVAR_NAME_MAX);
  netwib_co_i("NETWIBDEF_HAVEVAR_PAGE_SIZE",
              NETWIBDEF_HAVEVAR_PAGE_SIZE);
  netwib_co_i("NETWIBDEF_HAVEVAR_PAGESIZE",
              NETWIBDEF_HAVEVAR_PAGESIZE);
  netwib_co_i("NETWIBDEF_HAVEVAR_BIOCSHDRCMPLT",
              NETWIBDEF_HAVEVAR_BIOCSHDRCMPLT);
  netwib_co_i("NETWIBDEF_HAVEVAR_SIOCGLIFCONF",
              NETWIBDEF_HAVEVAR_SIOCGLIFCONF);
  netwib_co_i("NETWIBDEF_HAVEVAR_AF_INET6",
              NETWIBDEF_HAVEVAR_AF_INET6);
  netwib_co_i("NETWIBDEF_HAVEVAR_SC_GETPW_R_SIZE_MAX",
              NETWIBDEF_HAVEVAR_SC_GETPW_R_SIZE_MAX);
  netwib_co_i("NETWIBDEF_HAVEVAR_SC_GETGR_R_SIZE_MAX",
              NETWIBDEF_HAVEVAR_SC_GETGR_R_SIZE_MAX);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_internal(netwib_buf *pbuf)
{
  netwib_uint32 savedsize;
  netwib_err ret;

  if (pbuf == NULL) {
    return(NETWIB_ERR_OK);
  }

  /* save position in case of error */
  savedsize = netwib__buf_ref_data_size(pbuf);

  ret = netwib_buf_append_internal2(pbuf);

  /* on error, restore first pos */
  if (ret != NETWIB_ERR_OK) {
    pbuf->endoffset = pbuf->beginoffset + savedsize;
  }
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_internal_display(void)
{
  netwib_buf buf;
  netwib_string pc;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_internal2(&buf));
  netwib_er(netwib_buf_ref_string(&buf, &pc));
  printf("%s", pc);
  fflush(stdout);
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}
