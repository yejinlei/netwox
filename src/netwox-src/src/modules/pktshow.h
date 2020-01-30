

/*-------------------------------------------------------------*/
/*
  This module is adapted from netwib/shw/packet.c.
  Cross changes are checked using a private script to ensure they
  are both updated when one changes.
*/

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_link_show(netwib_bufpool *pbufpool,
                                netwib_device_dlttype dlttype,
                                netwib_constbuf *ppkt,
                                netwib_encodetype_context *pctx,
                                netwib_encodetype hdrencodetype,
                                netwib_encodetype dataencodetype,
                                netwib_buf *pbuf);
netwib_err netwox_pkt_link_display(netwib_bufpool *pbufpool,
                                   netwib_device_dlttype dlttype,
                                   netwib_constbuf *ppkt,
                                   netwib_encodetype_context *pctx,
                                   netwib_encodetype hdrencodetype,
                                   netwib_encodetype dataencodetype);

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_ip_show(netwib_bufpool *pbufpool,
                              netwib_constbuf *ppkt,
                              netwib_encodetype_context *pctx,
                              netwib_encodetype hdrencodetype,
                              netwib_encodetype dataencodetype,
                              netwib_buf *pbuf);
netwib_err netwox_pkt_ip_display(netwib_bufpool *pbufpool,
                                 netwib_constbuf *ppkt,
                                 netwib_encodetype_context *pctx,
                                 netwib_encodetype hdrencodetype,
                                 netwib_encodetype dataencodetype);

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_udp_show(netwib_bufpool *pbufpool,
                               netwib_constbuf *ppkt,
                               netwib_encodetype_context *pctx,
                               netwib_encodetype hdrencodetype,
                               netwib_encodetype dataencodetype,
                               netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_tcp_show(netwib_bufpool *pbufpool,
                               netwib_constbuf *ppkt,
                               netwib_encodetype_context *pctx,
                               netwib_encodetype hdrencodetype,
                               netwib_encodetype dataencodetype,
                               netwib_buf *pbuf);
