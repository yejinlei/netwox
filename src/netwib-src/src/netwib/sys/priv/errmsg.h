
/*-------------------------------------------------------------*/
netwib_err netwib_priv_errmsg_reinit(void);
netwib_err netwib_priv_errmsg_append_buf(netwib_constbuf *pbuf);
netwib_err netwib_priv_errmsg_append_string(netwib_conststring msg);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_errmsg_buf(netwib_constbuf *pbuf);
netwib_err netwib_priv_errmsg_string(netwib_conststring msg);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_errmsg_func_buf(netwib_conststring functionname,
                                       netwib_constbuf *pexplain);
netwib_err netwib_priv_errmsg_func_string(netwib_conststring functionname,
                                          netwib_conststring explain);

