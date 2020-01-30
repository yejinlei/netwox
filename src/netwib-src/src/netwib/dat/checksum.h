
/*-------------------------------------------------------------*/
/* Name : netwib_checksum_buf
   Description :
     Compute the checksum of a buffer.
   Input parameter(s) :
     *pbuf : buffer input data
   Input/output parameter(s) :
   Output parameter(s) :
     *pchecksum : checksum of data stored in *pbuf.
                  Note : this checksum is in host byte order
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_checksum_buf(netwib_constbuf *pbuf,
                               netwib_uint16 *pchecksum);

/*-------------------------------------------------------------*/
netwib_err netwib_checksum_init(netwib_uint32 *ptmpchecksum);
netwib_err netwib_checksum_update_buf(netwib_constbuf *pbuf,
                                      netwib_uint32 *ptmpchecksum);
netwib_err netwib_checksum_update_data(netwib_constdata data,
                                       netwib_uint32 datasize,
                                       netwib_uint32 *ptmpchecksum);
netwib_err netwib_checksum_close(netwib_uint32 tmpchecksum,
                                 netwib_uint16 *pchecksum);
