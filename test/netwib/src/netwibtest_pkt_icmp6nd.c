#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_icmp6nd_initdefault_test(void)
{
  netwib_icmp6nd icmp6nd;

  puts("++++----\nTest of netwib_icmp6nd_initdefault");

  netwib_er(netwib_icmp6nd_initdefault(NETWIB_ICMP6NDTYPE_SRCLINK, &icmp6nd));
  netwib_er(netwibtest_icmp6nd_content(&icmp6nd, "0101000000000000"));

  netwib_er(netwib_icmp6nd_initdefault(NETWIB_ICMP6NDTYPE_DSTLINK, &icmp6nd));
  netwib_er(netwibtest_icmp6nd_content(&icmp6nd, "0201000000000000"));

  netwib_er(netwib_icmp6nd_initdefault(NETWIB_ICMP6NDTYPE_PREFIX, &icmp6nd));
  netwib_er(netwibtest_icmp6nd_content(&icmp6nd, "0304000000000000000000000000000000000000000000000000000000000000"));

  netwib_er(netwib_icmp6nd_initdefault(NETWIB_ICMP6NDTYPE_REDIR, &icmp6nd));
  netwib_er(netwibtest_icmp6nd_content(&icmp6nd, "0401000000000000"));

  netwib_er(netwib_icmp6nd_initdefault(NETWIB_ICMP6NDTYPE_MTU, &icmp6nd));
  netwib_er(netwibtest_icmp6nd_content(&icmp6nd, "0501000000000000"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_icmp6nd_test2(netwib_icmp6nd *picmp6nd,
                                                  netwib_conststring mixed,
                                                  netwib_uint32 wantedskipsize)
{
  netwib_icmp6nd icmp6nd2;
  netwib_uint32 skipsize;
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_icmp6nd(picmp6nd, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, mixed));

  netwib_er(netwib_pkt_decode_icmp6nd(&buf, &icmp6nd2, &skipsize));
  if (skipsize != wantedskipsize) {
    return(NETWIBTEST_ERR_4);
  }
  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_icmp6nd(&icmp6nd2, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, mixed));

  netwib_er(netwib_buf_close(&buf));
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_pkt_append_icmp6nd_test(void)
{
  netwib_buf ippktmixed, ippkt;
  netwib_icmp6nd icmp6nd;

  puts("++++----\nTest of netwib_pkt_append_icmp6nd");

  netwib_er(netwib_buf_init_ext_string("45120021FADE000080112E871122334455667788 00010002000DAAAA 'hello'", &ippktmixed));
  netwib_er(netwib_buf_init_mallocdefault(&ippkt));
  netwib_er(netwib_buf_decode(&ippktmixed, NETWIB_DECODETYPE_MIXED, &ippkt));

  netwib_er(netwib_icmp6nd_initdefault(NETWIB_ICMP6NDTYPE_SRCLINK, &icmp6nd));
  netwib_er(netwib_eth_init_fields(1,2,3,4,5,6, &icmp6nd.opt.link.linkad));
  netwib_er(netwib_pkt_append_icmp6nd_test2(&icmp6nd, "0101010203040506", 8));

  netwib_er(netwib_icmp6nd_initdefault(NETWIB_ICMP6NDTYPE_DSTLINK, &icmp6nd));
  netwib_er(netwib_eth_init_fields(1,2,3,4,5,6, &icmp6nd.opt.link.linkad));
  netwib_er(netwib_pkt_append_icmp6nd_test2(&icmp6nd, "0201010203040506", 8));

  netwib_er(netwib_icmp6nd_initdefault(NETWIB_ICMP6NDTYPE_PREFIX, &icmp6nd));
  icmp6nd.opt.prefix.prefixlength = 1;
  icmp6nd.opt.prefix.onlink = NETWIB_TRUE;
  icmp6nd.opt.prefix.autonomous = NETWIB_FALSE;
  icmp6nd.opt.prefix.reserved1 = 2;
  icmp6nd.opt.prefix.validlifetime = 3;
  icmp6nd.opt.prefix.preferredlifetime = 4;
  icmp6nd.opt.prefix.reserved2 = 5;
  netwib_er(netwib_ip_init_ip6_fields(1,2,3,4, &icmp6nd.opt.prefix.prefix));
  netwib_er(netwib_pkt_append_icmp6nd_test2(&icmp6nd, "0304018200000003000000040000000500000001000000020000000300000004", 32));

  netwib_er(netwib_icmp6nd_initdefault(NETWIB_ICMP6NDTYPE_REDIR, &icmp6nd));
  icmp6nd.opt.redir.reserved1 = 1;
  icmp6nd.opt.redir.reserved2 = 2;
  icmp6nd.opt.redir.badippacket = ippkt;
  netwib_er(netwib_pkt_append_icmp6nd_test2(&icmp6nd, "040500010000000245120021FADE000080112E87112233445566778800010002000DAAAA68656C6C", 40));

  netwib_er(netwib_icmp6nd_initdefault(NETWIB_ICMP6NDTYPE_MTU, &icmp6nd));
  icmp6nd.opt.mtu.reserved = 1;
  icmp6nd.opt.mtu.mtu = 2;
  netwib_er(netwib_pkt_append_icmp6nd_test2(&icmp6nd, "0501000100000002", 8));

  netwib_er(netwib_buf_close(&ippkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_icmp6nd_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_icmp6nd");
  puts("Redundant with netwib_pkt_append_icmp6nd");
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_pkt_icmp6nd(void)
{
  puts("###########################################\nTest of pkt/icmp6nd");

  netwib_er(netwib_icmp6nd_initdefault_test());
  netwib_er(netwib_pkt_append_icmp6nd_test());
  netwib_er(netwib_pkt_decode_icmp6nd_test());

  return(NETWIB_ERR_OK);
}
