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
#if NETWIB_DEBUG==1
 #include "debug/disp.c"
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG==1
 #include "debug/process.c"
 #include "debug/thread.c"
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_GLOVARS_NEEDED==1
 #include "debug/glovars.c"
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_EXEC_NEEDED==1
 #include "debug/exec.c"
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_BACKTRACE_NEEDED==1
 #include "debug/backtrace.c"
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
 #include "debug/leak.c"
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_FDOPEN_NEEDED==1
 #include "debug/fdopen.c"
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_SIGHDL_NEEDED==1
 #include "debug/sighdl.c"
#endif

/*-------------------------------------------------------------*/
/* we need this function otherwise gcc refuses to compile this C file
   because it does not contain any code */
netwib_err netwib_unused_sys_debug(void);
netwib_err netwib_unused_sys_debug(void)
{
  netwib_unused_sys_debug();
  return(NETWIB_ERR_OK);
}

