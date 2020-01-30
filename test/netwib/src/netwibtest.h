
/*-------------------------------------------------------------*/
#ifndef NETWIB_NETWIBTEST_INCLUDED
#define NETWIB_NETWIBTEST_INCLUDED 1

/*-------------------------------------------------------------*/
#include <netwib/inc/maininc.h>

/*-------------------------------------------------------------*/
#include "conf.h"

/*-------------------------------------------------------------*/
#define netwib_eir(z,i) {netwib_err netwib_coderr=z;if(netwib_coderr!=i){if(netwib_coderr==NETWIB_ERR_OK)return(NETWIB_ERR_LOERROROKKO);else return(netwib_coderr);}}

/*-------------------------------------------------------------*/
#define NETWIBTEST_ERR_0 (netwib_err)12300
#define NETWIBTEST_ERR_1 (netwib_err)12301
#define NETWIBTEST_ERR_2 (netwib_err)12302
#define NETWIBTEST_ERR_3 (netwib_err)12303
#define NETWIBTEST_ERR_4 (netwib_err)12304
#define NETWIBTEST_ERR_5 (netwib_err)12305
#define NETWIBTEST_ERR_6 (netwib_err)12306
#define NETWIBTEST_ERR_7 (netwib_err)12307
#define NETWIBTEST_ERR_8 (netwib_err)12308
#define NETWIBTEST_ERR_9 (netwib_err)12309

/*-------------------------------------------------------------*/
#include "netwibtest_util.h"

/*-------------------------------------------------------------*/
extern netwib_uint32 netwibtest_testkbdlevel;
extern netwib_bool netwibtest_testadm;

/*-------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

netwib_err netwibtest_dat_priv(void);
netwib_err netwibtest_dat_debug(void);
netwib_err netwibtest_dat_c(void);
netwib_err netwibtest_dat_c2(void);
netwib_err netwibtest_dat_ptr(void);
netwib_err netwibtest_dat_buf(void);
netwib_err netwibtest_dat_bufpool(void);
netwib_err netwibtest_dat_bufenc(void);
netwib_err netwibtest_dat_bufdec(void);
netwib_err netwibtest_dat_fmt(void);
netwib_err netwibtest_dat_checksum(void);
netwib_err netwibtest_dat_regexp(void);
netwib_err netwibtest_dat_ring(void);
netwib_err netwibtest_dat_ringi(void);
netwib_err netwibtest_dat_hash(void);
netwib_err netwibtest_dat_hashi(void);
netwib_err netwibtest_dat_tlv(void);
netwib_err netwibtest_dat_array(void);
netwib_err netwibtest_dat_uint64(void);

netwib_err netwibtest_sys_priv(void);
netwib_err netwibtest_sys_debug(void);
netwib_err netwibtest_sys_time(void);
netwib_err netwibtest_sys_rand(void);
netwib_err netwibtest_sys_path(void);
netwib_err netwibtest_sys_pathname(void);
netwib_err netwibtest_sys_pathstat(void);
netwib_err netwibtest_sys_dirname(void);
netwib_err netwibtest_sys_filename(void);
netwib_err netwibtest_sys_dir(void);
netwib_err netwibtest_sys_thread(void);
netwib_err netwibtest_sys_threadmut(void);
netwib_err netwibtest_sys_threadrwl(void);
netwib_err netwibtest_sys_threadcond(void);
netwib_err netwibtest_sys_threadtsd(void);
netwib_err netwibtest_sys_threadlist(void);
netwib_err netwibtest_sys_io(void);
netwib_err netwibtest_sys_ionew(void);
netwib_err netwibtest_sys_iousual(void);
netwib_err netwibtest_sys_wait(void);
netwib_err netwibtest_sys_waitlist(void);
netwib_err netwibtest_sys_file(void);
netwib_err netwibtest_sys_fd(void);
netwib_err netwibtest_sys_stream(void);
netwib_err netwibtest_sys_kbd(void);
netwib_err netwibtest_sys_kbdchar(void);
netwib_err netwibtest_sys_kbdint(void);
netwib_err netwibtest_sys_kbdbuf(void);
netwib_err netwibtest_sys_kbdpress(void);
netwib_err netwibtest_sys_screen(void);
netwib_err netwibtest_sys_bufenc(void);
netwib_err netwibtest_sys_bufdec(void);
netwib_err netwibtest_sys_disp(void);
netwib_err netwibtest_sys_record(void);
netwib_err netwibtest_sys_beep(void);
netwib_err netwibtest_sys_global(void);
netwib_err netwibtest_sys_init(void);
netwib_err netwibtest_sys_error(void);
netwib_err netwibtest_sys_internal(void);
netwib_err netwibtest_sys_unix(void);
netwib_err netwibtest_sys_windows(void);

netwib_err netwibtest_net_priv(void);
netwib_err netwibtest_net_debug(void);
netwib_err netwibtest_net_device(void);
netwib_err netwibtest_net_ip(void);
netwib_err netwibtest_net_eth(void);
netwib_err netwibtest_net_port(void);
netwib_err netwibtest_net_ips(void);
netwib_err netwibtest_net_ipsi(void);
netwib_err netwibtest_net_eths(void);
netwib_err netwibtest_net_ethsi(void);
netwib_err netwibtest_net_ports(void);
netwib_err netwibtest_net_portsi(void);
netwib_err netwibtest_net_conf(void);
netwib_err netwibtest_net_confdev(void);
netwib_err netwibtest_net_confip(void);
netwib_err netwibtest_net_confarp(void);
netwib_err netwibtest_net_confrout(void);
netwib_err netwibtest_net_sock(void);
netwib_err netwibtest_net_sniff(void);
netwib_err netwibtest_net_spoof(void);

netwib_err netwibtest_pkt_priv(void);
netwib_err netwibtest_pkt_debug(void);
netwib_err netwibtest_pkt_linkhdr(void);
netwib_err netwibtest_pkt_iphdr(void);
netwib_err netwibtest_pkt_ip4opt(void);
netwib_err netwibtest_pkt_ip4opts(void);
netwib_err netwibtest_pkt_ip6ext(void);
netwib_err netwibtest_pkt_ip6exts(void);
netwib_err netwibtest_pkt_arphdr(void);
netwib_err netwibtest_pkt_udphdr(void);
netwib_err netwibtest_pkt_tcphdr(void);
netwib_err netwibtest_pkt_tcpopt(void);
netwib_err netwibtest_pkt_tcpopts(void);
netwib_err netwibtest_pkt_ip64bits(void);
netwib_err netwibtest_pkt_icmp4(void);
netwib_err netwibtest_pkt_icmp6(void);
netwib_err netwibtest_pkt_icmp6nd(void);
netwib_err netwibtest_pkt_layer(void);
netwib_err netwibtest_pkt_packet(void);
netwib_err netwibtest_pkt_conv(void);
netwib_err netwibtest_pkt_ipfrag(void);

netwib_err netwibtest_shw_priv(void);
netwib_err netwibtest_shw_debug(void);
netwib_err netwibtest_shw_array(void);
netwib_err netwibtest_shw_linkhdr(void);
netwib_err netwibtest_shw_iphdr(void);
netwib_err netwibtest_shw_ip4opt(void);
netwib_err netwibtest_shw_ip4opts(void);
netwib_err netwibtest_shw_ip6ext(void);
netwib_err netwibtest_shw_ip6exts(void);
netwib_err netwibtest_shw_arphdr(void);
netwib_err netwibtest_shw_udphdr(void);
netwib_err netwibtest_shw_tcphdr(void);
netwib_err netwibtest_shw_tcpopt(void);
netwib_err netwibtest_shw_tcpopts(void);
netwib_err netwibtest_shw_icmp4(void);
netwib_err netwibtest_shw_icmp6(void);
netwib_err netwibtest_shw_icmp6nd(void);
netwib_err netwibtest_shw_packet(void);

#ifdef __cplusplus
}
#endif

/*-------------------------------------------------------------*/
#endif

