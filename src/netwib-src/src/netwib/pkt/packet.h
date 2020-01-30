
/*-------------------------------------------------------------*/
/***************************************************************
 * Following functions create/decode packets. Their names      *
 * should be self explaining. They are specialized in decoding *
 * one kind of packet. To support several kind of packets, use *
 * netwib_pkt_decode_layer_abc instead.                        *
 ***************************************************************/

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_linkdata(netwib_constlinkhdr *plinkhdr,
                                      netwib_constbuf *pdata,
                                      netwib_buf *ppkt);
netwib_err netwib_pkt_decode_linkdata(netwib_device_dlttype dlttype,
                                      netwib_constbuf *ppkt,
                                      netwib_linkhdr *plinkhdr,
                                      netwib_bufext *pdata);

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_ipdata(netwib_constiphdr *piphdr,
                                    netwib_constbuf *pdata,
                                    netwib_buf *ppkt);
netwib_err netwib_pkt_decode_ipdata(netwib_constbuf *ppkt,
                                    netwib_iphdr *piphdr,
                                    netwib_bufext *pdata);
netwib_err netwib_pkt_append_linkipdata(netwib_constlinkhdr *plinkhdr,
                                        netwib_constiphdr *piphdr,
                                        netwib_constbuf *pdata,
                                        netwib_buf *ppkt);
netwib_err netwib_pkt_decode_linkipdata(netwib_device_dlttype dlttype,
                                        netwib_constbuf *ppkt,
                                        netwib_linkhdr *plinkhdr,
                                        netwib_iphdr *piphdr,
                                        netwib_bufext *pdata);

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_ipudpdata(netwib_constiphdr *piphdr,
                                       netwib_constudphdr *pudphdr,
                                       netwib_constbuf *pdata,
                                       netwib_buf *ppkt);
netwib_err netwib_pkt_decode_ipudpdata(netwib_constbuf *ppkt,
                                       netwib_iphdr *piphdr,
                                       netwib_udphdr *pudphdr,
                                       netwib_bufext *pdata);
netwib_err netwib_pkt_append_linkipudpdata(netwib_constlinkhdr *plinkhdr,
                                           netwib_constiphdr *piphdr,
                                           netwib_constudphdr *pudphdr,
                                           netwib_constbuf *pdata,
                                           netwib_buf *ppkt);
netwib_err netwib_pkt_decode_linkipudpdata(netwib_device_dlttype dlttype,
                                           netwib_constbuf *ppkt,
                                           netwib_linkhdr *plinkhdr,
                                           netwib_iphdr *piphdr,
                                           netwib_udphdr *pudphdr,
                                           netwib_bufext *pdata);

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_iptcpdata(netwib_constiphdr *piphdr,
                                       netwib_consttcphdr *ptcphdr,
                                       netwib_constbuf *pdata,
                                       netwib_buf *ppkt);
netwib_err netwib_pkt_decode_iptcpdata(netwib_constbuf *ppkt,
                                       netwib_iphdr *piphdr,
                                       netwib_tcphdr *ptcphdr,
                                       netwib_bufext *pdata);
netwib_err netwib_pkt_append_linkiptcpdata(netwib_constlinkhdr *plinkhdr,
                                           netwib_constiphdr *piphdr,
                                           netwib_consttcphdr *ptcphdr,
                                           netwib_constbuf *pdata,
                                           netwib_buf *ppkt);
netwib_err netwib_pkt_decode_linkiptcpdata(netwib_device_dlttype dlttype,
                                           netwib_constbuf *ppkt,
                                           netwib_linkhdr *plinkhdr,
                                           netwib_iphdr *piphdr,
                                           netwib_tcphdr *ptcphdr,
                                           netwib_bufext *pdata);

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_ipicmp4(netwib_constiphdr *piphdr,
                                     netwib_consticmp4 *picmp4,
                                     netwib_buf *ppkt);
netwib_err netwib_pkt_decode_ipicmp4(netwib_constbuf *ppkt,
                                     netwib_iphdr *piphdr,
                                     netwib_icmp4 *picmp4);
netwib_err netwib_pkt_append_linkipicmp4(netwib_constlinkhdr *plinkhdr,
                                         netwib_constiphdr *piphdr,
                                         netwib_consticmp4 *picmp4,
                                         netwib_buf *ppkt);
netwib_err netwib_pkt_decode_linkipicmp4(netwib_device_dlttype dlttype,
                                         netwib_constbuf *ppkt,
                                         netwib_linkhdr *plinkhdr,
                                         netwib_iphdr *piphdr,
                                         netwib_icmp4 *picmp4);

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_ipicmp6(netwib_constiphdr *piphdr,
                                     netwib_consticmp6 *picmp6,
                                     netwib_buf *ppkt);
netwib_err netwib_pkt_decode_ipicmp6(netwib_constbuf *ppkt,
                                     netwib_iphdr *piphdr,
                                     netwib_icmp6 *picmp6);
netwib_err netwib_pkt_append_linkipicmp6(netwib_constlinkhdr *plinkhdr,
                                         netwib_constiphdr *piphdr,
                                         netwib_consticmp6 *picmp6,
                                         netwib_buf *ppkt);
netwib_err netwib_pkt_decode_linkipicmp6(netwib_device_dlttype dlttype,
                                         netwib_constbuf *ppkt,
                                         netwib_linkhdr *plinkhdr,
                                         netwib_iphdr *piphdr,
                                         netwib_icmp6 *picmp6);

/*-------------------------------------------------------------*/
/* before calling this function, user has to set plinkhdr->type
   to ARP or RARP */
netwib_err netwib_pkt_append_linkarp(netwib_constlinkhdr *plinkhdr,
                                     netwib_constarphdr *parphdr,
                                     netwib_buf *ppkt);
netwib_err netwib_pkt_decode_linkarp(netwib_device_dlttype dlttype,
                                     netwib_constbuf *ppkt,
                                     netwib_linkhdr *plinkhdr,
                                     netwib_arphdr *parphdr);
