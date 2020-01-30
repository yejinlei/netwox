
/*-------------------------------------------------------------*/
/* NETWOX_SMBCMD_TYPE_SEARCH */

typedef struct {
  netwib_uint8 reserved; /* bit 0..4: set by server and echoed by client
                            bit 5..6: reserved for system
                            bit 7: set by client and echoed by server,
                                   but not done in captures
                         */
#define NETWOX_SMBCMDSEARCH_RESUMEKEY_COMPACTEDFILENAME_LEN 11
  netwox_smbcmdcmn_data compactedfilename; /* 8.3 file without the dot,
                                              but not done in captures */
  netwib_uint8 servercookie1; /* set by server and echoed by client
                                 (byte must be non zero when set) */
  netwib_uint32 servercookie2; /* set by server and echoed by client */
  netwib_uint32 clientcookie; /* set by client and echoed by server
                                 (first byte must be non zero when set),
                                 but not done in captures */
} netwox_smbcmdsearch_resumekey;
netwib_err netwox_smbcmdsearch_resumekey_init(netwox_smbcmdsearch_resumekey *presumekey);
netwib_err netwox_smbcmdsearch_resumekey_setdefault(netwox_smbcmdsearch_resumekey *presumekey);
netwib_err netwox_smbcmdsearch_resumekey_close(netwox_smbcmdsearch_resumekey *presumekey);
netwib_err netwox_smbcmdsearch_resumekey_decode(netwib_data *pdata,
                                                netwib_uint32 *pdatasize,
                                                netwox_smbcmdsearch_resumekey *presumekey);
#define NETWOX_SMBCMDSEARCH_RESUMEKEY_PKT_LEN 21
netwib_err netwox_smbcmdsearch_resumekey_append(const netwox_smbcmdsearch_resumekey *presumekey,
                                                netwib_buf *pbuf);
netwib_err netwox_smbcmdsearch_resumekey_show(const netwox_smbcmdsearch_resumekey *presumekey,
                                              netwib_buf *pbuf);
netwib_err netwox_smbcmdsearch_resumekey_display(netwib_bufpool *pbufpool,
                                                 const netwox_smbcmdsearch_resumekey *presumekey);


/*-------------------------------------------------------------*/
/* NETWOX_SMBCMD_TYPE_SEARCH_Q */

/* this just set/get resumekey field of smbcmd structure */
netwib_err netwox_smbcmdsearch_resumekey_encode_smbcmd(netwox_constsmbcmd *psmbcmd,
                                                       netwox_smbcmdsearch_resumekey *presumekey);
netwib_err netwox_smbcmdsearch_resumekey_decode_smbcmd(const netwox_smbcmdsearch_resumekey *presumekey,
                                                       netwox_smbcmd *psmbcmd);

/*-------------------------------------------------------------*/
/* NETWOX_SMBCMD_TYPE_SEARCH_R */

typedef struct {
  netwox_smbcmdsearch_resumekey resumekey;
  netwox_smbcmdcmn_fileattr16 fileattr;
  netwox_smbcmdcmn_timemsdos lastwritetime;
  netwib_uint32 filesize;
#define NETWOX_SMBCMDSEARCH_FILENAME_MAXLEN 12 /* 8 + '.' + 3 */
  netwox_smbcmdcmn_data filename;
} netwox_smbcmdsearch_entry;
netwib_err netwox_smbcmdsearch_entry_init(netwox_smbcmdsearch_entry *pentry);
netwib_err netwox_smbcmdsearch_entry_setdefault(netwox_smbcmdsearch_entry *pentry);
netwib_err netwox_smbcmdsearch_entry_close(netwox_smbcmdsearch_entry *pentry);
netwib_err netwox_smbcmdsearch_entry_decode(netwib_data *pdata,
                                            netwib_uint32 *pdatasize,
                                            netwox_smbcmdsearch_entry *pentry);
#define NETWOX_SMBCMDSEARCH_ENTRY_PKT_LEN 43
netwib_err netwox_smbcmdsearch_entry_append(const netwox_smbcmdsearch_entry *pentry,
                                            netwib_buf *pbuf);
netwib_err netwox_smbcmdsearch_entry_show(const netwox_smbcmdsearch_entry *pentry,
                                          netwib_buf *pbuf);

typedef struct {
  netwib_array entries; /* array of netwox_smbcmdsearch_entry */
} netwox_smbcmdsearch_entries;
netwib_err netwox_smbcmdsearch_entries_init(netwox_smbcmdsearch_entries *pentries);
netwib_err netwox_smbcmdsearch_entries_setdefault(netwox_smbcmdsearch_entries *pentries);
netwib_err netwox_smbcmdsearch_entries_close(netwox_smbcmdsearch_entries *pentries);
netwib_err netwox_smbcmdsearch_entries_show(const netwox_smbcmdsearch_entries *pentries,
                                            netwib_buf *pbuf);
netwib_err netwox_smbcmdsearch_entries_display(netwib_bufpool *pbufpool,
                                               const netwox_smbcmdsearch_entries *pentries);

/* this just set/get count/entries field of smbcmd structure */
netwib_err netwox_smbcmdsearch_entries_encode_smbcmd(netwox_constsmbcmd *psmbcmd,
                                                     netwox_smbcmdsearch_entries *pentries);
netwib_err netwox_smbcmdsearch_entries_decode_smbcmd(const netwox_smbcmdsearch_entries *pentries,
                                                     netwox_smbcmd *psmbcmd);
