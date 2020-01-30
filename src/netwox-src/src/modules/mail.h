
/*-------------------------------------------------------------*/
/* rfc 2822 : SMTP */
/* rfc 1036 : NNTP */

/*-------------------------------------------------------------*/
netwib_err netwox_mail_date(netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_mail_subject(netwib_constbuf *psubject,
                               netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_mail_newsgroups(netwib_constbuf *pnewsgroups,
                                  netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_mail_messageid(netwib_constbuf *pdomain,
                                 netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* obtain domain of an email address */
netwib_err netwox_mail_domain_init_email(netwib_constbuf *pemail,
                                         netwib_bufext *pdomain);

/*-------------------------------------------------------------*/
/* [hdr]: "name" <toto@example.com> CRLF */
netwib_err netwox_mail_email(netwib_conststring hdr,
                             netwib_constbuf *pname,
                             netwib_constbuf *pemail,
                             netwib_buf *pbuf);
#define netwox_mail_email_from(pname,pemail,pbuf) netwox_mail_email("From",pname,pemail,pbuf)
#define netwox_mail_email_to(pname,pemail,pbuf) netwox_mail_email("To",pname,pemail,pbuf)
#define netwox_mail_email_cc(pname,pemail,pbuf) netwox_mail_email("Cc",pname,pemail,pbuf)
#define netwox_mail_email_bcc(pname,pemail,pbuf) netwox_mail_email("Bcc",pname,pemail,pbuf)
#define netwox_mail_email_sender(pname,pemail,pbuf) netwox_mail_email("Sender",pname,pemail,pbuf)
#define netwox_mail_email_replyto(pname,pemail,pbuf) netwox_mail_email("Reply-To",pname,pemail,pbuf)

/*-------------------------------------------------------------*/
netwib_err netwox_mail_endheader(netwib_buf *pbuf);
