
/*-------------------------------------------------------------*/
netwib_err netwib_urllocalbdd_url_init_local(netwib_constbuf *prootdir,
                                             netwib_constbuf *plocalfilename,
                                             netwib_buf *purl);

/*-------------------------------------------------------------*/
netwib_err netwib_urllocalbdd_local_init_url(netwib_constbuf *prootdir,
                                             netwib_constbuf *purl,
                                             netwib_buf *plocalfilename);
