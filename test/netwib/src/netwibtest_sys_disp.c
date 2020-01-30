#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_display_test(void)
{
  netwib_buf buf;

  puts("++++----\nTest of netwib_buf_display");
  netwib_er(netwib_buf_init_ext_string("Hello", &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_fmt_display_test(void)
{
  puts("++++----\nTest of netwib_fmt_display");
  netwib_er(netwib_fmt_display("\n"));
  netwib_er(netwib_fmt_display("%s %{uint32}\n", "hello", 123));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_disp(void)
{
  puts("###########################################\nTest of sys/disp");
  netwib_er(netwib_buf_display_test());
  netwib_er(netwib_fmt_display_test());

  return(NETWIB_ERR_OK);
}
