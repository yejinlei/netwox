
/*-------------------------------------------------------------*/
/* rfc 977 */

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_NNTP_REPLYCODE_SVCREADY = 200,
  NETWOX_NNTP_REPLYCODE_SVCREADYNOPOST = 201,
  NETWOX_NNTP_REPLYCODE_SVCCLOSING = 205,
  NETWOX_NNTP_REPLYCODE_GROUPSELECTED = 211,
  NETWOX_NNTP_REPLYCODE_LISTFOLLOWS = 215,
  NETWOX_NNTP_REPLYCODE_ARTICLEHEADBODY = 220,
  NETWOX_NNTP_REPLYCODE_ARTICLEHEAD = 221,
  NETWOX_NNTP_REPLYCODE_ARTICLEBODY = 222,
  NETWOX_NNTP_REPLYCODE_XOVERFOLLOWS = 224,
  NETWOX_NNTP_REPLYCODE_POSTOK = 240,
  NETWOX_NNTP_REPLYCODE_AUTHOK = 281,
  NETWOX_NNTP_REPLYCODE_SENDDATA = 340,
  NETWOX_NNTP_REPLYCODE_AUTHPASS = 381,
  NETWOX_NNTP_REPLYCODE_NOARTICLENUMBER = 423,
  NETWOX_NNTP_REPLYCODE_NOARTICLEFOUND = 430,
  NETWOX_NNTP_REPLYCODE_AUTHUSER = 480
} netwox_nntp_replycode;

/*-------------------------------------------------------------*/
netwib_err netwox_nntp_write_modereader(netwib_io *pio);
netwib_err netwox_nntp_write_group(netwib_io *pio,
                                   netwib_constbuf *pnewsgroup);
netwib_err netwox_nntp_write_article(netwib_io *pio,
                                     netwib_uint32 artnum);
netwib_err netwox_nntp_write_list(netwib_io *pio);
netwib_err netwox_nntp_write_quit(netwib_io *pio);

/*-------------------------------------------------------------*/
/* rfc 2980 for AUTHINFO */
netwib_err netwox_nntp_write_authinfo_user(netwib_io *pio,
                                           netwib_constbuf *puser);
netwib_err netwox_nntp_write_authinfo_pass(netwib_io *pio,
                                           netwib_constbuf *ppassword);

/*-------------------------------------------------------------*/
/* rfc 2980 */
netwib_err netwox_nntp_write_xover(netwib_io *pio,
                                   netwib_uint32 firstartnum,
                                   netwib_uint32 lastartnum);

/*-------------------------------------------------------------*/
netwib_err netwox_nntp_write_post_begin(netwib_io *pio);
#define netwox_nntp_write_post_content(pio,pdata) netwox_smtp_write_data_content(pio,pdata)
#define netwox_nntp_write_post_end(pio) netwox_smtp_write_data_end(pio)
