
/*-------------------------------------------------------------*/
typedef struct netwib_thread netwib_thread;

/*-------------------------------------------------------------*/
/* thread ends when this function returns */
typedef netwib_err (*netwib_thread_pf)(netwib_ptr infosin,
                                       netwib_ptr *pinfosout);

/*-------------------------------------------------------------*/
/* Name : netwib_thread_init
   Description :
     Create a new thread, which will execute *pfunc and exit.
   Input parameter(s) :
     *pfunc : function executed by the new thread :
              infosin : set with infosin
              pinfosout : eventually set by user
     infosin : data to pass to the thread
   Input/output parameter(s) :
   Output parameter(s) :
     **ppthread : thread created
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_thread_init(netwib_thread_pf pfunc,
                              netwib_ptr infosin,
                              netwib_thread **ppthread);

/*-------------------------------------------------------------*/
/* Name : netwib_thread_close
   Description :
     Free memory allocated by a netwib_thread.
     Note : this function does not terminate the thread.
            function netwib_thread_wait must be called before
   Input parameter(s) :
   Input/output parameter(s) :
     **ppthread : thread to free
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_thread_close(netwib_thread **ppthead);

/*-------------------------------------------------------------*/
/* Name : netwib_thread_wait
   Description :
     Wait for the end of the thread.
   Input parameter(s) :
     *pthread : thread to wait for
     *pabstime : end time. If *pabstime is reached, function
                 returns (*pevent set to NETWIB_FALSE).
   Input/output parameter(s) :
   Output parameter(s) :
     *pevent : true if thread ended
     *preturnederror : value returned by the thread
     *pinfosout : info eventually set by thread
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_thread_wait(netwib_thread *pthread,
                              netwib_consttime *pabstime,
                              netwib_bool *pevent,
                              netwib_err *preturnederror,
                              netwib_ptr *pinfosout);
