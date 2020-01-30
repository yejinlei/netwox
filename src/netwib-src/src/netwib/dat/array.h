
/*-------------------------------------------------------------*/
/***************************************************************
 * A netwib_array permits to store a dynamic number of items.  *
 * Those items all have the same size.                         *
 *                                                             *
 * The common implementation of such storage is :              *
 *   p = malloc(numitems*sizeof(item));                        *
 *   p[1] = ...;                                               *
 * However, if sizeof(item) is not a multiple of the processor *
 * size, a bus error occurs on strict processors (Alpha,       *
 * Sparc). For example, storing 5 items of size 3 can be       *
 * represented in memory (start at address 0x76543210) :       *
 *   .- address 0x76543210                                     *
 *   ABCABCABCABCABC                                           *
 * The second item is at address 0x76543213 (0x76543210+3)     *
 * which is invalid and causes a bus error.                    *
 * The second drawback of this common implementation is memory *
 * management is not efficient, because when p contains a lot  *
 * of items, this big buffer has to be reallocated.            *
 *                                                             *
 * Netwib's implementation is different. Instead of creating   *
 * an array of items, netwib creates an array of pointers to   *
 * items. This solves both above problems.                     *
 *                                                             *
 * Usage example:                                              *
 *   netwib_array a;                                           *
 *   struct foobar *pfb;                                       *
 *   netwib_er(netwib_array_init(sizeof(struct foobar),3,&a)); *
 *   pfb = (struct foobar *)a.p[0];                            *
 *   pfb->...;                                                 *
 *   for (i = 0; i < a.size; i++) {                            *
 *     pfb = (struct foobar *)a.p[i];                          *
 *     pfb->...;                                               *
 *   }                                                         *
 *   netwib_er(netwib_array_ctl_set_size(&a, 6));              *
 *   pfb = (struct foobar *)a.p[5];                            *
 *   netwib_er(netwib_array_close(&a));                        *
 ***************************************************************/

/*-------------------------------------------------------------*/
typedef struct {
  netwib_ptr *p;      /* array of pointers to items */
  netwib_uint32 size; /* size of p array (0 -> size-1) */
  netwib_ptr opaque;  /* internal storage is here. Do not use. */
} netwib_array;
typedef const netwib_array netwib_constarray;

/*-------------------------------------------------------------*/
/* Name : netwib_array_init
   Description :
     Initialize a netwib_array.
   Input parameter(s) :
     itemsize : size of items to store
     initialsize : initial number of items to store
   Input/output parameter(s) :
   Output parameter(s) :
     *parray : netwib_array initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_array_init(netwib_uint32 itemsize,
                             netwib_uint32 initialsize,
                             netwib_array *parray);

/*-------------------------------------------------------------*/
/* Name : netwib_array_close
   Description :
     Close a netwib_array.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     *parray : netwib_array to close
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_array_close(netwib_array *parray);

/*-------------------------------------------------------------*/
/* Types to control a netwib_array */
typedef enum {
  NETWIB_ARRAY_CTLTYPE_SIZE = 1   /* request an array to grow
                                     or shrink */
} netwib_array_ctltype;
netwib_err netwib_array_ctl_set(netwib_array *parray,
                                netwib_array_ctltype type,
                                netwib_ptr p,
                                netwib_uint32 ui);
netwib_err netwib_array_ctl_get(netwib_array *parray,
                                netwib_array_ctltype type,
                                netwib_ptr p,
                                netwib_uint32 *pui);

/*-------------------------------------------------------------*/
/* netwib_err f(netwib_array *parray, netwib_uint32 newsize); */
#define netwib_array_ctl_set_size(parray,newsize) netwib_array_ctl_set(parray,NETWIB_ARRAY_CTLTYPE_SIZE,NULL,newsize)
#define netwib_array_ctl_get_size(parray,pnewsize) netwib_array_ctl_get(parray,NETWIB_ARRAY_CTLTYPE_SIZE,NULL,pnewsize)
