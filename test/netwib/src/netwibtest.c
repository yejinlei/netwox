
#include "netwibtest.h"

/*-------------------------------------------------------------*/
int main(int argc,
         char *argv[]);
static netwib_err main2(int argc,
                        char *argv[]);
static netwib_err calltest(netwib_uint32 minloop,
                           netwib_uint32 maxloop);

/*-------------------------------------------------------------*/
int main(int argc,
         char *argv[])
{
  netwib_err ret;

  ret = netwib_init();
  if (ret != NETWIB_ERR_OK) {
    netwib_err_display(ret, NETWIB_ERR_ENCODETYPE_FULL);
    return(ret);
  }

  ret = main2(argc, argv);
  if (ret != NETWIB_ERR_OK) {
    netwib_err_display(ret, NETWIB_ERR_ENCODETYPE_FULL);
    netwib_close();
    return(ret);
  }

  ret = netwib_close();
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_uint32 netwibtest_testkbdlevel;
netwib_bool netwibtest_testadm;
static netwib_err main2(int argc,
                        char *argv[])
{
  netwib_uint32 minloop, maxloop;
  netwib_err ret;

  if (argc < 2) {
    netwib_er(netwib_fmt_display("Usage : %s number [numbermax] [ka]\n",
                                 argv[0]));
    puts(" k : duplicate tests needing keyboard are not done");
    puts(" K : all tests needing keyboard are not done");
    puts(" a : tests needing administrator rights are not done");
    puts("Available numbers :");
    puts("  1 - test of dat/priv");
    puts("  2 - test of dat/debug");
    puts("  3 - test of dat/c");
    puts("  4 - test of dat/c2");
    puts("  5 - test of dat/ptr");
    puts("  6 - test of dat/buf");
    puts("  7 - test of dat/bufpool");
    puts("  8 - test of dat/bufenc");
    puts("  9 - test of dat/bufdec");
    puts(" 10 - test of dat/fmt");
    puts(" 11 - test of dat/checksum");
    puts(" 12 - test of dat/regexp");
    puts(" 13 - test of dat/ring");
    puts(" 14 - test of dat/ringi");
    puts(" 15 - test of dat/hash");
    puts(" 16 - test of dat/hashi");
    puts(" 17 - test of dat/tlv");
    puts(" 18 - test of dat/array");
    puts(" 19 - test of dat/uint64");
    puts("101 - test of sys/priv");
    puts("102 - test of sys/debug");
    puts("103 - test of sys/time");
    puts("104 - test of sys/rand");
    puts("105 - test of sys/path");
    puts("106 - test of sys/pathname");
    puts("107 - test of sys/pathstat");
    puts("108 - test of sys/dirname");
    puts("109 - test of sys/filename");
    puts("110 - test of sys/dir");
    puts("111 - test of sys/thread");
    puts("112 - test of sys/threadmut");
    puts("113 - test of sys/threadrwl");
    puts("114 - test of sys/threadcond");
    puts("115 - test of sys/threadtsd");
    puts("116 - test of sys/threadlist");
    puts("117 - test of sys/io");
    puts("118 - test of sys/ionew");
    puts("119 - test of sys/iousual");
    puts("120 - test of sys/wait");
    puts("121 - test of sys/waitlist");
    puts("122 - test of sys/file");
    puts("123 - test of sys/fd");
    puts("124 - test of sys/stream");
    puts("125 - test of sys/kbd");
    puts("126 - test of sys/kbdchar");
    puts("127 - test of sys/kbdint");
    puts("128 - test of sys/kbdbuf");
    puts("129 - test of sys/kbdpress");
    puts("130 - test of sys/screen");
    puts("131 - test of sys/bufenc");
    puts("132 - test of sys/bufdec");
    puts("133 - test of sys/disp");
    puts("134 - test of sys/record");
    puts("135 - test of sys/beep");
    puts("136 - test of sys/global");
    puts("137 - test of sys/init");
    puts("138 - test of sys/error");
    puts("139 - test of sys/internal");
    puts("140 - test of sys/unix");
    puts("141 - test of sys/windows");
    puts("201 - test of net/priv");
    puts("202 - test of net/debug");
    puts("203 - test of net/device");
    puts("204 - test of net/ip");
    puts("205 - test of net/eth");
    puts("206 - test of net/port");
    puts("207 - test of net/ips");
    puts("208 - test of net/ipsi");
    puts("209 - test of net/eths");
    puts("210 - test of net/ethsi");
    puts("211 - test of net/ports");
    puts("212 - test of net/portsi");
    puts("213 - test of net/conf");
    puts("214 - test of net/confdev");
    puts("215 - test of net/confip");
    puts("216 - test of net/confarp");
    puts("217 - test of net/confrout");
    puts("218 - test of net/sock");
    puts("219 - test of net/sniff");
    puts("220 - test of net/spoof");
    puts("301 - test of pkt/priv");
    puts("302 - test of pkt/debug");
    puts("303 - test of pkt/linkhdr");
    puts("304 - test of pkt/iphdr");
    puts("305 - test of pkt/ip4opt");
    puts("306 - test of pkt/ip4opts");
    puts("307 - test of pkt/ip6ext");
    puts("308 - test of pkt/ip6exts");
    puts("309 - test of pkt/arphdr");
    puts("310 - test of pkt/udphdr");
    puts("311 - test of pkt/tcphdr");
    puts("312 - test of pkt/tcpopt");
    puts("313 - test of pkt/tcpopts");
    puts("314 - test of pkt/ip64bits");
    puts("315 - test of pkt/icmp4");
    puts("316 - test of pkt/icmp6");
    puts("317 - test of pkt/icmp6nd");
    puts("318 - test of pkt/layer");
    puts("319 - test of pkt/packet");
    puts("320 - test of pkt/conv");
    puts("321 - test of pkt/ipfrag");
    puts("401 - test of shw/priv");
    puts("402 - test of shw/debug");
    puts("403 - test of shw/array");
    puts("404 - test of shw/linkhdr");
    puts("405 - test of shw/iphdr");
    puts("406 - test of shw/ip4opt");
    puts("407 - test of shw/ip4opts");
    puts("408 - test of shw/ip6ext");
    puts("409 - test of shw/ip6exts");
    puts("410 - test of shw/arphdr");
    puts("411 - test of shw/udphdr");
    puts("412 - test of shw/tcphdr");
    puts("413 - test of shw/tcpopt");
    puts("414 - test of shw/tcpopts");
    puts("415 - test of shw/icmp4");
    puts("416 - test of shw/icmp6");
    puts("417 - test of shw/icmp6nd");
    puts("418 - test of shw/packet");
    return(NETWIB_ERR_OK);
  }

  minloop = atoi(argv[1]);
  if (argc < 3) {
    maxloop = minloop;
  } else {
    maxloop = atoi(argv[2]);
  }

  netwibtest_testkbdlevel = 2;
  netwibtest_testadm = NETWIB_TRUE;
  if (argc > 3) {
    if (netwib_c_strchr(argv[3], 'k') != NULL) {
      netwibtest_testkbdlevel = 1;
    }
    if (netwib_c_strchr(argv[3], 'K') != NULL) {
      netwibtest_testkbdlevel = 0;
    }
    if (netwib_c_strchr(argv[3], 'a') != NULL) {
      netwibtest_testadm = NETWIB_FALSE;
    }
  }

  if (minloop == 0) {
    minloop = 1;
    maxloop = 500;
    netwib_er(netwib_fmt_display("All tests (%{uint32} to %{uint32})\n",
                                 minloop, maxloop));
  } else {
    if (minloop != maxloop) {
      netwib_er(netwib_fmt_display("Tests between %{uint32} and %{uint32}\n",
                                   minloop, maxloop));
    }
  }

  ret = calltest(minloop, maxloop);

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err calltest(netwib_uint32 minloop,
                           netwib_uint32 maxloop)
{
  netwib_uint32 choice;
  netwib_bool testdone;

  for (choice = minloop; choice <= maxloop; choice++) {
    testdone = NETWIB_TRUE;
    switch (choice) {
    case   1: netwib_er(netwibtest_dat_priv()); break;
    case   2: netwib_er(netwibtest_dat_debug()); break;
    case   3: netwib_er(netwibtest_dat_c()); break;
    case   4: netwib_er(netwibtest_dat_c2()); break;
    case   5: netwib_er(netwibtest_dat_ptr()); break;
    case   6: netwib_er(netwibtest_dat_buf()); break;
    case   7: netwib_er(netwibtest_dat_bufpool()); break;
    case   8: netwib_er(netwibtest_dat_bufenc()); break;
    case   9: netwib_er(netwibtest_dat_bufdec()); break;
    case  10: netwib_er(netwibtest_dat_fmt()); break;
    case  11: netwib_er(netwibtest_dat_checksum()); break;
    case  12: netwib_er(netwibtest_dat_regexp()); break;
    case  13: netwib_er(netwibtest_dat_ring()); break;
    case  14: netwib_er(netwibtest_dat_ringi()); break;
    case  15: netwib_er(netwibtest_dat_hash()); break;
    case  16: netwib_er(netwibtest_dat_hashi()); break;
    case  17: netwib_er(netwibtest_dat_tlv()); break;
    case  18: netwib_er(netwibtest_dat_array()); break;
    case  19: netwib_er(netwibtest_dat_uint64()); break;
    case 101: netwib_er(netwibtest_sys_priv()); break;
    case 102: netwib_er(netwibtest_sys_debug()); break;
    case 103: netwib_er(netwibtest_sys_time()); break;
    case 104: netwib_er(netwibtest_sys_rand()); break;
    case 105: netwib_er(netwibtest_sys_path()); break;
    case 106: netwib_er(netwibtest_sys_pathname()); break;
    case 107: netwib_er(netwibtest_sys_pathstat()); break;
    case 108: netwib_er(netwibtest_sys_dirname()); break;
    case 109: netwib_er(netwibtest_sys_filename()); break;
    case 110: netwib_er(netwibtest_sys_dir()); break;
    case 111: netwib_er(netwibtest_sys_thread()); break;
    case 112: netwib_er(netwibtest_sys_threadmut()); break;
    case 113: netwib_er(netwibtest_sys_threadrwl()); break;
    case 114: netwib_er(netwibtest_sys_threadcond()); break;
    case 115: netwib_er(netwibtest_sys_threadtsd()); break;
    case 116: netwib_er(netwibtest_sys_threadlist()); break;
    case 117: netwib_er(netwibtest_sys_io()); break;
    case 118: netwib_er(netwibtest_sys_ionew()); break;
    case 119: netwib_er(netwibtest_sys_iousual()); break;
    case 120: netwib_er(netwibtest_sys_wait()); break;
    case 121: netwib_er(netwibtest_sys_waitlist()); break;
    case 122: netwib_er(netwibtest_sys_file()); break;
    case 123: netwib_er(netwibtest_sys_fd()); break;
    case 124: netwib_er(netwibtest_sys_stream()); break;
    case 125: netwib_er(netwibtest_sys_kbd()); break;
    case 126: netwib_er(netwibtest_sys_kbdchar()); break;
    case 127: netwib_er(netwibtest_sys_kbdint()); break;
    case 128: netwib_er(netwibtest_sys_kbdbuf()); break;
    case 129: netwib_er(netwibtest_sys_kbdpress()); break;
    case 130: netwib_er(netwibtest_sys_screen()); break;
    case 131: netwib_er(netwibtest_sys_bufenc()); break;
    case 132: netwib_er(netwibtest_sys_bufdec()); break;
    case 133: netwib_er(netwibtest_sys_disp()); break;
    case 134: netwib_er(netwibtest_sys_record()); break;
    case 135: netwib_er(netwibtest_sys_beep()); break;
    case 136: netwib_er(netwibtest_sys_global()); break;
    case 137: netwib_er(netwibtest_sys_init()); break;
    case 138: netwib_er(netwibtest_sys_error()); break;
    case 139: netwib_er(netwibtest_sys_internal()); break;
    case 140: netwib_er(netwibtest_sys_unix()); break;
    case 141: netwib_er(netwibtest_sys_windows()); break;
    case 201: netwib_er(netwibtest_net_priv()); break;
    case 202: netwib_er(netwibtest_net_debug()); break;
    case 203: netwib_er(netwibtest_net_device()); break;
    case 204: netwib_er(netwibtest_net_ip()); break;
    case 205: netwib_er(netwibtest_net_eth()); break;
    case 206: netwib_er(netwibtest_net_port()); break;
    case 207: netwib_er(netwibtest_net_ips()); break;
    case 208: netwib_er(netwibtest_net_ipsi()); break;
    case 209: netwib_er(netwibtest_net_eths()); break;
    case 210: netwib_er(netwibtest_net_ethsi()); break;
    case 211: netwib_er(netwibtest_net_ports()); break;
    case 212: netwib_er(netwibtest_net_portsi()); break;
    case 213: netwib_er(netwibtest_net_conf()); break;
    case 214: netwib_er(netwibtest_net_confdev()); break;
    case 215: netwib_er(netwibtest_net_confip()); break;
    case 216: netwib_er(netwibtest_net_confarp()); break;
    case 217: netwib_er(netwibtest_net_confrout()); break;
    case 218: netwib_er(netwibtest_net_sock()); break;
    case 219: netwib_er(netwibtest_net_sniff()); break;
    case 220: netwib_er(netwibtest_net_spoof()); break;
    case 301: netwib_er(netwibtest_pkt_priv()); break;
    case 302: netwib_er(netwibtest_pkt_debug()); break;
    case 303: netwib_er(netwibtest_pkt_linkhdr()); break;
    case 304: netwib_er(netwibtest_pkt_iphdr()); break;
    case 305: netwib_er(netwibtest_pkt_ip4opt()); break;
    case 306: netwib_er(netwibtest_pkt_ip4opts()); break;
    case 307: netwib_er(netwibtest_pkt_ip6ext()); break;
    case 308: netwib_er(netwibtest_pkt_ip6exts()); break;
    case 309: netwib_er(netwibtest_pkt_arphdr()); break;
    case 310: netwib_er(netwibtest_pkt_udphdr()); break;
    case 311: netwib_er(netwibtest_pkt_tcphdr()); break;
    case 312: netwib_er(netwibtest_pkt_tcpopt()); break;
    case 313: netwib_er(netwibtest_pkt_tcpopts()); break;
    case 314: netwib_er(netwibtest_pkt_ip64bits()); break;
    case 315: netwib_er(netwibtest_pkt_icmp4()); break;
    case 316: netwib_er(netwibtest_pkt_icmp6()); break;
    case 317: netwib_er(netwibtest_pkt_icmp6nd()); break;
    case 318: netwib_er(netwibtest_pkt_layer()); break;
    case 319: netwib_er(netwibtest_pkt_packet()); break;
    case 320: netwib_er(netwibtest_pkt_conv()); break;
    case 321: netwib_er(netwibtest_pkt_ipfrag()); break;
    case 401: netwib_er(netwibtest_shw_priv()); break;
    case 402: netwib_er(netwibtest_shw_debug()); break;
    case 403: netwib_er(netwibtest_shw_array()); break;
    case 404: netwib_er(netwibtest_shw_linkhdr()); break;
    case 405: netwib_er(netwibtest_shw_iphdr()); break;
    case 406: netwib_er(netwibtest_shw_ip4opt()); break;
    case 407: netwib_er(netwibtest_shw_ip4opts()); break;
    case 408: netwib_er(netwibtest_shw_ip6ext()); break;
    case 409: netwib_er(netwibtest_shw_ip6exts()); break;
    case 410: netwib_er(netwibtest_shw_arphdr()); break;
    case 411: netwib_er(netwibtest_shw_udphdr()); break;
    case 412: netwib_er(netwibtest_shw_tcphdr()); break;
    case 413: netwib_er(netwibtest_shw_tcpopt()); break;
    case 414: netwib_er(netwibtest_shw_tcpopts()); break;
    case 415: netwib_er(netwibtest_shw_icmp4()); break;
    case 416: netwib_er(netwibtest_shw_icmp6()); break;
    case 417: netwib_er(netwibtest_shw_icmp6nd()); break;
    case 418: netwib_er(netwibtest_shw_packet()); break;
    default:
      testdone = NETWIB_FALSE;
    }
    if (testdone) {
      netwib_er(netwib_fmt_display("Test number %{uint32} successful.\n\n",
                                   choice));
    }
  }

  return(NETWIB_ERR_OK);
}

