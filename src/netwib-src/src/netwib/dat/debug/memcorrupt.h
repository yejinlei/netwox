
/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_MEMCORRUPT_NEEDED==1
/*used in netwib without #if...*/
netwib_err netwib_debug_memcorrupt_init(void);
netwib_err netwib_debug_memcorrupt_close(void);
netwib_err netwib_debug_memcorrupt_check_all(void);
netwib_err netwib_debug_memcorrupt_add(netwib_ptr ptr);
/*used in netwib_data*/
/*generic*/
netwib_err netwib_debug_memcorrupt_check(netwib_constptr ptr);
netwib_err netwib_debug_memcorrupt_check_periodic(void);
netwib_err netwib_debug_memcorrupt_realloc(netwib_uint32 newallocsize,
                                           netwib_ptr *pptr);
/*specific*/
netwib_err netwib_debug_memcorrupt_alloc(netwib_uint32 allocsize,
                                         netwib_ptr *pptr);
netwib_err netwib_debug_memcorrupt_free(netwib_ptr *pptr);
netwib_err netwib_debug_memcorrupt_alloc_check(netwib_constptr ptr,
                                               netwib_uint32 *pallocsize,
                                               netwib_uint32 *pptroffset,
                                               netwib_ptr *ptrueptr,
                                               netwib_uint32 *ptrueallocsize);
netwib_err netwib_debug_memcorrupt_free_check(netwib_constptr trueptr,
                                              netwib_uint32 trueallocsize);
#else
 /*used in netwib without #if...*/
 #define netwib_debug_memcorrupt_init() NETWIB_ERR_OK;
 #define netwib_debug_memcorrupt_close() NETWIB_ERR_OK;
 #define netwib_debug_memcorrupt_check_all() NETWIB_ERR_OK;
 #define netwib_debug_memcorrupt_add(a) NETWIB_ERR_OK;
#endif
