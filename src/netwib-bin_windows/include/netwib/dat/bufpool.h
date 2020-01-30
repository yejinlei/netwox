
/*-------------------------------------------------------------*/
/***************************************************************
 * A netwib_bufpool permits to obtain and release memory,      *
 * without having to malloc and free it : this is done once.   *
 * This is mainly advantageous for programs needing to allocate*
 * and free frequently. For programs allocating memory and then*
 * only freeing it at the end, function netwib_buf_init_malloc *
 * is more appropriated.                                       *
 ***************************************************************/

/*-------------------------------------------------------------*/
typedef struct netwib_bufpool netwib_bufpool;

/*-------------------------------------------------------------*/
/* Name : netwib_bufpool_init
   Description :
     Initialize a netwib_bufpool.
   Input parameter(s) :
     mt : for multithread access
   Input/output parameter(s) :
     **ppbufpool : bufpool initialized
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_bufpool_init(netwib_bool mt,
                               netwib_bufpool **ppbufpool);
#define netwib_bufpool_initdefault(ppbufpool) netwib_bufpool_init(NETWIB_FALSE,ppbufpool)

/*-------------------------------------------------------------*/
/* Name : netwib_bufpool_close
   Description :
     Close a netwib_bufpool.
   Input parameter(s) :
   Input/output parameter(s) :
     **ppbufpool : bufpool closed
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_bufpool_close(netwib_bufpool **ppbufpool);

/*-------------------------------------------------------------*/
/* Name : netwib_bufpool_buf_init
   Description :
     Give a buffer pointer to user.
   Input parameter(s) :
   Input/output parameter(s) :
     *pbufpool : bufpool
   Output parameter(s) :
     *ppbuf : pointer obtained
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_bufpool_buf_init(netwib_bufpool *pbufpool,
                                   netwib_buf **ppbuf);

/*-------------------------------------------------------------*/
/* Name : netwib_bufpool_buf_close
   Description :
     The user indicates he does not need the buffer anymore.
   Input parameter(s) :
   Input/output parameter(s) :
     *pbufpool : bufpool
     *ppbuf : pointer to close
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_bufpool_buf_close(netwib_bufpool *pbufpool,
                                    netwib_buf **ppbuf);



