
/*-------------------------------------------------------------*/
/***************************************************************
 * A netwib_ring_index is used to loop through a ring.         *
 *                                                             *
 * When the ring is only read, several indexes                 *
 * can simultaneously loop through the ring.                   *
 *                                                             *
 * However, when the ring is edited, strange behavior can      *
 * occur when a netwib_ring_index_xyz function is called,      *
 * depending on function used to edit the ring:                *
 *  - netwib_ring_index_next,                                  *
 *    netwib_ring_index_previous,                              *
 *    netwib_ring_index_this_value,                            *
 *    netwib_ring_index_this_replace,                          *
 *    netwib_ring_index_this_del:                              *
 *      No problem.                                            *
 *  - netwib_ring_index_add_before,                            *
 *    netwib_ring_index_add_after,                             *
 *    netwib_ring_add_first,                                   *
 *    netwib_ring_add_last,                                    *
 *    netwib_ring_index_add_ring_xyz:                          *
 *      The added item(s) may be returned or not returned,     *
 *      depending on the insertion position relative to the    *
 *      index position.                                        *
 *  - netwib_ring_sort,                                        *
 *    netwib_ring_group:                                       *
 *      The reordered items may be returned twice or never     *
 *      returned. There is no way to know.                     *
 *  - netwib_ring_del_criteria,                                *
 *    netwib_ring_del_duplicate:                               *
 *      You should not use them because if the item pointed by *
 *      the index is deleted, it will cause an access to freed *
 *      memory. You may use them only if you are sure to not   *
 *      delete the current item.                               *
 *      However, after netwib_ring_index_this_del, you must    *
 *      not use them because there is not way to know if they  *
 *      will delete internal items.                            *
 ***************************************************************/

/*-------------------------------------------------------------*/
typedef struct netwib_ring_index netwib_ring_index;

/*-------------------------------------------------------------*/
/* Name : netwib_ring_index_init
   Description :
     Initialize a netwib_ring_index used to loop through
     a netwib_ring.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     **ppringindex : netwib_ring_index allocated and initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ring_index_init(netwib_constring *pring,
                                  netwib_ring_index **ppringindex);

/*-------------------------------------------------------------*/
/* Name : netwib_ring_index_close
   Description :
     Close a netwib_ringindex.
   Input parameter(s) :
   Input/output parameter(s) :
     **ppringindex : netwib_ring_index closed
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ring_index_close(netwib_ring_index **ppringindex);

/*-------------------------------------------------------------*/
/* Types to control a netwib_ring_index */
typedef enum {
  NETWIB_RING_INDEX_CTLTYPE_REWIND = 1,  /* position at beginning */
  NETWIB_RING_INDEX_CTLTYPE_INDEX        /* reset with index pos */
} netwib_ring_index_ctltype;
/* Those functions permit to set/get parameters (pointer and
   integer) about a netwib_ring_index. It should not be used directly,
   but by the defines.
*/
netwib_err netwib_ring_index_ctl_set(netwib_ring_index *pringindex,
                                     netwib_ring_index_ctltype type,
                                     netwib_ptr p,
                                     netwib_uint32 ui);
netwib_err netwib_ring_index_ctl_get(netwib_ring_index *pringindex,
                                     netwib_ring_index_ctltype type,
                                     netwib_ptr p,
                                     netwib_uint32 *pui);

/*-------------------------------------------------------------*/
/* netwib_err f(netwib_ring_index *pringindex); */
#define netwib_ring_index_ctl_set_rewind(pringindex) netwib_ring_index_ctl_set(pringindex,NETWIB_RING_INDEX_CTLTYPE_REWIND,NULL,0)
/* netwib_err f(netwib_ring_index *pringindex,netwib_ring_index *pringindexref);*/
#define netwib_ring_index_ctl_set_index(pringindex,pringindexref) netwib_ring_index_ctl_set(pringindex,NETWIB_RING_INDEX_CTLTYPE_INDEX,pringindexref,0)

/*-------------------------------------------------------------*/
/* Name : netwib_ring_index_next_criteria
   Description :
     Get the next item in the ring.
   Input parameter(s) :
     pfunc_criteria : function used to decide to remove an item
                      If this function is NULL, no criterion is
                      applied.
   Input/output parameter(s) :
     *pringindex : netwib_ring_index
     pinfos : optional parameter (can be NULL) which will be
              used as the parameter for pfunc_criteria.
              This may be used to send information to *pfunc_criteria.
   Output parameter(s) :
     *ppitem : pointer to the memory containing the item
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_DATAEND : end of the ring reached
*/
netwib_err netwib_ring_index_next_criteria(netwib_ring_index *pringindex,
                                           netwib_ring_criteria_pf pfunc_criteria,
                                           netwib_ptr pinfos,
                                           netwib_ptr *ppitem);
#define netwib_ring_index_next(pringindex,ppitem) netwib_ring_index_next_criteria(pringindex,NULL, NULL,ppitem)
netwib_err netwib_ring_index_previous_criteria(netwib_ring_index *pringindex,
                                               netwib_ring_criteria_pf pfunc_criteria,
                                               netwib_ptr pinfos,
                                               netwib_ptr *ppitem);
#define netwib_ring_index_previous(pringindex,ppitem) netwib_ring_index_previous_criteria(pringindex,NULL, NULL,ppitem)

/*-------------------------------------------------------------*/
/* Name : netwib_ring_index_add_before
   Description :
     Add an item in the ring.
   Input parameter(s) :
   Input/output parameter(s) :
     *pringindex : netwib_ring_index
   Output parameter(s) :
     *ppitem : pointer to the memory containing the item
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ring_index_add_before(netwib_ring_index *pringindex,
                                        netwib_constptr pitem);
netwib_err netwib_ring_index_add_after(netwib_ring_index *pringindex,
                                       netwib_constptr pitem);

/*-------------------------------------------------------------*/
/* Name : netwib_ring_index_this_value
   Description :
     Re-give the last value.
   Input parameter(s) :
   Input/output parameter(s) :
     *pringindex : netwib_ring_index
   Output parameter(s) :
     *ppitem : pointer to the memory containing the item
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ring_index_this_value(netwib_ring_index *pringindex,
                                        netwib_ptr *ppitem);

/*-------------------------------------------------------------*/
/* Name : netwib_ring_index_this_del
   Description :
     Delete the last given value by a netwib_ring_index_next
     function.
   Input parameter(s) :
     eraseitem : if true, function pfunc_erase (set in
                 netwib_hash_init) is called to erase the
                 item located at position
   Input/output parameter(s) :
     *pringindex : netwib_ring_index
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ring_index_this_del(netwib_ring_index *pringindex,
                                      netwib_bool eraseitem);

/*-------------------------------------------------------------*/
/* Name : netwib_ring_index_this_replace
   Description :
     Replace value, but keep the same key.
   Input parameter(s) :
     pitem : pointer to an allocated memory containing the item
     erasepreviousitem : if true, function pfunc_erase (set in
                         netwib_ring_init) is called to erase the
                         item previously located in the ring
   Input/output parameter(s) :
     *pringindex : netwib_ring_index
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ring_index_this_replace(netwib_ring_index *pringindex,
                                          netwib_constptr pitem,
                                          netwib_bool erasepreviousitem);

/*-------------------------------------------------------------*/
/* Name : netwib_ring_index_add_ring_criteria
   Description :
     Add a ring within another ring. Only items matching a
     criterion are added.
   Input parameter(s) :
     *pringindex : ring is added after this index
     *pringtoadd : netwib_ring to add in pring
     pfunc_criteria : function used to decide to add an item
                      If this function is NULL, no criterion is
                      applied.
     duplicateitems : if true, function pfunc_duplicate (set in
                      netwib_ring_init) is called to duplicate
                      items
   Input/output parameter(s) :
     *pring : netwib_ring where to work
     pinfos : optional parameter (can be NULL) which will be
              used as the parameter for pfunc_criteria.
              This may be used to send information to *pfunc_criteria.
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
   Note :
     If an error occurs during addition, ring will only
     contain partial data.
*/
netwib_err netwib_ring_index_add_ring_criteria(netwib_ring_index *pringindex,
                                               netwib_ring *pringtoadd,
                                               netwib_ring_criteria_pf pfunc_criteria,
                                               netwib_ptr pinfos,
                                               netwib_bool duplicateitems);
#define netwib_ring_index_add_ring_all(pringindex,pringtoadd,duplicateitems) netwib_ring_index_add_ring_criteria(pringindex,pringtoadd,NULL,NULL,duplicateitems)
