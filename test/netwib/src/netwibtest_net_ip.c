#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_ip_init_ip4_fields_test(void)
{
  netwib_ip ipa;

  puts("++++----\nTest of netwib_ip_init_ip4_fields");
  netwib_er(netwib_ip_init_ip4_fields(1, 2, 3, 4, &ipa));
  netwib_er(netwibtest_ip_content(&ipa, "1.2.3.4"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ip_init_ip4_test(void)
{
  netwib_ip ipa;

  puts("++++----\nTest of netwib_ip_init_ip4");
  netwib_er(netwib_ip_init_ip4(0x01020304, &ipa));
  netwib_er(netwibtest_ip_content(&ipa, "1.2.3.4"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ip4_init_ip_test(void)
{
  netwib_ip ipa;
  netwib_ip4 ip4;

  puts("++++----\nTest of netwib_ip4_init_ip");
  netwib_er(netwib_ip_init_ip4_fields(1, 2, 3, 4, &ipa));
  netwib_er(netwib_ip4_init_ip(&ipa, &ip4));
  if (ip4 != 0x01020304) {
    return(NETWIBTEST_ERR_6);
  }
  netwib_er(netwib_ip_init_ip6_fields(0, 0, 0xFFFF, 0x01020304, &ipa));
  netwib_er(netwib_ip4_init_ip(&ipa, &ip4));
  if (ip4 != 0x01020304) {
    return(NETWIBTEST_ERR_6);
  }

  netwib_er(netwib_ip_init_ip6_fields(1, 2, 3, 4, &ipa));
  netwib_eir(netwib_ip4_init_ip(&ipa, &ip4), NETWIB_ERR_NOTCONVERTED);
  netwib_er(netwib_ip_init_ip6_fields(0, 0, 3, 4, &ipa));
  netwib_eir(netwib_ip4_init_ip(&ipa, &ip4), NETWIB_ERR_NOTCONVERTED);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ip_init_ip6_fields_test(void)
{
  netwib_ip ipa;

  puts("++++----\nTest of netwib_ip_init_ip6_fields");
  netwib_er(netwib_ip_init_ip6_fields(1, 2, 3, 4, &ipa));
  netwib_er(netwibtest_ip_content(&ipa, "0:1:0:2:0:3:0:4"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ip_init_ip6_test(void)
{
  netwib_ip6 ip6;
  netwib_ip ipa;

  puts("++++----\nTest of netwib_ip_init_ip6");
  memset(ip6.b, 0, NETWIB_IP6_LEN);
  ip6.b[5] = 0x67;
  netwib_er(netwib_ip_init_ip6(&ip6, &ipa));
  netwib_er(netwibtest_ip_content(&ipa, "0:0:67:0:0:0:0:0"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ip6_init_ip_test(void)
{
  netwib_ip6 ip6;
  netwib_ip ipa;

  puts("++++----\nTest of netwib_ip6_init_ip");
  netwib_er(netwib_ip_init_ip6_fields(1, 2, 3, 4, &ipa));
  netwib_er(netwib_ip6_init_ip(&ipa, &ip6));
  netwib_er(netwib_ip_init_ip6(&ip6, &ipa));
  netwib_er(netwibtest_ip_content(&ipa, "0:1:0:2:0:3:0:4"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ip_init_buf_test2(netwib_conststring ips,
                                           netwib_err wantedret,
                                           netwib_constip *pwantedip)
{
  netwib_buf buf;
  netwib_ip ipa;
  netwib_cmp cmp;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string(ips, &buf));
  ret = netwib_ip_init_buf_best(&buf, &ipa);
  if (ret != wantedret) {
    netwib_er(netwib_fmt_display("For %s :\n", ips));
    netwib_er(netwib_fmt_display("ret=%{uint32} != ", ret));
    netwib_er(netwib_fmt_display("wantedret=%{uint32}\n", wantedret));
    return(NETWIBTEST_ERR_6);
  }
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_ip_cmp(&ipa, pwantedip, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      netwib_er(netwib_fmt_display("For %s :\n", ips));
      netwib_er(netwib_fmt_display("IP=%{ip} != ", &ipa));
      netwib_er(netwib_fmt_display("wantedIP=%{ip}\n", pwantedip));
      return(NETWIBTEST_ERR_7);
    }
  }

  return(NETWIB_ERR_OK);
}
netwib_err netwib_ip_init_buf_test3(netwib_conststring ips,
                                    netwib_conststring wantedips);
netwib_err netwib_ip_init_buf_test3(netwib_conststring ips,
                                    netwib_conststring wantedips)
{
  netwib_buf buf;
  netwib_ip wantedip;

  netwib_er(netwib_buf_init_ext_string(wantedips, &buf));
  netwib_er(netwib_ip_init_buf_best(&buf, &wantedip));
  netwib_er(netwib_ip_init_buf_test2(ips, NETWIB_ERR_OK, &wantedip));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_ip_init_buf_test(void)
{
  netwib_ip ipa;

  puts("++++----\nTest of netwib_ip_init_buf");
  puts("(might be slow on some systems...)");

  netwib_er(netwib_ip_init_ip4(0x01020304, &ipa));
  netwib_er(netwib_ip_init_buf_test2("1.2.3.4", NETWIB_ERR_OK, &ipa));
  netwib_er(netwib_ip_init_buf_test2(" 1.2.3.4", NETWIB_ERR_OK, &ipa));
  netwib_er(netwib_ip_init_buf_test2("1.2.3.4 \t ", NETWIB_ERR_OK, &ipa));
  netwib_er(netwib_ip_init_ip4(0, &ipa));
  netwib_er(netwib_ip_init_buf_test2("0.0.0.0", NETWIB_ERR_OK, &ipa));
  netwib_er(netwib_ip_init_ip4(0xFFFFFFFFu, &ipa));
  netwib_er(netwib_ip_init_buf_test2("255.255.255.255", NETWIB_ERR_OK, &ipa));

#if 0
  /* as it s not a valid IP4/6 address, system asks DNS, so it's slow */
  netwib_er(netwib_ip_init_buf_test2("1.2.3.4z", NETWIB_ERR_NOTCONVERTED, &ipa));
  netwib_er(netwib_ip_init_buf_test2("1.2.e3.4", NETWIB_ERR_NOTCONVERTED, &ipa));
  netwib_er(netwib_ip_init_buf_test2("1.2.256.4", NETWIB_ERR_NOTCONVERTED, &ipa));

  netwib_er(netwib_ip_init_buf_test2("azeazeaze", NETWIB_ERR_NOTCONVERTED, &ipa));
#endif

  netwib_er(netwib_ip_init_ip6_fields(0x102, 0x304, 0x506, 0x708, &ipa));
  netwib_er(netwib_ip_init_buf_test2("0:102:0:304:0:506:0:708", NETWIB_ERR_OK, &ipa));
  netwib_er(netwib_ip_init_ip6_fields(0x12345670, 0x12345671, 0x12345672, 0x12345678, &ipa));
  netwib_er(netwib_ip_init_buf_test2("1234:5670:1234:5671:1234:5672:1234:5678", NETWIB_ERR_OK, &ipa));
  netwib_er(netwib_ip_init_ip6_fields(0x102, 0x304, 0x506, 0x01020304, &ipa));
  netwib_er(netwib_ip_init_buf_test2("0:102:0:304:0:506:1.2.3.4", NETWIB_ERR_OK, &ipa));
  netwib_er(netwib_ip_init_ip6_fields(0x12345670, 0x12345671, 0x12340000, 0x12345678, &ipa));
  netwib_er(netwib_ip_init_buf_test2("1234:5670:1234:5671:1234::1234:5678", NETWIB_ERR_OK, &ipa));
  netwib_er(netwib_ip_init_ip6_fields(0x12345670, 0x12345671, 0, 0x12345678, &ipa));
  netwib_er(netwib_ip_init_buf_test2("1234:5670:1234:5671::1234:5678", NETWIB_ERR_OK, &ipa));
  netwib_er(netwib_ip_init_ip6_fields(0x12345670, 0x12340000, 0, 0x12345678, &ipa));
  netwib_er(netwib_ip_init_buf_test2("1234:5670:1234::1234:5678", NETWIB_ERR_OK, &ipa));
  netwib_er(netwib_ip_init_ip6_fields(0, 0, 0, 0x12345678, &ipa));
  netwib_er(netwib_ip_init_buf_test2("::1234:5678", NETWIB_ERR_OK, &ipa));
  netwib_er(netwib_ip_init_ip6_fields(0, 0, 0, 1, &ipa));
  netwib_er(netwib_ip_init_buf_test2("::1", NETWIB_ERR_OK, &ipa));
  netwib_er(netwib_ip_init_ip6_fields(0, 0, 0, 0, &ipa));
  netwib_er(netwib_ip_init_buf_test2("::", NETWIB_ERR_OK, &ipa));

#if 0
  netwib_er(netwib_ip_init_buf_test2("1:2:3:4:5:6:7", NETWIB_ERR_NOTCONVERTED, &ipa));
  netwib_er(netwib_ip_init_buf_test2("1:2:3:4:5:6:7:8:9", NETWIB_ERR_NOTCONVERTED, &ipa));
  netwib_er(netwib_ip_init_buf_test2("1:2:3:4:5:6:77777:8", NETWIB_ERR_NOTCONVERTED, &ipa));
  netwib_er(netwib_ip_init_buf_test2("1:2:3:4:5:6:777g:8", NETWIB_ERR_NOTCONVERTED, &ipa));
  netwib_er(netwib_ip_init_buf_test2("1:2::4::6:777:8", NETWIB_ERR_NOTCONVERTED, &ipa));
  netwib_er(netwib_ip_init_buf_test2("1:2:3:4:5:6:7:1.2.3.4", NETWIB_ERR_NOTCONVERTED, &ipa));
  netwib_er(netwib_ip_init_buf_test2("1:2:3:4:5:1.2.3.4.5", NETWIB_ERR_NOTCONVERTED, &ipa));
  netwib_er(netwib_ip_init_buf_test2("1:2:3:4:1.2.3.4:5", NETWIB_ERR_NOTCONVERTED, &ipa));
#endif

#ifdef NETWIBTESTDEF_HOST_NAME_1
  if (NETWIBTESTDEF_HOST_NAMETOIP_1) {
    netwib_er(netwib_ip_init_buf_test3(NETWIBTESTDEF_HOST_NAME_1,
                                       NETWIBTESTDEF_HOST_IP_1));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_2
  if (NETWIBTESTDEF_HOST_NAMETOIP_2) {
    netwib_er(netwib_ip_init_buf_test3(NETWIBTESTDEF_HOST_NAME_2,
                                       NETWIBTESTDEF_HOST_IP_2));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_3
  if (NETWIBTESTDEF_HOST_NAMETOIP_3) {
    netwib_er(netwib_ip_init_buf_test3(NETWIBTESTDEF_HOST_NAME_3,
                                       NETWIBTESTDEF_HOST_IP_3));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_4
  if (NETWIBTESTDEF_HOST_NAMETOIP_4) {
    netwib_er(netwib_ip_init_buf_test3(NETWIBTESTDEF_HOST_NAME_4,
                                       NETWIBTESTDEF_HOST_IP_4));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_5
  if (NETWIBTESTDEF_HOST_NAMETOIP_5) {
    netwib_er(netwib_ip_init_buf_test3(NETWIBTESTDEF_HOST_NAME_5,
                                       NETWIBTESTDEF_HOST_IP_5));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_6
  if (NETWIBTESTDEF_HOST_NAMETOIP_6) {
    netwib_er(netwib_ip_init_buf_test3(NETWIBTESTDEF_HOST_NAME_6,
                                       NETWIBTESTDEF_HOST_IP_6));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_7
  if (NETWIBTESTDEF_HOST_NAMETOIP_7) {
    netwib_er(netwib_ip_init_buf_test3(NETWIBTESTDEF_HOST_NAME_7,
                                       NETWIBTESTDEF_HOST_IP_7));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_8
  if (NETWIBTESTDEF_HOST_NAMETOIP_8) {
    netwib_er(netwib_ip_init_buf_test3(NETWIBTESTDEF_HOST_NAME_8,
                                       NETWIBTESTDEF_HOST_IP_8));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_9
  if (NETWIBTESTDEF_HOST_NAMETOIP_9) {
    netwib_er(netwib_ip_init_buf_test3(NETWIBTESTDEF_HOST_NAME_9,
                                       NETWIBTESTDEF_HOST_IP_9));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_10
  if (NETWIBTESTDEF_HOST_NAMETOIP_10) {
    netwib_er(netwib_ip_init_buf_test3(NETWIBTESTDEF_HOST_NAME_10,
                                       NETWIBTESTDEF_HOST_IP_10));
  }
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ip_init_eth_test2(netwib_conststring eths,
                                    netwib_conststring wantedips);
netwib_err netwib_ip_init_eth_test2(netwib_conststring eths,
                                    netwib_conststring wantedips)
{
  netwib_buf buf;
  netwib_ip wantedip, ipa;
  netwib_eth eth;
  netwib_cmp cmp;

  netwib_er(netwib_buf_init_ext_string(wantedips, &buf));
  netwib_er(netwib_ip_init_buf_best(&buf, &wantedip));

  netwib_er(netwib_buf_init_ext_string(eths, &buf));
  netwib_er(netwib_eth_init_buf(&buf, &eth));

  netwib_er(netwib_ip_init_eth(&eth, &ipa));
  netwib_er(netwib_ip_cmp(&ipa, &wantedip, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    netwib_er(netwib_fmt_display("For %s :\n", eths));
    netwib_er(netwib_fmt_display("IP=%{ip} != ", &ipa));
    netwib_er(netwib_fmt_display("wantedIP=%{ip}\n", &wantedip));
    return(NETWIBTEST_ERR_7);
  }

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_ip_init_eth_test(void)
{
  netwib_eth eth;
  netwib_ip ipa;

  puts("++++----\nTest of netwib_ip_init_eth");
  netwib_er(netwib_eth_init_fields(0, 1, 2, 3, 4, 5, &eth));
  netwib_eir(netwib_ip_init_eth(&eth, &ipa), NETWIB_ERR_NOTCONVERTED);

#ifdef NETWIBTESTDEF_HOST_ETH_1
  if (NETWIBTESTDEF_HOST_ETHTOIP_1) {
    netwib_er(netwib_ip_init_eth_test2(NETWIBTESTDEF_HOST_ETH_1,
                                       NETWIBTESTDEF_HOST_IP_1));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_ETH_2
  if (NETWIBTESTDEF_HOST_ETHTOIP_2) {
    netwib_er(netwib_ip_init_eth_test2(NETWIBTESTDEF_HOST_ETH_2,
                                       NETWIBTESTDEF_HOST_IP_2));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_ETH_3
  if (NETWIBTESTDEF_HOST_ETHTOIP_3) {
    netwib_er(netwib_ip_init_eth_test2(NETWIBTESTDEF_HOST_ETH_3,
                                       NETWIBTESTDEF_HOST_IP_3));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_ETH_4
  if (NETWIBTESTDEF_HOST_ETHTOIP_4) {
    netwib_er(netwib_ip_init_eth_test2(NETWIBTESTDEF_HOST_ETH_4,
                                       NETWIBTESTDEF_HOST_IP_4));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_ETH_5
  if (NETWIBTESTDEF_HOST_ETHTOIP_5) {
    netwib_er(netwib_ip_init_eth_test2(NETWIBTESTDEF_HOST_ETH_5,
                                       NETWIBTESTDEF_HOST_IP_5));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_ETH_6
  if (NETWIBTESTDEF_HOST_ETHTOIP_6) {
    netwib_er(netwib_ip_init_eth_test2(NETWIBTESTDEF_HOST_ETH_6,
                                       NETWIBTESTDEF_HOST_IP_6));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_ETH_7
  if (NETWIBTESTDEF_HOST_ETHTOIP_7) {
    netwib_er(netwib_ip_init_eth_test2(NETWIBTESTDEF_HOST_ETH_7,
                                       NETWIBTESTDEF_HOST_IP_7));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_ETH_8
  if (NETWIBTESTDEF_HOST_ETHTOIP_8) {
    netwib_er(netwib_ip_init_eth_test2(NETWIBTESTDEF_HOST_ETH_8,
                                       NETWIBTESTDEF_HOST_IP_8));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_ETH_9
  if (NETWIBTESTDEF_HOST_ETHTOIP_9) {
    netwib_er(netwib_ip_init_eth_test2(NETWIBTESTDEF_HOST_ETH_9,
                                       NETWIBTESTDEF_HOST_IP_9));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_ETH_10
  if (NETWIBTESTDEF_HOST_ETHTOIP_10) {
    netwib_er(netwib_ip_init_eth_test2(NETWIBTESTDEF_HOST_ETH_10,
                                       NETWIBTESTDEF_HOST_IP_10));
  }
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ip_init_kbd_test(void)
{
  netwib_buf buf;
  netwib_ip ipa, wantedip;
  netwib_cmp cmp;

  puts("++++----\nTest of netwib_ip_init_kbd");
  netwib_er(netwib_buf_init_ext_string("Enter \"sdf\" then \"1.2.3.4\"", &buf));
  netwib_er(netwib_ip_init_kbd(&buf, NETWIB_IP_INIT_KBD_NODEF, &ipa));
  netwib_er(netwib_ip_init_ip4(0x01020304, &wantedip));
  netwib_er(netwib_ip_cmp(&ipa, &wantedip, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    return(NETWIBTEST_ERR_7);
  }

#ifdef NETWIBTESTDEF_HOST_NAME_1
  if (NETWIBTESTDEF_HOST_NAMETOIP_1) {
    netwib_buf def;
    netwib_er(netwib_buf_init_mallocdefault(&buf));
    netwib_er(netwib_buf_append_string("Press 'Return'", &buf));
    netwib_er(netwib_buf_init_ext_string(NETWIBTESTDEF_HOST_NAME_1, &def));
    netwib_er(netwib_ip_init_kbd(&buf, &def, &ipa));
    netwib_er(netwib_buf_close(&buf));
    netwib_er(netwib_buf_init_ext_string(NETWIBTESTDEF_HOST_IP_1, &buf));
    netwib_er(netwib_ip_init_buf_best(&buf, &wantedip));
    netwib_er(netwib_ip_cmp(ip, wantedip, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      return(NETWIBTEST_ERR_7);
    }
  }
#else
  /* do not do this test for other */
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ip_cmp_test(void)
{
  netwib_ip ip1, ip2;
  netwib_cmp cmp;

  puts("++++----\nTest of netwib_ip_cmp");

  netwib_er(netwib_ip_init_ip4(1, &ip1));
  netwib_er(netwib_ip_init_ip4(2, &ip2));
  netwib_er(netwib_ip_cmp(&ip1, &ip2, &cmp));
  if (cmp != NETWIB_CMP_LT) {
    return(NETWIBTEST_ERR_7);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_ip_test2(netwib_constip *pip,
                                             netwib_ip_encodetype encodetype,
                                             netwib_err wantedret,
                                             netwib_conststring wantedstr)
{
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  ret = netwib_buf_append_ip(pip, encodetype, &buf);
  if (ret != wantedret) {
    netwib_er(netwib_fmt_display("ret=%{uint32} != ", ret));
    netwib_er(netwib_fmt_display("wantedret=%{uint32} for %{ip}(%s)\n",
                                 wantedret, pip, wantedstr));
    return(NETWIBTEST_ERR_6);
  }
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwibtest_buf_content2(&buf, wantedstr));
  }
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}
netwib_err netwib_buf_append_ip_test3(netwib_conststring ips,
                                      netwib_bool canconvert,
                                      netwib_conststring hn);
netwib_err netwib_buf_append_ip_test3(netwib_conststring ips,
                                      netwib_bool canconvert,
                                      netwib_conststring hn)
{
  netwib_buf buf;
  netwib_ip ipa;

  netwib_er(netwib_buf_init_ext_string(ips, &buf));
  netwib_er(netwib_ip_init_buf_best(&buf, &ipa));

  netwib_er(netwib_buf_append_ip_test2(&ipa, NETWIB_IP_ENCODETYPE_IP,
                                       NETWIB_ERR_OK, ips));
  if (canconvert) {
    netwib_er(netwib_buf_append_ip_test2(&ipa, NETWIB_IP_ENCODETYPE_HN,
                                         NETWIB_ERR_OK, hn));
  }

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_buf_append_ip_test4(netwib_constip *pip,
                                             netwib_ip_encodetype encodetype)
{
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_fmt_display("encodetype %{uint32} : ", encodetype));
  ret = netwib_buf_append_ip(pip, encodetype, &buf);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_fmt_display("ret=%{uint32}\n", ret));
  } else {
    netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
    netwib_er(netwib_fmt_display("\n"));
  }
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_buf_append_ip_test(void)
{
  netwib_ip ipa;

  puts("++++----\nTest of netwib_buf_append_ip");
  netwib_er(netwib_ip_init_ip4(0x12345678, &ipa));
  netwib_er(netwib_buf_append_ip_test2(&ipa, NETWIB_IP_ENCODETYPE_IP,
                                       NETWIB_ERR_OK, "18.52.86.120"));
#if 0
  netwib_er(netwib_buf_append_ip_test2(&ipa, NETWIB_IP_ENCODETYPE_HN,
                                       NETWIB_ERR_NOTCONVERTED, ""));
  netwib_er(netwib_buf_append_ip_test2(&ipa, NETWIB_IP_ENCODETYPE_HNBEST,
                                       NETWIB_ERR_OK, "18.52.86.120"));
#endif

  netwib_er(netwib_ip_init_ip6_fields(1, 3, 4, 5, &ipa));
  netwib_er(netwib_buf_append_ip_test2(&ipa, NETWIB_IP_ENCODETYPE_IP,
                                       NETWIB_ERR_OK, "0:1:0:3:0:4:0:5"));
  netwib_er(netwib_ip_init_ip6_fields(1, 0, 4, 5, &ipa));
  netwib_er(netwib_buf_append_ip_test2(&ipa, NETWIB_IP_ENCODETYPE_IP,
                                       NETWIB_ERR_OK, "0:1::4:0:5"));
  netwib_er(netwib_ip_init_ip6_fields(1, 0, 4, 0, &ipa));
  netwib_er(netwib_buf_append_ip_test2(&ipa, NETWIB_IP_ENCODETYPE_IP,
                                       NETWIB_ERR_OK, "0:1::4:0:0"));
  netwib_er(netwib_ip_init_ip6_fields(0, 0, 4, 0, &ipa));
  netwib_er(netwib_buf_append_ip_test2(&ipa, NETWIB_IP_ENCODETYPE_IP,
                                       NETWIB_ERR_OK, "::4:0:0"));
  netwib_er(netwib_ip_init_ip6_fields(1, 2, 4, 0, &ipa));
  netwib_er(netwib_buf_append_ip_test2(&ipa, NETWIB_IP_ENCODETYPE_IP,
                                       NETWIB_ERR_OK, "0:1:0:2:0:4::"));
  netwib_er(netwib_ip_init_ip6_fields(0, 0, 0, 0, &ipa));
  netwib_er(netwib_buf_append_ip_test2(&ipa, NETWIB_IP_ENCODETYPE_IP,
                                       NETWIB_ERR_OK, "::"));

  netwib_er(netwib_ip_init_ip4(0x7F000001, &ipa));
  netwib_er(netwib_buf_append_ip_test4(&ipa, NETWIB_IP_ENCODETYPE_IP));
  netwib_er(netwib_buf_append_ip_test4(&ipa, NETWIB_IP_ENCODETYPE_HN));
  netwib_er(netwib_buf_append_ip_test4(&ipa, NETWIB_IP_ENCODETYPE_HNIP));
  netwib_er(netwib_buf_append_ip_test4(&ipa, NETWIB_IP_ENCODETYPE_HNS));
  netwib_er(netwib_buf_append_ip_test4(&ipa, NETWIB_IP_ENCODETYPE_BEST));
  netwib_er(netwib_ip_init_ip6_fields(0, 0, 0, 1, &ipa));
  netwib_er(netwib_buf_append_ip_test4(&ipa, NETWIB_IP_ENCODETYPE_IP));
  netwib_er(netwib_buf_append_ip_test4(&ipa, NETWIB_IP_ENCODETYPE_HN));
  netwib_er(netwib_buf_append_ip_test4(&ipa, NETWIB_IP_ENCODETYPE_HNIP));
  netwib_er(netwib_buf_append_ip_test4(&ipa, NETWIB_IP_ENCODETYPE_HNS));
  netwib_er(netwib_buf_append_ip_test4(&ipa, NETWIB_IP_ENCODETYPE_BEST));

#ifdef NETWIBTESTDEF_HOST_NAME_1
  netwib_er(netwib_buf_append_ip_test3(NETWIBTESTDEF_HOST_IP_1,
                                       NETWIBTESTDEF_HOST_IPTONAME_1,
                                       NETWIBTESTDEF_HOST_NAME_1));
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_2
  netwib_er(netwib_buf_append_ip_test3(NETWIBTESTDEF_HOST_IP_2,
                                       NETWIBTESTDEF_HOST_IPTONAME_2,
                                       NETWIBTESTDEF_HOST_NAME_2));
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_3
  netwib_er(netwib_buf_append_ip_test3(NETWIBTESTDEF_HOST_IP_3,
                                       NETWIBTESTDEF_HOST_IPTONAME_3,
                                       NETWIBTESTDEF_HOST_NAME_3));
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_4
  netwib_er(netwib_buf_append_ip_test3(NETWIBTESTDEF_HOST_IP_4,
                                       NETWIBTESTDEF_HOST_IPTONAME_4,
                                       NETWIBTESTDEF_HOST_NAME_4));
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_5
  netwib_er(netwib_buf_append_ip_test3(NETWIBTESTDEF_HOST_IP_5,
                                       NETWIBTESTDEF_HOST_IPTONAME_5,
                                       NETWIBTESTDEF_HOST_NAME_5));
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_6
  netwib_er(netwib_buf_append_ip_test3(NETWIBTESTDEF_HOST_IP_6,
                                       NETWIBTESTDEF_HOST_IPTONAME_6,
                                       NETWIBTESTDEF_HOST_NAME_6));
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_7
  netwib_er(netwib_buf_append_ip_test3(NETWIBTESTDEF_HOST_IP_7,
                                       NETWIBTESTDEF_HOST_IPTONAME_7,
                                       NETWIBTESTDEF_HOST_NAME_7));
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_8
  netwib_er(netwib_buf_append_ip_test3(NETWIBTESTDEF_HOST_IP_8,
                                       NETWIBTESTDEF_HOST_IPTONAME_8,
                                       NETWIBTESTDEF_HOST_NAME_8));
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_9
  netwib_er(netwib_buf_append_ip_test3(NETWIBTESTDEF_HOST_IP_9,
                                       NETWIBTESTDEF_HOST_IPTONAME_9,
                                       NETWIBTESTDEF_HOST_NAME_9));
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_10
  netwib_er(netwib_buf_append_ip_test3(NETWIBTESTDEF_HOST_IP_10,
                                       NETWIBTESTDEF_HOST_IPTONAME_10,
                                       NETWIBTESTDEF_HOST_NAME_10));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_iptype_test(void)
{
  netwib_buf buf;
  puts("++++----\nTest of netwib_buf_append_iptype");

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_iptype(NETWIB_IPTYPE_IP4, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "IPv4"));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_iptype_init_kbd_test(void)
{
  netwib_buf buf;
  netwib_iptype type;

  puts("++++----\nTest of netwib_iptype_init_kbd");
  netwib_er(netwib_buf_init_ext_string("Select...", &buf));
  netwib_er(netwib_iptype_init_kbd(&buf, NETWIB_IPTYPE_IP6, &type));
  netwib_er(netwib_iptype_init_kbd(NULL, NETWIB_IPTYPE_INIT_KBD_NODEF, &type));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_net_ip(void)
{
  puts("###########################################\nTest of net/ip");

  puts("Note : please ignore fd leak created by getnameinfo(), getipnodebyaddr(), etc. under Tru64 Unix 5.1");

  netwib_er(netwib_ip_init_ip4_fields_test());
  netwib_er(netwib_ip_init_ip4_test());
  netwib_er(netwib_ip4_init_ip_test());
  netwib_er(netwib_ip_init_ip6_fields_test());
  netwib_er(netwib_ip_init_ip6_test());
  netwib_er(netwib_ip6_init_ip_test());
  netwib_er(netwib_ip_init_buf_test());

  netwib_er(netwib_ip_init_eth_test());
  if (netwibtest_testkbdlevel == 2) {
    netwib_er(netwib_ip_init_kbd_test());
  }

  netwib_er(netwib_ip_cmp_test());

  netwib_er(netwib_buf_append_ip_test());

  netwib_er(netwib_buf_append_iptype_test());
  if (netwibtest_testkbdlevel == 2) {
    netwib_er(netwib_iptype_init_kbd_test());
  }

  return(NETWIB_ERR_OK);
}
