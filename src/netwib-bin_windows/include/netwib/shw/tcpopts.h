
/*-------------------------------------------------------------*/
/* Name : netwib_tcpopts_show
   Description :
     Print a buffer containing one or several TCP option.
   Input parameter(s) :
     *ptcpopts : TCP options to print
     encodetype : netwib_encodetype to use
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf receiving data
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_tcpopts_show(netwib_constbuf *ptcpopts,
                               netwib_encodetype encodetype,
                               netwib_buf *pbuf);







