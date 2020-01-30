
/*-------------------------------------------------------------*/
/* NETWOX_SMBCMD_TYPE_TRANSACTION2_R */

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bool isunicode;
  netwib_uint8 setupcount;
  netwib_uint16 setup[NETWOX_SMBCMD_TRANSACTION2_SETUP_MAX];
  netwib_buf para;
  netwib_buf data;
} netwox_smbcmdtsc2rstorage;
typedef const netwox_smbcmdtsc2rstorage netwox_constsmbcmdtsc2rstorage;
/* buffers have CANALLOC flag */
netwib_err netwox_smbcmdtsc2rstorage_init(netwib_bool isunicode,
                                          netwox_smbcmdtsc2rstorage *pstorage);
/* buffers are closed if they were allocated */
netwib_err netwox_smbcmdtsc2rstorage_close(netwox_smbcmdtsc2rstorage *pstorage);
/*
   - if smbcmd contains all the data:
       - pneedmoremsg is set to false
       - buf_init_ext_buf is used (so don't close smbcmd after this function)
   - else:
       - pneedmoremsg is set to true
       - buf_append_buf is used (so buffers are allocated)
*/
netwib_err netwox_smbcmdtsc2rstorage_append_smbcmd(netwox_constsmbcmd *psmbcmd,
                                          netwox_smbcmdtsc2rstorage *pstorage,
                                                   netwib_bool *pneedmoremsg);
/* Set offset to 0 before first call.
   This function only decodes setup/para/data using buf_init_ext (so
   don't close storage after this function). Other values must be set by user.
*/
netwib_err netwox_smbcmdtsc2rstorage_decode_smbcmd(netwox_constsmbcmdtsc2rstorage *pstorage,
                                                   netwib_uint32 *poffset,
                                                   netwib_uint32 maxsize,
                                                   netwox_smbcmd *psmbcmd,
                                                   netwib_bool *pneedmoremsg);

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SMBCMDTSC2R_TYPE_UNKNOWN = 0,
  NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_STANDARD,
  NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_DIRECTORY,
  NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_FULLDIRECTORY,
  NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_NAMESINFO,
  NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_BOTHDIRECTORY,
  NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_STANDARD,
  NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_DIRECTORY,
  NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_FULLDIRECTORY,
  NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_NAMESINFO,
  NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_BOTHDIRECTORY,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_OLDALLOCATION,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_OLDVOLUME,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_VOLUME,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_SIZE,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_DEVICE,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_ATTRIBUTE,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_EFULLSIZE,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_EOBJECTID,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_BASIC,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_STANDARD,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EA,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_NAME,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ALL,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ALTNAME,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_STREAM,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EBASIC,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ESTANDARD,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EEA,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ENAME,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EALL,
  /*NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EALTNAME not redefined by NT */
  NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ESTREAM,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EINTERNAL,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ENETWORKOPEN,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EATTRIBUTETAG,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_BASIC,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_STANDARD,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EA,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_NAME,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ALL,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ALTNAME,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_STREAM,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EBASIC,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ESTANDARD,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EEA,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ENAME,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EALL,
  /*NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EALTNAME not redefined by NT */
  NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ESTREAM,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EINTERNAL,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ENETWORKOPEN,
  NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EATTRIBUTETAG,
  NETWOX_SMBCMDTSC2R_TYPE_SETFILEINFO
} netwox_smbcmdtsc2r_type;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 sid;
  netwib_bool endofsearch; /* if a FINDNEXT2 is needed */
  netwib_uint16 eaerroroffset; /* ?, apparently always zero */
  netwib_array entries; /* array of netwox_smbcmdtsc2_fmt_find_entry */
} netwox_smbcmdtsc2r_findfirst2;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bool endofsearch; /* if another FINDNEXT2 is needed */
  netwib_uint16 eaerroroffset; /* ?, apparently always zero */
  netwib_array entries; /* array of netwox_smbcmdtsc2_fmt_find_entry */
} netwox_smbcmdtsc2r_findnext2;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 unknown; /* ?, apparently always zero */
  netwib_array streams; /* array of netwox_smbcmdtsc2_fmt_file_stream */
} netwox_smbcmdtsc2r_queryfileinfo_stream;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 unknown; /* ?, apparently always zero */
} netwox_smbcmdtsc2r_setfileinfo;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdtsc2r_type type;
  union {
    netwox_smbcmdtsc2r_findfirst2 findfirst2_common;
    netwox_smbcmdtsc2r_findnext2 findnext2_common;
    netwox_smbcmdtsc2_fmt_fs_oldallocation queryfsinfo_oldallocation;
    netwox_smbcmdtsc2_fmt_fs_oldvolume queryfsinfo_oldvolume;
    netwox_smbcmdtsc2_fmt_fs_volume queryfsinfo_volume;
    netwox_smbcmdtsc2_fmt_fs_size queryfsinfo_size;
    netwox_smbcmdtsc2_fmt_fs_device queryfsinfo_device;
    netwox_smbcmdtsc2_fmt_fs_attribute queryfsinfo_attribute;
    netwox_smbcmdtsc2_fmt_fs_efullsize queryfsinfo_efullsize;
    netwox_smbcmdtsc2_fmt_fs_eobjectid queryfsinfo_eobjectid;
    netwox_smbcmdtsc2_fmt_file_basic querypathfileinfo_basic;
    netwox_smbcmdtsc2_fmt_file_standard querypathfileinfo_standard;
    netwox_smbcmdtsc2_fmt_file_ea querypathfileinfo_ea;
    netwox_smbcmdtsc2_fmt_file_name querypathfileinfo_name;
    netwox_smbcmdtsc2_fmt_file_all querypathfileinfo_all;
    netwox_smbcmdtsc2_fmt_file_altname querypathfileinfo_altname;
    netwox_smbcmdtsc2r_queryfileinfo_stream querypathfileinfo_stream;
    netwox_smbcmdtsc2_fmt_file_einternal querypathfileinfo_einternal;
    netwox_smbcmdtsc2_fmt_file_eattributetag querypathfileinfo_eattributetag;
    netwox_smbcmdtsc2_fmt_file_enetworkopen querypathfileinfo_enetworkopen;
    netwox_smbcmdtsc2r_setfileinfo setfileinfo;
  } value;
} netwox_smbcmdtsc2r;
typedef const netwox_smbcmdtsc2r netwox_constsmbcmdtsc2r;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdtsc2r_type type;
  netwox_smbcmdtsc2_fmt_find fmtfind;
  netwib_bool fmtfindresumekeys;
} netwox_smbcmdtsc2rfmt;
typedef const netwox_smbcmdtsc2rfmt netwox_constsmbcmdtsc2rfmt;
netwib_err netwox_smbcmdtsc2rfmt_init_smbcmdtsc2q(netwox_constsmbcmdtsc2q *psmbcmdtsc2q,
                                                  netwox_smbcmdtsc2rfmt *psmbcmdtsc2rfmt);

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2r_init(netwox_smbcmdtsc2r *psmbcmdtsc2r);
netwib_err netwox_smbcmdtsc2r_selecttype(netwox_smbcmdtsc2r *psmbcmdtsc2r,
                                         netwox_smbcmdtsc2r_type type);
netwib_err netwox_smbcmdtsc2r_setdefault(netwox_smbcmdtsc2r *psmbcmdtsc2r);
netwib_err netwox_smbcmdtsc2r_close(netwox_smbcmdtsc2r *psmbcmdtsc2r);

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2r_encode_storager(netwox_constsmbcmdtsc2rstorage *pstorage,
                                  netwox_constsmbcmdtsc2rfmt *psmbcmdtsc2rfmt,
                                  netwox_smbcmdtsc2r *psmbcmdtsc2r);
netwib_err netwox_smbcmdtsc2r_decode_storager(netwox_constsmbcmdtsc2r *psmbcmdtsc2r,
                                  netwox_constsmbcmdtsc2rfmt *psmbcmdtsc2rfmt,
                                  netwox_smbcmdtsc2rstorage *pstorage);

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2r_show(netwox_constsmbcmdtsc2r *psmbcmdtsc2r,
                                   netwib_buf *pbuf);
netwib_err netwox_smbcmdtsc2r_display(netwib_bufpool *pbufpool,
                                      netwox_constsmbcmdtsc2r *psmbcmdtsc2r);

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_find_entry_decode(netwox_constsmbcmdtsc2rfmt *prfmt,
                                                   netwib_data *pdata,
                                                   netwib_uint32 *pdatasize,
                                                   netwox_smbcmdtsc2_fmt_find_entry *pitem);
netwib_err netwox_smbcmdtsc2_fmt_find_entry_offsetfilename(netwox_constsmbcmdtsc2rfmt *prfmt,
                                                           netwib_uint32 *psize);
netwib_err netwox_smbcmdtsc2_fmt_find_entry_size(netwox_constsmbcmdtsc2rfmt *prfmt,
                                                 netwox_smbcmdtsc2_fmt_find_entry *pitem,
                                                 netwib_uint32 *psize);
netwib_err netwox_smbcmdtsc2_fmt_find_entry_size_standard(netwox_constsmbcmdtsc2rfmt *prfmt,
                                                          netwib_uint32 *psize);
netwib_err netwox_smbcmdtsc2_fmt_find_entry_append(netwox_constsmbcmdtsc2rfmt *prfmt,
                                                   const netwox_smbcmdtsc2_fmt_find_entry *pitem,
                                                   netwib_buf *pbuf);
