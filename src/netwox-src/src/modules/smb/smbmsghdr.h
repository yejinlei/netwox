
/*-------------------------------------------------------------*/
/* this module groups smbnaked.h and smbnbtss.h functions in a common header */

/*-------------------------------------------------------------*/
/* between those two calls, append to ppkt */
netwib_err netwox_pkt_append_smbmsghdr_begin(netwib_bool isnaked,
                                             netwib_buf *ppkt);
netwib_err netwox_pkt_append_smbmsghdr_end(netwib_bool isnaked,
                                           netwib_buf *ppkt);

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_decode_smbmsghdr(netwib_constbuf *ppkt,
                                       netwib_bool isnaked,
                                       netwib_bufext *psmbmsg,
                                       netwib_uint32 *pskipsize);
