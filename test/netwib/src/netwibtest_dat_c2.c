#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err test__netwib_c2_uint(void)
{
  char a;
  a = (char)(unsigned char)0xDA;

  puts("++++----\nTest of function netwib_c2_uint");

  netwib_er(netwib_fmt_display("%{uint16:04X}\n", netwib_c2_uint16_2(a,a)));
  netwib_er(netwib_fmt_display("%{uint32:08X}\n", netwib_c2_uint32_4(a,a,a,a)));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_dat_c2(void)
{
  puts("###########################################\nTest of dat/c2");
  netwib_er(test__netwib_c2_uint());

  return(NETWIB_ERR_OK);
}
