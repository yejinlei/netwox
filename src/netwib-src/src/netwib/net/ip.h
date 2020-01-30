
/*-------------------------------------------------------------*/
/***************************************************************
 * A netwib_ip represents an IP address.                       *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* Name : netwib_ip_init_ip4_fields
   Description :
     Initialize an netwib_ip address.
   Input parameter(s) :
     a,b,c,d : bytes of the IP address
   Input/output parameter(s) :
   Output parameter(s) :
     *pip : netwib_ip set with a.b.c.d
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ip_init_ip4_fields(netwib_byte a,
                                     netwib_byte b,
                                     netwib_byte c,
                                     netwib_byte d,
                                     netwib_ip *pip);

/*-------------------------------------------------------------*/
/* Name : netwib_ip_init_ip4
   Description :
     Initialize an netwib_ip address from an netwib_ip4.
   Input parameter(s) :
     ip4 : netwib_ip4 address
   Input/output parameter(s) :
   Output parameter(s) :
     *pip : netwib_ip set
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ip_init_ip4(netwib_ip4 ip4,
                              netwib_ip *pip);

/*-------------------------------------------------------------*/
/* Name : netwib_ip4_init_ip
   Description :
     Initialize an netwib_ip4 address from an netwib_ip.
   Input parameter(s) :
     ip : netwib_ip address
   Input/output parameter(s) :
   Output parameter(s) :
     *pip4 : netwib_ip4 set
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_NOTCONVERTED : the address could not be represented
*/
netwib_err netwib_ip4_init_ip(netwib_constip *pip,
                              netwib_ip4 *pip4);

/*-------------------------------------------------------------*/
/* Name : netwib_ip_init_ip6_fields
   Description :
     Initialize an netwib_ip address.
   Input parameter(s) :
     a,b,c,d : "aaaa:aaaa:bbbb:bbbb:cccc:cccc:dddd:dddd"
   Input/output parameter(s) :
   Output parameter(s) :
     *pip : netwib_ip set with "aaaa:aaaa:bbbb:b..."
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ip_init_ip6_fields(netwib_uint32 a,
                                     netwib_uint32 b,
                                     netwib_uint32 c,
                                     netwib_uint32 d,
                                     netwib_ip *pip);

/*-------------------------------------------------------------*/
/* Name : netwib_ip_init_ip6
   Description :
     Initialize an netwib_ip address from an netwib_ip6.
   Input parameter(s) :
     ip6 : netwib_ip6 address
   Input/output parameter(s) :
   Output parameter(s) :
     *pip : netwib_ip set
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ip_init_ip6(netwib_constip6 *pip6,
                              netwib_ip *pip);

/*-------------------------------------------------------------*/
/* Name : netwib_ip6_init_ip
   Description :
     Initialize an netwib_ip6 address from an netwib_ip.
   Input parameter(s) :
     ip : netwib_ip address
   Input/output parameter(s) :
   Output parameter(s) :
     *pip6 : netwib_ip6 set
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ip6_init_ip(netwib_constip *pip,
                              netwib_ip6 *pip6);

/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_IP_DECODETYPE_IP4 = 1, /* "1.2.3.4" only */
  NETWIB_IP_DECODETYPE_IP6,     /* "aaaa:bbbb:..." only */
  NETWIB_IP_DECODETYPE_IP,      /* try IP4 then IP6 */
  NETWIB_IP_DECODETYPE_HN4,     /* server.example.com->IPv4 only */
  NETWIB_IP_DECODETYPE_HN6,     /* server.example.com->IPv4 only */
  NETWIB_IP_DECODETYPE_HN,      /* try HN4 then HN6 */
  NETWIB_IP_DECODETYPE_IP4HN4,  /* if IP4 does not work, use HN4 */
  NETWIB_IP_DECODETYPE_IP6HN6,  /* if IP6 does not work, use HN6 */
  NETWIB_IP_DECODETYPE_IPHN,    /* try IP, then HN */
  NETWIB_IP_DECODETYPE_BEST = NETWIB_IP_DECODETYPE_IPHN  /* best */
} netwib_ip_decodetype;

/*-------------------------------------------------------------*/
/* Name : netwib_ip_init_buf
   Description :
     Convert a string "1.2.3.4", "aaaa:bbbb:..." or a
     hostname to a netwib_ip. To convert a hostname resolving
     to several IP addresses, function netwib_ips_add_buf
     has to be used instead.
   Input parameter(s) :
     *pbuf : string to convert
   Input/output parameter(s) :
   Output parameter(s) :
     *pip : netwib_ip set
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_NOTCONVERTED : the address could not be resolved
*/
netwib_err netwib_ip_init_buf(netwib_constbuf *pbuf,
                              netwib_ip_decodetype decodetype,
                              netwib_ip *pip);
#define netwib_ip_init_buf_best(pbuf,pip) netwib_ip_init_buf(pbuf,NETWIB_IP_DECODETYPE_BEST,pip)

/*-------------------------------------------------------------*/
/* Name : netwib_ip_init_eth
   Description :
     Convert a netwib_eth to a netwib_ip.
   Input parameter(s) :
     eth : netwib_eth to convert
   Input/output parameter(s) :
   Output parameter(s) :
     *pip : netwib_ip set with eth value
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_NOTCONVERTED : the address could not be resolved
   Note :
     This function does not currently support IPv6. It have
     to be finished.
*/
netwib_err netwib_ip_init_eth(netwib_consteth *peth,
                              netwib_ip *pip);

/*-------------------------------------------------------------*/
/* Name : netwib_ip_init_kbd
   Description :
     Initialize an netwib_ip with the address entered by the
     user through the keyboard
   Input parameter(s) :
     message : message to print before
     defaulthn : default hostname or ip address to use if
                 user enters nothing. If NULL, there is no default
   Input/output parameter(s) :
   Output parameter(s) :
     *pip : netwib_ip set with user's string
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ip_init_kbd(netwib_constbuf *pmessage,
                              netwib_constbuf *pdefaulthn,
                              netwib_ip *pip);
#define NETWIB_IP_INIT_KBD_NODEF NULL

/*-------------------------------------------------------------*/
/* Name : netwib_ip_cmp
   Description :
     Compare two netwib_ip.
   Input parameter(s) :
     ip1 : netwib_ip to compare with ip2
     ip2 : netwib_ip to compare with ip1
   Input/output parameter(s) :
   Output parameter(s) :
     *pcmp :
       NETWIB_CMP_LT : ip1<ip2
       NETWIB_CMP_EQ : if ip1 and ip2 are equal
       NETWIB_CMP_GT : ip1>ip2
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ip_cmp(netwib_constip *pip1,
                         netwib_constip *pip2,
                         netwib_cmp *pcmp);

/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_IP_ENCODETYPE_IP = 1,   /* "1.2.3.4" */
  NETWIB_IP_ENCODETYPE_HN,       /* server.example.com */
  NETWIB_IP_ENCODETYPE_HNIP,     /* if HN does not work, use IP */
  NETWIB_IP_ENCODETYPE_HNS,      /* "host1,host2,etc." */
  NETWIB_IP_ENCODETYPE_BEST = NETWIB_IP_ENCODETYPE_HNIP /* best */
} netwib_ip_encodetype;

/*-------------------------------------------------------------*/
/* Name : netwib_buf_append_ip
   Description :
     Append a string representing a netwib_ip.
   Input parameter(s) :
     ip : netwib_ip to append
     encodetype : encodetype to use
   Input/output parameter(s) :
     pbuf : buffer updated
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_NOTCONVERTED : the address could not be resolved
*/
netwib_err netwib_buf_append_ip(netwib_constip *pip,
                                netwib_ip_encodetype encodetype,
                                netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* Name : netwib_buf_append_iptype
   Description :
     Obtains the buf string corresponding to a type.
   Input parameter(s) :
     type : type
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf set with the buf string
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_append_iptype(netwib_iptype type,
                                    netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* Name : netwib_iptype_init_kbd
   Description :
     Initialize a netwib_iptype with data entered
     through keyboard.
   Input parameter(s) :
     *pmessage : message to print before
     defaulttype : default type to use if user enters
                   nothing (if NETWIB_IPTYPE_UNKNOWN means
                   no default)
   Input/output parameter(s) :
   Output parameter(s) :
     *phwtype : netwib_hwtype initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_iptype_init_kbd(netwib_constbuf *pmessage,
                                  netwib_iptype defaulttype,
                                  netwib_iptype *ptype);
#define NETWIB_IPTYPE_INIT_KBD_NODEF NETWIB_IPTYPE_UNKNOWN
