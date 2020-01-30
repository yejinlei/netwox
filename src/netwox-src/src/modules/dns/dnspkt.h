
/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_dnshdr(netwox_constdnshdr *pdnshdr,
                                    netwib_buf *ppkt);
netwib_err netwox_pkt_decode_dnshdr(netwib_buf *ppkt,
                                    netwox_dnshdr *pdnshdr);

/*-------------------------------------------------------------*/
/* fullpkt start at DNS ID and is used for compression */
netwib_err netwox_pkt_append_dnsrr(netwib_bufpool *pbufpool,
                                   netwib_constbuf *pfullpkt,
                                   netwox_constdnsrr *pdnsrr,
                                   netwib_buf *ppkt);
netwib_err netwox_pkt_decode_dnsrr(netwib_bufpool *pbufpool,
                                   netwib_constbuf *pfullpkt,
                                   netwib_buf *ppkt,
                                   netwox_dnsrr *pdnsrr);

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_dns(netwib_bufpool *pbufpool,
                                 netwox_constdnshdr *pdnshdr,
                                 netwib_ring *prrring,
                                 netwib_buf *ppkt);
netwib_err netwox_pkt_decode_dns(netwib_bufpool *pbufpool,
                                 netwib_constbuf *ppkt,
                                 netwox_dnshdr *pdnshdr,
                                 netwib_ring *prrring);

/*-------------------------------------------------------------*/
/* for questions only, its a bit easier, so provide functions */
netwib_err netwox_pkt_append_dns_question(netwib_bufpool *pbufpool,
                                          netwox_constdnshdr *pdnshdr,
                                          netwib_constbuf *pname,
                                          netwox_dnsrr_type type,
                                          netwox_dnsrr_class rclass,
                                          netwib_buf *ppkt);
netwib_err netwox_pkt_decode_dns_question(netwib_bufpool *pbufpool,
                                          netwib_constbuf *ppkt,
                                          netwox_dnshdr *pdnshdr,
                                          netwib_buf *pname,
                                          netwox_dnsrr_type *ptype,
                                          netwox_dnsrr_class *pclass);

/*-------------------------------------------------------------*/
/* special for TCP because size is first stored as 2 bytes */
netwib_err netwox_pkt_append_tcpdns_prepare(netwib_buf *ppkt,
                                            netwib_uint32 *pprevioussize);
netwib_err netwox_pkt_append_tcpdns_finish(netwib_buf *ppkt,
                                           netwib_uint32 previoussize,
                                           netwib_err errcode);
netwib_err netwox_pkt_decode_tcpdns(netwib_constbuf *ppkt,
                                    netwib_bufext *pdnspkt,
                                    netwib_uint32 *pskipsize);

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_decode_dnsid(netwib_constbuf *ppkt,
                                   netwib_uint16 *pdnsid);
