
/*-------------------------------------------------------------*/
/* Name : netwib_udphdr_show
   Description :
     Print a netwib_udphdr.
   Input parameter(s) :
     *pudphdr : netwib_udphdr to print
     encodetype : netwib_encodetype to use
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf receiving data
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_udphdr_show(netwib_constudphdr *pudphdr,
                              netwib_encodetype encodetype,
                              netwib_buf *pbuf);
