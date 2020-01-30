#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err test_debug_memcorrupt(void)
{
  netwib_data data;

  puts("++++----\nTest of netwib_debug_memcorrupt");
  netwib_er(netwib_ptr_malloc(3, (netwib_ptr*)&data));
  netwib_er(netwib_fmt_display("%p\n", data));
  data[2] = 0;
  netwib_er(netwib_ptr_free((netwib_ptr*)&data));

  netwib_er(netwib_ptr_malloc(4, (netwib_ptr*)&data));
  netwib_er(netwib_fmt_display("%p\n", data));
  data[3] = 0;
  netwib_er(netwib_ptr_free((netwib_ptr*)&data));

  netwib_er(netwib_ptr_malloc(5, (netwib_ptr*)&data));
  netwib_er(netwib_fmt_display("%p\n", data));
  data[4] = 0;
  netwib_er(netwib_ptr_free((netwib_ptr*)&data));

  netwib_er(netwib_ptr_malloc(6, (netwib_ptr*)&data));
  netwib_er(netwib_fmt_display("%p\n", data));
  data[5] = 0;
  netwib_er(netwib_ptr_free((netwib_ptr*)&data));

  netwib_er(netwib_ptr_malloc(7, (netwib_ptr*)&data));
  netwib_er(netwib_fmt_display("%p\n", data));
  data[6] = 0;
  netwib_er(netwib_ptr_free((netwib_ptr*)&data));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_debug(void)
{
  puts("###########################################\nTest of sys/debug");
  netwib_er(test_debug_memcorrupt());

  return(NETWIB_ERR_OK);
}
