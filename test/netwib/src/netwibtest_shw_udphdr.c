#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_udphdr_show_test2(netwib_constudphdr *pudphdr,
                                           netwib_encodetype encodetype,
                                           netwib_conststring wantmixed)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_udphdr_show(pudphdr, encodetype, &buf));
  netwib_er(netwib_fmt_display("%{buf}\n", &buf));
  netwib_er(netwibtest_buf_content3(&buf, wantmixed));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_udphdr_show_test(void)
{
  netwib_udphdr udpheader;

  puts("++++----\nTest of netwib_udphdr_show");

  netwib_er(netwibtest_udphdr_init(&udpheader));
  netwib_er(netwib_udphdr_show_test2(&udpheader, NETWIB_ENCODETYPE_SYNTH,
                                    "'udp1->2'"));
  netwib_er(netwib_udphdr_show_test2(&udpheader, NETWIB_ENCODETYPE_ARRAY,
                                    "'UDP_____________________________________________________________.' 0A '|          source port          |       destination port        |' 0A '|___________0x0001=1____________|___________0x0002=2____________|' 0A '|            length             |           checksum            |' 0A '|___________0x0008=8____________|___________0x0000=0____________|' 0A"));
  netwib_er(netwib_udphdr_show_test2(&udpheader, NETWIB_ENCODETYPE_DUMP,
                                    "'00 01 00 02  00 08 00 00                            # ........' 0A"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_shw_udphdr(void)
{
  puts("###########################################\nTest of shw/udphdr");
  netwib_er(netwib_udphdr_show_test());

  return(NETWIB_ERR_OK);
}
