
/*-------------------------------------------------------------*/
/* MIME : rfc 2045 to 2049 */

/*-------------------------------------------------------------*/
/* rfc 2045
   MIME-Version: 1.0
*/
netwib_err netwox_mime_version(netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* rfc 2047
   quoted-printable : =?iso-8859-1?q?this=20is=20some=20text?=
   base64 : =?ISO-8859-1?B?aWYgeW91IHJlYWQgdGhpcyBzZW5kIG1lIGFuIGVtYWlsIDop?=
*/
typedef enum {
  NETWOX_MIME_HDRENCODE_TYPE_QUOTEDPRINTABLE = 1,
  NETWOX_MIME_HDRENCODE_TYPE_BASE64
} netwox_mime_hdrencode_type;
netwib_err netwox_mime_hdrencode(netwib_constbuf *ptext,
                                 netwox_mime_hdrencode_type type,
                                 netwib_buf *pbuf);
#define netwox_mime_hdrencode_q(ptext,pbuf) netwox_mime_hdrencode(ptext,NETWOX_MIME_HDRENCODE_TYPE_QUOTEDPRINTABLE,pbuf)
#define netwox_mime_hdrencode_b(ptext,pbuf) netwox_mime_hdrencode(ptext,NETWOX_MIME_HDRENCODE_TYPE_BASE64,pbuf)

/*-------------------------------------------------------------*/
/* rfc 2045
   Content-type: [type]; name=...
*/
netwib_err netwox_mime_ct(netwib_conststring type,
                          netwib_constbuf *pname,
                          netwib_buf *pbuf);
#define netwox_mime_ct_textplainusascii(pname,pbuf) netwox_mime_ct("text/plain; charset=us-ascii",pname,pbuf)
#define netwox_mime_ct_textplainiso88591(pname,pbuf) netwox_mime_ct("text/plain; charset=iso-8859-1",pname,pbuf)
#define netwox_mime_ct_messagerfc822iso88591(pname,pbuf) netwox_mime_ct("message/rfc822; charset=iso-8859-1",pname,pbuf)
#define netwox_mime_ct_appoctetstream(pname,pbuf) netwox_mime_ct("application/octet-stream",pname,pbuf)

/*-------------------------------------------------------------*/
/* rfc 2045
   Content-Transfer-Encoding: [type]
*/
netwib_err netwox_mime_cte(netwib_conststring type,
                           netwib_buf *pbuf);
#define netwox_mime_cte_7bit(pbuf) netwox_mime_cte("7bit",pbuf)
#define netwox_mime_cte_8bit(pbuf) netwox_mime_cte("8bit",pbuf)
#define netwox_mime_cte_base64(pbuf) netwox_mime_cte("base64",pbuf)

/*-------------------------------------------------------------*/
/* rfc 2183
   Content-Disposition: [type]; filename=...
*/
netwib_err netwox_mime_cd(netwib_conststring type,
                          netwib_constbuf *pfilename,
                          netwib_buf *pbuf);
#define netwox_mime_cd_inline(pbuf) netwox_mime_cd("inline",NULL,pbuf)
#define netwox_mime_cd_attachment(pfilename,pbuf) netwox_mime_cd("attachment",pfilename,pbuf)

/*-------------------------------------------------------------*/
/* rfc 2046
   Content-type: multipart/mixed; boundary="b"
   --b
   --b--
*/
netwib_err netwox_mime_mp_begin(netwib_buf *pboundary, /* generated */
                                netwib_buf *pbuf);
netwib_err netwox_mime_mp_next(netwib_constbuf *pboundary,
                               netwib_buf *pbuf);
netwib_err netwox_mime_mp_end(netwib_constbuf *pboundary,
                              netwib_buf *pbuf);
