
/*-------------------------------------------------------------*/
/* Name : netwib_ip6exts_show
   Description :
     Print a packet containing one or several IP options.
   Input parameter(s) :
     pktproto : protocol of the first netwib_ip6ext in ppkt
     *ppkt : buffer containing physical data
     encodetype : netwib_encodetype to use
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf receiving data
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ip6exts_show(netwib_ipproto pktproto,
                               netwib_constbuf *ppkt,
                               netwib_encodetype encodetype,
                               netwib_buf *pbuf);

