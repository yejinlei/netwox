
/*-------------------------------------------------------------*/
#include "priv/errmsg.h"
#include "priv/program_exit.h"
#include "priv/glovars.h"
#include "priv/rand.h"
#include "priv/fd.h"
#include "priv/stream.h"
#include "priv/err.h"
#include "priv/notify.h"
#include "priv/dir.h"
#include "priv/io.h"
#include "priv/cmdline.h"
#include "priv/pause.h"
#include "priv/kbd.h"

/*-------------------------------------------------------------*/
#if ! defined NETWIBDEF_PRIV_DONT_WANT_SYS
 #include "priv/mutex.h"
 #include "priv/stat.h"
 #include "priv/time.h"
#endif

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
 #include "priv/right.h"
#elif defined NETWIBDEF_SYSNAME_Windows
 #include "priv/handle.h"
 #include "priv/winsock.h"
 #include "priv/dll.h"
 #include "priv/dll_iphlpapi.h"
 #include "priv/dll_winsock2.h"
 #include "priv/dll_snmp.h"
 #include "priv/dll_kernel32.h"
 #include "priv/dll_packet.h"
 #include "priv/dll_winpcap.h"
 #include "priv/cs.h"
 #if 0
  /* currently unused */
  #include "priv/dll_rasapi32.h"
  #include "priv/winreg.h"
 #endif
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Solaris
 #include "priv/dlpi.h"
#endif

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
 #define netwib_c_close(a) close(a)
 #define netwib_c_closesocket(a) close(a)
 #define netwib_c_fdopen(a,b) fdopen(a,b)
 #define netwib_c_fileno(a) fileno(a)
 #define netwib_c_lseek(a,b,c) lseek(a,b,c)
 #define netwib_c_open(a,b) open(a,b)
 #define netwib_c_open3(a,b,c) open(a,b,c)
 #define netwib_c_read(a,b,c) read(a,b,c)
 #define netwib_c_rmdir(a) rmdir(a)
 #define netwib_c_unlink(a) unlink(a)
 #define netwib_c_write(a,b,c) write(a,b,c)
#elif defined NETWIBDEF_SYSNAME_Windows
 #define netwib_c_close(a) _close(a)
 #define netwib_c_closesocket(a) closesocket(a)
 #define netwib_c_fdopen(a,b) _fdopen(a,b)
 #define netwib_c_fileno(a) _fileno(a)
 #define netwib_c_lseek(a,b,c) _lseek(a,b,c)
 #define netwib_c_open(a,b) _open(a,b)
 #define netwib_c_open3(a,b,c) _open(a,b,c)
 #define netwib_c_read(a,b,c) _read(a,b,c)
 #define netwib_c_rmdir(a) _rmdir(a)
 #define netwib_c_unlink(a) _unlink(a)
 #define netwib_c_write(a,b,c) _write(a,b,c)
#else
 #error "Unknown value for NETWIBDEF_SYSTEM"
#endif

/*-------------------------------------------------------------*/
#if NETWIBDEF_HAVETYPE_SIZE_T==1
 #define netwib_c_sizet size_t
#else
 #define netwib_c_sizet int
#endif

/*-------------------------------------------------------------*/
#if ! defined NETWIBDEF_PRIV_DONT_WANT_SYS
 #if NETWIBDEF_HAVETYPE_SIGHANDLER_T==1
  typedef sighandler_t netwib_c_sighandlert;
 #elif NETWIBDEF_HAVETYPE_SIG_T==1
  typedef sig_t netwib_c_sighandlert;
 #else
  typedef void (*netwib_c_sighandlert)(int);
 #endif
#endif

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
 #define NETWIB_PRIV_MINMSGSIZE_READ   0x1FFFF
#elif defined NETWIBDEF_SYSNAME_Windows
 /* with a bigger value, Windows doesn't have sufficient memory */
 #define NETWIB_PRIV_MINMSGSIZE_READ    0x7000
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
#define netwib_ir(i,r) {if(i<0)return(r);}
#define netwib_ig(i,r) {if(i<0)netwib_goto(r);}
#define netwib_br(b,r) {if(!b)return(r);}
#define netwib_bg(b,r) {if(!b)netwib_goto(r);}

