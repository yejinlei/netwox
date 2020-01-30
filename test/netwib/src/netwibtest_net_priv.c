#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err test_1(netwib_conststring ips,
                         netwib_conststring masks,
                         netwib_uint32 prefix,
                         netwib_conststring wantnets)
{
  netwib_buf buf;
  netwib_ip ipad, mask, net;

  netwib_er(netwib_buf_init_ext_string(ips, &buf));
  netwib_er(netwib_ip_init_buf_best(&buf, &ipad));
  netwib_er(netwib_buf_init_ext_string(masks, &buf));
  netwib_er(netwib_ip_init_buf_best(&buf, &mask));
  netwib_er(netwib_priv_ip_net_init_ipmaskprefix(&ipad, &mask, prefix, &net));
  netwib_er(netwibtest_ip_content(&net, wantnets));

  return(NETWIB_ERR_OK);
}
static netwib_err test_ip_net_init_ipmaskprefix(void)
{
  puts("++++----\nTest of netwib_priv_ip_net_init_ipmaskprefix");
  netwib_er(test_1("1.2.3.130", "255.255.255.128", 25, "1.2.3.128"));
  netwib_er(test_1("1.2.3.130", "255.255.255.0", 24, "1.2.3.0"));
  netwib_er(test_1("1:2:3:4::1", "ffff:ffff:ffff::", 48, "1:2:3::"));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_2(netwib_conststring ips,
                         netwib_conststring nets,
                         netwib_conststring wantmasks,
                         netwib_uint32 wantprefix)
{
  netwib_buf buf;
  netwib_ip ipad, net, mask;
  netwib_uint32 prefix;

  netwib_er(netwib_buf_init_ext_string(ips, &buf));
  netwib_er(netwib_ip_init_buf_best(&buf, &ipad));
  netwib_er(netwib_buf_init_ext_string(nets, &buf));
  netwib_er(netwib_ip_init_buf_best(&buf, &net));

  prefix = 0;
  netwib_er(netwib_priv_ip_maskprefix_init_ipnet(&ipad, &net, &mask,
                                                 &prefix));
  netwib_er(netwibtest_ip_content(&mask, wantmasks));
  if (prefix != wantprefix) {
    return(NETWIBTEST_ERR_5);
  }

  return(NETWIB_ERR_OK);
}
static netwib_err test_ip_maskprefix_init_ipnet(void)
{
  puts("++++----\nTest of netwib_priv_ip_maskprefix_init_ipnet");
  netwib_er(test_2("1.2.3.130", "1.2.3.128", "255.255.255.128", 25));
  netwib_er(test_2("1.2.3.83", "1.2.3.64", "255.255.255.192", 26));
  netwib_er(test_2("1.2.3.48", "1.2.3.32", "255.255.255.224", 27));
  netwib_er(test_2("1.2.3.100", "1.2.3.96", "255.255.255.224", 27));
  netwib_er(test_2("1.2.3.130", "1.2.3.0", "255.255.255.0", 24));
  netwib_er(test_2("1.2.3.130", "1.2.0.0", "255.255.0.0", 16));
  netwib_er(test_2("1.2.3.130", "1.0.0.0", "255.0.0.0", 8));
  netwib_er(test_2("1.2.66.130", "1.64.0.0", "255.192.0.0", 10));
  netwib_er(test_2("1.2.196.130", "1.192.0.0", "255.192.0.0", 10));
  netwib_er(test_2("127.0.0.1", "127.0.0.0", "255.255.255.0", 24));
  netwib_er(test_2("1:2:3:4::1", "1:2:3:4::", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:0", 112));
  netwib_er(test_2("1:2:3:4:1::1", "1:2:3:4::", "ffff:ffff:ffff:ffff:0:0:0:0", 64));
  netwib_er(test_2("1:2:3:4::F801", "1:2:3:4::F800", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:f800", 117));
  netwib_er(test_2("1:2:3:4::F8F1", "1:2:3:4::F800", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:f800", 117));
  netwib_er(test_2("1:2:3:4::F8F1", "1:2:3:4::F880", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ff80", 121));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_3(netwib_iptype iptype,
                         netwib_uint32 prefix,
                         netwib_conststring wantmasks,
                         netwib_uint32 wantprefix)
{
  netwib_ip mask;
  netwib_uint32 prefix2;

  netwib_er(netwib_priv_ip_maskprefix_init_prefix(iptype, prefix, &mask,
                                                  &prefix2));
  netwib_er(netwibtest_ip_content(&mask, wantmasks));
  if (prefix2 != wantprefix) {
    return(NETWIBTEST_ERR_5);
  }

  return(NETWIB_ERR_OK);
}
static netwib_err test_ip_maskprefix_init_prefix(void)
{
  puts("++++----\nTest of netwib_priv_ip_maskprefix_init_prefix");
  netwib_er(test_3(NETWIB_IPTYPE_IP4, 2, "192.0.0.0", 2));
  netwib_er(test_3(NETWIB_IPTYPE_IP4, 25, "255.255.255.128", 25));
  netwib_er(test_3(NETWIB_IPTYPE_IP4, 27, "255.255.255.224", 27));
  netwib_er(test_3(NETWIB_IPTYPE_IP6, 121, "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ff80", 121));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_4(netwib_conststring str,
                         netwib_conststring wantnets,
                         netwib_conststring wantmasks,
                         netwib_uint32 wantprefix)
{
  netwib_buf buf;
  netwib_ip net, mask;
  netwib_uint32 prefix;

  netwib_er(netwib_buf_init_ext_string(str, &buf));
  netwib_er(netwib_priv_ip_netmaskprefix_init_buf(&buf, &net, &mask,
                                                  &prefix));
  netwib_er(netwibtest_ip_content(&net, wantnets));
  netwib_er(netwibtest_ip_content(&mask, wantmasks));
  if (prefix != wantprefix) {
    return(NETWIBTEST_ERR_5);
  }

  return(NETWIB_ERR_OK);
}
static netwib_err test_ip_netmaskprefix_init_buf(void)
{
  puts("++++----\nTest of netwib_priv_ip_netmaskprefix_init_buf");
  netwib_er(test_4("1.2.3.130", "1.2.3.130", "255.255.255.255", 32));
  netwib_er(test_4("1.2.3", "1.2.3.0", "255.255.255.0", 24));
  netwib_er(test_4("1.2", "1.2.0.0", "255.255.0.0", 16));
  netwib_er(test_4("127", "127.0.0.0", "255.0.0.0", 8));
  netwib_er(test_4("1.2.3/24", "1.2.3.0", "255.255.255.0", 24));
  netwib_er(test_4("1.2.3/255.255.255.0", "1.2.3.0", "255.255.255.0", 24));
  netwib_er(test_4("1.2.3/255.0.255.0", "1.2.3.0", "255.0.255.0", 8));
  netwib_er(test_4("1:2:3::/48", "1:2:3::", "ffff:ffff:ffff::", 48));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_5(netwib_iptype iptype,
                         netwib_conststring str,
                         netwib_conststring wantnets,
                         netwib_conststring wantmasks,
                         netwib_uint32 wantprefix)
{
  netwib_buf buf;
  netwib_ip net, mask;
  netwib_uint32 prefix;

  netwib_er(netwib_buf_init_ext_string(str, &buf));
  netwib_er(netwib_priv_ip_netmaskprefix_init_bufdefault(iptype, &buf, &net,
                                                         &mask, &prefix));
  netwib_er(netwibtest_ip_content(&net, wantnets));
  netwib_er(netwibtest_ip_content(&mask, wantmasks));
  if (prefix != wantprefix) {
    return(NETWIBTEST_ERR_5);
  }

  return(NETWIB_ERR_OK);
}
static netwib_err test_ip_netmaskprefix_init_bufdefault(void)
{
  puts("++++----\nTest of netwib_priv_ip_netmaskprefix_init_bufdefault");
  netwib_er(test_5(NETWIB_IPTYPE_IP4, "1.2.3.130",
                   "1.2.3.130", "255.255.255.255", 32));
  netwib_er(test_5(NETWIB_IPTYPE_IP4, "1.2.3",
                   "1.2.3.0", "255.255.255.0", 24));
  netwib_er(test_5(NETWIB_IPTYPE_IP4, "1.2",
                   "1.2.0.0", "255.255.0.0", 16));
  netwib_er(test_5(NETWIB_IPTYPE_IP4, "127",
                   "127.0.0.0", "255.0.0.0", 8));
  netwib_er(test_5(NETWIB_IPTYPE_IP4, "1.2.3/24",
                   "1.2.3.0", "255.255.255.0", 24));
  netwib_er(test_5(NETWIB_IPTYPE_IP4, "1.2.3/255.255.255.0",
                   "1.2.3.0", "255.255.255.0", 24));
  netwib_er(test_5(NETWIB_IPTYPE_IP4, "1.2.3/255.0.255.0",
                   "1.2.3.0", "255.0.255.0", 8));
  netwib_er(test_5(NETWIB_IPTYPE_IP4, "1:2:3::/48",
                   "1:2:3::", "ffff:ffff:ffff::", 48));
  netwib_er(test_5(NETWIB_IPTYPE_IP4, "default",
                   "0.0.0.0", "0.0.0.0", 0));
  netwib_er(test_5(NETWIB_IPTYPE_IP6, "default",
                   "::", "::", 0));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_6(netwib_conststring masks,
                         netwib_uint32 wantprefix)
{
  netwib_buf buf;
  netwib_ip mask;
  netwib_uint32 prefix;

  netwib_er(netwib_buf_init_ext_string(masks, &buf));
  netwib_er(netwib_ip_init_buf_best(&buf, &mask));
  netwib_er(netwib_priv_ip_prefix_init_mask(&mask, &prefix));
  if (prefix != wantprefix) {
    return(NETWIBTEST_ERR_5);
  }

  return(NETWIB_ERR_OK);
}
static netwib_err test_ip_prefix_init_mask(void)
{
  puts("++++----\nTest of netwib_priv_ip_prefix_init_mask");
  netwib_er(test_6("255.255.255.255", 32));
  netwib_er(test_6("255.255.255.0", 24));
  netwib_er(test_6("255.255.255.192", 26));
  netwib_er(test_6("255.0.255.0", 8));
  netwib_er(test_6("ffff:ffff:ffff::", 48));
  netwib_er(test_6("ff0f:ffff:ffff::", 8));
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_net_priv(void)
{
  puts("###########################################\nTest of net/priv");

  netwib_er(test_ip_net_init_ipmaskprefix());
  netwib_er(test_ip_maskprefix_init_ipnet());
  netwib_er(test_ip_maskprefix_init_prefix());
  netwib_er(test_ip_netmaskprefix_init_buf());
  netwib_er(test_ip_netmaskprefix_init_bufdefault());
  netwib_er(test_ip_prefix_init_mask());

  return(NETWIB_ERR_OK);
}
