
/*-------------------------------------------------------------*/
/* Name : netwib_icmp6_show
   Description :
     Print a netwib_icmp6.
   Input parameter(s) :
     *picmp6 : netwib_icmp6 to print
     encodetype : netwib_encodetype to use
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf receiving data
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_icmp6_show(netwib_consticmp6 *picmp6,
                             netwib_encodetype encodetype,
                             netwib_buf *pbuf);
