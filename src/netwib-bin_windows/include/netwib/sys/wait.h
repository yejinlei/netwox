
/*-------------------------------------------------------------*/
/***************************************************************
 * A netwib_wait permits to wait for an event during a user    *
 * defined duration.                                           *
 ***************************************************************/

/*-------------------------------------------------------------*/
typedef struct netwib_wait netwib_wait;

/*-------------------------------------------------------------*/
/* Name : netwib_wait_init
   Description :
     Initialize a netwib_wait : wait for an event decided by a
     function.
   Input parameter(s) :
     pfuncevent : memory address of the function which will
                  be called to check for an event
                  For each call, the first parameter ('infos')
                  will be set with the optional parameter below.
     pfuncclose : optional parameter (can be NULL) which
                  contain the function to call to free
                  resources allocated by infos (when
                  netwib_wait_close will be called)
   Input/output parameter(s) :
     infos : optional parameter (can be NULL) which will be
             used as the first parameter for *pfunc.
             This may be used to send information to *pfunc.
   Output parameter(s) :
     **ppwait : netwib_wait associated to function
   Normal return values :
     NETWIB_ERR_OK : ok
*/
typedef netwib_err (*netwib_wait_event_pf)(netwib_ptr infos,
                                           netwib_consttime *pabstime,
                                           netwib_bool *pevent);
typedef netwib_err (*netwib_wait_close_pf)(netwib_ptr infos);
netwib_err netwib_wait_init(netwib_wait_event_pf pfuncevent,
                            netwib_ptr infos,
                            netwib_wait_close_pf pfuncclose,
                            netwib_wait **ppwait);

/*-------------------------------------------------------------*/
/* Name : netwib_wait_close
   Description :
     Close a netwib_wait.
   Input parameter(s) :
   Input/output parameter(s) :
     **ppwait : netwib_wait to close
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_wait_close(netwib_wait **ppwait);

/*-------------------------------------------------------------*/
/* Name : netwib_wait_init_io
   Description :
     Initialize a netwib_wait : wait for data from the netwib_io.
   Input parameter(s) :
     *pio : netwib_io where to wait for data
   Input/output parameter(s) :
   Output parameter(s) :
     **ppwait : netwib_wait associated to *pio
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_wait_init_io(netwib_io *pio,
                               netwib_io_waytype way,
                               netwib_wait **ppwait);
/* netwib_err f(netwib_io *pio, netwib_wait **ppwait); */
#define netwib_wait_init_io_read(pio,ppwait) netwib_wait_init_io(pio,NETWIB_IO_WAYTYPE_READ,ppwait)
#define netwib_wait_init_io_write(pio,ppwait) netwib_wait_init_io(pio,NETWIB_IO_WAYTYPE_WRITE,ppwait)
#define netwib_wait_init_io_rdwr(pio,ppwait) netwib_wait_init_io(pio,NETWIB_IO_WAYTYPE_RDWR,ppwait)

/*-------------------------------------------------------------*/
/* Name : netwib_wait_init_thread_end
   Description :
     Initialize a netwib_wait : wait for the end of a thread.
   Input parameter(s) :
     *pthread : thread to wait for
   Input/output parameter(s) :
     preturnederror : address of a variable which will contain
                      returned error
     pinfosout : address of a variable which will contain
                 output information
   Output parameter(s) :
     *ppwait : netwib_wait associated to *pthread
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_wait_init_thread_end(netwib_thread *pthread,
                                       netwib_err *preturnederror,
                                       netwib_ptr *pinfosout,
                                       netwib_wait **ppwait);

/*-------------------------------------------------------------*/
/* Name : netwib_wait_init_thread_cond
   Description :
     Initialize a netwib_wait : wait for a condition
   Input parameter(s) :
     *pcond : condition to wait for
   Input/output parameter(s) :
     *pvalue : address of a variable which will contain
               condition value
   Output parameter(s) :
     *ppwait : netwib_wait associated to *pcond
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_wait_init_thread_cond(netwib_thread_cond *pcond,
                                        netwib_uint32 *pvalue,
                                        netwib_wait **ppwait);

/*-------------------------------------------------------------*/
/* Name : netwib_wait_wait1
   Description :
     Wait for 1 event.
   Input parameter(s) :
     *pwait : netwib_wait to wait for
     *pabstime : end time
   Input/output parameter(s) :
   Output parameter(s) :
     *pevent : an event occurred on *pwait. If *pabstime is
               reached *pevent is set to NETWIB_FALSE.
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_wait_wait1(netwib_wait *pwait,
                             netwib_consttime *pabstime,
                             netwib_bool *pevent);

/*-------------------------------------------------------------*/
/* Name : netwib_wait_wait5
   Description :
     Wait for 1 event amongst 5 netwib_wait.
   Input parameter(s) :
     *pwait1..5 : netwib_wait to wait for
     *pabstime : end time
   Input/output parameter(s) :
   Output parameter(s) :
     *pevent1..5 : an event occurred on *pwait1..5
                   If abstime is reached *pevent is set
                   to NETWIB_FALSE.
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_wait_wait5(netwib_wait *pwait1,
                             netwib_wait *pwait2,
                             netwib_wait *pwait3,
                             netwib_wait *pwait4,
                             netwib_wait *pwait5,
                             netwib_consttime *pabstime,
                             netwib_bool *pevent1,
                             netwib_bool *pevent2,
                             netwib_bool *pevent3,
                             netwib_bool *pevent4,
                             netwib_bool *pevent5);
#define netwib_wait_wait4(pwait1,pwait2,pwait3,pwait4,pabstime,pevent1,pevent2,pevent3,pevent4) netwib_wait_wait5(pwait1,pwait2,pwait3,pwait4,NULL,pabstime,pevent1,pevent2,pevent3,pevent4,NULL)
#define netwib_wait_wait3(pwait1,pwait2,pwait3,pabstime,pevent1,pevent2,pevent3) netwib_wait_wait5(pwait1,pwait2,pwait3,NULL,NULL,pabstime,pevent1,pevent2,pevent3,NULL,NULL)
#define netwib_wait_wait2(pwait1,pwait2,pabstime,pevent1,pevent2) netwib_wait_wait5(pwait1,pwait2,NULL,NULL,NULL,pabstime,pevent1,pevent2,NULL,NULL,NULL)
