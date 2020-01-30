
/*-------------------------------------------------------------*/
netwib_err netwox_snispo_init(netwib_constbuf *pdevice,
                              netwib_constbuf *psnifffilter,
                              netwib_spoof_inittype spoofinittype,
                              netwib_io **ppio);

/*-------------------------------------------------------------*/
netwib_err netwox_snispo_init_eth(netwib_constbuf *pdevice,
                                  netwib_constbuf *psnifffilter,
                                  netwib_io **ppio);

/*-------------------------------------------------------------*/
netwib_err netwox_snispo_init_ip(netwib_constbuf *pdevice,
                                 netwib_constbuf *psnifffilter,
                                 netwib_bool sniffipreas,
                                 netwib_bool snifftcpreord,
                                 netwib_spoof_ip_inittype spoofinittype,
                                 netwib_io **ppio);

