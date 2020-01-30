#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_icmp6_show_test2(netwib_consticmp6 *picmp6,
                                          netwib_encodetype encodetype,
                                          netwib_conststring wantmixed)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_icmp6_show(picmp6, encodetype, &buf));
  netwib_er(netwib_fmt_display("%{buf}\n", &buf));
  if (netwib_c_strlen(wantmixed)) {
    netwib_er(netwibtest_buf_content3(&buf, wantmixed));
  }

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_icmp6_show_test(void)
{
  netwib_buf bufmixed, buf;
  netwib_icmp6 icmp6;
  netwib_buf buftext;
  netwib_string text;

  puts("++++----\nTest of netwib_icmp6_show");
  netwib_er(netwib_buf_init_mallocdefault(&buftext));

  netwib_er(netwib_buf_init_ext_string("45120021FADE000080112E871122334455667788 00010002000DAAAA 'hello'", &bufmixed));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_decode(&bufmixed, NETWIB_DECODETYPE_MIXED, &buf));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_DSTUNREACH, &icmp6));
  icmp6.msg.dstunreach.badippacket = buf;
  netwib_er(netwib_icmp6_show_test2(&icmp6, NETWIB_ENCODETYPE_SYNTH,
                                    "'icmp6:1:3'"));
  netwib_er(netwib_icmp6_show_test2(&icmp6, NETWIB_ENCODETYPE_ARRAY, ""));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_PKTTOOBIG, &icmp6));
  icmp6.msg.pkttoobig.badippacket = buf;
  netwib_er(netwib_icmp6_show_test2(&icmp6, NETWIB_ENCODETYPE_SYNTH,
                                    "'icmp6:2:0'"));
  netwib_er(netwib_icmp6_show_test2(&icmp6, NETWIB_ENCODETYPE_ARRAY, ""));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_TIMEEXCEED, &icmp6));
  icmp6.msg.timeexceed.badippacket = buf;
  netwib_er(netwib_icmp6_show_test2(&icmp6, NETWIB_ENCODETYPE_SYNTH,
                                    "'icmp6:3:0'"));
  netwib_er(netwib_icmp6_show_test2(&icmp6, NETWIB_ENCODETYPE_ARRAY, ""));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_PARAPROB, &icmp6));
  icmp6.msg.paraprob.badippacket = buf;
  netwib_er(netwib_icmp6_show_test2(&icmp6, NETWIB_ENCODETYPE_SYNTH,
                                    "'icmp6:4:0'"));
  netwib_er(netwib_icmp6_show_test2(&icmp6, NETWIB_ENCODETYPE_ARRAY, ""));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_ECHOREQ, &icmp6));
  icmp6.msg.echo.id = 1;
  icmp6.msg.echo.seqnum = 2;
  netwib_er(netwib_buf_init_ext_string("1234", &icmp6.msg.echo.data));
  netwib_er(netwib_icmp6_show_test2(&icmp6, NETWIB_ENCODETYPE_SYNTH,
                                    "'icmp6:128:0'"));
  netwib__buf_reinit(&buftext);
  netwib_er(netwib_buf_append_string("'ICMP6_echo request______________________________________________.' 0A '|     type      |     code      |           checksum            |' 0A '|___0x80=128____|_", &buftext));
  netwib_er(netwib_buf_append_string("___0x00=0_____|___________0x0000=0____________|' 0A '|              id               |            seqnum             |' 0A '|___________0x0001=1____________|___________0x0002=2____________|' 0A '| data: 31323334                                                |' 0A '|_______________________________________________________________|' 0A", &buftext));
  netwib_er(netwib_buf_ref_string(&buftext, &text));
  netwib_er(netwib_icmp6_show_test2(&icmp6, NETWIB_ENCODETYPE_ARRAY, text));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_ECHOREP, &icmp6));
  netwib_er(netwib_buf_init_ext_string("1234", &icmp6.msg.echo.data));
  netwib_er(netwib_icmp6_show_test2(&icmp6, NETWIB_ENCODETYPE_SYNTH,
                                    "'icmp6:129:0'"));
  netwib__buf_reinit(&buftext);
  netwib_er(netwib_buf_append_string("'ICMP6_echo reply________________________________________________.' 0A '|     type      |     code      |           checksum            |' 0A '|___0x81=129____|____0x00=0_____|___________0x0000=0____________|' 0A '|              id          ", &buftext));
  netwib_er(netwib_buf_append_string("     |            seqnum             |' 0A '|___________0x0000=0____________|___________0x0000=0____________|' 0A '| data: 31323334                                                |' 0A '|_______________________________________________________________|' 0A", &buftext));
  netwib_er(netwib_buf_ref_string(&buftext, &text));
  netwib_er(netwib_icmp6_show_test2(&icmp6, NETWIB_ENCODETYPE_ARRAY, text));

  netwib_er(netwib_buf_close(&buf));

  /**/
  netwib_er(netwib_buf_init_ext_string("0101010203040506 0304018200000003000000040000000500000001000000020000000300000004 0501000100000002", &bufmixed));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_decode(&bufmixed, NETWIB_DECODETYPE_MIXED, &buf));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_ROUTERSOLICIT, &icmp6));
  icmp6.msg.routersolicit.reserved = 1;
  icmp6.msg.routersolicit.options = buf;
  netwib_er(netwib_icmp6_show_test2(&icmp6, NETWIB_ENCODETYPE_SYNTH,
                                    "'icmp6:133:0'"));
  netwib_er(netwib_icmp6_show_test2(&icmp6, NETWIB_ENCODETYPE_ARRAY, ""));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_ROUTERADVERT, &icmp6));
  icmp6.msg.routeradvert.curhoplimit = 1;
  icmp6.msg.routeradvert.managedaddress = NETWIB_TRUE;
  icmp6.msg.routeradvert.otherstateful = NETWIB_TRUE;
  icmp6.msg.routeradvert.reserved = 2;
  icmp6.msg.routeradvert.routerlifetime = 3;
  icmp6.msg.routeradvert.reachabletime = 4;
  icmp6.msg.routeradvert.retranstimer = 5;
  icmp6.msg.routeradvert.options = buf;
  netwib_er(netwib_icmp6_show_test2(&icmp6, NETWIB_ENCODETYPE_SYNTH,
                                    "'icmp6:134:0'"));
  netwib_er(netwib_icmp6_show_test2(&icmp6, NETWIB_ENCODETYPE_ARRAY, ""));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_NEIGHBORSOLICIT,
                                     &icmp6));
  icmp6.msg.neighborsolicit.reserved = 5;
  netwib_er(netwib_ip_init_ip6_fields(1,2,3,4,
                                      &icmp6.msg.neighborsolicit.target));
  icmp6.msg.neighborsolicit.options = buf;
  netwib_er(netwib_icmp6_show_test2(&icmp6, NETWIB_ENCODETYPE_SYNTH,
                                    "'icmp6:135:0'"));
  netwib_er(netwib_icmp6_show_test2(&icmp6, NETWIB_ENCODETYPE_ARRAY, ""));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_NEIGHBORADVERT, &icmp6));
  icmp6.msg.neighboradvert.router = NETWIB_TRUE;
  icmp6.msg.neighboradvert.solicited = NETWIB_TRUE;
  icmp6.msg.neighboradvert.override = NETWIB_TRUE;
  icmp6.msg.neighboradvert.reserved = 5;
  netwib_er(netwib_ip_init_ip6_fields(1,2,3,4,
                                      &icmp6.msg.neighboradvert.target));
  icmp6.msg.neighboradvert.options = buf;
  netwib_er(netwib_icmp6_show_test2(&icmp6, NETWIB_ENCODETYPE_SYNTH,
                                    "'icmp6:136:0'"));
  netwib_er(netwib_icmp6_show_test2(&icmp6, NETWIB_ENCODETYPE_ARRAY, ""));

  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_REDIRECT, &icmp6));
  icmp6.msg.redirect.reserved = 9;
  netwib_er(netwib_ip_init_ip6_fields(1,2,3,4,
                                      &icmp6.msg.redirect.target));
  netwib_er(netwib_ip_init_ip6_fields(5,6,7,8,
                                      &icmp6.msg.redirect.dst));
  icmp6.msg.redirect.options = buf;
  netwib_er(netwib_icmp6_show_test2(&icmp6, NETWIB_ENCODETYPE_SYNTH,
                                    "'icmp6:137:0'"));
  netwib_er(netwib_icmp6_show_test2(&icmp6, NETWIB_ENCODETYPE_ARRAY, ""));

  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_close(&buftext));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_shw_icmp6(void)
{
  puts("###########################################\nTest of shw/icmp6");
  netwib_er(netwib_icmp6_show_test());

  return(NETWIB_ERR_OK);
}
