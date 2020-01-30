
/*-------------------------------------------------------------*/
netwib_err netwox_pkt_iptype_init_ippkt(netwib_constbuf *pippkt,
                                        netwib_iptype *piptype);

/*-------------------------------------------------------------*/
netwib_err netwox_iphdr_isfrag(netwib_constiphdr *piphdr,
                               netwib_bool *pisfrag);

/*-------------------------------------------------------------*/
netwib_err netwox_ip6exts_decode_para(netwib_constbuf *pip6extsin,
                                      netwib_ipproto *pipproto,
                                      netwib_bufext *pip6extsout);
