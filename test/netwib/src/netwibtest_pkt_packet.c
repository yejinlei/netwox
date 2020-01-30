#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_linkdata_test(void)
{
  netwib_linkhdr linkhdr, linkhdr2;
  netwib_buf buf, data, datad;

  puts("++++----\nTest of netwib_pkt_append_linkdata");
  netwib_er(netwibtest_linkhdr_init(&linkhdr));
  netwib_er(netwib_buf_init_mallocdefault(&data));
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_buf_append_string("hello", &data));
  netwib_er(netwib_pkt_append_linkdata(&linkhdr, &data, &buf));
  netwib_er(netwibtest_buf_content3(&buf, "0102030405061122334455660800 'hello'"));

  netwib__buf_reinit(&data);
  netwib_er(netwib_pkt_decode_linkdata(linkhdr.type, &buf, &linkhdr2, &datad));
  netwib_er(netwibtest_linkhdr_content(&linkhdr2, "0102030405061122334455660800"));
  netwib_er(netwibtest_buf_content3(&datad, "'hello'"));

  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_close(&data));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_linkdata_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_linkdata");
  puts("Redundant with netwib_pkt_append_linkdata");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_ipdata_test2(netwib_constiphdr *piphdr,
                                                 netwib_conststring mixedpkt,
                                                 netwib_conststring mixedip)
{
  netwib_iphdr iphdr2;
  netwib_buf buf, data, datad;

  netwib_er(netwib_buf_init_mallocdefault(&data));
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_buf_append_string("hello", &data));
  netwib_er(netwib_pkt_append_ipdata(piphdr, &data, &buf));
  netwib_er(netwibtest_buf_content3(&buf, mixedpkt));

  netwib_er(netwib_pkt_ip_display(&buf, NULL, NETWIB_ENCODETYPE_ARRAY,
                                  NETWIB_ENCODETYPE_DUMP));

  netwib_er(netwib_pkt_decode_ipdata(&buf, &iphdr2, &datad));
  netwib_er(netwibtest_iphdr_content(&iphdr2, mixedip));
  netwib_er(netwibtest_buf_content3(&datad, "'hello'"));

  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_close(&data));

  netwib_er(netwib_fmt_display("------\n"));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_pkt_append_ipdata_test(void)
{
  netwib_iphdr iphdr;
  netwib_buf opt;

  puts("++++----\nTest of netwib_pkt_append_ipdata");
  netwib_er(netwib_buf_init_mallocdefault(&opt));

  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwib_pkt_append_ipdata_test2(&iphdr, "45120019FADE000080062E9A1122334455667788 'hello'", "45120019FADE000080062E9A1122334455667788"));

  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwib_buf_append_string("1234", &opt));
  iphdr.header.ip4.opts = opt;
  netwib_er(netwib_pkt_append_ipdata_test2(&iphdr, "4612001DFADE00008006C92F1122334455667788 '1234' 'hello'", "4612001DFADE00008006C92F1122334455667788 '1234'"));

  netwib_er(netwibtest_ip6hdr_init(&iphdr));
  netwib_er(netwib_pkt_append_ipdata_test2(&iphdr, "681004D2000506411122334400000005000000060000000755667788000000080000000900000000 'hello'", "681004D2000506411122334400000005000000060000000755667788000000080000000900000000"));

  netwib__buf_reinit(&opt);
  netwib_er(netwibtest_ip6hdr_init(&iphdr));
  netwib_er(netwibtest_ip6exts_init(NETWIB_IPPROTO_TCP, &opt));
  iphdr.protocol = NETWIB_IPPROTO_HOPOPTS;
  iphdr.header.ip6.exts = opt;
  netwib_er(netwib_pkt_append_ipdata_test2(&iphdr, "681004D2001500411122334400000005000000060000000755667788000000080000000900000000 3C0168656C6C6F00 06016F6C6C656800 'hello'", "681004D2001500411122334400000005000000060000000755667788000000080000000900000000 3C0168656C6C6F00 06016F6C6C656800"));

  netwib_er(netwib_buf_close(&opt));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_ipdata_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_ipdata");
  puts("Redundant with netwib_pkt_append_ipdata");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_linkipdata_test2(netwib_constlinkhdr *plinkhdr,
                                                     netwib_constiphdr *piphdr,
                                                     netwib_conststring mixedpkt,
                                                     netwib_conststring mixedlink,
                                                     netwib_conststring mixedip)
{
  netwib_linkhdr linkhdr2;
  netwib_iphdr iphdr2;
  netwib_buf buf, data, datad;

  netwib_er(netwib_buf_init_mallocdefault(&data));
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_buf_append_string("hello", &data));
  netwib_er(netwib_pkt_append_linkipdata(plinkhdr, piphdr, &data, &buf));
  netwib_er(netwibtest_buf_content3(&buf, mixedpkt));

  netwib_er(netwib_pkt_link_display(plinkhdr->type, &buf, NULL,
                                    NETWIB_ENCODETYPE_ARRAY,
                                    NETWIB_ENCODETYPE_DUMP));

  netwib_er(netwib_pkt_decode_linkipdata(plinkhdr->type, &buf, &linkhdr2,
                                         &iphdr2, &datad));
  netwib_er(netwibtest_linkhdr_content(&linkhdr2, mixedlink));
  netwib_er(netwibtest_iphdr_content(&iphdr2, mixedip));
  netwib_er(netwibtest_buf_content3(&datad, "'hello'"));

  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_close(&data));

  netwib_er(netwib_fmt_display("------\n"));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_pkt_append_linkipdata_test(void)
{
  netwib_linkhdr linkhdr;
  netwib_iphdr iphdr;
  netwib_buf opt;

  puts("++++----\nTest of netwib_pkt_append_linkipdata");
  netwib_er(netwib_buf_init_mallocdefault(&opt));

  netwib_er(netwibtest_linkhdr_init(&linkhdr));
  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwib_pkt_append_linkipdata_test2(&linkhdr, &iphdr, "0102030405061122334455660800 45120019FADE000080062E9A1122334455667788 'hello'", "0102030405061122334455660800", "45120019FADE000080062E9A1122334455667788"));

  netwib_er(netwibtest_linkhdr_init(&linkhdr));
  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwib_buf_append_string("1234", &opt));
  iphdr.header.ip4.opts = opt;
  netwib_er(netwib_pkt_append_linkipdata_test2(&linkhdr, &iphdr, "0102030405061122334455660800 4612001DFADE00008006C92F1122334455667788 '1234' 'hello'", "0102030405061122334455660800", "4612001DFADE00008006C92F1122334455667788 '1234'"));

  netwib_er(netwibtest_linkhdr_init(&linkhdr));
  netwib_er(netwibtest_ip6hdr_init(&iphdr));
  netwib_er(netwib_pkt_append_linkipdata_test2(&linkhdr, &iphdr, "01020304050611223344556686DD 681004D2000506411122334400000005000000060000000755667788000000080000000900000000 'hello'", "01020304050611223344556686DD", "681004D2000506411122334400000005000000060000000755667788000000080000000900000000"));

  netwib__buf_reinit(&opt);
  netwib_er(netwibtest_linkhdr_init(&linkhdr));
  netwib_er(netwibtest_ip6hdr_init(&iphdr));
  netwib_er(netwibtest_ip6exts_init(NETWIB_IPPROTO_TCP, &opt));
  iphdr.protocol = NETWIB_IPPROTO_HOPOPTS;
  iphdr.header.ip6.exts = opt;
  netwib_er(netwib_pkt_append_linkipdata_test2(&linkhdr, &iphdr, "01020304050611223344556686DD 681004D2001500411122334400000005000000060000000755667788000000080000000900000000 3C0168656C6C6F00 06016F6C6C656800 'hello'", "01020304050611223344556686DD", "681004D2001500411122334400000005000000060000000755667788000000080000000900000000 3C0168656C6C6F00 06016F6C6C656800"));

  netwib_er(netwib_buf_close(&opt));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_linkipdata_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_linkipdata");
  puts("Redundant with netwib_pkt_append_linkipdata");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_ipudpdata_test2(netwib_constiphdr *piphdr,
                                                 netwib_constudphdr *pudphdr,
                                                 netwib_conststring mixedpkt,
                                                 netwib_conststring mixedip,
                                                 netwib_conststring mixedudp)
{
  netwib_iphdr iphdr2;
  netwib_udphdr udphdr2;
  netwib_buf buf, data, datad;

  netwib_er(netwib_buf_init_mallocdefault(&data));
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_buf_append_string("hello", &data));
  netwib_er(netwib_pkt_append_ipudpdata(piphdr, pudphdr, &data, &buf));
  netwib_er(netwibtest_buf_content3(&buf, mixedpkt));

  netwib_er(netwib_pkt_ip_display(&buf, NULL, NETWIB_ENCODETYPE_ARRAY,
                                  NETWIB_ENCODETYPE_DUMP));

  netwib_er(netwib_pkt_decode_ipudpdata(&buf, &iphdr2, &udphdr2, &datad));
  netwib_er(netwibtest_iphdr_content(&iphdr2, mixedip));
  netwib_er(netwibtest_udphdr_content(&udphdr2, mixedudp));
  netwib_er(netwibtest_buf_content3(&datad, "'hello'"));

  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_close(&data));

  netwib_er(netwib_fmt_display("------\n"));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_pkt_append_ipudpdata_test(void)
{
  netwib_udphdr udpheader;
  netwib_iphdr iphdr;
  netwib_buf opt;

  puts("++++----\nTest of netwib_pkt_append_ipudpdata");
  netwib_er(netwib_buf_init_mallocdefault(&opt));

  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwibtest_udphdr_init(&udpheader));
  netwib_er(netwib_pkt_append_ipudpdata_test2(&iphdr, &udpheader, "45120021FADE000080112E871122334455667788 00010002000DAAAA 'hello'", "45120021FADE000080112E871122334455667788", "00010002000DAAAA"));

  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwib_buf_append_string("1234", &opt));
  iphdr.header.ip4.opts = opt;
  netwib_er(netwibtest_udphdr_init(&udpheader));
  netwib_er(netwib_pkt_append_ipudpdata_test2(&iphdr, &udpheader, "46120025FADE00008011C91C1122334455667788 '1234' 00010002000DAAAA 'hello'", "46120025FADE00008011C91C1122334455667788 '1234'", "00010002000DAAAA"));

  netwib_er(netwibtest_ip6hdr_init(&iphdr));
  netwib_er(netwibtest_udphdr_init(&udpheader));
  netwib_er(netwib_pkt_append_ipudpdata_test2(&iphdr, &udpheader, "681004D2000D11411122334400000005000000060000000755667788000000080000000900000000 00010002000DAA87 'hello'", "681004D2000D11411122334400000005000000060000000755667788000000080000000900000000", "00010002000DAA87"));

  netwib__buf_reinit(&opt);
  netwib_er(netwibtest_ip6hdr_init(&iphdr));
  netwib_er(netwibtest_ip6exts_init(NETWIB_IPPROTO_IP, &opt));
  iphdr.protocol = NETWIB_IPPROTO_HOPOPTS;
  iphdr.header.ip6.exts = opt;
  netwib_er(netwibtest_udphdr_init(&udpheader));
  netwib_er(netwib_pkt_append_ipudpdata_test2(&iphdr, &udpheader, "681004D2001D00411122334400000005000000060000000755667788000000080000000900000000 3C0168656C6C6F00 11016F6C6C656800 00010002000DAA87 'hello'", "681004D2001D00411122334400000005000000060000000755667788000000080000000900000000 3C0168656C6C6F00 11016F6C6C656800", "00010002000DAA87"));

  netwib_er(netwib_buf_close(&opt));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_ipudpdata_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_ipudpdata");
  puts("Redundant with netwib_pkt_append_ipudpdata");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_linkipudpdata_test2(netwib_constlinkhdr *plinkhdr,
                                                 netwib_constiphdr *piphdr,
                                                 netwib_constudphdr *pudphdr,
                                                 netwib_conststring mixedpkt,
                                                 netwib_conststring mixedlink,
                                                 netwib_conststring mixedip,
                                                 netwib_conststring mixedudp)
{
  netwib_linkhdr linkhdr2;
  netwib_iphdr iphdr2;
  netwib_udphdr udphdr2;
  netwib_buf buf, data, datad;

  netwib_er(netwib_buf_init_mallocdefault(&data));
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_buf_append_string("hello", &data));
  netwib_er(netwib_pkt_append_linkipudpdata(plinkhdr, piphdr, pudphdr, &data,
                                            &buf));
  netwib_er(netwibtest_buf_content3(&buf, mixedpkt));

  netwib_er(netwib_pkt_link_display(plinkhdr->type, &buf, NULL,
                                    NETWIB_ENCODETYPE_ARRAY,
                                    NETWIB_ENCODETYPE_DUMP));

  netwib_er(netwib_pkt_decode_linkipudpdata(plinkhdr->type, &buf, &linkhdr2,
                                            &iphdr2, &udphdr2, &datad));
  netwib_er(netwibtest_linkhdr_content(&linkhdr2, mixedlink));
  netwib_er(netwibtest_iphdr_content(&iphdr2, mixedip));
  netwib_er(netwibtest_udphdr_content(&udphdr2, mixedudp));
  netwib_er(netwibtest_buf_content3(&datad, "'hello'"));

  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_close(&data));

  netwib_er(netwib_fmt_display("------\n"));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_pkt_append_linkipudpdata_test(void)
{
  netwib_linkhdr linkhdr;
  netwib_iphdr iphdr;
  netwib_udphdr udpheader;
  netwib_buf opt;

  puts("++++----\nTest of netwib_pkt_append_linkipudpdata");
  netwib_er(netwib_buf_init_mallocdefault(&opt));

  netwib_er(netwibtest_linkhdr_init(&linkhdr));
  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwibtest_udphdr_init(&udpheader));
  netwib_er(netwib_pkt_append_linkipudpdata_test2(&linkhdr, &iphdr, &udpheader, "0102030405061122334455660800 45120021FADE000080112E871122334455667788 00010002000DAAAA 'hello'", "0102030405061122334455660800", "45120021FADE000080112E871122334455667788", "00010002000DAAAA"));

  netwib_er(netwibtest_linkhdr_init(&linkhdr));
  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwib_buf_append_string("1234", &opt));
  iphdr.header.ip4.opts = opt;
  netwib_er(netwibtest_udphdr_init(&udpheader));
  netwib_er(netwib_pkt_append_linkipudpdata_test2(&linkhdr, &iphdr, &udpheader, "0102030405061122334455660800 46120025FADE00008011C91C1122334455667788 '1234' 00010002000DAAAA 'hello'", "0102030405061122334455660800", "46120025FADE00008011C91C1122334455667788 '1234'", "00010002000DAAAA"));

  netwib_er(netwibtest_linkhdr_init(&linkhdr));
  netwib_er(netwibtest_ip6hdr_init(&iphdr));
  netwib_er(netwibtest_udphdr_init(&udpheader));
  netwib_er(netwib_pkt_append_linkipudpdata_test2(&linkhdr, &iphdr, &udpheader, "01020304050611223344556686DD 681004D2000D11411122334400000005000000060000000755667788000000080000000900000000 00010002000DAA87 'hello'", "01020304050611223344556686DD", "681004D2000D11411122334400000005000000060000000755667788000000080000000900000000", "00010002000DAA87"));

  netwib__buf_reinit(&opt);
  netwib_er(netwibtest_linkhdr_init(&linkhdr));
  netwib_er(netwibtest_ip6hdr_init(&iphdr));
  netwib_er(netwibtest_ip6exts_init(NETWIB_IPPROTO_IP, &opt));
  iphdr.protocol = NETWIB_IPPROTO_HOPOPTS;
  iphdr.header.ip6.exts = opt;
  netwib_er(netwibtest_udphdr_init(&udpheader));
  netwib_er(netwib_pkt_append_linkipudpdata_test2(&linkhdr, &iphdr, &udpheader, "01020304050611223344556686DD 681004D2001D00411122334400000005000000060000000755667788000000080000000900000000 3C0168656C6C6F00 11016F6C6C656800 00010002000DAA87 'hello'", "01020304050611223344556686DD", "681004D2001D00411122334400000005000000060000000755667788000000080000000900000000 3C0168656C6C6F00 11016F6C6C656800", "00010002000DAA87"));

  netwib_er(netwib_buf_close(&opt));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_linkipudpdata_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_linkipudpdata");
  puts("Redundant with netwib_pkt_append_linkipudpdata");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_iptcpdata_test2(netwib_constiphdr *piphdr,
                                                 netwib_consttcphdr *ptcphdr,
                                                 netwib_conststring mixedpkt,
                                                 netwib_conststring mixedip,
                                                 netwib_conststring mixedtcp)
{
  netwib_iphdr iphdr2;
  netwib_tcphdr tcphdr2;
  netwib_buf buf, data, datad;

  netwib_er(netwib_buf_init_mallocdefault(&data));
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_buf_append_string("hello", &data));
  netwib_er(netwib_pkt_append_iptcpdata(piphdr, ptcphdr, &data, &buf));
  netwib_er(netwibtest_buf_content3(&buf, mixedpkt));

  netwib_er(netwib_pkt_ip_display(&buf, NULL, NETWIB_ENCODETYPE_ARRAY,
                                  NETWIB_ENCODETYPE_DUMP));

  netwib_er(netwib_pkt_decode_iptcpdata(&buf, &iphdr2, &tcphdr2, &datad));
  netwib_er(netwibtest_iphdr_content(&iphdr2, mixedip));
  netwib_er(netwibtest_tcphdr_content(&tcphdr2, mixedtcp));
  netwib_er(netwibtest_buf_content3(&datad, "'hello'"));

  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_close(&data));

  netwib_er(netwib_fmt_display("------\n"));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_pkt_append_iptcpdata_test(void)
{
  netwib_iphdr iphdr;
  netwib_tcphdr tcpheader;
  netwib_buf opt, opt2;

  puts("++++----\nTest of netwib_pkt_append_iptcpdata");
  netwib_er(netwib_buf_init_mallocdefault(&opt));
  netwib_er(netwib_buf_init_mallocdefault(&opt2));

  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwibtest_tcphdr_init(&tcpheader));
  netwib_er(netwib_pkt_append_iptcpdata_test2(&iphdr, &tcpheader, "4512002DFADE000080062E861122334455667788 0001000211223344000000005A553434C5B41212 'hello'", "4512002DFADE000080062E861122334455667788", "0001000211223344000000005A553434C5B41212"));

  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwib_buf_append_string("1234", &opt));
  iphdr.header.ip4.opts = opt;
  netwib_er(netwibtest_tcphdr_init(&tcpheader));
  netwib_er(netwib_pkt_append_iptcpdata_test2(&iphdr, &tcpheader, "46120031FADE00008006C91B1122334455667788 '1234' 0001000211223344000000005A553434C5B41212 'hello'", "46120031FADE00008006C91B1122334455667788 '1234'", "0001000211223344000000005A553434C5B41212"));

  netwib__buf_reinit(&opt);
  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwib_buf_append_string("1234", &opt));
  iphdr.header.ip4.opts = opt;
  netwib_er(netwibtest_tcphdr_init(&tcpheader));
  netwib_er(netwib_buf_append_string("5678", &opt2));
  tcpheader.opts = opt2;
  netwib_er(netwib_pkt_append_iptcpdata_test2(&iphdr, &tcpheader, "46120035FADE00008006C9171122334455667788 '1234' 0001000211223344000000006A55343449421212 '5678' 'hello'", "46120035FADE00008006C9171122334455667788 '1234'", "0001000211223344000000006A55343449421212 '5678'"));

  netwib_er(netwibtest_ip6hdr_init(&iphdr));
  netwib_er(netwibtest_tcphdr_init(&tcpheader));
  netwib_er(netwib_pkt_append_iptcpdata_test2(&iphdr, &tcpheader, "681004D2001906411122334400000005000000060000000755667788000000080000000900000000 0001000211223344000000005A553434C5911212 'hello'", "681004D2001906411122334400000005000000060000000755667788000000080000000900000000", "0001000211223344000000005A553434C5911212"));

  netwib__buf_reinit(&opt);
  netwib_er(netwibtest_ip6hdr_init(&iphdr));
  netwib_er(netwibtest_ip6exts_init(NETWIB_IPPROTO_IP, &opt));
  iphdr.protocol = NETWIB_IPPROTO_HOPOPTS;
  iphdr.header.ip6.exts = opt;
  netwib_er(netwibtest_tcphdr_init(&tcpheader));
  netwib_er(netwib_pkt_append_iptcpdata_test2(&iphdr, &tcpheader, "681004D2002900411122334400000005000000060000000755667788000000080000000900000000 3C0168656C6C6F00 06016F6C6C656800 0001000211223344000000005A553434C5911212 'hello'", "681004D2002900411122334400000005000000060000000755667788000000080000000900000000 3C0168656C6C6F00 06016F6C6C656800", "0001000211223344000000005A553434C5911212"));

  netwib__buf_reinit(&opt);
  netwib_er(netwibtest_ip6hdr_init(&iphdr));
  netwib_er(netwibtest_ip6exts_init(NETWIB_IPPROTO_IP, &opt));
  iphdr.protocol = NETWIB_IPPROTO_HOPOPTS;
  iphdr.header.ip6.exts = opt;
  netwib_er(netwibtest_tcphdr_init(&tcpheader));
  netwib__buf_reinit(&opt2);
  netwib_er(netwib_buf_append_string("5678", &opt2));
  tcpheader.opts = opt2;
  netwib_er(netwib_pkt_append_iptcpdata_test2(&iphdr, &tcpheader, "681004D2002D00411122334400000005000000060000000755667788000000080000000900000000 3C0168656C6C6F00 06016F6C6C656800 0001000211223344000000006A553434491F1212 '5678' 'hello'", "681004D2002D00411122334400000005000000060000000755667788000000080000000900000000 3C0168656C6C6F00 06016F6C6C656800", "0001000211223344000000006A553434491F1212 '5678'"));

  netwib_er(netwib_buf_close(&opt2));
  netwib_er(netwib_buf_close(&opt));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_iptcpdata_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_iptcpdata");
  puts("Redundant with netwib_pkt_append_iptcpdata");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_linkiptcpdata_test2(netwib_constlinkhdr *plinkhdr,
                                                 netwib_constiphdr *piphdr,
                                                 netwib_consttcphdr *ptcphdr,
                                                 netwib_conststring mixedpkt,
                                                 netwib_conststring mixedlink,
                                                 netwib_conststring mixedip,
                                                 netwib_conststring mixedtcp)
{
  netwib_linkhdr linkhdr2;
  netwib_iphdr iphdr2;
  netwib_tcphdr tcphdr2;
  netwib_buf buf, data, datad;

  netwib_er(netwib_buf_init_mallocdefault(&data));
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_buf_append_string("hello", &data));
  netwib_er(netwib_pkt_append_linkiptcpdata(plinkhdr, piphdr, ptcphdr, &data,
                                            &buf));
  netwib_er(netwibtest_buf_content3(&buf, mixedpkt));

  netwib_er(netwib_pkt_link_display(plinkhdr->type, &buf, NULL,
                                    NETWIB_ENCODETYPE_ARRAY,
                                    NETWIB_ENCODETYPE_DUMP));

  netwib_er(netwib_pkt_decode_linkiptcpdata(plinkhdr->type, &buf, &linkhdr2,
                                            &iphdr2, &tcphdr2, &datad));
  netwib_er(netwibtest_linkhdr_content(&linkhdr2, mixedlink));
  netwib_er(netwibtest_iphdr_content(&iphdr2, mixedip));
  netwib_er(netwibtest_tcphdr_content(&tcphdr2, mixedtcp));
  netwib_er(netwibtest_buf_content3(&datad, "'hello'"));

  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_close(&data));

  netwib_er(netwib_fmt_display("------\n"));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_pkt_append_linkiptcpdata_test(void)
{
  netwib_linkhdr linkhdr;
  netwib_tcphdr tcpheader;
  netwib_iphdr iphdr;
  netwib_buf opt, opt2;

  puts("++++----\nTest of netwib_pkt_append_linkiptcpdata");
  netwib_er(netwib_buf_init_mallocdefault(&opt));
  netwib_er(netwib_buf_init_mallocdefault(&opt2));

  netwib_er(netwibtest_linkhdr_init(&linkhdr));
  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwibtest_tcphdr_init(&tcpheader));
  netwib_er(netwib_pkt_append_linkiptcpdata_test2(&linkhdr, &iphdr, &tcpheader, "0102030405061122334455660800 4512002DFADE000080062E861122334455667788 0001000211223344000000005A553434C5B41212 'hello'", "0102030405061122334455660800", "4512002DFADE000080062E861122334455667788", "0001000211223344000000005A553434C5B41212"));

  netwib_er(netwibtest_linkhdr_init(&linkhdr));
  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwib_buf_append_string("1234", &opt));
  iphdr.header.ip4.opts = opt;
  netwib_er(netwibtest_tcphdr_init(&tcpheader));
  netwib_er(netwib_pkt_append_linkiptcpdata_test2(&linkhdr, &iphdr, &tcpheader, "0102030405061122334455660800 46120031FADE00008006C91B1122334455667788 '1234' 0001000211223344000000005A553434C5B41212 'hello'", "0102030405061122334455660800", "46120031FADE00008006C91B1122334455667788 '1234'", "0001000211223344000000005A553434C5B41212"));

  netwib__buf_reinit(&opt);
  netwib_er(netwibtest_linkhdr_init(&linkhdr));
  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwib_buf_append_string("1234", &opt));
  iphdr.header.ip4.opts = opt;
  netwib_er(netwibtest_tcphdr_init(&tcpheader));
  netwib_er(netwib_buf_append_string("5678", &opt2));
  tcpheader.opts = opt2;
  netwib_er(netwib_pkt_append_linkiptcpdata_test2(&linkhdr, &iphdr, &tcpheader, "0102030405061122334455660800 46120035FADE00008006C9171122334455667788 '1234' 0001000211223344000000006A55343449421212 '5678' 'hello'", "0102030405061122334455660800", "46120035FADE00008006C9171122334455667788 '1234'", "0001000211223344000000006A55343449421212 '5678'"));

  netwib_er(netwibtest_linkhdr_init(&linkhdr));
  netwib_er(netwibtest_ip6hdr_init(&iphdr));
  netwib_er(netwibtest_tcphdr_init(&tcpheader));
  netwib_er(netwib_pkt_append_linkiptcpdata_test2(&linkhdr, &iphdr, &tcpheader, "01020304050611223344556686DD 681004D2001906411122334400000005000000060000000755667788000000080000000900000000 0001000211223344000000005A553434C5911212 'hello'", "01020304050611223344556686DD", "681004D2001906411122334400000005000000060000000755667788000000080000000900000000", "0001000211223344000000005A553434C5911212"));

  netwib__buf_reinit(&opt);
  netwib_er(netwibtest_linkhdr_init(&linkhdr));
  netwib_er(netwibtest_ip6hdr_init(&iphdr));
  netwib_er(netwibtest_ip6exts_init(NETWIB_IPPROTO_IP, &opt));
  iphdr.protocol = NETWIB_IPPROTO_HOPOPTS;
  iphdr.header.ip6.exts = opt;
  netwib_er(netwibtest_tcphdr_init(&tcpheader));
  netwib_er(netwib_pkt_append_linkiptcpdata_test2(&linkhdr, &iphdr, &tcpheader, "01020304050611223344556686DD 681004D2002900411122334400000005000000060000000755667788000000080000000900000000 3C0168656C6C6F00 06016F6C6C656800 0001000211223344000000005A553434C5911212 'hello'", "01020304050611223344556686DD", "681004D2002900411122334400000005000000060000000755667788000000080000000900000000 3C0168656C6C6F00 06016F6C6C656800", "0001000211223344000000005A553434C5911212"));

  netwib__buf_reinit(&opt);
  netwib_er(netwibtest_linkhdr_init(&linkhdr));
  netwib_er(netwibtest_ip6hdr_init(&iphdr));
  netwib_er(netwibtest_ip6exts_init(NETWIB_IPPROTO_IP, &opt));
  iphdr.protocol = NETWIB_IPPROTO_HOPOPTS;
  iphdr.header.ip6.exts = opt;
  netwib_er(netwibtest_tcphdr_init(&tcpheader));
  netwib__buf_reinit(&opt2);
  netwib_er(netwib_buf_append_string("5678", &opt2));
  tcpheader.opts = opt2;
  netwib_er(netwib_pkt_append_linkiptcpdata_test2(&linkhdr, &iphdr, &tcpheader, "01020304050611223344556686DD 681004D2002D00411122334400000005000000060000000755667788000000080000000900000000 3C0168656C6C6F00 06016F6C6C656800 0001000211223344000000006A553434491F1212 '5678' 'hello'", "01020304050611223344556686DD", "681004D2002D00411122334400000005000000060000000755667788000000080000000900000000 3C0168656C6C6F00 06016F6C6C656800", "0001000211223344000000006A553434491F1212 '5678'"));

  netwib_er(netwib_buf_close(&opt2));
  netwib_er(netwib_buf_close(&opt));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_linkiptcpdata_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_linkiptcpdata");
  puts("Redundant with netwib_pkt_append_linkiptcpdata");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_ipicmp4_test2(netwib_constiphdr *piphdr,
                                                 netwib_consticmp4 *picmp4,
                                                 netwib_conststring mixedpkt,
                                                 netwib_conststring mixedip,
                                                 netwib_conststring mixedicmp4)
{
  netwib_iphdr iphdr2;
  netwib_icmp4 icmp42;
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_pkt_append_ipicmp4(piphdr, picmp4, &buf));
  netwib_er(netwibtest_buf_content3(&buf, mixedpkt));

  netwib_er(netwib_pkt_ip_display(&buf, NULL, NETWIB_ENCODETYPE_ARRAY,
                                  NETWIB_ENCODETYPE_DUMP));

  netwib_er(netwib_pkt_decode_ipicmp4(&buf, &iphdr2, &icmp42));
  netwib_er(netwibtest_iphdr_content(&iphdr2, mixedip));
  netwib_er(netwibtest_icmp4_content(&icmp42, mixedicmp4));

  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_fmt_display("------\n"));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_pkt_append_ipicmp4_test(void)
{
  netwib_icmp4 icmp4header;
  netwib_iphdr iphdr;
  netwib_buf opt;

  puts("++++----\nTest of netwib_pkt_append_ipicmp4");
  netwib_er(netwib_buf_init_mallocdefault(&opt));

  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwibtest_icmp4_init(&icmp4header));
  netwib_er(netwib_pkt_append_ipicmp4_test2(&iphdr, &icmp4header, "4512001CFADE000080012E9C1122334455667788 0800F7FC00010002", "4512001CFADE000080012E9C1122334455667788", "0800F7FC00010002"));

  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwib_buf_append_string("1234", &opt));
  iphdr.header.ip4.opts = opt;
  netwib_er(netwibtest_icmp4_init(&icmp4header));
  netwib_er(netwib_pkt_append_ipicmp4_test2(&iphdr, &icmp4header, "46120020FADE00008001C9311122334455667788 '1234' 0800F7FC00010002", "46120020FADE00008001C9311122334455667788 '1234'", "0800F7FC00010002"));

  netwib_er(netwib_buf_close(&opt));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_ipicmp4_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_ipicmp4");
  puts("Redundant with netwib_pkt_append_ipicmp4");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_linkipicmp4_test2(netwib_constlinkhdr *plinkhdr,
                                                 netwib_constiphdr *piphdr,
                                                 netwib_consticmp4 *picmp4,
                                                 netwib_conststring mixedpkt,
                                                 netwib_conststring mixedlink,
                                                 netwib_conststring mixedip,
                                                 netwib_conststring mixedicmp4)
{
  netwib_linkhdr linkhdr2;
  netwib_iphdr iphdr2;
  netwib_icmp4 icmp42;
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_pkt_append_linkipicmp4(plinkhdr, piphdr, picmp4, &buf));
  netwib_er(netwibtest_buf_content3(&buf, mixedpkt));

  netwib_er(netwib_pkt_link_display(plinkhdr->type, &buf, NULL,
                                    NETWIB_ENCODETYPE_ARRAY,
                                    NETWIB_ENCODETYPE_DUMP));

  netwib_er(netwib_pkt_decode_linkipicmp4(plinkhdr->type, &buf, &linkhdr2,
                                          &iphdr2, &icmp42));
  netwib_er(netwibtest_linkhdr_content(&linkhdr2, mixedlink));
  netwib_er(netwibtest_iphdr_content(&iphdr2, mixedip));
  netwib_er(netwibtest_icmp4_content(&icmp42, mixedicmp4));

  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_fmt_display("------\n"));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_pkt_append_linkipicmp4_test(void)
{
  netwib_linkhdr linkhdr;
  netwib_iphdr iphdr;
  netwib_icmp4 icmp4header;
  netwib_buf opt;

  puts("++++----\nTest of netwib_pkt_append_linkipicmp4");
  netwib_er(netwib_buf_init_mallocdefault(&opt));

  netwib_er(netwibtest_linkhdr_init(&linkhdr));
  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwibtest_icmp4_init(&icmp4header));
  netwib_er(netwib_pkt_append_linkipicmp4_test2(&linkhdr, &iphdr, &icmp4header, "0102030405061122334455660800 4512001CFADE000080012E9C1122334455667788 0800F7FC00010002", "0102030405061122334455660800", "4512001CFADE000080012E9C1122334455667788", "0800F7FC00010002"));

  netwib_er(netwibtest_linkhdr_init(&linkhdr));
  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwib_buf_append_string("1234", &opt));
  iphdr.header.ip4.opts = opt;
  netwib_er(netwibtest_icmp4_init(&icmp4header));
  netwib_er(netwib_pkt_append_linkipicmp4_test2(&linkhdr, &iphdr, &icmp4header, "0102030405061122334455660800 46120020FADE00008001C9311122334455667788 '1234' 0800F7FC00010002", "0102030405061122334455660800", "46120020FADE00008001C9311122334455667788 '1234'", "0800F7FC00010002"));

  netwib_er(netwib_buf_close(&opt));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_linkipicmp4_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_linkipicmp4");
  puts("Redundant with netwib_pkt_append_linkipicmp4");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_ipicmp6_test2(netwib_constiphdr *piphdr,
                                                 netwib_consticmp6 *picmp6,
                                                 netwib_conststring mixedpkt,
                                                 netwib_conststring mixedip,
                                                 netwib_conststring mixedicmp6)
{
  netwib_iphdr iphdr2;
  netwib_icmp6 icmp62;
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_pkt_append_ipicmp6(piphdr, picmp6, &buf));
  netwib_er(netwibtest_buf_content3(&buf, mixedpkt));

  netwib_er(netwib_pkt_ip_display(&buf, NULL, NETWIB_ENCODETYPE_ARRAY,
                                  NETWIB_ENCODETYPE_DUMP));

  netwib_er(netwib_pkt_decode_ipicmp6(&buf, &iphdr2, &icmp62));
  netwib_er(netwibtest_iphdr_content(&iphdr2, mixedip));
  netwib_er(netwibtest_icmp6_content(&icmp62, mixedicmp6));

  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_fmt_display("------\n"));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_pkt_append_ipicmp6_test(void)
{
  netwib_icmp6 icmp6header;
  netwib_iphdr iphdr;
  netwib_buf opt;

  puts("++++----\nTest of netwib_pkt_append_ipicmp6");
  netwib_er(netwib_buf_init_mallocdefault(&opt));

  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwibtest_icmp6_init(&icmp6header));
  netwib_er(netwib_pkt_append_ipicmp6_test2(&iphdr, &icmp6header, "4512001CFADE0000803A2E631122334455667788 80006E6500010002", "4512001CFADE0000803A2E631122334455667788", "80006E6500010002"));

  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwib_buf_append_string("1234", &opt));
  iphdr.header.ip4.opts = opt;
  netwib_er(netwibtest_icmp6_init(&icmp6header));
  netwib_er(netwib_pkt_append_ipicmp6_test2(&iphdr, &icmp6header, "46120020FADE0000803AC8F81122334455667788 '1234' 80006E6500010002", "46120020FADE0000803AC8F81122334455667788 '1234'", "80006E6500010002"));

  netwib_er(netwib_buf_close(&opt));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_ipicmp6_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_ipicmp6");
  puts("Redundant with netwib_pkt_append_ipicmp6");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_linkipicmp6_test2(netwib_constlinkhdr *plinkhdr,
                                                 netwib_constiphdr *piphdr,
                                                 netwib_consticmp6 *picmp6,
                                                 netwib_conststring mixedpkt,
                                                 netwib_conststring mixedlink,
                                                 netwib_conststring mixedip,
                                                 netwib_conststring mixedicmp6)
{
  netwib_linkhdr linkhdr2;
  netwib_iphdr iphdr2;
  netwib_icmp6 icmp62;
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_pkt_append_linkipicmp6(plinkhdr, piphdr, picmp6, &buf));
  netwib_er(netwibtest_buf_content3(&buf, mixedpkt));

  netwib_er(netwib_pkt_link_display(plinkhdr->type, &buf, NULL,
                                    NETWIB_ENCODETYPE_ARRAY,
                                    NETWIB_ENCODETYPE_DUMP));

  netwib_er(netwib_pkt_decode_linkipicmp6(plinkhdr->type, &buf, &linkhdr2,
                                          &iphdr2, &icmp62));
  netwib_er(netwibtest_linkhdr_content(&linkhdr2, mixedlink));
  netwib_er(netwibtest_iphdr_content(&iphdr2, mixedip));
  netwib_er(netwibtest_icmp6_content(&icmp62, mixedicmp6));

  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_fmt_display("------\n"));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_pkt_append_linkipicmp6_test(void)
{
  netwib_linkhdr linkhdr;
  netwib_iphdr iphdr;
  netwib_icmp6 icmp6header;
  netwib_buf opt;

  puts("++++----\nTest of netwib_pkt_append_linkipicmp6");
  netwib_er(netwib_buf_init_mallocdefault(&opt));

  netwib_er(netwibtest_linkhdr_init(&linkhdr));
  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwibtest_icmp6_init(&icmp6header));
  netwib_er(netwib_pkt_append_linkipicmp6_test2(&linkhdr, &iphdr, &icmp6header, "0102030405061122334455660800 4512001CFADE0000803A2E631122334455667788 80006E6500010002", "0102030405061122334455660800", "4512001CFADE0000803A2E631122334455667788", "80006E6500010002"));

  netwib_er(netwibtest_linkhdr_init(&linkhdr));
  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwib_buf_append_string("1234", &opt));
  iphdr.header.ip4.opts = opt;
  netwib_er(netwibtest_icmp6_init(&icmp6header));
  netwib_er(netwib_pkt_append_linkipicmp6_test2(&linkhdr, &iphdr, &icmp6header, "0102030405061122334455660800 46120020FADE0000803AC8F81122334455667788 '1234' 80006E6500010002", "0102030405061122334455660800", "46120020FADE0000803AC8F81122334455667788 '1234'", "80006E6500010002"));

  netwib_er(netwib_buf_close(&opt));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_linkipicmp6_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_linkipicmp6");
  puts("Redundant with netwib_pkt_append_linkipicmp6");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_linkarp_test(void)
{
  netwib_linkhdr linkhdr, linkhdr2;
  netwib_arphdr arpheader, arpheader2;
  netwib_buf buf;

  puts("++++----\nTest of netwib_pkt_append_linkarp");
  netwib_er(netwibtest_linkhdr_init(&linkhdr));
  linkhdr.hdr.ether.type = NETWIB_ETHERHDRTYPE_ARP;
  netwib_er(netwibtest_arphdr_init(&arpheader));
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_pkt_append_linkarp(&linkhdr, &arpheader, &buf));
  netwib_er(netwibtest_buf_content3(&buf, "0102030405061122334455660806 00010800060400010102030405061122334400000000000055667788"));
  netwib_er(netwib_pkt_decode_linkarp(linkhdr.type, &buf, &linkhdr2, &arpheader2));
  netwib_er(netwibtest_linkhdr_content(&linkhdr2, "0102030405061122334455660806"));
  netwib_er(netwibtest_arphdr_content(&arpheader2, "00010800060400010102030405061122334400000000000055667788"));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_linkarp_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_linkarp");
  puts("Redundant with netwib_pkt_append_linkarp");
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_pkt_packet(void)
{
  puts("###########################################\nTest of pkt/packet");
  netwib_er(netwib_pkt_append_linkdata_test());
  netwib_er(netwib_pkt_decode_linkdata_test());
  netwib_er(netwib_pkt_append_ipdata_test());
  netwib_er(netwib_pkt_decode_ipdata_test());
  netwib_er(netwib_pkt_append_linkipdata_test());
  netwib_er(netwib_pkt_decode_linkipdata_test());
  netwib_er(netwib_pkt_append_ipudpdata_test());
  netwib_er(netwib_pkt_decode_ipudpdata_test());
  netwib_er(netwib_pkt_append_linkipudpdata_test());
  netwib_er(netwib_pkt_decode_linkipudpdata_test());
  netwib_er(netwib_pkt_append_iptcpdata_test());
  netwib_er(netwib_pkt_decode_iptcpdata_test());
  netwib_er(netwib_pkt_append_linkiptcpdata_test());
  netwib_er(netwib_pkt_decode_linkiptcpdata_test());
  netwib_er(netwib_pkt_append_ipicmp4_test());
  netwib_er(netwib_pkt_decode_ipicmp4_test());
  netwib_er(netwib_pkt_append_linkipicmp4_test());
  netwib_er(netwib_pkt_decode_linkipicmp4_test());
  netwib_er(netwib_pkt_append_ipicmp6_test());
  netwib_er(netwib_pkt_decode_ipicmp6_test());
  netwib_er(netwib_pkt_append_linkipicmp6_test());
  netwib_er(netwib_pkt_decode_linkipicmp6_test());
  netwib_er(netwib_pkt_append_linkarp_test());
  netwib_er(netwib_pkt_decode_linkarp_test());

  return(NETWIB_ERR_OK);
}
