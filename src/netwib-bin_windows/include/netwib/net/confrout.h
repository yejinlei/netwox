
/*-------------------------------------------------------------*/
typedef struct netwib_conf_routes_index netwib_conf_routes_index;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 devnum;
  netwib_ip dst;
  netwib_ip mask; /* set for IPv4 */
  netwib_uint32 prefix; /* set for IPv6 */
  netwib_bool srcset;
  netwib_ip src;
  netwib_bool gwset;
  netwib_ip gw;
  netwib_uint32 metric;
} netwib_conf_routes;
/*
  srcunset and gwunset : dst is a local device
  srcset and gwunset   : dst in on LAN
  srcunset and gwset   : error
  srcset and gwset     : dst is through gw
*/

/*-------------------------------------------------------------*/
/* Name : netwib_conf_routes_index_init
   Description :
     Initialize a netwib_conf_routes_index used to loop through
     a netwib_conf_routes.
   Input parameter(s) :
     pitem : address of netwib_conf_routes which will be set
             on each call of netwib_conf_routes_next
   Input/output parameter(s) :
   Output parameter(s) :
     **ppindex : netwib_conf_routes_index allocated and initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_conf_routes_index_init(netwib_conf_routes *pitem,
                                         netwib_conf_routes_index **ppindex);

/*-------------------------------------------------------------*/
/* Name : netwib_conf_routes_index_close
   Description :
     Close a netwib_conf_routes_index.
   Input parameter(s) :
   Input/output parameter(s) :
     **ppindex : netwib_conf_routes_index to close
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_conf_routes_index_close(netwib_conf_routes_index **ppindex);

/*-------------------------------------------------------------*/
/* Name : netwib_conf_routes_index_next
   Description :
     Get next entry.
   Input parameter(s) :
   Input/output parameter(s) :
     *pindex : netwib_conf_routes_index to work on
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_DATAEND : end reached
*/
netwib_err netwib_conf_routes_index_next(netwib_conf_routes_index *pindex);

/*-------------------------------------------------------------*/
/* Name : netwib_conf_routes_display
   Description :
     Print routes configuration.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf receiving data
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_conf_routes_display(void);
netwib_err netwib_buf_append_conf_routes(netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* Name : netwib_conf_routes_reach_eth
   Description :
     Determine device, sources addresses and destination Ethernet
     address to use to send an IP packet to a host.
   Input parameter(s) :
     dstip : host to reach
   Input/output parameter(s) :
   Output parameter(s) :
     *pdevice : device to use
     *psrceth : Ethernet source address to use
     *pdsteth : Ethernet destination address to use
     *psrcip : IP source address to use
   Normal return values :
     NETWIB_ERR_OK : ok (set: all)
     NETWIB_ERR_ROUTELOCALDEV : hostip is the address of a
                                 local device (set : device)
     NETWIB_ERR_ROUTENOTETHER : Ethernet is not used to reach
                                 destination (set : none)
     NETWIB_ERR_ROUTENOTFOUND : the routing table doesn't contain
                                 a route to hostip (set : none)
*/
netwib_err netwib_conf_routes_reach_eth(netwib_constip *pdstip,
                                        netwib_buf *pdevice,
                                        netwib_eth *psrceth,
                                        netwib_eth *pdsteth,
                                        netwib_ip *psrcip);

/*-------------------------------------------------------------*/
/* Name : netwib_conf_routes_reach_ip
   Description :
     Determine device and addresses to use to send an IP packet
     to a host.
   Input parameter(s) :
     dstip : host to reach
   Input/output parameter(s) :
   Output parameter(s) :
     *pdevice : device to use (for example to sniff)
     *psrcip : IP source address to use
   Normal return values :
     NETWIB_ERR_OK : ok (set: all)
     NETWIB_ERR_ROUTELOCALDEV : hostip is the address of a
                                 local device (set : *pdevice)
     NETWIB_ERR_ROUTENOTFOUND : the routing table doesn't contain
                                 a route to hostip (set : none)
*/
netwib_err netwib_conf_routes_reach_ip(netwib_constip *pdstip,
                                       netwib_buf *pdevice,
                                       netwib_ip *psrcip);

