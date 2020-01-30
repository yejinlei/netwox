
/*-------------------------------------------------------------*/
/* those functions create examples of DNS packets */

/*-------------------------------------------------------------*/
netwib_err netwox_dnspktex_answer_a(netwib_uint16 id,
                                    netwib_bool authanswer,
                                    netwib_bool recurdesired,
                                    netwib_bool recuravail,
                                    netwib_uint32 ttl,
                                    netwib_constbuf *phostname,
                                    netwib_constip *phostnameip,
                                    netwib_constbuf *pauthns,
                                    netwib_constip *pauthnsip,
                                    netwib_buf *ppkt);

/*-------------------------------------------------------------*/
netwib_err netwox_dnspktex_answer_ns(netwib_uint16 id,
                                     netwib_bool authanswer,
                                     netwib_bool recurdesired,
                                     netwib_bool recuravail,
                                     netwib_uint32 ttl,
                                     netwib_constbuf *pdomain,
                                     netwib_constbuf *pauthns,
                                     netwib_constip *pauthnsip,
                                     netwib_buf *ppkt);

/*-------------------------------------------------------------*/
netwib_err netwox_dnspktex_answer_ptr(netwib_uint16 id,
                                      netwib_bool authanswer,
                                      netwib_bool recurdesired,
                                      netwib_bool recuravail,
                                      netwib_uint32 ttl,
                                      netwib_constbuf *pip,
                                      netwib_constbuf *phostname,
                                      netwib_constbuf *pauthns,
                                      netwib_constip *pauthnsip,
                                      netwib_buf *ppkt);

/*-------------------------------------------------------------*/
netwib_err netwox_dnspktex_answer_mx(netwib_uint16 id,
                                     netwib_bool authanswer,
                                     netwib_bool recurdesired,
                                     netwib_bool recuravail,
                                     netwib_uint32 ttl,
                                     netwib_constbuf *pdomain,
                                     netwib_uint16 preference,
                                     netwib_constbuf *pexchange,
                                     netwib_constbuf *pauthns,
                                     netwib_constip *pauthnsip,
                                     netwib_buf *ppkt);

/*-------------------------------------------------------------*/
netwib_err netwox_dnspktex_answer_unimplemented(netwox_constdnshdr *pdnshdr,
                                                netwib_constbuf *pname,
                                                netwox_dnsrr_type type,
                                                netwox_dnsrr_class rclass,
                                                netwib_buf *ppkt);

/*-------------------------------------------------------------*/
/* always answer the same values */
netwib_err netwox_dnspktex_answer(netwib_constbuf *pquestion,
                                  netwib_bool ignoreunimplemented,
                                  netwib_constbuf *phostname,
                                  netwib_constip *phostnameip,
                                  netwib_constbuf *pauthns,
                                  netwib_constip *pauthnsip,
                                  netwib_uint32 ttl,
                                  netwib_buf *panswer);
