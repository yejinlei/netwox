
/*-------------------------------------------------------------*/
/***************************************************************
 * Refer to RFC 2461 to obtain more information about ICMPv6   *
 * Neighbor Discovery options.                                 *
 ***************************************************************/

/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_ICMP6NDTYPE_SRCLINK = 1, /* src link layer address */
  NETWIB_ICMP6NDTYPE_DSTLINK = 2, /* dst link layer address */
  NETWIB_ICMP6NDTYPE_PREFIX = 3,  /* prefix information */
  NETWIB_ICMP6NDTYPE_REDIR = 4,   /* redirected */
  NETWIB_ICMP6NDTYPE_MTU = 5      /* mtu */
} netwib_icmp6ndtype;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_eth linkad;
} netwib_icmp6nd_link;
typedef const netwib_icmp6nd_link netwib_consticmp6nd_link;

typedef struct {
  netwib_uint8 prefixlength;
  netwib_bool onlink;
  netwib_bool autonomous;
  netwib_uint8 reserved1;
  netwib_uint32 validlifetime;
  netwib_uint32 preferredlifetime;
  netwib_uint32 reserved2;
  netwib_ip prefix;
} netwib_icmp6nd_prefix;
typedef const netwib_icmp6nd_prefix netwib_consticmp6nd_prefix;

typedef struct {
  netwib_uint16 reserved1;
  netwib_uint32 reserved2;
  netwib_bufext badippacket;
} netwib_icmp6nd_redir;
typedef const netwib_icmp6nd_redir netwib_consticmp6nd_redir;

typedef struct {
  netwib_uint16 reserved;
  netwib_uint32 mtu;
} netwib_icmp6nd_mtu;
typedef const netwib_icmp6nd_mtu netwib_consticmp6nd_mtu;

typedef struct {
  netwib_icmp6ndtype type;
  union {
    netwib_icmp6nd_link link;
    netwib_icmp6nd_prefix prefix;
    netwib_icmp6nd_redir redir;
    netwib_icmp6nd_mtu mtu;
  } opt;
} netwib_icmp6nd;
typedef const netwib_icmp6nd netwib_consticmp6nd;

/*-------------------------------------------------------------*/
/* Name : netwib_icmp6nd_initdefault
   Description :
     Initialize a netwib_icmp6nd with default values.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     *picmp6nd : netwib_icmp6nd to initialize
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_icmp6nd_initdefault(netwib_icmp6ndtype icmp6ndtype,
                                      netwib_icmp6nd *picmp6nd);

/*-------------------------------------------------------------*/
/* Name : netwib_pkt_append_icmp6nd
   Description :
     Initialize physical data from a netwib_icmp6nd.
   Input parameter(s) :
     icmp6nd : netwib_icmp6nd
   Input/output parameter(s) :
     *ppht : buffer to initialize
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_pkt_append_icmp6nd(netwib_consticmp6nd *picmp6nd,
                                     netwib_buf *ppkt);

/*-------------------------------------------------------------*/
/* Name : netwib_pkt_decode_icmp6nd
   Description :
     Decode a netwib_icmp6nd from data coming from wire.
   Input parameter(s) :
     *ppkt : buffer containing physical data
   Input/output parameter(s) :
   Output parameter(s) :
     *picmp6nd : netwib_icmp6nd to initialize
     *pskipsize : size to skip this option
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_pkt_decode_icmp6nd(netwib_constbuf *ppkt,
                                     netwib_icmp6nd *picmp6nd,
                                     netwib_uint32 *pskipsize);
