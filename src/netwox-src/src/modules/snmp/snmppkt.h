
/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_snmpmsg(netwox_constsnmpmsg *psnmpmsg,
                                     netwib_buf *ppkt);
/* netwox_snmpmsg_init must be called before */
netwib_err netwox_pkt_decode_snmpmsg(netwib_buf *ppkt,
                                     netwox_snmpmsg *psnmpmsg);

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_snmp_show(netwib_bufpool *pbufpool,
                                netwib_constbuf *ppkt,
                                netwib_encodetype_context *pctx,
                                netwib_encodetype snmpencodetype,
                                netwib_encodetype dataencodetype,
                                netwib_buf *pbuf);
netwib_err netwox_pkt_snmp_display(netwib_bufpool *pbufpool,
                                   netwib_constbuf *ppkt,
                                   netwib_encodetype_context *pctx,
                                   netwib_encodetype snmpencodetype,
                                   netwib_encodetype dataencodetype);

/*-------------------------------------------------------------*/
/* only needed to test implementation */
netwib_err netwox_snmppkt_test(netwib_constbuf *ppkt);
netwib_err netwox_snmppkt_test_static(void);
netwib_err netwox_snmppkt_test_samples(void);
