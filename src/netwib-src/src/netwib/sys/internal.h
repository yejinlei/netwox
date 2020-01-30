
/*-------------------------------------------------------------*/
/* Name : netwib_internal_version
   Description :
     Obtain netwib version.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     *pversionmajor : major version
     *pversionminor : minor version
     *pversionmicro : micro version
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_internal_version(netwib_uint32 *pversionmajor,
                                   netwib_uint32 *pversionminor,
                                   netwib_uint32 *pversionmicro);

/*-------------------------------------------------------------*/
/* Name : netwib_buf_append_internal
   Description :
     Append a string representing all compilation parameters.
   Input parameter(s) :
   Input/output parameter(s) :
     *pbuf : netwib_buf receiving data
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_append_internal(netwib_buf *buf);

/*-------------------------------------------------------------*/
/* Name : netwib_internal_display
   Description :
     Print all compilation parameters.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_internal_display(void);
