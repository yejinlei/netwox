
/*-------------------------------------------------------------*/
typedef struct {
  netwib_ring *pdevices;
  netwib_ring *pip;
  netwib_ring *parpcache;
  netwib_ring *proutes;
} netwib_priv_conf_t;
extern netwib_priv_conf_t netwib_priv_conf;
extern netwib_bool netwib_priv_conf_needtobeupdated;

/*-------------------------------------------------------------*/
netwib_err netwib_priv_conf_rdlock(void);
netwib_err netwib_priv_conf_rdunlock(void);
netwib_err netwib_priv_conf_wrlock(void);
netwib_err netwib_priv_conf_wrunlock(void);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_conf_init(void);
netwib_err netwib_priv_conf_close(void);
netwib_err netwib_priv_conf_update(void);
#define netwib__priv_conf_eventuallyupdate() {if (netwib_priv_conf_needtobeupdated) {netwib_er(netwib_priv_conf_update());}}
