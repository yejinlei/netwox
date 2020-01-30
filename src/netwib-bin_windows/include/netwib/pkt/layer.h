
/*-------------------------------------------------------------*/
/***************************************************************
 * Following functions create/decode layers. Their names       *
 * should be self explaining.                                  *
 * Note : when decoding a layer, ppkt buffer is automatically  *
 *        shifted to point to next entry.                      *
 ***************************************************************/

/*-------------------------------------------------------------*/
/***************************************************************
 * Some header fields are computed by functions append_layer:  *
 *  - linkhdr.ethhdr.type                                      *
 *  - iphdr.protocol                                           *
 *  - iphdr.header.ip4.ihl                                     *
 *  - iphdr.header.ip4.totlen                                  *
 *  - iphdr.header.ip4.check                                   *
 *  - iphdr.header.ip6.payloadlength                           *
 *  - udphdr.len                                               *
 *  - udphdr.check                                             *
 *  - tcphdr.doff                                              *
 *  - tcphdr.check                                             *
 *  - icmp4hdr.check                                           *
 * To use fake values for those fields, they have to be edited *
 * after packet construction.                                  *
 ***************************************************************/

/*-------------------------------------------------------------*/
/***************************************************************
 * Which functions to use ?                                    *
 *                                                             *
 * Netwib has 3 kind of functions to decode packets:           *
 *  - netwib_pkt_decode_abc ie netwib_pkt_decode_iphdr         *
 *  - netwib_pkt_decode_layer_abc ie netwib_pkt_decode_layer_ip*
 *  - netwib_pkt_decode_abcABC ie netwib_pkt_decode_iptcp      *
 * Functions netwib_pkt_decode_abc and                         *
 * netwib_pkt_decode_layer_abc are almost equivalent.          *
 * Functions netwib_pkt_decode_abcABC permits to decode only   *
 * one kind of protocol : ABC on top of abc. So, they can only *
 * be used in simple cases.                                    *
 *                                                             *
 * Netwib has 3 kind of functions to create packets:           *
 *  - netwib_pkt_append_abc ie netwib_pkt_append_iphdr         *
 *  - netwib_pkt_append_layer_abc ie netwib_pkt_append_layer_ip*
 *  - netwib_pkt_append_abcABC ie netwib_pkt_append_iptcp      *
 * Functions netwib_pkt_append_abc use exact fields given as   *
 * parameters from users.                                      *
 * Functions netwib_pkt_append_layer_abc compute fields before *
 * storing them in the packet.                                 *
 * Functions netwib_pkt_append_abcABC permits to create only   *
 * one kind of protocol : ABC on top of abc. So, they can only *
 * be used in simple cases.                                    *
 *                                                             *
 * Summary: when possible, use netwib_pkt_append/decode_layer  *
 *          which are :                                        *
 *           - easier than netwib_pkt_append/decode_abc        *
 *             because they compute fields                     *
 *           - faster than netwib_pkt_append/decode_abcABC     *
 *             if several protocols of the same layer have to  *
 *             be decoded (for example : iptcp and ipudp)      *
 ***************************************************************/

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_layer_data(netwib_constbuf *pdata,
                                        netwib_buf *ppkt);
netwib_err netwib_pkt_decode_layer_data(netwib_buf *ppkt,
                                        netwib_bufext *pdata);

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_layer_link(netwib_constlinkhdr *plinkhdr,
                                        netwib_buf *ppkt);
netwib_err netwib_pkt_decode_layer_link(netwib_device_dlttype dlttype,
                                        netwib_buf *ppkt,
                                        netwib_linkhdr *plinkhdr);

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_layer_ip(netwib_constiphdr *piphdr,
                                      netwib_uint32 datasize,
                                      netwib_buf *ppkt);
netwib_err netwib_pkt_decode_layer_ip(netwib_buf *ppkt,
                                      netwib_iphdr *piphdr);

/*-------------------------------------------------------------*/
/* fields followed by "checksum" are needed only for checksum
   computation */
netwib_err netwib_pkt_append_layer_udp(netwib_constiphdr *piphdr,/*checksum*/
                                       netwib_constudphdr *pudphdr,
                                       netwib_constbuf *pudpdata,/*checksum*/
                                       netwib_buf *ppkt);
netwib_err netwib_pkt_decode_layer_udp(netwib_buf *ppkt,
                                       netwib_udphdr *pudphdr);

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_layer_tcp(netwib_constiphdr *piphdr,/*checksum*/
                                       netwib_consttcphdr *ptcphdr,
                                       netwib_constbuf *ptcpdata,/*checksum*/
                                       netwib_buf *ppkt);
netwib_err netwib_pkt_decode_layer_tcp(netwib_buf *ppkt,
                                       netwib_tcphdr *ptcphdr);

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_layer_icmp4(netwib_consticmp4 *picmp4,
                                         netwib_buf *ppkt);
netwib_err netwib_pkt_decode_layer_icmp4(netwib_buf *ppkt,
                                         netwib_icmp4 *picmp4);

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_layer_icmp6(netwib_constiphdr *piphdr,
                                         netwib_consticmp6 *picmp6,
                                         netwib_buf *ppkt);
netwib_err netwib_pkt_decode_layer_icmp6(netwib_buf *ppkt,
                                         netwib_icmp6 *picmp6);

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_append_layer_arp(netwib_constarphdr *parphdr,
                                       netwib_buf *ppkt);
netwib_err netwib_pkt_decode_layer_arp(netwib_buf *ppkt,
                                       netwib_arphdr *parphdr);
