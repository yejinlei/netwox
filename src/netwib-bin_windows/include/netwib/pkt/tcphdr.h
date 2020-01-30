
/*-------------------------------------------------------------*/
/* TCP header */
typedef struct {
  netwib_port src;
  netwib_port dst;
  netwib_uint32 seqnum;
  netwib_uint32 acknum;
  netwib_uint8 doff;
  netwib_bool reserved1;
  netwib_bool reserved2;
  netwib_bool reserved3;
  netwib_bool reserved4;
  netwib_bool cwr; /* RFC 3168 */
  netwib_bool ece; /* RFC 3168 */
  netwib_bool urg;
  netwib_bool ack;
  netwib_bool psh;
  netwib_bool rst;
  netwib_bool syn;
  netwib_bool fin;
  netwib_uint16 window;
  netwib_uint16 check;
  netwib_uint16 urgptr;
  netwib_bufext opts; /* TCP options as defined in tcpopts.h */
} netwib_tcphdr;
typedef const netwib_tcphdr netwib_consttcphdr;
#define NETWIB_TCPHDR_MINLEN 20
#define NETWIB_TCPHDR_MAXLEN 64

/*-------------------------------------------------------------*/
/* Name : netwib_tcphdr_initdefault
   Description :
     Initialize a netwib_tcphdr with default values.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     *ptcphdr : netwib_tcphdr to initialize
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_tcphdr_initdefault(netwib_tcphdr *ptcphdr);

/*-------------------------------------------------------------*/
/* Name : netwib_pkt_append_tcphdr
   Description :
     Initialize physical data from a netwib_tcphdr.
   Input parameter(s) :
     *ptcphdr : netwib_tcphdr
   Input/output parameter(s) :
     *ppht : buffer to initialize
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
   Note : field doff is eventually recomputed.
*/
netwib_err netwib_pkt_append_tcphdr(netwib_consttcphdr *ptcphdr,
                                    netwib_buf *ppkt);
netwib_err netwib_pkt_prepend_tcphdr(netwib_consttcphdr *ptcphdr,
                                     netwib_buf *ppkt);

/*-------------------------------------------------------------*/
/* Name : netwib_pkt_decode_tcphdr
   Description :
     Decode a netwib_tcphdr from physical data.
   Input parameter(s) :
     *ppkt : buffer containing physical data
   Input/output parameter(s) :
   Output parameter(s) :
     *ptcphdr : netwib_tcphdr contained in pkt
     *pskipsize : size to use to skip this header
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_NOTCONVERTED : pkt could not be decoded
     NETWIB_ERR_DATAMISSING : pkt was not big enough
*/
netwib_err netwib_pkt_decode_tcphdr(netwib_constbuf *ppkt,
                                    netwib_tcphdr *ptcphdr,
                                    netwib_uint32 *pskipsize);
