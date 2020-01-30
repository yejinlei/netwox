
/*-------------------------------------------------------------*/
/* this structure can be read by two threads */
typedef struct {
  /* can be changed by user */
  netwib_uint32 timeoutms;
  netwib_ips *pallowedclients;
  netwib_buf login;
  netwib_buf password;
  netwib_buf rootdir;
  netwib_bool allowput;
  /* internally needed */
  netwib_bufpool *pbufpool;
  netwox_sockinfo *pmainsockinfo;
} netwox_ftpserver;
typedef const netwox_ftpserver netwox_constftpserver;

netwib_err netwox_ftpserver_init(netwox_ftpserver *pftpserver);
netwib_err netwox_ftpserver_close(netwox_ftpserver *pftpserver);

/*-------------------------------------------------------------*/
netwib_err netwox_ftpserver_session(netwib_io *psockio,
                                    netwib_ptr pinfos);
