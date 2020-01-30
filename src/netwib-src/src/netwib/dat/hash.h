
/*-------------------------------------------------------------*/
/***************************************************************
 * A netwib_hash is an associative array : data is related to  *
 * a key. It's not possible to have two identical keys.        *
 *                                                             *
 * The hash contains pointers.                                 *
 *                                                             *
 * When an item is removed from the hash, its associated       *
 * information stored in the pointer can be :                  *
 *  - left untouched : user will have to free the pointer and  *
 *    its information                                          *
 *  - erased : the information is lost : a function of type    *
 *    netwib_hash_erase_pf has to be called                    *
 * For example, a user might want to store allocated memory    *
 * containing a netwib_buf. Then when the hash is closed, the  *
 * netwib_buf_close function has to be called and the memory   *
 * has to be freed.                                            *
 ***************************************************************/

/*-------------------------------------------------------------*/
typedef struct netwib_hash netwib_hash;
typedef const netwib_hash netwib_consthash;

/*-------------------------------------------------------------*/
/* This prototype defines a function erasing the item pitem */
typedef netwib_err (*netwib_hash_erase_pf)(netwib_ptr pitem);
/* This prototype defines a function duplicating an item */
typedef netwib_err (*netwib_hash_duplicate_pf)(netwib_constptr pitem,
                                               netwib_ptr *pdupofitem);
/* This prototype defines a function detecting if an item
   matches a criteria
    - if it matches, *pbool is set to NETWIB_TRUE
    - if it does not matches, *pbool is set to NETWIB_FALSE
   The parameter pinfos will be set with optional information
   given when calling the function.
 */
typedef netwib_err (*netwib_hash_criteria_pf)(netwib_constbuf *pkey,
                                              netwib_constptr pitem,
                                              netwib_ptr pinfos,
                                              netwib_bool *pbool);

/*-------------------------------------------------------------*/
/* Name : netwib_hash_init
   Description :
     Initialize a netwib_hash.
   Input parameter(s) :
     *pfunc_erase : function needed to erase items
                    This can be NULL
     *pfunc_duplicate : function needed to duplicate items
                        This can be NULL
   Input/output parameter(s) :
   Output parameter(s) :
     **pphash : netwib_hash allocated and initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_hash_init(netwib_hash_erase_pf pfunc_erase,
                            netwib_hash_duplicate_pf pfunc_duplicate,
                            netwib_hash **pphash);

/*-------------------------------------------------------------*/
/* Name : netwib_hash_close
   Description :
     Destroy a netwib_hash.
   Input parameter(s) :
     eraseitems : if true, function pfunc_erase (set in
                  netwib_hash_init) is called
   Input/output parameter(s) :
     **pphash : netwib_hash to destroy
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_hash_close(netwib_hash **pphash,
                             netwib_bool eraseitems);

/*-------------------------------------------------------------*/
/* Types to control a netwib_hash */
typedef enum {
  NETWIB_HASH_CTLTYPE_COUNT = 1   /* count number of items */
} netwib_hash_ctltype;
netwib_err netwib_hash_ctl_set(netwib_hash *phash,
                               netwib_hash_ctltype type,
                               netwib_ptr p,
                               netwib_uint32 ui);
netwib_err netwib_hash_ctl_get(netwib_hash *phash,
                               netwib_hash_ctltype type,
                               netwib_ptr p,
                               netwib_uint32 *pui);

/*-------------------------------------------------------------*/
/* netwib_err f(netwib_hash *phash, netwib_uint32 *pnumberofitems); */
#define netwib_hash_ctl_get_count(phash,pnumberofitems) netwib_hash_ctl_get(phash,NETWIB_HASH_CTLTYPE_COUNT,NULL,pnumberofitems)

/*-------------------------------------------------------------*/
/* Name : netwib_hash_add
   Description :
     Add an item to the netwib_hash. Is the key already exists,
     it is overwritten.
   Input parameter(s) :
     *pkey : key to store
     pitem : pointer to an allocated memory containing the item
     erasepreviousitem : if true, function pfunc_erase (set in
                         netwib_hash_init) is called to erase the
                         item previously located in the hash
   Input/output parameter(s) :
     *phash : netwib_hash where to work
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_hash_add(netwib_hash *phash,
                           netwib_constbuf *pkey,
                           netwib_constptr pitem,
                           netwib_bool erasepreviousitem);

/*-------------------------------------------------------------*/
/* Name : netwib_hash_add_hash_criteria
   Description :
     Add a hash into the netwib_hash. Existing keys
     are overwritten.
   Input parameter(s) :
     pfunc_criteria : function used to decide to add an item
                      If this function is NULL, no criterion is
                      applied.
     erasepreviousitems : if true, function pfunc_erase (set in
                          netwib_hash_init) is called to erase the
                          items previously located in the hash
   Input/output parameter(s) :
     *phash : netwib_hash where to work
     *phashtoadd : netwib_hash to add
     pinfos : optional parameter (can be NULL) which will be
              used as the parameter for pfunc_criteria.
              This may be used to send information to *pfunc_criteria.
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_hash_add_hash_criteria(netwib_hash *phash,
                                         netwib_hash *phashtoadd,
                                         netwib_hash_criteria_pf pfunc_criteria,
                                         netwib_ptr pinfos,
                                         netwib_bool erasepreviousitems);
#define netwib_hash_add_hash_all(phash,phashtoadd,erasepreviousitems) netwib_hash_add_hash_criteria(phash,phashtoadd,NULL,NULL,erasepreviousitems)

/*-------------------------------------------------------------*/
/* Name : netwib_hash_contains
   Description :
     Check if a key is in the hash.
   Input parameter(s) :
     *phash : netwib_hash where to work
     *pkey : key to obtain
   Input/output parameter(s) :
   Output parameter(s) :
     *pyes : true if key is found
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_hash_contains(netwib_hash *phash,
                                netwib_constbuf *pkey,
                                netwib_bool *pyes);

/*-------------------------------------------------------------*/
/* Name : netwib_hash_value
   Description :
     Get the value of an item.
   Input parameter(s) :
     *phash : netwib_hash where to work
     *pkey : key to obtain
   Input/output parameter(s) :
   Output parameter(s) :
     *ppitem : pointer to the memory containing the item
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_NOTFOUND : key not found
*/
netwib_err netwib_hash_value(netwib_hash *phash,
                             netwib_constbuf *pkey,
                             netwib_ptr *ppitem);

/*-------------------------------------------------------------*/
/* Name : netwib_hash_del
   Description :
     Remove an item from the netwib_hash.
   Input parameter(s) :
     *pkey : key to del
     eraseitem : if true, function pfunc_erase (set in
                 netwib_hash_init) is called to erase the
                 item located at position
   Input/output parameter(s) :
     *phash : netwib_hash where to work
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_NOTFOUND : key not found
*/
netwib_err netwib_hash_del(netwib_hash *phash,
                           netwib_constbuf *pkey,
                           netwib_bool eraseitem);

/*-------------------------------------------------------------*/
/* Name : netwib_hash_del_criteria
   Description :
     Del items matching a criterion.
   Input parameter(s) :
     pfunc_criteria : function used to decide to del an item
                      If this function is NULL, no criterion is
                      applied.
   Input/output parameter(s) :
     *phash : netwib_hash where to work
     pinfos : optional parameter (can be NULL) which will be
              used as the parameter for pfunc_criteria.
              This may be used to send information to *pfunc_criteria.
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_hash_del_criteria(netwib_hash *phash,
                                    netwib_hash_criteria_pf pfunc_criteria,
                                    netwib_ptr pinfos,
                                    netwib_bool eraseitems);
#define netwib_hash_del_all(phash,eraseitems) netwib_hash_del_criteria(phash,NULL,NULL,eraseitems)
