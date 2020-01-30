#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_iphdr_initdefault_test(void)
{
  netwib_iphdr iphdr;
  netwib_buf buf;

  puts("++++----\nTest of netwib_iphdr_initdefault");
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_iphdr_initdefault(NETWIB_IPTYPE_IP4, &iphdr));
  iphdr.header.ip4.id = 123;
  netwib_er(netwibtest_iphdr_content(&iphdr, "45000014007B0000800000000000000000000000"));
  netwib_er(netwib_iphdr_initdefault(NETWIB_IPTYPE_IP6, &iphdr));
  netwib_er(netwibtest_iphdr_content(&iphdr, "6000000000003B800000000000000000000000000000000000000000000000000000000000000000"));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_iphdr_test2(netwib_constiphdr *piphdr,
                                                netwib_conststring mixed,
                                                netwib_uint32 wantedskipsize)
{
  netwib_iphdr iphdr2;
  netwib_uint32 skipsize;
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_iphdr(piphdr, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_iphdr_content(piphdr, mixed));
  netwib_er(netwibtest_buf_content3(&buf, mixed));

  netwib_er(netwib_pkt_decode_iphdr(&buf, &iphdr2, &skipsize));
  if (skipsize != wantedskipsize) {
    return(NETWIBTEST_ERR_4);
  }
  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_prepend_iphdr(&iphdr2, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, mixed));

  netwib_er(netwib_buf_close(&buf));
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_pkt_append_iphdr_test(void)
{
  netwib_iphdr iphdr;
  netwib_ip6ext ip6ext;
  netwib_buf buf;

  puts("++++----\nTest of netwib_pkt_append_iphdr");

  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwib_pkt_append_iphdr_test2(&iphdr, "45120014  fade0000  80061234  11223344 55667788", NETWIB_PRIV_IP4HDR_MINLEN));
  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwib_buf_init_ext_string("abcd", &iphdr.header.ip4.opts));
  iphdr.header.ip4.ihl += 1;
  netwib_er(netwib_pkt_append_iphdr_test2(&iphdr, "46120014  fade0000  80061234  11223344 55667788 'abcd'", NETWIB_PRIV_IP4HDR_MINLEN+4));

  netwib_er(netwibtest_ip6hdr_init(&iphdr));
  netwib_er(netwib_pkt_append_iphdr_test2(&iphdr, "681004D2002206411122334400000005000000060000000755667788000000080000000900000000", NETWIB_PRIV_IP6HDR_MINLEN));

  netwib_er(netwibtest_ip6hdr_init(&iphdr));
  iphdr.protocol = NETWIB_IPPROTO_FRAGMENT;
  ip6ext.proto = NETWIB_IPPROTO_FRAGMENT;
  ip6ext.ext.fragment.fragmentoffset = 1;
  ip6ext.ext.fragment.reservedb1 = NETWIB_TRUE;
  ip6ext.ext.fragment.reservedb2 = NETWIB_FALSE;
  ip6ext.ext.fragment.morefrag = NETWIB_TRUE;
  ip6ext.ext.fragment.id = 3;
  ip6ext.nextproto = NETWIB_IPPROTO_FRAGMENT;
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_pkt_append_ip6ext(&ip6ext, &buf));
  netwib_er(netwib_buf_init_ext_buf(&buf, &iphdr.header.ip6.exts));
  netwib_er(netwib_pkt_append_iphdr_test2(&iphdr, "681004D200222C4111223344000000050000000600000007556677880000000800000009000000002C00000D00000003", NETWIB_PRIV_IP6HDR_MINLEN+8));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_prepend_iphdr_test(void)
{
  puts("++++----\nTest of netwib_pkt_prepend_iphdr");
  puts("Redundant with netwib_pkt_append_iphdr");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_iphdr_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_iphdr");
  puts("Redundant with netwib_pkt_append_iphdr");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_iphdr_get_proto_test(void)
{
  netwib_iphdr iphdr;
  netwib_ipproto ipproto;
  netwib_ip6ext ip6ext;
  netwib_buf buf;

  puts("++++----\nTest of netwib_iphdr_get_proto");

  netwib_er(netwibtest_ip4hdr_init(&iphdr));
  netwib_er(netwib_iphdr_set_proto(&iphdr, NETWIB_IPPROTO_TCP));
  netwib_er(netwib_iphdr_get_proto(&iphdr, &ipproto));
  if (ipproto != NETWIB_IPPROTO_TCP) {
    return(NETWIBTEST_ERR_5);
  }

  netwib_er(netwibtest_ip6hdr_init(&iphdr));
  netwib_er(netwib_iphdr_set_proto(&iphdr, NETWIB_IPPROTO_TCP));
  netwib_er(netwib_iphdr_get_proto(&iphdr, &ipproto));
  if (ipproto != NETWIB_IPPROTO_TCP) {
    return(NETWIBTEST_ERR_5);
  }

  netwib_er(netwibtest_ip6hdr_init(&iphdr));
  iphdr.protocol = NETWIB_IPPROTO_FRAGMENT;
  ip6ext.proto = NETWIB_IPPROTO_FRAGMENT;
  ip6ext.ext.fragment.fragmentoffset = 1;
  ip6ext.ext.fragment.reservedb1 = NETWIB_TRUE;
  ip6ext.ext.fragment.reservedb2 = NETWIB_FALSE;
  ip6ext.ext.fragment.morefrag = NETWIB_TRUE;
  ip6ext.ext.fragment.id = 3;
  ip6ext.nextproto = NETWIB_IPPROTO_UDP;
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_pkt_append_ip6ext(&ip6ext, &buf));
  netwib_er(netwib_buf_init_ext_buf(&buf, &iphdr.header.ip6.exts));
  netwib_er(netwib_iphdr_set_proto(&iphdr, NETWIB_IPPROTO_TCP));
  netwib_er(netwib_iphdr_get_proto(&iphdr, &ipproto));
  if (ipproto != NETWIB_IPPROTO_TCP) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_iphdr_set_proto_test(void)
{
  puts("++++----\nTest of netwib_iphdr_set_proto");
  puts("Redundant with netwib_iphdr_get_proto");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_pkt_iphdr(void)
{
  puts("###########################################\nTest of pkt/iphdr");
  netwib_er(netwib_iphdr_initdefault_test());
  netwib_er(netwib_pkt_append_iphdr_test());
  netwib_er(netwib_pkt_prepend_iphdr_test());
  netwib_er(netwib_pkt_decode_iphdr_test());

  netwib_er(netwib_iphdr_get_proto_test());
  netwib_er(netwib_iphdr_set_proto_test());

  return(NETWIB_ERR_OK);
}
