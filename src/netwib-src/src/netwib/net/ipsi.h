
/*-------------------------------------------------------------*/
typedef struct netwib_ips_index netwib_ips_index;

/*-------------------------------------------------------------*/
/* Name : netwib_ips_index_init
   Description :
     Initialize a netwib_ips_index used to loop through
     a netwib_ips.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     **ppipsindex : netwib_ips_index allocated and initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ips_index_init(netwib_constips *pips,
                                 netwib_ips_index **ppipsindex);

/*-------------------------------------------------------------*/
/* Name : netwib_ips_index_close
   Description :
     Close a netwib_ipsindex.
   Input parameter(s) :
   Input/output parameter(s) :
     **ppipsindex : netwib_ips_index closed
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ips_index_close(netwib_ips_index **ppipsindex);

/*-------------------------------------------------------------*/
/* Types to control a netwib_ips_index */
typedef enum {
  NETWIB_IPS_INDEX_CTLTYPE_REWIND = 1,  /* position at beginning */
  NETWIB_IPS_INDEX_CTLTYPE_INDEX        /* reset with index pos */
} netwib_ips_index_ctltype;
/* Those functions permit to set/get parameters (pointer and
   integer) about a netwib_ips_index. It should not be used directly,
   but by the defines.
*/
netwib_err netwib_ips_index_ctl_set(netwib_ips_index *pipsindex,
                                    netwib_ips_index_ctltype type,
                                    netwib_ptr p,
                                    netwib_uint32 ui);
netwib_err netwib_ips_index_ctl_get(netwib_ips_index *pipsindex,
                                    netwib_ips_index_ctltype type,
                                    netwib_ptr p,
                                    netwib_uint32 *pui);

/*-------------------------------------------------------------*/
/* netwib_err f(netwib_ips_index *pipsindex); */
#define netwib_ips_index_ctl_set_rewind(pipsindex) netwib_ips_index_ctl_set(pipsindex,NETWIB_IPS_INDEX_CTLTYPE_REWIND,NULL,0)
/* netwib_err f(netwib_ips_index *pipsindex,netwib_ips_index *pipsindexref);*/
#define netwib_ips_index_ctl_set_index(pipsindex,pipsindexref) netwib_ips_index_ctl_set(pipsindex,NETWIB_IPS_INDEX_CTLTYPE_INDEX,pipsindexref,0)

/*-------------------------------------------------------------*/
/* Name : netwib_ips_index_next_ip
   Description :
     Get the next netwib_ip in the list.
   Input parameter(s) :
   Input/output parameter(s) :
     *pipsindex : netwib_ips_index
   Output parameter(s) :
     *pip : next netwib_ip
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_DATAEND : end of the list reached
   Note :
     If one thread changes the netwib_ips between two calls,
     the next result might be incorrect. Developer have to
     lock the entire loop (going through each item), or to
     copy the netwib_ips and to loop on the copy.
*/
netwib_err netwib_ips_index_next_ip(netwib_ips_index *pipsindex,
                                    netwib_ip *pip);
netwib_err netwib_ips_index_next_iprange(netwib_ips_index *pipsindex,
                                         netwib_ip *pinfip,
                                         netwib_ip *psupip);

/*-------------------------------------------------------------*/
/* Name : netwib_ips_index_this_ip
   Description :
     Re-give the last netwib_ip in the list.
   Input parameter(s) :
   Input/output parameter(s) :
     *pipsindex : netwib_ips_index
   Output parameter(s) :
     *pip : next netwib_ip
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ips_index_this_ip(netwib_ips_index *pipsindex,
                                    netwib_ip *pip);
netwib_err netwib_ips_index_this_iprange(netwib_ips_index *pipsindex,
                                         netwib_ip *pinfip,
                                         netwib_ip *psupip);

/*-------------------------------------------------------------*/
/* Name : netwib_ips_index_this_del
   Description :
     Delete the last given value by a netwib_ips_index_next
     function.
   Input parameter(s) :
   Input/output parameter(s) :
     *pipsindex : netwib_ips_index
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ips_index_this_del(netwib_ips_index *pipsindex);
