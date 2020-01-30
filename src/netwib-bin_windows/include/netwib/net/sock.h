
/*-------------------------------------------------------------*/
/* We need a forward declaration for netwib_ipproto.
*/
#include "../pkt/types.h"

/*-------------------------------------------------------------*/
/***************************************************************
 * Parameter pip6exts permits to specify IP6 extensions (rfc   *
 * 3542). However, this is not yet implemented.                *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* Name : netwib_io_init_sock_xyz
   Description :
     Initialize a socket connection.
   Input parameter(s) :
     localip : local IP
     remoteip : remote IP
     iptype : iptype to use
     localport : local port
     remoteport : remote port
     *pip4opts : optional IP options to add
   Input/output parameter(s) :
   Output parameter(s) :
     **ppio : netwib_io initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_io_init_sock_udp_cli_full(netwib_constip *plocalip,
                                            netwib_constip *premoteip,
                                            netwib_port localport,
                                            netwib_port remoteport,
                                            netwib_constbuf *pip4opts,
                                            netwib_ipproto ip6extsproto,
                                            netwib_constbuf *pip6exts,
                                            netwib_io **ppio);
#define netwib_io_init_sock_udp_cli(plocalip,premoteip,localport,remoteport,ppio) netwib_io_init_sock_udp_cli_full(plocalip,premoteip,localport,remoteport,NULL,NETWIB_IPPROTO_NONE,NULL,ppio)
#define netwib_io_init_sock_udp_cli_easy(premoteip,remoteport,ppio) netwib_io_init_sock_udp_cli(NULL,premoteip,0,remoteport,ppio)

netwib_err netwib_io_init_sock_tcp_cli_full(netwib_constip *plocalip,
                                            netwib_constip *premoteip,
                                            netwib_port localport,
                                            netwib_port remoteport,
                                            netwib_constbuf *pip4opts,
                                            netwib_ipproto ip6extsproto,
                                            netwib_constbuf *pip6exts,
                                            netwib_io **ppio);
#define netwib_io_init_sock_tcp_cli(plocalip,premoteip,localport,remoteport,ppio) netwib_io_init_sock_tcp_cli_full(plocalip,premoteip,localport,remoteport,NULL,NETWIB_IPPROTO_NONE,NULL,ppio)
#define netwib_io_init_sock_tcp_cli_easy(premoteip,remoteport,ppio) netwib_io_init_sock_tcp_cli(NULL,premoteip,0,remoteport,ppio)

netwib_err netwib_io_init_sock_udp_ser_full(netwib_constip *plocalip,
                                            netwib_port localport,
                                            netwib_constbuf *pip4opts,
                                            netwib_ipproto ip6extsproto,
                                            netwib_constbuf *pip6exts,
                                            netwib_io **ppio);
#define netwib_io_init_sock_udp_ser(plocalip,localport,ppio) netwib_io_init_sock_udp_ser_full(plocalip,localport,NULL,NETWIB_IPPROTO_NONE,NULL,ppio)
netwib_err netwib_io_init_sock_udp_ser_easy(netwib_iptype iptype,
                                            netwib_port localport,
                                            netwib_io **ppio);

netwib_err netwib_io_init_sock_tcp_ser_full(netwib_constip *plocalip,
                                            netwib_port localport,
                                            netwib_constbuf *pip4opts,
                                            netwib_ipproto ip6extsproto,
                                            netwib_constbuf *pip6exts,
                                            netwib_io **ppio);
#define netwib_io_init_sock_tcp_ser(plocalip,localport,ppio) netwib_io_init_sock_tcp_ser_full(plocalip,localport,NULL,NETWIB_IPPROTO_NONE,NULL,ppio)
netwib_err netwib_io_init_sock_tcp_ser_easy(netwib_iptype iptype,
                                            netwib_port localport,
                                            netwib_io **ppio);

netwib_err netwib_io_init_sock_udp_mulser_full(netwib_constip *plocalip,
                                               netwib_port localport,
                                               netwib_constbuf *pip4opts,
                                               netwib_ipproto ip6extsproto,
                                               netwib_constbuf *pip6exts,
                                               netwib_io **ppio);
#define netwib_io_init_sock_udp_mulser(plocalip,localport,ppio) netwib_io_init_sock_udp_mulser_full(plocalip,localport,NULL,NETWIB_IPPROTO_NONE,NULL,ppio)
netwib_err netwib_io_init_sock_udp_mulser_easy(netwib_iptype iptype,
                                               netwib_port localport,
                                               netwib_io **ppio);

/*-------------------------------------------------------------*/
/* Name : netwib_sock_tcp_mulser_full
   Description :
     Deal with a tcp multi-client server.
     Note : we never exit from this function.
   Input parameter(s) :
     localip : listening IP address
     localport : listening port
     ip4opts : IP options to add
     pfunc : memory address of the function which will be
             called for each client. For each call, the first
             parameter ('pio') will be set with socket,
             and the second parameter ('pinfos')
             will be set with the optional parameter below.
             Note : at the end, this function must not close pio
   Input/output parameter(s) :
     pinfos : optional parameter (can be NULL) which will be
              used as the second parameter for *pfunc. This
              may be used to send information to *pfunc.
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
typedef netwib_err (*netwib_sock_tcp_mulser_pf)(netwib_io *pio,
                                                netwib_ptr pinfos);
netwib_err netwib_sock_tcp_mulser_full(netwib_constip *plocalip,
                                       netwib_port localport,
                                       netwib_constbuf *pip4opts,
                                       netwib_ipproto ip6extsproto,
                                       netwib_constbuf *pip6exts,
                                       netwib_sock_tcp_mulser_pf pfunc,
                                       netwib_ptr pinfos);
#define netwib_sock_tcp_mulser(plocalip,localport,pfunc,pinfos) netwib_sock_tcp_mulser_full(plocalip,localport,NULL,NETWIB_IPPROTO_NONE,NULL,pfunc,pinfos)
netwib_err netwib_sock_tcp_mulser_easy(netwib_iptype iptype,
                                       netwib_port localport,
                                       netwib_sock_tcp_mulser_pf pfunc,
                                       netwib_ptr pinfos);

/*-------------------------------------------------------------*/
/* Set/get IP4 options */
/* netwib_err f(netwib_io *pio, netwib_constbuf *pip4opts); */
#define netwib_sock_ctl_set_ip4opts(pio,pip4opts) netwib_io_ctl_set(pio,NETWIB_IO_WAYTYPE_SUPPORTED,NETWIB_IO_CTLTYPE_SOCK_IP4OPTS,pip4opts,0)
/* netwib_err f(netwib_io *pio, netwib_buf *pip4opts); */
#define netwib_sock_ctl_get_ip4opts(pio,pip4opts) netwib_io_ctl_get(pio,NETWIB_IO_WAYTYPE_SUPPORTED,NETWIB_IO_CTLTYPE_SOCK_IP4OPTS,pip4opts,NULL)

/*-------------------------------------------------------------*/
/* Set/get IP6 extensions */
/* netwib_err f(netwib_io *pio, netwib_ipproto ip6extsproto,
                netwib_constbuf *pip6exts); */
#define netwib_sock_ctl_set_ip6exts(pio,ip6extsproto,pip6exts) netwib_io_ctl_set(pio,NETWIB_IO_WAYTYPE_SUPPORTED,NETWIB_IO_CTLTYPE_SOCK_IP6EXTS,pip6exts,ip6extsproto)
/* netwib_err f(netwib_io *pio, netwib_ipproto *pip6extsproto,
                netwib_buf *pip6exts); */
#define netwib_sock_ctl_get_ip6exts(pio,pip6extsproto,pip6exts) netwib_io_ctl_get(pio,NETWIB_IO_WAYTYPE_SUPPORTED,NETWIB_IO_CTLTYPE_SOCK_IP6EXTS,pip6exts,pip6extsproto)

/*-------------------------------------------------------------*/
/* Obtain local IP address and port */
/* netwib_err f(netwib_io *pio, netwib_ip *pip, netwib_port *pport); */
#define netwib_sock_ctl_get_local(pio,pip,pport) netwib_io_ctl_get(pio,NETWIB_IO_WAYTYPE_SUPPORTED,NETWIB_IO_CTLTYPE_SOCK_LOCAL,pip,pport)
/* Obtain remote IP address and port */
/* netwib_err f(netwib_io *pio, netwib_ip *pip, netwib_port *pport); */
#define netwib_sock_ctl_get_remote(pio,pip,pport) netwib_io_ctl_get(pio,NETWIB_IO_WAYTYPE_SUPPORTED,NETWIB_IO_CTLTYPE_SOCK_REMOTE,pip,pport)

/*-------------------------------------------------------------*/
/* Change multicast TTL */
/* netwib_err f(netwib_io *pio, netwib_uin32 ttl); */
#define netwib_sock_ctl_set_multicastttl(pio,ttl) netwib_io_ctl_set(pio,NETWIB_IO_WAYTYPE_SUPPORTED,NETWIB_IO_CTLTYPE_SOCK_MULTICASTTTL,NULL,ttl)

/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_IO_SOCKTYPE_UDP_CLI = 1,
  NETWIB_IO_SOCKTYPE_TCP_CLI,
  NETWIB_IO_SOCKTYPE_UDP_SER,
  NETWIB_IO_SOCKTYPE_TCP_SER,
  NETWIB_IO_SOCKTYPE_UDP_MULSER,
  NETWIB_IO_SOCKTYPE_TCP_MULSER
} netwib_io_socktype;
/* Get socket type */
/* netwib_err f(netwib_io *pio, netwib_io_socktype *psocktype); */
#define netwib_sock_ctl_get_socktype(pio,psocktype) netwib_io_ctl_get(pio,NETWIB_IO_WAYTYPE_SUPPORTED,NETWIB_IO_CTLTYPE_SOCK_SOCKTYPE,psocktype,NULL)
