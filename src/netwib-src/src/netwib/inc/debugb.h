
/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_MEM_CORRUPT == 2 || NETWIB_DEBUG_MEM_CORRUPT == 3
 #if defined NETWIBDEF_SYSNAME_HPUX
  #error "under HP-UX, mprotect only works on a mmapped file"
 #endif
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_MEM_CORRUPT==2 || NETWIB_DEBUG_MEM_CORRUPT==3
 #define NETWIB_DEBUG_MEMPROTECT_NEEDED 1
#else
 #define NETWIB_DEBUG_MEMPROTECT_NEEDED 0
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_MEM_CORRUPT!=0
 #define NETWIB_DEBUG_MEMCORRUPT_NEEDED 1
#else
 #define NETWIB_DEBUG_MEMCORRUPT_NEEDED 0
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK==1
 #define NETWIB_DEBUG_LEAK_NEEDED 1
#else
 #define NETWIB_DEBUG_LEAK_NEEDED 0
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_ERROR_BT==1
 #define NETWIB_DEBUG_BACKTRACE_NEEDED 1
#elif NETWIB_DEBUG_LEAK==1
 #define NETWIB_DEBUG_BACKTRACE_NEEDED 1
#else
 #define NETWIB_DEBUG_BACKTRACE_NEEDED 0
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK==1
 #define NETWIB_DEBUG_GLOVARS_NEEDED 1
#elif NETWIB_DEBUG_MEM_CORRUPT!=0
 #define NETWIB_DEBUG_GLOVARS_NEEDED 1
#else
 #define NETWIB_DEBUG_GLOVARS_NEEDED 0
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_BACKTRACE_NEEDED==1
 #define NETWIB_DEBUG_EXEC_NEEDED 1
#else
 #define NETWIB_DEBUG_EXEC_NEEDED 0
#endif
#if defined NETWIBDEF_SYSNAME_Windows
 /* not yet implemented */
 #undef NETWIB_DEBUG_EXEC_NEEDED
 #define NETWIB_DEBUG_EXEC_NEEDED 0
#endif

/*-------------------------------------------------------------*/
#ifdef NETWIBDEF_SYSNAME_Unix
 #define NETWIB_DEBUG_CS_NEEDED 0
#elif defined NETWIBDEF_SYSNAME_Windows
 #if NETWIB_DEBUG_FD_OPEN==1
  #define NETWIB_DEBUG_CS_NEEDED 1
 #else
  #define NETWIB_DEBUG_CS_NEEDED 0
 #endif
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_FD_OPEN==1
 #define NETWIB_DEBUG_FDOPEN_NEEDED 1
#else
 #define NETWIB_DEBUG_FDOPEN_NEEDED 0
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_SIGHDL_REPORT_LEAKMEMFD==1
 #if defined NETWIBDEF_SYSNAME_Unix
  #define NETWIB_DEBUG_SIGHDL_NEEDED 1
 #elif defined NETWIBDEF_SYSNAME_Windows
  #define NETWIB_DEBUG_SIGHDL_NEEDED 0
 #endif
#else
 #define NETWIB_DEBUG_SIGHDL_NEEDED 0
#endif
