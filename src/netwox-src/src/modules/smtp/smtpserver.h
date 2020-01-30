
/*-------------------------------------------------------------*/
/* this structure can be read by two threads */
typedef struct {
  /* can be changed by user */
  netwib_uint32 timeoutms;
  netwib_ips *pallowedclients;
  netwib_buf maildir;
  /* internally needed */
  netwib_bufpool *pbufpool;
  netwib_uint32 mailnum;
  netwib_thread_mutex *pmailnummutex;
} netwox_smtpserver;
typedef const netwox_smtpserver netwox_constsmtpserver;

netwib_err netwox_smtpserver_init(netwox_smtpserver *psmtpserver);
netwib_err netwox_smtpserver_close(netwox_smtpserver *psmtpserver);

/*-------------------------------------------------------------*/
netwib_err netwox_smtpserver_session(netwib_io *psockio,
                                     netwib_ptr pinfos);
