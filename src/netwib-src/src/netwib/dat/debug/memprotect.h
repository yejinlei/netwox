
/*-------------------------------------------------------------*/
netwib_err netwib_debug_memprotect_alloc(netwib_uint32 allocsize,
                                         netwib_ptr *pptr);
netwib_err netwib_debug_memprotect_protect(netwib_ptr ptr,
                                           netwib_uint32 ptrsize,
                                           netwib_bool allowaccess);
netwib_err netwib_debug_memprotect_free(netwib_ptr ptr,
                                        netwib_uint32 ptrsize);

