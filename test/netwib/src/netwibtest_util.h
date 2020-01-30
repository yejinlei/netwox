
/*-------------------------------------------------------------*/
netwib_err netwibtest_data_print_string(netwib_constdata data,
                                        netwib_uint32 datasize);
netwib_err netwibtest_data_print_mixed(netwib_constdata data,
                                       netwib_uint32 datasize);
netwib_err netwibtest_data_print_hexa(netwib_constdata data,
                                      netwib_uint32 datasize);

/*-------------------------------------------------------------*/
netwib_err netwibtest_buf_content(netwib_constbuf *pbuf,
                                  netwib_conststring wanted,
                                  netwib_uint32 totalsize,
                                  netwib_uint32 beginoffset,
                                  netwib_uint32 endoffset);
netwib_err netwibtest_buf_content2(netwib_constbuf *pbuf,
                                   netwib_conststring wantedstring);
netwib_err netwibtest_buf_content3(netwib_constbuf *pbuf,
                                   netwib_conststring wantedmixed);
netwib_err netwibtest_buf_content4(netwib_constbuf *pbuf,
                                   netwib_uint16 wantedchecksum);

netwib_err netwibtest_ip_content(netwib_constip *pip,
                                 netwib_conststring wantedstring);
netwib_err netwibtest_eth_content(netwib_consteth *peth,
                                  netwib_conststring wantedstring);

netwib_err netwibtest_eths_content(netwib_eths *peths,
                                   netwib_conststring wantedstring);
netwib_err netwibtest_ips_content(netwib_ips *pips,
                                  netwib_conststring wantedstring);
netwib_err netwibtest_ports_content(netwib_ports *pports,
                                    netwib_conststring wantedstring);

netwib_err netwibtest_linkhdr_content(netwib_constlinkhdr *plinkhdr,
                                      netwib_conststring wantedmixed);
netwib_err netwibtest_iphdr_content(netwib_constiphdr *piphdr,
                                    netwib_conststring wantedmixed);
netwib_err netwibtest_udphdr_content(netwib_constudphdr *pudphdr,
                                     netwib_conststring wantedmixed);
netwib_err netwibtest_tcphdr_content(netwib_consttcphdr *ptcphdr,
                                     netwib_conststring wantedmixed);
netwib_err netwibtest_arphdr_content(netwib_constarphdr *parphdr,
                                     netwib_conststring wantedmixed);
netwib_err netwibtest_ip4opt_content(netwib_constip4opt *pip4opt,
                                     netwib_conststring wantedmixed);
netwib_err netwibtest_ip6ext_content(netwib_constip6ext *pip6ext,
                                     netwib_conststring wantedmixed);
netwib_err netwibtest_tcpopt_content(netwib_consttcpopt *ptcpopt,
                                     netwib_conststring wantedmixed);
netwib_err netwibtest_icmp4_content(netwib_consticmp4 *picmp4,
                                    netwib_conststring wantedmixed);
netwib_err netwibtest_icmp6_content(netwib_consticmp6 *picmp6,
                                    netwib_conststring wantedmixed);
netwib_err netwibtest_icmp6nd_content(netwib_consticmp6nd *picmp6nd,
                                      netwib_conststring wantedmixed);
netwib_err netwibtest_uint64_content(netwib_uint64 ui,
                                     netwib_conststring wanteddecimal);
netwib_err netwibtest_int64_content(netwib_int64 i,
                                    netwib_conststring wanteddecimal);

/*-------------------------------------------------------------*/
netwib_err netwibtest_linkhdr_init(netwib_linkhdr *plinkhdr);
netwib_err netwibtest_ip4hdr_init(netwib_iphdr *piphdr);
netwib_err netwibtest_ip6hdr_init(netwib_iphdr *piphdr);
netwib_err netwibtest_udphdr_init(netwib_udphdr *pudphdr);
netwib_err netwibtest_tcphdr_init(netwib_tcphdr *ptcphdr);
netwib_err netwibtest_arphdr_init(netwib_arphdr *parphdr);
/* starts with NETWIB_IPPROTO_HOPOPTS */
netwib_err netwibtest_ip6exts_init(netwib_ipproto lastproto,
                                   netwib_buf *ppkt);
netwib_err netwibtest_icmp4_init(netwib_icmp4 *picmp4);
netwib_err netwibtest_icmp6_init(netwib_icmp6 *picmp6);

/*-------------------------------------------------------------*/
netwib_err netwibtest_uint64_init(netwib_conststring hexavalue,
                                  netwib_uint64 *pui);
netwib_err netwibtest_int64_init(netwib_conststring hexavalue,
                                 netwib_int64 *pi);

/*-------------------------------------------------------------*/
netwib_err netwibtest_something_to_do(netwib_conststring msg);

/*-------------------------------------------------------------*/
netwib_err netwibtest_choose_device(netwib_buf *pbuf);
