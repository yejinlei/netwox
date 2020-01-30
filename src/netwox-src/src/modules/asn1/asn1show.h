
/*-------------------------------------------------------------*/
netwib_err netwox_asn1data_show(netwox_constasn1data *pasn1data,
                                netwib_encodetype encodetype,
                                netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_asn1_show(netwib_bufpool *pbufpool,
                                netwox_asn1tag_ext ext,
                                netwib_constbuf *ppkt,
                                netwib_encodetype_context *pctx,
                                netwib_encodetype asn1encodetype,
                                netwib_encodetype dataencodetype,
                                netwib_buf *pbuf);
netwib_err netwox_pkt_asn1_display(netwib_bufpool *pbufpool,
                                   netwox_asn1tag_ext ext,
                                   netwib_constbuf *ppkt,
                                   netwib_encodetype_context *pctx,
                                   netwib_encodetype asn1encodetype,
                                   netwib_encodetype dataencodetype);


