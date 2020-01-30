
/*-------------------------------------------------------------*/
typedef struct netwib_thread_cond netwib_thread_cond;

/*-------------------------------------------------------------*/
/* Name : netwib_thread_cond_init
   Description :
     Initialize a condition.
   Input parameter(s) :
   Input/output parameter(s) :
     *ppcond : netwib_thread_cond initialized
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_thread_cond_init(netwib_thread_cond **ppcond);

/*-------------------------------------------------------------*/
/* Name : netwib_thread_cond_close
   Description :
     Close a condition.
   Input parameter(s) :
   Input/output parameter(s) :
     *ppcond : netwib_thread_cond closed
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_thread_cond_close(netwib_thread_cond **ppcond);

/*-------------------------------------------------------------*/
/* Name : netwib_thread_cond_broadcast
   Description :
     Inform every waiting thread.
   Input parameter(s) :
     value : value to broadcast
   Input/output parameter(s) :
     *pcond : netwib_thread_cond
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_thread_cond_broadcast(netwib_thread_cond *pcond,
                                        netwib_uint32 value);

/*-------------------------------------------------------------*/
/* Name : netwib_thread_cond_wait
   Description :
     Wait for the condition.
   Input parameter(s) :
     *pcond : netwib_thread_cond
     *pabstime : end time. If *pabstime is reached, function
                 returns (*pevent set to NETWIB_FALSE).
   Input/output parameter(s) :
   Output parameter(s) :
     *pevent : condition reached
     *pvalue : value received
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_thread_cond_wait(netwib_thread_cond *pcond,
                                   netwib_consttime *pabstime,
                                   netwib_bool *pevent,
                                   netwib_uint32 *pvalue);

/*-------------------------------------------------------------*/
/* Name : netwib_thread_cond_reinit
   Description :
     Reset the condition. So netwib_thread_cond_broadcast
     can be used again.
   Input parameter(s) :
   Input/output parameter(s) :
     *pcond : netwib_thread_cond
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_thread_cond_reinit(netwib_thread_cond *pcond);
