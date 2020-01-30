#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_conf_display_test(void)
{
  puts("++++----\nTest of netwib_conf_display");
  netwib_er(netwib_conf_display());
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_conf_test(void)
{
  puts("++++----\nTest of netwib_buf_append_conf");
  puts("Redundant with netwib_buf_append_conf_display");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_conf_debug_display_test(void)
{
  puts("++++----\nTest of netwib_conf_debug_display");
  netwib_er(netwib_conf_debug_display());
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_conf_debug_test(void)
{
  puts("++++----\nTest of netwib_buf_append_conf_debug");
  puts("Redundant with netwib_buf_append_conf_debug_display");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_net_conf(void)
{
  puts("###########################################\nTest of net/conf");
  netwib_er(netwib_conf_display_test());
  netwib_er(netwib_buf_append_conf_test());
  netwib_er(netwib_conf_debug_display_test());
  netwib_er(netwib_buf_append_conf_debug_test());

  return(NETWIB_ERR_OK);
}
