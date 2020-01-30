
/*-------------------------------------------------------------*/
/* Name : netwib_ip6ext_show
   Description :
     Print a netwib_ip6ext.
   Input parameter(s) :
     ip6ext : netwib_ip6ext to print
     encodetype : netwib_encodetype to use
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf receiving data
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ip6ext_show(netwib_constip6ext *pip6ext,
                              netwib_encodetype encodetype,
                              netwib_buf *pbuf);

