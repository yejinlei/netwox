
/*-------------------------------------------------------------*/
netwib_err netwox_dnshdr_show(netwib_bufpool *pbufpool,
                              netwox_constdnshdr *pdnshdr,
                              netwib_encodetype encodetype,
                              netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrr_show(netwib_bufpool *pbufpool,
                             netwox_constdnsrr *pdnsrr,
                             netwib_encodetype encodetype,
                             netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_dns_show(netwib_bufpool *pbufpool,
                           netwox_constdnshdr *pdnshdr,
                           netwib_ring *prrring,
                           netwib_encodetype encodetype,
                           netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_dns_show(netwib_bufpool *pbufpool,
                               netwib_constbuf *ppkt,
                               netwib_encodetype_context *pctx,
                               netwib_encodetype dnsencodetype,
                               netwib_encodetype dataencodetype,
                               netwib_buf *pbuf);
netwib_err netwox_pkt_dns_display(netwib_bufpool *pbufpool,
                                  netwib_constbuf *ppkt,
                                  netwib_encodetype_context *pctx,
                                  netwib_encodetype dnsencodetype,
                                  netwib_encodetype dataencodetype);


