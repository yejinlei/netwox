
/*-------------------------------------------------------------*/
/***************************************************************
 * A netwib_dir permits to loop through all filenames in a     *
 * directory.                                                  *
 ***************************************************************/

/*-------------------------------------------------------------*/
typedef struct netwib_dir netwib_dir;

/*-------------------------------------------------------------*/
/* Name : netwib_dir_init
   Description :
     Open a directory.
   Input parameter(s) :
     *pdirname : directory name
   Input/output parameter(s) :
   Output parameter(s) :
     **ppdir : netwib_dir initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_dir_init(netwib_constbuf *pdirname,
                           netwib_dir **ppdir);

/*-------------------------------------------------------------*/
/* Name : netwib_dir_close
   Description :
     Close a netwib_dir.
   Input parameter(s) :
   Input/output parameter(s) :
     **ppdir : netwib_dir closed
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_dir_close(netwib_dir **ppdir);

/*-------------------------------------------------------------*/
/* Types to control a netwib_dir */
typedef enum {
  NETWIB_DIR_CTLTYPE_REWIND = 1        /* position at beginning */
} netwib_dir_ctltype;
netwib_err netwib_dir_ctl_set(netwib_dir *pdir,
                              netwib_dir_ctltype type,
                              netwib_ptr p,
                              netwib_uint32 ui);
netwib_err netwib_dir_ctl_get(netwib_dir *pdir,
                              netwib_dir_ctltype type,
                              netwib_ptr p,
                              netwib_uint32 *pui);

/*-------------------------------------------------------------*/
/* netwib_err f(netwib_dir *pdir); */
#define netwib_dir_ctl_set_rewind(pdir) netwib_dir_ctl_set(pdir,NETWIB_DIR_CTLTYPE_REWIND,NULL,0)

/*-------------------------------------------------------------*/
/* Name : netwib_dir_next
   Description :
     Obtain the next file of a netwib_dir.
   Input parameter(s) :
   Input/output parameter(s) :
     **ppdir : netwib_dir
     pbuffilename : buffer containing the filename
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
   Notes :
     If directory files change between two calls,
     the next result might be incorrect, but it will not crash.
*/
netwib_err netwib_dir_next(netwib_dir *pdir,
                           netwib_buf *pbuffilename);
