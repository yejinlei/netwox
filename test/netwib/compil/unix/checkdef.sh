:

# note : this script does not ensure to find errors. It just help

PROG_GREP=grep
PROG_SED=sed

deffile="../../../../src/netwib-src/src/netwib/def.h"

nametogrep ()
{
  ${PROG_GREP} "NETWIBDEF_HAVE" $deffile | ${PROG_GREP} " 0" |\
    ${PROG_GREP} -v "NETWIBDEF_HAVEINCL" |\
    ${PROG_SED} 's/^.*_HAVE//' |\
    ${PROG_SED} 's/ 0$//'

}

inctosearch ()
{
  ${PROG_GREP} "NETWIBDEF_HAVE" $deffile | ${PROG_GREP} " 0" |\
    ${PROG_GREP} "NETWIBDEF_HAVEINCL" |\
    ${PROG_SED} 's/^.*_HAVE//' |\
    ${PROG_SED} 's/ 0$//'

}

searchstring ()
{
  var=$1
  case $var in
    INCL_PTHREAD)
     echo "pthread.h"
     ;;
    INCL_STROPTS)
     echo "stropts.h"
     ;;
    INCL_SCHED)
     echo "sched.h"
     ;;
    INCL_PTY)
     echo "pty.h"
     ;;
    INCL_SYS_POLL)
     echo "sys/poll.h"
     ;;
    INCL_SYS_PARAM)
     echo "sys/param.h"
     ;;
    INCL_SYS_SYSCTL)
     echo "sys/systl.h"
     ;;
    INCL_SYS_IOCTL)
     echo "sys/ioctl.h"
     ;;
    INCL_SYS_SOCKIO)
     echo "sys/sockio.h"
     ;;
    INCL_SYS_DLPI)
     echo "sys/dpli.h"
     ;;
    INCL_SYS_BUFMOD)
     echo "sys/bufmod.h"
     ;;
    INCL_SYS_MMAN)
     echo "sys/mman.h"
     ;;
    INCL_NET_ETHERNET)
     echo "net/ethernet.h"
     ;;
    INCL_NET_IF_ARP)
     echo "net/if_arp.h"
     ;;
    INCL_NET_IF_TYPES)
     echo "net/if_types.h"
     ;;
    INCL_NET_IF_DL)
     echo "net/if_dl.h"
     ;;
    INCL_NET_BPF)
     echo "net/bpf.h"
     ;;
    INCL_NETINET_IF_ETHER)
     echo "netinit/if_ether.h"
     ;;
    INCL_NETINET_ETHER)
     echo "netinet/ether.h"
     ;;
    INCL_NETPACKET_PACKET)
     echo "netpacket/packet.h"
     ;;
    INCL_ASM_PAGE)
     echo "asm/page.h"
     ;;
    INCL_MACH_MACHINE_VMPARAM)
     echo "mach/machine/vmparam.h"
     ;;
    INCL_MACHINE_PARAM)
     echo "machine/param.h"
     ;;
    TYPE_SIZE_T)
     echo "size_t"
     ;;
    TYPE_SOCKLEN_T)
     echo "socklen_t"
     ;;
    TYPE_SIGHANDLER_T)
     echo "sighandler_t"
     ;;
    TYPE_SIG_T)
     echo "sig_t"
     ;;
    TYPE_OFF_T)
     echo "off_t"
     ;;
    TYPE_PTHREAD_RWLOCK_T)
     echo "pthread_rwlock_t"
     ;;
    TYPE_SOCKADDR_IN6)
     echo "sockaddr_in6"
     ;;
    FIELD_TM_GMTOFF)
     echo "tm_gmtoff"
     ;;
    FIELD_SOCKADDR_SALEN)
     echo "sa_len"
     ;;
    FIELD_SOCKADDRIN_SINLEN)
     echo "sin_len"
     ;;
    FIELD_SOCKADDRIN6_SIN6LEN)
     echo "sin6_len"
     ;;
    FUNC_ETHER_NTOHOST)
     echo "ether_ntohost"
     ;;
    FUNC_ETHER_HOSTTON)
     echo "ether_hostton"
     ;;
    FUNC_HSTRERROR)
     echo "hstrerror"
     ;;
    FUNC_SNPRINTF)
     echo "snprintf"
     ;;
    FUNC_PCAP_FREECODE)
     echo "pcap_freecode"
     ;;
    FUNC_PCAP_FREECODE2)
     echo "pcap_freecode"
     ;;
    FUNC_PCAP_OPENDEAD)
     echo "pcap_opendead"
     ;;
    FUNC_POLL)
     echo "poll"
     ;;
    FUNC_DIRFD)
     echo "dirfd"
     ;;
    FUNC_FPATHCONF)
     echo "fpathconf"
     ;;
    FUNC_PATHCONF)
     echo "pathconf"
     ;;
    FUNC_SYSCONF)
     echo "sysconf"
     ;;
    FUNC_GETPAGESIZE)
     echo "getpagesize"
     ;;
    FUNC_GETRLIMIT)
     echo "getrlimit"
     ;;
    FUNC_NANOSLEEP)
     echo "nanosleep"
     ;;
    FUNC_USLEEP)
     echo "usleep"
     ;;
    FUNC_GETTIMEOFDAY)
     echo "gettimeofday"
     ;;
    FUNC_CLOCK_GETTIME)
     echo "clock_gettime"
     ;;
    FUNC_PTHREAD_DELAY_NP)
     echo "pthread_delay_np"
     ;;
    FUNC_PTHREAD_MUTEX_TIMEDLOCK)
     echo "pthread_mutex_timedlock"
     ;;
    FUNC_PTHREAD_RWLOCK_TIMEDRDLOCK)
     echo "pthread_rwlock_timedrdlock"
     ;;
    FUNC_PTHREAD_RWLOCK_TIMEDWRLOCK)
     echo "pthread_rwlock_timedwrlock"
     ;;
    FUNC_PTHREAD_YIELD)
     echo "pthread_yield"
     ;;
    FUNC_SCHED_YIELD)
     echo "sched_yield"
     ;;
    FUNC_GETPWNAM_R)
     echo "getpwnam_r"
     ;;
    FUNC_GETGRNAM_R)
     echo "getgrnam_r"
     ;;
    FUNC_GETGRGID_R)
     echo "getgrgid_r"
     ;;
    FUNC_READDIR_R)
     echo "readdir_r"
     ;;
    FUNC_GETHOSTBYNAME_R)
     echo "gethostbyname_r"
     ;;
    FUNC_GETHOSTBYADDR_R)
     echo "gethostbyaddr_r"
     ;;
    FUNC_GETNAMEINFO)
     echo "getnameinfo"
     ;;
    FUNC_GETADDRINFO)
     echo "getaddrinfo"
     ;;
    FUNC_GETIPNODEBYNAME)
     echo "getipnodebyname"
     ;;
    FUNC_GETIPNODEBYADDR)
     echo "getipnodebyaddr"
     ;;
    FUNC_LOCALTIME_R)
     echo "localtime_r"
     ;;
    VAR_HERRNO)
     echo "herrno"
     ;;
    VAR_PATH_MAX)
     echo "PATH_MAX"
     ;;
    VAR_NAME_MAX)
     echo "NAME_MAX"
     ;;
    VAR_PAGE_SIZE)
     echo "PAGE_SIZE"
     ;;
    VAR_PAGESIZE)
     echo "PAGESIZE"
     ;;
    VAR_BIOCSHDRCMPLT)
     echo "BIOCSHDRCMPLT"
     ;;
    VAR_SIOCGLIFCONF)
     echo "SIOCGLIFCONF"
     ;;
    VAR_AF_INET6)
     echo "AF_INET6"
     ;;
    VAR_SC_GETPW_R_SIZE_MAX)
     echo "SC_GETPW_R_SIZE_MAX"
     ;;
    VAR_SC_GETGR_R_SIZE_MAX)
     echo "SC_GETGR_R_SIZE_MAX"
     ;;
    *)
     echo "Not found : $var" 1>&2
     exit
     ;;
  esac
}

##
inctosearch | while read name
do
  echo "==$name=="
  f=`searchstring $name`
  f="/usr/include/$f"
  if [ -f $f ] ; then
    echo "Found"
  fi
done

##
nametogrep | while read name
do
  echo "==$name=="
  s=`searchstring $name`
  ${PROG_GREP} $s /usr/include/*.h
  ${PROG_GREP} $s /usr/include/*/*.h
  ${PROG_GREP} $s /usr/include/*/*/*.h
done
