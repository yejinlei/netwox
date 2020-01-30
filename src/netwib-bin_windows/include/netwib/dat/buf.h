
/*-------------------------------------------------------------*/
/***************************************************************
 * A netwib_buf is the standard memory storage used in netwib. *
 *                                                             *
 * A netwib_buf points to an array (static or malloced) which  *
 * starts at 'first' and finishes at 'last'.                   *
 * The memory contains user data between 'begin' and 'end'.    *
 *    ---------------------------------------------------      *
 *    |       |               data              |       |      *
 *    |F|     |B|                               |E|     |L     *
 *    ---------------------------------------------------      *
 *   First   Begin                              End    Last    *
 *     0       x                                 y   totalsize *
 *                                                             *
 * Data between 'first and begin', and data between 'end and   *
 * last' may be corrupted when working on the buffer. To avoid *
 * this, use netwib_buf_init_ext_buf and work on the new buffer*
 ***************************************************************/

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 flags;       /* see below */
  netwib_data totalptr;      /* ptr to first */
  netwib_uint32 totalsize;   /* last - first */
  netwib_uint32 beginoffset; /* begin - first */
  netwib_uint32 endoffset;   /* end - first */
} netwib_buf;
typedef const netwib_buf netwib_constbuf;

/*-------------------------------------------------------------*/
/***************************************************************
 * Field "flags" is a bit field indicating some information    *
 * about a netwib_buf.                                         *
 ***************************************************************/

/* If totalptr points :
     0x0001 : to an allocated memory
    ~0x0001 : to an external array (stack array or array which
              does not need to be freed)
   Those two modes corresponds to the two ways to initialize a
   netwib_buf :
     - netwib_buf_init_malloc : it internally allocates memory
       and eventually reallocate it if more is needed. At the
       end, function netwib_buf_close must be called to free
       memory.
     - netwib_buf_init_ext_xyz : buffer does not contain memory,
       but points on an external array. At the end, function
       netwib_buf_close does not need to be called to free
       memory (no closing function is needed).
   This flag should not be directly modified by user.
 */
#define NETWIB_BUF_FLAGS_ALLOC 0x00000001u

/* In the following case :
     - flag NETWIB_BUF_FLAGS_ALLOC is unset, and
     - there is no sufficient space in the array
   we can :
     0x0002 : allocate memory and store array content in it
              (the array is no more used)
    ~0x0002 : return a space limitation error
   This flag is very useful to optimize code. When we know in
   most cases our code will need 80 bytes, we use an array
   of 80 bytes. In the rare cases where it is not sufficient,
   an allocated pointer is created. Like this for most
   frequently encountered cases, there is no need to allocate.
   Once allocated, it works like if it was allocated from beginning.
   Once allocated, the external array is no more used.
   Once allocated, flag NETWIB_BUF_FLAGS_ALLOC is automatically
   set.
   If array size is greater than 2k, it's not really advantageous
   to use this flag, because copying 2k needs almost the same time
   as a malloc and free.
   This is generally used with netwib_buf_init_ext_arrayempty.
   This flag defaults to false. It has to be explicitly set
   by user.
 */
#define NETWIB_BUF_FLAGS_CANALLOC 0x00000002u

/* If functions are :
     0x0004 : allowed to slide data (shrunk space between First
              and Begin), when there is no sufficient space
              between End and Last for appending
    ~0x0004 : not allowed to slide
   This flag defaults to false. It has to be explicitly set
   by user.
*/
#define NETWIB_BUF_FLAGS_CANSLIDE 0x00000004u

/* Sometimes, a buffer contains sensitive data such as a password,
   so we want to do a memset on this data when it is no more needed.
   This flags says that buffer contains sensitive data. Buffer will
   be wiped with memset(.0.) during:
     netwib_buf_close(),
     netwib__buf_reinit(),
     netwib__buf_erase(),
     netwib_bufpool_buf_close(),
     netwib_bufpool_close(),
     and all functions using above ones.
   It is developer's task to set this flag each time a buffer may
   contain sensitive data.
   Netwib supports a feature to transfer this flag (also known as
   tainting in other languages). For example, when a sensitive
   buffer is copied to another buffer, this one also becomes
   sensitive.
   Once buffer is closed, the flag is unset, so user has to set
   it each time sensitive data is initialized. The closing
   functions are:
     netwib_buf_close(),
     netwib_bufpool_buf_close(),
     netwib_bufpool_close().
   WARNING:
   This feature was conceived for PASSWORDS or cryptographic KEYS
   stored in a netwib_buf. It cannot help for other types such
   as a netwib_string, a netwib_ring or a netwib_ip.
   This feature was conceived for buffer manipulation functions,
   such as netwib_buf_append_buf, and not in functions unrelated
   to passwords or keys, such as netwib_pkt_... or
   netwib_sniff_... When a function supports this feature, it is
   indicated in its help comment before the prototype (.h file).
   Perhaps one day, I will expand this feature to erase other kind
   of data, but this is not the case currently.
   To sum up, don't expect this feature to be the Solution for
   your sensitive data management, but only a small step towards it.
   END OF WARNING.
   This flag defaults to false. It has to be explicitly set
   by user.
*/
#define NETWIB_BUF_FLAGS_SENSITIVE 0x00000008u
/* data is sensitive, but readonly (such as a password stored
   in a static string which cannot be wiped, but can be
   transfered when buffer is copied)
*/
#define NETWIB_BUF_FLAGS_SENSITIVE_READONLY 0x00000010u
/* to transfer the sensitive flag to the second buffer */
#define netwib__buf_transfersensitive(pbuf1,pbuf2) { if ((pbuf1) != NULL && (pbuf2) != NULL) { if ((pbuf1)->flags & NETWIB_BUF_FLAGS_SENSITIVE) { (pbuf2)->flags |= NETWIB_BUF_FLAGS_SENSITIVE; } } }
/* to wipe a local array used in parallel of a buffer */
#define netwib__localarray_wipe(arr) netwib_c_memset((arr),0,sizeof(arr))
#define netwib__localarray_ifbuf_wipe(pbuf,arr) { if ((pbuf) != NULL && ((pbuf)->flags & NETWIB_BUF_FLAGS_SENSITIVE)) netwib__localarray_wipe(arr); }

/* number of used bits */
#define NETWIB_BUF_FLAGS_USEDBITS 5

/*-------------------------------------------------------------*/
/***************************************************************
 * Type netwib_bufext is exactly the same as netwib_buf. It    *
 * permits to easily determine which kind of buffer is needed  *
 * by a function :                                             *
 *  - Functions having an output parameter of type netwib_buf  *
 *    must be called with the buffer previously initialized :  *
 *    they will append data to it                              *
 *  - Functions having an output parameter of type             *
 *    netwib_bufext will initialize it.                        *
 * Example :                                                   *
 *  An IP4 packet might contain an IP4 option. There is no     *
 *  need to allocate/copy data for this option, because it is  *
 *  simply contained in the input packet (at offset            *
 *  20==sizeof(ip4hdr)). So in this case a netwib_bufext is    *
 *  used.                                                      *
 ***************************************************************/
typedef netwib_buf netwib_bufext;

/*-------------------------------------------------------------*/
/* Name : netwib_buf_init_malloc
   Description :
     Initialize a buf. Its memory dynamically grows.
   Input parameter(s) :
     allocsize : allocated size. If 0, a
                 default value is used.
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_init_malloc(netwib_uint32 allocs,
                                  netwib_buf *pbuf);
#define netwib_buf_init_mallocdefault(pbuf) netwib_buf_init_malloc(1024,pbuf)

/*-------------------------------------------------------------*/
/* Name : netwib_buf_init_ext_array
   Description :
     Initialize a buf. Its memory corresponds to an external
     fixed size array.
   Input parameter(s) :
     array : external array
     arraysize : external array size
     beginoffset : offset of begin in this array
     endoffset : offset of end in this array
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_bufext initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_init_ext_array(netwib_constptr array,
                                     netwib_uint32 arraysize,
                                     netwib_uint32 beginoffset,
                                     netwib_uint32 endoffset,
                                     netwib_bufext *pbuf);
#define netwib_buf_init_ext_arrayempty(array,arraysize,pbuf) netwib_buf_init_ext_array(array,arraysize,0,0,pbuf)
#define netwib_buf_init_ext_arrayfilled(array,arraysize,pbuf) netwib_buf_init_ext_array(array,arraysize,0,arraysize,pbuf)
#define netwib_buf_init_ext_arraysizeofempty(array,pbuf) netwib_buf_init_ext_arrayempty(array,sizeof(array),pbuf)
#define netwib_buf_init_ext_arraysizeoffilled(array,pbuf) netwib_buf_init_ext_arrayfilled(array,sizeof(array),pbuf)
/* A buffer containing no data */
#define netwib_buf_init_ext_empty(pbuf) netwib_buf_init_ext_array(NULL,0,0,0,pbuf)

/*-------------------------------------------------------------*/
/* Name : netwib_buf_init_ext_storagearray
   Description :
     Initialize a buf. Its memory corresponds to an external
     fixed size array or to nothing. When data is added, it may
     be allocated. A call to netwib_buf_close() will have to be done.
   Input parameter(s) :
     array : external array
     arraysize : external array size
     beginoffset : offset of begin in this array
     endoffset : offset of end in this array
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_bufext initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_init_ext_storagearray(netwib_constptr array,
                                            netwib_uint32 arraysize,
                                            netwib_bufext *pbuf);
/* An empty buffer, allocated on first append */
#define netwib_buf_init_ext_storage(pbuf) netwib_buf_init_ext_storagearray(NULL,0,pbuf)
/* An empty array, allocated if not sufficiently big */
#define netwib_buf_init_ext_storagearraysizeof(array,pbuf) netwib_buf_init_ext_storagearray(array,sizeof(array),pbuf)

/*-------------------------------------------------------------*/
/* Name : netwib_buf_init_ext_buf
   Description :
     Initialize a buf. Its memory corresponds to an external
     buffer.
   Input parameter(s) :
     *pbuf : netwib_buf containing data
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_bufext initialized
   Normal return values :
     NETWIB_ERR_OK : ok
   This function supports NETWIB_BUF_FLAGS_SENSITIVE.
*/
netwib_err netwib_buf_init_ext_buf(netwib_constbuf *pbufin,
                                   netwib_bufext *pbuf);

/*-------------------------------------------------------------*/
/* Name : netwib_buf_init_ext_string
   Description :
     Initialize a buf. Its memory corresponds to an external
     preset string.
   Input parameter(s) :
     str : external string
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_bufext initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_init_ext_string(netwib_conststring str,
                                      netwib_bufext *pbuf);

/*-------------------------------------------------------------*/
/* Name : netwib_buf_append_xyz
   Description :
     Add data to a buf.
   Input parameter(s) :
     data : data to add
     datasize : size of data to add
     str : string to add
     c : character to add
   Input/output parameter(s) :
     *pbuf : netwib_buf updated
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
   The netwib_buf_append_buf function supports NETWIB_BUF_FLAGS_SENSITIVE.
*/
netwib_err netwib_buf_append_data(netwib_constdata data,
                                  netwib_uint32 datasize,
                                  netwib_buf *pbuf);
netwib_err netwib_buf_append_string(netwib_conststring str,
                                    netwib_buf *pbuf);
netwib_err netwib_buf_append_buf(netwib_constbuf *pbuftoappend,
                                 netwib_buf *pbuf);
netwib_err netwib_buf_append_byte(netwib_byte b,
                                  netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* Name : netwib_buf_prepend_buf
   Description :
     Prepend data to a buf.
   Input parameter(s) :
   Input/output parameter(s) :
     *pbuf : netwib_buf updated
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
   Note :
     Parameter canslide has no effect on this function (this is
     logical).
   This function supports NETWIB_BUF_FLAGS_SENSITIVE.
*/
netwib_err netwib_buf_prepend_buf(netwib_constbuf *pbuftoprepend,
                                  netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* Name : netwib_buf_ref_string
   Description :
     Get pointers to data stored in buf (between begin and end).
     If buffer internal storage does not end with '\0',
     a new one is added. So, we can have NETWIB_ERR_DATANOSPACE
     if there is no room for the '\0'.
   Input parameter(s) :
     *pbuf : buffer
   Input/output parameter(s) :
   Output parameter(s) :
     *pstr : pointer to start of string
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_DATANOSPACE : no room for the '\0'
*/
netwib_err netwib_buf_ref_string(netwib_buf *pbuf,
                                 netwib_string *pstr);

/*-------------------------------------------------------------*/
/* Name : netwib_constbuf_ref_string
   Description :
     Some as netwib_buf_ref_string except it does not modify
     the buffer to add the '\0'.
   Input parameter(s) :
     *pbuf : buffer
   Input/output parameter(s) :
   Output parameter(s) :
     *pstr : pointer to start of string
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_DATANOSPACE : the char after endoffset is not a '\0'
*/
netwib_err netwib_constbuf_ref_string(netwib_constbuf *pbuf,
                                      netwib_string *pstr);
/* to use at begin of a function to manage a local buffer */
#define netwib__constbuf_ref_string(pbuf, str, bufstorage, func) { netwib_err bufstorageret; bufstorageret = netwib_constbuf_ref_string(pbuf, &str); if (bufstorageret != NETWIB_ERR_OK) { if (bufstorageret == NETWIB_ERR_DATANOSPACE) { netwib_data bufstoragearray[512]; netwib_buf bufstorage; netwib_er(netwib_buf_init_ext_storagearraysizeof(bufstoragearray, &bufstorage)); netwib_er(netwib_buf_append_buf(pbuf, &bufstorage)); netwib_er(netwib_buf_append_byte(0, &bufstorage)); bufstorage.endoffset--; bufstorageret = func; netwib_er(netwib_buf_close(&bufstorage)); } return(bufstorageret); } }

/*-------------------------------------------------------------*/
/* Name : netwib_buf_wantspace
   Description :
     Request space in a buffer (from end to last).
     When buffer is initialized as malloced memory, it is
     possible to obtain unlimited space. Otherwise, we cannot
     obtain more space than array size (unless flag
     NETWIB_BUF_FLAGS_CANALLOC is set).
   Input parameter(s) :
     wantedspace : wanted space
   Input/output parameter(s) :
     *pbuf : buffer
   Output parameter(s) :
     *pdata : pointer to end (endoffset)
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_DATANOSPACE : there is less than wantedspace
*/
netwib_err netwib_buf_wantspace(netwib_buf *pbuf,
                                netwib_uint32 wantedspace,
                                netwib_data *pdata);

/*-------------------------------------------------------------*/
/* Name : netwib_buf_wishspace
   Description :
     Request space in a buffer (from end to last).
     When buffer is initialized as malloced memory, it is
     possible to obtain unlimited space. Otherwise, we cannot
     obtain more space than array size (unless flag
     NETWIB_BUF_FLAGS_CANALLOC is set).
   Input parameter(s) :
     wantedspace : wanted space
   Input/output parameter(s) :
     *pbuf : buffer
   Output parameter(s) :
     *pdata : pointer to end (endoffset)
     *pobtainedspace : obtained space (from end to last)
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_wishspace(netwib_buf *pbuf,
                                netwib_uint32 wantedspace,
                                netwib_data *pdata,
                                netwib_uint32 *pobtainedspace);

/*-------------------------------------------------------------*/
/* Name : netwib_buf_cmp
   Description :
     Compare two netwib_buf.
   Input parameter(s) :
     *pbuf1 : netwib_buf to compare with buf2
     *pbuf2 : netwib_buf to compare with buf1
   Input/output parameter(s) :
   Output parameter(s) :
     *pcmp :
       NETWIB_CMP_LT : buf1<buf2
       NETWIB_CMP_EQ : if buf1 and buf2 are equal
       NETWIB_CMP_GT : buf1>buf2
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_cmp(netwib_constbuf *pbuf1,
                          netwib_constbuf *pbuf2,
                          netwib_cmp *pcmp);
/* ignore case */
netwib_err netwib_buf_casecmp(netwib_constbuf *pbuf1,
                              netwib_constbuf *pbuf2,
                              netwib_cmp *pcmp);
/* compare to a string */
netwib_err netwib_buf_cmp_string(netwib_constbuf *pbuf1,
                                 netwib_conststring string2,
                                 netwib_cmp *pcmp);
netwib_err netwib_buf_casecmp_string(netwib_constbuf *pbuf1,
                                     netwib_conststring string2,
                                     netwib_cmp *pcmp);

/*-------------------------------------------------------------*/
/* Name : netwib_buf_shift
   Description :
     Shift data in a buf.
   Input parameter(s) :
     offset : offset
     truncbegend : truncate on begin/end edges (otherwise,
                   truncate only on first/last edges)
   Input/output parameter(s) :
     *pbuf : netwib_buf updated
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
   Examples :
     buf contains         "   data  "
     shift(buf,-1,0) =>   "  data   "
     shift(buf,-1,1) =>   "   ata   " (d is truncated)
     shift(buf,+1,0) =>   "    data "
     shift(buf,+4,0) =>   "       data" (if buffer is malloced)
     shift(buf,+4,0) =>   "       da" (if buffer is external)
     shift(buf,+1,1) =>   "    dat  " (a is truncated)
   Note :
     Flag NETWIB_BUF_FLAGS_CANSLIDE has no effect on this
     function (this is logical).
*/
netwib_err netwib_buf_shift(netwib_buf *pbuf,
                            netwib_int32 offset,
                            netwib_bool truncbegend);

/*-------------------------------------------------------------*/
/* Name : netwib_buf_close
   Description :
     Close buf, eventually freeing data it contains.
   Input parameter(s) :
   Input/output parameter(s) :
     *pbuf : buffer closed
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
   Note :
     This function is only needed for a buffer initialized
     with netwib_buf_init_malloc, or if flag
     NETWIB_BUF_FLAGS_CANALLOC is set.
*/
netwib_err netwib_buf_close(netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* Frequently needed defines */
#define netwib__buf_reinit(pbuf) { (pbuf)->beginoffset = 0; (pbuf)->endoffset = 0; if (((pbuf)->flags & NETWIB_BUF_FLAGS_SENSITIVE) && !((pbuf)->flags & NETWIB_BUF_FLAGS_SENSITIVE_READONLY)) { netwib_c_memset((pbuf)->totalptr, 0, (pbuf)->totalsize); } }
#define netwib__buf_erase(pbuf) { if (((pbuf)->flags & NETWIB_BUF_FLAGS_SENSITIVE) && !((pbuf)->flags & NETWIB_BUF_FLAGS_SENSITIVE_READONLY)) { netwib_c_memset((pbuf)->totalptr+(pbuf)->beginoffset, 0, (pbuf)->endoffset-(pbuf)->beginoffset); } (pbuf)->endoffset = (pbuf)->beginoffset; }
#define netwib__buf_ref_data_ptr(pbuf) ((pbuf)->totalptr + (pbuf)->beginoffset)
#define netwib__buf_ref_data_size(pbuf) ((pbuf)->endoffset -(pbuf)->beginoffset)
#define netwib__buf_ref_data_sizenull(pbuf) ((pbuf)!=NULL?netwib__buf_ref_data_size(pbuf):0)

