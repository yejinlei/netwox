#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_ip64bits_init_ippkt_test(void)
{
  netwib_buf ip64bits, ippkt, buf;

  puts("++++----\nTest of netwib_ip64bits_init_ippkt");
  netwib_er(netwib_buf_init_ext_string("46341234 12341234 12341234 12341234 12341234 abcdabcd 87654321 97654321 A7654321", &buf));
  netwib_er(netwib_buf_init_mallocdefault(&ippkt));
  netwib_er(netwib_buf_decode(&buf, NETWIB_DECODETYPE_HEXA, &ippkt));
  netwib_er(netwib_ip64bits_init_ippkt(&ippkt, &ip64bits));
  netwib_er(netwibtest_buf_content3(&ip64bits, "46341234 12341234 12341234 12341234 12341234 abcdabcd 87654321 97654321"));
  netwib_er(netwib_buf_close(&ippkt));

  netwib_er(netwib_buf_init_ext_string("681004D2000D11411122334400000005000000060000000755667788000000080000000900000000 00010002000DAA87 'hello'", &buf));
  netwib_er(netwib_buf_init_mallocdefault(&ippkt));
  netwib_er(netwib_buf_decode(&buf, NETWIB_DECODETYPE_MIXED, &ippkt));
  netwib_er(netwib_ip64bits_init_ippkt(&ippkt, &ip64bits));
  netwib_er(netwibtest_buf_content3(&ip64bits, "681004D2000D1141112233440000000500000006000000075566778800000008000000090000000000010002000DAA87"));
  netwib_er(netwib_buf_close(&ippkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ip64bits_decode_ipudp_test(void)
{
  netwib_buf ip64bits, buf;
  netwib_iphdr iphdr;
  netwib_port udpsrc, udpdst;

  puts("++++----\nTest of netwib_ip64bits_decode_ipudp");
  netwib_er(netwib_buf_init_ext_string("46341234 12341234 12111234 12341234 12341234 abcdabcd 87654321 97654321 A7654321", &buf));
  netwib_er(netwib_buf_init_mallocdefault(&ip64bits));
  netwib_er(netwib_buf_decode(&buf, NETWIB_DECODETYPE_HEXA, &ip64bits));

  netwib_er(netwib_ip64bits_decode_ipudp(&ip64bits, &iphdr, &udpsrc, &udpdst));

  netwib_er(netwibtest_iphdr_content(&iphdr, "46341234 12341234 12111234 12341234 12341234 abcdabcd"));
  if (udpsrc != 0x8765) {
    return(NETWIBTEST_ERR_4);
  }
  if (udpdst != 0x4321) {
    return(NETWIBTEST_ERR_4);
  }

  netwib_er(netwib_buf_close(&ip64bits));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ip64bits_decode_iptcp_test(void)
{
  netwib_buf ip64bits, buf;
  netwib_iphdr iphdr;
  netwib_port tcpsrc, tcpdst;

  puts("++++----\nTest of netwib_ip64bits_decode_iptcp");
  netwib_er(netwib_buf_init_ext_string("46341234 12341234 12061234 12341234 12341234 abcdabcd 87654321 97654321 A7654321", &buf));
  netwib_er(netwib_buf_init_mallocdefault(&ip64bits));
  netwib_er(netwib_buf_decode(&buf, NETWIB_DECODETYPE_HEXA, &ip64bits));

  netwib_er(netwib_ip64bits_decode_iptcp(&ip64bits, &iphdr, &tcpsrc, &tcpdst));

  netwib_er(netwibtest_iphdr_content(&iphdr, "46341234 12341234 12061234 12341234 12341234 abcdabcd"));
  if (tcpsrc != 0x8765) {
    return(NETWIBTEST_ERR_4);
  }
  if (tcpdst != 0x4321) {
    return(NETWIBTEST_ERR_4);
  }

  netwib_er(netwib_buf_close(&ip64bits));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_pkt_ip64bits(void)
{
  puts("###########################################\nTest of pkt/ip64bits");

  netwib_er(netwib_ip64bits_init_ippkt_test());
  netwib_er(netwib_ip64bits_decode_ipudp_test());
  netwib_er(netwib_ip64bits_decode_iptcp_test());

  return(NETWIB_ERR_OK);
}
