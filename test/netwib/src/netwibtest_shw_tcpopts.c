#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_tcpopts_show_test2(netwib_constbuf *ptcpopts,
                                            netwib_encodetype encodetype,
                                            netwib_conststring wantmixed)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_tcpopts_show(ptcpopts, encodetype, &buf));
  netwib_er(netwib_fmt_display("%{buf}\n", &buf));
  netwib_er(netwibtest_buf_content3(&buf, wantmixed));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_tcpopts_show_test(void)
{
  netwib_buf tcpopts;
  netwib_tcpopt tcpopt;

  puts("++++----\nTest of netwib_tcpopts_show");

  netwib_er(netwib_buf_init_ext_string("abc", &tcpopts));

  netwib_er(netwib_tcpopts_show_test2(&tcpopts, NETWIB_ENCODETYPE_SYNTH, "'tcpopts'"));
  netwib_er(netwib_tcpopts_show_test2(&tcpopts, NETWIB_ENCODETYPE_ARRAY, "'TCPOPTS_________________________________________________________.' 0A '| undecoded option: 616263                                      |' 0A '|_______________________________________________________________|' 0A"));

  netwib_er(netwib_buf_init_mallocdefault(&tcpopts));
  tcpopt.type = NETWIB_TCPOPTTYPE_TIMESTAMP;
  tcpopt.opt.timestamp.val = 2;
  tcpopt.opt.timestamp.echoreply = 3;
  netwib_er(netwib_pkt_append_tcpopt(&tcpopt, &tcpopts));
  tcpopt.type = NETWIB_TCPOPTTYPE_MSS;
  tcpopt.opt.mss.maxsegsize = 5;
  netwib_er(netwib_pkt_append_tcpopt(&tcpopt, &tcpopts));
  tcpopt.type = NETWIB_TCPOPTTYPE_SACKPERMITTED;
  netwib_er(netwib_pkt_append_tcpopt(&tcpopt, &tcpopts));
  netwib_er(netwib_tcpopts_show_test2(&tcpopts, NETWIB_ENCODETYPE_ARRAY, "'TCPOPTS_________________________________________________________.' 0A '| timestamp : val=2 echoreply=3                                 |' 0A '| mss=5                                                         |' 0A '| sackpermitted                                                 |' 0A '|_______________________________________________________________|' 0A"));
  netwib_er(netwib_buf_close(&tcpopts));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_shw_tcpopts(void)
{
  puts("###########################################\nTest of shw/tcpopts");
  netwib_er(netwib_tcpopts_show_test());

  return(NETWIB_ERR_OK);
}
