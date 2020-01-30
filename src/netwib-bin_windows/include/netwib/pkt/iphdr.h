
/*-------------------------------------------------------------*/
/***************************************************************
 * A netwib_iphdr permits to store :                           *
 *  - an IPv4 header (rfc 791), or                             *
 *  - an IPv6 header (rfc 2460)                                *
 ***************************************************************/
typedef struct {
  /* common fields */
  netwib_iptype iptype;
  netwib_ip src;
  netwib_ip dst;
  netwib_uint8 ttl;
  netwib_ipproto protocol;
  /* specific fields */
  union {
    struct {
      netwib_uint8 ihl;
      netwib_uint8 tos;
      netwib_uint16 totlen;
      netwib_uint16 id;
      netwib_bool reserved;
      netwib_bool dontfrag;
      netwib_bool morefrag;
      netwib_uint16 offsetfrag;
      netwib_uint16 check;
      netwib_bufext opts; /* IPv4 options as defined in ip4opts.h */
    } ip4;
    struct {
      netwib_uint8 trafficclass;
      netwib_uint32 flowlabel;
      netwib_uint16 payloadlength;
      netwib_bufext exts; /* IPv6 extensions as defined in ip6exts.h */
    } ip6;
  } header;
} netwib_iphdr;
typedef const netwib_iphdr netwib_constiphdr;
#define NETWIB_IPHDR_MINLEN 40

/*-------------------------------------------------------------*/
/* Name : netwib_iphdr_initdefault
   Description :
     Initialize a netwib_iphdr with default values.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     *piphdr : netwib_iphdr to initialize
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_iphdr_initdefault(netwib_iptype iptype,
                                    netwib_iphdr *piphdr);

/*-------------------------------------------------------------*/
/* Name : netwib_pkt_append_iphdr
   Description :
     Initialize physical data from a netwib_iphdr.
   Input parameter(s) :
     *piphdr : netwib_iphdr
   Input/output parameter(s) :
     *ppht : buffer to initialize
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_pkt_append_iphdr(netwib_constiphdr *piphdr,
                                   netwib_buf *ppkt);
netwib_err netwib_pkt_prepend_iphdr(netwib_constiphdr *piphdr,
                                    netwib_buf *ppkt);

/*-------------------------------------------------------------*/
/* Name : netwib_pkt_decode_iphdr
   Description :
     Decode a netwib_iphdr from physical data.
   Input parameter(s) :
     *ppkt : buffer containing physical data
   Input/output parameter(s) :
   Output parameter(s) :
     *piphdr : netwib_iphdr contained in pkt
     *pskipsize : size to use to skip this header
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_NOTCONVERTED : pkt could not be decoded
     NETWIB_ERR_DATAMISSING : pkt was not big enough
*/
netwib_err netwib_pkt_decode_iphdr(netwib_constbuf *ppkt,
                                   netwib_iphdr *piphdr,
                                   netwib_uint32 *pskipsize);

/*-------------------------------------------------------------*/
/***************************************************************
 * A netwib_iphdr contains a protocol field, it is stored :    *
 *  - IPv4 : in header.protocol                                *
 *  - IPv6 : if exts is empty : in header.protocol,            *
 *           else : in last extension of header.ip6.exts       *
 ***************************************************************/
netwib_err netwib_iphdr_set_proto(netwib_iphdr *piphdr,
                                  netwib_ipproto ipproto);
netwib_err netwib_iphdr_get_proto(netwib_constiphdr *piphdr,
                                  netwib_ipproto *pipproto);
