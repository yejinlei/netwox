#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_iphdr_show_test2(netwib_constiphdr *piphdr,
                                          netwib_encodetype encodetype,
                                          netwib_conststring wantmixed)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_iphdr_show(piphdr, encodetype, &buf));
  netwib_er(netwib_fmt_display("%{buf}\n", &buf));
  netwib_er(netwibtest_buf_content3(&buf, wantmixed));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_iphdr_show_test(void)
{
  netwib_iphdr ipheader;
  netwib_buf buftext;
  netwib_string text;

  puts("++++----\nTest of netwib_iphdr_show");
  netwib_er(netwib_buf_init_mallocdefault(&buftext));

  netwib_er(netwibtest_ip4hdr_init(&ipheader));
  netwib_er(netwib_iphdr_show_test2(&ipheader, NETWIB_ENCODETYPE_SYNTH,
                                    "'17.34.51.68->85.102.119.136'"));
  netwib_er(netwib_buf_append_string("'IP______________________________________________________________.' 0A '|version|  ihl  |      tos      |            totlen             |' 0A '|___4___|___5___|____0x12=18____|___________0x0014=20___________|' 0A '|              id               |r|D|M|       offsetfrag        |' 0A '|_________0xFADE=64222__________|0|0|0|________0x0000=0_________|' 0A '|      ttl      |   protocol    |           check", &buftext));
  netwib_er(netwib_buf_append_string("sum            |' 0A '|___0x80=128____|____0x06=6_____|____________0x1234_____________|' 0A '|                            source                             |' 0A '|__________________________17.34.51.68__________________________|' 0A '|                          destination                          |' 0A '|________________________85.102.119.136_________________________|' 0A", &buftext));
  netwib_er(netwib_buf_ref_string(&buftext, &text));
  netwib_er(netwib_iphdr_show_test2(&ipheader, NETWIB_ENCODETYPE_ARRAY, text));
  netwib_er(netwib_iphdr_show_test2(&ipheader, NETWIB_ENCODETYPE_DUMP, "343520313220303020313420206661206465203030203030202038302030362031322033342020313120323220333320343420202320452E2E2E2E2E2E2E2E2E2E342E2233440A3535203636203737203838202020202020202020202020202020202020202020202020202020202020202020202020202020202023205566772E0A"));

  netwib_er(netwibtest_ip6hdr_init(&ipheader));
  netwib_er(netwib_iphdr_show_test2(&ipheader, NETWIB_ENCODETYPE_SYNTH,
                                    "'1122:3344:0:5:0:6:0:7->5566:7788:0:8:0:9::'"));
  netwib__buf_reinit(&buftext);
  netwib_er(netwib_buf_append_string("'IP______________________________________________________________.' 0A '|version| traffic class |              flow label               |' 0A '|___6___|______129______|_________________1234__________________|' 0A '|        payload length         |  next header  |   hop limit   |' 0A '|___________0x0022=34___________|", &buftext));
  netwib_er(netwib_buf_append_string("____0x06=6_____|______65_______|' 0A '|                            source                             |' 0A '|_____________________1122:3344:0:5:0:6:0:7_____________________|' 0A '|                          destination                          |' 0A '|______________________5566:7788:0:8:0:9::______________________|' 0A", &buftext));
  netwib_er(netwib_buf_ref_string(&buftext, &text));
  netwib_er(netwib_iphdr_show_test2(&ipheader, NETWIB_ENCODETYPE_ARRAY, text));
  netwib_er(netwib_iphdr_show_test2(&ipheader, NETWIB_ENCODETYPE_DUMP, "363820313020303420643220203030203232203036203431202031312032322033332034342020303020303020303020303520202320682E2E2E2E222E412E2233442E2E2E2E0A3030203030203030203036202030302030302030302030372020353520363620373720383820203030203030203030203038202023202E2E2E2E2E2E2E2E5566772E2E2E2E2E0A3030203030203030203039202030302030302030302030302020202020202020202020202020202020202020202020202020202023202E2E2E2E2E2E2E2E0A"));

  netwib_er(netwib_buf_close(&buftext));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_shw_iphdr(void)
{
  puts("###########################################\nTest of shw/iphdr");
  netwib_er(netwib_iphdr_show_test());

  return(NETWIB_ERR_OK);
}
