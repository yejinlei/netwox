
/*-------------------------------------------------------------*/
netwib_err netwox_checksum_verify_ip(netwib_bufpool *ppool,
                                     netwib_buf *ppkt);
netwib_err netwox_checksum_verify_link(netwib_bufpool *ppool,
                                       netwib_device_dlttype dlttype,
                                       netwib_buf *ppkt);

/*-------------------------------------------------------------*/
netwib_err netwox_checksum_compute_ip(netwib_constbuf *ppktin,
                                      netwib_buf *ppktout);
netwib_err netwox_checksum_compute_link(netwib_device_dlttype dlttype,
                                        netwib_constbuf *ppktin,
                                        netwib_buf *ppktout);
