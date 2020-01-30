
/*-------------------------------------------------------------*/
#if NETWIB_DEBUG==1
 #include "debug/disp.h"
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG==1
 #include "debug/process.h"
 #include "debug/thread.h"
#endif

/*-------------------------------------------------------------*/
/* NETWIB_DEBUG_GLOVARS_NEEDED is checked inside */
#include "debug/glovars.h"

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_EXEC_NEEDED==1
 #include "debug/exec.h"
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_BACKTRACE_NEEDED==1
 #include "debug/backtrace.h"
#endif

/*-------------------------------------------------------------*/
/* NETWIB_DEBUG_LEAK_NEEDED is checked inside */
#include "debug/leak.h"

/*-------------------------------------------------------------*/
/* NETWIB_DEBUG_FDOPEN_NEEDED is checked inside */
#include "debug/fdopen.h"

/*-------------------------------------------------------------*/
/* NETWIB_DEBUG_SIGHDL_NEEDED is checked inside */
#include "debug/sighdl.h"


