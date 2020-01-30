
/*-------------------------------------------------------------*/
typedef struct netwib_conf_arpcache_index netwib_conf_arpcache_index;

/*-------------------------------------------------------------*/
/***************************************************************
 * This structure stores :                                     *
 *  - ARP cache for IPv4                                       *
 *  - neighbors for IPv6                                       *
 ***************************************************************/

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 devnum;
  netwib_eth eth;
  netwib_ip ip;
} netwib_conf_arpcache;

/*-------------------------------------------------------------*/
/* Name : netwib_conf_arpcache_index_init
   Description :
     Initialize a netwib_conf_arpcache_index used to loop through
     a netwib_conf_arpcache.
   Input parameter(s) :
     pitem : address of netwib_conf_arpcache which will be set
             on each call of netwib_conf_arpcache_next
   Input/output parameter(s) :
   Output parameter(s) :
     **ppindex : netwib_conf_arpcache_index allocated and initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_conf_arpcache_index_init(netwib_conf_arpcache *pitem,
                                           netwib_conf_arpcache_index **ppindex);

/*-------------------------------------------------------------*/
/* Name : netwib_conf_arpcache_index_close
   Description :
     Close a netwib_conf_arpcache_index.
   Input parameter(s) :
   Input/output parameter(s) :
     **ppindex : netwib_conf_arpcache_index to close
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_conf_arpcache_index_close(netwib_conf_arpcache_index **ppindex);

/*-------------------------------------------------------------*/
/* Name : netwib_conf_arpcache_index_next
   Description :
     Get next entry.
   Input parameter(s) :
   Input/output parameter(s) :
     *pindex : netwib_conf_arpcache_index to work on
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_DATAEND : end reached
*/
netwib_err netwib_conf_arpcache_index_next(netwib_conf_arpcache_index *pindex);

/*-------------------------------------------------------------*/
/* Name : netwib_conf_arpcache_display
   Description :
     Print arp configuration.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf receiving data
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_conf_arpcache_display(void);
netwib_err netwib_buf_append_conf_arpcache(netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* Name : netwib_conf_arpcache_add
   Description :
     Add an entry in the cache. Then resolving functions
     can find the value.
   Input parameter(s) :
     *pdevice : can be NULL if unknown
     *peth : Ethernet address
     *pip : IP address
   Input/output parameter(s) :
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_conf_arpcache_add(netwib_constbuf *pdevice,
                                    netwib_consteth *peth,
                                    netwib_constip *pip);
