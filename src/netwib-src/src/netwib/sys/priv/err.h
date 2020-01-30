
/*-------------------------------------------------------------*/
netwib_err netwib_priv_err_purge(void);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_err_syserrors(netwib_uint32 *perrno,
                                     netwib_uint32 *pherrno,
                                     netwib_uint32 *pgetlasterror);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_err_append_err(netwib_err error,
                                      netwib_uint32 varerrno,
                                      netwib_uint32 varherrno,
                                      netwib_uint32 vargetlasterror,
                                      netwib_err_encodetype encodetype,
                                      netwib_buf *pbuf);
