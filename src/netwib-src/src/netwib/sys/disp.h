
/*-------------------------------------------------------------*/
/* Name : netwib_buf_display
   Description :
     Print data contained in a netwib_buf.
   Input parameter(s) :
     *pbuf : buffer to print
     encodetype : netwib_encodetype to use
   Input/output parameter(s) :
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_display(netwib_constbuf *pbuf,
                              netwib_encodetype encodetype);

/*-------------------------------------------------------------*/
/* Name : netwib_fmt_display
   Description :
     Print formatted data.
   Input parameter(s) :
     fmt : format as explained in dat/fmt.h
   Input/output parameter(s) :
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_fmt_display(netwib_conststring fmt,
                              ...);
