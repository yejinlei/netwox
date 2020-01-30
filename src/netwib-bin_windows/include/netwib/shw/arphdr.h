
/*-------------------------------------------------------------*/
/* Name : netwib_arphdr_show
   Description :
     Print a netwib_arphdr.
   Input parameter(s) :
     *parphdr : netwib_arphdr to print
     encodetype : netwib_encodetype to use
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf receiving data
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_arphdr_show(netwib_constarphdr *parphdr,
                              netwib_encodetype encodetype,
                              netwib_buf *pbuf);

