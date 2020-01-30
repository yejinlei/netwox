/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_winreg_uint32(HKEY hive,
                                     netwib_constbuf *pkey,
                                     netwib_constbuf *pname,
                                     netwib_uint32 *pvalue)
{
  HKEY hivekey;
  DWORD type, valuesize, value;
  netwib_string key, name;
  int reti;

  netwib__constbuf_ref_string(pkey, key, bufstorage,
                              netwib_priv_winreg_uint32(hive, &bufstorage, pname, pvalue));
  netwib__constbuf_ref_string(pname, name, bufstorage,
                              netwib_priv_winreg_uint32(hive, pkey, &bufstorage, pvalue));

  /* get value from registry */
  reti = RegOpenKeyEx(hive, key, 0, KEY_READ, &hivekey);
  if (reti != ERROR_SUCCESS)
    return(NETWIB_ERR_FUREGOPENKEYEX);
  valuesize = sizeof(netwib_uint32);
  reti = RegQueryValueEx(hivekey, name, NULL, &type, (LPBYTE)&value,
                         &valuesize);
  if (reti != ERROR_SUCCESS) {
    RegCloseKey(hivekey);
    return(NETWIB_ERR_FUREGQUERYVALUEEX);
  }
  reti = RegCloseKey(hivekey);
  if (reti != ERROR_SUCCESS)
    return(NETWIB_ERR_FUREGCLOSEKEY);
  if (type != REG_DWORD) {
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  /* set output value */
  if (pvalue != NULL)
    *pvalue = value;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_winreg_buf(HKEY hive,
                                  netwib_constbuf *pkey,
                                  netwib_constbuf *pname,
                                  netwib_buf *pbuf)
{
  HKEY hivekey;
  DWORD type, valuesize;
  netwib_string key, name;
  netwib_data bufdata;
  netwib_err ret;
  int reti;

  netwib__constbuf_ref_string(pkey, key, bufstorage,
                              netwib_priv_winreg_buf(hive, &bufstorage, pname, pbuf));
  netwib__constbuf_ref_string(pname, name, bufstorage,
                              netwib_priv_winreg_buf(hive, pkey, &bufstorage, pbuf));

  /* get value from registry */
  reti = RegOpenKeyEx(hive, key, 0, KEY_READ, &hivekey);
  if (reti != ERROR_SUCCESS)
    return(NETWIB_ERR_FUREGOPENKEYEX);

  /* get the size of the value of this key */
  reti = RegQueryValueEx(hivekey, name, NULL, &type, NULL, &valuesize);
  if (reti != ERROR_SUCCESS) {
    RegCloseKey(hivekey);
    return(NETWIB_ERR_FUREGQUERYVALUEEX);
  }
  if (type != REG_BINARY && type != REG_SZ) {
    RegCloseKey(hivekey);
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  /* request size */
  ret = netwib_buf_wantspace(pbuf, valuesize, &bufdata);
  if (ret != NETWIB_ERR_OK) {
    RegCloseKey(hivekey);
    return(ret);
  }

  /* set data */
  reti = RegQueryValueEx(hivekey, name, NULL, NULL, bufdata, &valuesize);
  if (reti != ERROR_SUCCESS) {
    RegCloseKey(hivekey);
    return(NETWIB_ERR_FUREGQUERYVALUEEX);
  }
  reti = RegCloseKey(hivekey);
  if (reti != ERROR_SUCCESS) {
    return(NETWIB_ERR_FUREGCLOSEKEY);
  }

  /* update end */
  if (type == REG_SZ && valuesize != 0) valuesize--;
  pbuf.endoffset += valuesize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_winreg_buflist(HKEY hive,
                                      netwib_constbuf *pkey,
                                      netwib_constbuf *pname,
                                      netwib_ring **ppring)
{
  netwib_buf bufvalue, bufitem;
  netwib_data valuedata;
  netwib_uint32 valuedatasize, n, itemsize;
  netwib_ptr pitem;
  netwib_err ret;

  /* get value from registry */
  netwib_er(netwib_buf_init_mallocdefault(&bufvalue));
  ret = netwib_priv_winreg_buf(hive, pkey, pname, &bufvalue);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_buf_close(&bufvalue));
    return(ret);
  }
  valuedata = netwib__buf_ref_data_ptr(&bufvalue);
  valuedatasize = netwib__buf_ref_data_size(&bufvalue);

  /* store items in the list */
  netwib_er(netwib_priv_bufstore_ring_init(ppring));
  netwib_er(netwib_buf_init_mallocdefault(&bufitem));
  n = 0;
  while (n < valuedatasize) {
    itemsize = netwib_c_strlen((netwib_string)valuedata + n);
    if (itemsize == 0) break;
    netwib__buf_reinit(&bufitem);
    netwib_er(netwib_buf_append_data(valuedata + n, itemsize, &bufitem));
    netwib_er(netwib_priv_bufstore_create(&bufitem, &pitem));
    netwib_er(netwib_ring_add_last(*ppring, pitem));
    n += itemsize + 1;
  }
  netwib_er(netwib_buf_close(&bufitem));
  netwib_er(netwib_buf_close(&bufvalue));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_winreg_subkeylist(HKEY hive,
                                         netwib_constbuf *pkey,
                                         netwib_ring **ppring)
{
  HKEY hivekey;
  DWORD i, MaxSubKeyLen;
  netwib_string key, stringkeyname;
  netwib_buf bufkeyname, buflongkeyname;
  netwib_ptr pitem;
  BOOL err;
  netwib_err ret = NETWIB_ERR_OK;

  netwib__constbuf_ref_string(pkey, key, bufstorage,
                              netwib_priv_winreg_subkeylist(hive, &bufstorage,
                                                            ppring));

  /* get value from registry */
  err = RegOpenKeyEx(hive, key, 0, KEY_READ, &hivekey);
  if (err != ERROR_SUCCESS) {
    return(NETWIB_ERR_FUREGOPENKEYEX);
  }

  /* get the longest subkey name */
  err = RegQueryInfoKey(hivekey, NULL, NULL, NULL, NULL, &MaxSubKeyLen,
                        NULL, NULL, NULL, NULL, NULL, NULL);
  if (err != ERROR_SUCCESS) {
    RegCloseKey(hivekey);
    return(NETWIB_ERR_FUREGQUERYINFOKEY);
  }

  /* initialize list */
  netwib_er(netwib_priv_bufstore_ring_init(ppring));

  /* allocate data to store key names */
  netwib_er(netwib_buf_init_malloc(MaxSubKeyLen, &bufkeyname));
  netwib_er(netwib_buf_ref_string(&bufkeyname, &stringkeyname));
  netwib_er(netwib_buf_init_mallocdefault(&buflongkeyname));

  /* list the subkeys */
  i = 0;
  while (NETWIB_TRUE) {
    /* find it */
    err = RegEnumKey(hivekey, i, stringkeyname, MaxSubKeyLen + 1);
    if (err == ERROR_NO_MORE_ITEMS) {
      ret = NETWIB_ERR_OK;
      break;
    } else if (err != ERROR_SUCCESS) {
      ret = NETWIB_ERR_FUREGENUMKEY;
      break;
    }
    /* create the long name */
    netwib__buf_reinit(&buflongkeyname);
    netwib_er(netwib_buf_append_buf(pkey, &buflongkeyname));
    netwib_er(netwib_buf_append_string("\\", &buflongkeyname));
    netwib_er(netwib_buf_append_string(stringkeyname, &buflongkeyname));
    /* add it */
    netwib_er(netwib_priv_bufstore_create(&buflongkeyname, &pitem));
    netwib_er(netwib_ring_add_last(*ppring, pitem));
    i++;
  }
  netwib_er(netwib_buf_close(&bufkeyname));
  netwib_er(netwib_buf_close(&buflongkeyname));

  /* free list only on errors */
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_priv_bufstore_ring_close(ppring));
  }

  /* close the registry */
  err = RegCloseKey(hivekey);
  if (err != ERROR_SUCCESS) {
    return(NETWIB_ERR_FUREGCLOSEKEY);
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_winreg_subnamelist(HKEY hive,
                                          netwib_constbuf *pkey,
                                          netwib_ring **ppring)
{
  HKEY hivekey;
  DWORD i, MaxValueNameLen, stringvaluenamesize;
  netwib_string key, stringvaluename;
  netwib_buf bufvaluename;
  netwib_ptr pitem;
  BOOL err;
  netwib_err ret = NETWIB_ERR_OK;

  netwib__constbuf_ref_string(pkey, key, bufstorage,
                              netwib_priv_winreg_subnamelist(hive, &bufstorage,
                                                             ppring));

  /* get value from registry */
  err = RegOpenKeyEx(hive, key, 0, KEY_READ, &hivekey);
  if (err != ERROR_SUCCESS) {
    return(NETWIB_ERR_FUREGOPENKEYEX);
  }

  /* get the longest value name */
  err = RegQueryInfoKey(hivekey, NULL, NULL, NULL, NULL, NULL,
                        NULL, NULL, &MaxValueNameLen, NULL, NULL, NULL);
  if (err != ERROR_SUCCESS) {
    RegCloseKey(hivekey);
    return(NETWIB_ERR_FUREGQUERYINFOKEY);
  }

  /* initialize list */
  netwib_er(netwib_priv_bufstore_ring_init(ppring));

  /* allocate data to store value names */
  netwib_er(netwib_buf_init_malloc(MaxValueNameLen, &bufvaluename));
  netwib_er(netwib_buf_ref_string(&bufvaluename, &stringvaluename));

  /* list the names */
  i = 0;
  while (NETWIB_TRUE) {
    /* find it */
    stringvaluenamesize = MaxValueNameLen + 1;
    err = RegEnumValue(hivekey, i, stringvaluename, &stringvaluenamesize,
                       NULL, NULL,
                       NULL, NULL);
    if (err == ERROR_NO_MORE_ITEMS) {
      ret = NETWIB_ERR_OK;
      break;
    } else if (err != ERROR_SUCCESS) {
      ret = NETWIB_ERR_FUREGENUMVALUE;
      break;
    }
    /* add it */
    bufvaluename.endoffset = bufvaluename.beginoffset + stringvaluenamesize;
    netwib_er(netwib_priv_bufstore_create(&bufvaluename, &pitem));
    netwib_er(netwib_ring_add_last(*ppring, pitem));
    i++;
  }
  netwib_er(netwib_buf_close(&bufvaluename));

  /* free list only on errors */
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_priv_bufstore_ring_close(ppring));
  }

  /* close the registry */
  err = RegCloseKey(hivekey);
  if (err != ERROR_SUCCESS) {
    return(NETWIB_ERR_FUREGCLOSEKEY);
  }

  return(ret);
}
