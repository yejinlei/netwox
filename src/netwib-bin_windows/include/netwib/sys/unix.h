
/*-------------------------------------------------------------*/
/* Name : netwib_unix_symlink
   Description :
     Create a symlink plinkname -> ppathname
   Input parameter(s) :
     ppathname : pointed path
     plinkname : link
   Input/output parameter(s) :
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_unix_symlink(netwib_constbuf *ppathname,
                               netwib_constbuf *plinkname);

/*-------------------------------------------------------------*/
/* Name : netwib_unix_readlink
   Description :
     Read a symlink value
   Input parameter(s) :
     plinkname : link
   Input/output parameter(s) :
     ppathname : stored/pointed path
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_unix_readlink(netwib_constbuf *plinkname,
                                netwib_buf *ppathname);
