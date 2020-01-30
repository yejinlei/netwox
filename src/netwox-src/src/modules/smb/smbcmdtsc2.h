
/*-------------------------------------------------------------*/
/* common structures for NETWOX_SMBCMD_TYPE_TRANSACTION2 */

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SMBCMDTSC2_FMT_FIND_STANDARD =        1,
  NETWOX_SMBCMDTSC2_FMT_FIND_DIRECTORY =     257,
  NETWOX_SMBCMDTSC2_FMT_FIND_FULLDIRECTORY = 258,
  NETWOX_SMBCMDTSC2_FMT_FIND_NAMESINFO =     259,
  NETWOX_SMBCMDTSC2_FMT_FIND_BOTHDIRECTORY = 260
} netwox_smbcmdtsc2_fmt_find;

/*----*/
typedef struct {
  netwox_smbcmdtsc2_fmt_find fmtfind;
  /*                                 indicates if used in each type */
  /*                                           S D F N B */
  netwib_uint32 resumekey;                  /* A X X X X */
  netwox_smbcmdcmn_time1601 creationtime;   /* X X X _ X */
  netwox_smbcmdcmn_time1601 lastaccesstime; /* X X X _ X */
  netwox_smbcmdcmn_time1601 lastwritetime;  /* X X X _ X */
  netwox_smbcmdcmn_time1601 changetime;     /* _ X X _ X */
  netwox_smbcmdcmn_uint64 filesize;         /* X X X _ X */
  netwox_smbcmdcmn_uint64 allocationsize;   /* X X X _ X */
  netwox_smbcmdcmn_fileattr16 fileattr16;   /* X _ _ _ _ */
  netwox_smbcmdcmn_fileattr32 fileattr32;   /* _ X X _ X */
  netwib_uint32 easize; /* ? always 0 */    /* _ _ X _ X */
  netwox_smbcmdcmn_data shortfilename;      /* X _ _ _ X */
  netwox_smbcmdcmn_data longfilename;       /* _ X X X X */
  /* Note A : used if NETWOX_SMBCMDTSC2Q_FIND2_FLAGS_RESUMEKEYS is set */
} netwox_smbcmdtsc2_fmt_find_entry;
netwib_err netwox_smbcmdtsc2_fmt_find_entry_init(netwox_smbcmdtsc2_fmt_find_entry *pitem);
netwib_err netwox_smbcmdtsc2_fmt_find_entry_setdefault(netwox_smbcmdtsc2_fmt_find fmtfind,
                                                       netwox_smbcmdtsc2_fmt_find_entry *pitem);
netwib_err netwox_smbcmdtsc2_fmt_find_entry_close(netwox_smbcmdtsc2_fmt_find_entry *pitem);
netwib_err netwox_smbcmdtsc2_fmt_find_entry_show(const netwox_smbcmdtsc2_fmt_find_entry *pitem,
                                                 netwib_buf *pbuf);
/* other functions are declared in smbcmdtsc2r.h because they need fmt */

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SMBCMDTSC2_FMT_FS_OLDALLOCATION =     1,
  NETWOX_SMBCMDTSC2_FMT_FS_OLDVOLUME =         2,
  NETWOX_SMBCMDTSC2_FMT_FS_LABEL =           257,
  NETWOX_SMBCMDTSC2_FMT_FS_VOLUME =          258,
  NETWOX_SMBCMDTSC2_FMT_FS_SIZE =            259,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICE =          260,
  NETWOX_SMBCMDTSC2_FMT_FS_ATTRIBUTE =       261,
  /* NT redefined values, but use same structures */
  NETWOX_SMBCMDTSC2_FMT_FS_EVOLUME =        1001,
  NETWOX_SMBCMDTSC2_FMT_FS_ELABEL =         1002,
  NETWOX_SMBCMDTSC2_FMT_FS_ESIZE =          1003,
  NETWOX_SMBCMDTSC2_FMT_FS_EDEVICE =        1004,
  NETWOX_SMBCMDTSC2_FMT_FS_EATTRIBUTE =     1005,
  NETWOX_SMBCMDTSC2_FMT_FS_EQUOTA =         1006,
  NETWOX_SMBCMDTSC2_FMT_FS_EFULLSIZE =      1007,
  NETWOX_SMBCMDTSC2_FMT_FS_EOBJECTID =      1008
} netwox_smbcmdtsc2_fmt_fs;

/*----*/
typedef struct {
  netwib_uint32 fsid;
  netwib_uint32 sectorsperunit;
  netwib_uint32 totalunits;
  netwib_uint32 freeunits;
  netwib_uint16 bytespersector;
} netwox_smbcmdtsc2_fmt_fs_oldallocation;
netwib_err netwox_smbcmdtsc2_fmt_fs_oldallocation_init(netwox_smbcmdtsc2_fmt_fs_oldallocation *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_oldallocation_setdefault(netwox_smbcmdtsc2_fmt_fs_oldallocation *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_oldallocation_close(netwox_smbcmdtsc2_fmt_fs_oldallocation *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_oldallocation_decode(netwib_data *pdata,
                             netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_fs_oldallocation *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_oldallocation_append(const netwox_smbcmdtsc2_fmt_fs_oldallocation *pitem,
                                                         netwib_buf *pbuf);
netwib_err netwox_smbcmdtsc2_fmt_fs_oldallocation_show(const netwox_smbcmdtsc2_fmt_fs_oldallocation *pitem,
                                                       netwib_buf *pbuf);

/*----*/
typedef struct {
  netwib_uint32 serialnumber;
  netwox_smbcmdcmn_data label;
} netwox_smbcmdtsc2_fmt_fs_oldvolume;
netwib_err netwox_smbcmdtsc2_fmt_fs_oldvolume_init(netwox_smbcmdtsc2_fmt_fs_oldvolume *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_oldvolume_setdefault(netwox_smbcmdtsc2_fmt_fs_oldvolume *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_oldvolume_close(netwox_smbcmdtsc2_fmt_fs_oldvolume *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_oldvolume_decode(netwib_data *pdata,
                             netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_fs_oldvolume *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_oldvolume_append(const netwox_smbcmdtsc2_fmt_fs_oldvolume *pitem,
                                                  netwib_buf *pbuf);
netwib_err netwox_smbcmdtsc2_fmt_fs_oldvolume_show(const netwox_smbcmdtsc2_fmt_fs_oldvolume *pitem,
                                                netwib_buf *pbuf);

/*----*/
typedef struct {
  netwox_smbcmdcmn_time1601 creationtime;
  netwib_uint32 serialnumber;
  netwox_smbcmdcmn_data label;
} netwox_smbcmdtsc2_fmt_fs_volume;
netwib_err netwox_smbcmdtsc2_fmt_fs_volume_init(netwox_smbcmdtsc2_fmt_fs_volume *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_volume_setdefault(netwox_smbcmdtsc2_fmt_fs_volume *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_volume_close(netwox_smbcmdtsc2_fmt_fs_volume *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_volume_decode(netwib_data *pdata,
                             netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_fs_volume *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_volume_append(const netwox_smbcmdtsc2_fmt_fs_volume *pitem,
                                                  netwib_buf *pbuf);
netwib_err netwox_smbcmdtsc2_fmt_fs_volume_show(const netwox_smbcmdtsc2_fmt_fs_volume *pitem,
                                                netwib_buf *pbuf);

/*----*/
typedef struct {
  netwox_smbcmdcmn_uint64 totalunits;
  netwox_smbcmdcmn_uint64 freeunits;
  netwib_uint32 sectorsperunit;
  netwib_uint32 bytespersector;
} netwox_smbcmdtsc2_fmt_fs_size;
netwib_err netwox_smbcmdtsc2_fmt_fs_size_init(netwox_smbcmdtsc2_fmt_fs_size *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_size_setdefault(netwox_smbcmdtsc2_fmt_fs_size *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_size_close(netwox_smbcmdtsc2_fmt_fs_size *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_size_decode(netwib_data *pdata,
                             netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_fs_size *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_size_append(const netwox_smbcmdtsc2_fmt_fs_size *pitem,
                                                netwib_buf *pbuf);
netwib_err netwox_smbcmdtsc2_fmt_fs_size_show(const netwox_smbcmdtsc2_fmt_fs_size *pitem,
                                              netwib_buf *pbuf);

/*----*/
typedef enum {
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_BEEP =               1,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_CDROM =              2,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_CDROMFILESYSTEM =    3,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_CONTROLLER =         4,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_DATALINK =           5,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_DFS =                6,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_DISK =               7,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_DISKFILESYSTEM =     8,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_FILESYSTEM =         9,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_IMPORTPORT =        10,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_KEYBOARD =          11,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_MAILSLOT =          12,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_MIDIIN =            13,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_MIDIOUT =           14,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_MOUSE =             15,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_MULTIUNCPROVIDER =  16,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_NAMEDPIPE =         17,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_NETWORK =           18,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_NETWORKBROWSER =    19,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_NETWORKFILESYTEM =  20,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_NULL =              21,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_PARALLERPORT =      22,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_PHYSICALCARD =      23,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_PRINTER =           24,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_SCANNER =           25,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_SERIALMOUSEPORT =   26,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_SERIALPORT =        27,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_SCREEN =            28,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_SOUND =             29,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_STREAMS =           30,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_TAPE =              31,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_TAPEFILESYSTEM =    32,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_TRANSPORT =         33,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_UNKNOWN =           34,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_VIDEO =             35,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_VIRTUALDISK =       36,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_WAVEIN =            37,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_WAVEOUT =           38,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_8042PORT =          39,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_NETWORKREDIRECTOR = 40,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_BATTERY =           41,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_BUSEXTENDER =       42,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_MODEM =             43,
  NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_VDM =               44
} netwox_smbcmdtsc2_fmt_fs_devicetype;
#define NETWOX_SMBCMDTSC2_FMT_FS_DEVICECHARACTERISTICS_REMOVABLE 0x01
#define NETWOX_SMBCMDTSC2_FMT_FS_DEVICECHARACTERISTICS_READONLY  0x02
#define NETWOX_SMBCMDTSC2_FMT_FS_DEVICECHARACTERISTICS_FLOPPY    0x04
#define NETWOX_SMBCMDTSC2_FMT_FS_DEVICECHARACTERISTICS_WRITEONE  0x08
#define NETWOX_SMBCMDTSC2_FMT_FS_DEVICECHARACTERISTICS_REMOTE    0x10
#define NETWOX_SMBCMDTSC2_FMT_FS_DEVICECHARACTERISTICS_MOUNTED   0x20
#define NETWOX_SMBCMDTSC2_FMT_FS_DEVICECHARACTERISTICS_VIRTUAL   0x40
typedef struct {
  netwox_smbcmdtsc2_fmt_fs_devicetype devicetype;
  netwib_uint32 devicecharacteristics;
} netwox_smbcmdtsc2_fmt_fs_device;
netwib_err netwox_smbcmdtsc2_fmt_fs_device_init(netwox_smbcmdtsc2_fmt_fs_device *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_device_setdefault(netwox_smbcmdtsc2_fmt_fs_device *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_device_close(netwox_smbcmdtsc2_fmt_fs_device *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_device_decode(netwib_data *pdata,
                             netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_fs_device *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_device_append(const netwox_smbcmdtsc2_fmt_fs_device *pitem,
                                                  netwib_buf *pbuf);
netwib_err netwox_smbcmdtsc2_fmt_fs_device_show(const netwox_smbcmdtsc2_fmt_fs_device *pitem,
                                                netwib_buf *pbuf);

/*----*/
typedef struct {
  netwib_uint32 fsattributes;
#define NETWOX_SMBCMDTSC2_FS_ATTRIBUTE_CASESENSSEARCH 0x00000001
#define NETWOX_SMBCMDTSC2_FS_ATTRIBUTE_CASEPRESERVING 0x00000002
#define NETWOX_SMBCMDTSC2_FS_ATTRIBUTE_PERSISTENTACL  0x00000004
#define NETWOX_SMBCMDTSC2_FS_ATTRIBUTE_COMPRESSEDFILE 0x00000008
#define NETWOX_SMBCMDTSC2_FS_ATTRIBUTE_VOLUMEQUOTAS   0x00000010
#define NETWOX_SMBCMDTSC2_FS_ATTRIBUTE_MOUNTED        0x00000020
#define NETWOX_SMBCMDTSC2_FS_ATTRIBUTE_COMPRESSEDVOL  0x00008000
  netwib_uint32 maxnamesize; /* 255 */
  netwox_smbcmdcmn_data fsname; /* FAT, FAT32 or NTFS */
} netwox_smbcmdtsc2_fmt_fs_attribute;
netwib_err netwox_smbcmdtsc2_fmt_fs_attribute_init(netwox_smbcmdtsc2_fmt_fs_attribute *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_attribute_setdefault(netwox_smbcmdtsc2_fmt_fs_attribute *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_attribute_close(netwox_smbcmdtsc2_fmt_fs_attribute *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_attribute_decode(netwib_data *pdata,
                             netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_fs_attribute *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_attribute_append(const netwox_smbcmdtsc2_fmt_fs_attribute *pitem,
                                                     netwib_buf *pbuf);
netwib_err netwox_smbcmdtsc2_fmt_fs_attribute_show(const netwox_smbcmdtsc2_fmt_fs_attribute *pitem,
                                                   netwib_buf *pbuf);

/*----*/
typedef struct {
  netwox_smbcmdcmn_uint64 totalunits;
  netwox_smbcmdcmn_uint64 callerfreeunits;
  netwox_smbcmdcmn_uint64 actualfreeunits;
  netwib_uint32 sectorsperunit;
  netwib_uint32 bytespersector;
} netwox_smbcmdtsc2_fmt_fs_efullsize;
netwib_err netwox_smbcmdtsc2_fmt_fs_efullsize_init(netwox_smbcmdtsc2_fmt_fs_efullsize *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_efullsize_setdefault(netwox_smbcmdtsc2_fmt_fs_efullsize *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_efullsize_close(netwox_smbcmdtsc2_fmt_fs_efullsize *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_efullsize_decode(netwib_data *pdata,
                             netwib_uint32 *pdataefullsize,
                             netwox_smbcmdtsc2_fmt_fs_efullsize *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_efullsize_append(const netwox_smbcmdtsc2_fmt_fs_efullsize *pitem,
                                                     netwib_buf *pbuf);
netwib_err netwox_smbcmdtsc2_fmt_fs_efullsize_show(const netwox_smbcmdtsc2_fmt_fs_efullsize *pitem,
                                                   netwib_buf *pbuf);

/*----*/
typedef struct {
  netwox_smbcmdcmn_data guid;
  netwox_smbcmdcmn_data unknown;
} netwox_smbcmdtsc2_fmt_fs_eobjectid;
netwib_err netwox_smbcmdtsc2_fmt_fs_eobjectid_init(netwox_smbcmdtsc2_fmt_fs_eobjectid *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_eobjectid_setdefault(netwox_smbcmdtsc2_fmt_fs_eobjectid *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_eobjectid_close(netwox_smbcmdtsc2_fmt_fs_eobjectid *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_eobjectid_decode(netwib_data *pdata,
                             netwib_uint32 *pdataeobjectid,
                             netwox_smbcmdtsc2_fmt_fs_eobjectid *pitem);
netwib_err netwox_smbcmdtsc2_fmt_fs_eobjectid_append(const netwox_smbcmdtsc2_fmt_fs_eobjectid *pitem,
                                                     netwib_buf *pbuf);
netwib_err netwox_smbcmdtsc2_fmt_fs_eobjectid_show(const netwox_smbcmdtsc2_fmt_fs_eobjectid *pitem,
                                                   netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* values used in QueryFileInfo or QueryPathInfo */
typedef enum {
  NETWOX_SMBCMDTSC2_FMT_FILEQ_BASIC =           257,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_STANDARD =        258,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EA =              259,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_NAME =            260,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_ALLOCATION =      261,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EOF =             262,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_ALL =             263,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_ALTNAME =         264,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_STREAM =          265,
  /* NT redefined values, but used same structures */
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EDIRECTORY =     1001,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EFULLDIRECTORY = 1002,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EBOTHDIRECTORY = 1003,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EBASIC =         1004,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_ESTANDARD =      1005,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EINTERNAL =      1006,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EEA =            1007,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EACCESS =        1008,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_ENAME =          1009,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_ERENAME =        1010,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_ELINK =          1011,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_ENAMES =         1012,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EDISPOSITION =   1013,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EPOSITION =      1014,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EFULL_EA =       1015,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EMODE =          1016,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EALIGNMENT =     1017,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EALL =           1018,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EALLOCATION =    1019,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EEOF =           1020,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EALTERNATENAME = 1021,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_ESTREAM =        1022,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EPIPE =          1023,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EPIPELOCAL =     1024,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EPIPEREMOTE =    1025,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EMAILSLOTQUERY = 1026,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EMAILSLOTSET =   1027,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_ECOMPRESSION =   1028,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EOBJECTID =      1029,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_ECOMPLETION =    1030,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EMOVECLUSTER =   1031,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EQUOTA =         1032,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EREPARSEPOINT =  1033,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_ENETWORKOPEN =   1034,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EATTRIBUTETAG =  1035,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_ETRACKING =      1036,
  NETWOX_SMBCMDTSC2_FMT_FILEQ_EMAXIMUM =       1037
} netwox_smbcmdtsc2_fmt_fileq;

/*-------------------------------------------------------------*/
/* values used in SetFileInfo */
typedef enum {
  NETWOX_SMBCMDTSC2_FMT_FILES_BASIC =           257,
  NETWOX_SMBCMDTSC2_FMT_FILES_DISPOSITION =     258,
  NETWOX_SMBCMDTSC2_FMT_FILES_ALLOCATION =      259,
  NETWOX_SMBCMDTSC2_FMT_FILES_EOF =             260,
  /* NT redefined values, but use same structures */
  NETWOX_SMBCMDTSC2_FMT_FILES_EBASIC =         1004,
  NETWOX_SMBCMDTSC2_FMT_FILES_EDISPOSITION =   1013,
  NETWOX_SMBCMDTSC2_FMT_FILES_EALLOCATION =    1019,
  NETWOX_SMBCMDTSC2_FMT_FILES_EEOF =           1020
} netwox_smbcmdtsc2_fmt_files;

/*----*/
typedef struct {
  netwox_smbcmdcmn_time1601 creationtime;
  netwox_smbcmdcmn_time1601 lastaccesstime;
  netwox_smbcmdcmn_time1601 lastwritetime;
  netwox_smbcmdcmn_time1601 changetime;
  netwox_smbcmdcmn_fileattr32 fileattributes;
} netwox_smbcmdtsc2_fmt_file_basic;
netwib_err netwox_smbcmdtsc2_fmt_file_basic_init(netwox_smbcmdtsc2_fmt_file_basic *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_basic_setdefault(netwox_smbcmdtsc2_fmt_file_basic *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_basic_close(netwox_smbcmdtsc2_fmt_file_basic *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_basic_decode(netwib_data *pdata,
                                                   netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_file_basic *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_basic_append(const netwox_smbcmdtsc2_fmt_file_basic *pitem,
                                                   netwib_buf *pbuf);
netwib_err netwox_smbcmdtsc2_fmt_file_basic_show(const netwox_smbcmdtsc2_fmt_file_basic *pitem,
                                                 netwib_buf *pbuf);

/*----*/
typedef struct {
  netwox_smbcmdcmn_uint64 allocationsize;
  netwox_smbcmdcmn_uint64 filesize;
  netwib_uint32 linkcount; /* at least 1 */
  netwib_uint16 deletepending; /* 0 or 256 */
  netwib_bool isdirectory;
} netwox_smbcmdtsc2_fmt_file_standard;
netwib_err netwox_smbcmdtsc2_fmt_file_standard_init(netwox_smbcmdtsc2_fmt_file_standard *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_standard_setdefault(netwox_smbcmdtsc2_fmt_file_standard *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_standard_close(netwox_smbcmdtsc2_fmt_file_standard *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_standard_decode(netwib_data *pdata,
                                                      netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_file_standard *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_standard_append(const netwox_smbcmdtsc2_fmt_file_standard *pitem,
                                                      netwib_buf *pbuf);
netwib_err netwox_smbcmdtsc2_fmt_file_standard_show(const netwox_smbcmdtsc2_fmt_file_standard *pitem,
                                                    netwib_buf *pbuf);

/*----*/
typedef struct {
  netwib_uint32 easize; /* ?, seen 0, 5, 9, 11 or 15 */
} netwox_smbcmdtsc2_fmt_file_ea;
netwib_err netwox_smbcmdtsc2_fmt_file_ea_init(netwox_smbcmdtsc2_fmt_file_ea *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_ea_setdefault(netwox_smbcmdtsc2_fmt_file_ea *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_ea_close(netwox_smbcmdtsc2_fmt_file_ea *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_ea_decode(netwib_data *pdata,
                                                netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_file_ea *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_ea_append(const netwox_smbcmdtsc2_fmt_file_ea *pitem,
                                                netwib_buf *pbuf);
netwib_err netwox_smbcmdtsc2_fmt_file_ea_show(const netwox_smbcmdtsc2_fmt_file_ea *pitem,
                                              netwib_buf *pbuf);

/*----*/
typedef struct {
  netwox_smbcmdcmn_data filename;
} netwox_smbcmdtsc2_fmt_file_name;
netwib_err netwox_smbcmdtsc2_fmt_file_name_init(netwox_smbcmdtsc2_fmt_file_name *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_name_setdefault(netwox_smbcmdtsc2_fmt_file_name *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_name_close(netwox_smbcmdtsc2_fmt_file_name *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_name_decode(netwib_data *pdata,
                                                  netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_file_name *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_name_append(const netwox_smbcmdtsc2_fmt_file_name *pitem,
                                                  netwib_buf *pbuf);
netwib_err netwox_smbcmdtsc2_fmt_file_name_show(const netwox_smbcmdtsc2_fmt_file_name *pitem,
                                                netwib_buf *pbuf);

/*----*/
typedef struct {
  netwox_smbcmdcmn_uint64 allocationsize;
} netwox_smbcmdtsc2_fmt_file_allocation;
netwib_err netwox_smbcmdtsc2_fmt_file_allocation_init(netwox_smbcmdtsc2_fmt_file_allocation *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_allocation_setdefault(netwox_smbcmdtsc2_fmt_file_allocation *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_allocation_close(netwox_smbcmdtsc2_fmt_file_allocation *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_allocation_decode(netwib_data *pdata,
                             netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_file_allocation *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_allocation_append(const netwox_smbcmdtsc2_fmt_file_allocation *pitem,
                                                        netwib_buf *pbuf);
netwib_err netwox_smbcmdtsc2_fmt_file_allocation_show(const netwox_smbcmdtsc2_fmt_file_allocation *pitem,
                                                      netwib_buf *pbuf);

/*----*/
typedef struct {
  netwox_smbcmdcmn_uint64 filesize;
} netwox_smbcmdtsc2_fmt_file_eof;
netwib_err netwox_smbcmdtsc2_fmt_file_eof_init(netwox_smbcmdtsc2_fmt_file_eof *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_eof_setdefault(netwox_smbcmdtsc2_fmt_file_eof *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_eof_close(netwox_smbcmdtsc2_fmt_file_eof *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_eof_decode(netwib_data *pdata,
                             netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_file_eof *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_eof_append(const netwox_smbcmdtsc2_fmt_file_eof *pitem,
                                                 netwib_buf *pbuf);
netwib_err netwox_smbcmdtsc2_fmt_file_eof_show(const netwox_smbcmdtsc2_fmt_file_eof *pitem,
                                               netwib_buf *pbuf);

/*----*/
typedef struct {
  netwox_smbcmdcmn_time1601 creationtime;
  netwox_smbcmdcmn_time1601 lastaccesstime;
  netwox_smbcmdcmn_time1601 lastwritetime;
  netwox_smbcmdcmn_time1601 changetime;
  netwox_smbcmdcmn_fileattr32 fileattributes;
  netwib_uint32 unknown1; /* ?, always zero */
  netwox_smbcmdcmn_uint64 allocationsize;
  netwox_smbcmdcmn_uint64 filesize;
  netwib_uint32 linkcount;
  netwib_bool deletepending;
  netwib_bool isdirectory;
  netwib_uint16 unknown2; /* ?, always zero */
  netwib_uint32 easize;
  netwox_smbcmdcmn_data filename;
} netwox_smbcmdtsc2_fmt_file_all;
netwib_err netwox_smbcmdtsc2_fmt_file_all_init(netwox_smbcmdtsc2_fmt_file_all *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_all_setdefault(netwox_smbcmdtsc2_fmt_file_all *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_all_close(netwox_smbcmdtsc2_fmt_file_all *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_all_decode(netwib_data *pdata,
                             netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_file_all *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_all_append(const netwox_smbcmdtsc2_fmt_file_all *pitem,
                                                 netwib_buf *pbuf);
netwib_err netwox_smbcmdtsc2_fmt_file_all_show(const netwox_smbcmdtsc2_fmt_file_all *pitem,
                                               netwib_buf *pbuf);

/*----*/
typedef struct {
  netwox_smbcmdcmn_data filename;
} netwox_smbcmdtsc2_fmt_file_altname;
netwib_err netwox_smbcmdtsc2_fmt_file_altname_init(netwox_smbcmdtsc2_fmt_file_altname *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_altname_setdefault(netwox_smbcmdtsc2_fmt_file_altname *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_altname_close(netwox_smbcmdtsc2_fmt_file_altname *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_altname_decode(netwib_data *pdata,
                                                     netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_file_altname *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_altname_append(const netwox_smbcmdtsc2_fmt_file_altname *pitem,
                                                     netwib_buf *pbuf);
netwib_err netwox_smbcmdtsc2_fmt_file_altname_show(const netwox_smbcmdtsc2_fmt_file_altname *pitem,
                                                   netwib_buf *pbuf);

/*----*/
typedef struct {
  netwox_smbcmdcmn_uint64 streamsize;
  netwox_smbcmdcmn_uint64 allocationsize;
  netwox_smbcmdcmn_data name;
} netwox_smbcmdtsc2_fmt_file_stream;
netwib_err netwox_smbcmdtsc2_fmt_file_stream_init(netwox_smbcmdtsc2_fmt_file_stream *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_stream_setdefault(netwox_smbcmdtsc2_fmt_file_stream *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_stream_close(netwox_smbcmdtsc2_fmt_file_stream *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_stream_decode(netwib_data *pdata,
                             netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_file_stream *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_stream_size(netwox_smbcmdtsc2_fmt_file_stream *pitem,
                                                  netwib_uint32 *psize);
netwib_err netwox_smbcmdtsc2_fmt_file_stream_append(const netwox_smbcmdtsc2_fmt_file_stream *pitem,
                                                    netwib_buf *pbuf);
netwib_err netwox_smbcmdtsc2_fmt_file_stream_show(const netwox_smbcmdtsc2_fmt_file_stream *pitem,
                                                  netwib_buf *pbuf);

/*----*/
/* implemented without seing it on wire */
typedef struct {
  netwib_bool deleteonclose;
} netwox_smbcmdtsc2_fmt_file_disposition;
netwib_err netwox_smbcmdtsc2_fmt_file_disposition_init(netwox_smbcmdtsc2_fmt_file_disposition *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_disposition_setdefault(netwox_smbcmdtsc2_fmt_file_disposition *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_disposition_close(netwox_smbcmdtsc2_fmt_file_disposition *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_disposition_decode(netwib_data *pdata,
                             netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_file_disposition *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_disposition_append(const netwox_smbcmdtsc2_fmt_file_disposition *pitem,
                                                         netwib_buf *pbuf);
netwib_err netwox_smbcmdtsc2_fmt_file_disposition_show(const netwox_smbcmdtsc2_fmt_file_disposition *pitem,
                                                       netwib_buf *pbuf);

/*----*/
typedef struct {
  netwib_uint32 dev;
  netwib_uint32 ino;
} netwox_smbcmdtsc2_fmt_file_einternal;
netwib_err netwox_smbcmdtsc2_fmt_file_einternal_init(netwox_smbcmdtsc2_fmt_file_einternal *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_einternal_setdefault(netwox_smbcmdtsc2_fmt_file_einternal *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_einternal_close(netwox_smbcmdtsc2_fmt_file_einternal *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_einternal_decode(netwib_data *pdata,
                             netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_file_einternal *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_einternal_append(const netwox_smbcmdtsc2_fmt_file_einternal *pitem,
                                                       netwib_buf *pbuf);
netwib_err netwox_smbcmdtsc2_fmt_file_einternal_show(const netwox_smbcmdtsc2_fmt_file_einternal *pitem,
                                                     netwib_buf *pbuf);

/*----*/
typedef struct {
  netwox_smbcmdcmn_time1601 creationtime;
  netwox_smbcmdcmn_time1601 lastaccesstime;
  netwox_smbcmdcmn_time1601 lastwritetime;
  netwox_smbcmdcmn_time1601 changetime;
  netwox_smbcmdcmn_uint64 allocationsize;
  netwox_smbcmdcmn_uint64 filesize;
  netwox_smbcmdcmn_fileattr32 fileattributes;
} netwox_smbcmdtsc2_fmt_file_enetworkopen;
netwib_err netwox_smbcmdtsc2_fmt_file_enetworkopen_init(netwox_smbcmdtsc2_fmt_file_enetworkopen *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_enetworkopen_setdefault(netwox_smbcmdtsc2_fmt_file_enetworkopen *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_enetworkopen_close(netwox_smbcmdtsc2_fmt_file_enetworkopen *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_enetworkopen_decode(netwib_data *pdata,
                             netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_file_enetworkopen *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_enetworkopen_append(const netwox_smbcmdtsc2_fmt_file_enetworkopen *pitem,
                                                          netwib_buf *pbuf);
netwib_err netwox_smbcmdtsc2_fmt_file_enetworkopen_show(const netwox_smbcmdtsc2_fmt_file_enetworkopen *pitem,
                                                        netwib_buf *pbuf);

/*----*/
typedef struct {
  netwib_uint32 mode; /* ?, 16 or 32 */
  netwib_uint32 unknown; /* ?, always 0 */
} netwox_smbcmdtsc2_fmt_file_eattributetag;
netwib_err netwox_smbcmdtsc2_fmt_file_eattributetag_init(netwox_smbcmdtsc2_fmt_file_eattributetag *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_eattributetag_setdefault(netwox_smbcmdtsc2_fmt_file_eattributetag *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_eattributetag_close(netwox_smbcmdtsc2_fmt_file_eattributetag *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_eattributetag_decode(netwib_data *pdata,
                             netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_file_eattributetag *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_eattributetag_append(const netwox_smbcmdtsc2_fmt_file_eattributetag *pitem,
                                                           netwib_buf *pbuf);
netwib_err netwox_smbcmdtsc2_fmt_file_eattributetag_show(const netwox_smbcmdtsc2_fmt_file_eattributetag *pitem,
                                                         netwib_buf *pbuf);

/*----*/
/* for fast/first implementation of unknown types */
typedef struct {
  netwox_smbcmdcmn_data unknown;
} netwox_smbcmdtsc2_fmt_file_unknown;
netwib_err netwox_smbcmdtsc2_fmt_file_unknown_init(netwox_smbcmdtsc2_fmt_file_unknown *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_unknown_setdefault(netwox_smbcmdtsc2_fmt_file_unknown *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_unknown_close(netwox_smbcmdtsc2_fmt_file_unknown *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_unknown_decode(netwib_data *pdata,
                             netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_file_unknown *pitem);
netwib_err netwox_smbcmdtsc2_fmt_file_unknown_append(const netwox_smbcmdtsc2_fmt_file_unknown *pitem,
                                                     netwib_buf *pbuf);
netwib_err netwox_smbcmdtsc2_fmt_file_unknown_show(const netwox_smbcmdtsc2_fmt_file_unknown *pitem,
                                                   netwib_buf *pbuf);
