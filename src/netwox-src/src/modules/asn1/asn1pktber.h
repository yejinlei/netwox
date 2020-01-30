
/*-------------------------------------------------------------*/
netwib_err netwox_pktber_append_asn1data(netwox_constasn1data *pasn1data,
                                         netwib_buf *ppkt);
/* netwox_asn1data_init has to be called before */
netwib_err netwox_pktber_decode_asn1data(netwox_asn1tag_ext ext,
                                         netwox_asn1tag_ctx *pctx,
                                         netwib_buf *ppkt,
                                         netwox_asn1data *pasn1data);

/*-------------------------------------------------------------*/
/* netwox_asn1data_init has to be called before */
netwib_err netwox_pktber_decode_asn1data_snmp(netwib_buf *ppkt,
                                              netwox_asn1data *pasn1data);
netwib_err netwox_pktber_decode_asn1data_snmp3sp(netwib_buf *ppkt,
                                                 netwox_asn1data *pasn1data);

/*-------------------------------------------------------------*/
/* only needed to test implementation */
netwib_err netwox_pktber_test(netwox_asn1tag_ext ext,
                              netwib_constbuf *ppkt);
netwib_err netwox_pktber_test_static(void);
netwib_err netwox_pktber_test_samples(void);

