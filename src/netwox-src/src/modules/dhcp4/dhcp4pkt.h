
/*-------------------------------------------------------------*/
netwib_err netwox_pkt_dhcp4_show(netwib_constbuf *ppkt,
                                 netwib_encodetype_context *pctx,
                                 netwib_encodetype dhcp4encodetype,
                                 netwib_encodetype dataencodetype,
                                 netwib_buf *pbuf);
netwib_err netwox_pkt_dhcp4_display(netwib_bufpool *pbufpool,
                                    netwib_constbuf *ppkt,
                                    netwib_encodetype_context *pctx,
                                    netwib_encodetype dhcp4encodetype,
                                    netwib_encodetype dataencodetype);

