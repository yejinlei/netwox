
/*-------------------------------------------------------------*/
/* Name : netwib_linkhdr_show
   Description :
     Print a netwib_linkhdr.
   Input parameter(s) :
     linkhdr : netwib_linkhdr to print
     encodetype : netwib_encodetype to use
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf receiving data
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_linkhdr_show(netwib_constlinkhdr *plinkhdr,
                               netwib_encodetype encodetype,
                               netwib_buf *pbuf);
