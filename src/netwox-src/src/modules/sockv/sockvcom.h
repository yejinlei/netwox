
/*-------------------------------------------------------------*/
/* Functions below are used in sockvudp and sockvtcp */

/*-------------------------------------------------------------*/
typedef struct {
  /* common */
  netwox_socktype socktype;
  netwox_sockfamily sockfamily;
  netwib_buf device;
  netwib_eth localeth;
  netwib_ip localip;
  netwib_port localport;
  netwib_buf ip4opts;
  netwib_ipproto ip6extsproto;
  netwib_buf ip6exts;
  netwib_bool answeralive;
  /* for clients, or for server (set after first client connected) */
  netwib_eth remoteeth;
  netwib_ip remoteip;
  netwib_port remoteport;
  /* rd : for servers (info about whom is the client)
     rd : for tcp return_code_of_handshake is first returned
     wr : for udp mulser, info about current client
   */
  netwib_bool infoinitialized;
  netwib_buf infobufrd;
  netwib_buf infobufwr;
  netwib_thread_rwlock *pinfolockrd;
  netwib_thread_rwlock *pinfolockwr;
  /* for udp server/mulser */
  netwib_bool clientconnected;
  /* system thread */
  netwib_thread *pthread;
  /* tlv (data from user to system, and from system to user) */
  netwib_buf tlvbufrd;
  netwib_buf tlvbufwr;
  netwib_thread_rwlock *ptlvlockrd;
  netwib_thread_rwlock *ptlvlockwr;
  netwib_io *ptlvusrio;
  netwib_io *ptlvsysio;
  /* to indicate premature end */
  netwib_thread_cond *pendcond;
} netwox_priv_io_sockv;

/*-------------------------------------------------------------*/
netwib_err netwox_sockv_info_write_err(netwox_priv_io_sockv *pisv,
                                       netwib_err err);
netwib_err netwox_sockv_info_read_err_avail(netwox_priv_io_sockv *pisv,
                                            netwib_bool *pevent);
netwib_err netwox_sockv_info_read_err(netwox_priv_io_sockv *pisv,
                                      netwib_err *perr);
netwib_err netwox_sockv_info_write_cli(netwox_priv_io_sockv *pisv,
                                       netwib_bool fromchildtofather,
                                       netwib_consteth *pclieth,
                                       netwib_constip *pcliip,
                                       netwib_port cliport);
netwib_err netwox_sockv_info_read_cli(netwox_priv_io_sockv *pisv,
                                      netwib_bool fromchildtofather,
                                      netwib_eth *pclieth,
                                      netwib_ip *pcliip,
                                      netwib_port *pcliport);

