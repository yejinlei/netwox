/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_priv_dll_winsock2 netwib_global_dll_winsock2;

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_winsock2_init(netwib_priv_dll_winsock2 *pdll_winsock2)
{
  WSADATA wsaData;
  int major;
  int minor;
  int reti;

  pdll_winsock2->isloaded = NETWIB_FALSE;

  /* load the DLL */
  netwib_er(netwib_priv_dll_loadlibrary("ws2_32.dll",
                                        &pdll_winsock2->hinstance));

  /* init functions */
  netwib__priv_dll_f(pdll_winsock2, WSAStartup,
                     pWSAStartup, "WSAStartup");
  netwib__priv_dll_f(pdll_winsock2, WSACleanup,
                     pWSACleanup, "WSACleanup");
  netwib__priv_dll_f(pdll_winsock2, socket,
                     psocket, "socket");
  netwib__priv_dll_f(pdll_winsock2, closesocket,
                     pclosesocket, "closesocket");
  netwib__priv_dll_f(pdll_winsock2, setsockopt,
                     psetsockopt, "setsockopt");
  netwib__priv_dll_f(pdll_winsock2, sendto,
                     psendto, "sendto");

  /* init the wanted version */
  major = 2;
  minor = 2;
  reti = pdll_winsock2->WSAStartup(MAKEWORD(major, minor), &wsaData);
  if (reti != 0) {
    FreeLibrary(pdll_winsock2->hinstance);
    return(NETWIB_ERR_FUWSASTARTUP);
  }

  if (HIBYTE(wsaData.wVersion) != major || LOBYTE(wsaData.wVersion) != minor) {
    pdll_winsock2->WSACleanup();
    FreeLibrary(pdll_winsock2->hinstance);
    return(NETWIB_ERR_FUWSASTARTUP);
  }

  pdll_winsock2->isloaded = NETWIB_TRUE;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_winsock2_close(netwib_priv_dll_winsock2 *pdll_winsock2)
{
  int reti;

  if (!pdll_winsock2->isloaded)
    return(NETWIB_ERR_OK);

  /* unload the DLL */
  reti = FreeLibrary(pdll_winsock2->hinstance);
  if (!reti)
    return(NETWIB_ERR_FUFREELIBRARY);

  return(NETWIB_ERR_OK);
}
