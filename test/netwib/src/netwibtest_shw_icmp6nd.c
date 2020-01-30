#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_icmp6nd_show_test2(netwib_consticmp6nd *picmp6nd,
                                            netwib_encodetype encodetype,
                                            netwib_conststring wantmixed)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_icmp6nd_show(picmp6nd, encodetype, &buf));
  netwib_er(netwib_fmt_display("%{buf}\n", &buf));
  if (netwib_c_strlen(wantmixed)) {
    netwib_er(netwibtest_buf_content3(&buf, wantmixed));
  }

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_icmp6nd_show_test(void)
{
  netwib_buf ippktmixed, ippkt;
  netwib_icmp6nd icmp6nd;
  netwib_buf buftext;
  netwib_string text;

  puts("++++----\nTest of netwib_icmp6nd_show");
  netwib_er(netwib_buf_init_mallocdefault(&buftext));

  netwib_er(netwib_buf_init_ext_string("45120021FADE000080112E871122334455667788 00010002000DAAAA 'hello'", &ippktmixed));
  netwib_er(netwib_buf_init_mallocdefault(&ippkt));
  netwib_er(netwib_buf_decode(&ippktmixed, NETWIB_DECODETYPE_MIXED, &ippkt));

  netwib_er(netwib_icmp6nd_initdefault(NETWIB_ICMP6NDTYPE_SRCLINK, &icmp6nd));
  netwib_er(netwib_eth_init_fields(1,2,3,4,5,6, &icmp6nd.opt.link.linkad));
  netwib_er(netwib_icmp6nd_show_test2(&icmp6nd, NETWIB_ENCODETYPE_SYNTH,
                                      "'icmp6nd:1'"));
  netwib__buf_reinit(&buftext);
  netwib_er(netwib_buf_append_string("'| src link : linkad=01:02:03:04:05:06                           |' 0A", &buftext));
  netwib_er(netwib_buf_ref_string(&buftext, &text));
  netwib_er(netwib_icmp6nd_show_test2(&icmp6nd, NETWIB_ENCODETYPE_ARRAY,
                                      text));

  netwib_er(netwib_icmp6nd_initdefault(NETWIB_ICMP6NDTYPE_DSTLINK, &icmp6nd));
  netwib_er(netwib_eth_init_fields(1,2,3,4,5,6, &icmp6nd.opt.link.linkad));
  netwib_er(netwib_icmp6nd_show_test2(&icmp6nd, NETWIB_ENCODETYPE_SYNTH,
                                      "'icmp6nd:2'"));
  netwib__buf_reinit(&buftext);
  netwib_er(netwib_buf_append_string("'| dst link : linkad=01:02:03:04:05:06                           |' 0A", &buftext));
  netwib_er(netwib_buf_ref_string(&buftext, &text));
  netwib_er(netwib_icmp6nd_show_test2(&icmp6nd, NETWIB_ENCODETYPE_ARRAY,
                                      text));

  netwib_er(netwib_icmp6nd_initdefault(NETWIB_ICMP6NDTYPE_PREFIX, &icmp6nd));
  icmp6nd.opt.prefix.prefixlength = 1;
  icmp6nd.opt.prefix.onlink = NETWIB_TRUE;
  icmp6nd.opt.prefix.autonomous = NETWIB_FALSE;
  icmp6nd.opt.prefix.reserved1 = 2;
  icmp6nd.opt.prefix.validlifetime = 3;
  icmp6nd.opt.prefix.preferredlifetime = 4;
  icmp6nd.opt.prefix.reserved2 = 5;
  netwib_er(netwib_ip_init_ip6_fields(1,2,3,4, &icmp6nd.opt.prefix.prefix));
  netwib_er(netwib_icmp6nd_show_test2(&icmp6nd, NETWIB_ENCODETYPE_SYNTH,
                                      "'icmp6nd:3'"));
  netwib__buf_reinit(&buftext);
  netwib_er(netwib_buf_append_string("'| prefix : prefixlength=1 onlink=true autonomous=false          |' 0A '|          reserved1=2 reserved2=5                              |' 0A '|          validlifetime=3 preferredlifetime=4                  |' 0A '|          prefix=0:1:0:2:0:3:0:4                               |' 0A", &buftext));
  netwib_er(netwib_buf_ref_string(&buftext, &text));
  netwib_er(netwib_icmp6nd_show_test2(&icmp6nd, NETWIB_ENCODETYPE_ARRAY,
                                      text));

  netwib_er(netwib_icmp6nd_initdefault(NETWIB_ICMP6NDTYPE_REDIR, &icmp6nd));
  icmp6nd.opt.redir.reserved1 = 1;
  icmp6nd.opt.redir.reserved2 = 2;
  icmp6nd.opt.redir.badippacket = ippkt;
  netwib_er(netwib_icmp6nd_show_test2(&icmp6nd, NETWIB_ENCODETYPE_SYNTH,
                                      "'icmp6nd:4'"));
  netwib__buf_reinit(&buftext);
  netwib_er(netwib_buf_append_string("", &buftext));
  netwib_er(netwib_buf_ref_string(&buftext, &text));
  netwib_er(netwib_icmp6nd_show_test2(&icmp6nd, NETWIB_ENCODETYPE_ARRAY,
                                      text));

  netwib_er(netwib_icmp6nd_initdefault(NETWIB_ICMP6NDTYPE_MTU, &icmp6nd));
  icmp6nd.opt.mtu.reserved = 1;
  icmp6nd.opt.mtu.mtu = 2;
  netwib_er(netwib_icmp6nd_show_test2(&icmp6nd, NETWIB_ENCODETYPE_SYNTH,
                                      "'icmp6nd:5'"));
  netwib__buf_reinit(&buftext);
  netwib_er(netwib_buf_append_string("'| mtu : reserved=1 mtu=2                                        |' 0A", &buftext));
  netwib_er(netwib_buf_ref_string(&buftext, &text));
  netwib_er(netwib_icmp6nd_show_test2(&icmp6nd, NETWIB_ENCODETYPE_ARRAY,
                                      text));

  netwib_er(netwib_buf_close(&ippkt));
  netwib_er(netwib_buf_close(&buftext));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_icmp6nds_show_test(void)
{
  netwib_buf pkt, buf, pkthexa, buftext;
  netwib_string text;

  puts("++++----\nTest of netwib_icmp6nds_show");

  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_buf_init_ext_string("0101010203040506 0304018200000003000000040000000500000001000000020000000300000004 0501000100000002", &pkthexa));
  netwib_er(netwib_buf_decode(&pkthexa, NETWIB_DECODETYPE_HEXA, &pkt));

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_icmp6nds_show(&pkt, NETWIB_ENCODETYPE_SYNTH, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_fmt_display("\n"));
  netwib_er(netwibtest_buf_content3(&buf, "'icmp6nds'"));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_icmp6nds_show(&pkt, NETWIB_ENCODETYPE_ARRAY, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_buf_init_mallocdefault(&buftext));
  netwib_er(netwib_buf_append_string("'ICMP6NDS________________________________________________________.' 0A '| src link : linkad=01:02:03:04:05:06                           |' 0A '| prefix : prefixlength=1 onlink=true autonomous=false          |' 0A '|          reserved1=2 res", &buftext));
  netwib_er(netwib_buf_append_string("erved2=5                              |' 0A '|          validlifetime=3 preferredlifetime=4                  |' 0A '|          prefix=0:1:0:2:0:3:0:4                               |' 0A '| mtu : reserved=1 mtu=2                                        |' 0A '|_______________________________________________________________|' 0A", &buftext));
  netwib_er(netwib_buf_ref_string(&buftext, &text));
  netwib_er(netwibtest_buf_content3(&buf, text));
  netwib_er(netwib_buf_close(&buftext));

  netwib_er(netwib_buf_close(&pkt));
  netwib_er(netwib_buf_close(&buf));
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_shw_icmp6nd(void)
{
  puts("###########################################\nTest of shw/icmp6nd");
  netwib_er(netwib_icmp6nd_show_test());
  netwib_er(netwib_icmp6nds_show_test());

  return(NETWIB_ERR_OK);
}
