
/*-------------------------------------------------------------*/
/* decode a neighbor solicitation query and
   prepare a neighbor advertisement answer */
netwib_err netwox_icmp6nd_alive(netwib_consteth *plocaleth,
                                netwib_constip *plocalip,
                                netwib_constlinkhdr *psniffedlinkhdr,
                                netwib_constiphdr *psniffediphdr,
                                netwib_consticmp6 *psniffedicmp6,
                                netwib_bool *ppktset,
                                netwib_buf *ppkt);

/*-------------------------------------------------------------*/
/* prepare a neighbor solicitation query */
netwib_err netwox_icmp6nd_ns(netwib_consteth *plocaleth,
                             netwib_constip *plocalip,
                             netwib_constip *pqueriedip,
                             netwib_buf *ppkt);

/* decode a neighbor advertisement answer */
netwib_err netwox_icmp6nd_na(netwib_constip *pwantedip,
                             netwib_constlinkhdr *psniffedlinkhdr,
                             netwib_constiphdr *psniffediphdr,
                             netwib_consticmp6 *psniffedicmp6,
                             netwib_bool *pobtainedethset,
                             netwib_eth *pobtainedeth);
