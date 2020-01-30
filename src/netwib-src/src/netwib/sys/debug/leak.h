
/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
netwib_err netwib_debug_leak_init(void);
netwib_err netwib_debug_leak_close(void);

typedef enum {
  NETWIB_DEBUG_LEAKTYPE_MEM = 1,
  NETWIB_DEBUG_LEAKTYPE_FD,
  NETWIB_DEBUG_LEAKTYPE_BUF,
  NETWIB_DEBUG_LEAKTYPE_RING,
  NETWIB_DEBUG_LEAKTYPE_RINGI,
  NETWIB_DEBUG_LEAKTYPE_HASH,
  NETWIB_DEBUG_LEAKTYPE_HASHI,
  NETWIB_DEBUG_LEAKTYPE_ARRAY,
  NETWIB_DEBUG_LEAKTYPE_DIR,
  NETWIB_DEBUG_LEAKTYPE_IO,
  NETWIB_DEBUG_LEAKTYPE_THREAD,
  NETWIB_DEBUG_LEAKTYPE_THREAD_MUTEX,
  NETWIB_DEBUG_LEAKTYPE_THREAD_RWLOCK,
  NETWIB_DEBUG_LEAKTYPE_THREAD_COND,
  NETWIB_DEBUG_LEAKTYPE_THREAD_TSD,
  NETWIB_DEBUG_LEAKTYPE_WAIT,
  NETWIB_DEBUG_LEAKTYPE_ETHS,
  NETWIB_DEBUG_LEAKTYPE_IPS,
  NETWIB_DEBUG_LEAKTYPE_PORTS,
  NETWIB_DEBUG_LEAKTYPE_ETHS_INDEX,
  NETWIB_DEBUG_LEAKTYPE_IPS_INDEX,
  NETWIB_DEBUG_LEAKTYPE_PORTS_INDEX,
  NETWIB_DEBUG_LEAKTYPE_CONF_DEVICES_INDEX,
  NETWIB_DEBUG_LEAKTYPE_CONF_IP_INDEX,
  NETWIB_DEBUG_LEAKTYPE_CONF_ARPCACHE_INDEX,
  NETWIB_DEBUG_LEAKTYPE_CONF_ROUTES_INDEX,
  NETWIB_DEBUG_LEAKTYPE_ENDENUM
} netwib_debug_leaktype;
netwib_err netwib_debug_leak_add(netwib_debug_leaktype type,
                                 netwib_constptr infoptr,
                                 netwib_uint32 infoint);
netwib_err netwib_debug_leak_del(netwib_debug_leaktype type,
                                 netwib_bool useptr,
                                 netwib_constptr infoptr,
                                 netwib_bool useint,
                                 netwib_uint32 infoint);
netwib_err netwib_debug_leak_valid(netwib_debug_leaktype type,
                                   netwib_bool useptr,
                                   netwib_constptr infoptr,
                                   netwib_bool useint,
                                   netwib_uint32 infoint);

netwib_err netwib_debug_leak_report(void);

#else
 /*used in netwib without #if...*/
 #define netwib_debug_leak_init() NETWIB_ERR_OK;
 #define netwib_debug_leak_close() NETWIB_ERR_OK;
 #define netwib_debug_leak_report() NETWIB_ERR_OK;
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
netwib_err netwib_debug_leak_add_generic(netwib_constptr ptr,
                                         netwib_debug_leaktype leaktype);
netwib_err netwib_debug_leak_del_generic(netwib_constptr ptr,
                                         netwib_debug_leaktype leaktype);
netwib_err netwib_debug_leak_valid_generic(netwib_constptr ptr,
                                           netwib_debug_leaktype leaktype);
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
netwib_err netwib_debug_leak_add_mem(netwib_uint32 allocsize,
                                     netwib_constptr ptr);
netwib_err netwib_debug_leak_del_mem(netwib_constptr ptr);
netwib_err netwib_debug_leak_valid_mem(netwib_constptr ptr);
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
netwib_err netwib_debug_leak_add_fd(netwib_uint32 fd);
netwib_err netwib_debug_leak_del_fd(netwib_uint32 fd);
netwib_err netwib_debug_leak_valid_fd(netwib_uint32 fd);
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
#define NETWIB_DEBUG_LEAK_ID_BUF_IGNORE 99
netwib_err netwib_debug_leak_id_buf(netwib_uint32 *pleakid);
netwib_err netwib_debug_leak_add_buf(netwib_constptr ptr,
                                     netwib_uint32 leakid);
netwib_err netwib_debug_leak_del_buf(netwib_constptr ptr,
                                     netwib_uint32 leakid);
netwib_err netwib_debug_leak_valid_buf(netwib_constptr ptr,
                                       netwib_uint32 leakid);
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
 #define netwib_debug_leak_add_ring(ptr) netwib_debug_leak_add_generic(ptr,NETWIB_DEBUG_LEAKTYPE_RING)
 #define netwib_debug_leak_del_ring(ptr) netwib_debug_leak_del_generic(ptr,NETWIB_DEBUG_LEAKTYPE_RING)
 #define netwib_debug_leak_valid_ring(ptr) netwib_debug_leak_valid_generic(ptr,NETWIB_DEBUG_LEAKTYPE_RING)
 #define netwib_debug_leak_add_ringi(ptr) netwib_debug_leak_add_generic(ptr,NETWIB_DEBUG_LEAKTYPE_RINGI)
 #define netwib_debug_leak_del_ringi(ptr) netwib_debug_leak_del_generic(ptr,NETWIB_DEBUG_LEAKTYPE_RINGI)
 #define netwib_debug_leak_valid_ringi(ptr) netwib_debug_leak_valid_generic(ptr,NETWIB_DEBUG_LEAKTYPE_RINGI)
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
 #define netwib_debug_leak_add_hash(ptr) netwib_debug_leak_add_generic(ptr,NETWIB_DEBUG_LEAKTYPE_HASH)
 #define netwib_debug_leak_del_hash(ptr) netwib_debug_leak_del_generic(ptr,NETWIB_DEBUG_LEAKTYPE_HASH)
 #define netwib_debug_leak_valid_hash(ptr) netwib_debug_leak_valid_generic(ptr,NETWIB_DEBUG_LEAKTYPE_HASH)
 #define netwib_debug_leak_add_hashi(ptr) netwib_debug_leak_add_generic(ptr,NETWIB_DEBUG_LEAKTYPE_HASHI)
 #define netwib_debug_leak_del_hashi(ptr) netwib_debug_leak_del_generic(ptr,NETWIB_DEBUG_LEAKTYPE_HASHI)
 #define netwib_debug_leak_valid_hashi(ptr) netwib_debug_leak_valid_generic(ptr,NETWIB_DEBUG_LEAKTYPE_HASHI)
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
 #define netwib_debug_leak_add_array(ptr) netwib_debug_leak_add_generic((ptr)->opaque,NETWIB_DEBUG_LEAKTYPE_ARRAY)
 #define netwib_debug_leak_del_array(ptr) netwib_debug_leak_del_generic((ptr)->opaque,NETWIB_DEBUG_LEAKTYPE_ARRAY)
 #define netwib_debug_leak_valid_array(ptr) netwib_debug_leak_valid_generic((ptr)->opaque,NETWIB_DEBUG_LEAKTYPE_ARRAY)
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
 #define netwib_debug_leak_add_dir(ptr) netwib_debug_leak_add_generic(ptr,NETWIB_DEBUG_LEAKTYPE_DIR)
 #define netwib_debug_leak_del_dir(ptr) netwib_debug_leak_del_generic(ptr,NETWIB_DEBUG_LEAKTYPE_DIR)
 #define netwib_debug_leak_valid_dir(ptr) netwib_debug_leak_valid_generic(ptr,NETWIB_DEBUG_LEAKTYPE_DIR)
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
 #define netwib_debug_leak_add_io(ptr) netwib_debug_leak_add_generic(ptr,NETWIB_DEBUG_LEAKTYPE_IO)
 #define netwib_debug_leak_del_io(ptr) netwib_debug_leak_del_generic(ptr,NETWIB_DEBUG_LEAKTYPE_IO)
 #define netwib_debug_leak_valid_io(ptr) netwib_debug_leak_valid_generic(ptr,NETWIB_DEBUG_LEAKTYPE_IO)
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
 #define netwib_debug_leak_add_thread(ptr) netwib_debug_leak_add_generic(ptr,NETWIB_DEBUG_LEAKTYPE_THREAD)
 #define netwib_debug_leak_del_thread(ptr) netwib_debug_leak_del_generic(ptr,NETWIB_DEBUG_LEAKTYPE_THREAD)
 #define netwib_debug_leak_valid_thread(ptr) netwib_debug_leak_valid_generic(ptr,NETWIB_DEBUG_LEAKTYPE_THREAD)
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
 #define netwib_debug_leak_add_thread_mutex(ptr) netwib_debug_leak_add_generic(ptr,NETWIB_DEBUG_LEAKTYPE_THREAD_MUTEX)
 #define netwib_debug_leak_del_thread_mutex(ptr) netwib_debug_leak_del_generic(ptr,NETWIB_DEBUG_LEAKTYPE_THREAD_MUTEX)
 #define netwib_debug_leak_valid_thread_mutex(ptr) netwib_debug_leak_valid_generic(ptr,NETWIB_DEBUG_LEAKTYPE_THREAD_MUTEX)
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
 #define netwib_debug_leak_add_thread_rwlock(ptr) netwib_debug_leak_add_generic(ptr,NETWIB_DEBUG_LEAKTYPE_THREAD_RWLOCK)
 #define netwib_debug_leak_del_thread_rwlock(ptr) netwib_debug_leak_del_generic(ptr,NETWIB_DEBUG_LEAKTYPE_THREAD_RWLOCK)
 #define netwib_debug_leak_valid_thread_rwlock(ptr) netwib_debug_leak_valid_generic(ptr,NETWIB_DEBUG_LEAKTYPE_THREAD_RWLOCK)
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
 #define netwib_debug_leak_add_thread_cond(ptr) netwib_debug_leak_add_generic(ptr,NETWIB_DEBUG_LEAKTYPE_THREAD_COND)
 #define netwib_debug_leak_del_thread_cond(ptr) netwib_debug_leak_del_generic(ptr,NETWIB_DEBUG_LEAKTYPE_THREAD_COND)
 #define netwib_debug_leak_valid_thread_cond(ptr) netwib_debug_leak_valid_generic(ptr,NETWIB_DEBUG_LEAKTYPE_THREAD_COND)
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
 #define netwib_debug_leak_add_thread_tsd(ptr) netwib_debug_leak_add_generic(ptr,NETWIB_DEBUG_LEAKTYPE_THREAD_TSD)
 #define netwib_debug_leak_del_thread_tsd(ptr) netwib_debug_leak_del_generic(ptr,NETWIB_DEBUG_LEAKTYPE_THREAD_TSD)
 #define netwib_debug_leak_valid_thread_tsd(ptr) netwib_debug_leak_valid_generic(ptr,NETWIB_DEBUG_LEAKTYPE_THREAD_TSD)
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
 #define netwib_debug_leak_add_wait(ptr) netwib_debug_leak_add_generic(ptr,NETWIB_DEBUG_LEAKTYPE_WAIT)
 #define netwib_debug_leak_del_wait(ptr) netwib_debug_leak_del_generic(ptr,NETWIB_DEBUG_LEAKTYPE_WAIT)
 #define netwib_debug_leak_valid_wait(ptr) netwib_debug_leak_valid_generic(ptr,NETWIB_DEBUG_LEAKTYPE_WAIT)
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
 #define netwib_debug_leak_add_eths(ptr) netwib_debug_leak_add_generic(ptr,NETWIB_DEBUG_LEAKTYPE_ETHS)
 #define netwib_debug_leak_del_eths(ptr) netwib_debug_leak_del_generic(ptr,NETWIB_DEBUG_LEAKTYPE_ETHS)
 #define netwib_debug_leak_valid_eths(ptr) netwib_debug_leak_valid_generic(ptr,NETWIB_DEBUG_LEAKTYPE_ETHS)
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
 #define netwib_debug_leak_add_ips(ptr) netwib_debug_leak_add_generic(ptr,NETWIB_DEBUG_LEAKTYPE_IPS)
 #define netwib_debug_leak_del_ips(ptr) netwib_debug_leak_del_generic(ptr,NETWIB_DEBUG_LEAKTYPE_IPS)
 #define netwib_debug_leak_valid_ips(ptr) netwib_debug_leak_valid_generic(ptr,NETWIB_DEBUG_LEAKTYPE_IPS)
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
 #define netwib_debug_leak_add_ports(ptr) netwib_debug_leak_add_generic(ptr,NETWIB_DEBUG_LEAKTYPE_PORTS)
 #define netwib_debug_leak_del_ports(ptr) netwib_debug_leak_del_generic(ptr,NETWIB_DEBUG_LEAKTYPE_PORTS)
 #define netwib_debug_leak_valid_ports(ptr) netwib_debug_leak_valid_generic(ptr,NETWIB_DEBUG_LEAKTYPE_PORTS)
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
 #define netwib_debug_leak_add_eths_index(ptr) netwib_debug_leak_add_generic(ptr,NETWIB_DEBUG_LEAKTYPE_ETHS_INDEX)
 #define netwib_debug_leak_del_eths_index(ptr) netwib_debug_leak_del_generic(ptr,NETWIB_DEBUG_LEAKTYPE_ETHS_INDEX)
 #define netwib_debug_leak_valid_eths_index(ptr) netwib_debug_leak_valid_generic(ptr,NETWIB_DEBUG_LEAKTYPE_ETHS_INDEX)
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
 #define netwib_debug_leak_add_ips_index(ptr) netwib_debug_leak_add_generic(ptr,NETWIB_DEBUG_LEAKTYPE_IPS_INDEX)
 #define netwib_debug_leak_del_ips_index(ptr) netwib_debug_leak_del_generic(ptr,NETWIB_DEBUG_LEAKTYPE_IPS_INDEX)
 #define netwib_debug_leak_valid_ips_index(ptr) netwib_debug_leak_valid_generic(ptr,NETWIB_DEBUG_LEAKTYPE_IPS_INDEX)
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
 #define netwib_debug_leak_add_ports_index(ptr) netwib_debug_leak_add_generic(ptr,NETWIB_DEBUG_LEAKTYPE_PORTS_INDEX)
 #define netwib_debug_leak_del_ports_index(ptr) netwib_debug_leak_del_generic(ptr,NETWIB_DEBUG_LEAKTYPE_PORTS_INDEX)
 #define netwib_debug_leak_valid_ports_index(ptr) netwib_debug_leak_valid_generic(ptr,NETWIB_DEBUG_LEAKTYPE_PORTS_INDEX)
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
 #define netwib_debug_leak_add_conf_devices_index(ptr) netwib_debug_leak_add_generic(ptr,NETWIB_DEBUG_LEAKTYPE_CONF_DEVICES_INDEX)
 #define netwib_debug_leak_del_conf_devices_index(ptr) netwib_debug_leak_del_generic(ptr,NETWIB_DEBUG_LEAKTYPE_CONF_DEVICES_INDEX)
 #define netwib_debug_leak_valid_conf_devices_index(ptr) netwib_debug_leak_valid_generic(ptr,NETWIB_DEBUG_LEAKTYPE_CONF_DEVICES_INDEX)
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
 #define netwib_debug_leak_add_conf_ip_index(ptr) netwib_debug_leak_add_generic(ptr,NETWIB_DEBUG_LEAKTYPE_CONF_IP_INDEX)
 #define netwib_debug_leak_del_conf_ip_index(ptr) netwib_debug_leak_del_generic(ptr,NETWIB_DEBUG_LEAKTYPE_CONF_IP_INDEX)
 #define netwib_debug_leak_valid_conf_ip_index(ptr) netwib_debug_leak_valid_generic(ptr,NETWIB_DEBUG_LEAKTYPE_CONF_IP_INDEX)
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
 #define netwib_debug_leak_add_conf_arpcache_index(ptr) netwib_debug_leak_add_generic(ptr,NETWIB_DEBUG_LEAKTYPE_CONF_ARPCACHE_INDEX)
 #define netwib_debug_leak_del_conf_arpcache_index(ptr) netwib_debug_leak_del_generic(ptr,NETWIB_DEBUG_LEAKTYPE_CONF_ARPCACHE_INDEX)
 #define netwib_debug_leak_valid_conf_arpcache_index(ptr) netwib_debug_leak_valid_generic(ptr,NETWIB_DEBUG_LEAKTYPE_CONF_ARPCACHE_INDEX)
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK_NEEDED==1
 #define netwib_debug_leak_add_conf_routes_index(ptr) netwib_debug_leak_add_generic(ptr,NETWIB_DEBUG_LEAKTYPE_CONF_ROUTES_INDEX)
 #define netwib_debug_leak_del_conf_routes_index(ptr) netwib_debug_leak_del_generic(ptr,NETWIB_DEBUG_LEAKTYPE_CONF_ROUTES_INDEX)
 #define netwib_debug_leak_valid_conf_routes_index(ptr) netwib_debug_leak_valid_generic(ptr,NETWIB_DEBUG_LEAKTYPE_CONF_ROUTES_INDEX)
#endif
