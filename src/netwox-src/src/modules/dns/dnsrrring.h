
/*-------------------------------------------------------------*/
/* this ring contains items of type netwox_dnsrr */

/*-------------------------------------------------------------*/
/* this function call netwox_dnsrr_init to initialize it */
netwib_err netwox_dnsrrring_item_create(netwib_ptr *ppitem);

/*-------------------------------------------------------------*/
/* this function call netwox_dnsrr_close to close it */
netwib_err netwox_dnsrrring_item_erase(netwib_ptr pitem);

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrrring_item_duplicate(netwib_constptr pitem,
                                           netwib_ptr *pdupofitem);

/*-------------------------------------------------------------*/
#define netwox_dnsrrring_init(ppring) netwib_ring_init(&netwox_dnsrrring_item_erase,&netwox_dnsrrring_item_duplicate,ppring)
#define netwox_dnsrrring_close(ppring) netwib_ring_close(ppring,NETWIB_TRUE)


