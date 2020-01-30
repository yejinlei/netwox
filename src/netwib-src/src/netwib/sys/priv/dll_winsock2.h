
typedef int (WINAPI*pWSAStartup) (WORD wVersionRequested, LPWSADATA lpWSAData);
typedef int (WINAPI*pWSACleanup) (void);
typedef SOCKET (WINAPI*psocket) (int af, int type, int protocol);
typedef int (WINAPI*pclosesocket) (SOCKET s);
typedef int (WINAPI*psetsockopt) (SOCKET s, int level, int optname, const char FAR *optval, int optlen);
typedef int (WINAPI*psendto) (SOCKET s, const char FAR *buf, int len, int flags, const struct sockaddr FAR *to, int tolen);

typedef struct {
  netwib_bool isloaded;
  HINSTANCE hinstance;
  pWSAStartup WSAStartup;
  pWSACleanup WSACleanup;
  psocket socket;
  pclosesocket closesocket;
  psetsockopt setsockopt;
  psendto sendto;
} netwib_priv_dll_winsock2;
extern netwib_priv_dll_winsock2 netwib_global_dll_winsock2;

netwib_err netwib_priv_dll_winsock2_init(netwib_priv_dll_winsock2 *pdll_winsock2);
netwib_err netwib_priv_dll_winsock2_close(netwib_priv_dll_winsock2 *pdll_winsock2);
