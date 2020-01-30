/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_err_purge(void)
{
  netwib_er(netwib_priv_errmsg_reinit());

  errno = 0;
#if defined NETWIBDEF_SYSNAME_Unix
 #if NETWIBDEF_HAVEVAR_HERRNO==1
  h_errno = 0;
 #endif
#elif defined NETWIBDEF_SYSNAME_Windows
  WSASetLastError(0);
  SetLastError(0);
#else
#error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_err_text(netwib_err error,
                                       netwib_conststring *pc,
                                       netwib_conststring *pcfu)
{

  *pc = NULL;
  *pcfu = NULL;

  switch(error) {

  /**/
  case NETWIB_ERR_OK:
    *pc = "ok";
    break;

  /**/
  case NETWIB_ERR_DATAEND:
    *pc = "definitive end of data";
    break;
  case NETWIB_ERR_DATANOTAVAIL:
    *pc = "there is no data available";
    break;
  case NETWIB_ERR_DATANOSPACE:
    *pc = "no sufficient space to store data";
    break;
  case NETWIB_ERR_DATAOTHERTYPE:
    *pc = "data is present but does not have the expected type";
    break;
  case NETWIB_ERR_DATAMISSING:
    *pc = "more data is needed";
    break;
  case NETWIB_ERR_NOTFOUND:
    *pc = "not found";
    break;
  case NETWIB_ERR_NOTCONVERTED:
    *pc = "not converted";
    break;
  case NETWIB_ERR_ROUTELOCALDEV:
    *pc = "the destination is a local device";
    break;
  case NETWIB_ERR_ROUTENOTFOUND:
    *pc = "no route found to destination";
    break;
  case NETWIB_ERR_ROUTENOTETHER:
    *pc = "Ethernet is not used to reach destination";
    break;
  case NETWIB_ERR_PLEASETRYNEXT:
    *pc = "please, try next io";
    break;
  case NETWIB_ERR_PLEASECONSTRUCT:
    *pc = "please, construct it";
    break;
  case NETWIB_ERR_PLEASELOOPTIME:
    *pc = "please, loop to reach time";
    break;

  /**/
  case NETWIB_ERR_PAINVALIDTYPE:
    *pc = "invalid type";
    break;
  case NETWIB_ERR_PAINVALIDPOS:
    *pc = "invalid position";
    break;
  case NETWIB_ERR_PAINVALIDRANGE:
    *pc = "invalid range";
    break;
  case NETWIB_ERR_PAINVALIDDEFAULT:
    *pc = "invalid default value";
    break;
  case NETWIB_ERR_PANULLPTR:
    *pc = "a NULL pointer is not allowed";
    break;
  case NETWIB_ERR_PANULLPTRSIZE:
    *pc = "pointer is NULL, but size isn't zero";
    break;
  case NETWIB_ERR_PATOOLOW:
    *pc = "parameter is too low";
    break;
  case NETWIB_ERR_PATOOHIGH:
    *pc = "parameter is too high";
    break;
  case NETWIB_ERR_PAHEXACHAR:
    *pc = "an hexa string must not contain a character different from A-Fa-f0-9";
    break;
  case NETWIB_ERR_PAHEXAODD:
    *pc = "the byte number in the hexa string is odd";
    break;
  case NETWIB_ERR_PAMIXED:
    *pc = "the mixed string doesn't finish by '";
    break;
  case NETWIB_ERR_PABASE64CHAR:
    *pc = "all characters in a base64 must be in A-Za-z0-9+/=";
    break;
  case NETWIB_ERR_PABASE64X4:
    *pc = "with base64, the byte number must be a multiple of 4";
    break;
  case NETWIB_ERR_PABASE64LO:
    *pc = "base64 string is badly structured";
    break;
  case NETWIB_ERR_PAFMT:
    *pc = "the format is incorrect";
    break;
  case NETWIB_ERR_PAINT:
    *pc = "the string is not an integer";
    break;
  case NETWIB_ERR_PARINGFULL:
    *pc = "the ring cannot have more than 0x7FFFFFFF elements";
    break;
  case NETWIB_ERR_PATLVINVALID:
    *pc = "invalid TLV";
    break;
  case NETWIB_ERR_PATIMEDIFFNEG:
    *pc = "time difference is negative";
    break;
  case NETWIB_ERR_PAPATHROOTDOTDOT:
    *pc = "too many .. in the path : go up /";
    break;
  case NETWIB_ERR_PAPATHNOTCANON:
    *pc = "the path could not be canonized";
    break;
  case NETWIB_ERR_PAPATHCANTINIT:
    *pc = "it's not possible to init a path from the two given pathnames";
    break;
  case NETWIB_ERR_PAFILE2G:
    *pc = "file size greater than 2Gbytes";
    break;
  case NETWIB_ERR_PAFILENOTREG:
    *pc = "file is not regular";
    break;
  case NETWIB_ERR_PAINDEXNODATA:
    *pc = "index does not point to data (index is just rewinded, or item was deleted)";
    break;
  case NETWIB_ERR_PATOOBIGFORHDR:
    *pc = "a parameter is too big to go in the physical header";
    break;
  case NETWIB_ERR_PAIP4OPTSNOTX4:
    *pc = "the ip options size is not a multiple of 4";
    break;
  case NETWIB_ERR_PAIP4OPTSMAX10:
    *pc = "you cannot have more than 10 lines of ip options";
    break;
  case NETWIB_ERR_PAIP6EXTSNOTX4:
    *pc = "the ip extensions size is not a multiple of 4";
    break;
  case NETWIB_ERR_PATCPOPTSNOTX4:
    *pc = "the tcp options size is not a multiple of 4";
    break;
  case NETWIB_ERR_PATCPOPTSMAX10:
    *pc = "you cannot have more than 10 lines of tcp options";
    break;
  case NETWIB_ERR_PAIPTYPE:
    *pc = "bad IP type";
    break;
  case NETWIB_ERR_PAIPTYPENOT4:
    *pc = "IP type is not IPv4";
    break;
  case NETWIB_ERR_PAIPTYPENOT6:
    *pc = "IP type is not IPv6";
    break;

  /**/
  case NETWIB_ERR_LOINTERNALERROR:
    *pc = "internal error : contact Laurent";
    break;
  case NETWIB_ERR_LONOTIMPLEMENTED:
    *pc = "not implemented";
    break;
  case NETWIB_ERR_LONOTSUPPORTED:
    *pc = "not supported";
    break;
  case NETWIB_ERR_LOERROROKKO:
    *pc = "NETWIB_ERR_OK is returned by the function, but this should not happen";
    break;
  case NETWIB_ERR_LOOBJINITALREADYINIT:
    *pc = "already initialized";
    break;
  case NETWIB_ERR_LOOBJUSENOTINITIALIZED:
    *pc = "use of an object not initialized";
    break;
  case NETWIB_ERR_LOOBJUSECLOSED:
    *pc = "use of a closed object";
    break;
  case NETWIB_ERR_LOOBJUSECORRUPT:
    *pc = "use of a corrupted object";
    break;
  case NETWIB_ERR_LOOBJCLOSENOTINITIALIZED:
    *pc = "closing an object not initialized";
    break;
  case NETWIB_ERR_LOOBJCLOSEALREADYCLOSED:
    *pc = "already closed";
    break;
  case NETWIB_ERR_LOOBJCLOSECORRUPT:
    *pc = "closing a corrupted object";
    break;
  case NETWIB_ERR_LOOBJREADNOTSUPPORTED:
    *pc = "reading is not supported";
    break;
  case NETWIB_ERR_LOOBJREADNOTALLOWED:
    *pc = "reading is not allowed";
    break;
  case NETWIB_ERR_LOOBJWRITENOTSUPPORTED:
    *pc = "writing is not supported";
    break;
  case NETWIB_ERR_LOOBJWRITENOTALLOWED:
    *pc = "writing is not allowed";
    break;
  case NETWIB_ERR_LOOBJRDWRCONFLICT:
    *pc = "reading and writing return different values";
    break;
  case NETWIB_ERR_LOCANTEXEC:
    *pc = "cannot execute";
    break;
  case NETWIB_ERR_LOUDPSERREADBEFWRITE:
    *pc = "it is not possible to write before reading on a udp server";
    break;

  /* functions */
  case NETWIB_ERR_FUACCEPT: *pcfu = "accept"; break;
  case NETWIB_ERR_FUBIND: *pcfu = "bind"; break;
  case NETWIB_ERR_FUCHDIR: *pcfu = "chdir"; break;
  case NETWIB_ERR_FUCHMOD: *pcfu = "chmod"; break;
  case NETWIB_ERR_FUCHOWN: *pcfu = "chown"; break;
  case NETWIB_ERR_FUCLOCKGETTIME: *pcfu = "chsize"; break;
  case NETWIB_ERR_FUCLOSEDIR: *pcfu = "closedir"; break;
  case NETWIB_ERR_FUCLOSEHANDLE: *pcfu = "CloseHandle"; break;
  case NETWIB_ERR_FUCLOSE: *pcfu = "close"; break;
  case NETWIB_ERR_FUCONNECT: *pcfu = "connect"; break;
  case NETWIB_ERR_FUCREATEEVENT: *pcfu = "CreateEvent"; break;
  case NETWIB_ERR_FUCREATEFILE: *pcfu = "CreateFile"; break;
  case NETWIB_ERR_FUCREATEMUTEX: *pcfu = "CreateMutex"; break;
  case NETWIB_ERR_FUCREATEPIPE: *pcfu = "CreatePipe"; break;
  case NETWIB_ERR_FUCREATETHREAD: *pcfu = "CreateThread"; break;
  case NETWIB_ERR_FUDUP2: *pcfu = "dup2"; break;
  case NETWIB_ERR_FUDUPLICATEHANDLE: *pcfu = "DuplicateHandle"; break;
  case NETWIB_ERR_FUEXECLE: *pcfu = "execle"; break;
  case NETWIB_ERR_FUFCLOSE: *pcfu = "fclose"; break;
  case NETWIB_ERR_FUFCNTL: *pcfu = "fcntl"; break;
  case NETWIB_ERR_FUFFLUSH: *pcfu = "fflush"; break;
  case NETWIB_ERR_FUFILENO: *pcfu = "fileno"; break;
  case NETWIB_ERR_FUFINDCLOSE: *pcfu = "findclose"; break;
  case NETWIB_ERR_FUFOPEN: *pcfu = "fopen"; break;
  case NETWIB_ERR_FUFORK: *pcfu = "fork"; break;
  case NETWIB_ERR_FUFREELIBRARY: *pcfu = "FreeLibrary"; break;
  case NETWIB_ERR_FUFREAD: *pcfu = "fread"; break;
  case NETWIB_ERR_FUFSYNC: *pcfu = "fsync"; break;
  case NETWIB_ERR_FUFTRUNCATE: *pcfu = "ftruncate"; break;
  case NETWIB_ERR_FUFWRITE: *pcfu = "fwrite"; break;
  case NETWIB_ERR_FUGENERATECONSOLECTRLEVENT: *pcfu = "GenerateConsoleCtrlEvent"; break;
  case NETWIB_ERR_FUGETCONSOLEMODE: *pcfu = "GetConsoleMode"; break;
  case NETWIB_ERR_FUGETEXITCODEPROCESS: *pcfu = "GetExitCodeProcess"; break;
  case NETWIB_ERR_FUGETEXITCODETHREAD: *pcfu = "GetExitCodeThread"; break;
  case NETWIB_ERR_FUGETGRGIDR: *pcfu = "getgrgid_r"; break;
  case NETWIB_ERR_FUGETGRNAMR: *pcfu = "getgrnam_r"; break;
  case NETWIB_ERR_FUGETIFTABLE: *pcfu = "GetIfTable"; break;
  case NETWIB_ERR_FUGETIPADDRTABLE: *pcfu = "GetIpAddrTable"; break;
  case NETWIB_ERR_FUGETIPFORWARDTABLE: *pcfu = "GetIpForwardTable"; break;
  case NETWIB_ERR_FUGETIPNETTABLE: *pcfu = "GetIpNetTable"; break;
  case NETWIB_ERR_FUGETMSG: *pcfu = "getmsg"; break;
  case NETWIB_ERR_FUGETPEERNAME: *pcfu = "getpeername"; break;
  case NETWIB_ERR_FUGETPROCADDRESS: *pcfu = "GetProcAddress"; break;
  case NETWIB_ERR_FUGETRLIMIT: *pcfu = "getrlimit"; break;
  case NETWIB_ERR_FUGETPWNAMR: *pcfu = "getpwnam_r"; break;
  case NETWIB_ERR_FUGETPWUID: *pcfu = "getpwuid"; break;
  case NETWIB_ERR_FUGETSOCKNAME: *pcfu = "getsockname"; break;
  case NETWIB_ERR_FUGETTIMEOFDAY: *pcfu = "gettimeofday"; break;
  case NETWIB_ERR_FUGETUID: *pcfu = "getuid"; break;
  case NETWIB_ERR_FUGLOBALALLOC: *pcfu = "GlobalAlloc"; break;
  case NETWIB_ERR_FUGLOBALFREE: *pcfu = "GlobalFree"; break;
  case NETWIB_ERR_FUIOCTL: *pcfu = "ioctl"; break;
  case NETWIB_ERR_FUKILL: *pcfu = "kill"; break;
  case NETWIB_ERR_FULIBNETINIT: *pcfu = "libnet_init"; break;
  case NETWIB_ERR_FULIBNETCLOSELINKINT: *pcfu = "libnet_close_link_interface"; break;
  case NETWIB_ERR_FULIBNETCLOSERAWSOCK: *pcfu = "libnet_close_raw_sock"; break;
  case NETWIB_ERR_FULIBNETOPENRAWSOCK: *pcfu = "libnet_open_raw_sock"; break;
  case NETWIB_ERR_FULIBNETWRITEIP: *pcfu = "libnet_write_ip"; break;
  case NETWIB_ERR_FULIBNETWRITELINK: *pcfu = "libnet_write_link"; break;
  case NETWIB_ERR_FULIBNETWRITELL: *pcfu = "libnet_write_link_layer"; break;
  case NETWIB_ERR_FULIBNETWRITERAWIPV4: *pcfu = "libnet_write_raw_ipv4"; break;
  case NETWIB_ERR_FULIBNETWRITERAWIPV6: *pcfu = "libnet_write_raw_ipv6"; break;
  case NETWIB_ERR_FULISTEN: *pcfu = "listen"; break;
  case NETWIB_ERR_FULOADLIBRARY: *pcfu = "LoadLibrary"; break;
  case NETWIB_ERR_FULOCALTIMER: *pcfu = "localtime_r"; break;
  case NETWIB_ERR_FULSEEK: *pcfu = "lseek"; break;
  case NETWIB_ERR_FULSTAT: *pcfu = "lstat"; break;
  case NETWIB_ERR_FUMALLOC: *pcfu = "malloc"; break;
  case NETWIB_ERR_FUMKDIR: *pcfu = "mkdir"; break;
  case NETWIB_ERR_FUMPROTECT: *pcfu = "mprotect"; break;
  case NETWIB_ERR_FUNANOSLEEP: *pcfu = "nanosleep"; break;
  case NETWIB_ERR_FUOPEN: *pcfu = "open"; break;
  case NETWIB_ERR_FUOPENDIR: *pcfu = "opendir"; break;
  case NETWIB_ERR_FUOPENPROCESS: *pcfu = "OpenProcess"; break;
  case NETWIB_ERR_FUOPENPTY: *pcfu = "openpty"; break;
  case NETWIB_ERR_FUPACKETALLOCATEPACKET: *pcfu = "PacketAllocatePacket"; break;
  case NETWIB_ERR_FUPACKETGETNETTYPE: *pcfu = "PacketGetNetType"; break;
  case NETWIB_ERR_FUPACKETOPENADAPTER: *pcfu = "PacketOpenAdapter"; break;
  case NETWIB_ERR_FUPACKETREQUEST: *pcfu = "PacketRequest"; break;
  case NETWIB_ERR_FUPACKETSENDPACKET: *pcfu = "PacketSendPacket"; break;
  case NETWIB_ERR_FUPACKETSETBUFF: *pcfu = "PacketSetBuff"; break;
  case NETWIB_ERR_FUPCAPCOMPILE: *pcfu = "pcap_compile"; break;
  case NETWIB_ERR_FUPCAPDUMPOPEN: *pcfu = "pcap_dump_open"; break;
  case NETWIB_ERR_FUPCAPFINDALLDEVS: *pcfu = "pcap_findalldevs"; break;
  case NETWIB_ERR_FUPCAPGETEVENT: *pcfu = "pcap_getevent"; break;
  case NETWIB_ERR_FUPCAPOPENDEAD: *pcfu = "pcap_open_dead"; break;
  case NETWIB_ERR_FUPCAPOPENOFFLINE: *pcfu = "pcap_open_offline"; break;
  case NETWIB_ERR_FUPCAPSETFILTER: *pcfu = "pcap_setfilter"; break;
  case NETWIB_ERR_FUPEEKCONSOLEINPUT: *pcfu = "PeekConsoleInput"; break;
  case NETWIB_ERR_FUPIPE: *pcfu = "pipe"; break;
  case NETWIB_ERR_FUPOLL: *pcfu = "poll"; break;
  case NETWIB_ERR_FUPTHREADATTRINIT: *pcfu = "pthread_attr_init"; break;
  case NETWIB_ERR_FUPTHREADATTRDESTROY: *pcfu = "pthread_attr_destroy"; break;
  case NETWIB_ERR_FUPTHREADATTRSETDETACHSTATE: *pcfu = "pthread_attr_setdetachstate"; break;
  case NETWIB_ERR_FUPTHREADCONDINIT: *pcfu = "pthread_cond_init"; break;
  case NETWIB_ERR_FUPTHREADCONDBROADCAST: *pcfu = "pthread_cond_broadcast"; break;
  case NETWIB_ERR_FUPTHREADCONDDESTROY: *pcfu = "pthread_cond_destroy"; break;
  case NETWIB_ERR_FUPTHREADCONDTIMEDWAIT: *pcfu = "pthread_cond_timedwait"; break;
  case NETWIB_ERR_FUPTHREADCONDWAIT: *pcfu = "pthread_cond_wait"; break;
  case NETWIB_ERR_FUPTHREADCREATE: *pcfu = "pthread_create"; break;
  case NETWIB_ERR_FUPTHREADDELAYNP: *pcfu = "pthread_delay_np"; break;
  case NETWIB_ERR_FUPTHREADGETEXPIRATIONNP: *pcfu = "pthread_get_expiration_np"; break;
  case NETWIB_ERR_FUPTHREADGETSPECIFIC: *pcfu = "pthread_getspecific"; break;
  case NETWIB_ERR_FUPTHREADJOIN: *pcfu = "pthread_join"; break;
  case NETWIB_ERR_FUPTHREADKEYCREATE: *pcfu = "pthread_key_create"; break;
  case NETWIB_ERR_FUPTHREADKEYDELETE: *pcfu = "pthread_key_delete"; break;
  case NETWIB_ERR_FUPTHREADMUTEXINIT: *pcfu = "pthread_mutex_init"; break;
  case NETWIB_ERR_FUPTHREADMUTEXDESTROY: *pcfu = "pthread_mutex_destroy"; break;
  case NETWIB_ERR_FUPTHREADMUTEXLOCK: *pcfu = "pthread_mutex_lock"; break;
  case NETWIB_ERR_FUPTHREADMUTEXTIMEDLOCK: *pcfu = "pthread_mutex_timedlock"; break;
  case NETWIB_ERR_FUPTHREADMUTEXTRYLOCK: *pcfu = "pthread_mutex_trylock"; break;
  case NETWIB_ERR_FUPTHREADMUTEXUNLOCK: *pcfu = "pthread_mutex_unlock"; break;
  case NETWIB_ERR_FUPTHREADRWLOCKINIT: *pcfu = "pthread_rwlock_init"; break;
  case NETWIB_ERR_FUPTHREADRWLOCKDESTROY: *pcfu = "pthread_rwlock_destroy"; break;
  case NETWIB_ERR_FUPTHREADRWLOCKRDLOCK: *pcfu = "pthread_rwlock_rdlock"; break;
  case NETWIB_ERR_FUPTHREADRWLOCKWRLOCK: *pcfu = "pthread_rwlock_wrlock"; break;
  case NETWIB_ERR_FUPTHREADRWLOCKTIMEDRDLOCK: *pcfu = "pthread_rwlock_timedrdlock"; break;
  case NETWIB_ERR_FUPTHREADRWLOCKTIMEDWRLOCK: *pcfu = "pthread_rwlock_timedwrlock"; break;
  case NETWIB_ERR_FUPTHREADRWLOCKTRYRDLOCK: *pcfu = "pthread_rwlock_tryrdlock"; break;
  case NETWIB_ERR_FUPTHREADRWLOCKTRYWRLOCK: *pcfu = "pthread_rwlock_trywrlock"; break;
  case NETWIB_ERR_FUPTHREADRWLOCKUNLOCK: *pcfu = "pthread_rwlock_unlock"; break;
  case NETWIB_ERR_FUPTHREADSETSPECIFIC: *pcfu = "pthread_setspecific"; break;
  case NETWIB_ERR_FUPUTMSG: *pcfu = "putmsg"; break;
  case NETWIB_ERR_FUREADDIRR: *pcfu = "readdir_r"; break;
  case NETWIB_ERR_FUREADLINK: *pcfu = "readlink"; break;
  case NETWIB_ERR_FUREAD: *pcfu = "read"; break;
  case NETWIB_ERR_FUREALLOC: *pcfu = "realloc"; break;
  case NETWIB_ERR_FURECV: *pcfu = "recv"; break;
  case NETWIB_ERR_FURECVFROM: *pcfu = "recvfrom"; break;
  case NETWIB_ERR_FUREGCLOSEKEY: *pcfu = "RegCloseEx"; break;
  case NETWIB_ERR_FUREGCOMP: *pcfu = "regcomp"; break;
  case NETWIB_ERR_FUREGENUMKEY: *pcfu = "RegEnumKey"; break;
  case NETWIB_ERR_FUREGENUMVALUE: *pcfu = "RegEnumValue"; break;
  case NETWIB_ERR_FUREGOPENKEYEX: *pcfu = "RegOpenKeyEx"; break;
  case NETWIB_ERR_FUREGQUERYINFOKEY: *pcfu = "RegQueryInfoKey"; break;
  case NETWIB_ERR_FUREGQUERYVALUEEX: *pcfu = "RegQueryValueEx"; break;
  case NETWIB_ERR_FURELEASEMUTEX: *pcfu = "ReleaseMutex"; break;
  case NETWIB_ERR_FURENAME: *pcfu = "rename"; break;
  case NETWIB_ERR_FURESETEVENT: *pcfu = "ResetEvent"; break;
  case NETWIB_ERR_FURMDIR: *pcfu = "rmdir"; break;
  case NETWIB_ERR_FUSELECT: *pcfu = "select"; break;
  case NETWIB_ERR_FUSEND: *pcfu = "send"; break;
  case NETWIB_ERR_FUSENDTO: *pcfu = "sendto"; break;
  case NETWIB_ERR_FUSETEVENT: *pcfu = "SetEvent"; break;
  case NETWIB_ERR_FUSETGID: *pcfu = "setgid"; break;
  case NETWIB_ERR_FUSETGROUPS: *pcfu = "setgroups"; break;
  case NETWIB_ERR_FUSETSID: *pcfu = "setsid"; break;
  case NETWIB_ERR_FUSETUID: *pcfu = "setuid"; break;
  case NETWIB_ERR_FUSETSOCKOPT: *pcfu = "setsockopt"; break;
  case NETWIB_ERR_FUSIGNAL: *pcfu = "signal"; break;
  case NETWIB_ERR_FUSNMPEXTENSIONINIT: *pcfu = "SnmpExtensionInit"; break;
  case NETWIB_ERR_FUSNMPEXTENSIONQUERY: *pcfu = "SnmpExtensionQuery"; break;
  case NETWIB_ERR_FUSOCKET: *pcfu = "socket"; break;
  case NETWIB_ERR_FUSYMLINK: *pcfu = "symlink"; break;
  case NETWIB_ERR_FUSYSCTL: *pcfu = "sysctl"; break;
  case NETWIB_ERR_FUTCGETATTR: *pcfu = "tcgetattr"; break;
  case NETWIB_ERR_FUTCSETATTR: *pcfu = "tcsetattr"; break;
  case NETWIB_ERR_FUTERMINATEPROCESS: *pcfu = "TerminateProcess"; break;
  case NETWIB_ERR_FUTLSALLOC: *pcfu = "PeekNamedPipe"; break;
  case NETWIB_ERR_FUTLSFREE: *pcfu = "TlsFree"; break;
  case NETWIB_ERR_FUTLSSETVALUE: *pcfu = "TlsSetValue"; break;
  case NETWIB_ERR_FUUNLINK: *pcfu = "unlink"; break;
  case NETWIB_ERR_FUVIRTUALALLOC: *pcfu = "VirtualAlloc"; break;
  case NETWIB_ERR_FUVIRTUALFREE: *pcfu = "VirtualFree"; break;
  case NETWIB_ERR_FUVIRTUALPROTECT: *pcfu = "VirtualProtect"; break;
  case NETWIB_ERR_FUWAITFORSINGLEOBJECT: *pcfu = "WaitForSingleObject"; break;
  case NETWIB_ERR_FUWAITPID: *pcfu = "waitpid"; break;
  case NETWIB_ERR_FUWRITEFILE: *pcfu = "WriteFile"; break;
  case NETWIB_ERR_FUWRITE: *pcfu = "write"; break;
  case NETWIB_ERR_FUWSACLEANUP: *pcfu = "WSACleanup"; break;
  case NETWIB_ERR_FUWSASTARTUP: *pcfu = "WSAStartup"; break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_err_append_herrno(netwib_uint32 varherrno,
                                                netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_fmt(pbuf, " hint: h_errno = %{uint32}",
                                  varherrno));

#if defined NETWIBDEF_SYSNAME_Unix
 #if NETWIBDEF_HAVEFUNC_HSTRERROR==1
  netwib_er(netwib_buf_append_string(" = ", pbuf));
  netwib_er(netwib_buf_append_string(hstrerror(varherrno), pbuf));
 #endif
#elif defined NETWIBDEF_SYSNAME_Windows
  /* there is no portable way to get Winsock error messages,
     so we do it here */
  {
    netwib_conststring pc;
    pc = NULL;
    switch (varherrno) {
      case WSAEINTR:
        pc = "Interrupted system call";
        break;
      case WSAEBADF:
        pc = "Bad file number";
        break;
      case WSAEACCES:
        pc = "Permission denied";
        break;
      case WSAEFAULT:
        pc = "Bad address";
        break;
      case WSAEINVAL:
        pc = "Invalid argument";
        break;
      case WSAEMFILE:
        pc = "Too many open sockets";
        break;
      case WSAEWOULDBLOCK:
        pc = "Operation would block";
        break;
      case WSAEINPROGRESS:
        pc = "Operation now in progress";
        break;
      case WSAEALREADY:
        pc = "Operation already in progress";
        break;
      case WSAENOTSOCK:
        pc = "Socket operation on non-socket";
        break;
      case WSAEDESTADDRREQ:
        pc = "Destination address required";
        break;
      case WSAEMSGSIZE:
        pc = "Message too long";
        break;
      case WSAEPROTOTYPE:
        pc = "Protocol wrong type for socket";
        break;
      case WSAENOPROTOOPT:
        pc = "Bad protocol option";
        break;
      case WSAEPROTONOSUPPORT:
        pc = "Protocol not supported";
        break;
      case WSAESOCKTNOSUPPORT:
        pc = "Socket type not supported";
        break;
      case WSAEOPNOTSUPP:
        pc = "Operation not supported on socket";
        break;
      case WSAEPFNOSUPPORT:
        pc = "Protocol family not supported";
        break;
      case WSAEAFNOSUPPORT:
        pc = "Address family not supported";
        break;
      case WSAEADDRINUSE:
        pc = "Address already in use";
        break;
      case WSAEADDRNOTAVAIL:
        pc = "Can't assign requested address";
        break;
      case WSAENETDOWN:
        pc = "Network is down";
        break;
      case WSAENETUNREACH:
        pc = "Network is unreachable";
        break;
      case WSAENETRESET:
        pc = "Net connection reset";
        break;
      case WSAECONNABORTED:
        pc = "Software caused connection abort";
        break;
      case WSAECONNRESET:
        pc = "Connection reset by peer";
        break;
      case WSAENOBUFS:
        pc = "No buffer space available";
        break;
      case WSAEISCONN:
        pc = "Socket is already connected";
        break;
      case WSAENOTCONN:
        pc = "Socket is not connected";
        break;
      case WSAESHUTDOWN:
        pc = "Can't send after socket shutdown";
        break;
      case WSAETOOMANYREFS:
        pc = "Too many references, can't splice";
        break;
      case WSAETIMEDOUT:
        pc = "Connection timed out";
        break;
      case WSAECONNREFUSED:
        pc = "Connection refused";
        break;
      case WSAELOOP:
        pc = "Too many levels of symbolic links";
        break;
      case WSAENAMETOOLONG:
        pc = "File name too long";
        break;
      case WSAEHOSTDOWN:
        pc = "Host is down";
        break;
      case WSAEHOSTUNREACH:
        pc = "No route to host";
        break;
      case WSAENOTEMPTY:
        pc = "Directory not empty";
        break;
      case WSAEPROCLIM:
        pc = "Too many processes";
        break;
      case WSAEUSERS:
        pc = "Too many users";
        break;
      case WSAEDQUOT:
        pc = "Disc quota exceeded";
        break;
      case WSAESTALE:
        pc = "Stale NFS file handle";
        break;
      case WSAEREMOTE:
        pc = "Too many levels of remote in path";
        break;
      case WSASYSNOTREADY:
        pc = "Network system is unavailable";
        break;
      case WSAVERNOTSUPPORTED:
        pc = "Winsock DLL version out of range";
        break;
      case WSANOTINITIALISED:
        pc = "WSAStartup not yet called";
        break;
      case WSAEDISCON:
        pc = "Graceful shutdown in progress";
        break;
      case WSAHOST_NOT_FOUND:
        pc = "Host not found";
        break;
      case WSATRY_AGAIN:
        pc = "Non-Authoritative Host not found";
        break;
      case WSANO_RECOVERY:
        pc = "Non-Recoverable errors: FORMERR, REFUSED, NOTIMP";
        break;
      case WSANO_DATA:
        pc = "No host data of that type was found";
        break;
    }
    if (pc != NULL) {
      netwib_er(netwib_buf_append_string(" = ", pbuf));
      netwib_er(netwib_buf_append_string(pc, pbuf));
    }
  }
#else
#error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  netwib_er(netwib_buf_append_string("\n", pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_err_syserrors(netwib_uint32 *perrno,
                                     netwib_uint32 *pherrno,
                                     netwib_uint32 *pgetlasterror)
{

  *perrno = errno;
#if NETWIBDEF_HAVEVAR_HERRNO==1
  *pherrno = h_errno;
#else
  *pherrno = 0;
#endif
#if defined NETWIBDEF_SYSNAME_Unix
  *pgetlasterror = 0;
#elif defined NETWIBDEF_SYSNAME_Windows
  *pgetlasterror = GetLastError();
  /* we do not use WSAGetLastError, because it is equivalent to h_errno */
#else
#error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_err_append_err2(netwib_buf *pbuf)
{
  netwib_string pc;
  netwib_priv_glovars_t *ptr = &netwib_priv_glovars;

  netwib_er(netwib_buf_ref_string(&(ptr->errmsg), &pc));
  if (pc[0] != '\0') {
    netwib_er(netwib_buf_append_string(" hint: ", pbuf));
    netwib_er(netwib_buf_append_string(pc, pbuf));
    netwib_er(netwib_buf_append_string("\n", pbuf));
  }

  return(NETWIB_ERR_OK);
}
netwib_err netwib_priv_err_append_err(netwib_err error,
                                      netwib_uint32 varerrno,
                                      netwib_uint32 varherrno,
                                      netwib_uint32 vargetlasterror,
                                      netwib_err_encodetype encodetype,
                                      netwib_buf *pbuf)
{
  netwib_conststring perrstr, perrstrfu;
  netwib_bool canuseglo;
  netwib_err ret;

  /* "Error xyz : " */
  if (encodetype == NETWIB_ERR_ENCODETYPE_NUMTEXT ||
      encodetype == NETWIB_ERR_ENCODETYPE_FULL) {
    netwib_er(netwib_buf_append_fmt(pbuf, "Error %{uint32} : ", error));
  }

  /* error string */
  netwib_er(netwib_priv_err_text(error, &perrstr, &perrstrfu));
  if (perrstr == NULL) {
    if (perrstrfu == NULL) {
      netwib_er(netwib_buf_append_string("unknown error", pbuf));
    } else {
      netwib_er(netwib_buf_append_string("error in ", pbuf));
      netwib_er(netwib_buf_append_string(perrstrfu, pbuf));
      netwib_er(netwib_buf_append_string("()", pbuf));
    }
  } else {
    netwib_er(netwib_buf_append_string(perrstr, pbuf));
  }

  if (encodetype != NETWIB_ERR_ENCODETYPE_FULL) {
    /* stop here */
    return(NETWIB_ERR_OK);
  }
  netwib_er(netwib_buf_append_string("\n", pbuf));

  /* errno */
  if (varerrno) {
    netwib_er(netwib_buf_append_fmt(pbuf, " hint: errno = %{uint32} = %s\n",
                                    varerrno, strerror(varerrno)));
  }

  /* GetLastError */
#if defined NETWIBDEF_SYSNAME_Unix
  vargetlasterror = vargetlasterror;
#elif defined NETWIBDEF_SYSNAME_Windows
  if (vargetlasterror) {
    LPVOID lpMsgBuf;
    int bytenum;

    /* display error number */
    netwib_er(netwib_buf_append_fmt(pbuf, " hint: GetLastError = %{uint32}",
                                  vargetlasterror));
    /* try to get error string */
    bytenum = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                            FORMAT_MESSAGE_FROM_SYSTEM,
                            NULL, vargetlasterror,
                            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                            (LPTSTR) & lpMsgBuf, 0, NULL);
    if (bytenum) {
      netwib_er(netwib_buf_append_string(" = ", pbuf));
      netwib_er(netwib_buf_append_string((char *)lpMsgBuf, pbuf));
      /* use LocalFree, because it's not allocated in netwib */
      LocalFree(lpMsgBuf);
    } else {
      netwib_er(netwib_buf_append_string("\n", pbuf));
    }
  }
#else
#error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  /* h_errno */
  if (varherrno) {
    netwib_er(netwib_priv_err_append_herrno(varherrno, pbuf));
  }

  /* global error message */
  netwib_er(netwib_priv_glovars_canuse(&canuseglo));
  if (canuseglo) {
    netwib_er(netwib_priv_glovars_rdlock());
    ret = netwib_priv_err_append_err2(pbuf);
    netwib_er(netwib_priv_glovars_rdunlock());
    if (ret != NETWIB_ERR_OK) {
      return(ret);
    }
  }

  /* backtrace */
#if NETWIB_DEBUG_ERROR_BT==1
  if (error != NETWIB_ERR_OK) {
    netwib_string str;
    netwib_er(netwib_debug_backtrace_string(&str));
    netwib_er(netwib_buf_append_string(" backtrace: ", pbuf));
    netwib_er(netwib_buf_append_string(str, pbuf));
    netwib_er(netwib_buf_append_string("\n", pbuf));
    netwib_er(netwib_debug_string_close(&str));
  }
#endif

  /*get threadid */
#if NETWIB_DEBUG_ERROR_THREADID==1
  {
    netwib_uint32 threadid;

    netwib_er(netwib_debug_thread_id(&threadid));
    netwib_er(netwib_buf_append_fmt(pbuf, " thread: %{uint32}\n", threadid));
  }
#endif

  return(NETWIB_ERR_OK);
}
