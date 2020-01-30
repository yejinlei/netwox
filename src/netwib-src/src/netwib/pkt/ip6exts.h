
/*-------------------------------------------------------------*/
/***************************************************************
 * A "netwib_ip6exts" is a netwib_buf storing 0, 1 or more     *
 * netwib_ip6ext. There is no typedef for netwib_ip6exts.      *
 ***************************************************************/

/*-------------------------------------------------------------*/
/***************************************************************
 * To create a "netwib_ip6exts", loop using function           *
 * netwib_pkt_append_ip6ext.                                   *
 * To decode a "netwib_ip6exts", loop using function           *
 * netwib_pkt_decode_ip6ext.                                   *
 ***************************************************************/

/*-------------------------------------------------------------*/
/***************************************************************
 * Normally, an IPv6 packet should be created and decoded      *
 * layer by layer. So, there should be no need for an          *
 * netwib_ip6exts, containing several layers/ip6_extensions.   *
 * However, this behavior makes harder the creation of         *
 * functions for both IPv4 (which contains IP4 options) and    *
 * IPv6.                                                       *
 * So, in netwib the netwib_ip6hdr contains a buffer of        *
 * netwib_ip6exts.                                             *
 *                                                             *
 * When decoding a packet, the netwib_ip6exts buffer is set to *
 * all recognized extensions :                                 *
 *   NETWIB_IPPROTO_HOPOPTS                                    *
 *   NETWIB_IPPROTO_DSTOPTS                                    *
 *   NETWIB_IPPROTO_ROUTING                                    *
 *   NETWIB_IPPROTO_FRAGMENT                                   *
 *   NETWIB_IPPROTO_AH                                         *
 * Then, the packet eventually starts with unknown extensions  *
 * or real data.                                               *
 * Example 1 :                                                 *
 *   packet is IP6+HOPOPTS+FRAGMENT+TCP                        *
 *   ip6exts in ip6hdr will contain : HOPOPTS+FRAGMENT         *
 *   next data will be : TCP                                   *
 * Example 2 :                                                 *
 *   packet is IP6+TCP                                         *
 *   ip6exts in ip6hdr will contain : nothing                  *
 *   next data will be : TCP                                   *
 * Example 3 :                                                 *
 *   packet is IP6+HOPOPTS+unknown+TCP                         *
 *   ip6exts in ip6hdr will contain : HOPOPTS                  *
 *   next data will be : unknown+TCP                           *
 * Example 4 :                                                 *
 *   packet is IP6+HOPOPTS+unknown+FRAGMENT+TCP                *
 *   ip6exts in ip6hdr will contain : HOPOPTS                  *
 *   next data will be : unknown+FRAGMENT+TCP                  *
 * If you prefer the layer by layer behavior, just concatenate *
 * ip6exts and next_data.                                      *
 *                                                             *
 * When creating a packet, there are 2 ways :                  *
 *  - store extensions in ip6exts, or                          *
 *  - left empty ip6exts, and put extensions at the beginning  *
 *    of data                                                  *
 * The first solution is preferred, because netwib will        *
 * magically deal with them if necessary.                      *
 ***************************************************************/

/*-------------------------------------------------------------*/
/***************************************************************
 * RFC 2640 says "The Hop-by-Hop Options header, when present, *
 * must immediately follow the IPv6 header". This is not       *
 * enforced in netwib.                                         *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* Name : netwib_pkt_decode_ip6exts
   Description :
     Obtain size of all recognized extensions. Also obtain
     the last proto and its offset (to easily overwrite it).
     The recognized extensions are :
        NETWIB_IPPROTO_HOPOPTS
        NETWIB_IPPROTO_DSTOPTS
        NETWIB_IPPROTO_ROUTING
        NETWIB_IPPROTO_FRAGMENT
        NETWIB_IPPROTO_AH
   Input parameter(s) :
     pktproto : protocol of the netwib_ip6ext in ppkt
     *ppkt : buffer containing physical data
   Input/output parameter(s) :
   Output parameter(s) :
     *plastproto : last protocol
     *plastprotooffset : offset of the byte describing the
                         last protocol. This value is unset
                         if skipsize == 0.
     *pskipsize : size to skip this/those extensions. If 0,
                  there is no extension (in this case,
                  lastproto is set to pktproto).
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_pkt_decode_ip6exts(netwib_ipproto pktproto,
                                     netwib_constbuf *ppkt,
                                     netwib_ipproto *plastproto,
                                     netwib_uint32 *plastprotooffset,
                                     netwib_uint32 *pskipsize);
