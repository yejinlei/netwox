
/*-------------------------------------------------------------*/
typedef struct netwib_conf_devices_index netwib_conf_devices_index;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 devnum;
  netwib_buf device;
  netwib_buf deviceeasy;
  netwib_uint32 mtu;
  netwib_device_hwtype hwtype;
  netwib_eth eth;
} netwib_conf_devices;

/*-------------------------------------------------------------*/
/* Name : netwib_conf_devices_index_init
   Description :
     Initialize a netwib_conf_devices_index used to loop through
     a netwib_conf_devices.
   Input parameter(s) :
     pitem : address of netwib_conf_devices which will be set
             on each call of netwib_conf_devices_next
   Input/output parameter(s) :
   Output parameter(s) :
     **ppindex : netwib_conf_devices_index allocated and initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_conf_devices_index_init(netwib_conf_devices *pitem,
                                          netwib_conf_devices_index **ppindex);

/*-------------------------------------------------------------*/
/* Name : netwib_conf_devices_index_close
   Description :
     Close a netwib_conf_devices_index.
   Input parameter(s) :
   Input/output parameter(s) :
     **ppindex : netwib_conf_devices_index to close
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_conf_devices_index_close(netwib_conf_devices_index **ppindex);

/*-------------------------------------------------------------*/
/* Name : netwib_conf_devices_index_next
   Description :
     Get next entry.
   Input parameter(s) :
   Input/output parameter(s) :
     *pindex : netwib_conf_devices_index to work on
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_DATAEND : end reached
*/
netwib_err netwib_conf_devices_index_next(netwib_conf_devices_index *pindex);

/*-------------------------------------------------------------*/
/* Name : netwib_conf_devices_display
   Description :
     Print devices configuration.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf receiving data
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_conf_devices_display(void);
netwib_err netwib_buf_append_conf_devices(netwib_buf *pbuf);
