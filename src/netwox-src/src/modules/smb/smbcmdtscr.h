
/*-------------------------------------------------------------*/
/* NETWOX_SMBCMD_TYPE_TRANSACTION_R */

/* Note: in this module there is a lot of redundant code, because
         each function can be slightly different, and each type
         is complex to implement
*/

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bool isunicode;
  netwib_uint8 setupcount;
  netwib_uint16 setup[NETWOX_SMBCMD_TRANSACTION_SETUP_MAX];
  netwib_buf para;
  netwib_buf data;
} netwox_smbcmdtscrstorage;
typedef const netwox_smbcmdtscrstorage netwox_constsmbcmdtscrstorage;
/* buffers have CANALLOC flag */
netwib_err netwox_smbcmdtscrstorage_init(netwib_bool isunicode,
                                         netwox_smbcmdtscrstorage *pstorage);
/* buffers are closed if they were allocated */
netwib_err netwox_smbcmdtscrstorage_close(netwox_smbcmdtscrstorage *pstorage);
/*
   - if smbcmd contains all the data:
       - pneedmoremsg is set to false
       - buf_init_ext_buf is used (so don't close smbcmd after this function)
   - else:
       - pneedmoremsg is set to true
       - buf_append_buf is used (so buffers are allocated)
*/
netwib_err netwox_smbcmdtscrstorage_append_smbcmd(netwox_constsmbcmd *psmbcmd,
                                          netwox_smbcmdtscrstorage *pstorage,
                                                  netwib_bool *pneedmoremsg);
/* Set offset to 0 before first call.
   This function only decodes setup/para/data using buf_init_ext (so
   don't close storage after this function). Other values must be set by user.
   Parameter maxsize indicates the maximum size which will be stored
   in psmbcmd (if storage is bigger separate in several chunks/psmbcmd).
*/
netwib_err netwox_smbcmdtscrstorage_decode_smbcmd(netwox_constsmbcmdtscrstorage *pstorage,
                                                  netwib_uint32 *poffset,
                                                  netwib_uint32 maxsize,
                                                  netwox_smbcmd *psmbcmd,
                                                  netwib_bool *pneedmoremsg);

/*-------------------------------------------------------------*/
#define NETWOX_SMBCMDTSCR_MAJORVERSION_NT350 3
#define NETWOX_SMBCMDTSCR_MAJORVERSION_NT351 3
#define NETWOX_SMBCMDTSCR_MAJORVERSION_95 4
#define NETWOX_SMBCMDTSCR_MAJORVERSION_98 4
#define NETWOX_SMBCMDTSCR_MAJORVERSION_ME 4
#define NETWOX_SMBCMDTSCR_MAJORVERSION_NT4 4
#define NETWOX_SMBCMDTSCR_MAJORVERSION_2000 5
#define NETWOX_SMBCMDTSCR_MAJORVERSION_XP 5
#define NETWOX_SMBCMDTSCR_MAJORVERSION_2003 5

#define NETWOX_SMBCMDTSCR_MINORVERSION_NT350 50
#define NETWOX_SMBCMDTSCR_MINORVERSION_NT351 51
#define NETWOX_SMBCMDTSCR_MINORVERSION_95 0
#define NETWOX_SMBCMDTSCR_MINORVERSION_98 10
#define NETWOX_SMBCMDTSCR_MINORVERSION_ME 90
#define NETWOX_SMBCMDTSCR_MINORVERSION_NT4 0
#define NETWOX_SMBCMDTSCR_MINORVERSION_2000 0
#define NETWOX_SMBCMDTSCR_MINORVERSION_XP 1
#define NETWOX_SMBCMDTSCR_MINORVERSION_2003 2

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SMBCMDTSCR_TYPE_UNKNOWN = 0,
  NETWOX_SMBCMDTSCR_TYPE_NETSHAREENUM,
  NETWOX_SMBCMDTSCR_TYPE_NETSHAREGETINFO,
  NETWOX_SMBCMDTSCR_TYPE_NETSERVERGETINFO,
  NETWOX_SMBCMDTSCR_TYPE_NETWKSTAGETINFO,
  NETWOX_SMBCMDTSCR_TYPE_NETSERVERENUM2
} netwox_smbcmdtscr_type;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdcmn_data name;
  netwox_smbcmdcmn_sharetype sharetype;
  netwib_uint16 unknown;
  netwox_smbcmdcmn_data comment;
} netwox_smbcmdtscr_netshareenum_share;
netwib_err netwox_smbcmdtscr_netshareenum_share_init(netwox_smbcmdtscr_netshareenum_share *pitem);
netwib_err netwox_smbcmdtscr_netshareenum_share_setdefault(netwox_smbcmdtscr_netshareenum_share *pitem);
netwib_err netwox_smbcmdtscr_netshareenum_share_close(netwox_smbcmdtscr_netshareenum_share *pitem);
typedef struct {
  netwib_uint16 status; /* 0 == success */
  netwib_uint16 totalshares; /* because they might not be all in this tsc */
  netwib_array shares;
} netwox_smbcmdtscr_netshareenum;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 status; /* 0 == success */
  netwox_smbcmdcmn_data name;
  netwox_smbcmdcmn_sharetype sharetype;
  netwib_uint16 unknown;
  netwox_smbcmdcmn_data comment;
} netwox_smbcmdtscr_netsharegetinfo;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 status; /* 0 == success */
  netwox_smbcmdcmn_data name;
  netwib_uint8 majorversion;
  netwib_uint8 minorversion;
  netwox_smbcmdcmn_servertype servertype;
  netwox_smbcmdcmn_data comment;
} netwox_smbcmdtscr_netservergetinfo;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 status; /* 0 == success */
  netwox_smbcmdcmn_data computername;
  netwox_smbcmdcmn_data username;
  netwox_smbcmdcmn_data domain;
  netwib_uint8 majorversion;
  netwib_uint8 minorversion;
  netwox_smbcmdcmn_data logondomain;
  netwox_smbcmdcmn_data otherdomains;
} netwox_smbcmdtscr_netwkstagetinfo;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdcmn_data name;
  netwib_uint8 majorversion;
  netwib_uint8 minorversion;
  netwox_smbcmdcmn_servertype servertype;
  netwox_smbcmdcmn_data comment;
} netwox_smbcmdtscr_netserverenum2_server;
netwib_err netwox_smbcmdtscr_netserverenum2_server_init(netwox_smbcmdtscr_netserverenum2_server *pitem);
netwib_err netwox_smbcmdtscr_netserverenum2_server_setdefault(netwox_smbcmdtscr_netserverenum2_server *pitem);
netwib_err netwox_smbcmdtscr_netserverenum2_server_close(netwox_smbcmdtscr_netserverenum2_server *pitem);
typedef struct {
  netwib_uint16 status; /* 0 == success */
  netwib_uint16 totalservers; /* because they might not be all in this tsc */
  netwib_array servers;
} netwox_smbcmdtscr_netserverenum2;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdtscr_type type;
  union {
    netwox_smbcmdtscr_netshareenum netshareenum;
    netwox_smbcmdtscr_netsharegetinfo netsharegetinfo;
    netwox_smbcmdtscr_netservergetinfo netservergetinfo;
    netwox_smbcmdtscr_netwkstagetinfo netwkstagetinfo;
    netwox_smbcmdtscr_netserverenum2 netserverenum2;
  } value;
} netwox_smbcmdtscr;
typedef const netwox_smbcmdtscr netwox_constsmbcmdtscr;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdtscr_type type;
  union {
    struct {
      netwox_smbcmdtsc_netshareenum_vfmt vfmt; /* not used for now, but for
                                                  future implementation */
      netwib_uint32 sharenamelen;
    } netshareenum;
    struct {
      netwox_smbcmdtsc_netsharegetinfo_vfmt vfmt; /* not used for now */
      netwib_uint32 sharenamelen;
    } netsharegetinfo;
    struct {
      netwox_smbcmdtsc_netservergetinfo_vfmt vfmt; /* not used for now */
      netwib_uint32 servernamelen;
    } netservergetinfo;
    struct {
      netwox_smbcmdtsc_netwkstagetinfo_vfmt vfmt; /* not used for now */
    } netwkstagetinfo;
    struct {
      netwox_smbcmdtsc_netserverenum2_vfmt vfmt; /* not used for now */
      netwib_uint32 servernamelen;
    } netserverenum2;
  } value;
} netwox_smbcmdtscrfmt;
typedef const netwox_smbcmdtscrfmt netwox_constsmbcmdtscrfmt;
netwib_err netwox_smbcmdtscrfmt_init_smbcmdtscq(netwox_constsmbcmdtscq *psmbcmdtscq,
                                                netwox_smbcmdtscrfmt *psmbcmdtscrfmt);

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscr_init(netwox_smbcmdtscr *psmbcmdtscr);
netwib_err netwox_smbcmdtscr_selecttype(netwox_smbcmdtscr *psmbcmdtscr,
                                        netwox_smbcmdtscr_type type);
netwib_err netwox_smbcmdtscr_setdefault(netwox_smbcmdtscr *psmbcmdtscr);
netwib_err netwox_smbcmdtscr_close(netwox_smbcmdtscr *psmbcmdtscr);

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscr_encode_storager(netwox_constsmbcmdtscrstorage *pstorage,
                                    netwox_constsmbcmdtscrfmt *psmbcmdtscrfmt,
                                    netwox_smbcmdtscr *psmbcmdtscr);
netwib_err netwox_smbcmdtscr_decode_storager(netwox_constsmbcmdtscr *psmbcmdtscr,
                                    netwox_constsmbcmdtscrfmt *psmbcmdtscrfmt,
                                    netwox_smbcmdtscrstorage *pstorage);

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscr_show(netwox_constsmbcmdtscr *psmbcmdtscr,
                                  netwib_buf *pbuf);
netwib_err netwox_smbcmdtscr_display(netwib_bufpool *pbufpool,
                                     netwox_constsmbcmdtscr *psmbcmdtscr);
