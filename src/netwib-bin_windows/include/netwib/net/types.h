
#ifndef NETWIB_NET_TYPES_INCLUDED
#define NETWIB_NET_TYPES_INCLUDED 1

/*-------------------------------------------------------------*/
/* to store an Ethernet address */
#define NETWIB_ETH_LEN 6
typedef struct {
  netwib_byte b[NETWIB_ETH_LEN];
} netwib_eth;
typedef const netwib_eth netwib_consteth;

/*-------------------------------------------------------------*/
/* to store an IPv4 address */
#define NETWIB_IP4_LEN 4
typedef netwib_uint32 netwib_ip4;

/* to store an IPv6 address */
#define NETWIB_IP6_LEN 16
typedef struct {
  netwib_byte b[NETWIB_IP6_LEN];
} netwib_ip6;
typedef const netwib_ip6 netwib_constip6;

/* to store an IP address */
typedef enum {
  NETWIB_IPTYPE_UNKNOWN = 0,
  NETWIB_IPTYPE_IP4,
  NETWIB_IPTYPE_IP6
} netwib_iptype;
typedef struct {
  netwib_iptype iptype;
  union {
    netwib_ip4 ip4;
    netwib_ip6 ip6;
  } ipvalue;
} netwib_ip;
typedef const netwib_ip netwib_constip;

/*-------------------------------------------------------------*/
/* to store a port */
typedef netwib_uint32 netwib_port;

#endif
