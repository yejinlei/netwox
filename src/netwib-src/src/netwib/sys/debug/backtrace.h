
/*-------------------------------------------------------------*/
netwib_err netwib_debug_backtrace_array(netwib_uint32 btmaxsize,
                                        void **bt,
                                        netwib_uint32 *pbtsize);

/*-------------------------------------------------------------*/
/* use netwib_debug_string_close to free string */
netwib_err netwib_debug_backtrace_string(netwib_string *pstringout);

/*-------------------------------------------------------------*/
netwib_err netwib_debug_backtrace_disp(void);

/*-------------------------------------------------------------*/
/* this size is used by default in subfunctions */
#define NETWIB_DEBUG_BACKTRACE_MAXSIZE 50
