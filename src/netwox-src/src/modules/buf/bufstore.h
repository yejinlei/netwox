
/*
  This module is adapted from netwib/dat/priv/bufstore.c.
  Cross changes are checked using a private script to ensure they
  are both updated when one changes.
*/

/*-------------------------------------------------------------*/
/***************************************************************
 * To optimize the number of malloc :                          *
 *  - we allocate "NETWOX_BUFSTORE_ALIGN+datasize+1"==ptr      *
 *  - we set data to ptr+NETWOX_BUFSTORE_ALIGN                 *
 *  - we set datasize to size of buffer                        *
 *  - we copy buffer data to data                              *
 * Note : the "+1" is to allow to use netwib_buf_init_ext_...  *
 * ...datafilled.                                              *
 ***************************************************************/
typedef struct {
  netwib_data data;
  netwib_uint32 datasize;
} netwox_bufstore;
#define NETWOX_BUFSTORE_ALIGN ((sizeof(netwox_bufstore) + sizeof(void*) - 1) & ~(sizeof(void*) - 1))

/*-------------------------------------------------------------*/
netwib_err netwox_bufstore_create(netwib_constbuf *pbuf,
                                  netwib_ptr *ppitem);
netwib_err netwox_bufstore_decode(netwib_constptr pitem,
                                  netwib_bufext *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_bufstore_erase(netwib_ptr pitem);
netwib_err netwox_bufstore_duplicate(netwib_constptr pitem,
                                     netwib_ptr *pdupofitem);

/*-------------------------------------------------------------*/
/* a ring of bufstore */
#define netwox_bufstore_ring_init(ppring) netwib_ring_init(&netwox_bufstore_erase,&netwox_bufstore_duplicate,ppring)
#define netwox_bufstore_ring_close(ppring) netwib_ring_close(ppring,NETWIB_TRUE)

/*-------------------------------------------------------------*/
/* a hash of bufstore */
#define netwox_bufstore_hash_init(pphash) netwib_hash_init(&netwox_bufstore_erase,&netwox_bufstore_duplicate,pphash)
#define netwox_bufstore_hash_close(pphash) netwib_hash_close(pphash,NETWIB_TRUE)

/*-------------------------------------------------------------*/
/* sometimes, the key of a hash is not a buf but an integer */
#define NETWOX_BUFSTORE_HASH_KEY_SIZE 4
netwib_err netwox_bufstore_hash_key(netwib_byte keyarray[NETWOX_BUFSTORE_HASH_KEY_SIZE],
                                    netwib_uint32 id,
                                    netwib_buf *pkey);


