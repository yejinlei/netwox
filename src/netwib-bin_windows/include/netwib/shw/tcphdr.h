
/*-------------------------------------------------------------*/
/* Name : netwib_tcphdr_show
   Description :
     Print a netwib_tcphdr.
   Input parameter(s) :
     *ptcphdr : netwib_tcphdr to print
     encodetype : netwib_encodetype to use
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf receiving data
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_tcphdr_show(netwib_consttcphdr *ptcphdr,
                              netwib_encodetype encodetype,
                              netwib_buf *pbuf);
