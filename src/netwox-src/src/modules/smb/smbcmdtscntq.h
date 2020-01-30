
/*-------------------------------------------------------------*/
/* NETWOX_SMBCMD_TYPE_NTTRANSACT_Q */

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SMBCMDTSCNTQ_TYPE_UNKNOWN = 0,
  NETWOX_SMBCMDTSCNTQ_TYPE_NTIOCTL,
  NETWOX_SMBCMDTSCNTQ_TYPE_NTNOTIFY,
  NETWOX_SMBCMDTSCNTQ_TYPE_NTQUERYSECUDESC,
  NETWOX_SMBCMDTSCNTQ_TYPE_NTSETSECUDESC
} netwox_smbcmdtscntq_type;

/*-------------------------------------------------------------*/
/* from Visual winioctl.h, and described in MSDN */
typedef enum {
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_REQUESTOPLOCKLEVEL1 = 0x00090000,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_REQUESTOPLOCKLEVEL2 = 0x00090004,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_REQUESTBATCHOPLOCK = 0x00090008,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_OPLOCKBREAKACKNOWLEDGE = 0x0009000C,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_OPBATCHACKCLOSEPENDING = 0x00090010,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_OPLOCKBREAKNOTIFY = 0x00090014,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_LOCKVOLUME = 0x00090018,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_UNLOCKVOLUME = 0x0009001C,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_DISMOUNTVOLUME = 0x00090020,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_ISVOLUMEMOUNTED = 0x00090028,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_ISPATHNAMEVALID = 0x0009002C,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_MARKVOLUMEDIRTY = 0x00090030,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_QUERYRETRIEVALPOINTERS = 0x0009003B,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_GETCOMPRESSION = 0x0009003C,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_SETCOMPRESSION = 0x0009C040,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_MARKASSYSTEMHIVE = 0x0009004F,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_OPLOCKBREAKACKNO2 = 0x00090050,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_INVALIDATEVOLUMES = 0x00090054,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_QUERYFATBPB = 0x00090058,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_REQUESTFILTEROPLOCK = 0x0009005C,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_FILESYSTEMGETSTATISTICS = 0x00090060,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_GETNTFSVOLUMEDATA = 0x00090064,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_GETNTFSFILERECORD = 0x00090068,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_GETVOLUMEBITMAP = 0x0009006F,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_GETRETRIEVALPOINTERS = 0x00090073,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_MOVEFILE = 0x00090074,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_ISVOLUMEDIRTY = 0x00090078,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_GETHFSINFORMATION = 0x0009007C,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_ALLOWEXTENDEDDASDIO = 0x00090083,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_READPROPERTYDATA = 0x00090087,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_WRITEPROPERTYDATA = 0x0009008B,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_FINDFILESBYSID = 0x0009008F,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_DUMPPROPERTYDATA = 0x00090097,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_SETOBJECTID = 0x00098098,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_GETOBJECTID = 0x0009009C,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_DELETEOBJECTID = 0x000980A0,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_SETREPARSEPOINT = 0x000980A4,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_GETREPARSEPOINT = 0x000900A8,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_DELETEREPARSEPOINT = 0x000980AC,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_ENUMUSNDATA = 0x000940B3,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_SECURITYIDCHECK = 0x000940B7,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_READUSNJOURNAL = 0x000940BB,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_SETOBJECTIDEXTENDED = 0x000980BC,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_CREATEORGETOBJECTID = 0x000900C0,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_SETSPARSE = 0x000980C4,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_SETZERODATA = 0x000980C8,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_QUERYALLOCATEDRANGES = 0x000940CF,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_ENABLEUPGRADE = 0x000980D0,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_SETENCRYPTION = 0x000900D4,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_ENCRYPTIONFSCTLIO = 0x000900DB,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_WRITERAWENCRYPTED = 0x000900DF,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_READRAWENCRYPTED = 0x000900E3,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_CREATEUSNJOURNAL = 0x000940E7,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_READFILEUSNDATA = 0x000940EB,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_WRITEUSNCLOSERECORD = 0x000940EF,
  NETWOX_SMBCMDTSCNTQ_NTIOCTL_FUNCTION_FSCTL_EXTENDVOLUME = 0x000900F0
} netwox_smbcmdtscntq_ntioctl_function;

/* Captured values are:
    ISVOLUMEMOUNTED
      request data: empty
      reply data: empty
    GETREPARSEPOINT
      request data: empty
      reply: Windows allways returns error NETWOX_SMBERR_NOTAREPARSEPOINT
      Note: a reparse point is a file with user defined data, and with
            attribute NETWOX_SMBCMDCMN_FILEATTR32_REPARSE. It is for example
            used to create a junction (soft NTFS link).
    CREATEORGETOBJECTID
      request data: empty
      reply data: 64 bytes:
                    16 bytes: object id
                    16 bytes: birth volume id
                    16 bytes: birth object id
                    16 bytes: domain id
*/

/* Structure */
typedef struct {
  netwox_smbcmdtscntq_ntioctl_function function; /* function to call */
  netwib_uint16 fileid;
  netwib_bool isfsctl; /* NETWIB_FALSE: this ioctl applies to a device
                          NETWIB_TRUE: this ioctl applies to a filesystem
                       */
  netwib_uint8 flags; /* bitfield
                           bit 0:
                             0: this ioctl applies to current share
                             1: this ioctl applies to share root handle (DFS)
                           other bits: unknown
                      */
  netwox_smbcmdcmn_data data;
} netwox_smbcmdtscntq_ntioctl;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 completionfilter; /* event to watch for */
#define NETWOX_SMBCMDTSCNTQ_NTNOTIFY_COMPLETIONFILTER_FILENAME       0x0001
#define NETWOX_SMBCMDTSCNTQ_NTNOTIFY_COMPLETIONFILTER_DIRNAME        0x0002
#define NETWOX_SMBCMDTSCNTQ_NTNOTIFY_COMPLETIONFILTER_ATTRIBUTE      0x0004
#define NETWOX_SMBCMDTSCNTQ_NTNOTIFY_COMPLETIONFILTER_SIZE           0x0008
#define NETWOX_SMBCMDTSCNTQ_NTNOTIFY_COMPLETIONFILTER_LASTWRITETIME  0x0010
#define NETWOX_SMBCMDTSCNTQ_NTNOTIFY_COMPLETIONFILTER_LASTACCESSTIME 0x0020
#define NETWOX_SMBCMDTSCNTQ_NTNOTIFY_COMPLETIONFILTER_CREATIONTIME   0x0040
#define NETWOX_SMBCMDTSCNTQ_NTNOTIFY_COMPLETIONFILTER_EA             0x0080
#define NETWOX_SMBCMDTSCNTQ_NTNOTIFY_COMPLETIONFILTER_SECU           0x0100
#define NETWOX_SMBCMDTSCNTQ_NTNOTIFY_COMPLETIONFILTER_STREAMNAME     0x0200
#define NETWOX_SMBCMDTSCNTQ_NTNOTIFY_COMPLETIONFILTER_STREAMSIZE     0x0400
#define NETWOX_SMBCMDTSCNTQ_NTNOTIFY_COMPLETIONFILTER_STREAMWRITE    0x0800
  netwib_uint16 fileid;
  netwib_uint16 watchtree; /* if also watch in subdirectories */
#define NETWOX_SMBCMDTSCNTQ_NTNOTIFY_WATCHTREE_CURDIRONLY 0
#define NETWOX_SMBCMDTSCNTQ_NTNOTIFY_WATCHTREE_ALSOSUBDIR 1
} netwox_smbcmdtscntq_ntnotify;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 fileid;
  netwib_uint32 secinfo; /* requested info */
#define NETWOX_SMBCMDTSCNTQ_SECUDESC_SECINFO_OWNER 0x01
#define NETWOX_SMBCMDTSCNTQ_SECUDESC_SECINFO_GROUP 0x02
#define NETWOX_SMBCMDTSCNTQ_SECUDESC_SECINFO_DACL  0x04
#define NETWOX_SMBCMDTSCNTQ_SECUDESC_SECINFO_SACL  0x08
} netwox_smbcmdtscntq_ntquerysecudesc;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 fileid;
  netwib_uint32 secinfo; /* set info */
  netwox_smbcmdcmn_data secudesc; /* too complex to decode for now */
} netwox_smbcmdtscntq_ntsetsecudesc;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdtscntq_type type;
  union {
    netwox_smbcmdtscntq_ntioctl ntioctl;
    netwox_smbcmdtscntq_ntnotify ntnotify;
    netwox_smbcmdtscntq_ntquerysecudesc ntquerysecudesc;
    netwox_smbcmdtscntq_ntsetsecudesc ntsetsecudesc;
  } value;
} netwox_smbcmdtscntq;
typedef const netwox_smbcmdtscntq netwox_constsmbcmdtscntq;

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntq_init(netwox_smbcmdtscntq *psmbcmdtscntq);
netwib_err netwox_smbcmdtscntq_selecttype(netwox_smbcmdtscntq *psmbcmdtscntq,
                                          netwox_smbcmdtscntq_type type);
netwib_err netwox_smbcmdtscntq_setdefault(netwox_smbcmdtscntq *psmbcmdtscntq);
netwib_err netwox_smbcmdtscntq_close(netwox_smbcmdtscntq *psmbcmdtscntq);

/*-------------------------------------------------------------*/
/* for those 2 functions :
    - don't forget to set isunicode before
    - they suppose query is stored only in first message (most
      of the time, query is small, so this is ok)
*/
netwib_err netwox_smbcmdtscntq_encode_smbcmd(netwox_constsmbcmd *psmbcmd,
                                          netwox_smbcmdtscntq *psmbcmdtscntq);
/* only decode setup/para/data. Other values have to be set by user */
netwib_err netwox_smbcmdtscntq_decode_smbcmd(netwox_constsmbcmdtscntq *psmbcmdtscntq,
                                             netwox_smbcmd *psmbcmd);

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntq_show(netwox_constsmbcmdtscntq *psmbcmdtscntq,
                                    netwib_buf *pbuf);
netwib_err netwox_smbcmdtscntq_display(netwib_bufpool *pbufpool,
                                      netwox_constsmbcmdtscntq *psmbcmdtscntq);
