
/*-------------------------------------------------------------*/
/* presphdrs : response header (can be NULL)
   statuscode : value returned by server
   phdrio : where to write headers (can be NULL)
   pbodyio : where to read/write body (can be NULL)
*/

/*-------------------------------------------------------------*/
netwib_err netwox_httpcli_method(netwox_httphdr_method method,
                                 netwox_httpclictx *pctx,
                                 netwib_constbuf *puri,
                                 netwox_httpcliresphdrs *presphdrs,
                                 netwox_httphdr_statuscode *pstatuscode,
                                 netwib_io *phdrio,
                                 netwib_io *pbodyio);

/*-------------------------------------------------------------*/
#define netwox_httpcli_get(pctx,puri,pstatuscode,phdrio,pbodyio) netwox_httpcli_method(NETWOX_HTTPHDR_METHOD_GET,pctx,puri,NULL,pstatuscode,phdrio,pbodyio)
#define netwox_httpcli_head(pctx,puri,pstatuscode,phdrio) netwox_httpcli_method(NETWOX_HTTPHDR_METHOD_HEAD,pctx,puri,NULL,pstatuscode,phdrio,NULL)
#define netwox_httpcli_delete(pctx,puri,pstatuscode,phdrio) netwox_httpcli_method(NETWOX_HTTPHDR_METHOD_DELETE,pctx,puri,NULL,pstatuscode,phdrio,NULL)
#define netwox_httpcli_trace(pctx,puri,pstatuscode,phdrio,pbodyio) netwox_httpcli_method(NETWOX_HTTPHDR_METHOD_TRACE,pctx,puri,NULL,pstatuscode,phdrio,pbodyio)
#define netwox_httpcli_options(pctx,puri,pstatuscode,phdrio) netwox_httpcli_method(NETWOX_HTTPHDR_METHOD_OPTIONS,pctx,puri,NULL,pstatuscode,phdrio,NULL)

/*-------------------------------------------------------------*/
netwib_err netwox_httpcli_post(netwox_httpclictx *pctx,
                               netwib_constbuf *puri,
                               netwib_constbuf *pparameters,
                               netwox_httphdr_statuscode *pstatuscode,
                               netwib_io *phdrio,
                               netwib_io *pbodyio);

/*-------------------------------------------------------------*/
netwib_err netwox_httpcli_put(netwox_httpclictx *pctx,
                              netwib_constbuf *puri,
                              netwib_bool contentlengthset,
                              netwib_uint32 contentlength,
                              netwib_io *pbodyio,
                              netwox_httphdr_statuscode *pstatuscode,
                              netwib_io *phdrio);
