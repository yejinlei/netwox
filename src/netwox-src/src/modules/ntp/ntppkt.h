
/*-------------------------------------------------------------*/
netwib_err netwox_pkt_ntp_show(netwib_constbuf *ppkt,
                               netwib_encodetype_context *pctx,
                               netwib_encodetype ntpencodetype,
                               netwib_encodetype dataencodetype,
                               netwib_buf *pbuf);
netwib_err netwox_pkt_ntp_display(netwib_bufpool *pbufpool,
                                  netwib_constbuf *ppkt,
                                  netwib_encodetype_context *pctx,
                                  netwib_encodetype ntpencodetype,
                                  netwib_encodetype dataencodetype);

