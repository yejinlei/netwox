/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
/* This critical section is for PacketOpenAdapter
   (PacketGetAdapterNames and pcap_open_live internally call it).
   We also use it for close functions. */
CRITICAL_SECTION netwib_priv_criticalsection_packetdll;

/*-------------------------------------------------------------*/
netwib_err netwib_priv_cs_init(void)
{
  InitializeCriticalSection(&netwib_priv_criticalsection_packetdll);
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_cs_close(void)
{
  DeleteCriticalSection(&netwib_priv_criticalsection_packetdll);
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_cs_PacketOpenAdapter(LPTSTR AdapterName,
                                            LPADAPTER *plp)
{
  if (!netwib_global_dll_packet.isloaded) {
    return(NETWIB_ERR_LONOTSUPPORTED);
  }

  EnterCriticalSection(&netwib_priv_criticalsection_packetdll);
  *plp = netwib_global_dll_packet.PacketOpenAdapter(AdapterName);
  LeaveCriticalSection(&netwib_priv_criticalsection_packetdll);

  if (*plp == NULL)
    return(NETWIB_ERR_FUPACKETOPENADAPTER);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_cs_PacketCloseAdapter(LPADAPTER lpAdapter)
{
  if (!netwib_global_dll_packet.isloaded) {
    return(NETWIB_ERR_LONOTSUPPORTED);
  }

  EnterCriticalSection(&netwib_priv_criticalsection_packetdll);
  netwib_global_dll_packet.PacketCloseAdapter(lpAdapter);
  LeaveCriticalSection(&netwib_priv_criticalsection_packetdll);
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_cs_pcap_open_live(char *dev,
                                         int mtu,
                                         int promisc,
                                         int toms,
                                         char *err,
                                         pcap_t **ppc)
{
  if (!netwib_global_dll_winpcap.isloaded) {
    return(NETWIB_ERR_LONOTSUPPORTED);
  }

  EnterCriticalSection(&netwib_priv_criticalsection_packetdll);
  *ppc = netwib_global_dll_winpcap.pcap_open_live(dev, mtu, promisc, toms, err);
  LeaveCriticalSection(&netwib_priv_criticalsection_packetdll);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_cs_pcap_close(pcap_t *pt)
{
#if 0
  netwib_bool bugged;
  netwib_err ret;

  /* special case of RAS under Win95 : does not work */
  ret = netwib_priv_conf_winras_w95bugged(&bugged);
  if (ret == NETWIB_ERR_OK && bugged) {
    return(NETWIB_ERR_OK);
  }
#endif
  if (!netwib_global_dll_winpcap.isloaded) {
    return(NETWIB_ERR_LONOTSUPPORTED);
  }

  EnterCriticalSection(&netwib_priv_criticalsection_packetdll);
  netwib_global_dll_winpcap.pcap_close(pt);
  LeaveCriticalSection(&netwib_priv_criticalsection_packetdll);
  return(NETWIB_ERR_OK);
}

