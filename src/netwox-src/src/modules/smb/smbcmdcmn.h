
/*-------------------------------------------------------------*/
/* Data part of smbcmd structure is generally encoded
   like this:
     1(datablock) size data
     2(dialect) nul_terminated_string
     3(pathname) nul_terminated_string
     4(ascii) nul_terminated_string
     5(variableblock) size data
   Depending on NETWOX_SMBMSG_FLAGS2_UNICODESTRINGS, strings
   can be in UTF-16. In this case they have to be aligned.
*/
typedef enum {
  NETWOX_SMBCMDCMN_DATATYPE_UNKNOWN = 0,
  NETWOX_SMBCMDCMN_DATATYPE_DATABLOCK = 1,
  NETWOX_SMBCMDCMN_DATATYPE_DIALECT = 2,
  NETWOX_SMBCMDCMN_DATATYPE_PATHNAME = 3,
  NETWOX_SMBCMDCMN_DATATYPE_ASCII = 4,
  NETWOX_SMBCMDCMN_DATATYPE_VARIABLEBLOCK = 5,
  NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY = 0xFF,/* a special value defined by me
                                                 for string fields not
                                                 starting with type */
  NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI = 0xFE,/* never Unicode */
  NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_UNINOPAD = 0xFD,/* always Unicode, but
                                                          never padding */
  NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE = 0xFC /* a special value defined by me
                                                 for buffers with an external
                                                 size */
} netwox_smbcmdcmn_datatype;

/* alias */
typedef netwib_buf netwox_smbcmdcmn_data;
typedef const netwox_smbcmdcmn_data netwox_constsmbcmdcmn_data;

/* init and show */
netwib_err netwox_smbcmdcmn_data_initdefault(netwox_smbcmdcmn_data *pdata);
netwib_err netwox_smbcmdcmn_data_setdefault(netwox_smbcmdcmn_data *pdata);
netwib_err netwox_smbcmdcmn_data_close(netwox_smbcmdcmn_data *pdata);
netwib_err netwox_smbcmdcmn_data_show(netwib_conststring title,
                                      netwox_smbcmdcmn_datatype datatype,
                                      netwox_constsmbcmdcmn_data *pdata,
                                      netwib_buf *pbuf);

/*-----*/
/* for string, a '\0' (or '\0\0') it is added */
netwib_err netwox_smbcmdcmn_data_append(netwox_smbmsg *psmbmsg,
                                        netwib_uint32 idxcmd,
                                        netwox_smbcmdcmn_datatype datatype,
                                        netwox_constsmbcmdcmn_data *pbuf);
/* expecteddatatype indicates the expected datatype
   (NETWOX_SMBCMDCMN_DATATYPE_NONE if unknown).
   If pbuf is a string, the '\0' character is not stored in the buf.
   pdata and pdatasize are updated to skip the data.
*/
netwib_err netwox_smbcmdcmn_data_decode(netwox_constsmbmsg *psmbmsg,
                                        netwib_uint32 idxcmd,
                                   netwox_smbcmdcmn_datatype expecteddatatype,
                                        netwib_data *pdata,
                                        netwib_uint32 *pdatasize,
                                        netwox_smbcmdcmn_data *pbuf);

/*-----*/
/* for NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE only */
netwib_err netwox_smbcmdcmn_data_decodeext(netwib_data *pdata,
                                           netwib_uint32 *pdatasize,
                                           netwib_uint32 buffersize,
                                           netwox_smbcmdcmn_data *pbuf);

/*-----*/
/* !unicode: a '\0' it is added at end,
   unicode:  a '\0\0' it is added at end, but there is no padding at beginning
*/
netwib_err netwox_smbcmdcmn_data_appendstr(netwox_constsmbcmdcmn_data *pdata,
                                           netwib_bool isunicode,
                                           netwib_buf *pbuf);
/* decode, and do not store '\0' in buffer */
netwib_err netwox_smbcmdcmn_data_decodestr(netwib_data *pdata,
                                           netwib_uint32 *pdatasize,
                                           netwib_bool isunicode,
                                           netwox_smbcmdcmn_data *pbuf);

/*-------------------------------------------------------------*/
/* Uint64 */

/* alias */
typedef netwib_uint64 netwox_smbcmdcmn_uint64;
typedef const netwox_smbcmdcmn_uint64 netwox_constsmbcmdcmn_uint64;

/* common functions */
#define netwox__smbcmdcmn_uint64_initdefault(t) netwib__uint64_init_uint32(0,t)
netwib_err netwox_smbcmdcmn_uint64_show(netwib_conststring title,
                                        netwox_constsmbcmdcmn_uint64 ui64,
                                        netwib_buf *pbuf);
#define netwox__smbcmdcmn_uint64_append(data,t) netwib__data_append_uint64_le(data,t)
#define netwox__smbcmdcmn_uint64_decode(data,t) netwib__data_decode_uint64_le(data,t)

/*-------------------------------------------------------------*/
/* Time */

/* alias */
typedef netwox_time1601 netwox_smbcmdcmn_time1601;
typedef const netwox_smbcmdcmn_time1601 netwox_constsmbcmdcmn_time1601;

/* common functions */
#define netwox__smbcmdcmn_time1601_initdefault(t) netwib__uint64_init_uint32(0,t)
netwib_err netwox_smbcmdcmn_time1601_show(netwib_conststring title,
                                   netwox_constsmbcmdcmn_time1601 time1601,
                                   netwib_buf *pbuf);
#define netwox__smbcmdcmn_time1601_append(data,t) netwib__data_append_uint64_le(data,t)
#define netwox__smbcmdcmn_time1601_decode(data,t) netwib__data_decode_uint64_le(data,t)

/*-------------------------------------------------------------*/
/* Time, with 0xFFFFFFFF meaning undefined */

/* alias */
typedef netwox_time1970l netwox_smbcmdcmn_time1970l;
typedef const netwox_smbcmdcmn_time1970l netwox_constsmbcmdcmn_time1970l;

/* common functions */
#define netwox__smbcmdcmn_time1970l_initdefault(t) (t) = 0xFFFFFFFFu
netwib_err netwox_smbcmdcmn_time1970l_show(netwib_conststring title,
                                   netwox_constsmbcmdcmn_time1970l time1970l,
                                   netwib_buf *pbuf);
#define netwox__smbcmdcmn_time1970l_append(data,t) netwib__data_append_uint32_le(data,t)
#define netwox__smbcmdcmn_time1970l_decode(data,t) netwib__data_decode_uint32_le(data,t)

/*-------------------------------------------------------------*/
/* Time, MSDOS with 0 meaning undefined */

/* alias */
typedef netwox_timemsdos netwox_smbcmdcmn_timemsdos;
typedef const netwox_smbcmdcmn_timemsdos netwox_constsmbcmdcmn_timemsdos;

/* common functions */
#define netwox__smbcmdcmn_timemsdos_initdefault(t) (t).msdosdate = 0; (t).msdostime = 0
netwib_err netwox_smbcmdcmn_timemsdos_show(netwib_conststring title,
                                   netwox_constsmbcmdcmn_timemsdos timemsdos,
                                   netwib_buf *pbuf);
#define netwox__smbcmdcmn_timemsdos_append(data,t) netwib__data_append_uint16_le(data,(t).msdosdate); netwib__data_append_uint16_le(data,(t).msdostime)
#define netwox__smbcmdcmn_timemsdos_decode(data,t) netwib__data_decode_uint16_le(data,(t).msdosdate); netwib__data_decode_uint16_le(data,(t).msdostime)
/* sometimes, they are reversed */
#define netwox__smbcmdcmn_timemsdosreversed_append(data,t) netwib__data_append_uint16_le(data,(t).msdostime); netwib__data_append_uint16_le(data,(t).msdosdate)
#define netwox__smbcmdcmn_timemsdosreversed_decode(data,t) netwib__data_decode_uint16_le(data,(t).msdostime); netwib__data_decode_uint16_le(data,(t).msdosdate)

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/* Open flags */
#define NETWOX_SMBCMDCMN_OPENFLAGS16_ADDITIONALINFO   0x0001
#define NETWOX_SMBCMDCMN_OPENFLAGS16_EXCLUSIVEOPLOCK  0x0002
#define NETWOX_SMBCMDCMN_OPENFLAGS16_BATCHOPLOCK      0x0004
#define NETWOX_SMBCMDCMN_OPENFLAGS16_TARGETISADIR     0x0008
#define NETWOX_SMBCMDCMN_OPENFLAGS16_EXTENDEDRESPONSE 0x0010

/* alias */
typedef netwib_uint16 netwox_smbcmdcmn_openflags16;

/* common functions */
#define netwox__smbcmdcmn_openflags16_initdefault(o) o = 0
netwib_err netwox_smbcmdcmn_openflags16_show(netwib_conststring title,
                                        netwox_smbcmdcmn_openflags16 openflags,
                                        netwib_buf *pbuf);
#define netwox__smbcmdcmn_openflags16_append(data,o) netwib__data_append_uint16_le(data,o)
#define netwox__smbcmdcmn_openflags16_decode(data,o) netwib__data_decode_uint16_le(data,o)

/*-------------------------------------------------------------*/
/* Open flags */
#define NETWOX_SMBCMDCMN_OPENFLAGS32_ADDITIONALINFO   0x0001
#define NETWOX_SMBCMDCMN_OPENFLAGS32_EXCLUSIVEOPLOCK  0x0002
#define NETWOX_SMBCMDCMN_OPENFLAGS32_BATCHOPLOCK      0x0004
#define NETWOX_SMBCMDCMN_OPENFLAGS32_TARGETISADIR     0x0008
#define NETWOX_SMBCMDCMN_OPENFLAGS32_EXTENDEDRESPONSE 0x0010

/* alias */
typedef netwib_uint32 netwox_smbcmdcmn_openflags32;

/* common functions */
#define netwox__smbcmdcmn_openflags32_initdefault(o) o = 0
netwib_err netwox_smbcmdcmn_openflags32_show(netwib_conststring title,
                                        netwox_smbcmdcmn_openflags32 openflags,
                                        netwib_buf *pbuf);
#define netwox__smbcmdcmn_openflags32_append(data,o) netwib__data_append_uint32_le(data,o)
#define netwox__smbcmdcmn_openflags32_decode(data,o) netwib__data_decode_uint32_le(data,o)

/*-------------------------------------------------------------*/
/* File attributes (MS-DOS) */
#define NETWOX_SMBCMDCMN_FILEATTR16_READONLY 0x0001
#define NETWOX_SMBCMDCMN_FILEATTR16_HIDDEN   0x0002
#define NETWOX_SMBCMDCMN_FILEATTR16_SYSTEM   0x0004
#define NETWOX_SMBCMDCMN_FILEATTR16_VOLUME   0x0008
#define NETWOX_SMBCMDCMN_FILEATTR16_DIR      0x0010
#define NETWOX_SMBCMDCMN_FILEATTR16_ARCHIVE  0x0020

/* alias */
typedef netwib_uint16 netwox_smbcmdcmn_fileattr16;

/* common functions */
#define netwox__smbcmdcmn_fileattr16_initdefault(f) f = 0
netwib_err netwox_smbcmdcmn_fileattr16_show(netwib_conststring title,
                                        netwox_smbcmdcmn_fileattr16 fileattr16,
                                        netwib_buf *pbuf);
#define netwox__smbcmdcmn_fileattr16_append(data,f) netwib__data_append_uint16_le(data,f)
#define netwox__smbcmdcmn_fileattr16_decode(data,f) netwib__data_decode_uint16_le(data,f)

/*-------------------------------------------------------------*/
/* File attributes (Windows) */
#define NETWOX_SMBCMDCMN_FILEATTR32_READONLY   0x00000001
#define NETWOX_SMBCMDCMN_FILEATTR32_HIDDEN     0x00000002
#define NETWOX_SMBCMDCMN_FILEATTR32_SYSTEM     0x00000004
#define NETWOX_SMBCMDCMN_FILEATTR32_VOLUME     0x00000008
#define NETWOX_SMBCMDCMN_FILEATTR32_DIR        0x00000010
#define NETWOX_SMBCMDCMN_FILEATTR32_ARCHIVE    0x00000020
#define NETWOX_SMBCMDCMN_FILEATTR32_DEVICE     0x00000040
#define NETWOX_SMBCMDCMN_FILEATTR32_NORMAL     0x00000080
#define NETWOX_SMBCMDCMN_FILEATTR32_TEMP       0x00000100
#define NETWOX_SMBCMDCMN_FILEATTR32_SPARSE     0x00000200
#define NETWOX_SMBCMDCMN_FILEATTR32_REPARSE    0x00000400
#define NETWOX_SMBCMDCMN_FILEATTR32_COMPRESSED 0x00000800
#define NETWOX_SMBCMDCMN_FILEATTR32_OFFLINE    0x00001000
#define NETWOX_SMBCMDCMN_FILEATTR32_NONINDEXED 0x00002000
#define NETWOX_SMBCMDCMN_FILEATTR32_ENCRYPTED  0x00004000
#define NETWOX_SMBCMDCMN_FILEATTR32_POSIXSEMANTICS  0x01000000
#define NETWOX_SMBCMDCMN_FILEATTR32_BACKUPSEMANTICS 0x02000000
#define NETWOX_SMBCMDCMN_FILEATTR32_DELETEONCLOSE   0x04000000
#define NETWOX_SMBCMDCMN_FILEATTR32_SEQUENTIALSCAN  0x08000000
#define NETWOX_SMBCMDCMN_FILEATTR32_RANDOMACCESS    0x10000000
#define NETWOX_SMBCMDCMN_FILEATTR32_NOBUFFERING     0x20000000
#define NETWOX_SMBCMDCMN_FILEATTR32_WRITETHROUGH    0x80000000

/* alias */
typedef netwib_uint32 netwox_smbcmdcmn_fileattr32;

/* common functions */
#define netwox__smbcmdcmn_fileattr32_initdefault(f) f = 0
netwib_err netwox_smbcmdcmn_fileattr32_show(netwib_conststring title,
                                        netwox_smbcmdcmn_fileattr32 fileattr32,
                                        netwib_buf *pbuf);
#define netwox__smbcmdcmn_fileattr32_append(data,f) netwib__data_append_uint32_le(data,f)
#define netwox__smbcmdcmn_fileattr32_decode(data,f) netwib__data_decode_uint32_le(data,f)

/*-------------------------------------------------------------*/
/* Negotiated capabilities */
#define NETWOX_SMBCMDCMN_NEGOCAPA_RAWMODE              0x00000001
#define NETWOX_SMBCMDCMN_NEGOCAPA_MPXMODE              0x00000002
#define NETWOX_SMBCMDCMN_NEGOCAPA_UNICODE              0x00000004
#define NETWOX_SMBCMDCMN_NEGOCAPA_LARGEFILES           0x00000008
#define NETWOX_SMBCMDCMN_NEGOCAPA_NTSMBS               0x00000010
#define NETWOX_SMBCMDCMN_NEGOCAPA_RPCREMOTEAPIS        0x00000020
#define NETWOX_SMBCMDCMN_NEGOCAPA_NTERROR              0x00000040
#define NETWOX_SMBCMDCMN_NEGOCAPA_LEVEL2OPLOCKS        0x00000080
#define NETWOX_SMBCMDCMN_NEGOCAPA_LOCKANDREAD          0x00000100
#define NETWOX_SMBCMDCMN_NEGOCAPA_NTFIND               0x00000200
#define NETWOX_SMBCMDCMN_NEGOCAPA_DFS                  0x00001000
#define NETWOX_SMBCMDCMN_NEGOCAPA_INFOLEVELPASSTHROUGH 0x00002000
#define NETWOX_SMBCMDCMN_NEGOCAPA_LARGEREADANDX        0x00004000
#define NETWOX_SMBCMDCMN_NEGOCAPA_LARGEWRITEANDX       0x00008000
#define NETWOX_SMBCMDCMN_NEGOCAPA_UNIX                 0x00800000
#define NETWOX_SMBCMDCMN_NEGOCAPA_BULKTRANSFER         0x20000000
#define NETWOX_SMBCMDCMN_NEGOCAPA_COMPRESSEDDATA       0x40000000
#define NETWOX_SMBCMDCMN_NEGOCAPA_EXTENDEDSECURITY     0x80000000

/* alias */
typedef netwib_uint32 netwox_smbcmdcmn_negocapa;

/* common functions */
#define netwox__smbcmdcmn_negocapa_initdefault(f) f = 0
netwib_err netwox_smbcmdcmn_negocapa_show(netwib_conststring title,
                                          netwox_smbcmdcmn_negocapa negocapa,
                                          netwib_buf *pbuf);
#define netwox__smbcmdcmn_negocapa_append(data,f) netwib__data_append_uint32_le(data,f)
#define netwox__smbcmdcmn_negocapa_decode(data,f) netwib__data_decode_uint32_le(data,f)

/*-------------------------------------------------------------*/
/* Access mask (see CreateFile() function in MSDN) */
#define NETWOX_SMBCMDCMN_ACCESSMASK_READ           0x00000001
#define NETWOX_SMBCMDCMN_ACCESSMASK_WRITE          0x00000002
#define NETWOX_SMBCMDCMN_ACCESSMASK_APPEND         0x00000004
#define NETWOX_SMBCMDCMN_ACCESSMASK_READEA         0x00000008
#define NETWOX_SMBCMDCMN_ACCESSMASK_WRITEEA        0x00000010
#define NETWOX_SMBCMDCMN_ACCESSMASK_EXECUTE        0x00000020
#define NETWOX_SMBCMDCMN_ACCESSMASK_DELCHILD       0x00000040
#define NETWOX_SMBCMDCMN_ACCESSMASK_READATTR       0x00000080
#define NETWOX_SMBCMDCMN_ACCESSMASK_WRITEATTR      0x00000100
#define NETWOX_SMBCMDCMN_ACCESSMASK_DELETE         0x00010000
#define NETWOX_SMBCMDCMN_ACCESSMASK_READCONTROL    0x00020000
#define NETWOX_SMBCMDCMN_ACCESSMASK_WRITEDAC       0x00040000
#define NETWOX_SMBCMDCMN_ACCESSMASK_WRITEOWNER     0x00080000
#define NETWOX_SMBCMDCMN_ACCESSMASK_SYNCHRO        0x00100000
#define NETWOX_SMBCMDCMN_ACCESSMASK_SYSSECU        0x01000000
#define NETWOX_SMBCMDCMN_ACCESSMASK_MAXALLOWED     0x02000000
/* these 4 flags are set by developers, and system sets flags above */
#define NETWOX_SMBCMDCMN_ACCESSMASK_GENERICALL     0x10000000
#define NETWOX_SMBCMDCMN_ACCESSMASK_GENERICEXECUTE 0x20000000
#define NETWOX_SMBCMDCMN_ACCESSMASK_GENERICWRITE   0x40000000
#define NETWOX_SMBCMDCMN_ACCESSMASK_GENERICREAD    0x80000000

/* alias */
typedef netwib_uint32 netwox_smbcmdcmn_accessmask;

/* common functions */
#define netwox__smbcmdcmn_accessmask_initdefault(f) f = 0
netwib_err netwox_smbcmdcmn_accessmask_show(netwib_conststring title,
                                        netwox_smbcmdcmn_accessmask accessmask,
                                        netwib_buf *pbuf);
#define netwox__smbcmdcmn_accessmask_append(data,f) netwib__data_append_uint32_le(data,f)
#define netwox__smbcmdcmn_accessmask_decode(data,f) netwib__data_decode_uint32_le(data,f)

/*-------------------------------------------------------------*/
/* Share access (see CreateFile() function in MSDN) */
#define NETWOX_SMBCMDCMN_SHAREACCESS_READ   0x00000001
#define NETWOX_SMBCMDCMN_SHAREACCESS_WRITE  0x00000002
#define NETWOX_SMBCMDCMN_SHAREACCESS_DELETE 0x00000004

/* alias */
typedef netwib_uint32 netwox_smbcmdcmn_shareaccess;

/* common functions */
#define netwox__smbcmdcmn_shareaccess_initdefault(f) f = 0
netwib_err netwox_smbcmdcmn_shareaccess_show(netwib_conststring title,
                                      netwox_smbcmdcmn_shareaccess shareaccess,
                                      netwib_buf *pbuf);
#define netwox__smbcmdcmn_shareaccess_append(data,f) netwib__data_append_uint32_le(data,f)
#define netwox__smbcmdcmn_shareaccess_decode(data,f) netwib__data_decode_uint32_le(data,f)

/*-------------------------------------------------------------*/
/* Secu flags (see CreateFile() function in MSDN) */
#define NETWOX_SMBCMDCMN_SECUFLAGS_CONTEXTTRACKING 0x00000001
#define NETWOX_SMBCMDCMN_SECUFLAGS_EFFECTIVEONLY   0x00000002

/* alias */
typedef netwib_uint8 netwox_smbcmdcmn_secuflags;

/* common functions */
#define netwox__smbcmdcmn_secuflags_initdefault(f) f = 0
netwib_err netwox_smbcmdcmn_secuflags_show(netwib_conststring title,
                                      netwox_smbcmdcmn_secuflags secuflags,
                                      netwib_buf *pbuf);
#define netwox__smbcmdcmn_secuflags_append(data,f) netwib__data_append_uint8_le(data,f)
#define netwox__smbcmdcmn_secuflags_decode(data,f) netwib__data_decode_uint8_le(data,f)

/*-------------------------------------------------------------*/
/* Create options (see CreateFile() function in MSDN) */
#define NETWOX_SMBCMDCMN_CREATEOPT_DIRECTORY      0x00000001
#define NETWOX_SMBCMDCMN_CREATEOPT_WRITETHROUGH   0x00000002
#define NETWOX_SMBCMDCMN_CREATEOPT_SEQUENTIALONLY 0x00000004
#define NETWOX_SMBCMDCMN_CREATEOPT_SYNCIOALERT    0x00000010
#define NETWOX_SMBCMDCMN_CREATEOPT_SYNCIONONALERT 0x00000020
#define NETWOX_SMBCMDCMN_CREATEOPT_NONDIRECTORY   0x00000040
#define NETWOX_SMBCMDCMN_CREATEOPT_NOEAKNOWLEDGE  0x00000200
#define NETWOX_SMBCMDCMN_CREATEOPT_83ONLY         0x00000400
#define NETWOX_SMBCMDCMN_CREATEOPT_RANDOMACCESS   0x00000800
#define NETWOX_SMBCMDCMN_CREATEOPT_DELETEONCLOSE  0x00001000
#define NETWOX_SMBCMDCMN_CREATEOPT_OPENBYFILEID   0x00002000

/* alias */
typedef netwib_uint32 netwox_smbcmdcmn_createopt;

/* common functions */
#define netwox__smbcmdcmn_createopt_initdefault(f) f = 0
netwib_err netwox_smbcmdcmn_createopt_show(netwib_conststring title,
                                      netwox_smbcmdcmn_createopt createopt,
                                      netwib_buf *pbuf);
#define netwox__smbcmdcmn_createopt_append(data,f) netwib__data_append_uint32_le(data,f)
#define netwox__smbcmdcmn_createopt_decode(data,f) netwib__data_decode_uint32_le(data,f)

/*-------------------------------------------------------------*/
/* Server type */
#define NETWOX_SMBCMDCMN_SERVERTYPE_WORKSTATION           0x00000001
#define NETWOX_SMBCMDCMN_SERVERTYPE_SERVER                0x00000002
#define NETWOX_SMBCMDCMN_SERVERTYPE_SQLSERVER             0x00000004
#define NETWOX_SMBCMDCMN_SERVERTYPE_DOMAINCONTROLER       0x00000008
#define NETWOX_SMBCMDCMN_SERVERTYPE_DOMAINBACKUPCONTROLER 0x00000010
#define NETWOX_SMBCMDCMN_SERVERTYPE_TIMESOURCE            0x00000020
#define NETWOX_SMBCMDCMN_SERVERTYPE_APPLESERVER           0x00000040
#define NETWOX_SMBCMDCMN_SERVERTYPE_NOVELLSERVER          0x00000080
#define NETWOX_SMBCMDCMN_SERVERTYPE_DOMAINMEMBER          0x00000100
#define NETWOX_SMBCMDCMN_SERVERTYPE_PRINTQUEUESERVER      0x00000200
#define NETWOX_SMBCMDCMN_SERVERTYPE_DIALINSERVER          0x00000400
#define NETWOX_SMBCMDCMN_SERVERTYPE_XENIXSERVER           0x00000800
#define NETWOX_SMBCMDCMN_SERVERTYPE_NTWORKSTATION         0x00001000
#define NETWOX_SMBCMDCMN_SERVERTYPE_WINFORWORKGROUP       0x00002000
#define NETWOX_SMBCMDCMN_SERVERTYPE_MFPNSERVER            0x00004000
#define NETWOX_SMBCMDCMN_SERVERTYPE_NTSERVER              0x00008000
#define NETWOX_SMBCMDCMN_SERVERTYPE_POTENTIALBROWSER      0x00010000
#define NETWOX_SMBCMDCMN_SERVERTYPE_BACKUPBROWSER         0x00020000
#define NETWOX_SMBCMDCMN_SERVERTYPE_MASTERBROWSER         0x00040000
#define NETWOX_SMBCMDCMN_SERVERTYPE_DOMAINMASTERBROWSER   0x00080000
#define NETWOX_SMBCMDCMN_SERVERTYPE_OSFSERVER             0x00100000
#define NETWOX_SMBCMDCMN_SERVERTYPE_VMSSERVER             0x00200000
#define NETWOX_SMBCMDCMN_SERVERTYPE_WIN95ANDMORE          0x00400000
#define NETWOX_SMBCMDCMN_SERVERTYPE_DFS                   0x00800000
#define NETWOX_SMBCMDCMN_SERVERTYPE_NTCLUSTER             0x01000000
#define NETWOX_SMBCMDCMN_SERVERTYPE_DCE                   0x10000000u
#define NETWOX_SMBCMDCMN_SERVERTYPE_ALTERNATETRANSPORT    0x20000000u
#define NETWOX_SMBCMDCMN_SERVERTYPE_LOCALENUM             0x40000000u
#define NETWOX_SMBCMDCMN_SERVERTYPE_DOMAINENUM            0x80000000u

/* alias */
typedef netwib_uint32 netwox_smbcmdcmn_servertype;

/* common functions */
#define netwox__smbcmdcmn_servertype_initdefault(o) o = 0
netwib_err netwox_smbcmdcmn_servertype_show(netwib_conststring title,
                                       netwox_smbcmdcmn_servertype servertype,
                                       netwib_buf *pbuf);
#define netwox__smbcmdcmn_servertype_append(data,o) netwib__data_append_uint32_le(data,o)
#define netwox__smbcmdcmn_servertype_decode(data,o) netwib__data_decode_uint32_le(data,o)

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/* Access mode: rWrC rLLL rSSS rAAA */
#define NETWOX_SMBCMDCMN_ACCESSMODE_WRITETHROUGH     (1<<14)
#define NETWOX_SMBCMDCMN_ACCESSMODE_WRITETHROUGH_NO  (0<<14)
#define NETWOX_SMBCMDCMN_ACCESSMODE_WRITETHROUGH_YES (1<<14)
#define NETWOX_SMBCMDCMN_ACCESSMODE_CACHE        (1<<12)
#define NETWOX_SMBCMDCMN_ACCESSMODE_CACHE_NORMAL (0<<12)
#define NETWOX_SMBCMDCMN_ACCESSMODE_CACHE_NONE   (1<<12)
#define NETWOX_SMBCMDCMN_ACCESSMODE_LOCALITY            (7<<8)
#define NETWOX_SMBCMDCMN_ACCESSMODE_LOCALITY_UNKNOWN    (0<<8)
#define NETWOX_SMBCMDCMN_ACCESSMODE_LOCALITY_SEQUENTIAL (1<<8)
#define NETWOX_SMBCMDCMN_ACCESSMODE_LOCALITY_RANDOM     (2<<8)
#define NETWOX_SMBCMDCMN_ACCESSMODE_LOCALITY_RANDOMLOC  (3<<8)
#define NETWOX_SMBCMDCMN_ACCESSMODE_SHARING               (7<<4)
#define NETWOX_SMBCMDCMN_ACCESSMODE_SHARING_COMPATIBILITY (0<<4)
#define NETWOX_SMBCMDCMN_ACCESSMODE_SHARING_DENYRWX       (1<<4)
#define NETWOX_SMBCMDCMN_ACCESSMODE_SHARING_DENYW         (2<<4)
#define NETWOX_SMBCMDCMN_ACCESSMODE_SHARING_DENYRX        (3<<4)
#define NETWOX_SMBCMDCMN_ACCESSMODE_SHARING_DENYNONE      (4<<4)
#define NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS    (7)
#define NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS_R  (0)
#define NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS_W  (1)
#define NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS_RW (2)
#define NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS_X  (3)

/* alias */
typedef netwib_uint16 netwox_smbcmdcmn_accessmode;

/* common functions */
#define netwox__smbcmdcmn_accessmode_initdefault(a) a = 0
netwib_err netwox_smbcmdcmn_accessmode_show(netwib_conststring title,
                                 netwox_smbcmdcmn_accessmode accessmode,
                                 netwib_buf *pbuf);
#define netwox__smbcmdcmn_accessmode_append(data,a) netwib__data_append_uint16_le(data,a)
#define netwox__smbcmdcmn_accessmode_decode(data,a) netwib__data_decode_uint16_le(data,a)

/*-------------------------------------------------------------*/
/* Open function: rrrr rrrr rrrC rrOO */
#define NETWOX_SMBCMDCMN_OPENFUNCTION_CREATEFILENOEXIST           (1<<4)
#define NETWOX_SMBCMDCMN_OPENFUNCTION_CREATEFILENOEXIST_ERROR     (0<<4)
#define NETWOX_SMBCMDCMN_OPENFUNCTION_CREATEFILENOEXIST_OVERWRITE (1<<4)
#define NETWOX_SMBCMDCMN_OPENFUNCTION_OPENFILEEXIST           (3)
#define NETWOX_SMBCMDCMN_OPENFUNCTION_OPENFILEEXIST_ERROR     (0)
#define NETWOX_SMBCMDCMN_OPENFUNCTION_OPENFILEEXIST_OPEN      (1)
#define NETWOX_SMBCMDCMN_OPENFUNCTION_OPENFILEEXIST_TRUNCATE  (2)

/* alias */
typedef netwib_uint16 netwox_smbcmdcmn_openfunction;

/* common functions */
#define netwox__smbcmdcmn_openfunction_initdefault(o) o = 0
netwib_err netwox_smbcmdcmn_openfunction_show(netwib_conststring title,
                              netwox_smbcmdcmn_openfunction openfunction,
                              netwib_buf *pbuf);
#define netwox__smbcmdcmn_openfunction_append(data,o) netwib__data_append_uint16_le(data,o)
#define netwox__smbcmdcmn_openfunction_decode(data,o) netwib__data_decode_uint16_le(data,o)


/*-------------------------------------------------------------*/
/* Create action : Orrr rrrr rrrr rrFF */
#define NETWOX_SMBCMDCMN_CREATEACTION_OPLOCK     (1<<31)
#define NETWOX_SMBCMDCMN_CREATEACTION_OPLOCK_NO  (0<<31)
#define NETWOX_SMBCMDCMN_CREATEACTION_OPLOCK_YES (1<<31)
#define NETWOX_SMBCMDCMN_CREATEACTION_ACTION           (3)
#define NETWOX_SMBCMDCMN_CREATEACTION_ACTION_NONE           (0)
#define NETWOX_SMBCMDCMN_CREATEACTION_ACTION_EXISTOPENED    (1)
#define NETWOX_SMBCMDCMN_CREATEACTION_ACTION_NOEXISTCREATED (2)
#define NETWOX_SMBCMDCMN_CREATEACTION_ACTION_EXISTTRUNCATED (3)

/* alias */
typedef netwib_uint32 netwox_smbcmdcmn_createaction;

/* common functions */
#define netwox__smbcmdcmn_createaction_initdefault(o) o = 0
netwib_err netwox_smbcmdcmn_createaction_show(netwib_conststring title,
                              netwox_smbcmdcmn_createaction createaction,
                              netwib_buf *pbuf);
#define netwox__smbcmdcmn_createaction_append(data,o) netwib__data_append_uint32_le(data,o)
#define netwox__smbcmdcmn_createaction_decode(data,o) netwib__data_decode_uint32_le(data,o)

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/* Impersonation */

/* alias */
typedef enum {
  NETWOX_SMBCMDCMN_IMPERSONATION_ANONYMOUS = 0,
  NETWOX_SMBCMDCMN_IMPERSONATION_IDENTIFICATION = 1,
  NETWOX_SMBCMDCMN_IMPERSONATION_IMPERSONATION = 2,
  NETWOX_SMBCMDCMN_IMPERSONATION_DELEGATION = 3
} netwox_smbcmdcmn_impersonation;

/* common functions */
#define netwox__smbcmdcmn_impersonation_initdefault(f) f = NETWOX_SMBCMDCMN_IMPERSONATION_ANONYMOUS
netwib_err netwox_smbcmdcmn_impersonation_show(netwib_conststring title,
                                 netwox_smbcmdcmn_impersonation impersonation,
                                 netwib_buf *pbuf);
#define netwox__smbcmdcmn_impersonation_append(data,f) netwib__data_append_uint32_le(data,f)
#define netwox__smbcmdcmn_impersonation_decode(data,f) netwib__data_decode_uint32_le(data,f)

/*-------------------------------------------------------------*/
/* Oplock level */

/* alias */
typedef enum {
  NETWOX_SMBCMDCMN_OPLOCKLEVEL_NONE = 0,
  NETWOX_SMBCMDCMN_OPLOCKLEVEL_EXCLUSIVEOPLOCK = 1,
  NETWOX_SMBCMDCMN_OPLOCKLEVEL_BATCHOPLOCK = 2,
  NETWOX_SMBCMDCMN_OPLOCKLEVEL_LEVEL2OPLOCK = 3
} netwox_smbcmdcmn_oplocklevel;

/* common functions */
#define netwox__smbcmdcmn_oplocklevel_initdefault(f) f = NETWOX_SMBCMDCMN_OPLOCKLEVEL_NONE
netwib_err netwox_smbcmdcmn_oplocklevel_show(netwib_conststring title,
                                 netwox_smbcmdcmn_oplocklevel oplocklevel,
                                 netwib_buf *pbuf);
#define netwox__smbcmdcmn_oplocklevel_append(data,f) netwib__data_append_uint8_le(data,f)
#define netwox__smbcmdcmn_oplocklevel_decode(data,f) netwib__data_decode_uint8_le(data,f)

/*-------------------------------------------------------------*/
/* Create function */

/* alias */
typedef enum {                                    /* IfExists  IfNoExists */
  NETWOX_SMBCMDCMN_CREATEFUNCTION_SUPERSEDE = 0,  /* replace   create */
  NETWOX_SMBCMDCMN_CREATEFUNCTION_OPEN = 1,       /* open      error */
  NETWOX_SMBCMDCMN_CREATEFUNCTION_CREATE = 2,     /* error     create */
  NETWOX_SMBCMDCMN_CREATEFUNCTION_OPENIF = 3,     /* open      create */
  NETWOX_SMBCMDCMN_CREATEFUNCTION_OVERWRITE = 4,  /* replace   error */
  NETWOX_SMBCMDCMN_CREATEFUNCTION_OVERWRITEIF = 5 /* replace   create */
} netwox_smbcmdcmn_createfunction;

/* common functions */
#define netwox__smbcmdcmn_createfunction_initdefault(o) o = 0
netwib_err netwox_smbcmdcmn_createfunction_show(netwib_conststring title,
                              netwox_smbcmdcmn_createfunction createfunction,
                              netwib_buf *pbuf);
#define netwox__smbcmdcmn_createfunction_append(data,o) netwib__data_append_uint32_le(data,o)
#define netwox__smbcmdcmn_createfunction_decode(data,o) netwib__data_decode_uint32_le(data,o)

/*-------------------------------------------------------------*/
/* File type */

/* alias */
typedef enum {
  NETWOX_SMBCMDCMN_FILETYPE_FILEDIR = 0,
  NETWOX_SMBCMDCMN_FILETYPE_PIPEBYTE = 1,
  NETWOX_SMBCMDCMN_FILETYPE_PIPEPMSG = 2,
  NETWOX_SMBCMDCMN_FILETYPE_LPT = 3,
  NETWOX_SMBCMDCMN_FILETYPE_COM = 4
} netwox_smbcmdcmn_filetype;

/* common functions */
#define netwox__smbcmdcmn_filetype_initdefault(o) o = 0
netwib_err netwox_smbcmdcmn_filetype_show(netwib_conststring title,
                                          netwox_smbcmdcmn_filetype filetype,
                                          netwib_buf *pbuf);
#define netwox__smbcmdcmn_filetype_append(data,o) netwib__data_append_uint16_le(data,o)
#define netwox__smbcmdcmn_filetype_decode(data,o) netwib__data_decode_uint16_le(data,o)

/*-------------------------------------------------------------*/
/* Share type */

/* alias */
typedef enum {
  NETWOX_SMBCMDCMN_SHARETYPE_DIR = 0,
  NETWOX_SMBCMDCMN_SHARETYPE_PRINTER = 1,
  NETWOX_SMBCMDCMN_SHARETYPE_COM = 2,
  NETWOX_SMBCMDCMN_SHARETYPE_IPC = 3
} netwox_smbcmdcmn_sharetype;

/* common functions */
#define netwox__smbcmdcmn_sharetype_initdefault(o) o = 0
netwib_err netwox_smbcmdcmn_sharetype_show(netwib_conststring title,
                                          netwox_smbcmdcmn_sharetype sharetype,
                                          netwib_buf *pbuf);
#define netwox__smbcmdcmn_sharetype_append(data,o) netwib__data_append_uint16_le(data,o)
#define netwox__smbcmdcmn_sharetype_decode(data,o) netwib__data_decode_uint16_le(data,o)

/*-------------------------------------------------------------*/
/* Seek type */

/* alias */
typedef enum {
  NETWOX_SMBCMDCMN_SEEKTYPE_BEGIN = 0,
  NETWOX_SMBCMDCMN_SEEKTYPE_CURRENT = 1,
  NETWOX_SMBCMDCMN_SEEKTYPE_END = 2
} netwox_smbcmdcmn_seektype;

/* common functions */
#define netwox__smbcmdcmn_seektype_initdefault(o) o = 0
netwib_err netwox_smbcmdcmn_seektype_show(netwib_conststring title,
                                          netwox_smbcmdcmn_seektype seektype,
                                          netwib_buf *pbuf);
#define netwox__smbcmdcmn_seektype_append(data,o) netwib__data_append_uint16_le(data,o)
#define netwox__smbcmdcmn_seektype_decode(data,o) netwib__data_decode_uint16_le(data,o)

/*-------------------------------------------------------------*/
/* Seek type */

/* alias */
typedef enum {
  NETWOX_SMBCMDCMN_ALIGNMENT_1 =   0x000,
  NETWOX_SMBCMDCMN_ALIGNMENT_2 =   0x001,
  NETWOX_SMBCMDCMN_ALIGNMENT_4 =   0x003,
  NETWOX_SMBCMDCMN_ALIGNMENT_8 =   0x007,
  NETWOX_SMBCMDCMN_ALIGNMENT_16 =  0x00F,
  NETWOX_SMBCMDCMN_ALIGNMENT_32 =  0x01F,
  NETWOX_SMBCMDCMN_ALIGNMENT_64 =  0x03F,
  NETWOX_SMBCMDCMN_ALIGNMENT_128 = 0x07F,
  NETWOX_SMBCMDCMN_ALIGNMENT_256 = 0x0FF,
  NETWOX_SMBCMDCMN_ALIGNMENT_512 = 0x1FF
} netwox_smbcmdcmn_alignment;

/* common functions */
#define netwox__smbcmdcmn_alignment_initdefault(o) o = 0
netwib_err netwox_smbcmdcmn_alignment_show(netwib_conststring title,
                                           netwox_smbcmdcmn_alignment alignment,
                                           netwib_buf *pbuf);
#define netwox__smbcmdcmn_alignment_append(data,o) netwib__data_append_uint32_le(data,o)
#define netwox__smbcmdcmn_alignment_decode(data,o) netwib__data_decode_uint32_le(data,o)

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/* Those functions are used in smbcli and smbser. For conversions
   explained at top of smbcli.h and smbser.h */
/* special : convert to unicode if needed */
netwib_err netwox_smbcmd_append_buf(netwox_constsmbmsg *psmbmsg,
                                    netwib_constbuf *pbuf,
                                    netwib_buf *pbufcmd);
/* idem, with uppercase */
netwib_err netwox_smbcmd_append_buf_uc(netwox_constsmbmsg *psmbmsg,
                                       netwib_constbuf *pbuf,
                                       netwib_buf *pbufcmd);
/* idem, convert / to \ */
netwib_err netwox_smbcmd_append_buf_path(netwox_constsmbmsg *psmbmsg,
                                         netwib_constbuf *pbuf,
                                         netwib_buf *pbufcmd);
/* special : decode unicode if needed */
netwib_err netwox_smbcmd_decode_buf(netwox_constsmbmsg *psmbmsg,
                                    netwib_constbuf *pbufcmd,
                                    netwib_buf *pbuf);
/* idem, convert \ to / */
netwib_err netwox_smbcmd_decode_buf_path(netwox_constsmbmsg *psmbmsg,
                                         netwib_constbuf *pbufcmd,
                                         netwib_buf *pbuf);



