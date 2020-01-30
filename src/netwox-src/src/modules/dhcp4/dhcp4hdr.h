
/*-------------------------------------------------------------*/
/* rfc 2131 and 2132 : DHCPv4
   rfc 3315 describes DHCPv6 which is very different from DHCPv4,
   so it is not implemented.
 */

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_DHCP4OPCODE_UNKNOWN = 0,
  NETWOX_DHCP4OPCODE_BOOTREQUEST = 1,
  NETWOX_DHCP4OPCODE_BOOTREPLY = 2
} netwox_dhcp4opcode;

/*-------------------------------------------------------------*/
#define NETWOX_DHCP4HDR_FLAGS_BROADCAST 0x8000

/*-------------------------------------------------------------*/
/* Note: only support Ethernet */
typedef struct {
  netwox_dhcp4opcode dhcp4opcode;
  netwib_uint8 hops;
  netwib_uint32 xid;
  netwib_uint16 secs;
  netwib_uint16 flags;
  netwib_ip clientip;
  netwib_ip yourip;
  netwib_ip serverip;
  netwib_ip agentip;
  netwib_eth clienteth;
  netwib_bufext sname;
  netwib_bufext file;
  netwib_bufext opts; /* array of dhcp4opt (magic cookie of
                         RFC 1497 is not included) */
} netwox_dhcp4hdr;
typedef const netwox_dhcp4hdr netwox_constdhcp4hdr;

/*-------------------------------------------------------------*/
netwib_err netwox_dhcp4hdr_initdefault(netwox_dhcp4hdr *pdhcp4hdr);

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_dhcp4hdr(netwox_constdhcp4hdr *pdhcp4hdr,
                                      netwib_buf *ppkt);
netwib_err netwox_pkt_decode_dhcp4hdr(netwib_constbuf *ppkt,
                                      netwox_dhcp4hdr *pdhcp4hdr);

/*-------------------------------------------------------------*/
netwib_err netwox_dhcp4hdr_show(netwox_constdhcp4hdr *pdhcp4hdr,
                                netwib_encodetype encodetype,
                                netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_dhcp4hdr_test(void);
