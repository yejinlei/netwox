
/*-------------------------------------------------------------*/
#define NETWOX_SMBMSGHDR_LEN 32
#define NETWOX_SMBMSGANDX_LEN 4

/*-------------------------------------------------------------*/
/* we do not support more than 10 AndX commands */
#define NETWOX_SMBMSG_ANDX_MAX 10

/*-------------------------------------------------------------*/
/* only supported commands are defined */
typedef enum {
  NETWOX_SMBMSG_CMD_CREATEDIRECTORY = 0x00,
  NETWOX_SMBMSG_CMD_DELETEDIRECTORY = 0x01,
  NETWOX_SMBMSG_CMD_OPEN = 0x02,
  NETWOX_SMBMSG_CMD_CREATE = 0x03,
  NETWOX_SMBMSG_CMD_CLOSE = 0x04,
  NETWOX_SMBMSG_CMD_FLUSH = 0x05,
  NETWOX_SMBMSG_CMD_DELETE = 0x06,
  NETWOX_SMBMSG_CMD_RENAME = 0x07,
  NETWOX_SMBMSG_CMD_QUERYINFORMATION = 0x08,
  NETWOX_SMBMSG_CMD_SETINFORMATION = 0x09,
  NETWOX_SMBMSG_CMD_READ = 0x0A,
  NETWOX_SMBMSG_CMD_WRITE = 0x0B,
  NETWOX_SMBMSG_CMD_CREATETEMPORARY = 0x0E,
  NETWOX_SMBMSG_CMD_CREATENEW = 0x0F,
  NETWOX_SMBMSG_CMD_CHECKDIRPATH = 0x10,
  NETWOX_SMBMSG_CMD_SEEK = 0x12,
  NETWOX_SMBMSG_CMD_READRAW = 0x1A,
  NETWOX_SMBMSG_CMD_WRITERAW = 0x1D,
  NETWOX_SMBMSG_CMD_SETINFORMATION2 = 0x22,
  NETWOX_SMBMSG_CMD_QUERYINFORMATION2 = 0x23,
  NETWOX_SMBMSG_CMD_LOCKINGANDX = 0x24,
  NETWOX_SMBMSG_CMD_TRANSACTION = 0x25,
  NETWOX_SMBMSG_CMD_COPY = 0x29,
  NETWOX_SMBMSG_CMD_MOVE = 0x2A,
  NETWOX_SMBMSG_CMD_ECHO = 0x2B,
  NETWOX_SMBMSG_CMD_OPENANDX = 0x2D,
  NETWOX_SMBMSG_CMD_READANDX = 0x2E,
  NETWOX_SMBMSG_CMD_WRITEANDX = 0x2F,
  NETWOX_SMBMSG_CMD_CLOSETREEDISCONNECT = 0x31,
  NETWOX_SMBMSG_CMD_TRANSACTION2 = 0x32,
  NETWOX_SMBMSG_CMD_FINDCLOSE2 = 0x34,
  NETWOX_SMBMSG_CMD_TREEDISCONNECT = 0x71,
  NETWOX_SMBMSG_CMD_NEGOTIATE = 0x72,
  NETWOX_SMBMSG_CMD_SESSIONSETUPANDX = 0x73,
  NETWOX_SMBMSG_CMD_LOGOFFANDX = 0x74,
  NETWOX_SMBMSG_CMD_TREECONNECTANDX = 0x75,
  NETWOX_SMBMSG_CMD_QUERYINFORMATIONDISK = 0x80,
  NETWOX_SMBMSG_CMD_SEARCH = 0x81,
  NETWOX_SMBMSG_CMD_NTTRANSACT = 0xA0,
  NETWOX_SMBMSG_CMD_NTCREATEANDX = 0xA2,
  NETWOX_SMBMSG_CMD_NTCANCEL = 0xA4,
  NETWOX_SMBMSG_CMD_NTRENAME = 0xA5,
  NETWOX_SMBMSG_CMD_ENDOFLIST = 0xFF
} netwox_smbmsg_cmd;

/*-------------------------------------------------------------*/
/* flags */
#define NETWOX_SMBMSG_FLAGS_SUPPORTOLDLOCKCMDS 0x01 /*obsolete*/
#define NETWOX_SMBMSG_FLAGS_CLIENTBUFAVAIL     0x02 /*obsolete*/
#define NETWOX_SMBMSG_FLAGS_RESERVED           0x04
#define NETWOX_SMBMSG_FLAGS_CASELESSPATHNAMES  0x08
#define NETWOX_SMBMSG_FLAGS_CANONICALPATHNAMES 0x10 /*obsolete; must be set*/
#define NETWOX_SMBMSG_FLAGS_OPLOCK             0x20 /*obsolete; newer commands
                                                      have their own flag*/
#define NETWOX_SMBMSG_FLAGS_BATCHOPLOCK        0x40 /*obsolete; newer commands
                                                      have their own flag*/
#define NETWOX_SMBMSG_FLAGS_REPLY              0x80

/*-------------------------------------------------------------*/
/* flags2 */
#define NETWOX_SMBMSG_FLAGS2_ACCEPTSLONGPATHNAME  0x0001
#define NETWOX_SMBMSG_FLAGS2_EXTENDEDATTRIBUTES   0x0002
#define NETWOX_SMBMSG_FLAGS2_SECURITYSIGNATURE    0x0004
#define NETWOX_SMBMSG_FLAGS2_RESERVED1            0x0008
#define NETWOX_SMBMSG_FLAGS2_RESERVED2            0x0010
#define NETWOX_SMBMSG_FLAGS2_RESERVED3            0x0020
#define NETWOX_SMBMSG_FLAGS2_USESLONGPATHNAME     0x0040
#define NETWOX_SMBMSG_FLAGS2_RESERVED4            0x0080
#define NETWOX_SMBMSG_FLAGS2_RESERVED5            0x0100
#define NETWOX_SMBMSG_FLAGS2_RESERVED6            0x0200
#define NETWOX_SMBMSG_FLAGS2_RESERVED7            0x0400
#define NETWOX_SMBMSG_FLAGS2_EXTENDEDSECURITYNEGO 0x0800
#define NETWOX_SMBMSG_FLAGS2_DFSPATHNAMES         0x1000
#define NETWOX_SMBMSG_FLAGS2_READIFEXECUTE        0x2000
#define NETWOX_SMBMSG_FLAGS2_NTERROR              0x4000
#define NETWOX_SMBMSG_FLAGS2_UNICODESTRINGS       0x8000

/*-------------------------------------------------------------*/
typedef struct {
  struct {
    /* because of AndX, command is in the array and not here */
    netwib_uint32 error; /* its encoding depends on flags2 */
    netwib_uint8 flags;
    netwib_uint16 flags2;
    netwib_uint16 treeid; /* defined by server once a share has been
                             established. the client must echo it */
    netwib_uint32 processid; /* sent by client, and echoed by server */
    netwib_uint16 userid; /* defined by server once a user has authenticated.
                             the client must echo it */
    netwib_uint16 multiplexid; /* sent by client, and echoed by server.
                                  each request can have its own number */
    netwib_byte signature[8];
  } hdr;
  netwib_uint32 numcmds; /* from 1 to NETWOX_SMBMSG_ANDX_MAX */
  struct {
    netwox_smbmsg_cmd cmd;
    netwib_bufext para;
    netwib_bufext data;
    netwib_bool dataisaligned; /* unicode strings in data commands are
                                  encoded depending on their alignment
                                  related to SMB packet begin. IMPORTANT:
                                  size of para is always odd (8bits+n*16bits)
                                  so if command is aligned, data is not */
  } cmds[NETWOX_SMBMSG_ANDX_MAX];
} netwox_smbmsg;
typedef const netwox_smbmsg netwox_constsmbmsg;

/*-------------------------------------------------------------*/
netwib_err netwox_smbmsg_init(netwox_smbmsg *psmbmsg);
netwib_err netwox_smbmsg_setdefault(netwox_smbmsg *psmbmsg);
netwib_err netwox_smbmsg_close(netwox_smbmsg *psmbmsg);

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_smbmsg(netwox_constsmbmsg *psmbmsg,
                                    netwib_buf *ppkt);
/* init and close are not needed */
netwib_err netwox_pkt_decode_smbmsg(netwib_constbuf *ppkt,
                                    netwox_smbmsg *psmbmsg);

/*-------------------------------------------------------------*/
netwib_err netwox_smbmsg_show(netwox_constsmbmsg *psmbmsg,
                              netwib_buf *pbuf);
netwib_err netwox_smbmsg_display(netwib_bufpool *pbufpool,
                                 netwox_constsmbmsg *psmbmsg);
netwib_err netwox_smbmsg_error_display(netwib_bufpool *pbufpool,
                                       netwox_constsmbmsg *psmbmsg);

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/*
  Some notes about complex structures:

  For structures without type:
   _init:
     Prepare variables (buf with canalloc (permit a buf_append,
     or a buf_init_ext_buf), array_init, etc.) to permit
     a _close call. Don't set integers.
   _setdefault:
     Set integers and buffers to a default value.
   _close:
     Close.
  Note: some decoding functions do not require a _init or _close. It can
        be done, but for optimization it can be omitted. In this case,
        it is said in a comment before function prototype.

  For structures with type:
   _init :
     Set type to _unknown.
   _selecttype :
     It can only be called if previous type is _unknown.
     Set type and prepare variables (buf with canalloc, array_init,
     etc.) to permit a _close call. Don't set integers.
   _setdefault :
     It can only be called if previous type is not _unknown.
     Set integers and buffers to a default value.
   _close :
     Close.
*/
