
/*-------------------------------------------------------------*/
/* NETWOX_SMBCMD_TYPE_TRANSACTION2_Q */

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SMBCMDTSC2Q_TYPE_UNKNOWN = 0,
  NETWOX_SMBCMDTSC2Q_TYPE_FINDFIRST2,
  NETWOX_SMBCMDTSC2Q_TYPE_FINDNEXT2,
  NETWOX_SMBCMDTSC2Q_TYPE_QUERYFSINFO,
  NETWOX_SMBCMDTSC2Q_TYPE_QUERYPATHINFO,
  NETWOX_SMBCMDTSC2Q_TYPE_QUERYFILEINFO,
  NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_BASIC,
  NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_DISPOSITION,
  NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_ALLOCATION,
  NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EOF,
  NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EBASIC,
  NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EDISPOSITION,
  NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EALLOCATION,
  NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EEOF
} netwox_smbcmdtsc2q_type;

/*-------------------------------------------------------------*/
/* Note about resumekeys:
   It was used before Windows NT, however some Windows versions
   were bugged, so it is not used anymore. Windows 2000 always
   sets resumekeys to 0, in order to force usage of resumefilename.
*/

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdcmn_fileattr16 searchattributes; /* if 0, only return regular
                                                   files. Any specified flag
                                                   adds the associated type
                                                   to the list */
  netwib_uint16 maxentries; /* the max number of entries we can receive */
  netwib_uint16 flags;
#define NETWOX_SMBCMDTSC2Q_FIND2_FLAGS_CLOSE 0x1 /* close search after
                                                    this request */
#define NETWOX_SMBCMDTSC2Q_FIND2_FLAGS_CLOSEONEOS 0x2 /* close search if End
                                                         Of Search reached */
#define NETWOX_SMBCMDTSC2Q_FIND2_FLAGS_RESUMEKEYS 0x4 /* return resume keys */
#define NETWOX_SMBCMDTSC2Q_FIND2_FLAGS_CONTINUE 0x8 /* continue from previous
                                                       position */
#define NETWOX_SMBCMDTSC2Q_FIND2_FLAGS_BACKUPINTENT 0x10 /* backup intent */
  netwox_smbcmdtsc2_fmt_find fmt; /* format of replies */
  netwib_uint32 storagetype; /* ?, seems to be always zero */
  netwox_smbcmdcmn_data searchpattern;
} netwox_smbcmdtsc2q_findfirst2;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 sid; /* search id returned by reply */
  netwib_uint16 maxentries; /* the max number of entries we can receive */
  netwox_smbcmdtsc2_fmt_find fmt; /* format of replies */
  netwib_uint32 resumekey; /* last resumekey of previous reply. As described
                              above, it is always 0 */
  netwib_uint16 flags; /* like findfirst2 */
  netwox_smbcmdcmn_data resumefilename; /* last filename of previous reply */
} netwox_smbcmdtsc2q_findnext2;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdtsc2_fmt_fs fmt; /* format of replies */
  netwox_smbcmdcmn_data sharename;
} netwox_smbcmdtsc2q_queryfsinfo;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdtsc2_fmt_fileq fmt; /* format of replies */
  netwox_smbcmdcmn_data filename;
} netwox_smbcmdtsc2q_querypathinfo;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 fileid;
  netwox_smbcmdtsc2_fmt_fileq fmt; /* format of replies */
} netwox_smbcmdtsc2q_queryfileinfo;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 fileid;
  netwox_smbcmdtsc2_fmt_file_basic basic;
} netwox_smbcmdtsc2q_setfileinfo_basic;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 fileid;
  netwox_smbcmdtsc2_fmt_file_disposition disposition;
} netwox_smbcmdtsc2q_setfileinfo_disposition;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 fileid;
  netwox_smbcmdtsc2_fmt_file_allocation allocation;
} netwox_smbcmdtsc2q_setfileinfo_allocation;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 fileid;
  netwox_smbcmdtsc2_fmt_file_eof eof;
} netwox_smbcmdtsc2q_setfileinfo_eof;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdtsc2q_type type;
  union {
    netwox_smbcmdtsc2q_findfirst2 findfirst2;
    netwox_smbcmdtsc2q_findnext2 findnext2;
    netwox_smbcmdtsc2q_queryfsinfo queryfsinfo;
    netwox_smbcmdtsc2q_querypathinfo querypathinfo;
    netwox_smbcmdtsc2q_queryfileinfo queryfileinfo;
    netwox_smbcmdtsc2q_setfileinfo_basic setfileinfo_basic;
    netwox_smbcmdtsc2q_setfileinfo_disposition setfileinfo_disposition;
    netwox_smbcmdtsc2q_setfileinfo_allocation setfileinfo_allocation;
    netwox_smbcmdtsc2q_setfileinfo_eof setfileinfo_eof;
  } value;
} netwox_smbcmdtsc2q;
typedef const netwox_smbcmdtsc2q netwox_constsmbcmdtsc2q;

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2q_init(netwox_smbcmdtsc2q *psmbcmdtsc2q);
netwib_err netwox_smbcmdtsc2q_selecttype(netwox_smbcmdtsc2q *psmbcmdtsc2q,
                                         netwox_smbcmdtsc2q_type type);
netwib_err netwox_smbcmdtsc2q_setdefault(netwox_smbcmdtsc2q *psmbcmdtsc2q);
netwib_err netwox_smbcmdtsc2q_close(netwox_smbcmdtsc2q *psmbcmdtsc2q);

/*-------------------------------------------------------------*/
/* for those 2 functions :
    - don't forget to set isunicode before
    - they suppose query is stored only in first message (most
      of the time, query is small, so this is ok)
*/
netwib_err netwox_smbcmdtsc2q_encode_smbcmd(netwox_constsmbcmd *psmbcmd,
                                            netwox_smbcmdtsc2q *psmbcmdtsc2q);
/* only decode setup/para/data. Other values have to be set by user */
netwib_err netwox_smbcmdtsc2q_decode_smbcmd(netwox_constsmbcmdtsc2q *psmbcmdtsc2q,
                                            netwox_smbcmd *psmbcmd);

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2q_show(netwox_constsmbcmdtsc2q *psmbcmdtsc2q,
                                   netwib_buf *pbuf);
netwib_err netwox_smbcmdtsc2q_display(netwib_bufpool *pbufpool,
                                      netwox_constsmbcmdtsc2q *psmbcmdtsc2q);
