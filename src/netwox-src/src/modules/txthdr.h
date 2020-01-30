
/*-------------------------------------------------------------*/
netwib_err netwox_txthdr_decode(netwib_constbuf *pline,
                                netwib_conststring hdr,
                                netwib_bool *pdecoded,
                                netwib_bufext *pvalue);

/*-------------------------------------------------------------*/
netwib_err netwox_txthdr_decode_hostport(netwib_constbuf *pline,
                                         netwib_conststring hdr,
                                         netwib_bool *pdecoded,
                                         netwib_bufext *phost,
                                         netwib_port *pport);

/*-------------------------------------------------------------*/
netwib_err netwox_txthdr_decode_authbasic(netwib_bufpool *pbufpool,
                                          netwib_constbuf *pline,
                                          netwib_conststring hdr,
                                          netwib_bool *pdecoded,
                                          netwib_buf *puser,
                                          netwib_buf *ppassword);

/*-------------------------------------------------------------*/
netwib_err netwox_txthdr_decode_uint32(netwib_constbuf *pline,
                                       netwib_conststring hdr,
                                       netwib_bool *pdecoded,
                                       netwib_uint32 *pui);
