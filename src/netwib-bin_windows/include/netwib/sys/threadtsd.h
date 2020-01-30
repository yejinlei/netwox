
/*-------------------------------------------------------------*/
/***************************************************************
 * TSD : Thread Specific Data                                  *
 ***************************************************************/

/*-------------------------------------------------------------*/
typedef struct netwib_thread_tsd netwib_thread_tsd;

/*-------------------------------------------------------------*/
/* Name : netwib_thread_tsd_init
   Description :
     Initialize a tsd.
   Input parameter(s) :
   Input/output parameter(s) :
     *pptsd : netwib_thread_tsd initialized
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_thread_tsd_init(netwib_thread_tsd **pptsd);

/*-------------------------------------------------------------*/
/* Name : netwib_thread_tsd_close
   Description :
     Close a tsd.
   Input parameter(s) :
   Input/output parameter(s) :
     *pptsd : netwib_thread_tsd closed
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_thread_tsd_close(netwib_thread_tsd **pptsd);

/*-------------------------------------------------------------*/
/* Types to control a netwib_thread_tsd */
typedef enum {
  NETWIB_THREAD_TSD_CTLTYPE_VALUE = 1   /* get/set value */
} netwib_thread_tsd_ctltype;
netwib_err netwib_thread_tsd_ctl_set(netwib_thread_tsd *ptsd,
                                     netwib_thread_tsd_ctltype type,
                                     netwib_ptr p,
                                     netwib_uint32 ui);
netwib_err netwib_thread_tsd_ctl_get(netwib_thread_tsd *ptsd,
                                     netwib_thread_tsd_ctltype type,
                                     netwib_ptr p,
                                     netwib_uint32 *pui);

/*-------------------------------------------------------------*/
/* netwib_err f(netwib_thread_tsd *pthread_tsd, netwib_ptr ptr); */
#define netwib_thread_tsd_ctl_set_value(pthread_tsd,ptr) netwib_thread_tsd_ctl_set(pthread_tsd,NETWIB_THREAD_TSD_CTLTYPE_VALUE,ptr,0)
/* netwib_err f(netwib_thread_tsd *pthread_tsd, netwib_ptr ptr); */
#define netwib_thread_tsd_ctl_get_value(pthread_tsd,ptr) netwib_thread_tsd_ctl_get(pthread_tsd,NETWIB_THREAD_TSD_CTLTYPE_VALUE,ptr,0)
