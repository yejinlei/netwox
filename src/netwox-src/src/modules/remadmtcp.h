
/*-------------------------------------------------------------*/
typedef struct {
  netwib_io *pio;
  netwib_bool cypher;
  netwox_xor_context xorrd;
  netwox_xor_context xorwr;
  netwib_uint32 timeoutsec;
  netwib_buf bufrd;
} netwox_remadmtcp_io;

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
typedef struct {
  netwib_buf password;
  netwib_buf allowedclients;
  netwib_buf rootdir;
  netwib_uint32 timeoutsec;
  netwib_bool allowexec;
  netwib_bool allowget;
  netwib_bool allowput;
} netwox_remadmtcp_ser_para;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bufpool *pbufpool;
  netwox_remadmtcp_io io;
  netwib_bool isauthenticated;
  netwox_remadmtcp_ser_para *ppara;
} netwox_remadmtcp_ser;

/*-------------------------------------------------------------*/
netwib_err netwox_remadmtcp_ser_loop(netwib_io *pio,
                                     netwib_ptr pinfos);

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
typedef struct {
  netwib_bufpool *pbufpool;
  netwox_remadmtcp_io io;
} netwox_remadmtcp_cli;

/*-------------------------------------------------------------*/
netwib_err netwox_remadmtcp_cli_init(netwib_io *pio,
                                     netwib_uint32 timeoutsec,
                                     netwox_remadmtcp_cli *pbcli);
netwib_err netwox_remadmtcp_cli_close(netwox_remadmtcp_cli *pbcli);

/*-------------------------------------------------------------*/
netwib_err netwox_remadmtcp_cli_auth(netwox_remadmtcp_cli *pbcli,
                                     netwib_constbuf *ppassword);
netwib_err netwox_remadmtcp_cli_quit(netwox_remadmtcp_cli *pbcli);

/*-------------------------------------------------------------*/
/*
  Important :
    This cypher uses modules/xor.h with a small key (1024 bytes).
    So, it is breakable. Moreover the underlying protocol
    permits to obtain "known cleartext" parts, and to uncypher
    fragments.
    If you really want to cypher your data, use gpg.
    This option is here only for people requiring a small
    level of "encryption".
*/
netwib_err netwox_remadmtcp_cli_cypher(netwox_remadmtcp_cli *pbcli,
                                       netwib_constbuf *ppassword);

/*-------------------------------------------------------------*/
netwib_err netwox_remadmtcp_cli_exec(netwox_remadmtcp_cli *pbcli,
                                     netwib_constbuf *pcmd,
                                     netwib_io *plocalio,/* where data is wr */
                                     netwib_uint32 *perr);
netwib_err netwox_remadmtcp_cli_get(netwox_remadmtcp_cli *pbcli,
                                    netwib_constbuf *premotefile,
                                    netwib_io *plocalio);/* where data is wr */
netwib_err netwox_remadmtcp_cli_put(netwox_remadmtcp_cli *pbcli,
                                    netwib_io *plocalio,/* where data is rd */
                                    netwib_constbuf *premotefile);

