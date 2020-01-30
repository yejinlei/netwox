
/*-------------------------------------------------------------*/
typedef netwib_err (*netwox_webspidermt_pf)(netwox_webspider *pwebspider,
                                            netwib_constbuf *purl,
                                            netwib_constbuf *pcontenttype,
                                            netwib_uint32 recursionlevel,
                                            netwox_htmltag_elmtattr elmtattr,
                                            netwib_constbuf *pfilename,
                                         netwox_httphdr_statuscode statuscode);

/*-------------------------------------------------------------*/
netwib_err netwox_webspidermt_run(netwox_webspider *pwebspider,
                                  netwox_webspidermt_pf pfunc);
