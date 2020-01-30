/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_priv_dll_winpcap netwib_global_dll_winpcap;

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_winpcap_init(netwib_priv_dll_winpcap *pdll_winpcap)
{

  pdll_winpcap->isloaded = NETWIB_FALSE;

  /* load the DLL */
  netwib_er(netwib_priv_dll_loadlibrary("wpcap.dll",
                                        &pdll_winpcap->hinstance));

  /* init functions */
  netwib__priv_dll_f(pdll_winpcap, pcap_close,
                     ppcap_close, "pcap_close");
  netwib__priv_dll_f(pdll_winpcap, pcap_compile,
                     ppcap_compile, "pcap_compile");
  netwib__priv_dll_f(pdll_winpcap, pcap_datalink,
                     ppcap_datalink, "pcap_datalink");
  netwib__priv_dll_f(pdll_winpcap, pcap_dump,
                     ppcap_dump, "pcap_dump");
  netwib__priv_dll_f(pdll_winpcap, pcap_dump_close,
                     ppcap_dump_close, "pcap_dump_close");
  netwib__priv_dll_f(pdll_winpcap, pcap_dump_open,
                     ppcap_dump_open, "pcap_dump_open");
  netwib__priv_dll_f(pdll_winpcap, pcap_fileno,
                     ppcap_fileno, "pcap_fileno");
  netwib__priv_dll_f(pdll_winpcap, pcap_findalldevs,
                     ppcap_findalldevs, "pcap_findalldevs");
  netwib__priv_dll_f(pdll_winpcap, pcap_freealldevs,
                     ppcap_freealldevs, "pcap_freealldevs");
  netwib__priv_dll_f(pdll_winpcap, pcap_freecode,
                     ppcap_freecode, "pcap_freecode");
  netwib__priv_dll_f(pdll_winpcap, pcap_lookupnet,
                     ppcap_lookupnet, "pcap_lookupnet");
  netwib__priv_dll_f(pdll_winpcap, pcap_geterr,
                     ppcap_geterr, "pcap_geterr");
  netwib__priv_dll_f(pdll_winpcap, pcap_getevent,
                     ppcap_getevent, "pcap_getevent");
  netwib__priv_dll_f(pdll_winpcap, pcap_next,
                     ppcap_next, "pcap_next");
  netwib__priv_dll_f(pdll_winpcap, pcap_open_live,
                     ppcap_open_live, "pcap_open_live");
  netwib__priv_dll_f(pdll_winpcap, pcap_open_dead,
                     ppcap_open_dead, "pcap_open_dead");
  netwib__priv_dll_f(pdll_winpcap, pcap_open_offline,
                     ppcap_open_offline, "pcap_open_offline");
  netwib__priv_dll_f(pdll_winpcap, pcap_setfilter,
                     ppcap_setfilter, "pcap_setfilter");
  netwib__priv_dll_f(pdll_winpcap, pcap_setmintocopy,
                     ppcap_setmintocopy, "pcap_setmintocopy");

  pdll_winpcap->isloaded = NETWIB_TRUE;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_winpcap_close(netwib_priv_dll_winpcap *pdll_winpcap)
{
  int reti;

  if (!pdll_winpcap->isloaded)
    return(NETWIB_ERR_OK);

  /* unload the DLL */
  reti = FreeLibrary(pdll_winpcap->hinstance);
  if (!reti)
    return(NETWIB_ERR_FUFREELIBRARY);

  return(NETWIB_ERR_OK);
}
