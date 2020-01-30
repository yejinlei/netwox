
/*-------------------------------------------------------------*/
/* this structure can be read by two threads */
typedef struct {
  /* can be changed by user */
  netwib_uint32 timeoutms;
  netwib_ips *pallowedclients;
  netwib_buf username;
  /* internally needed */
  netwib_bufpool *pbufpool;
} netwox_identserver;
typedef const netwox_identserver netwox_constidentserver;

netwib_err netwox_identserver_init(netwox_identserver *pidentserver);
netwib_err netwox_identserver_close(netwox_identserver *pidentserver);

/*-------------------------------------------------------------*/
netwib_err netwox_identserver_session(netwib_io *psockio,
                                      netwib_ptr pinfos);
