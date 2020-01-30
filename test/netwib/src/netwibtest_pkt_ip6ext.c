#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_ip6ext_initdefault_test(void)
{
  netwib_ip6ext ip6ext;

  puts("++++----\nTest of netwib_ip6ext_initdefault");

  netwib_er(netwib_ip6ext_initdefault(NETWIB_IPPROTO_HOPOPTS, &ip6ext));
  netwib_er(netwibtest_ip6ext_content(&ip6ext, "3B01010400000000"));

  netwib_er(netwib_ip6ext_initdefault(NETWIB_IPPROTO_DSTOPTS, &ip6ext));
  netwib_er(netwibtest_ip6ext_content(&ip6ext, "3B01010400000000"));

  netwib_er(netwib_ip6ext_initdefault(NETWIB_IPPROTO_ROUTING, &ip6ext));
  netwib_er(netwibtest_ip6ext_content(&ip6ext, "3B00000000000000"));

  netwib_er(netwib_ip6ext_initdefault(NETWIB_IPPROTO_FRAGMENT, &ip6ext));
  ip6ext.ext.fragment.id = 3;
  netwib_er(netwibtest_ip6ext_content(&ip6ext, "3B00000000000003"));

  netwib_er(netwib_ip6ext_initdefault(NETWIB_IPPROTO_AH, &ip6ext));
  netwib_er(netwibtest_ip6ext_content(&ip6ext, "3B0100000000000000000000"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_ip6ext_test2(netwib_ip6ext *pip6ext,
                                                 netwib_conststring mixed,
                                                 netwib_uint32 wantskipsize)
{
  netwib_buf buf, pkt;
  netwib_ip6ext ip6ext2;
  netwib_uint32 skipsize;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&pkt));

  netwib_er(netwib_ip6ext_show(pip6ext, NETWIB_ENCODETYPE_ARRAY, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));

  netwib_er(netwib_pkt_append_ip6ext(pip6ext, &pkt));
  netwib_er(netwibtest_buf_content3(&pkt, mixed));
  netwib_er(netwib_pkt_decode_ip6ext(pip6ext->proto, &pkt, &ip6ext2,
                                     &skipsize));
  if (skipsize != wantskipsize) {
    return(NETWIBTEST_ERR_4);
  }
  netwib__buf_reinit(&buf);
  netwib_er(netwib_ip6ext_show(&ip6ext2, NETWIB_ENCODETYPE_SYNTH, &buf));
  netwib_er(netwib_fmt_display("%{buf}\n", &buf));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_ip6ext_show(&ip6ext2, NETWIB_ENCODETYPE_ARRAY, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_ip6ext_show(&ip6ext2, NETWIB_ENCODETYPE_DUMP, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));

  netwib_er(netwib_buf_close(&pkt));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_pkt_append_ip6ext_test(void)
{
  netwib_ip6ext ip6ext;

  puts("++++----\nTest of netwib_pkt_append_ip6ext");

  ip6ext.proto = NETWIB_IPPROTO_HOPOPTS;
  netwib_er(netwib_buf_init_ext_string("hello", &ip6ext.ext.hopopts.options));
  ip6ext.nextproto = NETWIB_IPPROTO_UDP;
  netwib_er(netwib_pkt_append_ip6ext_test2(&ip6ext, "110168656C6C6F00", 8));

  ip6ext.proto = NETWIB_IPPROTO_DSTOPTS;
  netwib_er(netwib_buf_init_ext_string("hell", &ip6ext.ext.hopopts.options));
  ip6ext.nextproto = NETWIB_IPPROTO_UDP;
  netwib_er(netwib_pkt_append_ip6ext_test2(&ip6ext, "110168656C6C0100", 8));

  ip6ext.proto = NETWIB_IPPROTO_ROUTING;
  ip6ext.ext.routing.routingtype = 1;
  ip6ext.ext.routing.segmentsleft = 2;
  netwib_er(netwib_buf_init_ext_string("hello world!",
                                     &ip6ext.ext.routing.data));
  ip6ext.nextproto = NETWIB_IPPROTO_UDP;
  netwib_er(netwib_pkt_append_ip6ext_test2(&ip6ext, "1102010268656C6C6F20776F726C6421", 16));

  ip6ext.proto = NETWIB_IPPROTO_FRAGMENT;
  ip6ext.ext.fragment.fragmentoffset = 1;
  ip6ext.ext.fragment.reservedb1 = NETWIB_TRUE;
  ip6ext.ext.fragment.reservedb2 = NETWIB_FALSE;
  ip6ext.ext.fragment.morefrag = NETWIB_TRUE;
  ip6ext.ext.fragment.id = 123;
  ip6ext.nextproto = NETWIB_IPPROTO_UDP;
  netwib_er(netwib_pkt_append_ip6ext_test2(&ip6ext, "1100000D0000007B", 8));

  ip6ext.proto = NETWIB_IPPROTO_AH;
  ip6ext.ext.ah.reserved = 1;
  ip6ext.ext.ah.spi = 2;
  ip6ext.ext.ah.seqnum = 3;
  netwib_er(netwib_buf_init_ext_string("hello world!", &ip6ext.ext.ah.data));
  ip6ext.nextproto = NETWIB_IPPROTO_UDP;
  netwib_er(netwib_pkt_append_ip6ext_test2(&ip6ext, "11040001000000020000000368656C6C6F20776F726C6421", 24));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_prepend_ip6ext_test(void)
{
  puts("++++----\nTest of netwib_pkt_prepend_ip6ext");
  puts("Not done");

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_ip6ext_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_ip6ext");
  puts("Redundant with netwib_pkt_append_ip6ext");

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_pkt_ip6ext(void)
{
  puts("###########################################\nTest of pkt/ip6ext");
  netwib_er(netwib_ip6ext_initdefault_test());
  netwib_er(netwib_pkt_append_ip6ext_test());
  netwib_er(netwib_pkt_prepend_ip6ext_test());
  netwib_er(netwib_pkt_decode_ip6ext_test());

  return(NETWIB_ERR_OK);
}
