#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_conf_arpcache_index_init_test(void)
{
  puts("++++----\nTest of netwib_conf_arpcache_index_init");
  puts("Redundant with netwib_conf_arpcache_index_next");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_conf_arpcache_index_close_test(void)
{
  puts("++++----\nTest of netwib_conf_arpcache_index_close");
  puts("Redundant with netwib_conf_arpcache_index_next");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_conf_arpcache_index_next_test(void)
{
  netwib_conf_arpcache conf;
  netwib_conf_arpcache_index *pconfindex;
  netwib_err ret;

  puts("++++----\nTest of netwib_conf_arpcache_index_next");

  netwib_er(netwib_conf_arpcache_index_init(&conf, &pconfindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_conf_arpcache_index_next(pconfindex);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_er(netwib_fmt_display("devnum=%{uint32} ip=%{ip}\n", conf.devnum,
                                 &conf.ip));
  }
  netwib_er(netwib_conf_arpcache_index_close(&pconfindex));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_conf_arpcache_display_test(void)
{
  puts("++++----\nTest of netwib_conf_arpcache_display");
  netwib_er(netwib_conf_arpcache_display());
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_conf_arpcache_add_test(void)
{
  netwib_eth eth;
  netwib_ip ipa;

  puts("++++----\nTest of netwib_conf_arpcache_add");

  netwib_er(netwib_eth_init_fields(1,2,3,4,5,6, &eth));
  netwib_er(netwib_ip_init_ip4(123, &ipa));
  netwib_er(netwib_conf_arpcache_add(NULL, &eth, &ipa));
  netwib_er(netwib_conf_arpcache_display());
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_conf_arpcache_test(void)
{
  puts("++++----\nTest of netwib_buf_append_conf_arpcache");
  puts("Redundant with netwib_conf_arpcache_display");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_net_confarp(void)
{
  puts("###########################################\nTest of net/confarp");
  netwib_er(netwib_conf_arpcache_index_init_test());
  netwib_er(netwib_conf_arpcache_index_close_test());
  netwib_er(netwib_conf_arpcache_index_next_test());
  netwib_er(netwib_conf_arpcache_display_test());
  netwib_er(netwib_conf_arpcache_add_test());
  netwib_er(netwib_buf_append_conf_arpcache_test());

  return(NETWIB_ERR_OK);
}
