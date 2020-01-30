
/*-------------------------------------------------------------*/
/* this structure can be read by two threads */
typedef struct {
  /* can be changed by user */
  netwib_uint32 timeoutms;
  netwib_ips *pallowedclients;
  netwib_buf login;
  netwib_buf password;
  netwib_buf root;
  netwox_telnet_negomode negomode;
  /* internally needed */
  netwib_bufpool *pbufpool;
} netwox_telnetserver;
typedef const netwox_telnetserver netwox_consttelnetserver;

netwib_err netwox_telnetserver_init(netwox_telnetserver *ptelnetserver);
netwib_err netwox_telnetserver_close(netwox_telnetserver *ptelnetserver);

/*-------------------------------------------------------------*/
netwib_err netwox_telnetserver_session(netwib_io *psockio,
                                       netwib_ptr pinfos);
