/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_mib_hwtype(netwib_uint32 systemhwtype,
                                              netwib_uint32 speedbps,
                                              netwib_device_hwtype *phwtype)
{

  *phwtype = NETWIB_DEVICE_HWTYPE_UNKNOWN;

  switch(systemhwtype) {
  case MIB_IF_TYPE_ETHERNET :
    if (speedbps >= 1000000) {
      *phwtype = NETWIB_DEVICE_HWTYPE_ETHER;
    }
    break;
  case MIB_IF_TYPE_LOOPBACK :
    *phwtype = NETWIB_DEVICE_HWTYPE_LOOPBACK;
    break;
  case MIB_IF_TYPE_PPP :
    *phwtype = NETWIB_DEVICE_HWTYPE_PPP;
    break;
  case MIB_IF_TYPE_SLIP :
    *phwtype = NETWIB_DEVICE_HWTYPE_SLIP;
    break;
  case 8 /*IF_TYPE_ISO88025_TOKENRING*/ :
    *phwtype = NETWIB_DEVICE_HWTYPE_UNKNOWN; /* Too late to add a new type. */
    break;
  case 37 /*IF_TYPE_ATM*/ :
    *phwtype = NETWIB_DEVICE_HWTYPE_UNKNOWN; /* Too late to add a new type. */
    break;
  case 71 /*IF_TYPE_IEEE80211 = WiFi - Note: before Windows 2008, WiFi devices were indicated as ETHERNET. */ :
    *phwtype = NETWIB_DEVICE_HWTYPE_ETHER;
    break;
  case 131 /*IF_TYPE_TUNNEL*/ :
    *phwtype = NETWIB_DEVICE_HWTYPE_UNKNOWN; /* Too late to add a new type. */
    break;
  case 144 /*IF_TYPE_IEEE1394*/ :
    *phwtype = NETWIB_DEVICE_HWTYPE_UNKNOWN; /* Too late to add a new type. */
    break;
  }

  return(NETWIB_ERR_OK);
}

