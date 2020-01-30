#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_eth_init_fields_test(void)
{
  netwib_eth eth;

  puts("++++----\nTest of netwib_eth_init_fields");
  netwib_er(netwib_eth_init_fields(0, 0, 0, 0, 0, 0, NULL));

  netwib_er(netwib_eth_init_fields(1, 2, 3, 4, 5, 6, &eth));
  netwib_er(netwibtest_eth_content(&eth, "01:02:03:04:05:06"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_eth_init_buf_test2(netwib_conststring eths,
                                            netwib_err wantedret,
                                            netwib_byte wanteda,
                                            netwib_byte wantedb,
                                            netwib_byte wantedc,
                                            netwib_byte wantedd,
                                            netwib_byte wantede,
                                            netwib_byte wantedf)
{
  netwib_buf buf;
  netwib_eth eth;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string(eths, &buf));
  ret = netwib_eth_init_buf(&buf, &eth);
  if (ret != wantedret) {
    netwib_er(netwib_fmt_display("For %s :\n", eths));
    netwib_er(netwib_fmt_display("ret=%{uint32} != ", ret));
    netwib_er(netwib_fmt_display("wantedret=%{uint32}\n", wantedret));
    return(NETWIBTEST_ERR_6);
  }
  if (ret == NETWIB_ERR_OK) {
    if (eth.b[0] != wanteda || eth.b[1] != wantedb ||
        eth.b[2] != wantedc || eth.b[3] != wantedd ||
        eth.b[4] != wantede || eth.b[5] != wantedf ) {
      netwib_er(netwib_fmt_display("Eth=%{uint32:02X}:%{uint32:02X}:%{uint32:02X}:%{uint32:02X}:%{uint32:02X}:%{uint32:02X} != ", eth.b[0], eth.b[1], eth.b[2], eth.b[3], eth.b[4], eth.b[5]));
      netwib_er(netwib_fmt_display("wantedEth=%{uint32:02X}:%{uint32:02X}:%{uint32:02X}:%{uint32:02X}:%{uint32:02X}:%{uint32:02X}\n", wanteda, wantedb, wantedc, wantedd, wantede, wantedf));
      return(NETWIBTEST_ERR_7);
    }
  }

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_eth_init_buf_test(void)
{
  netwib_eth eth;

  puts("++++----\nTest of netwib_eth_init_buf");
  netwib_er(netwib_eth_init_buf_test2("01:23:45:67:89:aB", NETWIB_ERR_OK,
                                      0x01, 0x23, 0x45, 0x67, 0x89, 0xab));
  netwib_er(netwib_eth_init_buf_test2("01:23:00:FF:89:aB", NETWIB_ERR_OK,
                                      0x01, 0x23, 0x00, 0xFF, 0x89, 0xab));
  netwib_er(netwib_eth_init_buf_test2(" 01:23:45:67:89:aB  \t ", NETWIB_ERR_OK,
                                      0x01, 0x23, 0x45, 0x67, 0x89, 0xab));
  netwib_er(netwib_eth_init_buf_test2("1:23:0:FF:89:aB", NETWIB_ERR_OK,
                                      0x01, 0x23, 0x00, 0xFF, 0x89, 0xab));

  netwib_er(netwib_eth_init_buf_test2("01:23:45:67:g9:aB",
                                      NETWIB_ERR_NOTCONVERTED, 0,0,0,0,0,0));
  netwib_er(netwib_eth_init_buf_test2("01:23:45:67:69:aB:",
                                      NETWIB_ERR_NOTCONVERTED, 0,0,0,0,0,0));
  netwib_er(netwib_eth_init_buf_test2("01:23:45:67:69:",
                                      NETWIB_ERR_NOTCONVERTED, 0,0,0,0,0,0));
  netwib_er(netwib_eth_init_buf_test2("01:23:45:67:69",
                                      NETWIB_ERR_NOTCONVERTED, 0,0,0,0,0,0));

  /* now we test netwibtest_eth_content which uses this function */
  eth.b[0] = 0;eth.b[1] = 1;eth.b[2] =2;eth.b[3] = 3;eth.b[4] = 4;eth.b[5] = 5;
  netwib_er(netwibtest_eth_content(&eth, "00:01:02:03:04:05"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_eth_init_ip_test2(netwib_conststring ips,
                                    netwib_conststring wantedeths);
netwib_err netwib_eth_init_ip_test2(netwib_conststring ips,
                                    netwib_conststring wantedeths)
{
  netwib_buf buf;
  netwib_eth eth;
  netwib_ip ipa;

  netwib_er(netwib_buf_init_ext_string(ips, &buf));
  netwib_er(netwib_ip_init_buf_best(&buf, &ipa));
  netwib_er(netwib_eth_init_ip(&ipa, &eth));
  netwib_er(netwibtest_eth_content(&eth, wantedeths));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_eth_init_ip_test(void)
{
  puts("++++----\nTest of netwib_eth_init_ip");

#ifdef NETWIBTESTDEF_HOST_ETH_1
  if (NETWIBTESTDEF_HOST_IPTOETH_1) {
    netwib_er(netwib_eth_init_ip_test2(NETWIBTESTDEF_HOST_IP_1,
                                       NETWIBTESTDEF_HOST_ETH_1));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_ETH_2
  if (NETWIBTESTDEF_HOST_IPTOETH_2) {
    netwib_er(netwib_eth_init_ip_test2(NETWIBTESTDEF_HOST_IP_2,
                                       NETWIBTESTDEF_HOST_ETH_2));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_ETH_3
  if (NETWIBTESTDEF_HOST_IPTOETH_3) {
    netwib_er(netwib_eth_init_ip_test2(NETWIBTESTDEF_HOST_IP_3,
                                       NETWIBTESTDEF_HOST_ETH_3));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_ETH_4
  if (NETWIBTESTDEF_HOST_IPTOETH_4) {
    netwib_er(netwib_eth_init_ip_test2(NETWIBTESTDEF_HOST_IP_4,
                                       NETWIBTESTDEF_HOST_ETH_4));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_ETH_5
  if (NETWIBTESTDEF_HOST_IPTOETH_5) {
    netwib_er(netwib_eth_init_ip_test2(NETWIBTESTDEF_HOST_IP_5,
                                       NETWIBTESTDEF_HOST_ETH_5));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_ETH_6
  if (NETWIBTESTDEF_HOST_IPTOETH_6) {
    netwib_er(netwib_eth_init_ip_test2(NETWIBTESTDEF_HOST_IP_6,
                                       NETWIBTESTDEF_HOST_ETH_6));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_ETH_7
  if (NETWIBTESTDEF_HOST_IPTOETH_7) {
    netwib_er(netwib_eth_init_ip_test2(NETWIBTESTDEF_HOST_IP_7,
                                       NETWIBTESTDEF_HOST_ETH_7));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_ETH_8
  if (NETWIBTESTDEF_HOST_IPTOETH_8) {
    netwib_er(netwib_eth_init_ip_test2(NETWIBTESTDEF_HOST_IP_8,
                                       NETWIBTESTDEF_HOST_ETH_8));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_ETH_9
  if (NETWIBTESTDEF_HOST_IPTOETH_9) {
    netwib_er(netwib_eth_init_ip_test2(NETWIBTESTDEF_HOST_IP_9,
                                       NETWIBTESTDEF_HOST_ETH_9));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_ETH_10
  if (NETWIBTESTDEF_HOST_IPTOETH_10) {
    netwib_er(netwib_eth_init_ip_test2(NETWIBTESTDEF_HOST_IP_10,
                                       NETWIBTESTDEF_HOST_ETH_10));
  }
#endif

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
static netwib_err netwib_eth_init_kbd_test(void)
{
  netwib_buf buf;
  netwib_eth eth, ethdef;

  puts("++++----\nTest of netwib_eth_init_kbd");
  netwib_er(netwib_buf_init_ext_string("Enter \"sdf\" then \"11:22:33:44:55:66\"",
                                     &buf));
  netwib_er(netwib_eth_init_kbd(&buf, NETWIB_ETH_INIT_KBD_NODEF, &eth));
  netwib_er(netwibtest_eth_content(&eth, "11:22:33:44:55:66"));

  netwib_er(netwib_eth_init_fields(1, 2, 3, 4, 5, 6, &ethdef));
  netwib_er(netwib_buf_init_ext_string("Press 'Enter'", &buf));
  netwib_er(netwib_eth_init_kbd(&buf, &ethdef, &eth));
  netwib_er(netwibtest_eth_content(&eth, "01:02:03:04:05:06"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_eth_cmp_test(void)
{
  netwib_eth eth1, eth2;
  netwib_cmp cmp;

  puts("++++----\nTest of netwib_eth_cmp");

  netwib_er(netwib_eth_init_fields(1, 2, 3, 4, 5, 6, &eth1));
  netwib_er(netwib_eth_init_fields(1, 2, 3, 4, 4, 6, &eth2));
  netwib_er(netwib_eth_cmp(&eth1, &eth2, &cmp));
  if (cmp != NETWIB_CMP_GT) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_eth_init_fields(1, 2, 3, 4, 5, 6, &eth1));
  netwib_er(netwib_eth_init_fields(1, 2, 3, 4, 7, 6, &eth2));
  netwib_er(netwib_eth_cmp(&eth1, &eth2, &cmp));
  if (cmp != NETWIB_CMP_LT) {
    return(NETWIBTEST_ERR_7);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_eth_test2(netwib_conststring ineth,
                                              netwib_conststring wantedeth)
{
  netwib_buf buf;
  netwib_eth eth;

  netwib_er(netwib_buf_init_ext_string(ineth, &buf));
  netwib_er(netwib_eth_init_buf(&buf, &eth));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_eth(&eth, &buf));
  netwib_er(netwibtest_buf_content2(&buf, wantedeth));
  netwib_er(netwib_buf_close(&buf));


  return(NETWIB_ERR_OK);
}
static netwib_err netwib_buf_append_eth_test(void)
{
  puts("++++----\nTest of netwib_buf_append_eth");
  netwib_er(netwib_buf_append_eth_test2("01:02:03:04:05:06",
                                        "01:02:03:04:05:06"));
  netwib_er(netwib_buf_append_eth_test2("01:2:3:b4:0:FF",
                                        "01:02:03:B4:00:FF"));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_net_eth(void)
{
  puts("###########################################\nTest of net/eth");
  netwib_er(netwib_eth_init_fields_test());
  netwib_er(netwib_eth_init_buf_test());
  netwib_er(netwib_eth_init_ip_test());
  if (netwibtest_testkbdlevel == 2) {
    netwib_er(netwib_eth_init_kbd_test());
  }

  netwib_er(netwib_eth_cmp_test());

  netwib_er(netwib_buf_append_eth_test());

  return(NETWIB_ERR_OK);
}
