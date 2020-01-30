/* integer of 8 bits */
typedef NETWIBDEF_TYPE_INT8 netwib_int8;
typedef NETWIBDEF_TYPE_UINT8 netwib_uint8;

/* integer of 16 bits */
typedef NETWIBDEF_TYPE_INT16 netwib_int16;
typedef NETWIBDEF_TYPE_UINT16 netwib_uint16;

/* integer of 32 bits */
typedef NETWIBDEF_TYPE_INT32 netwib_int32;
typedef NETWIBDEF_TYPE_UINT32 netwib_uint32;

/* integer of 64 bits */
#if NETWIBDEF_TYPE_INT64_FAKE == 0
  /* define the type */
  typedef NETWIBDEF_TYPE_INT64 netwib_int64;
  typedef NETWIBDEF_TYPE_UINT64 netwib_uint64;
  #define NETWIB_INT64_FAKE 0
#else
  /* define a fake structure allowing easy storage, but unusable for math */
  typedef struct {
    netwib_uint32 high;
    netwib_uint32 low;
  } netwib_uint64;
  typedef netwib_uint64 netwib_int64;
  #define NETWIB_INT64_FAKE 1
#endif

/* maximum size integer on the computer */
#if NETWIB_INT64_FAKE == 0
  typedef netwib_int64 netwib_intmax;
  typedef netwib_uint64 netwib_uintmax;
  #define NETWIB_INTMAX_BITS 64
#else
  typedef netwib_int32 netwib_intmax;
  typedef netwib_uint32 netwib_uintmax;
  #define NETWIB_INTMAX_BITS 32
#endif

/* size of pointers on the computer */
#if NETWIBDEF_ARCH_BITS == 32
  typedef netwib_int32 netwib_intptr;
  typedef netwib_uint32 netwib_uintptr;
  #define NETWIB_INTPTR_BITS 32
#elif NETWIBDEF_ARCH_BITS == 64
  typedef netwib_int64 netwib_intptr;
  typedef netwib_uint64 netwib_uintptr;
  #define NETWIB_INTPTR_BITS 64
#else
  #error "Unknown value for NETWIBDEF_ARCH_BITS"
#endif

/* char */
typedef char netwib_char;

/* byte */
typedef unsigned char netwib_byte;

/* pointer */
typedef void* netwib_ptr;
typedef const void* netwib_constptr;

/* data */
typedef netwib_byte* netwib_data;
typedef const netwib_byte* netwib_constdata;
/* string */
typedef netwib_char* netwib_string;
typedef const netwib_char* netwib_conststring;

/* boolean */
typedef enum {
  NETWIB_FALSE = 0,
  NETWIB_TRUE = !NETWIB_FALSE
} netwib_bool;

/* comparison */
typedef enum {
  NETWIB_CMP_LT = -1,
  NETWIB_CMP_EQ = 0,
  NETWIB_CMP_GT = +1
} netwib_cmp;

/* netwib contains several enum. User can define its own values
   starting from 10000 */
#define NETWIB_ENUM_USER_BEGIN 10000

/*-------------------------------------------------------------*/
/***************************************************************
 * Note about return values :                                  *
 * Every function returns a "netwib_err" which indicates :     *
 *   - NETWIB_ERR_OK  : everything went fine                   *
 *   - NETWIB_ERR_xyz : something strange occurred...          *
 ***************************************************************/

/*-------------------------------------------------------------*/
/***************************************************************
 * Note about parameters :                                     *
 * Some functions can accept NULL as parameter. This indicates *
 * the corresponding parameter is not needed.                  *
 * However this special case needs resources and specific      *
 * instruction paths. So, this is not supported for parameters *
 * such as netwib_ring, netwib_ips, etc. If you think we       *
 * missed one function needing this features, please contact   *
 * us.                                                         *
 ***************************************************************/
