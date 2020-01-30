
/*-------------------------------------------------------------*/
/* use this trick to convert a "netwib_conststring" to "netwib_string" */
#define netwib_priv_caststring(s) ((netwib_string)((netwib_uintptr)s))
/* use this trick to convert a "netwib_constdata" to "netwib_data" */
#define netwib_priv_castdata(s) ((netwib_data)((netwib_uintptr)s))
/* use this trick to convert a "netwib_constptr" to "netwib_ptr" */
#define netwib_priv_castptr(s) ((netwib_ptr)((netwib_uintptr)s))
/* use this trick to convert a "netwib_constptr*" to "netwib_ptr *" */
#define netwib_priv_castpptr(s) ((netwib_ptr*)((netwib_uintptr)s))

/*-------------------------------------------------------------*/
#if ! defined NETWIBDEF_PRIV_DONT_WANT_SYS
netwib_err netwib_priv_buf_append_vfmt(netwib_buf *pbuf,
                                       netwib_conststring fmt,
                                       va_list *pap);
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_priv_buf_wipe(netwib_buf *pbuf);

/*-------------------------------------------------------------*/
#define NETWIB_PRIV_NUM_1K 1000
#define NETWIB_PRIV_NUM_1M 1000000
#define NETWIB_PRIV_NUM_1G 1000000000

/*-------------------------------------------------------------*/
#include "priv/bufstore.h"

/*-------------------------------------------------------------*/
typedef struct netwib_ringitem netwib_ringitem;
struct netwib_ringitem {
  netwib_ringitem *pnext;
  netwib_ringitem *pprevious;
  netwib_ptr pitem;
};
struct netwib_ring {
  netwib_ringitem *pnext;
  netwib_ringitem *pprevious;
  netwib_uint32 numberofitems;
  netwib_ring_erase_pf pfunc_erase;
  netwib_ring_duplicate_pf pfunc_duplicate;
};

/*-------------------------------------------------------------*/
typedef struct netwib_hashitem netwib_hashitem;
struct netwib_hashitem {
  netwib_hashitem *pnext; /* must be the first ; see index_this_del func. */
  netwib_uint32 currenttablepos;
  netwib_ptr pitem;
  netwib_uint32 hashofkey;
  netwib_uint32 keysize;
  netwib_data key;
};

/*-------------------------------------------------------------*/
struct netwib_hash {
  netwib_uint32 numberofitems;
  netwib_uint32 tablemax; /* == tablesize-1 */
  netwib_hashitem **table;
  netwib_hash_erase_pf pfunc_erase;
  netwib_hash_duplicate_pf pfunc_duplicate;
  netwib_uint32 internalrandom;
};
