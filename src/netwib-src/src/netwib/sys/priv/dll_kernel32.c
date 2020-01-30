/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_priv_dll_kernel32 netwib_global_dll_kernel32;

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_kernel32_init(netwib_priv_dll_kernel32 *pdll_kernel32)
{

  pdll_kernel32->isloaded = NETWIB_FALSE;
  pdll_kernel32->switchtothread_avail = NETWIB_FALSE;
  pdll_kernel32->tryentercriticalsection_avail = NETWIB_FALSE;

  /* load the DLL */
  netwib_er(netwib_priv_dll_loadlibrary("kernel32.dll",
                                        &pdll_kernel32->hinstance));
  pdll_kernel32->isloaded = NETWIB_TRUE;

  /* init functions */
  pdll_kernel32->switchtothread =(pSwitchToThread)GetProcAddress(pdll_kernel32->hinstance, "SwitchToThread");
  if (pdll_kernel32->switchtothread != NULL) {
    pdll_kernel32->switchtothread_avail = NETWIB_TRUE;
  }
  pdll_kernel32->tryentercriticalsection =(pTryEnterCriticalSection)GetProcAddress(pdll_kernel32->hinstance, "TryEnterCriticalSection");
  if (pdll_kernel32->tryentercriticalsection != NULL) {
    pdll_kernel32->tryentercriticalsection_avail = NETWIB_TRUE;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_kernel32_close(netwib_priv_dll_kernel32 *pdll_kernel32)
{
  int reti;

  if (!pdll_kernel32->isloaded)
    return(NETWIB_ERR_OK);

  /* unload the DLL */
  reti = FreeLibrary(pdll_kernel32->hinstance);
  if (!reti)
    return(NETWIB_ERR_FUFREELIBRARY);

  return(NETWIB_ERR_OK);
}

