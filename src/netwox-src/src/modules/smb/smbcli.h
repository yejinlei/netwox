
/*-------------------------------------------------------------*/
/* Internal SMB implementation respects Unicode characters.
   However, netwox only supports ascii filenames. Conversion
   is done in this module (and in smbser for server). Users
   can use functions of this module without bothering by using
   ascii filenames.
 */

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SMBCLI_AUTHVERSION_LM = 0,
  NETWOX_SMBCLI_AUTHVERSION_NTLMV1 = 1,
  NETWOX_SMBCLI_AUTHVERSION_NTLMV2 = 2,
  NETWOX_SMBCLI_AUTHVERSION_UNSET = 3,
} netwox_smbcli_authversion;

/*-------------------------------------------------------------*/
typedef struct {
  /* public */
  netwib_bool isipc;
  netwib_bufpool *pbufpool;
  netwox_sockinfo sockinfo;
  netwib_io *pio;
  netwib_uint32 maxwaitms;
  netwib_bool debug;
  netwib_bool verbose;
  netwib_buf *pnetbiosname; /* ASCII (not Unicode) */
  netwib_buf *pshare; /* ASCII (not Unicode) */
  netwib_buf *puser; /* ASCII (not Unicode) */
  netwib_buf *ppassword; /* ASCII (not Unicode) */
  netwox_smbcli_authversion authversion;
  /* internal */
  netwib_buf *pbufrecv; /* receiving buffer storage */
  netwib_bool isnaked; /* port 139 or 445 */
  netwib_uint32 processid; /* chosen by client */
  netwib_uint16 multiplexid; /* chosen by client, we will only support
                                one multiplex */
  /* defined in a negotiate reply */
  netwib_bool negotiate_set; /* false if not yet seen */
  netwib_uint8 negotiate_secumode;
  netwib_uint32 negotiate_maxmsgsize; /* for queries I create, I don't check
                                         this value, because those queries
                                         are always small */
  netwib_uint32 negotiate_vcid;
  netwox_smbcmdcmn_negocapa negotiate_capabilities;
  netwib_buf *pnegotiate_challenge;
  netwib_buf *pnegotiate_domain; /* ASCII */
  /* defined in a sessionsetup reply */
  netwib_bool sessionsetup_set; /* false if not yet seen */
  netwib_uint16 sessionsetup_userid;
  /* defined in a treeconnect reply */
  netwib_bool treeconnect_set; /* false if not yet seen */
  netwib_uint16 treeconnect_treeid;
} netwox_smbcli;

/*-------------------------------------------------------------*/
#define NETWOX_SMBCLI_TOOLARG_COMMON NETWOX_TOOLARG_OPT_BUF_LOGIN('u', "user", "username", NULL), NETWOX_TOOLARG_OPT_BUF_PASSWORD('w', "password", "password", NULL), NETWOX_TOOLARG_OPTA_UINT32('v', "authversion", "0=Lanman, 1=NTLMv1, 2=NTLMv2, unset=best", "2"), NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in milliseconds", "60000"), NETWOX_TOOLARG_OPTA_BOOL('V', "verbose", "display SMB trace", "0"), NETWOX_TOOLARG_OPTA_BOOL('D', "debug", "display SMB packets", "0"), NETWOX_TOOLARG_OPT_BUF_NETBIOSNAME('N', "netbiosname", "server NetBIOS NAME", NULL)
#define NETWOX_SMBCLI_TOOLARG_SHARE NETWOX_SOCK_ARG_TCP_CLIPORT("139"), NETWOX_TOOLARG_REQ_BUF_SMBSHARE('s', "share", "share name", NULL), NETWOX_SMBCLI_TOOLARG_COMMON
#define NETWOX_SMBCLI_TOOLARG_IPC NETWOX_SOCK_ARG_TCP_CLIPORT("139"), NETWOX_SMBCLI_TOOLARG_COMMON

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_init(netwox_arg *parg,
                              netwib_bool isipc,
                              netwox_smbcli *psmbcli);
#define netwox_smbcli_init_share(parg, psmbcli) netwox_smbcli_init(parg, NETWIB_FALSE, psmbcli)
#define netwox_smbcli_init_ipc(parg, psmbcli) netwox_smbcli_init(parg, NETWIB_TRUE, psmbcli)
netwib_err netwox_smbcli_close(netwox_smbcli *psmbcli);

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_sessioninit(netwox_smbcli *psmbcli);
netwib_err netwox_smbcli_negotiate(netwox_smbcli *psmbcli);
netwib_err netwox_smbcli_sessionsetup(netwox_smbcli *psmbcli,
                                      netwox_smbcli_authversion authversion);
netwib_err netwox_smbcli_treeconnect(netwox_smbcli *psmbcli);
/* equivalent function */
netwib_err netwox_smbcli_begin(netwox_smbcli *psmbcli);

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_treedisconnect(netwox_smbcli *psmbcli);
netwib_err netwox_smbcli_logoff(netwox_smbcli *psmbcli);
/* equivalent function */
netwib_err netwox_smbcli_end(netwox_smbcli *psmbcli);

/*-------------------------------------------------------------*/
typedef struct {
  netwib_buf name;
  netwox_smbcmdcmn_sharetype sharetype;
  netwib_buf comment;
} netwox_smb_sharelist;
netwib_err netwox_smb_sharelist_erase(netwib_ptr pitem);
netwib_err netwox_smb_sharelist_duplicate(netwib_constptr pitem,
                                          netwib_ptr *pdupofitem);
#define netwox_smb_sharelist_ring_init(ppring) netwib_ring_init(&netwox_smb_sharelist_erase,&netwox_smb_sharelist_duplicate,ppring)
#define netwox_smb_sharelist_ring_close(ppring) netwib_ring_close(ppring,NETWIB_TRUE)
netwib_err netwox_smbcli_shares(netwox_smbcli *psmbcli,
                                netwib_ring *pring);

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_echo(netwox_smbcli *psmbcli);

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_stat(netwox_smbcli *psmbcli,
                              netwib_constbuf *ppathname,
                              netwib_pathstat *ppathstat);

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_dir_create(netwox_smbcli *psmbcli,
                                    netwib_constbuf *pdirname);
netwib_err netwox_smbcli_dir_delete(netwox_smbcli *psmbcli,
                                    netwib_constbuf *pdirname);
netwib_err netwox_smbcli_dir_rename(netwox_smbcli *psmbcli,
                                    netwib_constbuf *polddirname,
                                    netwib_constbuf *pnewdirname);

/*-------------------------------------------------------------*/
netwib_err netwox_smbcli_file_delete(netwox_smbcli *psmbcli,
                                     netwib_constbuf *pfilename);
netwib_err netwox_smbcli_file_rename(netwox_smbcli *psmbcli,
                                     netwib_constbuf *poldfilename,
                                     netwib_constbuf *pnewfilename);
netwib_err netwox_smbcli_file_get(netwox_smbcli *psmbcli,
                                  netwib_constbuf *premotefile,
                                  netwib_constbuf *plocalfile);
netwib_err netwox_smbcli_file_get_io(netwox_smbcli *psmbcli,
                                     netwib_constbuf *premotefile,
                                     netwib_io *plocalio);
netwib_err netwox_smbcli_file_put(netwox_smbcli *psmbcli,
                                  netwib_constbuf *plocalfile,
                                  netwib_constbuf *premotefile);
netwib_err netwox_smbcli_file_put_io(netwox_smbcli *psmbcli,
                                     netwib_io *plocalio,
                                     netwib_constbuf *premotefile);

/*-------------------------------------------------------------*/
typedef struct {
  netwib_buf pathname;
  netwib_pathstat pathstat;
} netwox_smb_dirlist;
netwib_err netwox_smb_dirlist_erase(netwib_ptr pitem);
netwib_err netwox_smb_dirlist_duplicate(netwib_constptr pitem,
                                        netwib_ptr *pdupofitem);
#define netwox_smb_dirlist_ring_init(ppring) netwib_ring_init(&netwox_smb_dirlist_erase,&netwox_smb_dirlist_duplicate,ppring)
#define netwox_smb_dirlist_ring_close(ppring) netwib_ring_close(ppring,NETWIB_TRUE)
netwib_err netwox_smbcli_dir_ls(netwox_smbcli *psmbcli,
                                netwib_constbuf *pdirname,
                                netwib_ring *pring);

/*-------------------------------------------------------------*/
/* recursive functions */
netwib_err netwox_smbcli_dirrec_get(netwox_smbcli *psmbcli,
                                    netwib_constbuf *premotedirname,
                                    netwib_constbuf *plocaldirname);
netwib_err netwox_smbcli_dirrec_put(netwox_smbcli *psmbcli,
                                    netwib_constbuf *plocaldirname,
                                    netwib_constbuf *premotedirname);
netwib_err netwox_smbcli_dirrec_del(netwox_smbcli *psmbcli,
                                    netwib_constbuf *pdirname);


