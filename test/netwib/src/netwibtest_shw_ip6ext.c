#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_ip6ext_show_test(void)
{
  puts("++++----\nTest of netwib_ip6ext_show");
  puts("Redundant with netwib_pkt_append_ip6ext");

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_shw_ip6ext(void)
{
  puts("###########################################\nTest of shw/ip6ext");
  netwib_er(netwib_ip6ext_show_test());

  return(NETWIB_ERR_OK);
}
