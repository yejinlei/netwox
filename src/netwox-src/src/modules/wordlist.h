
/*-------------------------------------------------------------*/
/* a wordlist if a ring of buffers (stored as bufstore) */

/*-------------------------------------------------------------*/
#define netwox_wordlist_init(ppring) netwox_bufstore_ring_init(ppring)
#define netwox_wordlist_close(ppring) netwox_bufstore_ring_close(ppring)

/*-------------------------------------------------------------*/
#define netwox_wordlist_item_create(pbuf,ppitem) netwox_bufstore_create(pbuf,ppitem)
#define netwox_wordlist_item_decode(pitem,pbuf) netwox_bufstore_decode(pitem,pbuf)

/*-------------------------------------------------------------*/
/* add one word */
netwib_err netwox_wordlist_add_buf(netwib_ring *pring,
                                   netwib_constbuf *pbuf);

/*-------------------------------------------------------------*/
/* add all words of a file (one word per line) */
netwib_err netwox_wordlist_add_file(netwib_ring *pring,
                                    netwib_constbuf *pfilename);

