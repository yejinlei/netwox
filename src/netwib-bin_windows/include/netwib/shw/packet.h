
/*-------------------------------------------------------------*/
/* Name : netwib_pkt_data_display
   Description :
     Print a buffer containing simple data.
   Input parameter(s) :
     *pkt : buffer to print
     encodetype : netwib_encodetype to use
   Input/output parameter(s) :
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_pkt_data_show(netwib_constbuf *ppkt,
                                netwib_encodetype_context *pctx,
                                netwib_encodetype encodetype,
                                netwib_buf *pbuf);
netwib_err netwib_pkt_data_display(netwib_constbuf *ppkt,
                                   netwib_encodetype_context *pctx,
                                   netwib_encodetype encodetype);

/*-------------------------------------------------------------*/
/* Name : netwib_pkt_link_display
   Description :
     Print a buffer containing an Link packet.
   Input parameter(s) :
     *pkt : buffer to print
     hdrencodetype : netwib_encodetype to use for headers
     dataencodetype : netwib_encodetype to use for data
   Input/output parameter(s) :
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_pkt_link_show(netwib_device_dlttype dlttype,
                                netwib_constbuf *ppkt,
                                netwib_encodetype_context *pctx,
                                netwib_encodetype hdrencodetype,
                                netwib_encodetype dataencodetype,
                                netwib_buf *pbuf);
netwib_err netwib_pkt_link_display(netwib_device_dlttype dlttype,
                                   netwib_constbuf *ppkt,
                                   netwib_encodetype_context *pctx,
                                   netwib_encodetype hdrencodetype,
                                   netwib_encodetype dataencodetype);

/*-------------------------------------------------------------*/
/* Print a buffer containing an IP packet
   Note: currently, IPv6 packets are not decoded, but are simply
         dumped using dataencodetype.
 */
netwib_err netwib_pkt_ip_show(netwib_constbuf *ppkt,
                              netwib_encodetype_context *pctx,
                              netwib_encodetype hdrencodetype,
                              netwib_encodetype dataencodetype,
                              netwib_buf *pbuf);
netwib_err netwib_pkt_ip_display(netwib_constbuf *ppkt,
                                 netwib_encodetype_context *pctx,
                                 netwib_encodetype hdrencodetype,
                                 netwib_encodetype dataencodetype);

/*-------------------------------------------------------------*/
/* Print a buffer containing an UDP packet */
netwib_err netwib_pkt_udp_show(netwib_constbuf *ppkt,
                               netwib_encodetype_context *pctx,
                               netwib_encodetype hdrencodetype,
                               netwib_encodetype dataencodetype,
                               netwib_buf *pbuf);
netwib_err netwib_pkt_udp_display(netwib_constbuf *ppkt,
                                  netwib_encodetype_context *pctx,
                                  netwib_encodetype hdrencodetype,
                                  netwib_encodetype dataencodetype);

/*-------------------------------------------------------------*/
/* Print a buffer containing a TCP packet */
netwib_err netwib_pkt_tcp_show(netwib_constbuf *ppkt,
                               netwib_encodetype_context *pctx,
                               netwib_encodetype hdrencodetype,
                                     netwib_encodetype dataencodetype,
                               netwib_buf *pbuf);
netwib_err netwib_pkt_tcp_display(netwib_constbuf *ppkt,
                                  netwib_encodetype_context *pctx,
                                  netwib_encodetype hdrencodetype,
                                  netwib_encodetype dataencodetype);

/*-------------------------------------------------------------*/
/* Print a buffer containing an ICMP4 packet */
netwib_err netwib_pkt_icmp4_show(netwib_constbuf *ppkt,
                                 netwib_encodetype_context *pctx,
                                 netwib_encodetype hdrencodetype,
                                 netwib_encodetype dataencodetype,
                                 netwib_buf *pbuf);
netwib_err netwib_pkt_icmp4_display(netwib_constbuf *ppkt,
                                    netwib_encodetype_context *pctx,
                                    netwib_encodetype hdrencodetype,
                                    netwib_encodetype dataencodetype);

/*-------------------------------------------------------------*/
/* Print a buffer containing an ICMP6 packet */
netwib_err netwib_pkt_icmp6_show(netwib_constbuf *ppkt,
                                 netwib_encodetype_context *pctx,
                                 netwib_encodetype hdrencodetype,
                                 netwib_encodetype dataencodetype,
                                 netwib_buf *pbuf);
netwib_err netwib_pkt_icmp6_display(netwib_constbuf *ppkt,
                                    netwib_encodetype_context *pctx,
                                    netwib_encodetype hdrencodetype,
                                    netwib_encodetype dataencodetype);


