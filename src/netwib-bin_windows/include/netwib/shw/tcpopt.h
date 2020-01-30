
/*-------------------------------------------------------------*/
/* Name : netwib_tcpopt_show
   Description :
     Print a netwib_tcpopt.
   Input parameter(s) :
     tcpopt : netwib_tcpopt to print
     encodetype : netwib_encodetype to use
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf receiving data
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_tcpopt_show(netwib_consttcpopt *ptcpopt,
                              netwib_encodetype encodetype,
                              netwib_buf *pbuf);




