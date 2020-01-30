
/*-------------------------------------------------------------*/
/* Name : netwib_icmp6nd_show
   Description :
     Print a netwib_icmp6nd.
   Input parameter(s) :
     icmp6nd : netwib_icmp6nd to print
     encodetype : netwib_encodetype to use
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf receiving data
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_icmp6nd_show(netwib_consticmp6nd *picmp6nd,
                               netwib_encodetype encodetype,
                               netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* Name : netwib_icmp6nds_show
   Description :
     Print a packet containing one or several netwib_icmp6nd.
   Input parameter(s) :
     *ppkt : buffer containing physical data
     encodetype : netwib_encodetype to use
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf receiving data
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_icmp6nds_show(netwib_constbuf *ppkt,
                                netwib_encodetype encodetype,
                                netwib_buf *pbuf);

