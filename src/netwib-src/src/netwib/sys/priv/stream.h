
/*-------------------------------------------------------------*/
netwib_err netwib_priv_stream_read(NETWIBFILE *pfile,
                                   netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_stream_write(NETWIBFILE *pfile,
                                    netwib_constbuf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_stream_wait(NETWIBFILE *pfile,
                                   netwib_io_waytype way,
                                   netwib_consttime *pabstime,
                                   netwib_bool *pevent);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_stream_flush(NETWIBFILE *pfile);
