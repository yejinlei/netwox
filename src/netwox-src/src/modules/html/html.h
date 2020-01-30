
/*-------------------------------------------------------------*/
/* encode data using :
    - alphanum chars and &#xy; (encodemostchars=true)
    - printable chars and &#xy; (encodemostchars=false)
*/
netwib_err netwox_html_data_encode(netwib_constbuf *pbuf,
                                   netwib_bool encodemostchars,
                                   netwib_buf *pencodedbuf);

/*-------------------------------------------------------------*/
/* decode data containing &#xy; &amp; etc.
   Value pbuf->beginoffset is updated to point past the decoded data.
   If a "&xyz;" or "&xyz "(rfc1866) is not decoded, return
   NETWIB_ERR_NOTCONVERTED, and set pbuf->beginoffset just before.
   If pbuf does not contain sufficient data (ie "&am"), return
   NETWIB_ERR_DATANOTAVAIL, and set pbuf->beginoffset just before.
*/
netwib_err netwox_html_data_decode(netwib_buf *pbuf,
                                   netwib_buf *pdecodedbuf);

/*-------------------------------------------------------------*/
/* idem, but error tolerant */
netwib_err netwox_html_data_decode_best(netwib_constbuf *pbuf,
                                        netwib_buf *pdecodedbuf);

