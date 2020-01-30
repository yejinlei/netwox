#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_tcphdr_show_test2(netwib_consttcphdr *ptcphdr,
                                           netwib_encodetype encodetype,
                                           netwib_conststring wantmixed)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_tcphdr_show(ptcphdr, encodetype, &buf));
  netwib_er(netwib_fmt_display("%{buf}\n", &buf));
  netwib_er(netwibtest_buf_content3(&buf, wantmixed));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_tcphdr_show_test(void)
{
  netwib_tcphdr tcpheader;
  netwib_buf buftext;
  netwib_string text;

  puts("++++----\nTest of netwib_tcphdr_show");
  netwib_er(netwib_buf_init_mallocdefault(&buftext));

  netwib_er(netwibtest_tcphdr_init(&tcpheader));
  netwib_er(netwib_tcphdr_show_test2(&tcpheader, NETWIB_ENCODETYPE_SYNTH,
                                    "'tcp1->2:afre'"));
  netwib_er(netwib_buf_append_string("'TCP_____________________________________________________________.' 0A '|          source port          |       destination port        |' 0A '|___________0x0001=1____________|___________0x0002=2____________|' 0A '|                            seqnum                             |' 0A '|_____________________0x11223344=287454020______________________|' 0A '|                            ack", &buftext));
  netwib_er(netwib_buf_append_string("num                             |' 0A '|_________________________0x00000000=0__________________________|' 0A '| doff  |r|r|r|r|C|E|U|A|P|R|S|F|            window             |' 0A '|___5___|1|0|1|0|0|1|0|1|0|1|0|1|_________0x3434=13364__________|' 0A '|           checksum            |            urgptr             |' 0A '|___________0x0000=0____________|__________0x1212=4626__________|' 0A", &buftext));
  netwib_er(netwib_buf_ref_string(&buftext, &text));
  netwib_er(netwib_tcphdr_show_test2(&tcpheader, NETWIB_ENCODETYPE_ARRAY,
                                     text));

  netwib_er(netwibtest_tcphdr_init(&tcpheader));
  netwib_er(netwib_buf_init_ext_string("abcd", &tcpheader.opts));
  netwib_er(netwib_tcphdr_show_test2(&tcpheader, NETWIB_ENCODETYPE_SYNTH,
                                    "'tcp1->2:afre - tcpopts'"));
  netwib__buf_reinit(&buftext);
  netwib_er(netwib_buf_append_string("'TCP_____________________________________________________________.' 0A '|          source port          |       destination port        |' 0A '|___________0x0001=1____________|___________0x0002=2____________|' 0A '|                            seqnum                             |' 0A '|_____________________0x11223344=287", &buftext));
  netwib_er(netwib_buf_append_string("454020______________________|' 0A '|                            acknum                             |' 0A '|_________________________0x00000000=0__________________________|' 0A '| doff  |r|r|r|r|C|E|U|A|P|R|S|F|            window             |' 0A '|___5___|1|0|1|0|0|1|0|1|0|1|0|1|_________0x3434=13364__________|' 0A '", &buftext));
  netwib_er(netwib_buf_append_string("|           checksum            |            urgptr             |' 0A '|___________0x0000=0____________|__________0x1212=4626__________|' 0A 'TCPOPTS_________________________________________________________.' 0A '| undecoded option: 61626364                                    |' 0A '|_______________________________________________________________|' 0A", &buftext));
  netwib_er(netwib_buf_ref_string(&buftext, &text));
  netwib_er(netwib_tcphdr_show_test2(&tcpheader, NETWIB_ENCODETYPE_ARRAY,
                                     text));

  netwib_er(netwib_buf_close(&buftext));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_shw_tcphdr(void)
{
  puts("###########################################\nTest of shw/tcphdr");
  netwib_er(netwib_tcphdr_show_test());

  return(NETWIB_ERR_OK);
}
