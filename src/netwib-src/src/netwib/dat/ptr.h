
/*-------------------------------------------------------------*/
/* Name : netwib_ptr_malloc
   Description :
     Allocate a memory array.
   Input parameter(s) :
     allocsize : size of this array
   Input/output parameter(s) :
   Output parameter(s) :
     *pptr : pointer which will be malloced (so, the
             memory will have to be freed by the
             user with 'netwib_ptr_free(pptr)').
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ptr_malloc(netwib_uint32 allocsize,
                             netwib_ptr *pptr);

/*-------------------------------------------------------------*/
/* Name : netwib_ptr_realloc
   Description :
     Reallocate a memory array.
   Input parameter(s) :
     newallocsize : new size of this array
   Input/output parameter(s) :
   Output parameter(s) :
     *pptr : pointer which will be reallocated (so, the
             memory will have to be freed by the
             user with 'netwib_ptr_free(pptr)').
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ptr_realloc(netwib_uint32 newallocsize,
                              netwib_ptr *pptr);

/*-------------------------------------------------------------*/
/* Name : netwib_ptr_free
   Description :
     Free a memory array.
   Input parameter(s) :
     *pptr : pointer to the memory to free
   Input/output parameter(s) :
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ptr_free(netwib_ptr *pptr);
