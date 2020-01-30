
/*-------------------------------------------------------------*/
/***************************************************************
 * A virtual client/server uses spoofed IP address and         *
 * Ethernet address. For example, a virtual tcp client behaves *
 * like this :                                                 *
 *  - spoof a syn packet                                       *
 *  - sniff the syn-ack from the server                        *
 *  - spoof a ack packet to terminate the handshake            *
 *  - then, it behaves exactly like a real client : we can use *
 *    the netwib_sock to read or write data on the network      *
 * This virtual IP stack doesn't exactly implement RFC 768 and *
 * RFC 793 : it's working, but all specials cases are not      *
 * implemented.                                                *
 ***************************************************************/

/*-------------------------------------------------------------*/
netwib_err netwox_io_init_sockv_udp_cli_full(netwib_constbuf *pdevice,
                                             netwib_consteth *plocaleth,
                                             netwib_consteth *premoteeth,
                                             netwib_constip *plocalip,
                                             netwib_constip *premoteip,
                                             netwib_port localport,
                                             netwib_port remoteport,
                                             netwib_constbuf *pip4opts,
                                             netwib_ipproto ip6extsproto,
                                             netwib_constbuf *pip6exts,
                                             netwib_bool answeralive,
                                             netwib_io **ppio);
#define netwox_io_init_sockv_udp_cli(pdevice,plocaleth,premoteeth,plocalip,premoteip,localport,remoteport,answeralive,ppio) netwox_io_init_sockv_udp_cli_full(pdevice,plocaleth,premoteeth,plocalip,premoteip,localport,remoteport,NULL,NETWIB_IPPROTO_NONE,NULL,answeralive,ppio)

/*-------------------------------------------------------------*/
netwib_err netwox_io_init_sockv_tcp_cli_full(netwib_constbuf *pdevice,
                                             netwib_consteth *plocaleth,
                                             netwib_consteth *premoteeth,
                                             netwib_constip *plocalip,
                                             netwib_constip *premoteip,
                                             netwib_port localport,
                                             netwib_port remoteport,
                                             netwib_constbuf *pip4opts,
                                             netwib_ipproto ip6extsproto,
                                             netwib_constbuf *pip6exts,
                                             netwib_bool answeralive,
                                             netwib_io **ppio);
#define netwox_io_init_sockv_tcp_cli(pdevice,plocaleth,premoteeth,plocalip,premoteip,localport,remoteport,answeralive,ppio) netwox_io_init_sockv_tcp_cli_full(pdevice,plocaleth,premoteeth,plocalip,premoteip,localport,remoteport,NULL,NETWIB_IPPROTO_NONE,NULL,answeralive,ppio)

/*-------------------------------------------------------------*/
netwib_err netwox_io_init_sockv_udp_ser_full(netwib_constbuf *pdevice,
                                             netwib_consteth *plocaleth,
                                             netwib_constip *plocalip,
                                             netwib_port localport,
                                             netwib_constbuf *pip4opts,
                                             netwib_ipproto ip6extsproto,
                                             netwib_constbuf *pip6exts,
                                             netwib_bool answeralive,
                                             netwib_io **ppio);
#define netwox_io_init_sockv_udp_ser(pdevice,plocaleth,plocalip,localport,answeralive,ppio) netwox_io_init_sockv_udp_ser_full(pdevice,plocaleth,plocalip,localport,NULL,NETWIB_IPPROTO_NONE,NULL,answeralive,ppio)

/*-------------------------------------------------------------*/
netwib_err netwox_io_init_sockv_tcp_ser_full(netwib_constbuf *pdevice,
                                             netwib_consteth *plocaleth,
                                             netwib_constip *plocalip,
                                             netwib_port localport,
                                             netwib_constbuf *pip4opts,
                                             netwib_ipproto ip6extsproto,
                                             netwib_constbuf *pip6exts,
                                             netwib_bool answeralive,
                                             netwib_io **ppio);
#define netwox_io_init_sockv_tcp_ser(pdevice,plocaleth,plocalip,localport,answeralive,ppio) netwox_io_init_sockv_tcp_ser_full(pdevice,plocaleth,plocalip,localport,NULL,NETWIB_IPPROTO_NONE,NULL,answeralive,ppio)

/*-------------------------------------------------------------*/
netwib_err netwox_io_init_sockv_udp_mulser_full(netwib_constbuf *pdevice,
                                                netwib_consteth *plocaleth,
                                                netwib_constip *plocalip,
                                                netwib_port localport,
                                                netwib_constbuf *pip4opts,
                                                netwib_ipproto ip6extsproto,
                                                netwib_constbuf *pip6exts,
                                                netwib_bool answeralive,
                                                netwib_io **ppio);
#define netwox_io_init_sockv_udp_mulser_ser(pdevice,plocaleth,plocalip,localport,answeralive,ppio) netwox_io_init_sockv_udp_mulser_full(pdevice,plocaleth,plocalip,localport,NULL,NETWIB_IPPROTO_NONE,NULL,answeralive,ppio)

/*-------------------------------------------------------------*/
/* Note: this implementation is not multithread, but it works.
   It will be improved when vstack.h will be implemented */
netwib_err netwox_sockv_tcp_mulser_full(netwib_constbuf *pdevice,
                                        netwib_consteth *plocaleth,
                                        netwib_constip *plocalip,
                                        netwib_port localport,
                                        netwib_constbuf *pip4opts,
                                        netwib_ipproto ip6extsproto,
                                        netwib_constbuf *pip6exts,
                                        netwib_bool answeralive,
                                        netwib_sock_tcp_mulser_pf pfunc,
                                        netwib_ptr pinfos);
#define netwox_sockv_tcp_mulser_ser(pdevice,plocaleth,plocalip,localport,answeralive,pfunc,pinfos) netwox_sockv_tcp_mulser_ser_full(pdevice,plocaleth,plocalip,localport,NULL,NETWIB_IPPROTO_NONE,NULL,answeralive,pfunc,pinfos)
