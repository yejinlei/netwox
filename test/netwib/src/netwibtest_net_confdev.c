#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_conf_devices_index_init_test(void)
{
  puts("++++----\nTest of netwib_conf_devices_index_init");
  puts("Redundant with netwib_conf_devices_index_next");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_conf_devices_index_close_test(void)
{
  puts("++++----\nTest of netwib_conf_devices_index_close");
  puts("Redundant with netwib_conf_devices_index_next");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_conf_devices_index_next_test(void)
{
  netwib_conf_devices conf;
  netwib_conf_devices_index *pconfindex;
  netwib_err ret;

  puts("++++----\nTest of netwib_conf_devices_index_next");

  netwib_er(netwib_conf_devices_index_init(&conf, &pconfindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_conf_devices_index_next(pconfindex);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_er(netwib_fmt_display("device=%{buf} eth=%{eth}\n", &conf.device,
                                 &conf.eth));
  }
  netwib_er(netwib_conf_devices_index_close(&pconfindex));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_conf_devices_display_test(void)
{
  puts("++++----\nTest of netwib_conf_devices_display");
  netwib_er(netwib_conf_devices_display());
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_conf_devices_test(void)
{
  puts("++++----\nTest of netwib_buf_append_conf_devices");
  puts("Redundant with netwib_conf_devices_display");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_net_confdev(void)
{
  puts("###########################################\nTest of net/confdev");
  netwib_er(netwib_conf_devices_index_init_test());
  netwib_er(netwib_conf_devices_index_close_test());
  netwib_er(netwib_conf_devices_index_next_test());
  netwib_er(netwib_conf_devices_display_test());
  netwib_er(netwib_buf_append_conf_devices_test());

  return(NETWIB_ERR_OK);
}
