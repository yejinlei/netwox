
/*-------------------------------------------------------------*/
/* rfc 2821 */

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SMTPCMD_UNKNOWN = 1,
  NETWOX_SMTPCMD_EHLO,
  NETWOX_SMTPCMD_HELO,
  NETWOX_SMTPCMD_MAIL,
  NETWOX_SMTPCMD_RCPT,
  NETWOX_SMTPCMD_DATA,
  NETWOX_SMTPCMD_RSET,
  NETWOX_SMTPCMD_VRFY,
  NETWOX_SMTPCMD_EXPN,
  NETWOX_SMTPCMD_HELP,
  NETWOX_SMTPCMD_NOOP,
  NETWOX_SMTPCMD_QUIT
} netwox_smtpcmd;
netwib_err netwox_smtpcmd_init_buf(netwib_constbuf *pbuf,
                                   netwox_smtpcmd *psmtpcmd);

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SMTPREPLY_214_HELP = 214,
  NETWOX_SMTPREPLY_220_SVCREADY = 220,
  NETWOX_SMTPREPLY_221_SVCCLOSING = 221,
  NETWOX_SMTPREPLY_250_OK = 250,
  NETWOX_SMTPREPLY_251_OKNOTLOCAL = 251,
  NETWOX_SMTPREPLY_252_OKCANTVRFY = 252,
  NETWOX_SMTPREPLY_354_OKSENDDATA = 354,
  NETWOX_SMTPREPLY_500_SYNTAXERROR = 500,
  NETWOX_SMTPREPLY_502_NOTIMPLEMENTED = 502,
  NETWOX_SMTPREPLY_554_TRANSACTIONFAILED = 554
} netwox_smtpreply;

/*-------------------------------------------------------------*/
/* HELO domain CRLF */
netwib_err netwox_smtp_write_helo(netwib_io *pio,
                                  netwib_constbuf *pdomain);

/*-------------------------------------------------------------*/
/* [cmd]:<toto@example.com> CRLF */
netwib_err netwox_smtp_write_email(netwib_io *pio,
                                   netwib_conststring cmd,
                                   netwib_constbuf *pemail);
#define netwox_smtp_write_email_mailfrom(pio,pemail) netwox_smtp_write_email(pio,"MAIL FROM",pemail)
#define netwox_smtp_write_email_rcptto(pio,pemail) netwox_smtp_write_email(pio,"RCPT TO",pemail)

/*-------------------------------------------------------------*/
/* DATA */
netwib_err netwox_smtp_write_data_begin(netwib_io *pio);
/* notes :
    - pdata must contain lines terminated by "\r\n"
    - last call to content has to end with "\r\n"
*/
netwib_err netwox_smtp_write_data_content(netwib_io *pio,
                                          netwib_constbuf *pdata);
netwib_err netwox_smtp_write_data_end(netwib_io *pio);

/*-------------------------------------------------------------*/
/* QUIT */
netwib_err netwox_smtp_write_quit(netwib_io *pio);

/*-------------------------------------------------------------*/
/* send one mail (don't forget to call netwox_txtproto_io before) */
netwib_err netwox_smtp_send(netwib_bufpool *pbufpool,
                            netwib_io *pio,
                            netwib_uint32 maxwaitms,
                            netwib_constbuf *pmailfrom,
                            netwib_constbuf *prcptto,
                            netwib_constbuf *pdata);
