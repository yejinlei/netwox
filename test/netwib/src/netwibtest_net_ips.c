#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err test_add_ip(netwib_ips *pips,
                              netwib_ip4 ip4)
{
  netwib_ip ipa;
  netwib_er(netwib_ip_init_ip4(ip4, &ipa));
  netwib_er(netwib_ips_add_ip(pips, &ipa));
  return(NETWIB_ERR_OK);
}
static netwib_err test_add_ip6(netwib_ips *pips,
                               netwib_uint32 a,
                               netwib_uint32 b,
                               netwib_uint32 c,
                               netwib_uint32 d)
{
  netwib_ip ipa;
  netwib_er(netwib_ip_init_ip6_fields(a, b, c, d, &ipa));
  netwib_er(netwib_ips_add_ip(pips, &ipa));
  return(NETWIB_ERR_OK);
}
static netwib_err test_del_ip(netwib_ips *pips,
                              netwib_ip4 ip4)
{
  netwib_ip ipa;
  netwib_er(netwib_ip_init_ip4(ip4, &ipa));
  netwib_er(netwib_ips_del_ip(pips, &ipa));
  return(NETWIB_ERR_OK);
}
static netwib_err test_contains_ip(netwib_ips *pips,
                                   netwib_ip4 ip4,
                                   netwib_bool *pyes)
{
  netwib_ip ipa;
  netwib_er(netwib_ip_init_ip4(ip4, &ipa));
  netwib_er(netwib_ips_contains_ip(pips, &ipa, pyes));
  return(NETWIB_ERR_OK);
}
static netwib_err test_add_iprange(netwib_ips *pips,
                                   netwib_ip4 infip4,
                                   netwib_ip4 supip4)
{
  netwib_ip infip, supip;
  netwib_er(netwib_ip_init_ip4(infip4, &infip));
  netwib_er(netwib_ip_init_ip4(supip4, &supip));
  netwib_er(netwib_ips_add_iprange(pips, &infip, &supip));
  return(NETWIB_ERR_OK);
}
static netwib_err test_del_iprange(netwib_ips *pips,
                                   netwib_ip4 infip4,
                                   netwib_ip4 supip4)
{
  netwib_ip infip, supip;
  netwib_er(netwib_ip_init_ip4(infip4, &infip));
  netwib_er(netwib_ip_init_ip4(supip4, &supip));
  netwib_er(netwib_ips_del_iprange(pips, &infip, &supip));
  return(NETWIB_ERR_OK);
}
static netwib_err test_contains_iprange(netwib_ips *pips,
                                        netwib_ip4 infip4,
                                        netwib_ip4 supip4,
                                        netwib_bool *pyes)
{
  netwib_ip infip, supip;
  netwib_er(netwib_ip_init_ip4(infip4, &infip));
  netwib_er(netwib_ip_init_ip4(supip4, &supip));
  netwib_er(netwib_ips_contains_iprange(pips, &infip, &supip, pyes));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ips_init_test(void)
{
  netwib_ips *pips;

  puts("++++----\nTest of netwib_ips_init");
  netwib_er(netwib_ips_initdefault(&pips));
  netwib_er(netwib_ips_close(&pips));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ips_close_test(void)
{
  netwib_ips *pips;

  puts("++++----\nTest of netwib_ips_close");
  netwib_er(netwib_ips_initdefault(&pips));
  netwib_er(netwib_ips_close(&pips));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ips_add_ip_test(void)
{
  netwib_ips *pips;

  puts("++++----\nTest of netwib_ips_add_ip");
  netwib_er(netwib_ips_initdefault(&pips));
  netwib_er(netwibtest_ips_content(pips, ""));
  netwib_er(test_add_ip(pips, 0x12345678));
  netwib_er(netwibtest_ips_content(pips, "18.52.86.120"));
  netwib_er(test_add_ip(pips, 0x12345678));
  netwib_er(netwibtest_ips_content(pips, "18.52.86.120"));
  netwib_er(test_add_ip(pips, 0x01020304));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4,18.52.86.120"));
  netwib_er(test_add_ip(pips, 0x01020305));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5,18.52.86.120"));
  netwib_er(test_add_ip(pips, 0x01020303));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.3-1.2.3.5,18.52.86.120"));
  netwib_er(test_add_ip(pips, 0x01020307));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.3-1.2.3.5,1.2.3.7,18.52.86.120"));
  netwib_er(test_add_ip(pips, 0x01020306));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.3-1.2.3.7,18.52.86.120"));
  netwib_er(test_add_ip(pips, 0x40020306));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.3-1.2.3.7,18.52.86.120,64.2.3.6"));
  netwib_er(test_add_ip6(pips, 1, 2, 3, 4));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.3-1.2.3.7,18.52.86.120,64.2.3.6,0:1:0:2:0:3:0:4"));
  netwib_er(test_add_ip6(pips, 1, 2, 3, 5));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.3-1.2.3.7,18.52.86.120,64.2.3.6,0:1:0:2:0:3:0:4-0:1:0:2:0:3:0:5"));
  netwib_er(netwib_ips_close(&pips));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ips_add_iprange_test(void)
{
  netwib_ips *pips;

  puts("++++----\nTest of netwib_ips_add_iprange");
  netwib_er(netwib_ips_initdefault(&pips));
  netwib_er(netwibtest_ips_content(pips, ""));
  netwib_eir(test_add_iprange(pips, 0x12345679, 0x12345678),
             NETWIB_ERR_PAINVALIDRANGE);
  netwib_er(test_add_iprange(pips, 0x12345678, 0x12345678));
  netwib_er(netwibtest_ips_content(pips, "18.52.86.120"));
  netwib_er(test_add_iprange(pips, 0x12345678, 0x12345678));
  netwib_er(netwibtest_ips_content(pips, "18.52.86.120"));
  netwib_er(test_add_iprange(pips, 0x01020304, 0x01020304));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4,18.52.86.120"));
  netwib_er(test_add_iprange(pips, 0x01020305, 0x01020305));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5,18.52.86.120"));
  netwib_er(test_add_iprange(pips, 0x01020303, 0x01020303));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.3-1.2.3.5,18.52.86.120"));
  netwib_er(test_add_iprange(pips, 0x01020307, 0x01020307));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.3-1.2.3.5,1.2.3.7,18.52.86.120"));
  netwib_er(test_add_iprange(pips, 0x01020306, 0x01020306));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.3-1.2.3.7,18.52.86.120"));
  netwib_er(test_add_iprange(pips, 0x40020306, 0x40020306));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.3-1.2.3.7,18.52.86.120,64.2.3.6"));
  netwib_er(test_add_iprange(pips, 0x02030405, 0x02030408));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.3-1.2.3.7,2.3.4.5-2.3.4.8,18.52.86.120,64.2.3.6"));
  netwib_er(test_add_iprange(pips, 0x02030406, 0x02030407));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.3-1.2.3.7,2.3.4.5-2.3.4.8,18.52.86.120,64.2.3.6"));
  netwib_er(test_add_iprange(pips, 0x0102030F, 0x02030400));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.3-1.2.3.7,1.2.3.15-2.3.4.0,2.3.4.5-2.3.4.8,18.52.86.120,64.2.3.6"));
  netwib_er(test_add_iprange(pips, 0x01020308, 0x0102030A));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.3-1.2.3.10,1.2.3.15-2.3.4.0,2.3.4.5-2.3.4.8,18.52.86.120,64.2.3.6"));
  netwib_er(test_add_iprange(pips, 0x0102030D, 0x0102030E));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.3-1.2.3.10,1.2.3.13-2.3.4.0,2.3.4.5-2.3.4.8,18.52.86.120,64.2.3.6"));
  netwib_er(test_add_iprange(pips, 0x0102030B, 0x0102030C));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.3-2.3.4.0,2.3.4.5-2.3.4.8,18.52.86.120,64.2.3.6"));
  netwib_er(test_add_iprange(pips, 0x02030403, 0x02030407));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.3-2.3.4.0,2.3.4.3-2.3.4.8,18.52.86.120,64.2.3.6"));
  netwib_er(test_add_iprange(pips, 0x020303FF, 0x02030402));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.3-2.3.4.8,18.52.86.120,64.2.3.6"));
  netwib_er(test_add_iprange(pips, 0x020303FF, 0x50455667));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.3-80.69.86.103"));
  netwib_er(test_add_iprange(pips, 0x00010203, 0x00020304));
  netwib_er(netwibtest_ips_content(pips, "0.1.2.3-0.2.3.4,1.2.3.3-80.69.86.103"));
  netwib_er(test_add_iprange(pips, 0x00010200, 0x00020004));
  netwib_er(netwibtest_ips_content(pips, "0.1.2.0-0.2.3.4,1.2.3.3-80.69.86.103"));
  netwib_er(test_add_iprange(pips, 0x00010200, 0x50455668));
  netwib_er(netwibtest_ips_content(pips, "0.1.2.0-80.69.86.104"));
  netwib_er(netwib_ips_close(&pips));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ips_add_ips_test(void)
{
  netwib_ips *pips1, *pips2;

  puts("++++----\nTest of netwib_ips_add_ips");
  netwib_er(netwib_ips_initdefault(&pips1));
  netwib_er(test_add_iprange(pips1, 0x01020304, 0x01020305));
  netwib_er(test_add_iprange(pips1, 0x02020304, 0x0202030F));
  netwib_er(test_add_iprange(pips1, 0x03020304, 0x03020304));
  netwib_er(netwibtest_ips_content(pips1, "1.2.3.4-1.2.3.5,2.2.3.4-2.2.3.15,3.2.3.4"));
  netwib_er(netwib_ips_initdefault(&pips2));
  netwib_er(test_add_iprange(pips2, 0x01020301, 0x01020306));
  netwib_er(test_add_iprange(pips2, 0x02020306, 0x02020310));
  netwib_er(test_add_iprange(pips2, 0x03020300, 0x03020308));
  netwib_er(netwibtest_ips_content(pips2, "1.2.3.1-1.2.3.6,2.2.3.6-2.2.3.16,3.2.3.0-3.2.3.8"));
  netwib_er(netwib_ips_add_ips(pips1, pips2));
  netwib_er(netwibtest_ips_content(pips1, "1.2.3.1-1.2.3.6,2.2.3.4-2.2.3.16,3.2.3.0-3.2.3.8"));
  netwib_er(netwibtest_ips_content(pips2, "1.2.3.1-1.2.3.6,2.2.3.6-2.2.3.16,3.2.3.0-3.2.3.8"));

  netwib_er(netwib_ips_close(&pips1));
  netwib_er(netwib_ips_close(&pips2));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ips_add_buf_test2(netwib_conststring text,
                                           netwib_conststring wantedtext);
static netwib_err netwib_ips_add_buf_test2(netwib_conststring text,
                                           netwib_conststring wantedtext)
{
  netwib_buf buf;
  netwib_ips *pips;
  netwib_err ret;

  netwib_er(netwib_ips_initdefault(&pips));
  netwib_er(netwib_buf_init_ext_string(text, &buf));
  ret = netwib_ips_add_buf(pips, &buf);
  if (ret == NETWIB_ERR_OK) {
    ret = netwibtest_ips_content(pips, wantedtext);
  }
  netwib_er(netwib_ips_close(&pips));

  return(ret);
}
/*-------------------------------------------------------------*/
netwib_err netwib_ips_add_buf_test3(netwib_conststring hn,
                                    netwib_conststring ipa);
netwib_err netwib_ips_add_buf_test3(netwib_conststring hn,
                                    netwib_conststring ipa)
{
  netwib_buf buf;
  netwib_string pc;

  netwib_er(netwib_ips_add_buf_test2(hn, ipa));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_string(hn, &buf));
  netwib_er(netwib_buf_append_byte('=', &buf));
  netwib_er(netwib_buf_append_string(hn, &buf));
  netwib_er(netwib_buf_ref_string(&buf, &pc));
  netwib_er(netwib_ips_add_buf_test2(pc, ipa));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_ips_add_buf_test(void)
{
  puts("++++----\nTest of netwib_ips_add_buf");
  puts("(might be slow on some systems...)");

  netwib_er(netwib_ips_add_buf_test2(" 1.2.3.4 ", "1.2.3.4"));
  netwib_er(netwib_ips_add_buf_test2("0.0.0.0", "0.0.0.0"));
  netwib_er(netwib_ips_add_buf_test2("1.2.3.4-2.3.4.5", "1.2.3.4-2.3.4.5"));
  netwib_er(netwib_ips_add_buf_test2("1.2.3.4-2.3.4.5, 5.2.3.4-6.3.4.5", "1.2.3.4-2.3.4.5,5.2.3.4-6.3.4.5"));
  netwib_er(netwib_ips_add_buf_test2("1.2.3.4-2.3.4.5  ,\t  5.2.3.4-6.3.4.5", "1.2.3.4-2.3.4.5,5.2.3.4-6.3.4.5"));
  netwib_er(netwib_ips_add_buf_test2("1.2.3.4-2.3.4.5  \t  5.2.3.4-6.3.4.5", "1.2.3.4-2.3.4.5,5.2.3.4-6.3.4.5"));
  netwib_er(netwib_ips_add_buf_test2("1.2.3.0/255.255.255.0", "1.2.3.0-1.2.3.255"));
  netwib_er(netwib_ips_add_buf_test2("1.2.3.1/255.255.255.0", "1.2.3.0-1.2.3.255"));
  netwib_er(netwib_ips_add_buf_test2("1.2.3/255.255.255.0", "1.2.3.0-1.2.3.255"));
  netwib_er(netwib_ips_add_buf_test2("1/255.255.255.0", "1.0.0.0-1.0.0.255"));
  netwib_er(netwib_ips_add_buf_test2("1.2.3.0/24", "1.2.3.0-1.2.3.255"));
  netwib_er(netwib_ips_add_buf_test2("1.2.3.4/32", "1.2.3.4"));
  netwib_er(netwib_ips_add_buf_test2("1.2.3.0%255.255.255.0", "1.2.3.1-1.2.3.254"));
  netwib_er(netwib_ips_add_buf_test2("1.2.3.1%255.255.255.0", "1.2.3.1-1.2.3.254"));
  netwib_er(netwib_ips_add_buf_test2("1.2.3%255.255.255.0", "1.2.3.1-1.2.3.254"));
  netwib_er(netwib_ips_add_buf_test2("1%255.255.255.0", "1.0.0.1-1.0.0.254"));
  netwib_er(netwib_ips_add_buf_test2("1.2.3.0%24", "1.2.3.1-1.2.3.254"));

  netwib_er(netwib_ips_add_buf_test2("::1", "::1"));
  netwib_er(netwib_ips_add_buf_test2("::", "::"));
  netwib_er(netwib_ips_add_buf_test2("3::45", "3::45"));
  netwib_er(netwib_ips_add_buf_test2("::-ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", "::-ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"));

#if 0
  netwib_eir(netwib_ips_add_buf_test2("1.2.3.4%32", ""), NETWIB_ERR_PAINVALIDRANGE);
  netwib_eir(netwib_ips_add_buf_test2("1.2.3.4.5", ""), NETWIB_ERR_NOTCONVERTED);
  netwib_eir(netwib_ips_add_buf_test2("1.2.3.4-5", ""), NETWIB_ERR_NOTCONVERTED);
  netwib_eir(netwib_ips_add_buf_test2("1.2.3.4-5.6.7.8.9", ""), NETWIB_ERR_NOTCONVERTED);
  netwib_eir(netwib_ips_add_buf_test2("1.2.3.4/33", ""), NETWIB_ERR_NOTCONVERTED);
  netwib_eir(netwib_ips_add_buf_test2("1.2.3.4%33", ""), NETWIB_ERR_NOTCONVERTED);
  netwib_eir(netwib_ips_add_buf_test2("1.2.3.4/33.4", ""), NETWIB_ERR_NOTCONVERTED);
  netwib_eir(netwib_ips_add_buf_test2("1.2.3.4/33.4.0.0.", ""), NETWIB_ERR_NOTCONVERTED);
#endif

  netwib_er(netwib_ips_add_buf_test2("all", "0.0.0.0-255.255.255.255,::-ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"));
  netwib_er(netwib_ips_add_buf_test2("all,!1.2.3.4-2.3.4.5", "0.0.0.0-1.2.3.3,2.3.4.6-255.255.255.255,::-ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"));
  netwib_er(netwib_ips_add_buf_test2("all, !  1.2.3.4-2.3.4.5", "0.0.0.0-1.2.3.3,2.3.4.6-255.255.255.255,::-ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"));
  netwib_er(netwib_ips_add_buf_test2("all,!1.2.3.4-2.3.4.5,1.2.4.6", "0.0.0.0-1.2.3.3,1.2.4.6,2.3.4.6-255.255.255.255,::-ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"));
  netwib_er(netwib_ips_add_buf_test2("all !1.2.3.4-2.3.4.5 1.2.4.6", "0.0.0.0-1.2.3.3,1.2.4.6,2.3.4.6-255.255.255.255,::-ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"));
  netwib_er(netwib_ips_add_buf_test2("all,1.2.3.4,!all", ""));

#ifdef NETWIBTESTDEF_HOST_NAME_1
  if (NETWIBTESTDEF_HOST_NAMETOIP_1) {
    netwib_er(netwib_ips_add_buf_test3(NETWIBTESTDEF_HOST_NAME_1,
                                       NETWIBTESTDEF_HOST_IP_1));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_2
  if (NETWIBTESTDEF_HOST_NAMETOIP_2) {
    netwib_er(netwib_ips_add_buf_test3(NETWIBTESTDEF_HOST_NAME_2,
                                       NETWIBTESTDEF_HOST_IP_2));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_3
  if (NETWIBTESTDEF_HOST_NAMETOIP_3) {
    netwib_er(netwib_ips_add_buf_test3(NETWIBTESTDEF_HOST_NAME_3,
                                       NETWIBTESTDEF_HOST_IP_3));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_4
  if (NETWIBTESTDEF_HOST_NAMETOIP_4) {
    netwib_er(netwib_ips_add_buf_test3(NETWIBTESTDEF_HOST_NAME_4,
                                       NETWIBTESTDEF_HOST_IP_4));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_5
  if (NETWIBTESTDEF_HOST_NAMETOIP_5) {
    netwib_er(netwib_ips_add_buf_test3(NETWIBTESTDEF_HOST_NAME_5,
                                       NETWIBTESTDEF_HOST_IP_5));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_6
  if (NETWIBTESTDEF_HOST_NAMETOIP_6) {
    netwib_er(netwib_ips_add_buf_test3(NETWIBTESTDEF_HOST_NAME_6,
                                       NETWIBTESTDEF_HOST_IP_6));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_7
  if (NETWIBTESTDEF_HOST_NAMETOIP_7) {
    netwib_er(netwib_ips_add_buf_test3(NETWIBTESTDEF_HOST_NAME_7,
                                       NETWIBTESTDEF_HOST_IP_7));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_8
  if (NETWIBTESTDEF_HOST_NAMETOIP_8) {
    netwib_er(netwib_ips_add_buf_test3(NETWIBTESTDEF_HOST_NAME_8,
                                       NETWIBTESTDEF_HOST_IP_8));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_9
  if (NETWIBTESTDEF_HOST_NAMETOIP_9) {
    netwib_er(netwib_ips_add_buf_test3(NETWIBTESTDEF_HOST_NAME_9,
                                       NETWIBTESTDEF_HOST_IP_9));
  }
#endif
#ifdef NETWIBTESTDEF_HOST_NAME_10
  if (NETWIBTESTDEF_HOST_NAMETOIP_10) {
    netwib_er(netwib_ips_add_buf_test3(NETWIBTESTDEF_HOST_NAME_10,
                                       NETWIBTESTDEF_HOST_IP_10));
  }
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ips_add_kbd_test(void)
{
  netwib_buf buf, defbuf;
  netwib_ips *pips;

  puts("++++----\nTest of netwib_ips_add_kbd");

  netwib_er(netwib_ips_initdefault(&pips));
  netwib_er(netwib_buf_init_ext_string("Enter \"1.2.3.4,5.6.7.8\"", &buf));
  netwib_er(netwib_ips_add_kbd(pips, &buf, NULL));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4,5.6.7.8"));
  netwib_er(netwib_ips_close(&pips));

  netwib_er(netwib_ips_initdefault(&pips));
  netwib_er(netwib_buf_init_ext_string("Press 'Enter'", &buf));
  netwib_er(netwib_buf_init_ext_string("1.1.1.1-1.2.2.2", &defbuf));
  netwib_er(netwib_ips_add_kbd(pips, &buf, &defbuf));
  netwib_er(netwibtest_ips_content(pips, "1.1.1.1-1.2.2.2"));
  netwib_er(netwib_ips_close(&pips));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ips_del_ip_test(void)
{
  netwib_ips *pips;

  puts("++++----\nTest of netwib_ips_del_ip");
  netwib_er(netwib_ips_initdefault(&pips));
  netwib_er(test_add_iprange(pips, 0x01020304, 0x01020305));
  netwib_er(test_add_iprange(pips, 0x02020304, 0x0202030F));
  netwib_er(test_add_iprange(pips, 0x03020304, 0x03020304));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5,2.2.3.4-2.2.3.15,3.2.3.4"));

  netwib_er(test_del_ip(pips, 0x12345678));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5,2.2.3.4-2.2.3.15,3.2.3.4"));
  netwib_er(test_del_ip(pips, 0x03020304));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5,2.2.3.4-2.2.3.15"));
  netwib_er(test_del_ip(pips, 0x0202030F));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5,2.2.3.4-2.2.3.14"));
  netwib_er(test_del_ip(pips, 0x0202030D));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5,2.2.3.4-2.2.3.12,2.2.3.14"));
  netwib_er(test_del_ip(pips, 0x0202030E));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5,2.2.3.4-2.2.3.12"));

  netwib_er(netwib_ips_close(&pips));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ips_del_iprange_test(void)
{
  netwib_ips *pips;

  puts("++++----\nTest of netwib_ips_del_iprange");
  netwib_er(netwib_ips_initdefault(&pips));
  netwib_er(test_add_iprange(pips, 0x01020304, 0x01020305));
  netwib_er(test_add_iprange(pips, 0x02020304, 0x0202030F));
  netwib_er(test_add_iprange(pips, 0x03020304, 0x03020304));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5,2.2.3.4-2.2.3.15,3.2.3.4"));

  netwib_er(test_del_iprange(pips, 0x12345678, 0x12345678));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5,2.2.3.4-2.2.3.15,3.2.3.4"));
  netwib_er(test_del_iprange(pips, 0x03020304, 0x03020304));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5,2.2.3.4-2.2.3.15"));
  netwib_er(test_del_iprange(pips, 0x0202030F, 0x0202030F));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5,2.2.3.4-2.2.3.14"));
  netwib_er(test_del_iprange(pips, 0x0202030D, 0x0202030D));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5,2.2.3.4-2.2.3.12,2.2.3.14"));
  netwib_er(test_del_iprange(pips, 0x0202030E, 0x0202030E));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5,2.2.3.4-2.2.3.12"));

  netwib_er(test_del_iprange(pips, 0x02020306, 0x02020308));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5,2.2.3.4-2.2.3.5,2.2.3.9-2.2.3.12"));
  netwib_er(test_del_iprange(pips, 0x0202030B, 0x0202030D));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5,2.2.3.4-2.2.3.5,2.2.3.9-2.2.3.10"));
  netwib_er(test_del_iprange(pips, 0x02020305, 0x02020309));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5,2.2.3.4,2.2.3.10"));
  netwib_er(test_del_iprange(pips, 0x02020302, 0x02020319));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5"));
  netwib_er(test_del_iprange(pips, 0x00020302, 0x01020303));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5"));
  netwib_er(test_del_iprange(pips, 0x00020304, 0x01020304));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.5"));
  netwib_er(test_del_iprange(pips, 0x00020300, 0x01020314));
  netwib_er(netwibtest_ips_content(pips, ""));

  netwib_er(netwib_ips_close(&pips));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ips_del_ips_test(void)
{
  netwib_ips *pips1, *pips2;

  puts("++++----\nTest of netwib_ips_del_ips");
  netwib_er(netwib_ips_initdefault(&pips1));
  netwib_er(test_add_iprange(pips1, 0x01020304, 0x01020305));
  netwib_er(test_add_iprange(pips1, 0x02020304, 0x0202030F));
  netwib_er(test_add_iprange(pips1, 0x03020304, 0x03020304));
  netwib_er(netwibtest_ips_content(pips1, "1.2.3.4-1.2.3.5,2.2.3.4-2.2.3.15,3.2.3.4"));
  netwib_er(netwib_ips_initdefault(&pips2));
  netwib_er(test_add_iprange(pips2, 0x01020301, 0x01020306));
  netwib_er(test_add_iprange(pips2, 0x02020306, 0x02020310));
  netwib_er(test_add_iprange(pips2, 0x03020300, 0x03020308));
  netwib_er(netwibtest_ips_content(pips2, "1.2.3.1-1.2.3.6,2.2.3.6-2.2.3.16,3.2.3.0-3.2.3.8"));

  netwib_er(netwib_ips_del_ips(pips1, pips2));
  netwib_er(netwibtest_ips_content(pips1, "2.2.3.4-2.2.3.5"));
  netwib_er(netwibtest_ips_content(pips2, "1.2.3.1-1.2.3.6,2.2.3.6-2.2.3.16,3.2.3.0-3.2.3.8"));

  netwib_er(netwib_ips_close(&pips1));
  netwib_er(netwib_ips_close(&pips2));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ips_contains_ip_test(void)
{
  netwib_ips *pips;
  netwib_bool b;

  puts("++++----\nTest of netwib_ips_contains_ip");
  netwib_er(netwib_ips_initdefault(&pips));
  netwib_er(test_add_iprange(pips, 0x01020304, 0x01020305));
  netwib_er(test_add_iprange(pips, 0x02020304, 0x0202030F));
  netwib_er(test_add_iprange(pips, 0x03020304, 0x03020304));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5,2.2.3.4-2.2.3.15,3.2.3.4"));

  netwib_er(test_contains_ip(pips, 0x01020304, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(test_contains_ip(pips, 0x01020305, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(test_contains_ip(pips, 0x01020306, &b));
  if (b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(test_contains_ip(pips, 0x81020306u, &b));
  if (b) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_ips_close(&pips));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ips_contains_iprange_test(void)
{
  netwib_ips *pips;
  netwib_bool b;

  puts("++++----\nTest of netwib_ips_contains_iprange");
  netwib_er(netwib_ips_initdefault(&pips));
  netwib_er(test_add_iprange(pips, 0x01020304, 0x01020305));
  netwib_er(test_add_iprange(pips, 0x02020304, 0x0202030F));
  netwib_er(test_add_iprange(pips, 0x03020304, 0x03020304));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5,2.2.3.4-2.2.3.15,3.2.3.4"));

  netwib_er(test_contains_iprange(pips, 0x01020304, 0x01020304, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(test_contains_iprange(pips, 0x01020305, 0x01020305, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(test_contains_iprange(pips, 0x01020306, 0x01020306, &b));
  if (b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(test_contains_iprange(pips, 0x81020306u, 0x81020306u, &b));
  if (b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(test_contains_iprange(pips, 0x02020305, 0x02020307, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(test_contains_iprange(pips, 0x02020300, 0x02020307, &b));
  if (b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(test_contains_iprange(pips, 0x01020305, 0x02020307, &b));
  if (b) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_ips_close(&pips));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_ips_test(void)
{
  netwib_ips *pips;
  netwib_buf buf;

  puts("++++----\nTest of netwib_buf_append_ips");
  netwib_er(netwib_ips_initdefault(&pips));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_ips(pips, NETWIB_IPS_ENCODETYPE_BEST, &buf));
  netwib_er(netwibtest_buf_content2(&buf, ""));
  netwib_er(test_add_iprange(pips, 0x01020304, 0x01020305));
  netwib_er(netwib_buf_append_ips(pips, NETWIB_IPS_ENCODETYPE_BEST, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "1.2.3.4-1.2.3.5"));
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_ips_close(&pips));

  netwib_er(netwib_ips_initdefault(&pips));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(test_add_ip6(pips, 0x01020304, 0x01020305, 0x01020304,0x01020305));
  netwib_er(netwib_buf_append_ips(pips, NETWIB_IPS_ENCODETYPE_BEST, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "102:304:102:305:102:304:102:305"));
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_ips_close(&pips));

  netwib_er(netwib_ips_initdefault(&pips));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(test_add_ip6(pips, 0,0,0,0));
  netwib_er(netwib_buf_append_ips(pips, NETWIB_IPS_ENCODETYPE_BEST, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "::"));
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_ips_close(&pips));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_net_ips(void)
{
  puts("###########################################\nTest of net/ips");

  netwib_er(netwib_ips_init_test());
  netwib_er(netwib_ips_close_test());

  netwib_er(netwib_ips_add_ip_test());
  netwib_er(netwib_ips_add_iprange_test());
  netwib_er(netwib_ips_add_ips_test());

  netwib_er(netwib_ips_add_buf_test());
  if (netwibtest_testkbdlevel == 2) {
    netwib_er(netwib_ips_add_kbd_test());
  }

  netwib_er(netwib_ips_del_ip_test());
  netwib_er(netwib_ips_del_iprange_test());
  netwib_er(netwib_ips_del_ips_test());

  netwib_er(netwib_ips_contains_ip_test());
  netwib_er(netwib_ips_contains_iprange_test());

  netwib_er(netwib_buf_append_ips_test());

  return(NETWIB_ERR_OK);
}
