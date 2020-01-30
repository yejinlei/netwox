
/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_MEM_CORRUPT!=0
 /* number of free to keep before releasing them (so, stopping
    checking them) */
 #define NETWIB_DEBUG_MEMCORRUPT_FREELIST_SIZE 100
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_GLOVARS_NEEDED==1
typedef struct {
 #if NETWIB_DEBUG_LEAK_NEEDED==1
  netwib_ptr leak_list;
 #endif
 #if NETWIB_DEBUG_MEM_CORRUPT!=0
  const void **corrupt_alloclist;
  netwib_uintptr corrupt_alloclist_maxitems;
  netwib_ptr corrupt_freelist_ptr[NETWIB_DEBUG_MEMCORRUPT_FREELIST_SIZE];
  netwib_uint32 corrupt_freelist_size[NETWIB_DEBUG_MEMCORRUPT_FREELIST_SIZE];
  netwib_uint32 corrupt_freelist_nextpost;
 #endif
} netwib_debug_glovars_t;
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_GLOVARS_NEEDED==1
extern netwib_debug_glovars_t netwib_debug_glovars;
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_GLOVARS_NEEDED==1
netwib_err netwib_debug_glovars_init(void);
netwib_err netwib_debug_glovars_close(void);
netwib_err netwib_debug_glovars_rdlock(void);
netwib_err netwib_debug_glovars_rdunlock(void);
netwib_err netwib_debug_glovars_wrlock(void);
netwib_err netwib_debug_glovars_wrunlock(void);
#else
 /*used in netwib without #if...*/
 #define netwib_debug_glovars_init() NETWIB_ERR_OK;
 #define netwib_debug_glovars_close() NETWIB_ERR_OK;
#endif
