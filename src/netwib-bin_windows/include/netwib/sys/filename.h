
/*-------------------------------------------------------------*/
/* Name : netwib_filename_exists
   Description :
     Check if a regular file exists.
   Input parameter(s) :
     *pfilename : filename
   Input/output parameter(s) :
   Output parameter(s) :
     *pyes : if file exists
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_filename_exists(netwib_constbuf *pfilename,
                                  netwib_bool *pyes);

/*-------------------------------------------------------------*/
/* Name : netwib_filename_size
   Description :
     Get size of a file.
   Input parameter(s) :
     *pfilename : filename
   Input/output parameter(s) :
   Output parameter(s) :
     *psize : size of the file
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_NOTFOUND : file was not found
   Note :
     On a large filesystem, if file size is greater than
     2G, this function returns the error NETWIB_ERR_PAFILE2G.
     Use netwib_pathstat_init instead.
*/
netwib_err netwib_filename_size(netwib_constbuf *pfilename,
                                netwib_uint32 *psize);

/*-------------------------------------------------------------*/
/* Name : netwib_filename_create
   Description :
     Create an empty file. If it exists, truncate its contents.
   Input parameter(s) :
     *pfilename : filename
   Input/output parameter(s) :
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_filename_create(netwib_constbuf *pfilename);

/*-------------------------------------------------------------*/
/* Name : netwib_filename_remove
   Description :
     Remove a file.
   Input parameter(s) :
     *pfilename : filename
   Input/output parameter(s) :
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_NOTFOUND : file was not found
*/
netwib_err netwib_filename_remove(netwib_constbuf *pfilename);

/*-------------------------------------------------------------*/
/* Name : netwib_filename_rename
   Description :
     Rename a file.
   Input parameter(s) :
     *poldfilename : old filename
     *pnewfilename : new filename
   Input/output parameter(s) :
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_NOTFOUND : file was not found
*/
netwib_err netwib_filename_rename(netwib_constbuf *poldfilename,
                                  netwib_constbuf *pnewfilename);

/*-------------------------------------------------------------*/
/* Name : netwib_filename_copy
   Description :
     Copy a file.
   Input parameter(s) :
     *poldfilename : old filename
     *pnewfilename : new filename
   Input/output parameter(s) :
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_NOTFOUND : file was not found
*/
netwib_err netwib_filename_copy(netwib_constbuf *poldfilename,
                                netwib_constbuf *pnewfilename);
