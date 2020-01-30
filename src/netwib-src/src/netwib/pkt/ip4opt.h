
/*-------------------------------------------------------------*/
/***************************************************************
 * Refer to RFC 791 to obtain more information about IPv4      *
 * options.                                                    *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* Comments starting with "NI" are not implemented */
typedef enum {
  NETWIB_IP4OPTTYPE_END = 0,        /* end of options */
  NETWIB_IP4OPTTYPE_NOOP = 1,       /* no operation */
  NETWIB_IP4OPTTYPE_RR = 7,         /* record route */
  NETWIB_IP4OPTTYPE_TIME = 68,      /* timestamp */
  NETWIB_IP4OPTTYPE_SECURITY = 130, /* NI security compartmentation */
  NETWIB_IP4OPTTYPE_LSRR = 131,     /* loose source and record route */
  NETWIB_IP4OPTTYPE_STREAM = 136,   /* NI SATNET stream identifier */
  NETWIB_IP4OPTTYPE_SSRR = 137      /* strict source and record route */
} netwib_ip4opttype;

/*-------------------------------------------------------------*/
/* maximum length of one IP4 option */
#define NETWIB_IP4OPT_MAXLEN 40

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 storagesize; /* 0 to 9 */
  netwib_uint32 storedvalues; /* 0 to storagesize */
#define NETWIB_IP4OPT_RR_IP_LEN 9
  netwib_ip ip[NETWIB_IP4OPT_RR_IP_LEN];
} netwib_ip4opt_rr;
typedef const netwib_ip4opt_rr netwib_constip4opt_rr;

typedef struct {
  netwib_uint32 storagesize; /* 0 to 9 */
  netwib_uint32 usedvalues; /* 0 to storagesize */
#define NETWIB_IP4OPT_SRR_IP_LEN 9
  netwib_ip ip[NETWIB_IP4OPT_SRR_IP_LEN];
} netwib_ip4opt_srr;
typedef const netwib_ip4opt_srr netwib_constip4opt_srr;

typedef enum {
  NETWIB_IP4OPT_TIMEFLAG_TS = 0,
  NETWIB_IP4OPT_TIMEFLAG_IPTS = 1,
  NETWIB_IP4OPT_TIMEFLAG_IPPTS = 3
} netwib_ip4opt_timeflag;
typedef struct {
  netwib_uint32 storagesize; /* 0 to 4or9 (4 for ipts/ippts; 9 for ts) */
  netwib_uint32 storedvalues; /* 0 to storeagesize */
  netwib_uint8 overflow;
  netwib_ip4opt_timeflag flag;
#define NETWIB_IP4OPT_TIME_IP_LEN 4
  netwib_ip ip[NETWIB_IP4OPT_TIME_IP_LEN];
#define NETWIB_IP4OPT_TIME_TIMESTAMP_LEN 9
  netwib_uint32 timestamp[NETWIB_IP4OPT_TIME_TIMESTAMP_LEN];
} netwib_ip4opt_time;
typedef const netwib_ip4opt_time netwib_constip4opt_time;

typedef struct {
  netwib_ip4opttype type;
  union {
    /* type end : no value */
    /* type noop : no value */
    netwib_ip4opt_rr rr;
    netwib_ip4opt_time time;
    /* type security : NI */
    netwib_ip4opt_srr lsrr;
    /* type stream : NI */
    netwib_ip4opt_srr ssrr;
  } opt;
} netwib_ip4opt;
typedef const netwib_ip4opt netwib_constip4opt;

/*-------------------------------------------------------------*/
/* Name : netwib_ip4opt_initdefault
   Description :
     Initialize a netwib_ip4opt with default values.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     *pip4opt : netwib_ip4opt to initialize
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ip4opt_initdefault(netwib_ip4opttype ip4opttype,
                                     netwib_ip4opt *pip4opt);

/*-------------------------------------------------------------*/
/* Name : netwib_pkt_append_ip4opt
   Description :
     Initialize physical data from a netwib_ip4opt.
   Input parameter(s) :
     ip4opt : netwib_ip4opt
   Input/output parameter(s) :
     *ppht : buffer to initialize
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_pkt_append_ip4opt(netwib_constip4opt *pip4opt,
                                    netwib_buf *ppkt);
netwib_err netwib_pkt_prepend_ip4opt(netwib_constip4opt *pip4opt,
                                     netwib_buf *ppkt);

/*-------------------------------------------------------------*/
/* Name : netwib_pkt_decode_ip4opt
   Description :
     Decode a netwib_ip4opt from data coming from wire.
   Input parameter(s) :
     *ppkt : buffer containing physical data
   Input/output parameter(s) :
   Output parameter(s) :
     *pip4opt : netwib_ip4opt to initialize
     *pskipsize : size to skip this option
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_pkt_decode_ip4opt(netwib_constbuf *ppkt,
                                    netwib_ip4opt *pip4opt,
                                    netwib_uint32 *pskipsize);
