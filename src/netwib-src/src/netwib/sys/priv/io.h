
/*-------------------------------------------------------------*/
netwib_err netwib_priv_io_supported(netwib_io *pio,
                                    netwib_io_waytype way);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_io_last(netwib_io *pio,
                               netwib_io_waytype way,
                               netwib_io **pplastio);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_io_search(netwib_io *pio,
                                 netwib_io_waytype way,
                                 netwib_io *psearchedio,
                                 netwib_io **ppprevio);





