
/*-------------------------------------------------------------*/
netwib_err netwib_priv_dlpi_open(netwib_conststring device,
                                 int *pfd);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dlpi_putmsg(int fd,
                                   netwib_ptr ptr,
                                   int len,
                                   int flags);
netwib_err netwib_priv_dlpi_getmsg(int fd,
                                   int len,
                                   netwib_ptr ptr);

