
/*-------------------------------------------------------------*/
#define NETWOX_RELAY_ARG_IP NETWOX_TOOLARG_REQ_IP_DST('x', "server-ip", NULL, NULL), NETWOX_TOOLARG_REQ_PORT_DST('X', "server-port", NULL, NULL), NETWOX_TOOLARG_OPTA_IPS_SU('c', "allowed-clients", "clients allowed to connect", NULL)
#define NETWOX_RELAY_ARG_IPS NETWOX_TOOLARG_REQ_IPS_SU('x', "server-ip", NULL, "1.2.3.4,1.2.3.5"), NETWOX_TOOLARG_REQ_PORT_DST('X', "server-port", NULL, NULL), NETWOX_TOOLARG_OPTA_IPS_SU('c', "allowed-clients", "clients allowed to connect", NULL)

/*-------------------------------------------------------------*/
/* can be shared for read between threads */
typedef struct {
  netwox_socktype socktype;
  netwib_port serverport;
  netwib_ips *pallowedclients;
  netwib_uint32 numservers; /* number of servers */
  netwib_ip serverip; /* used if one server only */
  netwib_array serverips; /* used if more than one */
} netwox_relay;

/*-------------------------------------------------------------*/
netwib_err netwox_relay_init_arg(netwox_arg *parg,
                                 netwox_socktype socktype,
                                 netwox_relay *prelay);
netwib_err netwox_relay_close(netwox_relay *prelay);

/*-------------------------------------------------------------*/
netwib_err netwox_relay_loop(netwox_relay *prelay,
                             netwib_io *pserverio);
