
/*-------------------------------------------------------------*/
/* Name : netwib_io_init_handle
   Description :
     Open a Windows HANDLE. This function only exists
     Windows.
   Input parameter(s) :
     h : HANDLE
   Input/output parameter(s) :
     closeatend : if HANDLE have to be closed when the io is
                  closed
   Output parameter(s) :
     **ppio : io created
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_io_init_handle(NETWIBHANDLE h,
                                 netwib_bool closeatend,
                                 netwib_io **ppio);

/*-------------------------------------------------------------*/
/* Name : netwib_windowstype_init
   Description :
     Get system version.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     *ptype : system type
   Normal return values :
     NETWIB_ERR_OK : ok
*/
typedef enum {
  NETWIB_WINDOWSTYPE_UNKNOWN = 0, /* Unknown system */
  NETWIB_WINDOWSTYPE_31,          /* Windows 3.1 */
  NETWIB_WINDOWSTYPE_95,          /* Windows 95 */
  NETWIB_WINDOWSTYPE_98,          /* Windows 98 */
  NETWIB_WINDOWSTYPE_ME,          /* Windows Me */
  NETWIB_WINDOWSTYPE_NT350,       /* Windows NT 3.5.0 */
  NETWIB_WINDOWSTYPE_NT351,       /* Windows NT 3.5.1 */
  NETWIB_WINDOWSTYPE_NT4,         /* Windows NT 4 */
  NETWIB_WINDOWSTYPE_2000,        /* Windows 2000 */
  NETWIB_WINDOWSTYPE_XP,          /* Windows XP */
  NETWIB_WINDOWSTYPE_2003,        /* Windows 2003 */
  NETWIB_WINDOWSTYPE_2008,        /* Windows 2008 */
  NETWIB_WINDOWSTYPE_7            /* Windows 7 */
} netwib_windowstype;
netwib_err netwib_windowstype_init(netwib_windowstype *ptype);
