/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_init(void)
{
  netwib_err ret;

  /* init iphlpapi */
  ret = netwib_priv_dll_iphlpapi_init(&netwib_global_dll_iphlpapi);
  if (ret != NETWIB_ERR_OK) {
    /* ensure it is not loaded */
    netwib_global_dll_iphlpapi.isloaded = NETWIB_FALSE;
  }

  /* init winsock version 2.2  */
  ret = netwib_priv_dll_winsock2_init(&netwib_global_dll_winsock2);
  if (ret != NETWIB_ERR_OK) {
    /* ensure it is not loaded */
    netwib_global_dll_winsock2.isloaded = NETWIB_FALSE;
  }

  /* init snmp */
  ret = netwib_priv_dll_snmp_init(&netwib_global_dll_snmp);
  if (ret != NETWIB_ERR_OK) {
    /* ensure it is not loaded */
    netwib_global_dll_snmp.isloaded = NETWIB_FALSE;
  }

  /* init kernel32 */
  ret = netwib_priv_dll_kernel32_init(&netwib_global_dll_kernel32);
  if (ret != NETWIB_ERR_OK) {
    /* ensure it is not loaded */
    netwib_global_dll_kernel32.isloaded = NETWIB_FALSE;
  }

  /* init packet */
  ret = netwib_priv_dll_packet_init(&netwib_global_dll_packet);
  if (ret != NETWIB_ERR_OK) {
    /* ensure it is not loaded */
    netwib_global_dll_packet.isloaded = NETWIB_FALSE;
  }

  /* init winpcap */
  ret = netwib_priv_dll_winpcap_init(&netwib_global_dll_winpcap);
  if (ret != NETWIB_ERR_OK) {
    /* ensure it is not loaded */
    netwib_global_dll_winpcap.isloaded = NETWIB_FALSE;
  }

#if 0
  /* currently unused */
  /* init RAS api */
  ret = netwib_priv_dll_rasapi32_init(&netwib_global_dll_rasapi32);
  if (ret != NETWIB_ERR_OK) {
    /* ensure it is not loaded */
    netwib_global_dll_rasapi32.isloaded = NETWIB_FALSE;
  }
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_close(void)
{
  netwib_err ret, retret;

  /* ignore error, but return it */
  retret = NETWIB_ERR_OK;

#if 0
  /* currently unused */
  ret = netwib_priv_dll_rasapi32_close(&netwib_global_dll_rasapi32);
  if (ret != NETWIB_ERR_OK)
    retret = ret;
#endif
  ret = netwib_priv_dll_winpcap_close(&netwib_global_dll_winpcap);
  if (ret != NETWIB_ERR_OK)
    retret = ret;
  ret = netwib_priv_dll_packet_close(&netwib_global_dll_packet);
  if (ret != NETWIB_ERR_OK)
    retret = ret;
  ret = netwib_priv_dll_kernel32_close(&netwib_global_dll_kernel32);
  if (ret != NETWIB_ERR_OK)
    retret = ret;
  ret = netwib_priv_dll_snmp_close(&netwib_global_dll_snmp);
  if (ret != NETWIB_ERR_OK)
    retret = ret;
  ret = netwib_priv_dll_winsock2_close(&netwib_global_dll_winsock2);
  if (ret != NETWIB_ERR_OK)
    retret = ret;
  ret = netwib_priv_dll_iphlpapi_close(&netwib_global_dll_iphlpapi);
  if (ret != NETWIB_ERR_OK)
    retret = ret;

  return(retret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_loadlibrary(netwib_conststring name,
                                       HINSTANCE *ph)
{
  netwib_data bufarray[MAX_PATH];
  netwib_string str;
  netwib_buf buf;
  UINT err;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(bufarray, &buf));

  /* search in system directory */
  err = GetSystemDirectory((netwib_string)netwib__buf_ref_data_ptr(&buf), MAX_PATH);
  if (err != 0 && err <= MAX_PATH) {
    buf.endoffset = err;
    netwib_er(netwib_buf_append_byte('\\', &buf));
    netwib_er(netwib_buf_append_string(name, &buf));
    netwib_er(netwib_buf_ref_string(&buf, &str));
    *ph = LoadLibrary(str);
    if (*ph != NULL) {
      netwib_er(netwib_buf_close(&buf));
      return(NETWIB_ERR_OK);
    }
  }

  /* search in windows directory */
  err = GetWindowsDirectory((netwib_string)netwib__buf_ref_data_ptr(&buf), MAX_PATH);
  if (err != 0 && err <= MAX_PATH) {
    buf.endoffset = err;
    netwib_er(netwib_buf_append_byte('\\', &buf));
    netwib_er(netwib_buf_append_string(name, &buf));
    netwib_er(netwib_buf_ref_string(&buf, &str));
    *ph = LoadLibrary(str);
    if (*ph != NULL) {
      netwib_er(netwib_buf_close(&buf));
      return(NETWIB_ERR_OK);
    }
  }
  netwib_er(netwib_buf_close(&buf));

  /* search in path */
  *ph = LoadLibrary(name);
  if (*ph == NULL) {
    return(NETWIB_ERR_FULOADLIBRARY);
  }

  return(NETWIB_ERR_OK);
}
