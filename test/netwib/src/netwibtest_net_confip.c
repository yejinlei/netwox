#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_conf_ip_index_init_test(void)
{
  puts("++++----\nTest of netwib_conf_ip_index_init");
  puts("Redundant with netwib_conf_ip_index_next");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_conf_ip_index_close_test(void)
{
  puts("++++----\nTest of netwib_conf_ip_index_close");
  puts("Redundant with netwib_conf_ip_index_next");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_conf_ip_index_next_test(void)
{
  netwib_conf_ip conf;
  netwib_conf_ip_index *pconfindex;
  netwib_err ret;

  puts("++++----\nTest of netwib_conf_ip_index_next");

  netwib_er(netwib_conf_ip_index_init(&conf, &pconfindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_conf_ip_index_next(pconfindex);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_er(netwib_fmt_display("ip=%{ip}\n", &conf.ip));
  }
  netwib_er(netwib_conf_ip_index_close(&pconfindex));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_conf_ip_display_test(void)
{
  puts("++++----\nTest of netwib_conf_ip_display");
  netwib_er(netwib_conf_ip_display());
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_conf_ip_test(void)
{
  puts("++++----\nTest of netwib_buf_append_conf_ip");
  puts("Redundant with netwib_conf_ip_display");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_net_confip(void)
{
  puts("###########################################\nTest of net/confip");
  netwib_er(netwib_conf_ip_index_init_test());
  netwib_er(netwib_conf_ip_index_close_test());
  netwib_er(netwib_conf_ip_index_next_test());
  netwib_er(netwib_conf_ip_display_test());
  netwib_er(netwib_buf_append_conf_ip_test());

  return(NETWIB_ERR_OK);
}
