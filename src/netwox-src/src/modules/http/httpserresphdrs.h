
/*-------------------------------------------------------------*/
/* this function send common headers (does not add the final
   empty line : permits to add more headers)
 */
netwib_err netwox_httpserresphdrs_init(netwox_httpserctx *pctx,
                                       netwib_io *pio,
                                       netwox_httphdr_statuscode statuscode,
                                       netwib_conststring statusmessage);

