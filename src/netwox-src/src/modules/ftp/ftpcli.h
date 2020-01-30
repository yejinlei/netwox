
/*-------------------------------------------------------------*/
netwib_err netwox_ftpcli_user(netwib_io *pio,
                              netwib_constbuf *puser);
netwib_err netwox_ftpcli_pass(netwib_io *pio,
                              netwib_constbuf *ppassword);
netwib_err netwox_ftpcli_quit(netwib_io *pio);

/*-------------------------------------------------------------*/
netwib_err netwox_ftpcli_port(netwib_io *pio,
                              netwib_constip *pip,
                              netwib_port port);
netwib_err netwox_ftpcli_pasv(netwib_io *pio);

/*-------------------------------------------------------------*/
netwib_err netwox_ftpcli_eprt(netwib_io *pio,
                              netwib_constip *pip,
                              netwib_port port);
netwib_err netwox_ftpcli_epsv(netwib_io *pio);

/*-------------------------------------------------------------*/
netwib_err netwox_ftpcli_retr(netwib_io *pio,
                              netwib_constbuf *pfilename);
netwib_err netwox_ftpcli_stor(netwib_io *pio,
                              netwib_constbuf *pfilename);
netwib_err netwox_ftpcli_dele(netwib_io *pio,
                              netwib_constbuf *pfilename);
netwib_err netwox_ftpcli_size(netwib_io *pio,
                              netwib_constbuf *pfilename);

/*-------------------------------------------------------------*/
netwib_err netwox_ftpcli_type(netwib_io *pio,
                              netwib_conststring type);
#define netwox_ftpcli_type_ascii(pio) netwox_ftpcli_type(pio,"A")
#define netwox_ftpcli_type_bin(pio) netwox_ftpcli_type(pio,"I")

/*-------------------------------------------------------------*/
netwib_err netwox_ftpcli_mkd(netwib_io *pio,
                             netwib_constbuf *pdirname);
netwib_err netwox_ftpcli_rmd(netwib_io *pio,
                             netwib_constbuf *pdirname);
netwib_err netwox_ftpcli_cwd(netwib_io *pio,
                             netwib_constbuf *pdirname);
netwib_err netwox_ftpcli_list(netwib_io *pio,
                              netwib_constbuf *pdirname);
