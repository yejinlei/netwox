#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_layer_data_test(void)
{
  netwib_buf buf, data;

  puts("++++----\nTest of netwib_pkt_append_layer_data");
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&data));

  netwib_er(netwib_buf_append_string("hello", &data));
  netwib_er(netwib_pkt_append_layer_data(&data, &buf));
  netwib_er(netwibtest_buf_content3(&buf, "'hello'"));
  netwib_er(netwib_buf_close(&data));
  netwib_er(netwib_pkt_decode_layer_data(&buf, &data));
  netwib_er(netwibtest_buf_content3(&data, "'hello'"));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_layer_data_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_layer_data");
  puts("Redundant with netwib_pkt_append_layer_data");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_layer_link_test(void)
{
  netwib_linkhdr linkhdr, linkhdr2;
  netwib_buf buf;

  puts("++++----\nTest of netwib_pkt_append_layer_link");
  netwib_er(netwibtest_linkhdr_init(&linkhdr));
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_pkt_append_layer_link(&linkhdr, &buf));
  netwib_er(netwibtest_buf_content3(&buf, "0102030405061122334455660800"));

  netwib_er(netwib_pkt_decode_layer_link(linkhdr.type, &buf, &linkhdr2));
  netwib_er(netwibtest_linkhdr_content(&linkhdr2, "0102030405061122334455660800"));
  netwib_er(netwibtest_buf_content3(&buf, ""));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_layer_link_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_layer_link");
  puts("Redundant with netwib_pkt_append_layer_link");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_layer_ip_test2(netwib_constiphdr *piphdr,
                                                   netwib_conststring mixed)
{
  netwib_iphdr iphdr2;
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_pkt_append_layer_ip(piphdr, 5, &buf));
  netwib_er(netwibtest_buf_content3(&buf, mixed));
  netwib_er(netwib_pkt_decode_layer_ip(&buf, &iphdr2));
  netwib_er(netwibtest_iphdr_content(&iphdr2, mixed));
  netwib_er(netwibtest_buf_content3(&buf, ""));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_pkt_append_layer_ip_test(void)
{
  netwib_iphdr iphdr;
  netwib_buf opt;

  puts("++++----\nTest of netwib_pkt_append_layer_ip");
  netwib_er(netwib_buf_init_mallocdefault(&opt));

  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwib_pkt_append_layer_ip_test2(&iphdr, "45120019FADE000080062E9A1122334455667788"));

  netwib__buf_reinit(&opt);
  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwib_buf_append_string("1234", &opt));
  iphdr.header.ip4.opts = opt;
  netwib_er(netwib_pkt_append_layer_ip_test2(&iphdr, "4612001DFADE00008006C92F1122334455667788 '1234'"));

  netwib_er(netwibtest_ip6hdr_init(&iphdr));
  netwib_er(netwib_pkt_append_layer_ip_test2(&iphdr, "681004D2000506411122334400000005000000060000000755667788000000080000000900000000"));

  netwib__buf_reinit(&opt);
  netwib_er(netwibtest_ip6hdr_init(&iphdr));
  netwib_er(netwibtest_ip6exts_init(NETWIB_IPPROTO_TCP, &opt));
  iphdr.protocol = NETWIB_IPPROTO_HOPOPTS;
  iphdr.header.ip6.exts = opt;
  netwib_er(netwib_pkt_append_layer_ip_test2(&iphdr, "681004D2001500411122334400000005000000060000000755667788000000080000000900000000 3C0168656C6C6F00 06016F6C6C656800"));

  netwib_er(netwib_buf_close(&opt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_layer_ip_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_layer_ip");
  puts("Redundant with netwib_pkt_append_layer_ip");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_layer_udp_test2(netwib_constiphdr *piphdr,
                                                  netwib_constudphdr *pudphdr,
                                                    netwib_conststring mixed)
{
  netwib_udphdr udpheader2;
  netwib_buf buf, data;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&data));

  netwib_er(netwib_buf_append_string("hello", &data));
  netwib_er(netwib_pkt_append_layer_udp(piphdr, pudphdr, &data, &buf));
  netwib_er(netwibtest_buf_content3(&buf, mixed));
  netwib_er(netwib_pkt_decode_layer_udp(&buf, &udpheader2));
  netwib_er(netwibtest_udphdr_content(&udpheader2, mixed));
  netwib_er(netwibtest_buf_content3(&buf, ""));

  netwib_er(netwib_buf_close(&data));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_pkt_append_layer_udp_test(void)
{
  netwib_iphdr iphdr;
  netwib_udphdr udpheader;

  puts("++++----\nTest of netwib_pkt_append_layer_udp");
  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwibtest_udphdr_init(&udpheader));
  netwib_er(netwib_pkt_append_layer_udp_test2(&iphdr, &udpheader,
                                              "00010002000DAAAA"));

  netwib_er(netwibtest_ip6hdr_init(&iphdr));
  netwib_er(netwibtest_udphdr_init(&udpheader));
  netwib_er(netwib_pkt_append_layer_udp_test2(&iphdr, &udpheader,
                                              "00010002000DAA87"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_layer_udp_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_layer_udp");
  puts("Redundant with netwib_pkt_append_layer_udp");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_layer_tcp_test2(netwib_constiphdr *piphdr,
                                                  netwib_consttcphdr *ptcphdr,
                                                    netwib_conststring mixed)
{
  netwib_tcphdr tcpheader2;
  netwib_buf buf, data;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&data));

  netwib_er(netwib_buf_append_string("hello", &data));
  netwib_er(netwib_pkt_append_layer_tcp(piphdr, ptcphdr, &data, &buf));
  netwib_er(netwibtest_buf_content3(&buf, mixed));
  netwib_er(netwib_pkt_decode_layer_tcp(&buf, &tcpheader2));
  netwib_er(netwibtest_tcphdr_content(&tcpheader2, mixed));
  netwib_er(netwibtest_buf_content3(&buf, ""));

  netwib_er(netwib_buf_close(&data));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_pkt_append_layer_tcp_test(void)
{
  netwib_iphdr iphdr;
  netwib_tcphdr tcpheader;
  netwib_buf opt;

  puts("++++----\nTest of netwib_pkt_append_layer_tcp");
  netwib_er(netwib_buf_init_mallocdefault(&opt));

  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwibtest_tcphdr_init(&tcpheader));
  netwib_er(netwib_pkt_append_layer_tcp_test2(&iphdr, &tcpheader, "0001000211223344000000005A553434C5B41212"));

  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwibtest_tcphdr_init(&tcpheader));
  netwib_er(netwib_buf_init_ext_string("1234", &tcpheader.opts));
  netwib_er(netwib_pkt_append_layer_tcp_test2(&iphdr, &tcpheader, "0001000211223344000000006A553434514A1212 '1234'"));

  netwib_er(netwibtest_ip6hdr_init(&iphdr));
  netwib_er(netwibtest_tcphdr_init(&tcpheader));
  netwib_er(netwib_pkt_append_layer_tcp_test2(&iphdr, &tcpheader, "0001000211223344000000005A553434C5911212"));

  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwibtest_ip6exts_init(NETWIB_IPPROTO_TCP, &opt));
  iphdr.protocol = NETWIB_IPPROTO_HOPOPTS;
  iphdr.header.ip6.exts = opt;
  netwib_er(netwibtest_tcphdr_init(&tcpheader));
  netwib_er(netwib_buf_init_ext_string("1234", &tcpheader.opts));
  netwib_er(netwib_pkt_append_layer_tcp_test2(&iphdr, &tcpheader, "0001000211223344000000006A553434514A1212 '1234'"));

  netwib_er(netwib_buf_close(&opt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_layer_tcp_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_layer_tcp");
  puts("Redundant with netwib_pkt_append_layer_tcp");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_layer_icmp4_test(void)
{
  netwib_icmp4 icmp4, icmp42;
  netwib_buf buf;

  puts("++++----\nTest of netwib_pkt_append_layer_icmp4");
  netwib_er(netwibtest_icmp4_init(&icmp4));
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_pkt_append_layer_icmp4(&icmp4, &buf));
  netwib_er(netwibtest_buf_content3(&buf, "0800F7FC00010002"));

  netwib_er(netwib_pkt_decode_layer_icmp4(&buf, &icmp42));
  netwib_er(netwibtest_icmp4_content(&icmp42, "0800F7FC00010002"));
  netwib_er(netwibtest_buf_content3(&buf, ""));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_layer_icmp4_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_layer_icmp4");
  puts("Redundant with netwib_pkt_append_layer_icmp4");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_layer_icmp6_test(void)
{
  netwib_iphdr iphdr;
  netwib_icmp6 icmp6, icmp62;
  netwib_buf buf;

  puts("++++----\nTest of netwib_pkt_append_layer_icmp6");
  netwib_er(netwibtest_ip6hdr_init(&iphdr));
  netwib_er(netwibtest_icmp6_init(&icmp6));
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_pkt_append_layer_icmp6(&iphdr, &icmp6, &buf));
  netwib_er(netwibtest_buf_content3(&buf, "80006E4200010002"));

  netwib_er(netwib_pkt_decode_layer_icmp6(&buf, &icmp62));
  netwib_er(netwibtest_icmp6_content(&icmp62, "80006E4200010002"));
  netwib_er(netwibtest_buf_content3(&buf, ""));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_layer_icmp6_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_layer_icmp6");
  puts("Redundant with netwib_pkt_append_layer_icmp6");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_layer_arp_test(void)
{
  netwib_arphdr arpheader, arpheader2;
  netwib_buf buf;

  puts("++++----\nTest of netwib_pkt_append_layer_arp");
  netwib_er(netwibtest_arphdr_init(&arpheader));
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_pkt_append_layer_arp(&arpheader, &buf));
  netwib_er(netwibtest_buf_content3(&buf, "00010800060400010102030405061122334400000000000055667788"));

  netwib_er(netwib_pkt_decode_layer_arp(&buf, &arpheader2));
  netwib_er(netwibtest_arphdr_content(&arpheader2, "00010800060400010102030405061122334400000000000055667788"));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_layer_arp_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_layer_arp");
  puts("Redundant with netwib_pkt_append_layer_arp");
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_pkt_layer(void)
{
  puts("###########################################\nTest of pkt/layer");
  netwib_er(netwib_pkt_append_layer_data_test());
  netwib_er(netwib_pkt_decode_layer_data_test());
  netwib_er(netwib_pkt_append_layer_link_test());
  netwib_er(netwib_pkt_decode_layer_link_test());
  netwib_er(netwib_pkt_append_layer_ip_test());
  netwib_er(netwib_pkt_decode_layer_ip_test());
  netwib_er(netwib_pkt_append_layer_udp_test());
  netwib_er(netwib_pkt_decode_layer_udp_test());
  netwib_er(netwib_pkt_append_layer_tcp_test());
  netwib_er(netwib_pkt_decode_layer_tcp_test());
  netwib_er(netwib_pkt_append_layer_icmp4_test());
  netwib_er(netwib_pkt_decode_layer_icmp4_test());
  netwib_er(netwib_pkt_append_layer_icmp6_test());
  netwib_er(netwib_pkt_decode_layer_icmp6_test());
  netwib_er(netwib_pkt_append_layer_arp_test());
  netwib_er(netwib_pkt_decode_layer_arp_test());

  return(NETWIB_ERR_OK);
}
