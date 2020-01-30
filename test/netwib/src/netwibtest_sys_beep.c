#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_beep_test(void)
{
  puts("++++----\nTest of netwib_beep");
  puts("Now, you should hear a BEEP");
  netwib_er(netwib_beep());

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_beep(void)
{
  puts("###########################################\nTest of sys/beep");
  netwib_er(netwib_beep_test());

  return(NETWIB_ERR_OK);
}
