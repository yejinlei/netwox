#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_icmp6type_test(void)
{
  netwib_buf buf;

  puts("++++----\nTest of netwib_buf_append_icmp6type");

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_icmp6type(NETWIB_ICMP6TYPE_DSTUNREACH, &buf));
  netwib_er(netwibtest_buf_content3(&buf, "'destination unreachable'"));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_icmp6code_test(void)
{
  netwib_buf buf;

  puts("++++----\nTest of netwib_buf_append_icmp6code");

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_icmp6code(NETWIB_ICMP6TYPE_DSTUNREACH,
                                        NETWIB_ICMP6CODE_DSTUNREACH_HOST,
                                        &buf));
  netwib_er(netwibtest_buf_content3(&buf, "'host'"));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_icmp6_initdefault_test(void)
{
  netwib_icmp6 icmp6;

  puts("++++----\nTest of netwib_icmp6_initdefault");

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_DSTUNREACH, &icmp6));
  netwib_er(netwibtest_icmp6_content(&icmp6, "0103000000000000"));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_PKTTOOBIG, &icmp6));
  netwib_er(netwibtest_icmp6_content(&icmp6, "0200000000000000"));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_TIMEEXCEED, &icmp6));
  netwib_er(netwibtest_icmp6_content(&icmp6, "0300000000000000"));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_PARAPROB, &icmp6));
  netwib_er(netwibtest_icmp6_content(&icmp6, "0400000000000000"));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_ECHOREQ, &icmp6));
  icmp6.msg.echo.id = 1;
  icmp6.msg.echo.seqnum = 2;
  netwib_er(netwibtest_icmp6_content(&icmp6, "8000000000010002"));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_ECHOREP, &icmp6));
  netwib_er(netwibtest_icmp6_content(&icmp6, "8100000000000000"));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_ROUTERSOLICIT, &icmp6));
  netwib_er(netwibtest_icmp6_content(&icmp6, "8500000000000000"));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_ROUTERADVERT, &icmp6));
  netwib_er(netwibtest_icmp6_content(&icmp6,
                                     "86000000000000000000000000000000"));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_NEIGHBORSOLICIT,
                                     &icmp6));
  netwib_er(netwibtest_icmp6_content(&icmp6, "870000000000000000000000000000000000000000000000"));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_NEIGHBORADVERT, &icmp6));
  netwib_er(netwibtest_icmp6_content(&icmp6, "880000000000000000000000000000000000000000000000"));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_REDIRECT, &icmp6));
  netwib_er(netwibtest_icmp6_content(&icmp6, "89000000000000000000000000000000000000000000000000000000000000000000000000000000"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_icmp6_test2(netwib_icmp6 *picmp6,
                                                netwib_conststring mixed,
                                                netwib_uint32 wantedskipsize)
{
  netwib_icmp6 icmp62;
  netwib_uint32 skipsize;
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_icmp6(picmp6, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, mixed));

  netwib_er(netwib_pkt_decode_icmp6(&buf, &icmp62, &skipsize));
  if (skipsize != wantedskipsize) {
    netwib_er(netwib_fmt_display("%{uint32} != %{uint32}\n", skipsize,
                                 wantedskipsize));
    return(NETWIBTEST_ERR_4);
  }
  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_icmp6(&icmp62, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, mixed));

  netwib_er(netwib_buf_close(&buf));
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_pkt_append_icmp6_test(void)
{
  netwib_buf bufmixed, buf;
  netwib_icmp6 icmp6;

  puts("++++----\nTest of netwib_pkt_append_icmp6");

  netwib_er(netwib_buf_init_ext_string("45120021FADE000080112E871122334455667788 00010002000DAAAA 'hello'", &bufmixed));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_decode(&bufmixed, NETWIB_DECODETYPE_MIXED, &buf));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_DSTUNREACH, &icmp6));
  icmp6.msg.dstunreach.badippacket = buf;
  netwib_er(netwib_pkt_append_icmp6_test2(&icmp6, "0103000000000000 45120021FADE000080112E871122334455667788 00010002000DAAAA", 36));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_PKTTOOBIG, &icmp6));
  icmp6.msg.pkttoobig.badippacket = buf;
  netwib_er(netwib_pkt_append_icmp6_test2(&icmp6, "0200000000000000 45120021FADE000080112E871122334455667788 00010002000DAAAA", 36));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_TIMEEXCEED, &icmp6));
  icmp6.msg.timeexceed.badippacket = buf;
  netwib_er(netwib_pkt_append_icmp6_test2(&icmp6, "0300000000000000 45120021FADE000080112E871122334455667788 00010002000DAAAA", 36));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_PARAPROB, &icmp6));
  icmp6.msg.paraprob.badippacket = buf;
  netwib_er(netwib_pkt_append_icmp6_test2(&icmp6, "0400000000000000 45120021FADE000080112E871122334455667788 00010002000DAAAA", 36));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_ECHOREQ, &icmp6));
  icmp6.msg.echo.id = 1;
  icmp6.msg.echo.seqnum = 2;
  netwib_er(netwib_buf_init_ext_string("1234", &icmp6.msg.echo.data));
  netwib_er(netwib_pkt_append_icmp6_test2(&icmp6, "8000000000010002 31323334", 12));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_ECHOREP, &icmp6));
  netwib_er(netwib_buf_init_ext_string("1234", &icmp6.msg.echo.data));
  netwib_er(netwib_pkt_append_icmp6_test2(&icmp6, "8100000000000000 31323334", 12));

  netwib_er(netwib_buf_close(&buf));

  /**/
  netwib_er(netwib_buf_init_ext_string("0101010203040506 0304018200000003000000040000000500000001000000020000000300000004 0501000100000002", &bufmixed));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_decode(&bufmixed, NETWIB_DECODETYPE_MIXED, &buf));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_ROUTERSOLICIT, &icmp6));
  icmp6.msg.routersolicit.reserved = 1;
  icmp6.msg.routersolicit.options = buf;
  netwib_er(netwib_pkt_append_icmp6_test2(&icmp6, "8500000000000001010101020304050603040182000000030000000400000005000000010000000200000003000000040501000100000002", 56));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_ROUTERADVERT, &icmp6));
  icmp6.msg.routeradvert.curhoplimit = 1;
  icmp6.msg.routeradvert.managedaddress = NETWIB_TRUE;
  icmp6.msg.routeradvert.otherstateful = NETWIB_TRUE;
  icmp6.msg.routeradvert.reserved = 2;
  icmp6.msg.routeradvert.routerlifetime = 3;
  icmp6.msg.routeradvert.reachabletime = 4;
  icmp6.msg.routeradvert.retranstimer = 5;
  icmp6.msg.routeradvert.options = buf;
  netwib_er(netwib_pkt_append_icmp6_test2(&icmp6, "8600000001C200030000000400000005010101020304050603040182000000030000000400000005000000010000000200000003000000040501000100000002", 64));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_NEIGHBORSOLICIT,
                                     &icmp6));
  icmp6.msg.neighborsolicit.reserved = 5;
  netwib_er(netwib_ip_init_ip6_fields(1,2,3,4,
                                      &icmp6.msg.neighborsolicit.target));
  icmp6.msg.neighborsolicit.options = buf;
  netwib_er(netwib_pkt_append_icmp6_test2(&icmp6, "870000000000000500000001000000020000000300000004010101020304050603040182000000030000000400000005000000010000000200000003000000040501000100000002", 72));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_NEIGHBORADVERT, &icmp6));
  icmp6.msg.neighboradvert.router = NETWIB_TRUE;
  icmp6.msg.neighboradvert.solicited = NETWIB_TRUE;
  icmp6.msg.neighboradvert.override = NETWIB_TRUE;
  icmp6.msg.neighboradvert.reserved = 5;
  netwib_er(netwib_ip_init_ip6_fields(1,2,3,4,
                                      &icmp6.msg.neighboradvert.target));
  icmp6.msg.neighboradvert.options = buf;
  netwib_er(netwib_pkt_append_icmp6_test2(&icmp6, "88000000E000000500000001000000020000000300000004010101020304050603040182000000030000000400000005000000010000000200000003000000040501000100000002", 72));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_REDIRECT, &icmp6));
  icmp6.msg.redirect.reserved = 9;
  netwib_er(netwib_ip_init_ip6_fields(1,2,3,4,
                                      &icmp6.msg.redirect.target));
  netwib_er(netwib_ip_init_ip6_fields(5,6,7,8,
                                      &icmp6.msg.redirect.dst));
  icmp6.msg.redirect.options = buf;
  netwib_er(netwib_pkt_append_icmp6_test2(&icmp6, "89000000000000090000000100000002000000030000000400000005000000060000000700000008010101020304050603040182000000030000000400000005000000010000000200000003000000040501000100000002", 88));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_icmp6_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_icmp6");
  puts("Redundant with netwib_pkt_append_icmp6");
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_pkt_icmp6(void)
{
  puts("###########################################\nTest of pkt/icmp6");
  netwib_er(netwib_buf_append_icmp6type_test());
  netwib_er(netwib_buf_append_icmp6code_test());

  netwib_er(netwib_icmp6_initdefault_test());
  netwib_er(netwib_pkt_append_icmp6_test());
  netwib_er(netwib_pkt_decode_icmp6_test());

  return(NETWIB_ERR_OK);
}
