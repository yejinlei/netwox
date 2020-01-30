
/*-------------------------------------------------------------*/
/***************************************************************
 * A netwib_hash_index is used to loop through a hash.         *
 *                                                             *
 * When the hash is only read, several indexes                 *
 * can simultaneously loop through the hash.                   *
 *                                                             *
 * However, when the hash is edited, strange behavior can      *
 * occur when a netwib_hash_index_xyz function is called,      *
 * depending on function used to edit the hash:                *
 *  - netwib_hash_contains,                                    *
 *    netwib_hash_value,                                       *
 *    netwib_hash_index_next                                   *
 *    netwib_hash_index_this_value,                            *
 *    netwib_hash_index_this_replace,                          *
 *    netwib_hash_index_this_del:                              *
 *      No problem.                                            *
 *  - netwib_hash_add,                                         *
 *    netwib_hash_add_hash_criteria:                           *
 *      The added item(s) may be returned or not returned,     *
 *      depending on the (unpredictable) insertion position    *
 *      relative to the index position.                        *
 *  - netwib_hash_del,                                         *
 *    netwib_hash_del_criteria:                                *
 *      You should not use them because if the item pointed by *
 *      the index is deleted, it will cause an access to freed *
 *      memory. You may use them only if you are sure to not   *
 *      delete the current item.                               *
 *      However, after netwib_hash_index_this_del, you must    *
 *      not use them because there is not way to know if they  *
 *      will delete internal items.                            *
 ***************************************************************/

/*-------------------------------------------------------------*/
typedef struct netwib_hash_index netwib_hash_index;

/*-------------------------------------------------------------*/
/* Name : netwib_hash_index_init
   Description :
     Initialize a netwib_hash_index used to loop through
     a netwib_hash.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     **pphashindex : netwib_hash_index allocated and initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_hash_index_init(netwib_consthash *phash,
                                  netwib_hash_index **pphashindex);

/*-------------------------------------------------------------*/
/* Name : netwib_hash_index_close
   Description :
     Close a netwib_hashindex.
   Input parameter(s) :
   Input/output parameter(s) :
     **pphashindex : netwib_hash_index closed
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_hash_index_close(netwib_hash_index **pphashindex);

/*-------------------------------------------------------------*/
/* Types to control a netwib_hash_index */
typedef enum {
  NETWIB_HASH_INDEX_CTLTYPE_REWIND = 1,  /* position at beginning */
  NETWIB_HASH_INDEX_CTLTYPE_INDEX        /* reset with index pos */
} netwib_hash_index_ctltype;
/* Those functions permit to set/get parameters (pointer and
   integer) about a netwib_hash_index. It should not be used directly,
   but by the defines.
*/
netwib_err netwib_hash_index_ctl_set(netwib_hash_index *phashindex,
                                     netwib_hash_index_ctltype type,
                                     netwib_ptr p,
                                     netwib_uint32 ui);
netwib_err netwib_hash_index_ctl_get(netwib_hash_index *phashindex,
                                     netwib_hash_index_ctltype type,
                                     netwib_ptr p,
                                     netwib_uint32 *pui);

/*-------------------------------------------------------------*/
/* netwib_err f(netwib_hash_index *phashindex); */
#define netwib_hash_index_ctl_set_rewind(phashindex) netwib_hash_index_ctl_set(phashindex,NETWIB_HASH_INDEX_CTLTYPE_REWIND,NULL,0)
/* netwib_err f(netwib_hash_index *phashindex,netwib_hash_index *phashindexref);*/
#define netwib_hash_index_ctl_set_index(phashindex,phashindexref) netwib_hash_index_ctl_set(phashindex,NETWIB_HASH_INDEX_CTLTYPE_INDEX,phashindexref,0)

/*-------------------------------------------------------------*/
/* Name : netwib_hash_index_next_criteria
   Description :
     Get the next item in the hash.
   Input parameter(s) :
     pfunc_search : function used to match the item
   Input/output parameter(s) :
     *phashindex : netwib_hash_index
     pinfos : optional parameter (can be NULL) which will be
              used as the second parameter for *pfunc_search.
              This may be used to send information to *pfunc_search.
   Output parameter(s) :
     *pkey : found key
     *ppitem : found item
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_DATAEND : end of the hash reached
*/
netwib_err netwib_hash_index_next_criteria(netwib_hash_index *phashindex,
                                           netwib_hash_criteria_pf pfunc_search,
                                           netwib_ptr pinfos,
                                           netwib_buf *pkey,
                                           netwib_ptr *ppitem);
#define netwib_hash_index_next(phashindex,pkey,ppitem) netwib_hash_index_next_criteria(phashindex,NULL,NULL,pkey,ppitem)

/*-------------------------------------------------------------*/
/* Name : netwib_hash_index_this_value
   Description :
     Re-give the last value.
   Input parameter(s) :
   Input/output parameter(s) :
     *phashindex : netwib_hash_index
   Output parameter(s) :
     *pkey : found key
     *ppitem : found item
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_hash_index_this_value(netwib_hash_index *phashindex,
                                        netwib_buf *pkey,
                                        netwib_ptr *ppitem);

/*-------------------------------------------------------------*/
/* Name : netwib_hash_index_this_del
   Description :
     Delete the last given value by a netwib_hash_index_next
     function.
   Input parameter(s) :
     eraseitem : if true, function pfunc_erase (set in
                 netwib_hash_init) is called to erase the
                 item located at position
   Input/output parameter(s) :
     *phashindex : netwib_hash_index
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_hash_index_this_del(netwib_hash_index *phashindex,
                                      netwib_bool eraseitem);

/*-------------------------------------------------------------*/
/* Name : netwib_hash_index_this_replace
   Description :
     Replace value, but keep the same key.
   Input parameter(s) :
     pitem : pointer to an allocated memory containing the item
     erasepreviousitem : if true, function pfunc_erase (set in
                         netwib_hash_init) is called to erase the
                         item previously located in the hash
   Input/output parameter(s) :
     *phashindex : netwib_hash_index
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_hash_index_this_replace(netwib_hash_index *phashindex,
                                          netwib_constptr pitem,
                                          netwib_bool erasepreviousitem);
