
/*-------------------------------------------------------------*/
netwib_err netwib_priv_ip_ip4_init_ip6(netwib_constip6 *pip6,
                                       netwib_ip4 *pip4);
netwib_err netwib_priv_ip_ip6_init_ip4(netwib_ip4 ip4,
                                       netwib_ip6 *pip6);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ip_init_sip4(netwib_conststring str,
                                    netwib_ip *pip);
netwib_err netwib_priv_ip_init_sip6(netwib_conststring str,
                                    netwib_ip *pip);
netwib_err netwib_priv_ip_init_hn4(netwib_conststring str,
                                   netwib_ip *pip);
netwib_err netwib_priv_ip_init_hn6(netwib_conststring str,
                                   netwib_ip *pip);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ip_buf_append_ip4(netwib_constip *pip,
                                         netwib_buf *pbuf);
netwib_err netwib_priv_ip_buf_append_ip6(netwib_constip *pip,
                                         netwib_buf *pbuf);
netwib_err netwib_priv_ip_buf_append_hn(netwib_constip *pip,
                                        netwib_buf *pbuf);
netwib_err netwib_priv_ip_buf_append_hns(netwib_constip *pip,
                                         netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ip_ip6_supported(netwib_bool *pyes);

/*-------------------------------------------------------------*/
/* pip=1.2.3.4, pmask=255.255.255.0 => pnet=1.2.3.0 */
netwib_err netwib_priv_ip_net_init_ipmaskprefix(netwib_constip *pip,
                                                netwib_constip *pmask,
                                                netwib_uint32 prefix,
                                                netwib_ip *pnet);
/* check if two IP are in the same network */
netwib_err netwib_priv_ip_net_cmp_ipmaskprefix(netwib_constip *pip1,
                                               netwib_constip *pip2,
                                               netwib_constip *pmask,
                                               netwib_uint32 prefix,
                                               netwib_cmp *pcmp);

/*-------------------------------------------------------------*/
/* pip=1.2.3.130, pnet=1.2.3.128 => pmask=255.255.255.128 (guess) */
netwib_err netwib_priv_ip_maskprefix_init_ipnet(netwib_constip *pip,
                                                netwib_constip *pnet,
                                                netwib_ip *pmask,
                                                netwib_uint32 *pprefix);
/* socka:data=0xFFFFFFF00 => pmask=255.255.255.0 */
netwib_err netwib_priv_ip_maskprefix_init_sali(const netwib_priv_sockaddr_unalign *psa,
                                               netwib_priv_sockaddr_len supposedsalen,
                                               netwib_iptype supposediptype,
                                               netwib_ip *pmask,
                                               netwib_uint32 *pprefix);
#define netwib_priv_ip_maskprefix_init_sal(psa,supposedsalen,pmask,pprefix) netwib_priv_ip_maskprefix_init_sali(psa,supposedsalen,NETWIB_IPTYPE_UNKNOWN,pmask,pprefix)
#define netwib_priv_ip_maskprefix_init_sai(psa,supposediptype,pmask,pprefix) netwib_priv_ip_maskprefix_init_sali(psa,0xFFFFFFFFu,supposediptype,pmask,pprefix)
#define netwib_priv_ip_maskprefix_init_sa(psa,pmask,pprefix) netwib_priv_ip_maskprefix_init_sali(psa,0xFFFFFFFFu,NETWIB_IPTYPE_UNKNOWN,pmask,pprefix)
/* prefix=(ip4)24 => pmask=0xFFFFFF00, pprefix=prefix
   prefix=(ip6)24 => pmask=0xFFFFFF00, pprefix=prefix
   prefix=(ip4)44 => error
 */
netwib_err netwib_priv_ip_maskprefix_init_prefix(netwib_iptype iptype,
                                                 netwib_uint32 prefix,
                                                 netwib_ip *pmask,
                                                 netwib_uint32 *pprefix);

/*-------------------------------------------------------------*/
/* pbuf="1.2.3/24" => pnet=1.2.3.0, pmask=255.255.255.0
   pbuf="1.2.3/255.255.255.0" => pnet=1.2.3.0, pmask=255.255.255.0
   pbuf="1.0.3/255.0.255.0" => pnet=1.2.3.0, pmask=255.0.255.0
   pbuf="1.2.3.4" => pnet=1.2.3.4, pmask=255.255.255.255
   pbuf="1.2.3" => pnet=1.2.3.0, pmask=255.255.255.0 (guess)
*/
netwib_err netwib_priv_ip_netmaskprefix_init_buf(netwib_constbuf *pbuf,
                                                 netwib_ip *pnet,
                                                 netwib_ip *pmask,
                                                 netwib_uint32 *pprefix);
#define netwib_priv_ip_net_init_buf(pbuf,pnet) netwib_priv_ip_netmaskprefix_init_buf(pbuf,pnet,NULL,NULL)
/* idem, but accept "default". iptype is needed because default
   can be for IPv4 or IPv6 */
netwib_err netwib_priv_ip_netmaskprefix_init_bufdefault(netwib_iptype iptype,
                                                        netwib_constbuf *pbuf,
                                                        netwib_ip *pnet,
                                                        netwib_ip *pmask,
                                                       netwib_uint32 *pprefix);
#define netwib_priv_ip_net_init_bufdefault(iptype,pbuf,pnet) netwib_priv_ip_netmaskprefix_init_bufdefault(iptype,pbuf,pnet,NULL,NULL)

/*-------------------------------------------------------------*/
/* pmask=(ip6)0xFFFFFFFFFFFFFFF00000... => pprefix=60
   pmask=(ip6)0xFF0FFFFFFFFFFFF00000... => pprefix=8 (even if invalid)
   pmask=(ip4)0xFFFFF000 => pprefix=20
   pmask=(ip4)0xFF0FF000 => pprefix=8 (even if invalid)
*/
netwib_err netwib_priv_ip_prefix_init_mask(netwib_constip *pmask,
                                           netwib_uint32 *pprefix);
