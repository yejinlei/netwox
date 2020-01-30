
/*-------------------------------------------------------------*/
netwib_err netwox_alive_ether(netwib_io *pspoofio, /* link level */
                              netwib_consteth *peth,
                              netwib_constip *pip,
                              netwib_constbuf *ppkt,
                              netwib_bool answerarp,
                              netwib_bool answerping,
                              netwib_bool answertcp,
                              netwib_bool *panswered);

/*-------------------------------------------------------------*/
/* answer to ARP request */
netwib_err netwox_alive_linkarp(netwib_io *pspoofio, /* link level */
                                netwib_consteth *peth,
                                netwib_constip *pip,
                                netwib_constlinkhdr *plinkhdr,
                                netwib_constarphdr *parphdr,
                                netwib_bool *panswered);
/* answer to IPv6 neighbor solicitation */
netwib_err netwox_alive_linkipicmp6nd(netwib_io *pspoofio, /* link level */
                                      netwib_consteth *peth,
                                      netwib_constip *pip,
                                      netwib_constlinkhdr *plinkhdr,
                                      netwib_constiphdr *piphdr,
                                      netwib_consticmp6 *picmp6,
                                      netwib_bool *panswered);

/*-------------------------------------------------------------*/
/* answer to ping */
netwib_err netwox_alive_linkipicmp4ping(netwib_io *pspoofio, /* link level */
                                        netwib_consteth *peth,
                                        netwib_constip *pip,
                                        netwib_constlinkhdr *plinkhdr,
                                        netwib_constiphdr *piphdr,
                                        netwib_consticmp4 *picmp4,
                                        netwib_bool *panswered);
netwib_err netwox_alive_linkipicmp6ping(netwib_io *pspoofio, /* link level */
                                        netwib_consteth *peth,
                                        netwib_constip *pip,
                                        netwib_constlinkhdr *plinkhdr,
                                        netwib_constiphdr *piphdr,
                                        netwib_consticmp6 *picmp6,
                                        netwib_bool *panswered);

/*-------------------------------------------------------------*/
/* reset TCP SYN-ACK */
netwib_err netwox_alive_linkiptcpdata(netwib_io *pspoofio, /* link level */
                                      netwib_consteth *peth,
                                      netwib_constip *pip,
                                      netwib_constlinkhdr *plinkhdr,
                                      netwib_constiphdr *piphdr,
                                      netwib_consttcphdr *ptcphdr,
                                      netwib_constbuf *ptcpdata,
                                      netwib_bool *panswered);

