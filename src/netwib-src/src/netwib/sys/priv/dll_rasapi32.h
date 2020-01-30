
/*-------------------------------------------------------------*/
typedef DWORD (WINAPI*pRasEnumConnections) (LPRASCONN lprasconn, LPDWORD lpcb, LPDWORD lpcConnections);
typedef DWORD (WINAPI*pRasGetProjectionInfo) (HRASCONN hrasconn, RASPROJECTION rasprojection, LPVOID lpprojection, LPDWORD lpcb);

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bool isloaded;
  HINSTANCE hinstance;
  pRasEnumConnections RasEnumConnections;
  pRasGetProjectionInfo RasGetProjectionInfo;
} netwib_priv_dll_rasapi32;
extern netwib_priv_dll_rasapi32 netwib_global_dll_rasapi32;

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_rasapi32_init(netwib_priv_dll_rasapi32 *pdll_rasapi32);
netwib_err netwib_priv_dll_rasapi32_close(netwib_priv_dll_rasapi32 *pdll_rasapi32);
