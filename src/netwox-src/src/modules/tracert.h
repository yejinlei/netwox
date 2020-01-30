
/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_TRACERT_METHOD_PING = 1,
  NETWOX_TRACERT_METHOD_PING_ETHIPSPOOF,
  NETWOX_TRACERT_METHOD_TCP,
  NETWOX_TRACERT_METHOD_TCP_ETHIPSPOOF,
  NETWOX_TRACERT_METHOD_UDP,
  NETWOX_TRACERT_METHOD_UDP_ETHIPSPOOF,
  NETWOX_TRACERT_METHOD_PROTO,
  NETWOX_TRACERT_METHOD_PROTO_ETHIPSPOOF
} netwox_tracert_method;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_spoof_ip_inittype spoofipinittype;
  netwib_buf device;
  netwib_bool srcethset;
  netwib_eth srceth;
  netwib_bool dstethset;
  netwib_eth dsteth;
  netwib_bool srcipset;
  netwib_ip srcip;
  netwib_port dstport; /* tcp and udp */
  netwib_ipproto ipproto;
  netwib_buf ipdata;
  netwib_uint8 minttl;
  netwib_uint8 maxttl;
  netwib_uint32 maxwaitms; /* 0=default */
  netwib_bool resolve;
} netwox_tracert_addinfo;

/*-------------------------------------------------------------*/
netwib_err netwox_tracert_addinfo_initdefault(netwox_tracert_addinfo *paddinfo);

/*-------------------------------------------------------------*/
netwib_err netwox_tracert_loop(netwox_tracert_method method,
                               netwib_constip *pdstip,
                               netwox_tracert_addinfo *paddinfo,
                               netwib_ring *presult);


