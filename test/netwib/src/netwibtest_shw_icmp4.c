#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_icmp4_show_test2(netwib_consticmp4 *picmp4,
                                          netwib_encodetype encodetype,
                                          netwib_conststring wantmixed)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_icmp4_show(picmp4, encodetype, &buf));
  netwib_er(netwib_fmt_display("%{buf}\n", &buf));
  if (netwib_c_strlen(wantmixed)) {
    netwib_er(netwibtest_buf_content3(&buf, wantmixed));
  }

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_icmp4_show_test(void)
{
  netwib_buf ippktmixed, ippkt;
  netwib_icmp4 icmp4;
  netwib_buf buftext;
  netwib_string text;

  puts("++++----\nTest of netwib_icmp4_show");
  netwib_er(netwib_buf_init_mallocdefault(&buftext));

  netwib_er(netwib_buf_init_ext_string("45120021FADE000080112E871122334455667788 00010002000DAAAA 'hello'", &ippktmixed));
  netwib_er(netwib_buf_init_mallocdefault(&ippkt));
  netwib_er(netwib_buf_decode(&ippktmixed, NETWIB_DECODETYPE_MIXED, &ippkt));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_ECHOREP, &icmp4));
  netwib_er(netwib_buf_init_ext_string("1234", &icmp4.msg.echo.data));
  netwib_er(netwib_icmp4_show_test2(&icmp4, NETWIB_ENCODETYPE_SYNTH,
                                    "'icmp4:0:0'"));
  netwib_er(netwib_buf_append_string("'ICMP4_echo reply________________________________________________.' 0A '|     type      |     code      |           checksum            |' 0A '|____0x00=0_____|____0x00=0_____|___________0x0000=0____________|' 0A '|              id          ", &buftext));
  netwib_er(netwib_buf_append_string("     |            seqnum             |' 0A '|___________0x0000=0____________|___________0x0000=0____________|' 0A '| data: 31323334                                                |' 0A '|_______________________________________________________________|' 0A", &buftext));
  netwib_er(netwib_buf_ref_string(&buftext, &text));
  netwib_er(netwib_icmp4_show_test2(&icmp4, NETWIB_ENCODETYPE_ARRAY, text));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_DSTUNREACH, &icmp4));
  icmp4.msg.dstunreach.badippacket = ippkt;
  netwib_er(netwib_icmp4_show_test2(&icmp4, NETWIB_ENCODETYPE_SYNTH,
                                    "'icmp4:3:1'"));
  netwib_er(netwib_icmp4_show_test2(&icmp4, NETWIB_ENCODETYPE_ARRAY, ""));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_SRCQUENCH, &icmp4));
  icmp4.msg.srcquench.badippacket = ippkt;
  netwib_er(netwib_icmp4_show_test2(&icmp4, NETWIB_ENCODETYPE_SYNTH,
                                    "'icmp4:4:0'"));
  netwib_er(netwib_icmp4_show_test2(&icmp4, NETWIB_ENCODETYPE_ARRAY, ""));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_REDIRECT, &icmp4));
  icmp4.msg.redirect.badippacket = ippkt;
  netwib_er(netwib_icmp4_show_test2(&icmp4, NETWIB_ENCODETYPE_SYNTH,
                                    "'icmp4:5:1'"));
  netwib_er(netwib_icmp4_show_test2(&icmp4, NETWIB_ENCODETYPE_ARRAY, ""));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_ECHOREQ, &icmp4));
  icmp4.msg.echo.id = 1;
  icmp4.msg.echo.seqnum = 2;
  netwib_er(netwib_buf_init_ext_string("1234", &icmp4.msg.echo.data));
  netwib_er(netwib_icmp4_show_test2(&icmp4, NETWIB_ENCODETYPE_SYNTH,
                                    "'icmp4:8:0'"));
  netwib__buf_reinit(&buftext);
  netwib_er(netwib_buf_append_string("'ICMP4_echo request______________________________________________.' 0A '|     type      |     code      |           checksum            |' 0A '|____0x08=8_____", &buftext));
  netwib_er(netwib_buf_append_string("|____0x00=0_____|___________0x0000=0____________|' 0A '|              id               |            seqnum             |' 0A '|___________0x0001=1____________|___________0x0002=2____________|' 0A '| data: 31323334                                                |' 0A '|_______________________________________________________________|' 0A", &buftext));
  netwib_er(netwib_buf_ref_string(&buftext, &text));
  netwib_er(netwib_icmp4_show_test2(&icmp4, NETWIB_ENCODETYPE_ARRAY, text));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_TIMEEXCEED, &icmp4));
  icmp4.msg.timeexceed.badippacket = ippkt;
  netwib_er(netwib_icmp4_show_test2(&icmp4, NETWIB_ENCODETYPE_SYNTH,
                                    "'icmp4:11:0'"));
  netwib_er(netwib_icmp4_show_test2(&icmp4, NETWIB_ENCODETYPE_ARRAY, ""));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_PARAPROB, &icmp4));
  icmp4.msg.paraprob.badippacket = ippkt;
  netwib_er(netwib_icmp4_show_test2(&icmp4, NETWIB_ENCODETYPE_SYNTH,
                                    "'icmp4:12:0'"));
  netwib_er(netwib_icmp4_show_test2(&icmp4, NETWIB_ENCODETYPE_ARRAY, ""));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_TIMESTAMPREQ, &icmp4));
  icmp4.msg.timestamp.id = 1;
  icmp4.msg.timestamp.seqnum = 2;
  netwib_er(netwib_icmp4_show_test2(&icmp4, NETWIB_ENCODETYPE_SYNTH,
                                    "'icmp4:13:0'"));
  netwib__buf_reinit(&buftext);
  netwib_er(netwib_buf_append_string("'ICMP4_timestamp request_________________________________________.' 0A '|     type      |     code      |           checksum            |' 0A '|____0x0D=13____|____0x00=0_____|___________0x0000=0____________|' 0A '|              id               |            seqnum             |' 0A '|___________0x0001=1____________", &buftext));
  netwib_er(netwib_buf_append_string("|___________0x0002=2____________|' 0A '|                      originate timestamp                      |' 0A '|_______________________________0_______________________________|' 0A '|                       receive timestamp                       |' 0A '|_______________________________0_______________________________|' 0A '|                      transmit timestamp                       |' 0A '|_______________________________0_______________________________|' 0A", &buftext));
  netwib_er(netwib_buf_ref_string(&buftext, &text));
  netwib_er(netwib_icmp4_show_test2(&icmp4, NETWIB_ENCODETYPE_ARRAY, text));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_TIMESTAMPREP, &icmp4));
  icmp4.msg.timestamp.id = 1;
  icmp4.msg.timestamp.seqnum = 2;
  netwib_er(netwib_icmp4_show_test2(&icmp4, NETWIB_ENCODETYPE_SYNTH,
                                    "'icmp4:14:0'"));
  netwib__buf_reinit(&buftext);
  netwib_er(netwib_buf_append_string("'ICMP4_timestamp reply___________________________________________.' 0A '|     type      |     code      |           checksum            |' 0A '|____0x0E=14____|____0x00=0_____|___________0x0000=0____________|' 0A '|              id               |            seqnum             |' 0A '|___________0x0001=1________", &buftext));
  netwib_er(netwib_buf_append_string("____|___________0x0002=2____________|' 0A '|                      originate timestamp                      |' 0A '|_______________________________0_______________________________|' 0A '|                       receive timestamp                       |' 0A '|_______________________________0_______________________________|' 0A '|                      transmit timestamp                       |' 0A '|_______________________________0_______________________________|' 0A", &buftext));
  netwib_er(netwib_buf_ref_string(&buftext, &text));
  netwib_er(netwib_icmp4_show_test2(&icmp4, NETWIB_ENCODETYPE_ARRAY, text));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_INFOREQ, &icmp4));
  icmp4.msg.info.id = 1;
  icmp4.msg.info.seqnum = 2;
  netwib_er(netwib_buf_init_ext_string("1234", &icmp4.msg.info.data));
  netwib_er(netwib_icmp4_show_test2(&icmp4, NETWIB_ENCODETYPE_SYNTH,
                                    "'icmp4:15:0'"));
  netwib__buf_reinit(&buftext);
  netwib_er(netwib_buf_append_string("'ICMP4_information request_______________________________________.' 0A '|     type      |     code      |           checksum            |' 0A '|____0x0F=15____|_", &buftext));
  netwib_er(netwib_buf_append_string("___0x00=0_____|___________0x0000=0____________|' 0A '|              id               |            seqnum             |' 0A '|___________0x0001=1____________|___________0x0002=2____________|' 0A '| data: 31323334                                                |' 0A '|_______________________________________________________________|' 0A", &buftext));
  netwib_er(netwib_buf_ref_string(&buftext, &text));
  netwib_er(netwib_icmp4_show_test2(&icmp4, NETWIB_ENCODETYPE_ARRAY, text));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_INFOREP, &icmp4));
  netwib_er(netwib_buf_init_ext_string("1234", &icmp4.msg.info.data));
  netwib_er(netwib_icmp4_show_test2(&icmp4, NETWIB_ENCODETYPE_SYNTH,
                                    "'icmp4:16:0'"));
  netwib__buf_reinit(&buftext);
  netwib_er(netwib_buf_append_string("'ICMP4_information reply_________________________________________.' 0A '|     type      |     code      |           checksum            |' 0A '|____0x10=16____|", &buftext));
  netwib_er(netwib_buf_append_string("____0x00=0_____|___________0x0000=0____________|' 0A '|              id               |            seqnum             |' 0A '|___________0x0000=0____________|___________0x0000=0____________|' 0A '| data: 31323334                                                |' 0A '|_______________________________________________________________|' 0A", &buftext));
  netwib_er(netwib_buf_ref_string(&buftext, &text));
  netwib_er(netwib_icmp4_show_test2(&icmp4, NETWIB_ENCODETYPE_ARRAY, text));

  netwib_er(netwib_buf_close(&ippkt));
  netwib_er(netwib_buf_close(&buftext));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_shw_icmp4(void)
{
  puts("###########################################\nTest of shw/icmp4");
  netwib_er(netwib_icmp4_show_test());

  return(NETWIB_ERR_OK);
}
