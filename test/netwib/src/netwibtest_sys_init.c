#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_init_test(void)
{
  puts("++++----\nTest of netwib_init");
#if NETWIB_DEBUG==0
  /* otherwise debugging action done in netwib_close are short-circuited */
  netwib_er(netwib_init());
  netwib_er(netwib_init());
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_close_test(void)
{
  puts("++++----\nTest of netwib_close");
#if NETWIB_DEBUG==0
  /* otherwise debugging action done in netwib_close are short-circuited */
  netwib_er(netwib_close());
  netwib_er(netwib_close());
  netwib_er(netwib_init());
  netwib_er(netwib_close());
  netwib_er(netwib_init());
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_init(void)
{
  puts("###########################################\nTest of sys/init");
  netwib_er(netwib_init_test());
  netwib_er(netwib_close_test());

  return(NETWIB_ERR_OK);
}
