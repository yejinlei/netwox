
/*-------------------------------------------------------------*/
typedef netwib_err (*netwib_ip_frag_pf)(netwib_constbuf *pfrag,
                                        netwib_ptr infos);

/*-------------------------------------------------------------*/
/* Name : netwib_ip_frag
   Description :
     Fragment an IP packet
   Input parameter(s) :
     *pippkt : IP packet (containing IP header)
     fragdatasize : data size to send in each fragment in bytes.
                    It is rounded to 8 : 1->8bytes, 7->8bytes,
                    8->8, 9->16, etc.
                    The first fragment may be bigger than
                    the others because it contains more IPv4
                    options, or IPv6 extensions.
                    If fragdatasize == 0, this limit is not used.
     fragtotalsize : maximum total size of the packet in bytes.
                     If fragtotalsize == 0, this limit is not used.
     pfunc : memory address of the function which will
             be called for each fragment
             For each call, the first parameter ('infos')
             will be set with the optional parameter below.
   Input/output parameter(s) :
     infos : optional parameter (can be NULL) which will be
             used as the first parameter for *pfunc.
             This may be used to send information to *pfunc.
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ip_frag(netwib_constbuf *pippkt,
                          netwib_uint32 fragdatasize,
                          netwib_uint32 fragtotalsize,
                          netwib_ip_frag_pf pfunc,
                          netwib_ptr infos);

/*-------------------------------------------------------------*/
#define NETWIB_MTU_MIN_IP4 576 /* rfc 791 */
#define NETWIB_MTU_MIN_IP6 1280 /* rfc 2460 */
