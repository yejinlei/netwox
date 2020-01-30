
/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 devnum;
  netwib_buf device;
  netwib_buf deviceeasy;
  netwib_uint32 mtu;
  netwib_device_hwtype hwtype;
  netwib_eth eth;
} netwib_priv_confwork_devices;

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_devices_init(netwib_priv_confwork_devices **ppitem);
netwib_err netwib_priv_confwork_devices_close(netwib_priv_confwork_devices **ppitem);
netwib_err netwib_priv_confwork_devices_add(netwib_priv_confwork *pcw,
                                            netwib_priv_confwork_devices *pitem);
netwib_err netwib_priv_confwork_devices_erase(netwib_ptr pitem);

/*-------------------------------------------------------------*/
netwib_err netwib_conf_devices_init(netwib_conf_devices **ppitem);
netwib_err netwib_conf_devices_close(netwib_conf_devices **ppitem);
netwib_err netwib_conf_devices_add(netwib_priv_conf_t *pc,
                                   netwib_conf_devices *pitem);
netwib_err netwib_conf_devices_erase(netwib_ptr pitem);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_conf_device_info(netwib_constbuf *pdevice,
                                        netwib_buf *prealdevice,
                                        netwib_uint32 *pmtu,
                                        netwib_device_hwtype *phwtype,
                                        netwib_eth *peth);
