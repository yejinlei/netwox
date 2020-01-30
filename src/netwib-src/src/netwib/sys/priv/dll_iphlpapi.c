/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_priv_dll_iphlpapi netwib_global_dll_iphlpapi;

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_iphlpapi_init(netwib_priv_dll_iphlpapi *pdll_iphlpapi)
{
  netwib_windowstype windowstype;

  pdll_iphlpapi->isloaded = NETWIB_FALSE;
  pdll_iphlpapi->sendarp_avail = NETWIB_FALSE;

  /* load the DLL */
  netwib_er(netwib_priv_dll_loadlibrary("iphlpapi.dll",
                                        &pdll_iphlpapi->hinstance));

  /* init functions */
  netwib__priv_dll_f(pdll_iphlpapi, GetIfTable,
                     pGetIfTable, "GetIfTable");
  netwib__priv_dll_f(pdll_iphlpapi, GetIpAddrTable,
                     pGetIpAddrTable, "GetIpAddrTable");
  netwib__priv_dll_f(pdll_iphlpapi, GetIpNetTable,
                     pGetIpNetTable, "GetIpNetTable");
  netwib__priv_dll_f(pdll_iphlpapi, GetIpForwardTable,
                     pGetIpForwardTable, "GetIpForwardTable");

  netwib_er(netwib_windowstype_init(&windowstype));
  if (windowstype != NETWIB_WINDOWSTYPE_95 &&
      windowstype != NETWIB_WINDOWSTYPE_98 &&
      windowstype != NETWIB_WINDOWSTYPE_ME) {
    pdll_iphlpapi->SendARP =
      (pSendARP) GetProcAddress(pdll_iphlpapi->hinstance, "SendARP");
    if (pdll_iphlpapi->SendARP != NULL) {
      pdll_iphlpapi->sendarp_avail = NETWIB_TRUE;
    }
  }

  pdll_iphlpapi->isloaded = NETWIB_TRUE;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_iphlpapi_close(netwib_priv_dll_iphlpapi *pdll_iphlpapi)
{
  int reti;

  if (!pdll_iphlpapi->isloaded)
    return(NETWIB_ERR_OK);

  /* unload the DLL */
  reti = FreeLibrary(pdll_iphlpapi->hinstance);
  if (!reti)
    return(NETWIB_ERR_FUFREELIBRARY);

  return(NETWIB_ERR_OK);
}
