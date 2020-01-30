
/*-------------------------------------------------------------*/
/***************************************************************
 * NOTE ABOUT SECURITY UNDER UNIX                              *
 * The only way to deal securely with a file/dir is to store   *
 * it in a secure directory (every dir in the path should only *
 * be writable by root or current user).                       *
 * Otherwise a malicious user can create a symlink and force   *
 * corruption/creation/removing of a file/dir.                 *
 * So, to secure your program, you should force user to work   *
 * only in secured directories. Function netwib_dirname_secure *
 * permits to check if a directory is secure. Note that netwib *
 * functions do not check for secure directories internally :  *
 * you should do it yourself.                                  *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* Name : netwib_dirname_secure
   Description :
     Check if a directory is secure.
   Input parameter(s) :
     *pdirname : name of the directory
   Input/output parameter(s) :
   Output parameter(s) :
     *pyes : true if directory is secure
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_dirname_secure(netwib_constbuf *pdirname,
                                 netwib_bool *pyes);

/*-------------------------------------------------------------*/
/* Name : netwib_dirname_cwd
   Description :
     Get current working directory.
   Input parameter(s) :
   Input/output parameter(s) :
     *pdirname : name of the directory
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_dirname_cwd(netwib_buf *pdirname);

/*-------------------------------------------------------------*/
/* Name : netwib_dirname_exists
   Description :
     Check if a directory exist.
   Input parameter(s) :
     *pdirname : name of the directory
   Input/output parameter(s) :
   Output parameter(s) :
     *pyes : true if directory exists
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_dirname_exists(netwib_constbuf *pdirname,
                                 netwib_bool *pyes);

/*-------------------------------------------------------------*/
/* Name : netwib_dirname_create
   Description :
     Create a directory.
     Warning : this function is not secured against
               symlink races
   Input parameter(s) :
     *pdirname : name of the directory
   Input/output parameter(s) :
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_dirname_create(netwib_constbuf *pdirname);

/*-------------------------------------------------------------*/
/* Name : netwib_dirname_remove
   Description :
     Remove a directory.
   Input parameter(s) :
     *pdirname : name of the directory
   Input/output parameter(s) :
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_dirname_remove(netwib_constbuf *pdirname);

/*-------------------------------------------------------------*/
/* Name : netwib_dirname_rename
   Description :
     Rename a directory.
   Input parameter(s) :
     *polddirname : previous name of the directory
     *pnewdirname : new name for the directory
   Input/output parameter(s) :
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_dirname_rename(netwib_constbuf *polddirname,
                                 netwib_constbuf *pnewdirname);

