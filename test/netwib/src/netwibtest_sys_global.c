#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_global_ctl_set_test(void)
{
  puts("++++----\nTest of netwib_global_ctl_set");
  netwib_er(netwib_global_ctl_set_err_purge());

  netwib_er(netwib_global_ctl_set_conf_update());

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_global_ctl_get_test(void)
{
  netwib_bool pressed;

  puts("++++----\nTest of netwib_global_ctl_get");
  netwib_er(netwib_global_ctl_get_debug_ctrlc_pressed(&pressed));
  if (pressed) {
    puts("Control-C PRESSED");
  } else {
    puts("Control-C NOT PRESSED");
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_global(void)
{
  puts("###########################################\nTest of sys/global");
  netwib_er(netwib_global_ctl_set_test());
  netwib_er(netwib_global_ctl_get_test());

  return(NETWIB_ERR_OK);
}
