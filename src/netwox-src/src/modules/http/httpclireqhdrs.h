
/*-------------------------------------------------------------*/
/* this function connects to server and send common headers
   (does not add the final empty line : permits to add more headers)
   The socket io is created, and an io_init_data is plugged on top.
 */
netwib_err netwox_httpclireqhdrs_init(netwox_httpclictx *pctx,
                                      netwox_httphdr_method method,
                                      netwib_constbuf *puri,
                                      netwib_io **ppio);
