
/*-------------------------------------------------------------*/
/***************************************************************
 * Currently, only Ethernet-IPv4 is implemented as ARP.        *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* ARP header */
typedef enum {
  NETWIB_ARPHDROP_ARPREQ = 1,
  NETWIB_ARPHDROP_ARPREP = 2,
  NETWIB_ARPHDROP_RARPREQ = 3,
  NETWIB_ARPHDROP_RARPREP = 4
} netwib_arphdrop;
typedef struct
{ netwib_arphdrop op;
  netwib_eth ethsrc;
  netwib_ip ipsrc;
  netwib_eth ethdst;
  netwib_ip ipdst;
} netwib_arphdr;
typedef const netwib_arphdr netwib_constarphdr;
#define NETWIB_ARPHDR_LEN 28

/*-------------------------------------------------------------*/
/* Name : netwib_arphdr_initdefault
   Description :
     Initialize a netwib_arphdr with default values.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     *parphdr : netwib_arphdr to initialize
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_arphdr_initdefault(netwib_arphdr *parphdr);

/*-------------------------------------------------------------*/
/* Name : netwib_pkt_append_arphdr
   Description :
     Initialize physical data from a netwib_arphdr.
   Input parameter(s) :
     *parphdr : netwib_arphdr
   Input/output parameter(s) :
     *ppht : buffer to initialize
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_pkt_append_arphdr(netwib_constarphdr *parphdr,
                                    netwib_buf *ppkt);
netwib_err netwib_pkt_prepend_arphdr(netwib_constarphdr *parphdr,
                                     netwib_buf *ppkt);

/*-------------------------------------------------------------*/
/* Name : netwib_pkt_decode_arphdr
   Description :
     Decode a netwib_arphdr from physical data.
   Input parameter(s) :
     *ppkt : buffer containing physical data
   Input/output parameter(s) :
   Output parameter(s) :
     *parphdr : netwib_arphdr contained in pkt
     *pskipsize : size to use to skip this header
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_NOTCONVERTED : pkt could not be decoded
     NETWIB_ERR_DATAMISSING : pkt was not big enough
*/
netwib_err netwib_pkt_decode_arphdr(netwib_constbuf *ppkt,
                                    netwib_arphdr *parphdr,
                                    netwib_uint32 *pskipsize);
