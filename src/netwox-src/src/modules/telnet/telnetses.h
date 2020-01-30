
/*-------------------------------------------------------------*/
netwib_err netwox_telnetses_login(netwib_io *pio,
                                  netwib_consttime *pabstime,
                                  netwib_constbuf *plogin,
                                  netwib_constbuf *ppassword);

/*-------------------------------------------------------------*/
netwib_err netwox_telnetses_logout(netwib_io *pio);

/*-------------------------------------------------------------*/
netwib_err netwox_telnetses_exec(netwib_io *pio,
                                 netwib_consttime *pabstime,
                                 netwib_constbuf *pcmd,
                                 netwib_buf *pbuf);
