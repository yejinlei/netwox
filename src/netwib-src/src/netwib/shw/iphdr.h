
/*-------------------------------------------------------------*/
/* Name : netwib_iphdr_show
   Description :
     Print a netwib_iphdr.
   Input parameter(s) :
     *piphdr : netwib_iphdr to print
     encodetype : netwib_encodetype to use
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf receiving data
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_iphdr_show(netwib_constiphdr *piphdr,
                             netwib_encodetype encodetype,
                             netwib_buf *pbuf);
