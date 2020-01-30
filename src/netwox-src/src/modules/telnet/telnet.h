
/*-------------------------------------------------------------*/
/* RFC 854 */

/*-------------------------------------------------------------*/
/* interpret as command */
#define NETWOX_TELNET_IAC 255

/*-------------------------------------------------------------*/
/* commands following IAC */
typedef enum {
  NETWOX_TELNET_CMD_EOF =   236, /* end of file */
  NETWOX_TELNET_CMD_SUSP =  237, /* suspend process (Ctrl-Z) */
  NETWOX_TELNET_CMD_ABORT = 238, /* abort process */
  NETWOX_TELNET_CMD_EOR =   239, /* end of record */
  NETWOX_TELNET_CMD_SE =    240, /* end of subnegotiation */
  NETWOX_TELNET_CMD_NOP =   241, /* no operation */
  NETWOX_TELNET_CMD_DM =    242, /* data mark */
  NETWOX_TELNET_CMD_BRK =   243, /* break */
  NETWOX_TELNET_CMD_IP =    244, /* interrupt process (Ctrl-C) */
  NETWOX_TELNET_CMD_AO =    245, /* abort output */
  NETWOX_TELNET_CMD_AYT =   246, /* are you there */
  NETWOX_TELNET_CMD_EC =    247, /* erase character */
  NETWOX_TELNET_CMD_EL =    248, /* erase line */
  NETWOX_TELNET_CMD_GA =    249, /* go ahead */
  NETWOX_TELNET_CMD_SB =    250, /* sub negotiation */
  NETWOX_TELNET_CMD_WILL =  251,
  NETWOX_TELNET_CMD_WONT =  252,
  NETWOX_TELNET_CMD_DO =    253,
  NETWOX_TELNET_CMD_DONT =  254
} netwox_telnet_cmd;

/*-------------------------------------------------------------*/
/* options for WILL, WONT, DO and DONT */
typedef enum {
  NETWOX_TELNET_OPT_BINARY =   0, /* 8-bit data path */
  NETWOX_TELNET_OPT_ECHO =     1, /* echo */
  NETWOX_TELNET_OPT_RCP =      2, /* prepare to reconnect */
  NETWOX_TELNET_OPT_SGA =      3, /* suppress go ahead */
  NETWOX_TELNET_OPT_NAMS =     4, /* approximate message size */
  NETWOX_TELNET_OPT_STATUS =   5, /* give status */
  NETWOX_TELNET_OPT_TM =       6, /* timing mark */
  NETWOX_TELNET_OPT_RCTE =     7, /* remote controlled transmission and echo */
  NETWOX_TELNET_OPT_NAOL =     8, /* negotiate about output line width */
  NETWOX_TELNET_OPT_NAOP =     9, /* negotiate about output page size */
  NETWOX_TELNET_OPT_NAOCRD =  10, /* negotiate about CR disposition */
  NETWOX_TELNET_OPT_NAOHTS =  11, /* negotiate about horizontal tabstops */
  NETWOX_TELNET_OPT_NAOHTD =  12, /* nego. about horizontal tab disposition */
  NETWOX_TELNET_OPT_NAOFFD =  13, /* negotiate about formfeed disposition */
  NETWOX_TELNET_OPT_NAOVTS =  14, /* negotiate about vertical tab stops */
  NETWOX_TELNET_OPT_NAOVTD =  15, /* nego. about vertical tab disposition */
  NETWOX_TELNET_OPT_NAOLFD =  16, /* negotiate about output LF disposition */
  NETWOX_TELNET_OPT_XASCII =  17, /* extended ascii character set */
  NETWOX_TELNET_OPT_LOGOUT =  18, /* force logout */
  NETWOX_TELNET_OPT_BM =      19, /* byte macro */
  NETWOX_TELNET_OPT_DET =     20, /* data entry terminal */
  NETWOX_TELNET_OPT_SUPDUP =  21, /* supdup protocol */
  NETWOX_TELNET_OPT_SUPDUPOUTPUT =    22, /* supdup output */
  NETWOX_TELNET_OPT_SNDLOC =          23, /* send location */
  NETWOX_TELNET_OPT_TTYPE =           24, /* terminal type */
  NETWOX_TELNET_OPT_EOR =             25, /* end or record */
  NETWOX_TELNET_OPT_TUID =            26, /* TACACS user identification */
  NETWOX_TELNET_OPT_OUTMRK =          27, /* output marking */
  NETWOX_TELNET_OPT_TTYLOC =          28, /* terminal location number */
  NETWOX_TELNET_OPT_3270REGIME =      29, /* 3270 regime */
  NETWOX_TELNET_OPT_X3PAD =           30, /* X.3 PAD */
  NETWOX_TELNET_OPT_NAWS =            31, /* window size */
  NETWOX_TELNET_OPT_TSPEED =          32, /* terminal speed */
  NETWOX_TELNET_OPT_LFLOW =           33, /* remote flow control */
  NETWOX_TELNET_OPT_LINEMODE =        34, /* Linemode option */
  NETWOX_TELNET_OPT_XDISPLOC =        35, /* X Display Location */
  NETWOX_TELNET_OPT_OLD_ENVIRON =     36, /* Old - Environment variables */
  NETWOX_TELNET_OPT_AUTHENTICATION =  37, /* Authenticate */
  NETWOX_TELNET_OPT_ENCRYPT =         38, /* Encryption option */
  NETWOX_TELNET_OPT_NEW_ENVIRON =     39, /* New - Environment variables */
  NETWOX_TELNET_OPT_TN3270E =         40, /* TN3270E */
  NETWOX_TELNET_OPT_EXOPL  =         255  /* extended-options-list */
} netwox_telnet_opt;

/*-------------------------------------------------------------*/
/* common structure linked to the io */
typedef enum {
  NETWOX_TELNET_NEGOMODE_NONE = 1, /* nothing negotiated */
  NETWOX_TELNET_NEGOMODE_LINE, /* send line by line to server, and
                                  echo is done on client. It is always
                                  supported. */
  NETWOX_TELNET_NEGOMODE_CHARACTER, /* send char by char to server
                                       which echoes for us */
  NETWOX_TELNET_NEGOMODE_LINEMODE /* rfc 1184: not yet implemented */
} netwox_telnet_negomode;
typedef struct {
  netwib_bool unused; /* currently unused */
} netwox_telnet_clientstate;
typedef struct {
  netwib_bool ctrlc; /* client user pressed Ctrl-C */
  netwib_bool ctrld; /* client user pressed Ctrl-D */
  netwib_bool ctrlz; /* client user pressed Ctrl-Z */
} netwox_telnet_serverstate;
typedef struct {
  netwib_bool isaclient; /* true if current IO is a client */
  netwox_telnet_negomode negomode; /* mode negotiated */
  netwox_telnet_clientstate client; /* used for client */
  netwox_telnet_serverstate server; /* used for server */
  /* private */
  netwox_telnet_negomode wantednegomode;
  netwib_uint32 negoinsistcount;
} netwox_telnet_state;
typedef struct {
  /* public */
  netwox_telnet_state state;
  /* private */
  netwib_buf buftmp;
  netwib_buf bufrd;
  netwib_buf bufrdready;
} netwox_telnet;

/*-------------------------------------------------------------*/
netwib_err netwox_telnet_init(netwib_bool isaclient,
                              netwib_io **ppio);

/*-------------------------------------------------------------*/
/* this functions is used to write options (write without
   escaping NETWOX_TELNET_IAC) */
netwib_err netwox_telnet_write_noiac(netwib_io *pio,
                                     netwib_constbuf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_telnet_write_opt(netwib_io *pio,
                                   netwox_telnet_cmd cmd,
                                   netwox_telnet_opt opt);

/*-------------------------------------------------------------*/
/* negotiate mode. Note, if unsupported, we can fall back in
   another mode, so read telnet.state.negomode which contains mode
   really obtained after this function */
netwib_err netwox_telnet_negotiate(netwib_io *pio,
                                   netwox_telnet_negomode negomode);
