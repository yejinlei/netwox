
/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 devnum;
  netwib_buf device;
  netwib_eth eth;
  netwib_ip ip;
} netwib_priv_confwork_arpcache;

/*-------------------------------------------------------------*/
/* deal with one item */
netwib_err netwib_priv_confwork_arpcache_init(netwib_priv_confwork_arpcache **ppitem);
netwib_err netwib_priv_confwork_arpcache_close(netwib_priv_confwork_arpcache **ppitem);
netwib_err netwib_priv_confwork_arpcache_add(netwib_priv_confwork *pcw,
                                             netwib_priv_confwork_arpcache *pitem);
netwib_err netwib_priv_confwork_arpcache_erase(netwib_ptr pitem);

/*-------------------------------------------------------------*/
netwib_err netwib_conf_arpcache_init(netwib_conf_arpcache **ppitem);
netwib_err netwib_conf_arpcache_close(netwib_conf_arpcache **ppitem);
netwib_err netwib_conf_arpcache_addi(netwib_priv_conf_t *pc,
                                     netwib_conf_arpcache *pitem);
netwib_err netwib_conf_arpcache_erase(netwib_ptr pitem);

/*-------------------------------------------------------------*/
/* those functions directly work on global configuration */
netwib_err netwib_priv_confglo_arpcache_add(netwib_constbuf *device,
                                            netwib_consteth *peth,
                                            netwib_constip *pip);
netwib_err netwib_priv_confglo_arpcache_ip(netwib_consteth *peth,
                                           netwib_ip *pip);
netwib_err netwib_priv_confglo_arpcache_eth(netwib_constip *pip,
                                            netwib_eth *peth);
/* those functions reload the arpcache and query it */
netwib_err netwib_priv_confrel_arpcache_ip(netwib_consteth *peth,
                                           netwib_ip *pip);
netwib_err netwib_priv_confrel_arpcache_eth(netwib_constip *pip,
                                            netwib_eth *peth);

