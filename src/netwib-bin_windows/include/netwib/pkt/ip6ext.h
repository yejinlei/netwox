
/*-------------------------------------------------------------*/
/***************************************************************
 * Following functions work on extension headers of IPv6.      *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* IPv6 Hop-by-Hop Options extension header */
typedef struct {
  netwib_bufext options;
} netwib_ip6ext_hopopts;
typedef const netwib_ip6ext_hopopts netwib_constip6ext_hopopts;

/*-------------------------------------------------------------*/
/* IPv6 Destination Options extension header */
typedef struct {
  netwib_bufext options;
} netwib_ip6ext_dstopts;
typedef const netwib_ip6ext_dstopts netwib_constip6ext_dstopts;

/*-------------------------------------------------------------*/
/* IPv6 Routing extension header */
typedef struct {
  netwib_uint8 routingtype;
  netwib_uint8 segmentsleft;
  netwib_bufext data;
} netwib_ip6ext_routing;
typedef const netwib_ip6ext_routing netwib_constip6ext_routing;

/*-------------------------------------------------------------*/
/* IPv6 Fragment extension header */
typedef struct {
  netwib_uint16 fragmentoffset;
  netwib_bool reservedb1;
  netwib_bool reservedb2;
  netwib_bool morefrag;
  netwib_uint32 id;
} netwib_ip6ext_fragment;
typedef const netwib_ip6ext_fragment netwib_constip6ext_fragment;

/*-------------------------------------------------------------*/
/* IPv6 Authentication Header extension header (rfc 2402) */
typedef struct {
  netwib_uint16 reserved;
  netwib_uint32 spi;
  netwib_uint32 seqnum;
  netwib_bufext data;
} netwib_ip6ext_ah;
typedef const netwib_ip6ext_ah netwib_constip6ext_ah;

/*-------------------------------------------------------------*/
/* Generic IPv6 extension header */
typedef struct {
  netwib_ipproto proto;
  union {
    netwib_ip6ext_hopopts hopopts;
    netwib_ip6ext_dstopts dstopts;
    netwib_ip6ext_routing routing;
    netwib_ip6ext_fragment fragment;
    netwib_ip6ext_ah ah;
  } ext;
  netwib_ipproto nextproto;
} netwib_ip6ext;
typedef const netwib_ip6ext netwib_constip6ext;

/*-------------------------------------------------------------*/
/* Name : netwib_ip6ext_initdefault
   Description :
     Initialize a netwib_ip6ext with default values.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     *pip6ext : netwib_ip6ext to initialize
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ip6ext_initdefault(netwib_ipproto proto,
                                     netwib_ip6ext *pip6ext);

/*-------------------------------------------------------------*/
/* Name : netwib_pkt_append_ip6ext
   Description :
     Initialize physical data from a netwib_ip6ext.
   Input parameter(s) :
     ip6ext : netwib_ip6ext
   Input/output parameter(s) :
     *ppht : buffer to initialize
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_pkt_append_ip6ext(netwib_constip6ext *pip6ext,
                                    netwib_buf *ppkt);
netwib_err netwib_pkt_prepend_ip6ext(netwib_constip6ext *pip6ext,
                                     netwib_buf *ppkt);

/*-------------------------------------------------------------*/
/* Name : netwib_pkt_decode_ip6ext
   Description :
     Decode a netwib_ip6ext from data coming from wire.
   Input parameter(s) :
     pktproto : protocol of the netwib_ip6ext in ppkt
     *ppkt : buffer containing physical data
   Input/output parameter(s) :
   Output parameter(s) :
     *pip6ext : netwib_ip6ext to initialize
     *pskipsize : size to skip this header
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_pkt_decode_ip6ext(netwib_ipproto pktproto,
                                    netwib_constbuf *ppkt,
                                    netwib_ip6ext *pip6ext,
                                    netwib_uint32 *pskipsize);
