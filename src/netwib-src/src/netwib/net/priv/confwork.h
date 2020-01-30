
/*-------------------------------------------------------------*/
typedef struct {
  netwib_ring *pdevices;
  netwib_ring *pip;
  netwib_ring *parpcache;
  netwib_ring *proutes;
  netwib_ring *plocaldevice;
  netwib_eths *plocaleth;
  netwib_ips *plocalip;
  netwib_ring *pdevicenumbername;
  netwib_ring *pdhcpinfo;
  netwib_ring *pincomplete;
  netwib_bool debug;
  netwib_buf *pdebugbuf;
} netwib_priv_confwork;

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_init(netwib_priv_confwork *pcw);
netwib_err netwib_priv_confwork_close(netwib_priv_confwork *pcw);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_obtain(netwib_priv_confwork *pcw,
                                       netwib_bool debug,
                                       netwib_buf *pdebugbuf);
netwib_err netwib_priv_confwork_obtain_arpcache(netwib_priv_confwork *pcw);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_debug(netwib_priv_confwork *pcw,
                                      netwib_conststring title);
