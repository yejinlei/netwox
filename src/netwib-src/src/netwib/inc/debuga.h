
/* Note : those values should only be modified by netwib developers */

/*-------------------------------------------------------------*/
#define NETWIB_DEBUG_UNSUPPORTED_ALLOWCOMPIL 0
#define NETWIB_DEBUG_CONF_FAKE               0

#define NETWIB_DEBUG                         0
#define NETWIB_DEBUG_ERROR_THREADID          0
#define NETWIB_DEBUG_ERROR_BT                0
#define NETWIB_DEBUG_TYPE_NOTSET             0
#define NETWIB_DEBUG_LEAK                    0
#define NETWIB_DEBUG_MEM_CORRUPT             0
#define NETWIB_DEBUG_FD_OPEN                 0
#define NETWIB_DEBUG_SIGHDL_REPORT_LEAKMEMFD 0
#define NETWIB_DEBUG_RING_CHECK              0
#define NETWIB_DEBUG_HASH_CHECK              0
#define NETWIB_DEBUG_RANGES_CHECK            0

/*-------------------------------------------------------------*/
/*

  NETWIB_DEBUG_UNSUPPORTED_ALLOWCOMPIL
    Allow compilation to test Unsupported_0_xyz.
    0 : do not allow (NETWIB_DEBUG_UNSUPPORTED_TAKEDECISION in use)
    1 : allow

  NETWIB_DEBUG_CONF_FAKE
    Use a fake network configuration.
    0 : no
    1 : yes

  NETWIB_DEBUG
    Activate several debugging functions.
    0 : no
    1 : yes

  NETWIB_DEBUG_ERROR_THREADID
    Display thread id in error messages.
    0 : no
    1 : yes

  NETWIB_DEBUG_ERROR_BT
    Display backtrace of function where the error was first printed.
    0 : no display
    1 : display

  NETWIB_DEBUG_TYPE_NOTSET
    Using functions netwib_priv_notset_xyz, user can mark variables
    as not set.
    0 : do not mark
    1 : mark

  NETWIB_DEBUG_LEAK
    Debug resource leaks.
    0 : no
    1 : yes

  NETWIB_DEBUG_MEM_CORRUPT
    To detect memory corruption.
    0 : no debug
    1 : uninitialized data :
         - read : read data contain a fake uninit_pattern
         - write : no check, this is logical
        bottom data (before allocated data zone) :
         - read : no debug
         - write : check outer_pattern modifications
        top data (after allocated data zone) :
         - read : no debug
         - write : check outer_pattern modifications
        free :
         - read : read data contain a fake free_pattern
         - write : check free_pattern modifications
    2 : uninitialized data :
         - read : read data contain a fake uninit_pattern
         - write : no check, this is logical
        bottom data (before allocated data zone) :
         - read : no debug
         - write : check outer_pattern modifications
        top data (after allocated data zone) :
         - read : protected memory
         - write : protected memory
        free :
         - read : protected memory
         - write : protected memory
    3 : uninitialized data :
         - read : read data contain a fake uninit_pattern
         - write : no check, this is logical
        bottom data (before allocated data zone) :
         - read : protected memory
         - write : protected memory
        top data (after allocated data zone) :
         - read : no debug
         - write : check outer_pattern modifications
        free :
         - read : protected memory
         - write : protected memory

  NETWIB_DEBUG_FD_OPEN
    To detect open file descriptors at the end of process.
    0 : no detect
    1 : detect

  NETWIB_DEBUG_SIGHDL_REPORT_LEAKMEMFD
    When a signal (Control-C) is received, report memory leaks, corrupted
    memory and fd leaks.
    0 : no
    1 : yes

  NETWIB_DEBUG_RING_CHECK
    Debug netwib_ring.
    0 : no
    1 : yes

  NETWIB_DEBUG_HASH_CHECK
    Debug netwib_hash.
    0 : no
    1 : yes

  NETWIB_DEBUG_RANGES_CHECK
    Debug netwib_ip4s, netwib_eths, netwib_ports.
    0 : no
    1 : yes

*/

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_UNSUPPORTED_ALLOWCOMPIL==0
 #define NETWIB_UNSUPPORTED_TAKEDECISION takeadecision+takeadecision;
#else
 #define NETWIB_UNSUPPORTED_TAKEDECISION
#endif

