
/*
  This module creates a ring/hash containing an array of buffers.
*/

/*-------------------------------------------------------------*/
/* create the array */
netwib_err netwox_bufarraystore_create(netwib_ptr *ppitem);

/*-------------------------------------------------------------*/
/* add a buf at the end of the array */
netwib_err netwox_bufarraystore_add_buf(netwib_ptr pitem,
                                        netwib_constbuf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_bufarraystore_erase(netwib_ptr pitem);
netwib_err netwox_bufarraystore_duplicate(netwib_constptr pitem,
                                          netwib_ptr *pdupofitem);

/*-------------------------------------------------------------*/
/* a ring of bufarraystore */
#define netwox_bufarraystore_ring_init(ppring) netwib_ring_init(&netwox_bufarraystore_erase,&netwox_bufarraystore_duplicate,ppring)
#define netwox_bufarraystore_ring_close(ppring) netwib_ring_close(ppring,NETWIB_TRUE)

/*-------------------------------------------------------------*/
/* a hash of bufarraystore */
#define netwox_bufarraystore_hash_init(pphash) netwib_hash_init(&netwox_bufarraystore_erase,&netwox_bufarraystore_duplicate,pphash)
#define netwox_bufarraystore_hash_close(pphash) netwib_hash_close(pphash,NETWIB_TRUE)

