
/*-------------------------------------------------------------*/
/***************************************************************
 * An IP range is of the form :                                *
 *                                   ip4inf     ip4sup         *
 *   1.2.3.4                  :     1.2.3.4     1.2.3.4        *
 *   1.2.3.4-5.6.7.8          :     1.2.3.4     5.6.7.8        *
 *   1.2.3.0/255.255.255.0    :     1.2.3.0   1.2.3.255        *
 *   1.2.3.0/24               :     1.2.3.0   1.2.3.255        *
 *   1.2.3/255.255.255.0      :     1.2.3.0   1.2.3.255        *
 *   1.2.3/24                 :     1.2.3.0   1.2.3.255        *
 *   1.2.3.0%255.255.255.O    :     1.2.3.1   1.2.3.254        *
 *   1.2.3.0%24               :     1.2.3.1   1.2.3.254        *
 *   1.2.3%255.255.255.O      :     1.2.3.1   1.2.3.254        *
 *   1.2.3%24                 :     1.2.3.1   1.2.3.254        *
 *   fec0:0:0:1::1            :  fec0:0:0:1::1 fec0:0:0:1::1   *
 * Notes :                                                     *
 *  - '%' has the same meaning as '/', except that the         *
 *    broadcast addresses are excluded from range.             *
 *  - It is possible to use a hostname range as input. In this *
 *    case the separator is '=' (because '-' is a valid        *
 *    hostname separator).                                     *
 *                                                             *
 * An netwib_ips is of the form :                              *
 *   ip,ip,iprange,iprange                                     *
 *   all,!ip,!iprange                                          *
 *                                                             *
 * Complete examples :                                         *
 *   1.2.3.4                                                   *
 *   1.2.3.4-1.2.3.5                                           *
 *   1.2.3.4,5.6.7.8                                           *
 *   1.2.3.4,1.2.3.56-1.2.3.58                                 *
 *   all,!1.2.3.4,!1.2.4.4-1.2.4.6                             *
 *   hostname                                                  *
 *   host1=host2                                               *
 *   host1,host2                                               *
 ***************************************************************/

/*-------------------------------------------------------------*/
/***************************************************************
 * Those functions ignores following error cases :             *
 *  - if we try to add a value already in the list             *
 *  - if we try to remove a value not in the list              *
 ***************************************************************/

/*-------------------------------------------------------------*/
typedef struct netwib_ips netwib_ips;
typedef const netwib_ips netwib_constips;

/*-------------------------------------------------------------*/
/* Name : netwib_ips_init
   Description :
     Initialize a netwib_ips used to store IP list.
   Input parameter(s) :
     inittype : if future added items will be sorted and/or unique
   Input/output parameter(s) :
   Output parameter(s) :
     **ppips : netwib_ips allocated and initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
typedef enum {
  NETWIB_IPS_INITTYPE_SORTUNIQ = 1, /* sorted and unique */
  NETWIB_IPS_INITTYPE_NOTSORTUNIQ = 2, /* not sorted and unique */
  NETWIB_IPS_INITTYPE_NOTSORTNOTUNIQ = 3 /* not sorted and not
                                            unique (duplicates
                                            are not removed) */
} netwib_ips_inittype;
netwib_err netwib_ips_init(netwib_ips_inittype inittype,
                           netwib_ips **ppips);
#define netwib_ips_initdefault(ppips) netwib_ips_init(NETWIB_IPS_INITTYPE_SORTUNIQ,ppips)

/*-------------------------------------------------------------*/
/* Name : netwib_ips_close
   Description :
     Close a netwib_ips.
   Input parameter(s) :
   Input/output parameter(s) :
     **ppips : netwib_ips closed
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ips_close(netwib_ips **ppips);

/*-------------------------------------------------------------*/
/* Name : netwib_ips_add_ip
   Description :
     Add an address to the netwib_ips.
   Input parameter(s) :
     ip : address to add
   Input/output parameter(s) :
     *pips : netwib_ips where to work
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ips_add_ip(netwib_ips *pips,
                             netwib_constip *pip);

/*-------------------------------------------------------------*/
/* Name : netwib_ips_add_iprange
   Description :
     Add a range of addresses to the netwib_ips.
   Input parameter(s) :
     infip : inferior ip
     supip : superior ip
   Input/output parameter(s) :
     *pips : netwib_ips where to work
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ips_add_iprange(netwib_ips *pips,
                                  netwib_constip *pinfip,
                                  netwib_constip *psupip);

/*-------------------------------------------------------------*/
/* Name : netwib_ips_add_ips
   Description :
     Add a list of addresses to the netwib_ips.
   Input parameter(s) :
     *pipstoadd : netwib_ips to add
   Input/output parameter(s) :
     *pips : netwib_ips where to work
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ips_add_ips(netwib_ips *pips,
                              netwib_constips *pipstoadd);

/*-------------------------------------------------------------*/
/* Name : netwib_ips_add_buf
   Description :
     Update a netwib_ips with a string like "1.2.3.4-1.2.3.5".
   Input parameter(s) :
     pbuf : buffer containing string
   Input/output parameter(s) :
     *pips : netwib_ips updated
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
   Note :
     If an error occurs during insertion, result will only
     contain partial data. It's developer's job to use a
     temporary netwib_ips to deal with such errors.
*/
netwib_err netwib_ips_add_buf(netwib_ips *pips,
                              netwib_constbuf *pbuf);

/*-------------------------------------------------------------*/
/* Name : netwib_ips_add_kbd
   Description :
     Update a netwib_ips with data entered through keyboard.
   Input parameter(s) :
     *pmessage : message to print before
     *pdefaultlist : default list to use if user enters nothing
                     if NULL, there is no default
   Input/output parameter(s) :
     *pips : netwib_ips updated
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ips_add_kbd(netwib_ips *pips,
                              netwib_constbuf *pmessage,
                              netwib_constbuf *pdefaultlist);

/*-------------------------------------------------------------*/
/* Name : netwib_ips_del_ip
   Description :
     Del an address to the netwib_ips.
   Input parameter(s) :
     ip : address to delete
   Input/output parameter(s) :
     *pips : netwib_ips where to work
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ips_del_ip(netwib_ips *pips,
                             netwib_constip *pip);

/*-------------------------------------------------------------*/
/* Name : netwib_ips_del_iprange
   Description :
     Del a range of addresses to the netwib_ips.
   Input parameter(s) :
     infip : inferior ip
     supip : superior ip
   Input/output parameter(s) :
     *pips : netwib_ips where to work
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ips_del_iprange(netwib_ips *pips,
                                  netwib_constip *pinfip,
                                  netwib_constip *psupip);

/*-------------------------------------------------------------*/
/* Name : netwib_ips_del_ips
   Description :
     Remove a list of addresses to the netwib_ips.
   Input parameter(s) :
     *pipstodel : netwib_ips to remove
   Input/output parameter(s) :
     *pips : netwib_ips where to work
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ips_del_ips(netwib_ips *pips,
                              netwib_constips *pipstodel);

/*-------------------------------------------------------------*/
/* Name : netwib_ips_contains_xyz
   Description :
     Check if a netwib_ip is in the list.
   Input parameter(s) :
     ip : netwib_ip to find
   Input/output parameter(s) :
     *pips : netwib_ips containing the list of addresses
   Output parameter(s) :
     *pyes : true if netwib_ip is found
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ips_contains_ip(netwib_constips *pips,
                                  netwib_constip *pip,
                                  netwib_bool *pyes);
netwib_err netwib_ips_contains_iprange(netwib_constips *pips,
                                       netwib_constip *pinfip,
                                       netwib_constip *psupip,
                                       netwib_bool *pyes);

/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_IPS_ENCODETYPE_HNRANGE = 1,/* "host1=host2,etc." */
  NETWIB_IPS_ENCODETYPE_IPRANGE,    /* "1.2.3.4-1.2.3.5,etc." */
  NETWIB_IPS_ENCODETYPE_IPNUMBER,   /* "1.1.1.0/24,etc." */
  NETWIB_IPS_ENCODETYPE_IPMASK,     /* "1.1.1.0/255.255.255.255" */
  NETWIB_IPS_ENCODETYPE_HNBEST = NETWIB_IPS_ENCODETYPE_HNRANGE,
  NETWIB_IPS_ENCODETYPE_IPBEST = NETWIB_IPS_ENCODETYPE_IPRANGE,
  NETWIB_IPS_ENCODETYPE_BEST = NETWIB_IPS_ENCODETYPE_IPBEST
} netwib_ips_encodetype;

/*-------------------------------------------------------------*/
/* Name : netwib_buf_append_ips
   Description :
     Append a string representing a netwib_ips.
   Input parameter(s) :
     *pips : netwib_ips to append
   Input/output parameter(s) :
     pbuf : buffer updated
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_append_ips(netwib_constips *pips,
                                 netwib_ips_encodetype encodetype,
                                 netwib_buf *pbuf);

