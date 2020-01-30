
/*-------------------------------------------------------------*/
#ifndef NETWOX_NETWOX_INCLUDED
#define NETWOX_NETWOX_INCLUDED 1

/*-------------------------------------------------------------*/
#ifdef WIN32
  #ifdef __MINGW32__
    /* mingw32 and Dev-cpp */
    #define NETWOXDEF_PROGCC_MINGW32 1
  #else
    /* Visual C++ */
    #define NETWOXDEF_PROGCC_VISUALC 1
    /* disable warning "conditional expression is constant" */
    #pragma warning(disable : 4127)
    /* decide which kind of code to generate */
    #define _WIN32_WINNT 0x0400
  #endif
#endif

/*-------------------------------------------------------------*/
/* For the moment, we only need NETWIBDEF_SYSNAME_... */
#define NETWIB_DEF_ONLY 1
#include "netwib.h"

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
#elif defined NETWIBDEF_SYSNAME_FreeBSD
 #define _POSIX_C_SOURCE 1
#elif defined NETWIBDEF_SYSNAME_MacOS
#elif defined NETWIBDEF_SYSNAME_NetBSD
#elif defined NETWIBDEF_SYSNAME_OpenBSD
#elif defined NETWIBDEF_SYSNAME_Solaris
 #define __EXTENSIONS__ 1
 #define _POSIX_PTHREAD_SEMANTICS 1
#elif defined NETWIBDEF_SYSNAME_HPUX
 #define _HPUX_SOURCE 1
#elif defined NETWIBDEF_SYSNAME_Tru64UNIX
 #define _POSIX_PII 1
 #define _POSIX_C_SOURCE 1
 /* under Tru64 version 5.1, if _POSIX_C_SOURCE is defined, _XOPEN_SOURCE
    is required because of a bug in includes */
 #define _XOPEN_SOURCE 500
#elif defined NETWIBDEF_SYSNAME_AIX
#elif defined NETWIBDEF_SYSNAME_Unsupported
#elif defined NETWIBDEF_SYSNAME_Windows
 /* nothing to include for the moment */
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#if defined NETWIBDEF_SYSNAME_Windows
 #include <windows.h>
#endif

/*-------------------------------------------------------------*/
/* now, we can fully include netwib */
#define NETWIB_EXT_FILE 1
#if defined NETWIBDEF_SYSNAME_Windows
 #define NETWIB_EXT_HANDLE 1
#endif
#include "netwib.h"

/*-------------------------------------------------------------*/
#include "def.h"

/*-------------------------------------------------------------*/
#include "modules.h"
#include "tools.h"

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
 #define NETWOX_DIREX "mydir"
 #define NETWOX_DIREX_RD "/tmp"
 #define NETWOX_DIREX_WR "/tmp/mydir2"
#elif defined NETWIBDEF_SYSNAME_Windows
 #define NETWOX_DIREX "mydir"
 #define NETWOX_DIREX_RD "c:/"
 #define NETWOX_DIREX_WR "c:/mydir2"
#endif

/*-------------------------------------------------------------*/
#endif
