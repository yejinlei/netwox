/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_priv_dll_packet netwib_global_dll_packet;

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_packet_init(netwib_priv_dll_packet *pdll_packet)
{
  PCHAR version;

  pdll_packet->isloaded = NETWIB_FALSE;

  /* load the DLL */
  netwib_er(netwib_priv_dll_loadlibrary("packet.dll",
                                        &pdll_packet->hinstance));

  /* init functions */
  netwib__priv_dll_f(pdll_packet, PacketAllocatePacket,
                     pPacketAllocatePacket, "PacketAllocatePacket");
  netwib__priv_dll_f(pdll_packet, PacketCloseAdapter,
                     pPacketCloseAdapter, "PacketCloseAdapter");
  netwib__priv_dll_f(pdll_packet, PacketFreePacket,
                     pPacketFreePacket, "PacketFreePacket");
  netwib__priv_dll_f(pdll_packet, PacketGetNetType,
                     pPacketGetNetType, "PacketGetNetType");
  netwib__priv_dll_f(pdll_packet, PacketGetVersion,
                     pPacketGetVersion, "PacketGetVersion");
  netwib__priv_dll_f(pdll_packet, PacketInitPacket,
                     pPacketInitPacket, "PacketInitPacket");
  netwib__priv_dll_f(pdll_packet, PacketOpenAdapter,
                     pPacketOpenAdapter, "PacketOpenAdapter");
  netwib__priv_dll_f(pdll_packet, PacketRequest,
                     pPacketRequest, "PacketRequest");
  netwib__priv_dll_f(pdll_packet, PacketSendPacket,
                     pPacketSendPacket, "PacketSendPacket");
  netwib__priv_dll_f(pdll_packet, PacketSetBuff,
                     pPacketSetBuff, "PacketSetBuff");

  pdll_packet->versionmajor = 0;
  pdll_packet->versionminor = 0;
  version = pdll_packet->PacketGetVersion();
  if (!netwib_c_strncmp(version, "3.0", 3)) {
    pdll_packet->versionmajor = 3;
    pdll_packet->versionminor = 0;
  } else if (!netwib_c_strncmp(version, "3, 1,", 5)) {
    pdll_packet->versionmajor = 3;
    pdll_packet->versionminor = 1;
  } else {
    pdll_packet->versionmajor = 3;
    pdll_packet->versionminor = 99;
  }

  pdll_packet->isloaded = NETWIB_TRUE;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_packet_close(netwib_priv_dll_packet *pdll_packet)
{
  int reti;

  if (!pdll_packet->isloaded)
    return(NETWIB_ERR_OK);

  /* unload the DLL */
  reti = FreeLibrary(pdll_packet->hinstance);
  if (!reti)
    return(NETWIB_ERR_FUFREELIBRARY);

  return(NETWIB_ERR_OK);
}
