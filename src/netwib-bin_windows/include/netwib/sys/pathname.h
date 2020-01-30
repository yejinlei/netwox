
/*-------------------------------------------------------------*/
/* Name : netwib_pathname_exists
   Description :
     Check if a path exists.
   Input parameter(s) :
     *ppathname : pathname
   Input/output parameter(s) :
   Output parameter(s) :
     *pyes : if path exists
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_pathname_exists(netwib_constbuf *ppathname,
                                  netwib_bool *pyes);

