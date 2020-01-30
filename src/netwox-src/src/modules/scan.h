
/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SCAN_METHOD_PING = 1,
  NETWOX_SCAN_METHOD_PING_ETHIPSPOOF,
  NETWOX_SCAN_METHOD_TCP,
  NETWOX_SCAN_METHOD_TCP_ETHIPSPOOF,
  NETWOX_SCAN_METHOD_UDP,
  NETWOX_SCAN_METHOD_UDP_ETHIPSPOOF,
  NETWOX_SCAN_METHOD_ARP,
  NETWOX_SCAN_METHOD_ARP_ETHIPSPOOF,
  NETWOX_SCAN_METHOD_ICMP6ND,
  NETWOX_SCAN_METHOD_ICMP6ND_ETHIPSPOOF
} netwox_scan_method;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_ips *pdstips;
  netwib_ports *pdstports; /* tcp and udp */
  netwib_spoof_ip_inittype spoofipinittype;
  netwib_buf device;
  netwib_bool srcethset;
  netwib_eth srceth;
  netwib_bool srcipset;
  netwib_ip srcip;
  netwib_uint32 mindelayms;
  netwib_uint32 maxwaitms;
  netwib_bool onlyuseful;
  netwib_uint32 numtargets;
} netwox_scan_info;

/*-------------------------------------------------------------*/
netwib_err netwox_scan_info_initdefault(netwox_scan_info *pinfo);

/*-------------------------------------------------------------*/
netwib_err netwox_scan_loop(netwox_scan_method method,
                            netwox_scan_info *pinfo);


