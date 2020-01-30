/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_priv_dll_rasapi32 netwib_global_dll_rasapi32;

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_rasapi32_init(netwib_priv_dll_rasapi32 *pdll_rasapi32)
{
  pdll_rasapi32->isloaded = NETWIB_FALSE;

  /* load the DLL */
  netwib_er(netwib_priv_dll_loadlibrary("rasapi32.dll",
                                        &pdll_rasapi32->hinstance));

  /* init functions */
  netwib__priv_dll_f(pdll_rasapi32, RasEnumConnections,
                     pRasEnumConnections,
#ifdef UNICODE
                     "RasEnumConnectionsW"
#else
                     "RasEnumConnectionsA"
#endif
                     );
  netwib__priv_dll_f(pdll_rasapi32, RasGetProjectionInfo,
                     pRasGetProjectionInfo,
#ifdef UNICODE
                     "RasGetProjectionInfoW"
#else
                     "RasGetProjectionInfoA"
#endif
                     );

  pdll_rasapi32->isloaded = NETWIB_TRUE;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_rasapi32_close(netwib_priv_dll_rasapi32 *pdll_rasapi32)
{
  int reti;

  if (!pdll_rasapi32->isloaded)
    return(NETWIB_ERR_OK);

  /* unload the DLL */
  reti = FreeLibrary(pdll_rasapi32->hinstance);
  pdll_rasapi32->isloaded = NETWIB_FALSE;
  if (!reti)
    return(NETWIB_ERR_FUFREELIBRARY);

  return(NETWIB_ERR_OK);
}
