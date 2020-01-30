
/*-------------------------------------------------------------*/
/* NETWOX_SMBCMD_TYPE_NTTRANSACT_R */

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bool isunicode;
  netwib_uint8 setupcount;
  netwib_uint16 setup[NETWOX_SMBCMD_NTTRANSACT_SETUP_MAX];
  netwib_buf para;
  netwib_buf data;
} netwox_smbcmdtscntrstorage;
typedef const netwox_smbcmdtscntrstorage netwox_constsmbcmdtscntrstorage;
/* buffers have CANALLOC flag */
netwib_err netwox_smbcmdtscntrstorage_init(netwib_bool isunicode,
                                        netwox_smbcmdtscntrstorage *pstorage);
/* buffers are closed if they were allocated */
netwib_err netwox_smbcmdtscntrstorage_close(netwox_smbcmdtscntrstorage *pstorage);
/*
   - if smbcmd contains all the data:
       - pneedmoremsg is set to false
       - buf_init_ext_buf is used (so don't close smbcmd after this function)
   - else:
       - pneedmoremsg is set to true
       - buf_append_buf is used (so buffers are allocated)
*/
netwib_err netwox_smbcmdtscntrstorage_append_smbcmd(netwox_constsmbcmd *psmbcmd,
                                          netwox_smbcmdtscntrstorage *pstorage,
                                                    netwib_bool *pneedmoremsg);
/* Set offset to 0 before first call.
   This function only decodes setup/para/data using buf_init_ext (so
   don't close storage after this function). Other values must be set by user.
*/
netwib_err netwox_smbcmdtscntrstorage_decode_smbcmd(netwox_constsmbcmdtscntrstorage *pstorage,
                                                    netwib_uint32 *poffset,
                                                    netwib_uint32 maxsize,
                                                    netwox_smbcmd *psmbcmd,
                                                    netwib_bool *pneedmoremsg);

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SMBCMDTSCNTR_TYPE_UNKNOWN = 0,
  NETWOX_SMBCMDTSCNTR_TYPE_NTIOCTL,
  NETWOX_SMBCMDTSCNTR_TYPE_NTNOTIFY,
  NETWOX_SMBCMDTSCNTR_TYPE_NTQUERYSECUDESC,
  NETWOX_SMBCMDTSCNTR_TYPE_NTSETSECUDESC
} netwox_smbcmdtscntr_type;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdcmn_data data;
} netwox_smbcmdtscntr_ntioctl;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 action;
#define NETWOX_SMBCMDTSCNTR_NTNOTIFY_ACTION_ADDED 1
#define NETWOX_SMBCMDTSCNTR_NTNOTIFY_ACTION_REMOVED 2
#define NETWOX_SMBCMDTSCNTR_NTNOTIFY_ACTION_MODIFIED 3
#define NETWOX_SMBCMDTSCNTR_NTNOTIFY_ACTION_RENAMEDOLDNAME 4
#define NETWOX_SMBCMDTSCNTR_NTNOTIFY_ACTION_RENAMEDNEWNAME 5
#define NETWOX_SMBCMDTSCNTR_NTNOTIFY_ACTION_STREAMADDED 6
#define NETWOX_SMBCMDTSCNTR_NTNOTIFY_ACTION_STREAMREMOVED 7
#define NETWOX_SMBCMDTSCNTR_NTNOTIFY_ACTION_STREAMMODIFIED 8
  netwox_smbcmdcmn_data filename;
} netwox_smbcmdtscntr_ntnotify_item;
netwib_err netwox_smbcmdtscntr_ntnotify_item_init(netwox_smbcmdtscntr_ntnotify_item *pitem);
netwib_err netwox_smbcmdtscntr_ntnotify_item_setdefault(netwox_smbcmdtscntr_ntnotify_item *pitem);
netwib_err netwox_smbcmdtscntr_ntnotify_item_close(netwox_smbcmdtscntr_ntnotify_item *pitem);
typedef struct {
  netwib_array infos; /* array of netwox_smbcmdtscntr_ntnotify_item */
} netwox_smbcmdtscntr_ntnotify;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdcmn_data secudesc; /* too complex to decode for now */
} netwox_smbcmdtscntr_ntquerysecudesc;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdtscntr_type type;
  union {
    netwox_smbcmdtscntr_ntioctl ntioctl;
    netwox_smbcmdtscntr_ntnotify ntnotify;
    netwox_smbcmdtscntr_ntquerysecudesc ntquerysecudesc;
    /* ntsetsecudesc not needed */
  } value;
} netwox_smbcmdtscntr;
typedef const netwox_smbcmdtscntr netwox_constsmbcmdtscntr;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdtscntr_type type;
} netwox_smbcmdtscntrfmt;
typedef const netwox_smbcmdtscntrfmt netwox_constsmbcmdtscntrfmt;
netwib_err netwox_smbcmdtscntrfmt_init_smbcmdtscntq(netwox_constsmbcmdtscntq *psmbcmdtscntq,
                                                  netwox_smbcmdtscntrfmt *psmbcmdtscntrfmt);

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntr_init(netwox_smbcmdtscntr *psmbcmdtscntr);
netwib_err netwox_smbcmdtscntr_selecttype(netwox_smbcmdtscntr *psmbcmdtscntr,
                                          netwox_smbcmdtscntr_type type);
netwib_err netwox_smbcmdtscntr_setdefault(netwox_smbcmdtscntr *psmbcmdtscntr);
netwib_err netwox_smbcmdtscntr_close(netwox_smbcmdtscntr *psmbcmdtscntr);

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntr_encode_storager(netwox_constsmbcmdtscntrstorage *pstorage,
                                netwox_constsmbcmdtscntrfmt *psmbcmdtscntrfmt,
                                netwox_smbcmdtscntr *psmbcmdtscntr);
netwib_err netwox_smbcmdtscntr_decode_storager(netwox_constsmbcmdtscntr *psmbcmdtscntr,
                                netwox_constsmbcmdtscntrfmt *psmbcmdtscntrfmt,
                                netwox_smbcmdtscntrstorage *pstorage);

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntr_show(netwox_constsmbcmdtscntr *psmbcmdtscntr,
                                    netwib_buf *pbuf);
netwib_err netwox_smbcmdtscntr_display(netwib_bufpool *pbufpool,
                                      netwox_constsmbcmdtscntr *psmbcmdtscntr);

