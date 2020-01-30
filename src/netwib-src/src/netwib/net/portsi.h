
/*-------------------------------------------------------------*/
typedef struct netwib_ports_index netwib_ports_index;

/*-------------------------------------------------------------*/
/* Name : netwib_ports_index_init
   Description :
     Initialize a netwib_ports_index used to loop through
     a netwib_ports.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     **ppportsindex : netwib_ports_index allocated and initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ports_index_init(netwib_constports *pports,
                                   netwib_ports_index **ppportsindex);

/*-------------------------------------------------------------*/
/* Name : netwib_ports_index_close
   Description :
     Close a netwib_portsindex.
   Input parameter(s) :
   Input/output parameter(s) :
     **ppportsindex : netwib_ports_index closed
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ports_index_close(netwib_ports_index **ppportsindex);

/*-------------------------------------------------------------*/
/* Types to control a netwib_ports_index */
typedef enum {
  NETWIB_PORTS_INDEX_CTLTYPE_REWIND = 1,  /* position at beginning */
  NETWIB_PORTS_INDEX_CTLTYPE_INDEX        /* reset with index pos */
} netwib_ports_index_ctltype;
/* Those functions permit to set/get parameters (pointer and
   integer) about a netwib_ports_index. It should not be used directly,
   but by the defines.
*/
netwib_err netwib_ports_index_ctl_set(netwib_ports_index *pportsindex,
                                      netwib_ports_index_ctltype type,
                                      netwib_ptr p,
                                      netwib_uint32 ui);
netwib_err netwib_ports_index_ctl_get(netwib_ports_index *pportsindex,
                                      netwib_ports_index_ctltype type,
                                      netwib_ptr p,
                                      netwib_uint32 *pui);

/*-------------------------------------------------------------*/
/* netwib_err f(netwib_ports_index *pportsindex); */
#define netwib_ports_index_ctl_set_rewind(pportsindex) netwib_ports_index_ctl_set(pportsindex,NETWIB_PORTS_INDEX_CTLTYPE_REWIND,NULL,0)
/* netwib_err f(netwib_ports_index *pportsindex,netwib_ports_index *pportsindexref);*/
#define netwib_ports_index_ctl_set_index(pportsindex,pportsindexref) netwib_ports_index_ctl_set(pportsindex,NETWIB_PORTS_INDEX_CTLTYPE_INDEX,pportsindexref,0)

/*-------------------------------------------------------------*/
/* Name : netwib_ports_index_next_port
   Description :
     Get the next netwib_port in the list.
   Input parameter(s) :
   Input/output parameter(s) :
     *pportsindex : netwib_ports_index
   Output parameter(s) :
     *pport : next netwib_port
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_DATAEND : end of the list reached
   Note :
     If one thread changes the netwib_ports between two calls,
     the next result might be incorrect. Developer have to
     lock the entire loop (going through each item), or to
     copy the netwib_ports and to loop on the copy.
*/
netwib_err netwib_ports_index_next_port(netwib_ports_index *pportsindex,
                                        netwib_port *pport);
netwib_err netwib_ports_index_next_portrange(netwib_ports_index *pportsindex,
                                             netwib_port *pinfport,
                                             netwib_port *psupport);

/*-------------------------------------------------------------*/
/* Name : netwib_ports_index_this_port
   Description :
     Re-give the last netwib_port in the list.
   Input parameter(s) :
   Input/output parameter(s) :
     *pportsindex : netwib_ports_index
   Output parameter(s) :
     *pport : next netwib_port
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ports_index_this_port(netwib_ports_index *pportsindex,
                                        netwib_port *pport);
netwib_err netwib_ports_index_this_portrange(netwib_ports_index *pportsindex,
                                             netwib_port *pinfport,
                                             netwib_port *psupport);

/*-------------------------------------------------------------*/
/* Name : netwib_ports_index_this_del
   Description :
     Delete the last given value by a netwib_ports_index_next
     function.
   Input parameter(s) :
   Input/output parameter(s) :
     *pportsindex : netwib_ports_index
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ports_index_this_del(netwib_ports_index *pportsindex);
