
/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 devnum;
  netwib_buf device;
  netwib_ip dst;
  netwib_ip mask;
  netwib_uint32 prefix;
  netwib_bool srcset;
  netwib_ip src;
  netwib_bool gwset;
  netwib_ip gw;
  netwib_uint32 metric;
} netwib_priv_confwork_routes;

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_routes_init(netwib_priv_confwork_routes **ppitem);
netwib_err netwib_priv_confwork_routes_close(netwib_priv_confwork_routes **ppitem);
netwib_err netwib_priv_confwork_routes_add(netwib_priv_confwork *pcw,
                                           netwib_priv_confwork_routes *pitem);
netwib_err netwib_priv_confwork_routes_erase(netwib_ptr pitem);

/*-------------------------------------------------------------*/
netwib_err netwib_conf_routes_init(netwib_conf_routes **ppitem);
netwib_err netwib_conf_routes_close(netwib_conf_routes **ppitem);
netwib_err netwib_conf_routes_add(netwib_priv_conf_t *pc,
                                  netwib_conf_routes *pitem);
netwib_err netwib_conf_routes_erase(netwib_ptr pitem);
