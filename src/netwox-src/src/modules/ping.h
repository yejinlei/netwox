
/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_PING_METHOD_PING = 1,
  NETWOX_PING_METHOD_PING_ETHIPSPOOF,
  NETWOX_PING_METHOD_TCP,
  NETWOX_PING_METHOD_TCP_ETHIPSPOOF,
  NETWOX_PING_METHOD_UDP,
  NETWOX_PING_METHOD_UDP_ETHIPSPOOF,
  NETWOX_PING_METHOD_ARP,
  NETWOX_PING_METHOD_ARP_ETHIPSPOOF,
  NETWOX_PING_METHOD_ICMP6ND,
  NETWOX_PING_METHOD_ICMP6ND_ETHIPSPOOF
} netwox_ping_method;

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
  netwib_uint32 maxcount; /* if 0, infinite loop */
  netwib_uint32 maxwaitms; /* 0=default */
  netwib_bool beep; /* beep for each reached */
  netwib_bool display01; /* in this case maxcount=1 */
} netwox_ping_addinfo;

/*-------------------------------------------------------------*/
netwib_err netwox_ping_addinfo_initdefault(netwox_ping_addinfo *paddinfo);

/*-------------------------------------------------------------*/
netwib_err netwox_ping_loop(netwox_ping_method method,
                            netwib_constip *pdstip,
                            netwox_ping_addinfo *paddinfo);


