
/*-------------------------------------------------------------*/
/***************************************************************
 * Refer to RFC 2463 to obtain more information about ICMPv6.  *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* ICMP6 types. Comments starting with "NI" are not implemented */
typedef enum {
  NETWIB_ICMP6TYPE_DSTUNREACH = 1,    /* destination unreachable */
  NETWIB_ICMP6TYPE_PKTTOOBIG = 2,     /* packet too big */
  NETWIB_ICMP6TYPE_TIMEEXCEED = 3,    /* time exceeded */
  NETWIB_ICMP6TYPE_PARAPROB = 4,      /* parameter problem */
  NETWIB_ICMP6TYPE_ECHOREQ = 128,     /* echo request */
  NETWIB_ICMP6TYPE_ECHOREP = 129,     /* echo reply */
  NETWIB_ICMP6TYPE_ROUTERSOLICIT = 133, /* router solicitation (rfc2461) */
  NETWIB_ICMP6TYPE_ROUTERADVERT = 134,    /* router advert. (rfc2461) */
  NETWIB_ICMP6TYPE_NEIGHBORSOLICIT = 135, /* neighbor soli. (rfc2461) */
  NETWIB_ICMP6TYPE_NEIGHBORADVERT = 136,  /* neighbor advert. (rfc2461) */
  NETWIB_ICMP6TYPE_REDIRECT = 137         /* redirect (rfc2461) */
} netwib_icmp6type;

/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_ICMP6CODE_DSTUNREACH_NOROUTE = 0,
  NETWIB_ICMP6CODE_DSTUNREACH_PROHI = 1,
  NETWIB_ICMP6CODE_DSTUNREACH_HOST = 3,
  NETWIB_ICMP6CODE_DSTUNREACH_PORT = 4,
  NETWIB_ICMP6CODE_PKTTOOBIG = 0,
  NETWIB_ICMP6CODE_TIMEEXCEED_TTL = 0,
  NETWIB_ICMP6CODE_TIMEEXCEED_FRAG = 1,
  NETWIB_ICMP6CODE_PARAPROB_HDR = 0,
  NETWIB_ICMP6CODE_PARAPROB_NEXTHDR = 1,
  NETWIB_ICMP6CODE_PARAPROB_OPTION = 2,
  NETWIB_ICMP6CODE_ECHOREQ = 0,
  NETWIB_ICMP6CODE_ECHOREP = 0,
  NETWIB_ICMP6CODE_ROUTERSOLICIT = 0,
  NETWIB_ICMP6CODE_ROUTERADVERT = 0,
  NETWIB_ICMP6CODE_NEIGHBORSOLICIT = 0,
  NETWIB_ICMP6CODE_NEIGHBORADVERT = 0,
  NETWIB_ICMP6CODE_REDIRECT = 0
} netwib_icmp6code;

/*-------------------------------------------------------------*/
/* Name : netwib_buf_append_icmp6type
   Description :
     Print the text corresponding to icmp type.
   Input parameter(s) :
     type : icmp6 type
   Input/output parameter(s) :
     *pbuf : netwib_buf receiving data
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_append_icmp6type(netwib_icmp6type type,
                                       netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* Name : netwib_buf_append_icmp6code
   Description :
     Print the text corresponding to icmp code.
   Input parameter(s) :
     type : icmp6 type
     code : icmp6 code
   Input/output parameter(s) :
     *pbuf : netwib_buf receiving data
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_append_icmp6code(netwib_icmp6type type,
                                       netwib_icmp6code code,
                                       netwib_buf *pbuf);

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 reserved;
  netwib_bufext badippacket;
} netwib_icmp6_dstunreach;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 mtu;
  netwib_bufext badippacket;
} netwib_icmp6_pkttoobig;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 reserved;
  netwib_bufext badippacket;
} netwib_icmp6_timeexceed;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 pointer;
  netwib_bufext badippacket;
} netwib_icmp6_paraprob;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 id;
  netwib_uint16 seqnum;
  netwib_bufext data;
} netwib_icmp6_echo;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 reserved;
  netwib_bufext options; /* buffer contains netwib_icmp6nd */
} netwib_icmp6_routersolicit;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint8 curhoplimit;
  netwib_bool managedaddress;
  netwib_bool otherstateful;
  netwib_uint8 reserved;
  netwib_uint16 routerlifetime;
  netwib_uint32 reachabletime;
  netwib_uint32 retranstimer;
  netwib_bufext options; /* buffer contains netwib_icmp6nd */
} netwib_icmp6_routeradvert;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 reserved;
  netwib_ip target;
  netwib_bufext options; /* buffer contains netwib_icmp6nd */
} netwib_icmp6_neighborsolicit;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bool router;
  netwib_bool solicited;
  netwib_bool override;
  netwib_uint32 reserved;
  netwib_ip target;
  netwib_bufext options; /* buffer contains netwib_icmp6nd */
} netwib_icmp6_neighboradvert;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 reserved;
  netwib_ip target;
  netwib_ip dst;
  netwib_bufext options; /* buffer contains netwib_icmp6nd */
} netwib_icmp6_redirect;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_icmp6type type;
  netwib_icmp6code code;
  netwib_uint16 check;
  union {
    netwib_icmp6_dstunreach dstunreach;
    netwib_icmp6_pkttoobig pkttoobig;
    netwib_icmp6_timeexceed timeexceed;
    netwib_icmp6_paraprob paraprob;
    netwib_icmp6_echo echo; /* used for request and reply */
    netwib_icmp6_routersolicit routersolicit;
    netwib_icmp6_routeradvert routeradvert;
    netwib_icmp6_neighborsolicit neighborsolicit;
    netwib_icmp6_neighboradvert neighboradvert;
    netwib_icmp6_redirect redirect;
  } msg;
} netwib_icmp6;
typedef const netwib_icmp6 netwib_consticmp6;
#define NETWIB_ICMP6_MINLEN 4

/*-------------------------------------------------------------*/
/* Name : netwib_icmp6_initdefault
   Description :
     Initialize a netwib_icmp6 with default values.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     *picmp6hdr : netwib_icmp6hdr to initialize
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_icmp6_initdefault(netwib_icmp6type type,
                                    netwib_icmp6 *picmp6);

/*-------------------------------------------------------------*/
/* Name : netwib_pkt_append_icmp6
   Description :
     Initialize physical data from a netwib_icmp6.
   Input parameter(s) :
     icmp6 : netwib_icmp6
   Input/output parameter(s) :
     *ppht : buffer to initialize
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_pkt_append_icmp6(netwib_consticmp6 *picmp6,
                                   netwib_buf *ppkt);

/*-------------------------------------------------------------*/
/* Name : netwib_pkt_decode_icmp6
   Description :
     Decode a netwib_icmp6 from data coming from wire.
   Input parameter(s) :
     pktproto : protocol of the netwib_icmp6 in ppkt
     *ppkt : buffer containing physical data
   Input/output parameter(s) :
   Output parameter(s) :
     *picmp6 : netwib_icmp6 to initialize
     *pskipsize : size to skip this header
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_pkt_decode_icmp6(netwib_constbuf *ppkt,
                                   netwib_icmp6 *picmp6,
                                   netwib_uint32 *pskipsize);


