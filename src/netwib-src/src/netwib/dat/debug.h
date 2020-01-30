
/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_TYPE_NOTSET==1
 #define NETWIB_DEBUG_NOTSET_UINT8  0xAA
 #define NETWIB_DEBUG_NOTSET_UINT16 0xAAAA
 #define NETWIB_DEBUG_NOTSET_UINT32 0xAAAAAAAAu
 #define NETWIB_DEBUG_NOTSET_INT8  0x7A
 #define NETWIB_DEBUG_NOTSET_INT16 0x7AAA
 #define NETWIB_DEBUG_NOTSET_INT32 0x7AAAAAAAu
 #define NETWIB_DEBUG_NOTSET_STRING "-notset-"
 #define netwib_debug_notset_bool(a) {a=NETWIB_DEBUG_NOTSET_UINT8;}
 #define netwib_debug_notset_int(a) {a=NETWIB_DEBUG_NOTSET_INT32;}
 #define netwib_debug_notset_int8(a) {a=NETWIB_DEBUG_NOTSET_INT8;}
 #define netwib_debug_notset_int16(a) {a=NETWIB_DEBUG_NOTSET_INT16;}
 #define netwib_debug_notset_int32(a) {a=NETWIB_DEBUG_NOTSET_INT32;}
 #define netwib_debug_notset_uint8(a) {a=NETWIB_DEBUG_NOTSET_UINT8;}
 #define netwib_debug_notset_uint16(a) {a=NETWIB_DEBUG_NOTSET_UINT16;}
 #define netwib_debug_notset_uint32(a) {a=NETWIB_DEBUG_NOTSET_UINT32;}
 #define netwib_debug_notset_ptr(a) {a=NULL;}
#else
 #define netwib_debug_notset_bool(a) {}
 #define netwib_debug_notset_int(a) {}
 #define netwib_debug_notset_int8(a) {}
 #define netwib_debug_notset_int16(a) {}
 #define netwib_debug_notset_int32(a) {}
 #define netwib_debug_notset_uint8(a) {}
 #define netwib_debug_notset_uint16(a) {}
 #define netwib_debug_notset_uint32(a) {}
 #define netwib_debug_notset_ptr(a) {}
#endif


/*-------------------------------------------------------------*/
#if NETWIB_DEBUG==1
 #include "debug/string.h"
 #include "debug/string2.h"
#endif


/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_MEMPROTECT_NEEDED==1
 #include "debug/memprotect.h"
#endif

/*-------------------------------------------------------------*/
/* NETWIB_DEBUG_MEMCORRUPT_NEEDED is checked inside */
#include "debug/memcorrupt.h"


