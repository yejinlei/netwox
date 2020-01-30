
/*-------------------------------------------------------------*/
/* Name : netwib_ip4opts_show
   Description :
     Print a packet containing one or several IP options.
   Input parameter(s) :
     *ppkt : buffer containing physical data
     encodetype : netwib_encodetype to use
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf receiving data
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ip4opts_show(netwib_constbuf *ppkt,
                               netwib_encodetype encodetype,
                               netwib_buf *pbuf);

