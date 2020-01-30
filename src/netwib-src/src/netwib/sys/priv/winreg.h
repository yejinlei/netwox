
/*-------------------------------------------------------------*/
netwib_err netwib_priv_winreg_uint32(HKEY hive,
                                     netwib_constbuf *pkey,
                                     netwib_constbuf *pname,
                                     netwib_uint32 *pvalue);
netwib_err netwib_priv_winreg_buf(HKEY hive,
                                  netwib_constbuf *pkey,
                                  netwib_constbuf *pname,
                                  netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* those 3 ring contains netwib_priv_bufstore */
/* key::name contain "abc\0cde\0\0" */
netwib_err netwib_priv_winreg_buflist(HKEY hive,
                                      netwib_constbuf *pkey,
                                      netwib_constbuf *pname,
                                      netwib_ring **ppring);
/* list of keys under pkey */
netwib_err netwib_priv_winreg_subkeylist(HKEY hive,
                                         netwib_constbuf *pkey,
                                         netwib_ring **ppring);
/* list of names under pkey */
netwib_err netwib_priv_winreg_subnamelist(HKEY hive,
                                          netwib_constbuf *pkey,
                                          netwib_ring **ppring);
