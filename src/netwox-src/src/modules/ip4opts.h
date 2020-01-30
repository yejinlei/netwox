
/*-------------------------------------------------------------*/
netwib_err netwox_ip4opts_pkt_noop(netwib_buf *ppkt);

/*-------------------------------------------------------------*/
netwib_err netwox_ip4opts_pkt_rr(netwib_buf *ppkt);
netwib_err netwox_ip4opts_pkt_lsrr(netwib_constips *pips,
                                   netwib_buf *ppkt);
netwib_err netwox_ip4opts_pkt_ssrr(netwib_constips *pips,
                                   netwib_buf *ppkt);

/*-------------------------------------------------------------*/
netwib_err netwox_ip4opts_pkt_ts(netwib_buf *ppkt);
netwib_err netwox_ip4opts_pkt_ipts(netwib_buf *ppkt);
netwib_err netwox_ip4opts_pkt_ippts(netwib_constip *pip,
                                    netwib_buf *ppkt);
