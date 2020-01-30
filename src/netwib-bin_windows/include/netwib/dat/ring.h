
/*-------------------------------------------------------------*/
/***************************************************************
 * A netwib_ring is a double linked ring containing pointers.  *
 * For example, a ring containing 3 items looks like this :    *
 *                                                             *
 * ,________________________________________________________.  *
 * | ,----------------------------------------------------. |  *
 * | |  ._______.   .________.   .________.   .________.  | |  *
 * | `->|       |-->|        |-->|        |-->|        |--' |  *
 * |    | start |   |  ptr1  |   |  ptr2  |   |  ptr3  |    |  *
 * `----|_______|<--|________|<--|________|<--|________|<---'  *
 *                                                             *
 * The ring contains pointers (ptr1, ptr2 and ptr3 in the      *
 * example).                                                   *
 *                                                             *
 * When an item is removed from the ring, its associated       *
 * information stored in the pointer can be :                  *
 *  - left untouched : user will have to free the pointer and  *
 *    its information                                          *
 *  - erased : the information is lost : a function of type    *
 *    netwib_ring_erase_pf has to be called                    *
 * For example, a user might want to store allocated memory    *
 * containing a netwib_buf. Then when the ring is closed, the  *
 * netwib_buf_close function has to be called and the memory   *
 * has to be freed.                                            *
 ***************************************************************/

/*-------------------------------------------------------------*/
typedef struct netwib_ring netwib_ring;
typedef const netwib_ring netwib_constring;

/*-------------------------------------------------------------*/
/* This prototype defines a function erasing the item pitem */
typedef netwib_err (*netwib_ring_erase_pf)(netwib_ptr pitem);
/* This prototype defines a function duplicating an item */
typedef netwib_err (*netwib_ring_duplicate_pf)(netwib_constptr pitem,
                                               netwib_ptr *pdupofitem);
/* This prototype defines a function comparing two items
    - if iteminf<itemsup, *pcmp is set to NETWIB_CMP_LT
    - if iteminf>itemsup, *pcmp is set to NETWIB_CMP_GT
    - if iteminf==itemsup, *pcmp is set to NETWIB_CMP_EQ
   The parameter pinfos will be set with optional information
   given when calling the function.
 */
typedef netwib_err (*netwib_ring_compare_pf)(netwib_constptr piteminf,
                                             netwib_constptr pitemsup,
                                             netwib_ptr pinfos,
                                             netwib_cmp *pcmp);
/* This prototype defines a function detecting if an item
   matches a criteria
    - if it matches, *pbool is set to NETWIB_TRUE
    - if it does not matches, *pbool is set to NETWIB_FALSE
   The parameter pinfos will be set with optional information
   given when calling the function.
 */
typedef netwib_err (*netwib_ring_criteria_pf)(netwib_constptr pitem,
                                              netwib_ptr pinfos,
                                              netwib_bool *pbool);

/*-------------------------------------------------------------*/
/* Name : netwib_ring_init
   Description :
     Initialize a netwib_ring.
   Input parameter(s) :
     *pfunc_erase : function needed to erase items
                    This can be NULL
     *pfunc_duplicate : function needed to duplicate items
                        This can be NULL
   Input/output parameter(s) :
   Output parameter(s) :
     **ppring : netwib_ring allocated and initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ring_init(netwib_ring_erase_pf pfunc_erase,
                            netwib_ring_duplicate_pf pfunc_duplicate,
                            netwib_ring **ppring);

/*-------------------------------------------------------------*/
/* Name : netwib_ring_close
   Description :
     Destroy a netwib_ring.
   Input parameter(s) :
     eraseitems : if true, function pfunc_erase (set in
                  netwib_ring_init) is called
   Input/output parameter(s) :
     **ppring : netwib_ring to destroy
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ring_close(netwib_ring **ppring,
                             netwib_bool eraseitems);

/*-------------------------------------------------------------*/
/* Types to control a netwib_ring */
typedef enum {
  NETWIB_RING_CTLTYPE_COUNT = 1   /* count number of items */
} netwib_ring_ctltype;
netwib_err netwib_ring_ctl_set(netwib_ring *pring,
                               netwib_ring_ctltype type,
                               netwib_ptr p,
                               netwib_uint32 ui);
netwib_err netwib_ring_ctl_get(netwib_ring *pring,
                               netwib_ring_ctltype type,
                               netwib_ptr p,
                               netwib_uint32 *pui);

/*-------------------------------------------------------------*/
/* netwib_err f(netwib_ring *pring, netwib_uint32 *pnumberofitems); */
#define netwib_ring_ctl_get_count(pring,pnumberofitems) netwib_ring_ctl_get(pring,NETWIB_RING_CTLTYPE_COUNT,NULL,pnumberofitems)

/*-------------------------------------------------------------*/
/* Name : netwib_ring_add_first
   Description :
     Add an item to the netwib_ring.
   Input parameter(s) :
     pitem : pointer to an allocated memory containing the item
   Input/output parameter(s) :
     *pring : netwib_ring where to work
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ring_add_first(netwib_ring *pring,
                                 netwib_constptr pitem);
netwib_err netwib_ring_add_last(netwib_ring *pring,
                                netwib_constptr pitem);

/*-------------------------------------------------------------*/
/* Name : netwib_ring_del_criteria
   Description :
     Del items matching a criterion.
   Input parameter(s) :
     pfunc_criteria : function used to decide to del an item
                      If this function is NULL, no criterion is
                      applied.
   Input/output parameter(s) :
     *pring : netwib_ring where to work
     pinfos : optional parameter (can be NULL) which will be
              used as the parameter for pfunc_criteria.
              This may be used to send information to *pfunc_criteria.
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ring_del_criteria(netwib_ring *pring,
                                    netwib_ring_criteria_pf pfunc_criteria,
                                    netwib_ptr pinfos,
                                    netwib_bool eraseitems);
#define netwib_ring_del_all(pring,eraseitems) netwib_ring_del_criteria(pring,NULL,NULL,eraseitems)

/*-------------------------------------------------------------*/
/* Name : netwib_ring_del_duplicate
   Description :
     Del duplicate items from a netwib_ring.
   Input parameter(s) :
     pfunc_compare : function used to compare two items
   Input/output parameter(s) :
     *pring : netwib_ring where to work
     pinfos : optional parameter (can be NULL) which will be
              used as the parameter for pfunc_compare.
              This may be used to send information to *pfunc_compare.
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ring_del_duplicate(netwib_ring *pring,
                                     netwib_ring_compare_pf pfunc_compare,
                                     netwib_ptr pinfos,
                                     netwib_bool eraseitems);

/*-------------------------------------------------------------*/
/* Name : netwib_ring_sort
   Description :
     Sort items of the netwib_ring.
   Input parameter(s) :
     pfunc_compare : function used to compare two items
   Input/output parameter(s) :
     *pring : netwib_ring where to work
     pinfos : optional parameter (can be NULL) which will be
              used as the parameter for *pfunc_compare.
              This may be used to send information to *pfunc_compare.
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ring_sort(netwib_ring *pring,
                            netwib_ring_compare_pf pfunc_compare,
                            netwib_ptr pinfos);

/*-------------------------------------------------------------*/
/* Name : netwib_ring_group
   Description :
     Group items of the netwib_ring.
   Input parameter(s) :
     pfunc_compare : function used to compare two items
   Input/output parameter(s) :
     *pring : netwib_ring where to work
     pinfos : optional parameter (can be NULL) which will be
              used as the parameter for *pfunc_compare.
              This may be used to send information to *pfunc_compare.
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ring_group(netwib_ring *pring,
                             netwib_ring_compare_pf pfunc_compare,
                             netwib_ptr pinfos);
