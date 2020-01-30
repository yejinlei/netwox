
/*-------------------------------------------------------------*/
typedef struct netwib_thread_mutex netwib_thread_mutex;

/*-------------------------------------------------------------*/
/* Name : netwib_thread_mutex_init
   Description :
     Initialize a mutex.
   Input parameter(s) :
   Input/output parameter(s) :
     *ppmutex : netwib_thread_mutex initialized
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_thread_mutex_init(netwib_thread_mutex **ppmutex);

/*-------------------------------------------------------------*/
/* Name : netwib_thread_mutex_close
   Description :
     Close a mutex.
   Input parameter(s) :
   Input/output parameter(s) :
     *ppmutex : netwib_thread_mutex closed
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_thread_mutex_close(netwib_thread_mutex **ppmutex);

/*-------------------------------------------------------------*/
/* Name : netwib_thread_mutex_lock
   Description :
     Wait for the mutex.
   Input parameter(s) :
     *pmutex : netwib_thread_mutex
     *pabstime : end time. If *pabstime is reached, locking is
                 not done (*plocked set to NETWIB_FALSE).
   Input/output parameter(s) :
   Output parameter(s) :
     *plocked : the mutex could be locked
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_thread_mutex_lock(netwib_thread_mutex *pmutex,
                                    netwib_consttime *pabstime,
                                    netwib_bool *plocked);

/*-------------------------------------------------------------*/
/* Name : netwib_thread_mutex_unlock
   Description :
     Unlock a mutex.
   Input parameter(s) :
   Input/output parameter(s) :
     *pmutex : netwib_thread_mutex to unlock
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_thread_mutex_unlock(netwib_thread_mutex *pmutex);

