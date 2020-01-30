#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_tcpopt_show_test2(netwib_consttcpopt *ptcpopt,
                                           netwib_encodetype encodetype,
                                           netwib_conststring wantmixed)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_tcpopt_show(ptcpopt, encodetype, &buf));
  netwib_er(netwib_fmt_display("%{buf}\n", &buf));
  netwib_er(netwibtest_buf_content3(&buf, wantmixed));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_tcpopt_show_test(void)
{
  netwib_tcpopt tcpopt;

  puts("++++----\nTest of netwib_tcpopt_show");

  /**/
  tcpopt.type = NETWIB_TCPOPTTYPE_END;
  netwib_er(netwib_tcpopt_show_test2(&tcpopt, NETWIB_ENCODETYPE_SYNTH,
                                     "'end'"));
  netwib_er(netwib_tcpopt_show_test2(&tcpopt, NETWIB_ENCODETYPE_ARRAY,
                                     "'| end                                                           |' 0A"));

  /**/
  tcpopt.type = NETWIB_TCPOPTTYPE_NOOP;
  netwib_er(netwib_tcpopt_show_test2(&tcpopt, NETWIB_ENCODETYPE_SYNTH,
                                     "'noop'"));
  netwib_er(netwib_tcpopt_show_test2(&tcpopt, NETWIB_ENCODETYPE_ARRAY,
                                     "'| noop                                                          |' 0A"));

  /**/
  tcpopt.type = NETWIB_TCPOPTTYPE_MSS;
  tcpopt.opt.mss.maxsegsize = 5;
  netwib_er(netwib_tcpopt_show_test2(&tcpopt, NETWIB_ENCODETYPE_SYNTH,
                                     "'mss'"));
  netwib_er(netwib_tcpopt_show_test2(&tcpopt, NETWIB_ENCODETYPE_ARRAY,
                                     " '| mss=5                                                         |' 0A"));

  /**/
  tcpopt.type = NETWIB_TCPOPTTYPE_WINDOWSCALE;
  tcpopt.opt.windowscale.windowscale = 2;
  netwib_er(netwib_tcpopt_show_test2(&tcpopt, NETWIB_ENCODETYPE_SYNTH,
                                     "'windowscale'"));
  netwib_er(netwib_tcpopt_show_test2(&tcpopt, NETWIB_ENCODETYPE_ARRAY,
                                     "'| windowscale=2                                                 |' 0A"));

  /**/
  tcpopt.type = NETWIB_TCPOPTTYPE_SACKPERMITTED;
  netwib_er(netwib_tcpopt_show_test2(&tcpopt, NETWIB_ENCODETYPE_SYNTH,
                                     "'sackpermitted'"));
  netwib_er(netwib_tcpopt_show_test2(&tcpopt, NETWIB_ENCODETYPE_ARRAY,
                                     "'| sackpermitted                                                 |' 0A"));

  /**/
  tcpopt.type = NETWIB_TCPOPTTYPE_SACK;
  tcpopt.opt.sack.storedvalues = 2;
  tcpopt.opt.sack.leftedge[0] = 1;
  tcpopt.opt.sack.rightedge[0] = 2;
  tcpopt.opt.sack.leftedge[1] = 3;
  tcpopt.opt.sack.rightedge[1] = 4;
  netwib_er(netwib_tcpopt_show_test2(&tcpopt, NETWIB_ENCODETYPE_SYNTH,
                                     "'sack'"));
  netwib_er(netwib_tcpopt_show_test2(&tcpopt, NETWIB_ENCODETYPE_ARRAY,
                                     "'| sack (storedvalues=2) :                                       |' 0A '|   1 -> 2                                                      |' 0A '|   3 -> 4                                                      |' 0A"));

  /**/
  tcpopt.type = NETWIB_TCPOPTTYPE_ECHOREQUEST;
  tcpopt.opt.echo.data = 4;
  netwib_er(netwib_tcpopt_show_test2(&tcpopt, NETWIB_ENCODETYPE_SYNTH,
                                     "'echorequest'"));
  netwib_er(netwib_tcpopt_show_test2(&tcpopt, NETWIB_ENCODETYPE_ARRAY,
                                     "'| echorequest : data=4                                          |' 0A"));

  /**/
  tcpopt.type = NETWIB_TCPOPTTYPE_ECHOREPLY;
  tcpopt.opt.echo.data = 4;
  netwib_er(netwib_tcpopt_show_test2(&tcpopt, NETWIB_ENCODETYPE_SYNTH,
                                     "'echoreply'"));
  netwib_er(netwib_tcpopt_show_test2(&tcpopt, NETWIB_ENCODETYPE_ARRAY,
                                     "'| echoreply : data=4                                            |' 0A"));

  /**/
  tcpopt.type = NETWIB_TCPOPTTYPE_TIMESTAMP;
  tcpopt.opt.timestamp.val = 2;
  tcpopt.opt.timestamp.echoreply = 3;
  netwib_er(netwib_tcpopt_show_test2(&tcpopt, NETWIB_ENCODETYPE_SYNTH,
                                     "'timestamp'"));
  netwib_er(netwib_tcpopt_show_test2(&tcpopt, NETWIB_ENCODETYPE_ARRAY,
                                     "'| timestamp : val=2 echoreply=3                                 |' 0A"));

  /**/
  tcpopt.type = NETWIB_TCPOPTTYPE_CC;
  tcpopt.opt.cc.connectioncount = 3;
  netwib_er(netwib_tcpopt_show_test2(&tcpopt, NETWIB_ENCODETYPE_SYNTH,
                                     "'cc'"));
  netwib_er(netwib_tcpopt_show_test2(&tcpopt, NETWIB_ENCODETYPE_ARRAY,
                                     "'| cc : connectioncount=3                                        |' 0A"));

  /**/
  tcpopt.type = NETWIB_TCPOPTTYPE_CCNEW;
  tcpopt.opt.cc.connectioncount = 3;
  netwib_er(netwib_tcpopt_show_test2(&tcpopt, NETWIB_ENCODETYPE_SYNTH,
                                     "'ccnew'"));
  netwib_er(netwib_tcpopt_show_test2(&tcpopt, NETWIB_ENCODETYPE_ARRAY,
                                     "'| ccnew : connectioncount=3                                     |' 0A"));

  /**/
  tcpopt.type = NETWIB_TCPOPTTYPE_CCECHO;
  tcpopt.opt.cc.connectioncount = 3;
  netwib_er(netwib_tcpopt_show_test2(&tcpopt, NETWIB_ENCODETYPE_SYNTH,
                                     "'ccecho'"));
  netwib_er(netwib_tcpopt_show_test2(&tcpopt, NETWIB_ENCODETYPE_ARRAY,
                                     "'| ccecho : connectioncount=3                                    |' 0A"));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_shw_tcpopt(void)
{
  puts("###########################################\nTest of shw/tcpopt");
  netwib_er(netwib_tcpopt_show_test());

  return(NETWIB_ERR_OK);
}

