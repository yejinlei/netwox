
/*-------------------------------------------------------------*/
/* Name : netwib_ip4opt_show
   Description :
     Print a netwib_ip4opt.
   Input parameter(s) :
     ip4opt : netwib_ip4opt to print
     encodetype : netwib_encodetype to use
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf receiving data
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ip4opt_show(netwib_constip4opt *pip4opt,
                              netwib_encodetype encodetype,
                              netwib_buf *pbuf);
