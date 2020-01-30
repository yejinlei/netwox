
/*-------------------------------------------------------------*/
/* command and Query/Reply and specific Responses */
typedef enum {
  NETWOX_SMBCMD_TYPE_UNKNOWN = 0,
  NETWOX_SMBCMD_TYPE_CREATEDIRECTORY_Q,
  NETWOX_SMBCMD_TYPE_CREATEDIRECTORY_R,
  NETWOX_SMBCMD_TYPE_DELETEDIRECTORY_Q,
  NETWOX_SMBCMD_TYPE_DELETEDIRECTORY_R,
  NETWOX_SMBCMD_TYPE_CREATE_Q,
  NETWOX_SMBCMD_TYPE_CREATE_R,
  NETWOX_SMBCMD_TYPE_OPEN_Q,
  NETWOX_SMBCMD_TYPE_OPEN_R,
  NETWOX_SMBCMD_TYPE_CLOSE_Q,
  NETWOX_SMBCMD_TYPE_CLOSE_R,
  NETWOX_SMBCMD_TYPE_FLUSH_Q,
  NETWOX_SMBCMD_TYPE_FLUSH_R,
  NETWOX_SMBCMD_TYPE_DELETE_Q,
  NETWOX_SMBCMD_TYPE_DELETE_R,
  NETWOX_SMBCMD_TYPE_RENAME_Q,
  NETWOX_SMBCMD_TYPE_RENAME_R,
  NETWOX_SMBCMD_TYPE_QUERYINFORMATION_Q,
  NETWOX_SMBCMD_TYPE_QUERYINFORMATION_R,
  NETWOX_SMBCMD_TYPE_SETINFORMATION_Q,
  NETWOX_SMBCMD_TYPE_SETINFORMATION_R,
  NETWOX_SMBCMD_TYPE_READ_Q,
  NETWOX_SMBCMD_TYPE_READ_R,
  NETWOX_SMBCMD_TYPE_WRITE_Q,
  NETWOX_SMBCMD_TYPE_WRITE_R,
  NETWOX_SMBCMD_TYPE_CREATETEMPORARY_Q,
  NETWOX_SMBCMD_TYPE_CREATETEMPORARY_R,
  NETWOX_SMBCMD_TYPE_CREATENEW_Q,
  NETWOX_SMBCMD_TYPE_CREATENEW_R,
  NETWOX_SMBCMD_TYPE_CHECKDIRPATH_Q,
  NETWOX_SMBCMD_TYPE_CHECKDIRPATH_R,
  NETWOX_SMBCMD_TYPE_SEEK_Q,
  NETWOX_SMBCMD_TYPE_SEEK_R,
  NETWOX_SMBCMD_TYPE_READRAW_Q, /* there is no reply : it is stored
                                   as raw in the next NETWOX_SMBNBTSSTYPE_MSG
                                   message */
  NETWOX_SMBCMD_TYPE_WRITERAW_Q,
  NETWOX_SMBCMD_TYPE_WRITERAW_R,
  NETWOX_SMBCMD_TYPE_SETINFORMATION2_Q,
  NETWOX_SMBCMD_TYPE_SETINFORMATION2_R,
  NETWOX_SMBCMD_TYPE_QUERYINFORMATION2_Q,
  NETWOX_SMBCMD_TYPE_QUERYINFORMATION2_R,
  NETWOX_SMBCMD_TYPE_LOCKINGANDX_Q,
  NETWOX_SMBCMD_TYPE_LOCKINGANDX_R,
  NETWOX_SMBCMD_TYPE_TRANSACTION_Q,
  NETWOX_SMBCMD_TYPE_TRANSACTION_R,
  NETWOX_SMBCMD_TYPE_ECHO_Q,
  NETWOX_SMBCMD_TYPE_ECHO_R,
  NETWOX_SMBCMD_TYPE_OPENANDX_Q,
  NETWOX_SMBCMD_TYPE_OPENANDX_R,
  NETWOX_SMBCMD_TYPE_READANDX_Q,
  NETWOX_SMBCMD_TYPE_READANDX_R,
  NETWOX_SMBCMD_TYPE_WRITEANDX_Q,
  NETWOX_SMBCMD_TYPE_WRITEANDX_R,
  NETWOX_SMBCMD_TYPE_CLOSETREEDISCONNECT_Q,
  NETWOX_SMBCMD_TYPE_CLOSETREEDISCONNECT_R,
  NETWOX_SMBCMD_TYPE_TRANSACTION2_Q,
  NETWOX_SMBCMD_TYPE_TRANSACTION2_R,
  NETWOX_SMBCMD_TYPE_FINDCLOSE2_Q,
  NETWOX_SMBCMD_TYPE_FINDCLOSE2_R,
  NETWOX_SMBCMD_TYPE_TREEDISCONNECT_Q,
  NETWOX_SMBCMD_TYPE_TREEDISCONNECT_R,
  NETWOX_SMBCMD_TYPE_NEGOTIATE_Q,
  NETWOX_SMBCMD_TYPE_NEGOTIATE_R0,
  NETWOX_SMBCMD_TYPE_NEGOTIATE_R1,
  NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_Q0,
  NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_Q1,
  NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_R0,
  NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_R1,
  NETWOX_SMBCMD_TYPE_LOGOFFANDX_Q,
  NETWOX_SMBCMD_TYPE_LOGOFFANDX_R,
  NETWOX_SMBCMD_TYPE_TREECONNECTANDX_Q,
  NETWOX_SMBCMD_TYPE_TREECONNECTANDX_R,
  NETWOX_SMBCMD_TYPE_QUERYINFORMATIONDISK_Q,
  NETWOX_SMBCMD_TYPE_QUERYINFORMATIONDISK_R,
  NETWOX_SMBCMD_TYPE_SEARCH_Q,
  NETWOX_SMBCMD_TYPE_SEARCH_R,
  NETWOX_SMBCMD_TYPE_NTTRANSACT_Q,
  NETWOX_SMBCMD_TYPE_NTTRANSACT_R,
  NETWOX_SMBCMD_TYPE_NTCREATEANDX_Q,
  NETWOX_SMBCMD_TYPE_NTCREATEANDX_R,
  NETWOX_SMBCMD_TYPE_NTCANCEL_Q,
  NETWOX_SMBCMD_TYPE_NTCANCEL_R,
  NETWOX_SMBCMD_TYPE_NTRENAME_Q,
  NETWOX_SMBCMD_TYPE_NTRENAME_R
} netwox_smbcmd_type;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdcmn_data dirname;
} netwox_smbcmd_createdirectory_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdcmn_data dirname;
} netwox_smbcmd_deletedirectory_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdcmn_fileattr16 fileattr;
  netwox_smbcmdcmn_time1970l creationtime;
  netwox_smbcmdcmn_data filename;
} netwox_smbcmd_create_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 fileid;
} netwox_smbcmd_create_r;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdcmn_accessmode desiredaccess;
  netwox_smbcmdcmn_fileattr16 searchattributes;
  netwox_smbcmdcmn_data filename;
} netwox_smbcmd_open_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 fileid;
  netwox_smbcmdcmn_fileattr16 fileattributes;
  netwox_smbcmdcmn_time1970l lastwritetime;
  netwib_uint32 filesize;
  netwox_smbcmdcmn_accessmode grantedaccess;
} netwox_smbcmd_open_r;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 fileid;
  netwox_smbcmdcmn_time1970l lastwritetime; /* to indicate desired last write
                                               time to set for attributes */
} netwox_smbcmd_close_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 fileid;
} netwox_smbcmd_flush_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdcmn_fileattr16 searchattributes; /* if 0, only return regular
                                                   files. Any specified flag
                                                   adds the associated type
                                                   to the list */
  netwox_smbcmdcmn_data filename; /* can contain wildcards */
} netwox_smbcmd_delete_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdcmn_fileattr16 searchattributes; /* if 0, only return regular
                                                   files. Any specified flag
                                                   adds the associated type
                                                   to the list */
  netwox_smbcmdcmn_data oldname; /* can contain wildcards */
  netwox_smbcmdcmn_data newname; /* can not contain wildcards */
} netwox_smbcmd_rename_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdcmn_data filename;
} netwox_smbcmd_queryinformation_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdcmn_fileattr16 fileattributes;
  netwox_smbcmdcmn_time1970l lastwritetime;
  netwib_uint32 filesize;
} netwox_smbcmd_queryinformation_r;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdcmn_fileattr16 fileattributes;
  netwox_smbcmdcmn_time1970l lastwritetime;
  netwox_smbcmdcmn_data filename;
} netwox_smbcmd_setinformation_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 fileid;
  netwib_uint16 size; /* size to read */
  netwib_uint32 offset; /* offset from file begin of data to read */
#define NETWOX_SMBCMD_READ_Q_REMAINING_SAFE 0
  netwib_uint16 remaining; /* estimation of the total size to be read
                              (including those returned after this request).
                              Zero means no estimation. */
} netwox_smbcmd_read_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdcmn_data data; /* size of this data is the same as requested
                                 size in netwox_smbcmd_read_q. If it is less,
                                 it indicates end of file. */
} netwox_smbcmd_read_r;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 fileid;
  netwib_uint32 offset; /* start writing at this offset */
#define NETWOX_SMBCMD_WRITE_Q_REMAINING_SAFE 0
  netwib_uint16 remaining; /* estimation of the total size to be written
                              (including those written in this request).
                              Zero means no estimation. */
  netwox_smbcmdcmn_data data; /* if data size is zero, this commands
                                 truncates the file */
} netwox_smbcmd_write_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 writtensize; /* normally, size of data in request,
                                but might be less */
} netwox_smbcmd_write_r;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdcmn_fileattr16 fileattr;
  netwox_smbcmdcmn_time1970l creationtime;
  netwox_smbcmdcmn_data dirname;
} netwox_smbcmd_createtemporary_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 fileid;
  netwox_smbcmdcmn_data filename;
} netwox_smbcmd_createtemporary_r;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdcmn_fileattr16 fileattributes;
  netwox_smbcmdcmn_time1970l creationtime;
  netwox_smbcmdcmn_data filename;
} netwox_smbcmd_createnew_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 fileid;
} netwox_smbcmd_createnew_r;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdcmn_data dirname;
} netwox_smbcmd_checkdirpath_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 fileid;
  netwox_smbcmdcmn_seektype seektype;
  netwib_uint32 offset;
} netwox_smbcmd_seek_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 offset;
} netwox_smbcmd_seek_r;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 fileid;
  netwox_smbcmdcmn_uint64 offset; /* offset from file begin of data to read */
  netwib_uint16 maxsize; /* max size to read */
  netwib_uint16 minsize; /* min size to read, from 0 to maxsize.
                            A safe value is maxsize. */
  netwib_uint32 timeout; /* in milliseconds */
} netwox_smbcmd_readraw_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 fileid;
  netwib_uint32 rawdatasize; /* size of data stored as raw in the next
                                NETWOX_SMBNBTSSTYPE_MSG message (preceded
                                by a writeraw_r) */
  netwox_smbcmdcmn_uint64 offset; /* start writing at this offset */
  netwib_uint32 timeout; /* in milliseconds */
  netwox_smbcmdcmn_accessmode accessmode;
  netwox_smbcmdcmn_data data; /* this data is followed by rawdatasize
                                 byte in raw messages.
                                 If data size is zero, this commands
                                 does NOT truncate the file */
} netwox_smbcmd_writeraw_q;

/*-------------------------------------------------------------*/
typedef struct {
#define NETWOX_SMBCMD_WRITERAW_R_REMAINING_SAFE 0xFFFFu
  netwib_uint16 remaining; /* for pipes: size left in the pipe available
                                         for a future readraw_q request
                              for files: 0xFFFF
                              a safe value is 0xFFFF */
} netwox_smbcmd_writeraw_r;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 fileid;
  netwox_smbcmdcmn_timemsdos creationtime;
  netwox_smbcmdcmn_timemsdos lastaccesstime;
  netwox_smbcmdcmn_timemsdos lastwritetime;
} netwox_smbcmd_setinformation2_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 fileid;
} netwox_smbcmd_queryinformation2_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdcmn_timemsdos creationtime;
  netwox_smbcmdcmn_timemsdos lastaccesstime;
  netwox_smbcmdcmn_timemsdos lastwritetime;
  netwib_uint32 filesize; /* 0 for directory */
  netwib_uint32 allocationsize; /* 0 for directory, else size of disk
                                   storage (might be greater than filesize) */
  netwox_smbcmdcmn_fileattr16 fileattributes;
} netwox_smbcmd_queryinformation2_r;

/*-------------------------------------------------------------*/
#define NETWOX_SMBCMD_LOCKINGANDX_LOCKTYPE_SHARED     0x01
#define NETWOX_SMBCMD_LOCKINGANDX_LOCKTYPE_RELEASE    0x02
#define NETWOX_SMBCMD_LOCKINGANDX_LOCKTYPE_CHANGE     0x04
#define NETWOX_SMBCMD_LOCKINGANDX_LOCKTYPE_CANCEL     0x08
#define NETWOX_SMBCMD_LOCKINGANDX_LOCKTYPE_LARGEFILES 0x10
typedef struct {
  netwib_uint16 pid;
  netwox_smbcmdcmn_uint64 offset;
  netwox_smbcmdcmn_uint64 length;
} netwox_smbcmd_lockingandx_range;
typedef struct {
  netwib_uint16 fileid;
  netwib_uint8 locktype;
  netwox_smbcmdcmn_oplocklevel oplocklevel;
  netwib_uint32 timeout; /* in milliseconds */
  netwib_uint16 unlockscount; /* size of unlocks array */
  netwib_uint16 lockscount; /* size of locks array */
#define NETWOX_SMBCMD_LOCKINGANDX_RANGE_MAX 16
  netwox_smbcmd_lockingandx_range unlocks[NETWOX_SMBCMD_LOCKINGANDX_RANGE_MAX];
  netwox_smbcmd_lockingandx_range locks[NETWOX_SMBCMD_LOCKINGANDX_RANGE_MAX];
} netwox_smbcmd_lockingandx_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 totalparasize; /* para in this command plus those
                                  in following commands (in bytes) */
  netwib_uint16 totaldatasize;
  netwib_uint16 maxparasize; /* max parameter size we can receive
                                as a reply to this command */
  netwib_uint16 maxdatasize; /* max data size we can receive */
  netwib_uint8 maxsetupcount; /* max setup word count we can receive */
  netwib_uint16 flags; /* 0, most of the time */
  netwib_uint32 timeout; /* in milliseconds */
  netwib_uint8 setupcount; /* size of setup array */
#define NETWOX_SMBCMD_TRANSACTION_SETUP_MAX 16
  netwib_uint16 setup[NETWOX_SMBCMD_TRANSACTION_SETUP_MAX];
  netwox_smbcmdcmn_data name;
#define NETWOX_SMBCMD_TRANSACTION_NAME_PIPE "\\PIPE\\"
#define NETWOX_SMBCMD_TRANSACTION_NAME_PIPELANMAN "\\PIPE\\LANMAN"
  netwox_smbcmdcmn_data para;
  netwox_smbcmdcmn_data data;
  /* isunicode is not in the packet but has to be set to decide
     how to construct the transaction */
  netwib_bool isunicode;
} netwox_smbcmd_transaction_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 totalparasize; /* para in this command plus those
                                  in following commands (in bytes) */
  netwib_uint16 totaldatasize;
  netwib_uint16 paradisplacement;
  netwib_uint16 datadisplacement;
  netwib_uint8 setupcount; /* size of setup array */
  netwib_uint16 setup[NETWOX_SMBCMD_TRANSACTION_SETUP_MAX];
  netwox_smbcmdcmn_data para;
  netwox_smbcmdcmn_data data;
} netwox_smbcmd_transaction_r;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 count; /* number of times to reply */
  netwox_smbcmdcmn_data data;
} netwox_smbcmd_echo_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 number; /* number of the reply */
  netwox_smbcmdcmn_data data;
} netwox_smbcmd_echo_r;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdcmn_openflags16 openflags;
  netwox_smbcmdcmn_accessmode desiredaccess;
  netwox_smbcmdcmn_fileattr16 searchattributes;
  netwox_smbcmdcmn_fileattr16 fileattributes; /* attributes for creation */
  netwox_smbcmdcmn_time1970l creationtime;
  netwox_smbcmdcmn_openfunction openfunction;
  netwib_uint32 allocationsize; /* initial allocation size for creation */
  netwib_uint32 timeout; /* in milliseconds */
  netwox_smbcmdcmn_data filename;
} netwox_smbcmd_openandx_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 fileid;
  netwox_smbcmdcmn_fileattr16 fileattributes;
  netwox_smbcmdcmn_time1970l lastwritetime;
  netwib_uint32 filesize;
  netwox_smbcmdcmn_accessmode grantedaccess;
  netwox_smbcmdcmn_filetype filetype;
  netwib_uint16 ipcstate; /* always 0; only used for IPC, so not decoded */
  netwox_smbcmdcmn_createaction action;
  netwib_uint32 serverfileid; /* ?, always 0 */
} netwox_smbcmd_openandx_r;

/*-------------------------------------------------------------*/
/* do not support NETWOX_SMBCMDCMN_NEGOCAPA_LARGEREADANDX because
   psmbmsg->cmds[i].data size is limited to 0xFFFF bytes in
   this implementation */
typedef struct {
  netwib_uint16 fileid;
  netwox_smbcmdcmn_uint64 offset; /* offset from file begin of data to read */
  netwib_uint16 maxsize; /* max size to read */
  netwib_uint16 minsize; /* min size to read, from 0 to maxsize.
                            A safe value is maxsize. */
  netwib_uint32 timeout; /* in milliseconds */
#define NETWOX_SMBCMD_READANDX_Q_REMAINING_SAFE(maxsize) (maxsize)
  netwib_uint16 remaining; /* total size to be read (including those
                              returned after this request).
                              a safe value is maxsize */
} netwox_smbcmd_readandx_q;

/*-------------------------------------------------------------*/
typedef struct {
#define NETWOX_SMBCMD_READANDX_R_REMAINING_SAFE 0xFFFFu
  netwib_uint16 remaining; /* for pipes: size left in the pipe excluding
                                         data in this reply
                              for files: 0xFFFF
                              a safe value is 0xFFFF */
  netwib_uint16 datacompactionmode; /* ?, always 0 */
  netwox_smbcmdcmn_data data;
} netwox_smbcmd_readandx_r;

/*-------------------------------------------------------------*/
/* do not support NETWOX_SMBCMDCMN_NEGOCAPA_LARGEWRITEANDX because
   psmbmsg->cmds[i].data size is limited to 0xFFFF bytes in
   this implementation */
typedef struct {
  netwib_uint16 fileid;
  netwox_smbcmdcmn_uint64 offset; /* offset of data from file begin */
  netwib_uint32 timeout; /* in milliseconds */
  netwib_uint32 writemode; /* not decoded, generally 0 */
#define NETWOX_SMBCMD_WRITEANDX_Q_REMAINING_SAFE(pdata) (netwib_uint16)(netwib__buf_ref_data_sizenull(pdata)>0xFFFFu?0xFFFFu:netwib__buf_ref_data_sizenull(pdata))
  netwib_uint16 remaining; /* total size to be written (including those of
                              this request).
                              a safe value is data size */
  netwox_smbcmdcmn_data data; /* if data size is zero, this commands
                                 does NOT truncate the file */
} netwox_smbcmd_writeandx_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 writtensize;
#define NETWOX_SMBCMD_WRITEANDX_R_REMAINING_SAFE 0xFFFFu
  netwib_uint16 remaining; /* for pipes: size left in the pipe available
                                         for a future readandx_q request
                              for files: 0xFFFF
                              a safe value is 0xFFFF */
} netwox_smbcmd_writeandx_r;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 fileid;
  netwox_smbcmdcmn_time1970l lastwritetime; /* to indicate desired last write
                                               time to set for attributes */
} netwox_smbcmd_closetreedisconnect_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 totalparasize; /* para in this command plus those
                                  in following commands (in bytes) */
  netwib_uint16 totaldatasize;
  netwib_uint16 maxparasize; /* max parameter size we can receive
                                as a reply to this command */
  netwib_uint16 maxdatasize; /* max data size we can receive */
  netwib_uint8 maxsetupcount; /* max setup word count we can receive */
  netwib_uint16 flags; /* 0, most of the time */
  netwib_uint32 timeout; /* in milliseconds */
  netwib_uint8 setupcount; /* size of setup array */
#define NETWOX_SMBCMD_TRANSACTION2_SETUP_MAX 16
  netwib_uint16 setup[NETWOX_SMBCMD_TRANSACTION2_SETUP_MAX];
  netwox_smbcmdcmn_data para;
  netwox_smbcmdcmn_data data;
  /* isunicode is not in the packet but has to be set to decide
     how to construct the transaction */
  netwib_bool isunicode;
} netwox_smbcmd_transaction2_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 totalparasize; /* para in this command plus those
                                  in following commands (in bytes) */
  netwib_uint16 totaldatasize;
  netwib_uint16 paradisplacement;
  netwib_uint16 datadisplacement;
  netwib_uint8 setupcount; /* size of setup array */
  netwib_uint16 setup[NETWOX_SMBCMD_TRANSACTION2_SETUP_MAX];
  netwox_smbcmdcmn_data para;
  netwox_smbcmdcmn_data data;
} netwox_smbcmd_transaction2_r;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 sid;
} netwox_smbcmd_findclose2_q;

/*-------------------------------------------------------------*/
typedef struct {
  /* only support NT LM 0.12 */
#define NETWOX_SMBCMD_NEGOTIATE_DIALECT_NTLM012 "NT LM 0.12"
  netwib_uint32 ntlm012position; /* 0=notfound, 1=first, etc. */
} netwox_smbcmd_negotiate_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 dialectindex; /* == ntlm012position - 1 */
  netwib_uint8 secumode;
#define NETWOX_SMBCMD_NEGOTIATE_SECUMODE_USERLEVEL    0x01 /* != share level */
#define NETWOX_SMBCMD_NEGOTIATE_SECUMODE_CHALRESP     0x02
#define NETWOX_SMBCMD_NEGOTIATE_SECUMODE_SIGNENABLED  0x04
#define NETWOX_SMBCMD_NEGOTIATE_SECUMODE_SIGNREQUIRED 0x08
  netwib_uint16 maxmultiplex; /* server indicates the max number of
                                 simultaneous pending request it can handle.
                                 This is related to multiplexid field in
                                 smbmsg header. */
  netwib_uint16 maxvc; /* server indicates the max number of virtual circuits
                          it can handle. Apparently, this never worked.
                          Use 1. */
  netwib_uint32 maxmsgsize; /* server indicates max size of SMB messages */
  netwib_uint32 maxrawsize; /* server indicates max size of raw data */
  netwib_uint32 vcid; /* server generates this id. client will have
                         to echo it back in smbcmd_sessionsetupandx_q.
                         this is related to virtual circuits, so
                         its usage is not very clear. */
  netwox_smbcmdcmn_negocapa capabilities; /* capabilities of server */
  netwox_time1601 systemtime;
  netwib_int16 timezoneoffset; /* offset between local and GMT */
  netwox_smbcmdcmn_data challenge; /* 8 byte challenge */
  netwox_smbcmdcmn_data domain; /* always unicode because use NETWOX_
                                   SMBCMDCMN_DATATYPE_STRINGONLY_UNINOPAD */
  netwib_bool i1set; /* additional info */
  struct {
    netwox_smbcmdcmn_data server; /* always unicode because use NETWOX_
                                     SMBCMDCMN_DATATYPE_STRINGONLY_UNINOPAD */
  } i1;
} netwox_smbcmd_negotiate_r0; /* ! NETWOX_SMBCMDCMN_NEGOCAPA_EXTENDEDSECURITY*/

typedef struct {
  netwib_uint32 dialectindex;
  netwib_uint8 secumode;
  netwib_uint16 maxmultiplex;
  netwib_uint16 maxvc;
  netwib_uint32 maxmsgsize;
  netwib_uint32 maxrawsize;
  netwib_uint32 vcid;
  netwox_smbcmdcmn_negocapa capabilities;
  netwox_time1601 systemtime;
  netwib_int16 timezoneoffset;
  netwox_smbcmdcmn_data serverguid; /* 16 bytes GUID */
  netwox_smbcmdcmn_data securityblob;
} netwox_smbcmd_negotiate_r1; /* NETWOX_SMBCMDCMN_NEGOCAPA_EXTENDEDSECURITY */

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 maxmsgsize; /* client indicates the max size of SMB messages */
  netwib_uint16 maxmultiplex; /* client indicates the max number of
                                 simultaneous pending request it can handle. */
  netwib_uint16 vcnumber; /* number of the virtual circuit (0=first) */
  netwib_uint32 vcid; /* echo it back from smbcmd_negotiate_r */
  netwox_smbcmdcmn_negocapa capabilities; /* capabilities of client */
  netwox_smbcmdcmn_data password; /* 24 bytes challenge answer */
  netwox_smbcmdcmn_data unicodepassword; /* 24 bytes challenge answer (or more
                                            for NTLMv2) */
  netwox_smbcmdcmn_data user;
  netwox_smbcmdcmn_data domain;
  netwox_smbcmdcmn_data nativeos; /* common values are : Windows 4.0,
                                     Windows 5.0, Windows 2000 2abc,
                                     Windows 2002 2abc */
  netwox_smbcmdcmn_data nativelanman; /* common values are : Windows 4.0,
                                         Windows 2000 5.0, Windows 2002 5.1,
                                         Windows 2000 LAN Manager */
  netwib_bool i1set; /* additional info */
  struct {
    netwox_smbcmdcmn_data unknown; /* ?, always empty */
  } i1;
} netwox_smbcmd_sessionsetupandx_q0; /* LM, NTLMv1 and NTLMv2 */

typedef struct {
  netwib_uint16 maxmsgsize;
  netwib_uint16 maxmultiplex;
  netwib_uint16 vcnumber;
  netwib_uint32 vcid;
  netwox_smbcmdcmn_negocapa capabilities;
  netwox_smbcmdcmn_data secublob;
  netwox_smbcmdcmn_data nativeos;
  netwox_smbcmdcmn_data nativelanman;
  netwox_smbcmdcmn_data domain;
} netwox_smbcmd_sessionsetupandx_q1; /* Extended Security */

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 action;
#define NETWOX_SMBCMD_SESSIONSETUPANDX_ACTION_GUEST 0x1 /* logged as guest */
  netwib_bool i1set; /* additional info */
  struct {
    netwox_smbcmdcmn_data nativeos;
    netwox_smbcmdcmn_data nativelanman;
    netwib_bool i2set; /* additional info */
    struct {
      netwox_smbcmdcmn_data domain;
    } i2;
  } i1;
} netwox_smbcmd_sessionsetupandx_r0; /* LM, NTLMv1 and NTLMv2 */

typedef struct {
  netwib_uint16 action;
  netwox_smbcmdcmn_data secublob;
  netwox_smbcmdcmn_data nativeos;
  netwox_smbcmdcmn_data nativelanman;
  netwib_bool i2set; /* additional info */
  struct {
    netwox_smbcmdcmn_data domain;
  } i2;
} netwox_smbcmd_sessionsetupandx_r1; /* Extended Security */

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 flags;
#define NETWOX_SMBCMD_TREECONNECTANDX_FLAGS_DISCONNECTTID 0x01
  netwox_smbcmdcmn_data password;
  netwox_smbcmdcmn_data path;
  netwox_smbcmdcmn_data service;
#define NETWOX_SMBCMD_TREECONNECTANDX_SERVICE_SHARE     "A:"
#define NETWOX_SMBCMD_TREECONNECTANDX_SERVICE_PRINTER   "LPT1:"
#define NETWOX_SMBCMD_TREECONNECTANDX_SERVICE_IPC       "IPC"
#define NETWOX_SMBCMD_TREECONNECTANDX_SERVICE_COMDEVICE "COMM"
#define NETWOX_SMBCMD_TREECONNECTANDX_SERVICE_ANY       "?????"
} netwox_smbcmd_treeconnectandx_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdcmn_data service; /* like in query */
  netwib_bool i1set; /* additional info */
  struct {
#define NETWOX_SMBCMD_TREECONNECTANDX_OPTIONALSUPPORT_SEARCHBITS 0x0001
#define NETWOX_SMBCMD_TREECONNECTANDX_OPTIONALSUPPORT_SHAREINDFS 0x0002
    netwib_uint16 optionalsupport; /* bitfield for search and
                                      DFS. A safe value is 0 */
    netwox_smbcmdcmn_data nativefs; /* empty, NTFS, FAT, SAMBA */
    netwib_bool i2set; /* additional info */
    struct {
      netwib_uint32 ui32a; /* ?, often 0 or 511 */
      netwib_uint32 ui32b; /* ?, often 0 or 511 */
    } i2;
  } i1;
} netwox_smbcmd_treeconnectandx_r;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 totalunits;
  netwib_uint16 sectorsperunit;
  netwib_uint16 bytespersector;
  netwib_uint16 freeunits;
} netwox_smbcmd_queryinformationdisk_r;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 maxentries; /* the max number of entries we can receive */
  netwox_smbcmdcmn_fileattr16 searchattributes; /* if 0, only return regular
                                                   files. Any specified flag
                                                   adds the associated type
                                                   to the list */
  netwox_smbcmdcmn_data searchpattern; /* can contain wildcards */
  netwox_smbcmdcmn_data resumekey; /* see smbcmdsearch.h to decode/encode */
} netwox_smbcmd_search_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 count; /* entries count */
  netwox_smbcmdcmn_data entries; /* see smbcmdsearch.h to decode/encode */
} netwox_smbcmd_search_r;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint8 maxsetupcount; /* max setup word count we can receive */
  netwib_uint32 totalparasize; /* para in this command plus those
                                  in following commands (in bytes) */
  netwib_uint32 totaldatasize;
  netwib_uint32 maxparasize; /* max parameter size we can receive
                                as a reply to this command */
  netwib_uint32 maxdatasize; /* max data size we can receive */
  netwib_uint16 subcmd;
  netwib_uint8 setupcount; /* size of setup array */
#define NETWOX_SMBCMD_NTTRANSACT_SETUP_MAX 16
  netwib_uint16 setup[NETWOX_SMBCMD_NTTRANSACT_SETUP_MAX];
  netwox_smbcmdcmn_data para;
  netwox_smbcmdcmn_data data;
  /* isunicode is not in the packet but has to be set to decide
     how to construct the transaction */
  netwib_bool isunicode;
} netwox_smbcmd_nttransact_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 totalparasize; /* para in this command plus those
                                  in following commands (in bytes) */
  netwib_uint32 totaldatasize;
  netwib_uint32 paradisplacement;
  netwib_uint32 datadisplacement;
  netwib_uint8 setupcount; /* size of setup array */
  netwib_uint16 setup[NETWOX_SMBCMD_NTTRANSACT_SETUP_MAX];
  netwox_smbcmdcmn_data para;
  netwox_smbcmdcmn_data data;
} netwox_smbcmd_nttransact_r;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdcmn_openflags32 openflags;
  netwib_uint32 rootfileid; /*  if not zero, open is relative to this dir */
  netwox_smbcmdcmn_accessmask accessmask;
  netwox_smbcmdcmn_uint64 allocationsize;
  netwox_smbcmdcmn_fileattr32 fileattributes;
  netwox_smbcmdcmn_shareaccess shareaccess;
  netwox_smbcmdcmn_createfunction createfunction;
  netwox_smbcmdcmn_createopt createoptions;
  netwox_smbcmdcmn_impersonation impersonation;
  netwox_smbcmdcmn_secuflags secuflags;
  netwox_smbcmdcmn_data filename;
} netwox_smbcmd_ntcreateandx_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdcmn_oplocklevel oplocklevel;
  netwib_uint16 fileid;
  netwox_smbcmdcmn_createaction createaction;
  netwox_smbcmdcmn_time1601 creationtime;
  netwox_smbcmdcmn_time1601 lastaccesstime;
  netwox_smbcmdcmn_time1601 lastwritetime;
  netwox_smbcmdcmn_time1601 changetime;
  netwox_smbcmdcmn_fileattr32 fileattributes;
  netwox_smbcmdcmn_uint64 allocationsize;
  netwox_smbcmdcmn_uint64 filesize;
  netwox_smbcmdcmn_filetype filetype;
  netwib_uint16 ipcstate; /* only used for IPC, so not decoded */
  netwib_bool isdirectory;
} netwox_smbcmd_ntcreateandx_r;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdcmn_fileattr16 searchattributes;
  netwib_uint16 informationlevel;
  netwib_uint32 clustercount;
  netwox_smbcmdcmn_data oldname;
  netwox_smbcmdcmn_data newname;
} netwox_smbcmd_ntrename_q;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmd_type type;
  union {
    netwox_smbcmd_createdirectory_q createdirectory_q;
    /* createdirectory_r not needed */
    netwox_smbcmd_deletedirectory_q deletedirectory_q;
    /* deletedirectory_r not needed */
    netwox_smbcmd_create_q create_q;
    netwox_smbcmd_create_r create_r;
    netwox_smbcmd_open_q open_q;
    netwox_smbcmd_open_r open_r;
    netwox_smbcmd_close_q close_q;
    /* close_r not needed */
    netwox_smbcmd_flush_q flush_q;
    /* flush_r not needed */
    netwox_smbcmd_delete_q delete_q;
    /* delete_r not needed */
    netwox_smbcmd_rename_q rename_q;
    /* rename_r not needed */
    netwox_smbcmd_queryinformation_q queryinformation_q;
    netwox_smbcmd_queryinformation_r queryinformation_r;
    netwox_smbcmd_setinformation_q setinformation_q;
    /* setinformation_r not needed */
    netwox_smbcmd_read_q read_q;
    netwox_smbcmd_read_r read_r;
    netwox_smbcmd_write_q write_q;
    netwox_smbcmd_write_r write_r;
    netwox_smbcmd_createtemporary_q createtemporary_q;
    netwox_smbcmd_createtemporary_r createtemporary_r;
    netwox_smbcmd_createnew_q createnew_q;
    netwox_smbcmd_createnew_r createnew_r;
    netwox_smbcmd_checkdirpath_q checkdirpath_q;
    /* checkdirpath_r not needed */
    netwox_smbcmd_seek_q seek_q;
    netwox_smbcmd_seek_r seek_r;
    netwox_smbcmd_readraw_q readraw_q;
    netwox_smbcmd_writeraw_q writeraw_q;
    netwox_smbcmd_writeraw_r writeraw_r;
    netwox_smbcmd_setinformation2_q setinformation2_q;
    /* setinformation2_r not needed */
    netwox_smbcmd_queryinformation2_q queryinformation2_q;
    netwox_smbcmd_queryinformation2_r queryinformation2_r;
    netwox_smbcmd_lockingandx_q lockingandx_q;
    /* lockingandx_r not needed */
    netwox_smbcmd_transaction_q transaction_q;
    netwox_smbcmd_transaction_r transaction_r;
    netwox_smbcmd_echo_q echo_q;
    netwox_smbcmd_echo_r echo_r;
    netwox_smbcmd_openandx_q openandx_q;
    netwox_smbcmd_openandx_r openandx_r;
    netwox_smbcmd_readandx_q readandx_q;
    netwox_smbcmd_readandx_r readandx_r;
    netwox_smbcmd_writeandx_q writeandx_q;
    netwox_smbcmd_writeandx_r writeandx_r;
    netwox_smbcmd_closetreedisconnect_q closetreedisconnect_q;
    /* closetreedisconnect_r not needed */
    netwox_smbcmd_negotiate_q negotiate_q;
    netwox_smbcmd_negotiate_r0 negotiate_r0;
    netwox_smbcmd_negotiate_r1 negotiate_r1;
    netwox_smbcmd_transaction2_q transaction2_q;
    netwox_smbcmd_transaction2_r transaction2_r;
    netwox_smbcmd_findclose2_q findclose2_q;
    /* findclose2_r not needed */
    /* treedisconnect_q not needed */
    /* treedisconnect_r not needed */
    netwox_smbcmd_sessionsetupandx_q0 sessionsetupandx_q0;
    netwox_smbcmd_sessionsetupandx_q1 sessionsetupandx_q1;
    netwox_smbcmd_sessionsetupandx_r0 sessionsetupandx_r0;
    netwox_smbcmd_sessionsetupandx_r1 sessionsetupandx_r1;
    /* logoffandx_q not needed */
    /* logoffandx_r not needed */
    netwox_smbcmd_treeconnectandx_q treeconnectandx_q;
    netwox_smbcmd_treeconnectandx_r treeconnectandx_r;
    /* queryinformationdisk_q not needed */
    netwox_smbcmd_queryinformationdisk_r queryinformationdisk_r;
    netwox_smbcmd_search_q search_q;
    netwox_smbcmd_search_r search_r;
    netwox_smbcmd_nttransact_q nttransact_q;
    netwox_smbcmd_nttransact_r nttransact_r;
    netwox_smbcmd_ntcreateandx_q ntcreateandx_q;
    netwox_smbcmd_ntcreateandx_r ntcreateandx_r;
    /* ntcancel_q not needed */
    /* ntcancel_r not needed */
    netwox_smbcmd_ntrename_q ntrename_q;
    /* ntrename_r not needed */
  } value;
} netwox_smbcmd;
typedef const netwox_smbcmd netwox_constsmbcmd;

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmd_init(netwox_smbcmd *psmbcmd);
netwib_err netwox_smbcmd_selecttype(netwox_smbcmd *psmbcmd,
                                    netwox_smbcmd_type type);
netwib_err netwox_smbcmd_setdefault(netwox_smbcmd *psmbcmd);
netwib_err netwox_smbcmd_close(netwox_smbcmd *psmbcmd);

/*-------------------------------------------------------------*/
netwib_err netwox_smbmsg_append_smbcmd(netwox_constsmbcmd *psmbcmd,
                                       netwox_smbmsg *psmbmsg);
netwib_err netwox_smbmsg_decode_smbcmd(netwox_constsmbmsg *psmbmsg,
                                       netwib_uint32 idxcmd,
                                       netwox_smbcmd *psmbcmd);

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmd_show(netwox_constsmbcmd *psmbcmd,
                              netwib_buf *pbuf);
