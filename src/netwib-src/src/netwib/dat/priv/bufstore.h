
/*-------------------------------------------------------------*/
/***************************************************************
 * To optimize the number of malloc :                          *
 *  - we allocate "NETWIB_PRIV_BUFSTORE_ALIGN+datasize+1"==ptr *
 *  - we set data to ptr+NETWIB_PRIV_BUFSTORE_ALIGN            *
 *  - we set datasize to size of buffer                        *
 *  - we copy buffer data to data                              *
 * Note : the "+1" is to allow to use netwib_buf_init_ext_...  *
 * ...datafilled.                                              *
 ***************************************************************/
typedef struct {
  netwib_data data;
  netwib_uint32 datasize;
} netwib_priv_bufstore;
#define NETWIB_PRIV_BUFSTORE_ALIGN ((sizeof(netwib_priv_bufstore) + sizeof(void*) - 1) & ~(sizeof(void*) - 1))

/*-------------------------------------------------------------*/
netwib_err netwib_priv_bufstore_create(netwib_constbuf *pbuf,
                                       netwib_ptr *ppitem);
netwib_err netwib_priv_bufstore_decode(netwib_constptr pitem,
                                       netwib_bufext *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_bufstore_erase(netwib_ptr pitem);
netwib_err netwib_priv_bufstore_duplicate(netwib_constptr pitem,
                                          netwib_ptr *pdupofitem);

/*-------------------------------------------------------------*/
/* a ring of bufstore */
#define netwib_priv_bufstore_ring_init(ppring) netwib_ring_init(&netwib_priv_bufstore_erase,&netwib_priv_bufstore_duplicate,ppring)
#define netwib_priv_bufstore_ring_close(ppring) netwib_ring_close(ppring,NETWIB_TRUE)

/*-------------------------------------------------------------*/
/* a hash of bufstore */
#define netwib_priv_bufstore_hash_init(pphash) netwib_hash_init(&netwib_priv_bufstore_erase,&netwib_priv_bufstore_duplicate,pphash)
#define netwib_priv_bufstore_hash_close(pphash) netwib_hash_close(pphash,NETWIB_TRUE)
