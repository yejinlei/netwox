
/*-------------------------------------------------------------*/
/***************************************************************
 * Those functions deals with several threads, referenced      *
 * by their id.                                                *
 ***************************************************************/

/*-------------------------------------------------------------*/
typedef struct {
  netwib_thread *pthread;
  netwib_uint32 threadid;
} netwib_threadringitem;

/*-------------------------------------------------------------*/
/* Name : netwib_threadlist_init
   Description :
     Create a new threadlist.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     **ppring : netwib_ring initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_threadlist_init(netwib_ring **ppring);

/*-------------------------------------------------------------*/
/* Name : netwib_threadlist_close
   Description :
     Wait for threads in the list (this can be infinite if one
     thread never ends). If one thread returns on error, it is
     ignored. Then, close the threadlist.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     **ppring : netwib_ring to close
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_threadlist_close(netwib_ring **ppring);

/*-------------------------------------------------------------*/
/* Name : netwib_threadlist_add
   Description :
     Add a new thread.
   Input parameter(s) :
     *pthread : thread to add
     threadid : id of the newly added thread (any value decided
                by user)
   Input/output parameter(s) :
     *pring : netwib_ring where to add the thread
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_threadlist_add(netwib_ring *pring,
                                 netwib_thread *pthread,
                                 netwib_uint32 threadid);

/*-------------------------------------------------------------*/
/* Name : netwib_threadlist_del
   Description :
     Remove a thread from the ring.
   Input parameter(s) :
     threadid : id of the thread to remove
   Input/output parameter(s) :
     *pring : netwib_ring where to remove the thread
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_threadlist_del(netwib_ring *pring,
                                 netwib_uint32 threadid);

/*-------------------------------------------------------------*/
/* Name : netwib_threadlist_wait
   Description :
     Wait for the end of one thread. When this occurs, the
     netwib_thread associated is closed (use netwib_threadlist_del
     to remove a thread from the list : you can wait and close
     it yourself).
   Input parameter(s) :
     *pring : netwib_ring containing threads
     *pabstime : end time. If *pabstime is reached, function
                 returns (*pevent set to NETWIB_FALSE).
   Input/output parameter(s) :
   Output parameter(s) :
     *pevent : a thread finished
     *pthreadid : id of the thread
     *preturnederror : value returned by the thread
     *pinfosout : info eventually set by thread
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_DATAEND : there is no more thread in the ring
*/
netwib_err netwib_threadlist_wait(netwib_ring *pring,
                                  netwib_consttime *pabstime,
                                  netwib_bool *pevent,
                                  netwib_uint32 *pthreadid,
                                  netwib_err *preturnederror,
                                  netwib_ptr *pinfosout);

/*-------------------------------------------------------------*/
/***************************************************************
 * For other functions, you can directly use functions of      *
 * ring.h.                                                     *
 * To do so, booleans "eraseitems" and "duplicateitems" have   *
 * be set to NETWIB_TRUE. See netwib_threadlist_close for      *
 * example.                                                    *
 ***************************************************************/
