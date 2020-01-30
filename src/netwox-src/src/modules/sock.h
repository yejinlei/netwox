
/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SOCKTYPE_UDP_CLI = 1,
  NETWOX_SOCKTYPE_TCP_CLI,
  NETWOX_SOCKTYPE_UDP_SER,
  NETWOX_SOCKTYPE_TCP_SER,
  NETWOX_SOCKTYPE_UDP_MULSER,
  NETWOX_SOCKTYPE_TCP_MULSER
} netwox_socktype;

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SOCKFAMILY_CLI = 1,
  NETWOX_SOCKFAMILY_SER
} netwox_sockfamily;

/*-------------------------------------------------------------*/
netwib_err netwox_sockfamily_init_socktype(netwox_socktype socktype,
                                           netwox_sockfamily *psockfamily);

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SOCKINIT_REAL = 1,
  NETWOX_SOCKINIT_REALEASY,
  NETWOX_SOCKINIT_VIRT
} netwox_sockinit; /* means what's initialized */

/*-------------------------------------------------------------*/
typedef struct {
  netwox_socktype socktype;
  netwox_sockfamily sockfamily;
  netwox_sockinit sockinit;
  netwib_buf device;      /* virt */
  netwib_bool answeralive;/* virt */
  netwib_eth localeth;    /* virt */
  netwib_eth remoteeth;   /* virt cli */
  netwib_ip localip;      /* storage */
  netwib_ip *plocalip;    /* NULL or &storage */
  netwib_ip remoteip;     /* storage */
  netwib_ip *premoteip;   /* NULL or &storage */
  netwib_port localport;  /* 0 or port */
  netwib_port remoteport; /* 0 or port */
  netwib_buf ip4opts;     /* storage */
  netwib_buf *pip4opts;   /* NULL or &storage */
  netwib_ipproto ip6extsproto;
  netwib_buf ip6exts;     /* storage */
  netwib_buf *pip6exts;   /* NULL or &storage */
  netwib_iptype iptype;   /* real ser */
} netwox_sockinfo;

/*-------------------------------------------------------------*/
netwib_err netwox_sockinfo_init(netwox_socktype socktype,
                                netwox_sockinfo *psockinfo);
netwib_err netwox_sockinfo_close(netwox_sockinfo *psockinfo);

/*-------------------------------------------------------------*/
#define NETWOX_SOCK_ARG_KEY_DEVICE 'd'
#define NETWOX_SOCK_ARG_KEY_ETH_SRC 'E'
#define NETWOX_SOCK_ARG_KEY_ETH_DST 'e'
#define NETWOX_SOCK_ARG_KEY_IP_SRC 'I'
#define NETWOX_SOCK_ARG_KEY_IP_DST 'i'
#define NETWOX_SOCK_ARG_KEY_PORT_SRC 'P'
#define NETWOX_SOCK_ARG_KEY_PORT_DST 'p'
#define NETWOX_SOCK_ARG_KEY_IP4OPTS 'o'
/* ip6exts protocol is stored in first byte */
#define NETWOX_SOCK_ARG_KEY_IP6EXTS 'O'
#define NETWOX_SOCK_ARG_KEY_IPTYPE 't'

#define NETWOX_SOCK_ARG_TCP_CLI NETWOX_TOOLARG_OPTA_BUF_DEVICE(NETWOX_SOCK_ARG_KEY_DEVICE, NULL, NULL, NULL), NETWOX_TOOLARG_OPTA_ETH_SRC(NETWOX_SOCK_ARG_KEY_ETH_SRC, NULL, NULL, "0:2:3:4:5:6"), NETWOX_TOOLARG_OPTA_ETH_DST(NETWOX_SOCK_ARG_KEY_ETH_DST, NULL, NULL, NULL), NETWOX_TOOLARG_OPTA_IP_SRC(NETWOX_SOCK_ARG_KEY_IP_SRC, NULL, NULL, NULL), NETWOX_TOOLARG_REQ_IP_DST(NETWOX_SOCK_ARG_KEY_IP_DST, NULL, NULL, NULL), NETWOX_TOOLARG_OPTA_PORT_SRC(NETWOX_SOCK_ARG_KEY_PORT_SRC, NULL, NULL, "0"), NETWOX_TOOLARG_REQ_PORT_DST(NETWOX_SOCK_ARG_KEY_PORT_DST, NULL, NULL, NULL), NETWOX_TOOLARG_OPTA_BUF_IP4OPTS(NETWOX_SOCK_ARG_KEY_IP4OPTS, NULL, NULL, NULL), NETWOX_TOOLARG_OPTA_BUF_IP6EXTS(NETWOX_SOCK_ARG_KEY_IP6EXTS, NULL, NULL, NULL)
#define NETWOX_SOCK_ARG_TCP_CLIPORT(port) NETWOX_TOOLARG_OPTA_BUF_DEVICE(NETWOX_SOCK_ARG_KEY_DEVICE, NULL, NULL, NULL), NETWOX_TOOLARG_OPTA_ETH_SRC(NETWOX_SOCK_ARG_KEY_ETH_SRC, NULL, NULL, "0:2:3:4:5:6"), NETWOX_TOOLARG_OPTA_ETH_DST(NETWOX_SOCK_ARG_KEY_ETH_DST, NULL, NULL, NULL), NETWOX_TOOLARG_OPTA_IP_SRC(NETWOX_SOCK_ARG_KEY_IP_SRC, NULL, NULL, NULL), NETWOX_TOOLARG_REQ_IP_DST(NETWOX_SOCK_ARG_KEY_IP_DST, NULL, NULL, NULL), NETWOX_TOOLARG_OPTA_PORT_SRC(NETWOX_SOCK_ARG_KEY_PORT_SRC, NULL, NULL, "0"), NETWOX_TOOLARG_OPTA_PORT_DST(NETWOX_SOCK_ARG_KEY_PORT_DST, NULL, NULL, port), NETWOX_TOOLARG_OPTA_BUF_IP4OPTS(NETWOX_SOCK_ARG_KEY_IP4OPTS, NULL, NULL, NULL), NETWOX_TOOLARG_OPTA_BUF_IP6EXTS(NETWOX_SOCK_ARG_KEY_IP6EXTS, NULL, NULL, NULL)
#define NETWOX_SOCK_ARG_UDP_CLI NETWOX_SOCK_ARG_TCP_CLI
#define NETWOX_SOCK_ARG_UDP_CLIPORT(port) NETWOX_SOCK_ARG_TCP_CLIPORT(port)
#define NETWOX_SOCK_ARG_TCP_SER NETWOX_TOOLARG_OPTA_BUF_DEVICE(NETWOX_SOCK_ARG_KEY_DEVICE, NULL, NULL, NULL), NETWOX_TOOLARG_OPTA_ETH_SRC(NETWOX_SOCK_ARG_KEY_ETH_SRC, NULL, NULL, "0:2:3:4:5:6"), NETWOX_TOOLARG_OPTA_IP_SRC(NETWOX_SOCK_ARG_KEY_IP_SRC, NULL, NULL, NULL), NETWOX_TOOLARG_REQ_PORT_SRC(NETWOX_SOCK_ARG_KEY_PORT_SRC, NULL, NULL, NULL), NETWOX_TOOLARG_OPTA_BUF_IP4OPTS(NETWOX_SOCK_ARG_KEY_IP4OPTS, NULL, NULL, NULL), NETWOX_TOOLARG_OPTA_BUF_IP6EXTS(NETWOX_SOCK_ARG_KEY_IP6EXTS, NULL, NULL, NULL), NETWOX_TOOLARG_OPTA_IPTYPE(NETWOX_SOCK_ARG_KEY_IPTYPE, NULL, "type to use when src-ip is unset", NULL)
#define NETWOX_SOCK_ARG_TCP_SERPORT(port) NETWOX_TOOLARG_OPTA_BUF_DEVICE(NETWOX_SOCK_ARG_KEY_DEVICE, NULL, NULL, NULL), NETWOX_TOOLARG_OPTA_ETH_SRC(NETWOX_SOCK_ARG_KEY_ETH_SRC, NULL, NULL, "0:2:3:4:5:6"), NETWOX_TOOLARG_OPTA_IP_SRC(NETWOX_SOCK_ARG_KEY_IP_SRC, NULL, NULL, NULL), NETWOX_TOOLARG_OPTA_PORT_SRC(NETWOX_SOCK_ARG_KEY_PORT_SRC, NULL, NULL, port), NETWOX_TOOLARG_OPTA_BUF_IP4OPTS(NETWOX_SOCK_ARG_KEY_IP4OPTS, NULL, NULL, NULL), NETWOX_TOOLARG_OPTA_BUF_IP6EXTS(NETWOX_SOCK_ARG_KEY_IP6EXTS, NULL, NULL, NULL), NETWOX_TOOLARG_OPTA_IPTYPE(NETWOX_SOCK_ARG_KEY_IPTYPE, NULL, "type to use when src-ip is unset", NULL)
#define NETWOX_SOCK_ARG_UDP_SER NETWOX_SOCK_ARG_TCP_SER
#define NETWOX_SOCK_ARG_UDP_SERPORT(port) NETWOX_SOCK_ARG_TCP_SERPORT(port)
#define NETWOX_SOCK_ARG_UDP_MULSER NETWOX_SOCK_ARG_UDP_SER
#define NETWOX_SOCK_ARG_UDP_MULSERPORT(port) NETWOX_SOCK_ARG_UDP_SERPORT(port)
#define NETWOX_SOCK_ARG_TCP_MULSER NETWOX_SOCK_ARG_TCP_SER
#define NETWOX_SOCK_ARG_TCP_MULSERPORT(port) NETWOX_SOCK_ARG_TCP_SERPORT(port)

netwib_err netwox_sockinfo_init_arg(netwox_socktype socktype,
                                    netwox_arg *parg,
                                    netwox_sockinfo *psockinfo);
#define netwox_sockinfo_init_arg_udp_cli(parg,psockinfo) netwox_sockinfo_init_arg(NETWOX_SOCKTYPE_UDP_CLI,parg,psockinfo)
#define netwox_sockinfo_init_arg_tcp_cli(parg,psockinfo) netwox_sockinfo_init_arg(NETWOX_SOCKTYPE_TCP_CLI,parg,psockinfo)
#define netwox_sockinfo_init_arg_udp_ser(parg,psockinfo) netwox_sockinfo_init_arg(NETWOX_SOCKTYPE_UDP_SER,parg,psockinfo)
#define netwox_sockinfo_init_arg_tcp_ser(parg,psockinfo) netwox_sockinfo_init_arg(NETWOX_SOCKTYPE_TCP_SER,parg,psockinfo)
#define netwox_sockinfo_init_arg_udp_mulser(parg,psockinfo) netwox_sockinfo_init_arg(NETWOX_SOCKTYPE_UDP_MULSER,parg,psockinfo)
#define netwox_sockinfo_init_arg_tcp_mulser(parg,psockinfo) netwox_sockinfo_init_arg(NETWOX_SOCKTYPE_TCP_MULSER,parg,psockinfo)

/*-------------------------------------------------------------*/
netwib_err netwox_sockinfo_init_values(netwox_socktype socktype,
                                       netwox_sockinit sockinit,
                                       netwib_iptype iptype,
                                       netwib_constbuf *pdevice,
                                       netwib_consteth *plocaleth,
                                       netwib_consteth *premoteeth,
                                       netwib_constip *plocalip,
                                       netwib_constip *premoteip,
                                       netwib_port localport,
                                       netwib_port remoteport,
                                       netwib_constbuf *pip4opts,
                                       netwib_ipproto ip6extsproto,
                                       netwib_constbuf *pip6exts,
                                       netwox_sockinfo *psockinfo);
#define netwox_sockinfo_init_values_real_tcp_cli_easy(premoteip,remoteport,psockinfo) netwox_sockinfo_init_values(NETWOX_SOCKTYPE_TCP_CLI,NETWOX_SOCKINIT_REALEASY,0,NULL,NULL,NULL,NULL,premoteip,0,remoteport,NULL,NETWIB_IPPROTO_NONE,NULL,psockinfo)

/*-------------------------------------------------------------*/
/* use information of pcurrentsockinfo (which must be connected
   with another computer) and prepare info
   for listening on localport. User can then optionally
   change pnewsockinfo values before calling netwox_sock_init */
netwib_err netwox_sockinfo_init_listen(const netwox_sockinfo *pcurrentsockinfo,
                                       netwib_io *pcurrentio,
                                       netwib_port localport,
                                       netwox_sockinfo *pnewsockinfo);

/*-------------------------------------------------------------*/
/* use information of pcurrentsockinfo (which must be connected
   with another computer) and prepare info
   for connecting back to remoteport. User can then optionally
   change pnewsockinfo values before calling netwox_sock_init */
netwib_err netwox_sockinfo_init_connect(const netwox_sockinfo *pcurrentsockinfo,
                                        netwib_io *pcurrentio,
                                        netwib_port remoteport,
                                        netwox_sockinfo *pnewsockinfo);

/*-------------------------------------------------------------*/
netwib_err netwox_sock_init(netwox_sockinfo *psockinfo,
                            netwib_io **ppio);
netwib_err netwox_sock_tcp_mulser(netwox_sockinfo *psockinfo,
                                  netwib_sock_tcp_mulser_pf pfunc,
                                  netwib_ptr pinfos);
