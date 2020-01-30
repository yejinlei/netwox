
/*-------------------------------------------------------------*/
typedef struct {
  netwib_wait *pwait;
  netwib_uint32 waitident; /* because waitid exists */
} netwib_waitringitem;

/*-------------------------------------------------------------*/
/* Name : netwib_waitlist_init
   Description :
     Initialize a list of events. It can be use to wait for more
     than 5 event (otherwise use netwib_wait_wait5) which is
     easier.
   Input parameter(s) :
   Input/output parameter(s) :
     **ppring : ring initialized
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_waitlist_init(netwib_ring **ppring);

/*-------------------------------------------------------------*/
/* Name : netwib_waitlist_add
   Description :
     Add an event to the waiting ring.
   Input parameter(s) :
     waitident : id of the item to add (any value chosen by user)
     *pwait : netwib_wait to add
   Input/output parameter(s) :
     *pring : ring where to add items
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_waitlist_add(netwib_ring *pring,
                               netwib_wait *pwait,
                               netwib_uint32 waitident);

/*-------------------------------------------------------------*/
/* Name : netwib_waitlist_del
   Description :
     Remove an event to the waiting ring.
   Input parameter(s) :
     waitident : id of the item to remove
   Input/output parameter(s) :
     *pring : ring where to remove items
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_waitlist_del(netwib_ring *pring,
                               netwib_uint32 waitident);

/*-------------------------------------------------------------*/
/* Name : netwib_waitlist_wait
   Description :
     Wait for an event in the waiting ring. The netwib_wait
     is never closed by this function (different behavior
     compared to netwib_threadlist_wait).
   Input parameter(s) :
     *pabstime : end time
   Input/output parameter(s) :
     *pring : ring where to wait for
   Output parameter(s) :
     *pevent : true if an event occurred. If *pabstime is
               reached *pevent is set to NETWIB_FALSE.
     *pringofwaitid : ring of all events id
                      This ring contains netwib_uintptr
                      cast-ed to netwib_ptr. This ring is
                      initialized only when there is an event.
                      It's user's job to close this ring with :
                  netwib_ring_close(ppringofwaitid, NETWIB_FALSE)
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_DATAEND: ring is empty
*/
netwib_err netwib_waitlist_wait(netwib_ring *pring,
                                netwib_consttime *pabstime,
                                netwib_bool *pevent,
                                netwib_ring **ppringofwaitid);
#define netwib_waitlist_waitident_init_ptr(p) ((netwib_uint32)(netwib_uintptr)(p))

/*-------------------------------------------------------------*/
/***************************************************************
 * For other functions, you can directly use functions of      *
 * ring.h.                                                     *
 * To do so, booleans "eraseitems" and "duplicateitems" have   *
 * be set to NETWIB_TRUE. See netwib_waitlist_close for        *
 * example.                                                    *
 ***************************************************************/

#define netwib_waitlist_close(ppring) netwib_ring_close(ppring,NETWIB_TRUE)
