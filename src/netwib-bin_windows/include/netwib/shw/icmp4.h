
/*-------------------------------------------------------------*/
/* Name : netwib_icmp4_show
   Description :
     Print a netwib_icmp4.
   Input parameter(s) :
     *picmp4 : netwib_icmp4 to print
     encodetype : netwib_encodetype to use
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf receiving data
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_icmp4_show(netwib_consticmp4 *picmp4,
                             netwib_encodetype encodetype,
                             netwib_buf *pbuf);
