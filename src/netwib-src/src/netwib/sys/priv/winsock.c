/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_winsock_init(void)
{
  WSADATA wsaData;
  int major;
  int minor;
  int reti;

  major = 1;
  minor = 1;
  reti = WSAStartup(MAKEWORD(major, minor), &wsaData);
  if (reti != 0) {
    return(NETWIB_ERR_FUWSASTARTUP);
  }

  if (HIBYTE(wsaData.wVersion) != major || LOBYTE(wsaData.wVersion) != minor) {
    WSACleanup();
    return(NETWIB_ERR_FUWSASTARTUP);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_winsock_close(void)
{
  int reti;

  reti = WSACleanup();
  if (reti != 0) {
    return(NETWIB_ERR_FUWSACLEANUP);
  }

  return(NETWIB_ERR_OK);
}
