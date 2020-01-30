
/*-------------------------------------------------------------*/
/* rfc 1459 */

/*-------------------------------------------------------------*/
#define NETWOX_IRC_MODE_CHAN_OPER "o"
#define NETWOX_IRC_MODE_CHAN_PRIVATE "p"
#define NETWOX_IRC_MODE_CHAN_SECRET "s"
#define NETWOX_IRC_MODE_CHAN_INVITE "i"
#define NETWOX_IRC_MODE_CHAN_TOPIC "t"
#define NETWOX_IRC_MODE_CHAN_NOMSG "n"
#define NETWOX_IRC_MODE_CHAN_MODERATED "m"
#define NETWOX_IRC_MODE_CHAN_LIMIT "l"

/*-------------------------------------------------------------*/
#define NETWOX_IRC_MODE_USER_AWAY "a"
#define NETWOX_IRC_MODE_USER_INVISIBLE "i"
#define NETWOX_IRC_MODE_USER_WALLOPS "w"
#define NETWOX_IRC_MODE_USER_RESTRICTED "r"
#define NETWOX_IRC_MODE_USER_OPER "o"
#define NETWOX_IRC_MODE_USER_LOCALOPER "O"
#define NETWOX_IRC_MODE_USER_SRVNOTICES "s"

/*-------------------------------------------------------------*/
typedef enum {
  /* commands (text) */
  NETWOX_IRC_CMD_UNKNOWN = 1000,
  NETWOX_IRC_CMD_JOIN,
  NETWOX_IRC_CMD_KICK,
  NETWOX_IRC_CMD_NICK,
  NETWOX_IRC_CMD_NOTICE,
  NETWOX_IRC_CMD_PART,
  NETWOX_IRC_CMD_PING,
  NETWOX_IRC_CMD_PRIVMSG,
  NETWOX_IRC_CMD_QUIT,
  /* error codes (1<=numbers<=999) */
  NETWOX_IRC_CMD_WELCOME = 1,
  NETWOX_IRC_CMD_YOURHOST = 2,
  NETWOX_IRC_CMD_CREATED = 3,
  NETWOX_IRC_CMD_MYINFO = 4,
  NETWOX_IRC_CMD_BOUNCE = 5,
  NETWOX_IRC_CMD_LUSERCLIENT = 251,
  NETWOX_IRC_CMD_LUSEROP = 252,
  NETWOX_IRC_CMD_LUSERUNKNOWN = 253,
  NETWOX_IRC_CMD_LUSERCHANNELS = 254,
  NETWOX_IRC_CMD_LUSERME = 255,
  NETWOX_IRC_CMD_LIST = 322,
  NETWOX_IRC_CMD_LISTEND = 323,
  NETWOX_IRC_CMD_MOTDSTART = 375,
  NETWOX_IRC_CMD_MOTD = 372,
  NETWOX_IRC_CMD_ENDOFMOTD = 376,
  NETWOX_IRC_CMD_TOPIC = 332,
  NETWOX_IRC_CMD_NAMREPLY = 353,
  NETWOX_IRC_CMD_ENDOFNAMES = 366
  /* ... */
} netwox_irc_cmd;

/*-------------------------------------------------------------*/
netwib_err netwox_irc_cmd_init_buf(netwib_constbuf *ptxtcmd,
                                   netwox_irc_cmd *pcmd);

/*-------------------------------------------------------------*/
/* parse a line like:
    "command parameters/trailing"
    ":prefix command parameters/trailing"
 */
netwib_err netwox_irc_decode_line(netwib_constbuf *pline,
                                  netwib_bufext *pprefix,
                                  netwox_irc_cmd *pcmd,
                                  netwib_bufext *pparameters);
