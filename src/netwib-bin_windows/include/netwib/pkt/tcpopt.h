
/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_TCPOPTTYPE_END = 0,          /* rfc 793 - end of options */
  NETWIB_TCPOPTTYPE_NOOP = 1,         /* rfc 793 - no operation */
  NETWIB_TCPOPTTYPE_MSS = 2,          /* rfc 793 - maximum segment size */
  NETWIB_TCPOPTTYPE_WINDOWSCALE = 3,  /* rfc 1323 - window scale */
  NETWIB_TCPOPTTYPE_SACKPERMITTED = 4,/* rfc 2018 - SACK permitted */
  NETWIB_TCPOPTTYPE_SACK = 5,    /* rfc 2018 - selective acknowledgment */
  NETWIB_TCPOPTTYPE_ECHOREQUEST = 6,  /* rfc 1072 - echo request */
  NETWIB_TCPOPTTYPE_ECHOREPLY = 7,    /* rfc 1072 - echo reply */
  NETWIB_TCPOPTTYPE_TIMESTAMP = 8,    /* rfc 1323 - timestamp */
  NETWIB_TCPOPTTYPE_CC = 11,          /* rfc 1644 - connection count */
  NETWIB_TCPOPTTYPE_CCNEW = 12,       /* rfc 1644 - connection count */
  NETWIB_TCPOPTTYPE_CCECHO = 13       /* rfc 1644 - connection count */
} netwib_tcpopttype;

/*-------------------------------------------------------------*/
/* maximum length of one TCP option */
#define NETWIB_TCPOPT_MAXLEN 40

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 maxsegsize;
} netwib_tcpopt_mss;
typedef const netwib_tcpopt_mss netwib_consttcpopt_mss;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint8 windowscale;
} netwib_tcpopt_windowscale;
typedef const netwib_tcpopt_windowscale netwib_consttcpopt_windowscale;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 storedvalues; /* 0 to 4 */
  netwib_uint32 leftedge[4];
  netwib_uint32 rightedge[4];
} netwib_tcpopt_sack;
typedef const netwib_tcpopt_sack netwib_consttcpopt_sack;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 data;
} netwib_tcpopt_echo;
typedef const netwib_tcpopt_echo netwib_consttcpopt_echo;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 val;
  netwib_uint32 echoreply;
} netwib_tcpopt_timestamp;
typedef const netwib_tcpopt_timestamp netwib_consttcpopt_timestamp;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 connectioncount;
} netwib_tcpopt_cc;
typedef const netwib_tcpopt_cc netwib_consttcpopt_cc;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_tcpopttype type;
  union {
    /* type end : no value */
    /* type noop : no value */
    netwib_tcpopt_mss mss;
    netwib_tcpopt_windowscale windowscale;
    /* type sackpermitted : no value */
    netwib_tcpopt_sack sack;
    netwib_tcpopt_echo echo; /* used for request and reply */
    netwib_tcpopt_timestamp timestamp;
    netwib_tcpopt_cc cc; /* used for cc, ccnew and ccecho */
  } opt;
} netwib_tcpopt;
typedef const netwib_tcpopt netwib_consttcpopt;

/*-------------------------------------------------------------*/
/* Name : netwib_tcpopt_initdefault
   Description :
     Initialize a netwib_tcpopt with default values.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     *ptcpopt : netwib_tcpopt to initialize
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_tcpopt_initdefault(netwib_tcpopttype tcpopttype,
                                     netwib_tcpopt *ptcpopt);

/*-------------------------------------------------------------*/
/* Name : netwib_pkt_append_tcpopt
   Description :
     Initialize physical data from a netwib_tcpopt.
   Input parameter(s) :
     tcpopt : netwib_tcpopt
   Input/output parameter(s) :
     *ppht : buffer to initialize
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_pkt_append_tcpopt(netwib_consttcpopt *ptcpopt,
                                    netwib_buf *ppkt);
netwib_err netwib_pkt_prepend_tcpopt(netwib_consttcpopt *ptcpopt,
                                     netwib_buf *ppkt);

/*-------------------------------------------------------------*/
/* Name : netwib_pkt_decode_tcpopt
   Description :
     Decode a netwib_tcpopt from data coming from wire.
   Input parameter(s) :
     *ppkt : buffer containing physical data
   Input/output parameter(s) :
   Output parameter(s) :
     *ptcpopt : netwib_tcpopt to initialize
     *pskipsize : size to skip this option
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_pkt_decode_tcpopt(netwib_constbuf *ppkt,
                                    netwib_tcpopt *ptcpopt,
                                    netwib_uint32 *pskipsize);
