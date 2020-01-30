
/*-------------------------------------------------------------*/
typedef DWORD (WINAPI*pGetIfTable) (PMIB_IFTABLE pIfTable, PULONG pdwSize, BOOL bOrder);
typedef DWORD (WINAPI*pGetIpAddrTable) (PMIB_IPADDRTABLE pIpAddrTable, PULONG pdwSize, BOOL bOrder);
typedef DWORD (WINAPI*pGetIpNetTable) (PMIB_IPNETTABLE pIpNetTable, PULONG pdwSize, BOOL bOrder);
typedef DWORD (WINAPI*pGetIpForwardTable) (PMIB_IPFORWARDTABLE pIpForwardTable, PULONG pdwSize, BOOL bOrder);
typedef DWORD (WINAPI*pSendARP) (IPAddr DestIP, IPAddr SrcIP, PULONG pMacAddr, PULONG PhyAddrLen);

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bool isloaded;
  HINSTANCE hinstance;
  pGetIfTable GetIfTable;
  pGetIpAddrTable GetIpAddrTable;
  pGetIpNetTable GetIpNetTable;
  pGetIpForwardTable GetIpForwardTable;
  netwib_bool sendarp_avail;
  pSendARP SendARP;
} netwib_priv_dll_iphlpapi;
extern netwib_priv_dll_iphlpapi netwib_global_dll_iphlpapi;

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_iphlpapi_init(netwib_priv_dll_iphlpapi *pdll_iphlpapi);
netwib_err netwib_priv_dll_iphlpapi_close(netwib_priv_dll_iphlpapi *pdll_iphlpapi);
