#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_conf_routes_index_init_test(void)
{
  puts("++++----\nTest of netwib_conf_routes_index_init");
  puts("Redundant with netwib_conf_routes_index_next");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_conf_routes_index_close_test(void)
{
  puts("++++----\nTest of netwib_conf_routes_index_close");
  puts("Redundant with netwib_conf_routes_index_next");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_conf_routes_index_next_test(void)
{
  netwib_conf_routes conf;
  netwib_conf_routes_index *pconfindex;
  netwib_err ret;

  puts("++++----\nTest of netwib_conf_routes_index_next");

  netwib_er(netwib_conf_routes_index_init(&conf, &pconfindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_conf_routes_index_next(pconfindex);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_er(netwib_fmt_display("devnum=%{uint32} ip=%{ip}\n", &conf.devnum,
                                 &conf.dst));
  }
  netwib_er(netwib_conf_routes_index_close(&pconfindex));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_conf_routes_display_test(void)
{
  puts("++++----\nTest of netwib_conf_routes_display");
  netwib_er(netwib_conf_routes_display());
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_conf_routes_test(void)
{
  puts("++++----\nTest of netwib_buf_append_conf_routes");
  puts("Redundant with netwib_conf_routes_display");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_conf_routes_reach_eth_test2(netwib_constip *pdstip)
{
  netwib_buf dev;
  netwib_ip srcip;
  netwib_eth srceth, dsteth;
  netwib_err ret;

  netwib_er(netwib_buf_init_mallocdefault(&dev));
  netwib_er(netwib_fmt_display("to reach %{ip} : ", pdstip));
  ret = netwib_conf_routes_reach_eth(pdstip, &dev, &srceth, &dsteth, &srcip);
  switch(ret) {
  case NETWIB_ERR_OK :
    netwib_er(netwib_fmt_display("%{buf} %{eth} %{eth} %{ip}\n",
                                 &dev, &srceth, &dsteth, &srcip));
    break;
  case NETWIB_ERR_ROUTELOCALDEV :
    netwib_er(netwib_fmt_display("local dev : %{buf}\n", &dev));
    break;
  case NETWIB_ERR_ROUTENOTETHER :
    netwib_er(netwib_fmt_display("not ether\n"));
    break;
  case NETWIB_ERR_ROUTENOTFOUND :
    netwib_er(netwib_fmt_display("not found\n"));
    break;
  default :
    netwib_er(netwib_fmt_display("error %{uint32}\n", ret));
  }
  netwib_er(netwib_buf_close(&dev));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_conf_routes_reach_eth_test(void)
{
  netwib_ip dstip;

  puts("++++----\nTest of netwib_conf_routes_reach_eth");
  netwib_er(netwib_ip_init_ip4(0x12345678, &dstip));
  netwib_er(netwib_conf_routes_reach_eth_test2(&dstip));
  netwib_er(netwib_ip_init_ip4(0x7F000001, &dstip));
  netwib_er(netwib_conf_routes_reach_eth_test2(&dstip));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_conf_routes_reach_ip_test2(netwib_constip *pdstip)
{
  netwib_buf dev;
  netwib_ip srcip;
  netwib_err ret;

  netwib_er(netwib_buf_init_mallocdefault(&dev));
  netwib_er(netwib_fmt_display("to reach %{ip} : ", pdstip));
  ret = netwib_conf_routes_reach_ip(pdstip, &dev, &srcip);
  switch(ret) {
  case NETWIB_ERR_OK :
    netwib_er(netwib_fmt_display("%{buf} %{ip}\n", &dev, &srcip));
    break;
  case NETWIB_ERR_ROUTELOCALDEV :
    netwib_er(netwib_fmt_display("local dev : %{buf}\n", &dev));
    break;
  case NETWIB_ERR_ROUTENOTFOUND :
    netwib_er(netwib_fmt_display("not found\n"));
    break;
  default :
    netwib_er(netwib_fmt_display("error %{uint32}\n", ret));
  }
  netwib_er(netwib_buf_close(&dev));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_conf_routes_reach_ip_test(void)
{
  netwib_ip dstip;

  puts("++++----\nTest of netwib_conf_routes_reach_ip");
  netwib_er(netwib_ip_init_ip4(0x12345678, &dstip));
  netwib_er(netwib_conf_routes_reach_ip_test2(&dstip));
  netwib_er(netwib_ip_init_ip4(0x7F000001, &dstip));
  netwib_er(netwib_conf_routes_reach_ip_test2(&dstip));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_net_confrout(void)
{
  puts("###########################################\nTest of net/confrout");
  netwib_er(netwib_conf_routes_index_init_test());
  netwib_er(netwib_conf_routes_index_close_test());
  netwib_er(netwib_conf_routes_index_next_test());
  netwib_er(netwib_conf_routes_display_test());
  netwib_er(netwib_buf_append_conf_routes_test());
  netwib_er(netwib_conf_routes_reach_eth_test());
  netwib_er(netwib_conf_routes_reach_ip_test());

  return(NETWIB_ERR_OK);
}

