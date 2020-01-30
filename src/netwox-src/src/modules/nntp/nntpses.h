
/*-------------------------------------------------------------*/
/* rfc 2980
   NNTP authentication is special. Instead of authenticating at
   beginning, each command can require an auth.
 */

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bufpool *pbufpool;
  netwib_io *pio;
  netwib_bufext user;
  netwib_bufext password;
  netwib_uint32 maxwaitms;
  netwib_uint32 errnum;
  netwib_buf *perrmsg;
} netwox_nntpses;

/*-------------------------------------------------------------*/
netwib_err netwox_nntpses_init(netwib_io *pioserver,
                               netwib_constbuf *puser,
                               netwib_constbuf *ppassword,
                               netwib_uint32 maxwaitms,
                               netwox_nntpses *pnntpses);
netwib_err netwox_nntpses_banner(netwox_nntpses *pnntpses);

/*-------------------------------------------------------------*/
netwib_err netwox_nntpses_modereader(netwox_nntpses *pnntpses);

/*-------------------------------------------------------------*/
netwib_err netwox_nntpses_group(netwox_nntpses *pnntpses,
                                netwib_constbuf *pnewsgroup);

/*-------------------------------------------------------------*/
netwib_err netwox_nntpses_article(netwox_nntpses *pnntpses,
                                  netwib_uint32 artnum);

/*-------------------------------------------------------------*/
netwib_err netwox_nntpses_post(netwox_nntpses *pnntpses,
                               netwib_constbuf *pdata);

/*-------------------------------------------------------------*/
netwib_err netwox_nntpses_list(netwox_nntpses *pnntpses);

/*-------------------------------------------------------------*/
netwib_err netwox_nntpses_xover(netwox_nntpses *pnntpses,
                                netwib_uint32 firstartnum,
                                netwib_uint32 lastartnum);

/*-------------------------------------------------------------*/
netwib_err netwox_nntpses_auth(netwox_nntpses *pnntpses);

/*-------------------------------------------------------------*/
netwib_err netwox_nntpses_quit(netwox_nntpses *pnntpses);
netwib_err netwox_nntpses_close(netwox_nntpses *pnntpses);
