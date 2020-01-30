
/*-------------------------------------------------------------*/
/* rfc 1034 and 1035 : DNS
   rfc 1886(AAAA) and 2874(A6) : IPv6
   rfc 2671 is not implemented, but partially decoded (see
   NETWOX_DNSRR_TYPE_OPT)
 */

/*-------------------------------------------------------------*/
/* do not construct ip6.arpa (rfc 2874) but ip6.int (rfc 1886)
   for IPv6 addresses (because cannot construct ip6.arpa
   without prefix length prefix name, structure, etc.) */
netwib_err netwox_dns_inaddrarpa(netwib_constip *pip,
                                 netwib_buf *pbuf);

/*-------------------------------------------------------------*/
typedef netwib_err (*netwox_dns_query_pf)(netwib_bufpool *pbufpool,
                                          netwib_constbuf *pdnspkt,
                                          netwib_ptr infos);
netwib_err netwox_dns_query(netwib_bufpool *pbufpool,
                            netwib_io *pio,
                            netwib_bool ioistcp,
                            netwib_constbuf *pquestion,
                            netwib_consttime *pabstime,
                            netwox_dns_query_pf pfunc,
                            netwib_ptr infos);
