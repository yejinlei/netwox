
/*-------------------------------------------------------------*/
typedef struct netwib_eths_index netwib_eths_index;

/*-------------------------------------------------------------*/
/* Name : netwib_eths_index_init
   Description :
     Initialize a netwib_eths_index used to loop through
     a netwib_eths.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     **ppethsindex : netwib_eths_index allocated and initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_eths_index_init(netwib_consteths *peths,
                                  netwib_eths_index **ppethsindex);

/*-------------------------------------------------------------*/
/* Name : netwib_eths_index_close
   Description :
     Close a netwib_ethsindex.
   Input parameter(s) :
   Input/output parameter(s) :
     **ppethsindex : netwib_eths_index closed
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_eths_index_close(netwib_eths_index **ppethsindex);

/*-------------------------------------------------------------*/
/* Types to control a netwib_eths_index */
typedef enum {
  NETWIB_ETHS_INDEX_CTLTYPE_REWIND = 1,  /* position at beginning */
  NETWIB_ETHS_INDEX_CTLTYPE_INDEX        /* reset with index pos */
} netwib_eths_index_ctltype;
/* Those functions permit to set/get parameters (pointer and
   integer) about a netwib_eths_index. It should not be used directly,
   but by the defines.
*/
netwib_err netwib_eths_index_ctl_set(netwib_eths_index *pethsindex,
                                     netwib_eths_index_ctltype type,
                                     netwib_ptr p,
                                     netwib_uint32 ui);
netwib_err netwib_eths_index_ctl_get(netwib_eths_index *pethsindex,
                                     netwib_eths_index_ctltype type,
                                     netwib_ptr p,
                                     netwib_uint32 *pui);

/*-------------------------------------------------------------*/
/* netwib_err f(netwib_eths_index *pethsindex); */
#define netwib_eths_index_ctl_set_rewind(pethsindex) netwib_eths_index_ctl_set(pethsindex,NETWIB_ETHS_INDEX_CTLTYPE_REWIND,NULL,0)
/* netwib_err f(netwib_eths_index *pethsindex,netwib_eths_index *pethsindexref);*/
#define netwib_eths_index_ctl_set_index(pethsindex,pethsindexref) netwib_eths_index_ctl_set(pethsindex,NETWIB_ETHS_INDEX_CTLTYPE_INDEX,pethsindexref,0)

/*-------------------------------------------------------------*/
/* Name : netwib_eths_index_next_eth
   Description :
     Get the next netwib_eth in the list.
   Input parameter(s) :
   Input/output parameter(s) :
     *pethsindex : netwib_eths_index
   Output parameter(s) :
     *peth : next netwib_eth
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_DATAEND : end of the list reached
   Note :
     If one thread changes the netwib_eths between two calls,
     the next result might be incorrect. Developer have to
     lock the entire loop (going through each item), or to
     copy the netwib_eths and to loop on the copy.
*/
netwib_err netwib_eths_index_next_eth(netwib_eths_index *pethsindex,
                                      netwib_eth *peth);
netwib_err netwib_eths_index_next_ethrange(netwib_eths_index *pethsindex,
                                           netwib_eth *pinfeth,
                                           netwib_eth *psupeth);

/*-------------------------------------------------------------*/
/* Name : netwib_eths_index_this_eth
   Description :
     Re-give the last netwib_eth in the list.
   Input parameter(s) :
   Input/output parameter(s) :
     *pethsindex : netwib_eths_index
   Output parameter(s) :
     *peth : next netwib_eth
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_eths_index_this_eth(netwib_eths_index *pethsindex,
                                      netwib_eth *peth);
netwib_err netwib_eths_index_this_ethrange(netwib_eths_index *pethsindex,
                                           netwib_eth *pinfeth,
                                           netwib_eth *psupeth);

/*-------------------------------------------------------------*/
/* Name : netwib_eths_index_this_del
   Description :
     Delete the last given value by a netwib_eths_index_next
     function.
   Input parameter(s) :
   Input/output parameter(s) :
     *pethsindex : netwib_eths_index
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_eths_index_this_del(netwib_eths_index *pethsindex);
