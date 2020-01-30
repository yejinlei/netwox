
/*-------------------------------------------------------------*/
/* UDP header */
typedef struct {
  netwib_port src;
  netwib_port dst;
  netwib_uint16 len;
  netwib_uint16 check;
} netwib_udphdr;
typedef const netwib_udphdr netwib_constudphdr;
#define NETWIB_UDPHDR_LEN 8

/*-------------------------------------------------------------*/
/* Name : netwib_udphdr_initdefault
   Description :
     Initialize a netwib_udphdr with default values.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     *pudphdr : netwib_udphdr to initialize
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_udphdr_initdefault(netwib_udphdr *pudphdr);

/*-------------------------------------------------------------*/
/* Name : netwib_pkt_append_udphdr
   Description :
     Initialize physical data from a netwib_udphdr.
   Input parameter(s) :
     *pudphdr : netwib_udphdr
   Input/output parameter(s) :
     *ppht : buffer to initialize
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_pkt_append_udphdr(netwib_constudphdr *pudphdr,
                                    netwib_buf *ppkt);
netwib_err netwib_pkt_prepend_udphdr(netwib_constudphdr *pudphdr,
                                     netwib_buf *ppkt);

/*-------------------------------------------------------------*/
/* Name : netwib_pkt_decode_udphdr
   Description :
     Decode a netwib_udphdr from physical data.
   Input parameter(s) :
     *ppkt : buffer containing physical data
   Input/output parameter(s) :
   Output parameter(s) :
     *pudphdr : netwib_udphdr contained in pkt
     *pskipsize : size to use to skip this header
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_NOTCONVERTED : pkt could not be decoded
     NETWIB_ERR_DATAMISSING : pkt was not big enough
*/
netwib_err netwib_pkt_decode_udphdr(netwib_constbuf *ppkt,
                                    netwib_udphdr *pudphdr,
                                    netwib_uint32 *pskipsize);
