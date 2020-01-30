
/*-------------------------------------------------------------*/
netwib_err netwib_priv_cmdline_init(netwib_constbuf *pbufcommand,
                                    netwib_string *pfilename,
                                    int *pargc,
                                    netwib_string *pargv[]);
netwib_err netwib_priv_cmdline_close(netwib_string *pfilename,
                                     netwib_string *pargv[]);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_cmdline_win(netwib_constbuf *pbufcommand,
                                   netwib_buf *poutbuf);
