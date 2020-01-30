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
#include "priv/mutex.c"
#include "priv/errmsg.c"
#include "priv/program_exit.c"
#include "priv/glovars.c"
#include "priv/rand.c"
#include "priv/fd.c"
#include "priv/stream.c"
#include "priv/err.c"
#include "priv/notify.c"
#include "priv/stat.c"
#include "priv/dir.c"
#include "priv/io.c"
#include "priv/cmdline.c"
#include "priv/time.c"
#include "priv/pause.c"
#include "priv/kbd.c"

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
 #include "priv/right.c"
#elif defined NETWIBDEF_SYSNAME_Windows
 #include "priv/handle.c"
 #include "priv/winsock.c"
 #include "priv/dll.c"
 #include "priv/dll_iphlpapi.c"
 #include "priv/dll_winsock2.c"
 #include "priv/dll_snmp.c"
 #include "priv/dll_kernel32.c"
 #include "priv/dll_packet.c"
 #include "priv/dll_winpcap.c"
 #if 0
  /* currently unused */
  #include "priv/dll_rasapi32.c"
  #include "priv/winreg.c"
 #endif
 #include "priv/cs.c"
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Solaris
 #include "priv/dlpi.c"
#endif

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
  /* this function is not needed */
#elif defined NETWIBDEF_SYSNAME_Windows
BOOL WINAPI DllMain(HINSTANCE hinstDLL,
                    DWORD fdwReason,
                    LPVOID lpReserved)
{
  switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
      break;
    case DLL_THREAD_ATTACH:
      break;
    case DLL_THREAD_DETACH:
      break;
    case DLL_PROCESS_DETACH:
      break;
  }
  hinstDLL = hinstDLL; /* for compiler warning */
  lpReserved = lpReserved; /* for compiler warning */
  return TRUE;
}
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif
