
/*-------------------------------------------------------------*/
/***************************************************************
 * Refer to RFC 792 to obtain more information about ICMPv4.   *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* ICMP4 types. Comments starting with "NI" are not implemented */
typedef enum {
  NETWIB_ICMP4TYPE_ECHOREP = 0,       /* echo reply */
  NETWIB_ICMP4TYPE_DSTUNREACH = 3,    /* destination unreachable */
  NETWIB_ICMP4TYPE_SRCQUENCH = 4,     /* source quench */
  NETWIB_ICMP4TYPE_REDIRECT = 5,      /* redirect */
  NETWIB_ICMP4TYPE_ALTHOSTAD = 6,     /* NI www.iana.org */
  NETWIB_ICMP4TYPE_ECHOREQ = 8,       /* echo request */
  NETWIB_ICMP4TYPE_ROUTERADVERT = 9,  /* NI rfc 1256 */
  NETWIB_ICMP4TYPE_ROUTERSELECT = 10, /* NI rfc 1256 */
  NETWIB_ICMP4TYPE_TIMEEXCEED = 11,   /* time exceeded */
  NETWIB_ICMP4TYPE_PARAPROB = 12,     /* parameter problem */
  NETWIB_ICMP4TYPE_TIMESTAMPREQ = 13, /* timestamp request */
  NETWIB_ICMP4TYPE_TIMESTAMPREP = 14, /* timestamp reply */
  NETWIB_ICMP4TYPE_INFOREQ = 15,      /* information request */
  NETWIB_ICMP4TYPE_INFOREP = 16,      /* information reply */
  NETWIB_ICMP4TYPE_ADMASKREQ = 17,    /* NI rfc 950 */
  NETWIB_ICMP4TYPE_ADMASKREP = 18,    /* NI rfc 950 */
  NETWIB_ICMP4TYPE_TRACEROUTE = 30,   /* NI rfc 1393 */
  NETWIB_ICMP4TYPE_DATACONVERR = 31,  /* NI rfc 1474 */
  NETWIB_ICMP4TYPE_MOBHOSTREDIR = 32, /* NI www.iana.org */
  NETWIB_ICMP4TYPE_IPV6WHEREYOU = 33, /* NI www.iana.org */
  NETWIB_ICMP4TYPE_IPV6IAMHERE = 34,  /* NI www.iana.org */
  NETWIB_ICMP4TYPE_MOBREGREQ = 35,    /* NI www.iana.org */
  NETWIB_ICMP4TYPE_MOBREGREP = 36,    /* NI www.iana.org */
  NETWIB_ICMP4TYPE_SKIP = 39,         /* NI www.iana.org */
  NETWIB_ICMP4TYPE_PHOTURIS = 40      /* NI www.iana.org */
} netwib_icmp4type;

/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_ICMP4CODE_ECHOREP = 0,
  NETWIB_ICMP4CODE_DSTUNREACH_NET = 0,
  NETWIB_ICMP4CODE_DSTUNREACH_HOST = 1,
  NETWIB_ICMP4CODE_DSTUNREACH_PROTO = 2,
  NETWIB_ICMP4CODE_DSTUNREACH_PORT = 3,
  NETWIB_ICMP4CODE_DSTUNREACH_FRAG = 4,
  NETWIB_ICMP4CODE_DSTUNREACH_SR = 5,
  NETWIB_ICMP4CODE_DSTUNREACH_NETUNKNOWN = 6,
  NETWIB_ICMP4CODE_DSTUNREACH_HOSTUNKNOWN = 7,
  NETWIB_ICMP4CODE_DSTUNREACH_HOSTISOL = 8,
  NETWIB_ICMP4CODE_DSTUNREACH_NETPROHI = 9,
  NETWIB_ICMP4CODE_DSTUNREACH_HOSTPROHI = 10,
  NETWIB_ICMP4CODE_DSTUNREACH_NETTOS = 11,
  NETWIB_ICMP4CODE_DSTUNREACH_HOSTTOS = 12,
  NETWIB_ICMP4CODE_DSTUNREACH_PROHI = 13,
  NETWIB_ICMP4CODE_DSTUNREACH_PRECVIOL = 14,
  NETWIB_ICMP4CODE_DSTUNREACH_PRECCUTOFF = 15,
  NETWIB_ICMP4CODE_SRCQUENCH = 0,
  NETWIB_ICMP4CODE_REDIRECT_NET = 0,
  NETWIB_ICMP4CODE_REDIRECT_HOST = 1,
  NETWIB_ICMP4CODE_REDIRECT_NETTOS = 2,
  NETWIB_ICMP4CODE_REDIRECT_HOSTTOS = 3,
  NETWIB_ICMP4CODE_ECHOREQ = 0,
  NETWIB_ICMP4CODE_TIMEEXCEED_TTL = 0,
  NETWIB_ICMP4CODE_TIMEEXCEED_FRAG = 1,
  NETWIB_ICMP4CODE_PARAPROB_POINTER = 0,
  NETWIB_ICMP4CODE_PARAPROB_MISSOPT = 1,
  NETWIB_ICMP4CODE_PARAPROB_BADLEN = 2,
  NETWIB_ICMP4CODE_TIMESTAMPREQ = 0,
  NETWIB_ICMP4CODE_TIMESTAMPREP = 0,
  NETWIB_ICMP4CODE_INFOREQ = 0,
  NETWIB_ICMP4CODE_INFOREP = 0
} netwib_icmp4code;

/*-------------------------------------------------------------*/
/* Name : netwib_buf_append_icmp4type
   Description :
     Print the text corresponding to icmp type.
   Input parameter(s) :
     type : icmp4 type
   Input/output parameter(s) :
     *pbuf : netwib_buf receiving data
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_append_icmp4type(netwib_icmp4type type,
                                       netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* Name : netwib_buf_append_icmp4code
   Description :
     Print the text corresponding to icmp code.
   Input parameter(s) :
     type : icmp4 type
     code : icmp4 code
   Input/output parameter(s) :
     *pbuf : netwib_buf receiving data
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_append_icmp4code(netwib_icmp4type type,
                                       netwib_icmp4code code,
                                       netwib_buf *pbuf);

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 id;
  netwib_uint16 seqnum;
  netwib_bufext data;
} netwib_icmp4_echo;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 reserved;
  netwib_bufext badippacket;
} netwib_icmp4_dstunreach;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 reserved;
  netwib_bufext badippacket;
} netwib_icmp4_srcquench;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_ip gw;
  netwib_bufext badippacket;
} netwib_icmp4_redirect;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 reserved;
  netwib_bufext badippacket;
} netwib_icmp4_timeexceed;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint8 pointer;
  netwib_uint32 reserved;
  netwib_bufext badippacket;
} netwib_icmp4_paraprob;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 id;
  netwib_uint16 seqnum;
  netwib_uint32 originatetimestamp;
  netwib_uint32 receivetimestamp;
  netwib_uint32 transmittimestamp;
} netwib_icmp4_timestamp;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 id;
  netwib_uint16 seqnum;
  netwib_bufext data;
} netwib_icmp4_info;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_icmp4type type;
  netwib_icmp4code code;
  netwib_uint16 check;
  union {
    netwib_icmp4_echo echo; /* used for request and reply */
    netwib_icmp4_dstunreach dstunreach;
    netwib_icmp4_srcquench srcquench;
    netwib_icmp4_redirect redirect;
    netwib_icmp4_timeexceed timeexceed;
    netwib_icmp4_paraprob paraprob;
    netwib_icmp4_timestamp timestamp; /* used for req and rep */
    netwib_icmp4_info info; /* used for request and reply */
  } msg;
} netwib_icmp4;
typedef const netwib_icmp4 netwib_consticmp4;
#define NETWIB_ICMP4_MINLEN 4

/*-------------------------------------------------------------*/
/* Name : netwib_icmp4_initdefault
   Description :
     Initialize a netwib_icmp4 with default values.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     *picmp4hdr : netwib_icmp4hdr to initialize
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_icmp4_initdefault(netwib_icmp4type type,
                                    netwib_icmp4 *picmp4);

/*-------------------------------------------------------------*/
/* Name : netwib_pkt_append_icmp4
   Description :
     Initialize physical data from a netwib_icmp4.
   Input parameter(s) :
     icmp4 : netwib_icmp4
   Input/output parameter(s) :
     *ppht : buffer to initialize
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_pkt_append_icmp4(netwib_consticmp4 *picmp4,
                                   netwib_buf *ppkt);

/*-------------------------------------------------------------*/
/* Name : netwib_pkt_decode_icmp4
   Description :
     Decode a netwib_icmp4 from data coming from wire.
   Input parameter(s) :
     pktproto : protocol of the netwib_icmp4 in ppkt
     *ppkt : buffer containing physical data
   Input/output parameter(s) :
   Output parameter(s) :
     *picmp4 : netwib_icmp4 to initialize
     *pskipsize : size to skip this header
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_pkt_decode_icmp4(netwib_constbuf *ppkt,
                                   netwib_icmp4 *picmp4,
                                   netwib_uint32 *pskipsize);
