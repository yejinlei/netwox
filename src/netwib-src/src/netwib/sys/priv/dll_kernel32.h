
/*-------------------------------------------------------------*/
typedef BOOL(WINAPI *pSwitchToThread) (VOID);
typedef BOOL(WINAPI *pTryEnterCriticalSection) (LPCRITICAL_SECTION lpCriticalSection);

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bool isloaded;
  HINSTANCE hinstance;
  netwib_bool switchtothread_avail;
  pSwitchToThread switchtothread;
  netwib_bool tryentercriticalsection_avail;
  pTryEnterCriticalSection tryentercriticalsection;
} netwib_priv_dll_kernel32;
extern netwib_priv_dll_kernel32 netwib_global_dll_kernel32;

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_kernel32_init(netwib_priv_dll_kernel32 *pdll_kernel32);
netwib_err netwib_priv_dll_kernel32_close(netwib_priv_dll_kernel32 *pdll_kernel32);

