
/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 devnum;
  netwib_buf device;
  /* ip configuration */
  netwib_ip ip;
  netwib_ip mask;
  netwib_uint32 prefix;
  /* point to point configuration */
  netwib_bool ispointtopoint;
  netwib_ip pointtopointip;
} netwib_priv_confwork_ip;

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_ip_init(netwib_priv_confwork_ip **ppitem);
netwib_err netwib_priv_confwork_ip_close(netwib_priv_confwork_ip **ppitem);
netwib_err netwib_priv_confwork_ip_add(netwib_priv_confwork *pcw,
                                       netwib_priv_confwork_ip *pitem);
netwib_err netwib_priv_confwork_ip_erase(netwib_ptr pitem);

/*-------------------------------------------------------------*/
netwib_err netwib_conf_ip_init(netwib_conf_ip **ppitem);
netwib_err netwib_conf_ip_close(netwib_conf_ip **ppitem);
netwib_err netwib_conf_ip_add(netwib_priv_conf_t *pc,
                              netwib_conf_ip *pitem);
netwib_err netwib_conf_ip_erase(netwib_ptr pitem);


