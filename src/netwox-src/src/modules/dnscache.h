
/*-------------------------------------------------------------*/
/* this structure can be used by two threads */
typedef struct {
  netwib_hash *phashhost;
  netwib_hash *phaship;
  netwib_thread_rwlock *prwlock;
} netwox_dnscache;

/*-------------------------------------------------------------*/
netwib_err netwox_dnscache_init(netwib_bool supportipfunction,
                                netwib_bool supporthostfunction,
                                netwox_dnscache *pdnscache);
netwib_err netwox_dnscache_close(netwox_dnscache *pdnscache);

/*-------------------------------------------------------------*/
netwib_err netwox_dnscache_ip(netwox_dnscache *pdnscache,
                              netwib_constbuf *phost,
                              netwib_ip *pip);

/*-------------------------------------------------------------*/
netwib_err netwox_dnscache_host(netwox_dnscache *pdnscache,
                                netwib_constip *pip,
                                netwib_buf *phost);

