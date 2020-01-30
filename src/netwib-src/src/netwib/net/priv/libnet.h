
/*-------------------------------------------------------------*/
typedef struct {
  netwib_spoof_inittype inittype;
  netwib_buf device;
  netwib_device_hwtype hwtype;
  netwib_device_dlttype dlttype;
  netwib_ptr plli;     /* used by libnet 1.0 */
  netwib_ptr plibnett; /* used by libnet 1.1 */
  netwib_ptr padapter; /* used by packet.dll */
  netwib_ptr ppacket;  /* used by packet.dll */
  int fd;
} netwib_priv_libnet;

/*-------------------------------------------------------------*/
netwib_err netwib_priv_libnet_supports(netwib_constbuf *pdevice,
                                       netwib_spoof_inittype inittype,
                                       netwib_bool *pyes);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_libnet_init(netwib_constbuf *pdevice,
                                   netwib_spoof_inittype inittype,
                                   netwib_priv_libnet *plib);
netwib_err netwib_priv_libnet_close(netwib_priv_libnet *plib);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_libnet_write(netwib_priv_libnet *plib,
                                    netwib_constbuf *pbuf);
netwib_err netwib_priv_libnet_wait(netwib_priv_libnet *plib,
                                   netwib_io_waytype way,
                                   netwib_consttime *pabstime,
                                   netwib_bool *pevent);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_libnet_get_dlt(netwib_priv_libnet *plib);
