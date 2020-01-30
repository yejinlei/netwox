/* Virtual IP stack : not yet implemented. It will replace sockv.
   Currently, it only stores ideas to not forget them.
   I don't know when it will be implemented. */

/*-------------------------------------------------------------*/
/***************************************************************
 * This module implements a virtual IP stack.                  *
 *                                                             *
 * This IP stack is called "virtual" because it is not real.   *
 * It uses fake/spoofed Ethernet and IP addresses.             *
 * For example, a virtual tcp client behaves like this :       *
 *  - spoof a syn packet                                       *
 *  - sniff the syn-ack from the server                        *
 *  - spoof an ack packet to terminate the handshake           *
 *  - then, it behaves exactly like a real client : we can use *
 *    the netwib_io to read or write data on the network       *
 * This virtual IP stack doesn't exactly implement RFC 768, 791*
 * and 793 : it's working, but all specials cases are not      *
 * implemented.                                                *
 ***************************************************************/

/*-------------------------------------------------------------*/
/***************************************************************
 * A virtual IP stack has one Ethernet address and one         *
 * (or more) IP address(es). It can support (for its           *
 * Ethernet/IP pair):                                          *
 *  - ping                                                     *
 *  - one or several TCP client                                *
 *  - one or several TCP server                                *
 *  - one or several UDP client                                *
 *  - one or several UDP server                                *
 *  - send a reset for closed TCP ports                        *
 *  - send an ICMP for closed UDP ports                        *
 *                                                             *
 * Examples:                                                   *
 * Virtual Computer 1 can be defined as:                       *
 *  - Ethernet address 0:1:2:3:4:5                             *
 *  - IPv4 address 1.2.3.4                                     *
 *  - answers to ping                                          *
 *  - send a reset for closed TCP ports                        *
 *  - a TCP server listening on port 80                        *
 *  - a TCP client connected to 1.2.3.5                        *
 * Virtual Computer 2 can be defined as:                       *
 *  - Ethernet address 0:1:2:3:4:6                             *
 *  - IPv4 address 1.2.3.6 and IPv6 address fec0:0:0:1::6      *
 *  - no answers to ping                                       *
 *  - no send reset for closed TCP ports                       *
 *  - a TCP client connected to fec0:0:0:1::6                  *
 ***************************************************************/

/*-------------------------------------------------------------*/
/***************************************************************
 * In which cases using a virtual IP stack ?                   *
 * - to create an IPv6 client/server on a computer supporting  *
 *   only IPv4                                                 *
 * - if it's not easy to change IP address of computer         *
 * - to simulate a client/server located on another network    *
 *   block. If router exists, Ethernet source address has to   *
 *   its address. If router does not exists, it has to be      *
 *   simulated with tool 73                                    *
 ***************************************************************/

/*-------------------------------------------------------------*/
typedef struct netwox_vstack netwox_vstack;

/*-------------------------------------------------------------*/
netwib_err netwox_vstack_init(netwib_constbuf *pdevice,
                              netwib_consteth *plocaleth,
                              netwib_ips *plocalips,
                              netwox_vstack *pvstack);
netwib_err netwox_vstack_close(netwox_vstack *pvstack);

/*-------------------------------------------------------------*/
netwib_err netwox_vstack_answer_ping(netwox_vstack *pvstack,
                                     netwib_uint32 vcid,
                                     netwib_bool answer);
netwib_err netwox_vstack_answer_tcprst(netwox_vstack *pvstack,
                                       netwib_uint32 vcid,
                                       netwib_bool answer);
netwib_err netwox_vstack_answer_udpunreach(netwox_vstack *pvstack,
                                           netwib_uint32 vcid,
                                           netwib_bool answer);

/*-------------------------------------------------------------*/
netwib_err netwox_vstack_udp_cli_full(netwox_vstack *pvstack,
                                      netwib_uint32 vcid,
                                      netwib_consteth *premoteeth,
                                      netwib_constip *plocalip,
                                      netwib_constip *premoteip,
                                      netwib_port localport,
                                      netwib_port remoteport,
                                      netwib_constbuf *pip4opts,
                                      netwib_ipproto ip6extsproto,
                                      netwib_constbuf *pip6exts,
                                      netwib_io **ppio);
#define netwox_vstack_udp_cli(pvstack,vcid,premoteeth,plocalip,premoteip,localport,remoteport,ppio) netwox_vstack_udp_cli_full(pvstack,vcid,premoteeth,plocalip,premoteip,localport,remoteport,NULL,NETWIB_IPPROTO_NONE,NULL,ppio)

/*-------------------------------------------------------------*/
netwib_err netwox_vstack_tcp_cli_full(netwox_vstack *pvstack,
                                      netwib_uint32 vcid,
                                      netwib_consteth *premoteeth,
                                      netwib_constip *plocalip,
                                      netwib_constip *premoteip,
                                      netwib_port localport,
                                      netwib_port remoteport,
                                      netwib_constbuf *pip4opts,
                                      netwib_ipproto ip6extsproto,
                                      netwib_constbuf *pip6exts,
                                      netwib_io **ppio);
#define netwox_vstack_tcp_cli(pvstack,vcid,premoteeth,plocalip,premoteip,localport,remoteport,ppio) netwox_vstack_tcp_cli_full((pvstack,vcid,premoteeth,plocalip,premoteip,localport,remoteport,NULL,NETWIB_IPPROTO_NONE,NULL,ppio)

/*-------------------------------------------------------------*/
netwib_err netwox_vstack_udp_ser_full(netwox_vstack *pvstack,
                                      netwib_uint32 vcid,
                                      netwib_constip *plocalip,
                                      netwib_port localport,
                                      netwib_constbuf *pip4opts,
                                      netwib_ipproto ip6extsproto,
                                      netwib_constbuf *pip6exts,
                                      netwib_io **ppio);
#define netwox_vstack_udp_ser(pvstack,vcid,plocalip,localport,ppio) netwox_vstack_udp_ser_full(pvstack,vcid,plocalip,localport,NULL,NETWIB_IPPROTO_NONE,NULL,ppio)

/*-------------------------------------------------------------*/
netwib_err netwox_vstack_tcp_ser_full(netwox_vstack *pvstack,
                                      netwib_uint32 vcid,
                                      netwib_constip *plocalip,
                                      netwib_port localport,
                                      netwib_constbuf *pip4opts,
                                      netwib_ipproto ip6extsproto,
                                      netwib_constbuf *pip6exts,
                                      netwib_io **ppio);
#define netwox_vstack_tcp_ser(pvstack,vcid,plocalip,localport,ppio) netwox_vstack_tcp_ser_full(pvstack,vcid,plocalip,localport,NULL,NETWIB_IPPROTO_NONE,NULL,ppio)

/*-------------------------------------------------------------*/
netwib_err netwox_vstack_udp_mulser_full(netwox_vstack *pvstack,
                                         netwib_uint32 vcid,
                                         netwib_constip *plocalip,
                                         netwib_port localport,
                                         netwib_constbuf *pip4opts,
                                         netwib_ipproto ip6extsproto,
                                         netwib_constbuf *pip6exts,
                                         netwib_io **ppio);
#define netwox_vstack_muludp_ser(pvstack,vcid,plocalip,localport,ppio) netwox_vstack_muludp_ser_full(pvstack,vcid,plocalip,localport,NULL,NETWIB_IPPROTO_NONE,NULL,ppio)

/*-------------------------------------------------------------*/
netwib_err netwox_vstack_tcp_mulser_full(netwox_vstack *pvstack,
                                         netwib_uint32 vcid,
                                         netwib_constip *plocalip,
                                         netwib_port localport,
                                         netwib_constbuf *pip4opts,
                                         netwib_ipproto ip6extsproto,
                                         netwib_constbuf *pip6exts,
                                         netwib_sock_tcp_mulser_pf pfunc,
                                         netwib_ptr pinfos);
#define netwox_vstack_multcp_ser(pvstack,vcid,plocalip,localportpfunc,pinfos) netwox_vstack_multcp_ser_full(pvstack,vcid,plocalip,localport,NULL,NETWIB_IPPROTO_NONE,NULLpfunc,pinfos)
