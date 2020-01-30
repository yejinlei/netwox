
/*-------------------------------------------------------------*/
typedef struct netwib_thread_rwlock netwib_thread_rwlock;

/*-------------------------------------------------------------*/
/* Name : netwib_thread_rwlock_init
   Description :
     Initialize a rwlock.
   Input parameter(s) :
   Input/output parameter(s) :
     *pprwlock : netwib_thread_rwlock initialized
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_thread_rwlock_init(netwib_thread_rwlock **pprwlock);

/*-------------------------------------------------------------*/
/* Name : netwib_thread_rwlock_close
   Description :
     Close a rwlock.
   Input parameter(s) :
   Input/output parameter(s) :
     *pprwlock : netwib_thread_rwlock closed
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_thread_rwlock_close(netwib_thread_rwlock **pprwlock);

/*-------------------------------------------------------------*/
/* Name : netwib_thread_rwlock_rdlock
   Description :
     Wait for the rwlock.
     We lock for reading.
   Input parameter(s) :
     *prwlock : netwib_thread_rwlock
     *pabstime : end time. If *pabstime is reached, locking is
                 not done (*plocked set to NETWIB_FALSE).
   Input/output parameter(s) :
   Output parameter(s) :
     *plocked : the rwlock could be locked
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_thread_rwlock_rdlock(netwib_thread_rwlock *prwlock,
                                       netwib_consttime *pabstime,
                                       netwib_bool *plocked);

/*-------------------------------------------------------------*/
/* Name : netwib_thread_rwlock_rdunlock
   Description :
     Unlock reading of a rwlock.
   Input parameter(s) :
   Input/output parameter(s) :
     *prwlock : netwib_thread_rwlock to unlock
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_thread_rwlock_rdunlock(netwib_thread_rwlock *prwlock);

/*-------------------------------------------------------------*/
/* Name : netwib_thread_rwlock_wrlock
   Description :
     Wait for the rwlock.
     We lock for writing.
   Input parameter(s) :
     *prwlock : netwib_thread_rwlock
     *pabstime : end time. If *pabstime is reached, locking is
                 not done (*plocked set to NETWIB_FALSE).
   Input/output parameter(s) :
   Output parameter(s) :
     *plocked : the rwlock could be locked
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_thread_rwlock_wrlock(netwib_thread_rwlock *prwlock,
                                       netwib_consttime *pabstime,
                                       netwib_bool *plocked);

/*-------------------------------------------------------------*/
/* Name : netwib_thread_rwlock_wrunlock
   Description :
     Unlock writing of a rwlock.
   Input parameter(s) :
   Input/output parameter(s) :
     *prwlock : netwib_thread_rwlock to unlock
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_thread_rwlock_wrunlock(netwib_thread_rwlock *prwlock);
