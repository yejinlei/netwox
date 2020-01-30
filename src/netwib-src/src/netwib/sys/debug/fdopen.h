
/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_FDOPEN_NEEDED==1
netwib_err netwib_debug_fdopen_init(void);
netwib_err netwib_debug_fdopen_check(void);
#else
 /*used in netwib without #if...*/
 #define netwib_debug_fdopen_init() NETWIB_ERR_OK;
 #define netwib_debug_fdopen_check() NETWIB_ERR_OK;
#endif
