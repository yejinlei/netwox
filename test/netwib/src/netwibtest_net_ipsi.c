#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err test_add2_ip(netwib_ips *pips,
                              netwib_ip4 ip4)
{
  netwib_ip ipa;
  netwib_er(netwib_ip_init_ip4(ip4, &ipa));
  netwib_er(netwib_ips_add_ip(pips, &ipa));
  return(NETWIB_ERR_OK);
}
static netwib_err test_del2_ip(netwib_ips *pips,
                              netwib_ip4 ip4)
{
  netwib_ip ipa;
  netwib_er(netwib_ip_init_ip4(ip4, &ipa));
  netwib_er(netwib_ips_del_ip(pips, &ipa));
  return(NETWIB_ERR_OK);
}
static netwib_err test_add2_iprange(netwib_ips *pips,
                                   netwib_ip4 infip4,
                                   netwib_ip4 supip4)
{
  netwib_ip infip, supip;
  netwib_er(netwib_ip_init_ip4(infip4, &infip));
  netwib_er(netwib_ip_init_ip4(supip4, &supip));
  netwib_er(netwib_ips_add_iprange(pips, &infip, &supip));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ips_index_init_test(void)
{
  netwib_ips *pips;
  netwib_ips_index *pipsi;

  puts("++++----\nTest of netwib_ips_index_init");
  netwib_er(netwib_ips_initdefault(&pips));
  netwib_er(netwib_ips_index_init(pips, &pipsi));
  netwib_er(netwib_ips_index_close(&pipsi));
  netwib_er(netwib_ips_close(&pips));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ips_index_close_test(void)
{
  netwib_ips *pips;
  netwib_ips_index *pipsi;

  puts("++++----\nTest of netwib_ips_index_close");
  netwib_er(netwib_ips_initdefault(&pips));
  netwib_er(netwib_ips_index_init(pips, &pipsi));
  netwib_er(netwib_ips_index_close(&pipsi));
  netwib_er(netwib_ips_close(&pips));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ips_index_ctl_set_test(void)
{
  netwib_ips *pips;
  netwib_ips_index *pipsi;

  puts("++++----\nTest of netwib_ips_index_ctl_set");
  netwib_er(netwib_ips_initdefault(&pips));
  netwib_er(netwib_ips_index_init(pips, &pipsi));
  netwib_er(netwib_ips_index_ctl_set_rewind(pipsi));
  netwib_er(netwib_ips_index_ctl_set_rewind(pipsi));
  netwib_er(netwib_ips_index_ctl_set_index(pipsi, pipsi));
  netwib_er(netwib_ips_index_close(&pipsi));
  netwib_er(netwib_ips_close(&pips));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ips_index_ctl_get_test(void)
{
  netwib_ips *pips;
  netwib_ips_index *pipsi;

  puts("++++----\nTest of netwib_ips_index_ctl_get");
  netwib_er(netwib_ips_initdefault(&pips));
  netwib_er(netwib_ips_index_init(pips, &pipsi));
  netwib_eir(netwib_ips_index_ctl_get(pipsi, NETWIB_IPS_INDEX_CTLTYPE_REWIND,
                                      NULL, NULL),
             NETWIB_ERR_PAINVALIDTYPE);
  netwib_eir(netwib_ips_index_ctl_get(pipsi, NETWIB_IPS_INDEX_CTLTYPE_INDEX,
                                      NULL, NULL),
             NETWIB_ERR_PAINVALIDTYPE);
  netwib_er(netwib_ips_index_close(&pipsi));
  netwib_er(netwib_ips_close(&pips));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ips_index_next_ip_test(void)
{
  netwib_ips *pips;
  netwib_ips_index *pipsi, *pipsi2;
  netwib_ip ipa;

  puts("++++----\nTest of netwib_ips_index_next_ip");
  netwib_er(netwib_ips_initdefault(&pips));
  netwib_er(netwib_ips_index_init(pips, &pipsi));
  netwib_eir(netwib_ips_index_next_ip(pipsi, &ipa), NETWIB_ERR_DATAEND);

  netwib_er(test_add2_iprange(pips, 0x01020304, 0x01020305));
  netwib_er(test_add2_ip(pips, 0x02020304));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5,2.2.3.4"));
  netwib_er(netwib_ips_index_ctl_set_rewind(pipsi));
  netwib_er(netwib_ips_index_next_ip(pipsi, &ipa));
  netwib_er(netwibtest_ip_content(&ipa, "1.2.3.4"));
  netwib_er(netwib_ips_index_next_ip(pipsi, &ipa));
  netwib_er(netwibtest_ip_content(&ipa, "1.2.3.5"));
  netwib_er(netwib_ips_index_next_ip(pipsi, &ipa));
  netwib_er(netwibtest_ip_content(&ipa, "2.2.3.4"));
  netwib_eir(netwib_ips_index_next_ip(pipsi, &ipa), NETWIB_ERR_DATAEND);
  netwib_er(netwib_ips_index_close(&pipsi));
  netwib_er(netwib_ips_close(&pips));

  netwib_er(netwib_ips_initdefault(&pips));
  netwib_er(netwib_ips_index_init(pips, &pipsi));
  netwib_er(test_add2_iprange(pips, 0x01020304, 0x01020305));
  netwib_er(test_add2_ip(pips, 0x02020304));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5,2.2.3.4"));
  netwib_er(netwib_ips_index_ctl_set_rewind(pipsi));
  netwib_er(netwib_ips_index_next_ip(pipsi, &ipa));
  netwib_er(netwibtest_ip_content(&ipa, "1.2.3.4"));
  netwib_er(test_del2_ip(pips, 0x01020304));
  netwib_er(netwib_ips_index_init(pips, &pipsi2));
  netwib_er(netwib_ips_index_ctl_set_index(pipsi2, pipsi));
  netwib_er(netwib_ips_index_next_ip(pipsi2, &ipa));
  netwib_er(netwib_ips_index_close(&pipsi2));
  netwib_er(netwibtest_ip_content(&ipa, "1.2.3.5"));
  netwib_er(netwib_ips_index_next_ip(pipsi, &ipa));
  netwib_er(netwibtest_ip_content(&ipa, "1.2.3.5"));
  netwib_er(netwib_ips_index_next_ip(pipsi, &ipa));
  netwib_er(netwibtest_ip_content(&ipa, "2.2.3.4"));
  netwib_er(test_del2_ip(pips, 0x02020304));
  netwib_eir(netwib_ips_index_next_ip(pipsi, &ipa), NETWIB_ERR_DATAEND);
  netwib_er(netwib_ips_index_close(&pipsi));
  netwib_er(netwib_ips_close(&pips));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ips_index_next_iprange_test(void)
{
  netwib_ips *pips;
  netwib_ips_index *pipsi;
  netwib_ip ipinf, ipsup;

  puts("++++----\nTest of netwib_ips_index_next_iprange");
  netwib_er(netwib_ips_initdefault(&pips));
  netwib_er(netwib_ips_index_init(pips, &pipsi));
  netwib_er(netwib_ips_index_ctl_set_rewind(pipsi));
  netwib_eir(netwib_ips_index_next_iprange(pipsi, &ipinf, &ipsup),
             NETWIB_ERR_DATAEND);

  netwib_er(test_add2_iprange(pips, 0x01020304, 0x01020305));
  netwib_er(test_add2_ip(pips, 0x02020304));
  netwib_er(test_add2_iprange(pips, 0x03020304, 0x03020308));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5,2.2.3.4,3.2.3.4-3.2.3.8"));
  netwib_er(netwib_ips_index_ctl_set_rewind(pipsi));
  netwib_er(netwib_ips_index_next_iprange(pipsi, &ipinf, &ipsup));
  netwib_er(netwibtest_ip_content(&ipinf, "1.2.3.4"));
  netwib_er(netwibtest_ip_content(&ipsup, "1.2.3.5"));
  netwib_er(netwib_ips_index_next_iprange(pipsi, &ipinf, &ipsup));
  netwib_er(netwibtest_ip_content(&ipinf, "2.2.3.4"));
  netwib_er(netwibtest_ip_content(&ipsup, "2.2.3.4"));
  netwib_er(netwib_ips_index_next_ip(pipsi, &ipinf));
  netwib_er(netwibtest_ip_content(&ipinf, "3.2.3.4"));
  netwib_er(netwib_ips_index_next_iprange(pipsi, &ipinf, &ipsup));
  netwib_er(netwibtest_ip_content(&ipinf, "3.2.3.5"));
  netwib_er(netwibtest_ip_content(&ipsup, "3.2.3.8"));
  netwib_eir(netwib_ips_index_next_iprange(pipsi, &ipinf, &ipsup),
             NETWIB_ERR_DATAEND);
  netwib_er(netwib_ips_close(&pips));
  netwib_er(netwib_ips_index_close(&pipsi));

  netwib_er(netwib_ips_initdefault(&pips));
  netwib_er(netwib_ips_index_init(pips, &pipsi));
  netwib_er(test_add2_iprange(pips, 0x01020304, 0x01020305));
  netwib_er(test_add2_ip(pips, 0x02020304));
  netwib_er(test_add2_iprange(pips, 0x03020304, 0x03020308));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5,2.2.3.4,3.2.3.4-3.2.3.8"));
  netwib_er(netwib_ips_index_ctl_set_rewind(pipsi));
  netwib_er(netwib_ips_index_next_iprange(pipsi, &ipinf, &ipsup));
  netwib_er(netwibtest_ip_content(&ipinf, "1.2.3.4"));
  netwib_er(netwibtest_ip_content(&ipsup, "1.2.3.5"));
  netwib_er(test_del2_ip(pips, 0x01020305));
  netwib_er(netwib_ips_index_next_iprange(pipsi, &ipinf, &ipsup));
  netwib_er(netwibtest_ip_content(&ipinf, "2.2.3.4"));
  netwib_er(netwibtest_ip_content(&ipsup, "2.2.3.4"));
  netwib_er(netwib_ips_index_next_ip(pipsi, &ipinf));
  netwib_er(netwibtest_ip_content(&ipinf, "3.2.3.4"));
  netwib_er(test_del2_ip(pips, 0x03020304));
  netwib_er(netwib_ips_index_next_iprange(pipsi, &ipinf, &ipsup));
  netwib_er(netwibtest_ip_content(&ipinf, "3.2.3.5"));
  netwib_er(netwibtest_ip_content(&ipsup, "3.2.3.8"));
  netwib_eir(netwib_ips_index_next_iprange(pipsi, &ipinf, &ipsup),
             NETWIB_ERR_DATAEND);
  netwib_er(netwib_ips_close(&pips));
  netwib_er(netwib_ips_index_close(&pipsi));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ips_index_this_ip_test(void)
{
  netwib_ips *pips;
  netwib_ips_index *pipsi;
  netwib_ip ipa;

  puts("++++----\nTest of netwib_ips_index_this_ip");
  netwib_er(netwib_ips_initdefault(&pips));
  netwib_er(netwib_ips_index_init(pips, &pipsi));
  netwib_eir(netwib_ips_index_this_ip(pipsi, &ipa), NETWIB_ERR_PAINDEXNODATA);

  netwib_er(test_add2_iprange(pips, 0x01020304, 0x01020305));
  netwib_er(test_add2_ip(pips, 0x02020304));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5,2.2.3.4"));
  netwib_er(netwib_ips_index_ctl_set_rewind(pipsi));
  netwib_er(netwib_ips_index_next_ip(pipsi, &ipa));
  netwib_er(netwibtest_ip_content(&ipa, "1.2.3.4"));
  netwib_er(netwib_ips_index_next_ip(pipsi, &ipa));
  netwib_er(netwibtest_ip_content(&ipa, "1.2.3.5"));
  netwib_er(netwib_ips_index_this_ip(pipsi, &ipa));
  netwib_er(netwibtest_ip_content(&ipa, "1.2.3.5"));
  netwib_er(netwib_ips_index_this_ip(pipsi, &ipa));
  netwib_er(netwibtest_ip_content(&ipa, "1.2.3.5"));
  netwib_er(netwib_ips_index_close(&pipsi));
  netwib_er(netwib_ips_close(&pips));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ips_index_this_iprange_test(void)
{
  netwib_ips *pips;
  netwib_ips_index *pipsi;
  netwib_ip ipinf, ipsup;

  puts("++++----\nTest of netwib_ips_index_this_iprange");
  netwib_er(netwib_ips_initdefault(&pips));
  netwib_er(netwib_ips_index_init(pips, &pipsi));
  netwib_er(netwib_ips_index_ctl_set_rewind(pipsi));
  netwib_eir(netwib_ips_index_this_iprange(pipsi, &ipinf, &ipsup),
             NETWIB_ERR_PAINDEXNODATA);

  netwib_er(test_add2_iprange(pips, 0x01020304, 0x01020305));
  netwib_er(test_add2_ip(pips, 0x02020304));
  netwib_er(test_add2_iprange(pips, 0x03020304, 0x03020308));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5,2.2.3.4,3.2.3.4-3.2.3.8"));
  netwib_er(netwib_ips_index_ctl_set_rewind(pipsi));
  netwib_er(netwib_ips_index_next_iprange(pipsi, &ipinf, &ipsup));
  netwib_er(netwibtest_ip_content(&ipinf, "1.2.3.4"));
  netwib_er(netwibtest_ip_content(&ipsup, "1.2.3.5"));
  netwib_er(netwib_ips_index_next_iprange(pipsi, &ipinf, &ipsup));
  netwib_er(netwibtest_ip_content(&ipinf, "2.2.3.4"));
  netwib_er(netwibtest_ip_content(&ipsup, "2.2.3.4"));
  netwib_er(netwib_ips_index_this_iprange(pipsi, &ipinf, &ipsup));
  netwib_er(netwibtest_ip_content(&ipinf, "2.2.3.4"));
  netwib_er(netwibtest_ip_content(&ipsup, "2.2.3.4"));
  netwib_er(netwib_ips_index_this_iprange(pipsi, &ipinf, &ipsup));
  netwib_er(netwibtest_ip_content(&ipinf, "2.2.3.4"));
  netwib_er(netwibtest_ip_content(&ipsup, "2.2.3.4"));
  netwib_er(netwib_ips_close(&pips));
  netwib_er(netwib_ips_index_close(&pipsi));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ips_index_this_del_test(void)
{
  netwib_ips *pips;
  netwib_ips_index *pipsi;
  netwib_ip ipa;

  puts("++++----\nTest of netwib_ips_index_this_del");
  netwib_er(netwib_ips_initdefault(&pips));
  netwib_er(netwib_ips_index_init(pips, &pipsi));
  netwib_eir(netwib_ips_index_this_del(pipsi), NETWIB_ERR_PAINDEXNODATA);

  netwib_er(test_add2_iprange(pips, 0x01020304, 0x01020305));
  netwib_er(test_add2_ip(pips, 0x02020304));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4-1.2.3.5,2.2.3.4"));
  netwib_er(netwib_ips_index_ctl_set_rewind(pipsi));
  netwib_er(netwib_ips_index_next_ip(pipsi, &ipa));
  netwib_er(netwibtest_ip_content(&ipa, "1.2.3.4"));
  netwib_er(netwib_ips_index_next_ip(pipsi, &ipa));
  netwib_er(netwibtest_ip_content(&ipa, "1.2.3.5"));
  netwib_er(netwib_ips_index_this_del(pipsi));
  netwib_er(netwibtest_ips_content(pips, "1.2.3.4,2.2.3.4"));
  netwib_er(netwib_ips_index_next_ip(pipsi, &ipa));
  netwib_er(netwibtest_ip_content(&ipa, "2.2.3.4"));
  netwib_er(netwib_ips_index_close(&pipsi));
  netwib_er(netwib_ips_close(&pips));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_net_ipsi(void)
{
  puts("###########################################\nTest of net/ipsi");
  netwib_er(netwib_ips_index_init_test());
  netwib_er(netwib_ips_index_close_test());

  netwib_er(netwib_ips_index_ctl_set_test());
  netwib_er(netwib_ips_index_ctl_get_test());

  netwib_er(netwib_ips_index_next_ip_test());
  netwib_er(netwib_ips_index_next_iprange_test());

  netwib_er(netwib_ips_index_this_ip_test());
  netwib_er(netwib_ips_index_this_iprange_test());

  netwib_er(netwib_ips_index_this_del_test());

  return(NETWIB_ERR_OK);
}
