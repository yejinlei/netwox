#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_arphdr_show_test(void)
{
  netwib_arphdr arpheader;
  netwib_buf buf;

  puts("++++----\nTest of netwib_arphdr_show");
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwibtest_arphdr_init(&arpheader));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_arphdr_show(&arpheader, NETWIB_ENCODETYPE_SYNTH, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_fmt_display("\n"));
  netwib_er(netwibtest_buf_content3(&buf, "'arpQ_17.34.51.68_asks_85.102.119.136'"));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_arphdr_show(&arpheader, NETWIB_ENCODETYPE_ARRAY, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwibtest_buf_content3(&buf, "'ARP Request_____________________________________________________.' 0A '| this address : 01:02:03:04:05:06 17.34.51.68                  |' 0A '| asks         : 00:00:00:00:00:00 85.102.119.136               |' 0A '|_______________________________________________________________|' 0A"));

  netwib_er(netwib_buf_close(&buf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_shw_arphdr(void)
{
  puts("###########################################\nTest of shw/arphdr");
  netwib_er(netwib_arphdr_show_test());

  return(NETWIB_ERR_OK);
}
