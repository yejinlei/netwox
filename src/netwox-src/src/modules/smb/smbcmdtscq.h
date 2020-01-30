
/*-------------------------------------------------------------*/
/* NETWOX_SMBCMD_TYPE_TRANSACTION_Q */

/* Note: in this module there is a lot of redundant code, because
         each function can be slightly different
*/

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SMBCMDTSCQ_TYPE_UNKNOWN = 0,
  NETWOX_SMBCMDTSCQ_TYPE_NETSHAREENUM,
  NETWOX_SMBCMDTSCQ_TYPE_NETSHAREGETINFO,
  NETWOX_SMBCMDTSCQ_TYPE_NETSERVERGETINFO,
  NETWOX_SMBCMDTSCQ_TYPE_NETWKSTAGETINFO,
  NETWOX_SMBCMDTSCQ_TYPE_NETSERVERENUM2
} netwox_smbcmdtscq_type;

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SMBCMDTSC_NETSHAREENUM_VFMT_1 = 100 /* not a real value */
} netwox_smbcmdtsc_netshareenum_vfmt;
typedef struct {
  netwox_smbcmdtsc_netshareenum_vfmt vfmt; /* virtual format of replies
                                              is based on 2 real values
                                              from packet
                                               - return descriptor
                                               - real fmt
                                           */
  netwib_uint16 receivebufferlen;
} netwox_smbcmdtscq_netshareenum;

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SMBCMDTSC_NETSHAREGETINFO_VFMT_1 = 100 /* not a real value */
} netwox_smbcmdtsc_netsharegetinfo_vfmt;
typedef struct {
  netwox_smbcmdtsc_netsharegetinfo_vfmt vfmt; /* virtual format */
  netwox_smbcmdcmn_data share;
  netwib_uint16 receivebufferlen;
} netwox_smbcmdtscq_netsharegetinfo;

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SMBCMDTSC_NETSERVERGETINFO_VFMT_1 = 100 /* not a real value */
} netwox_smbcmdtsc_netservergetinfo_vfmt;
typedef struct {
  netwox_smbcmdtsc_netservergetinfo_vfmt vfmt; /* virtual format */
  netwib_uint16 receivebufferlen;
} netwox_smbcmdtscq_netservergetinfo;

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SMBCMDTSC_NETWKSTAGETINFO_VFMT_1 = 100 /* not a real value */
} netwox_smbcmdtsc_netwkstagetinfo_vfmt;
typedef struct {
  netwox_smbcmdtsc_netwkstagetinfo_vfmt vfmt; /* virtual format */
  netwib_uint16 receivebufferlen;
} netwox_smbcmdtscq_netwkstagetinfo;

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SMBCMDTSC_NETSERVERENUM2_VFMT_1 = 100 /* not a real value */
} netwox_smbcmdtsc_netserverenum2_vfmt;
typedef struct {
  netwox_smbcmdtsc_netserverenum2_vfmt vfmt; /* virtual format */
  netwib_uint16 receivebufferlen;
  netwox_smbcmdcmn_servertype servertype;
} netwox_smbcmdtscq_netserverenum2;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbcmdtscq_type type;
  union {
    netwox_smbcmdtscq_netshareenum netshareenum;
    netwox_smbcmdtscq_netsharegetinfo netsharegetinfo;
    netwox_smbcmdtscq_netservergetinfo netservergetinfo;
    netwox_smbcmdtscq_netwkstagetinfo netwkstagetinfo;
    netwox_smbcmdtscq_netserverenum2 netserverenum2;
  } value;
} netwox_smbcmdtscq;
typedef const netwox_smbcmdtscq netwox_constsmbcmdtscq;

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscq_init(netwox_smbcmdtscq *psmbcmdtscq);
netwib_err netwox_smbcmdtscq_selecttype(netwox_smbcmdtscq *psmbcmdtscq,
                                        netwox_smbcmdtscq_type type);
netwib_err netwox_smbcmdtscq_setdefault(netwox_smbcmdtscq *psmbcmdtscq);
netwib_err netwox_smbcmdtscq_close(netwox_smbcmdtscq *psmbcmdtscq);

/*-------------------------------------------------------------*/
/* for those 2 functions :
    - don't forget to set isunicode before
    - they suppose query is stored only in first message (most
      of the time, query is small, so this is ok)
*/
netwib_err netwox_smbcmdtscq_encode_smbcmd(netwox_constsmbcmd *psmbcmd,
                                           netwox_smbcmdtscq *psmbcmdtscq);
/* only decode setup/para/data. Other values have to be set by user */
netwib_err netwox_smbcmdtscq_decode_smbcmd(netwox_constsmbcmdtscq *psmbcmdtscq,
                                           netwox_smbcmd *psmbcmd);

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscq_show(netwox_constsmbcmdtscq *psmbcmdtscq,
                                  netwib_buf *pbuf);
netwib_err netwox_smbcmdtscq_display(netwib_bufpool *pbufpool,
                                     netwox_constsmbcmdtscq *psmbcmdtscq);
