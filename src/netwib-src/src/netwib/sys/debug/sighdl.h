
/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_SIGHDL_NEEDED==1
netwib_err netwib_debug_sighdl_init(void);
netwib_err netwib_debug_sighdl_close(void);
#else
 /*used in netwib without #if...*/
 #define netwib_debug_sighdl_init() NETWIB_ERR_OK;
 #define netwib_debug_sighdl_close() NETWIB_ERR_OK;
#endif

