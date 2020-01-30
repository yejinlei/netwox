
/*-------------------------------------------------------------*/
netwib_err netwib_priv_pkt_convert(netwib_device_dlttype dlttypein,
                                   netwib_constbuf *ppktin,
                                   netwib_pkt_newfield newfieldopt,
                                   netwib_pkt_newfield newfieldreq,
                                   netwib_ip *pgw, /* NULL if unknown */
                                   netwib_device_dlttype dlttypeout,
                                   netwib_linkhdr *plinkhdrout,
                                   netwib_bufext *ppktout,
                                   netwib_bool *pdevnumset,
                                   netwib_uint32 *pdevnum);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ippkt_decode_iptype(netwib_constbuf *ppkt,
                                           netwib_iptype *piptype);
netwib_err netwib_priv_ippkt_decode_srcdst(netwib_constbuf *ppkt,
                                           netwib_ip *psrc,
                                           netwib_ip *pdst);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ip6exts_skip_ip6ext(netwib_ipproto pktproto,
                                           netwib_constbuf *ppkt,
                                           netwib_ipproto *pnextproto,
                                           netwib_uint32 *pskipsize);
netwib_err netwib_priv_ip6exts_skip_notfrag(netwib_ipproto pktproto,
                                            netwib_constbuf *ppkt,
                                            netwib_uint32 *pskipsize);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_icmp4_size(netwib_consticmp4 *picmp4,
                                  netwib_uint32 *psize);
netwib_err netwib_priv_icmp6_size(netwib_consticmp6 *picmp6,
                                  netwib_uint32 *psize);

/*-------------------------------------------------------------*/
#define NETWIB_PRIV_IP4HDR_MINLEN 20
#define NETWIB_PRIV_IP4HDR_MAXLEN 64
#define NETWIB_PRIV_IP6HDR_MINLEN 40
