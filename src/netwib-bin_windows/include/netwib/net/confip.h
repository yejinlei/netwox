
/*-------------------------------------------------------------*/
typedef struct netwib_conf_ip_index netwib_conf_ip_index;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 devnum;
  /* ip configuration */
  netwib_ip ip;
  netwib_ip mask; /* set for IPv4 */
  netwib_uint32 prefix; /* set for IPv6 */
  /* point to point configuration */
  netwib_bool ispointtopoint;
  netwib_ip pointtopointip;
} netwib_conf_ip;

/*-------------------------------------------------------------*/
/* Name : netwib_conf_ip_index_init
   Description :
     Initialize a netwib_conf_ip_index used to loop through
     a netwib_conf_ip.
   Input parameter(s) :
     pitem : address of netwib_conf_ip which will be set
             on each call of netwib_conf_ip_next
   Input/output parameter(s) :
   Output parameter(s) :
     **ppindex : netwib_conf_ip_index allocated and initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_conf_ip_index_init(netwib_conf_ip *pitem,
                                     netwib_conf_ip_index **ppindex);

/*-------------------------------------------------------------*/
/* Name : netwib_conf_ip_index_close
   Description :
     Close a netwib_conf_ip_index.
   Input parameter(s) :
   Input/output parameter(s) :
     **ppindex : netwib_conf_ip_index to close
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_conf_ip_index_close(netwib_conf_ip_index **ppindex);

/*-------------------------------------------------------------*/
/* Name : netwib_conf_ip_index_next
   Description :
     Get next entry.
   Input parameter(s) :
   Input/output parameter(s) :
     *pindex : netwib_conf_ip_index to work on
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_DATAEND : end reached
*/
netwib_err netwib_conf_ip_index_next(netwib_conf_ip_index *pindex);

/*-------------------------------------------------------------*/
/* Name : netwib_conf_ip_display
   Description :
     Print ip configuration.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf receiving data
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_conf_ip_display(void);
netwib_err netwib_buf_append_conf_ip(netwib_buf *pbuf);
