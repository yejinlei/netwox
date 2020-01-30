#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_tcpopt_initdefault_test(void)
{
  netwib_tcpopt tcpopt;

  puts("++++----\nTest of netwib_tcpopt_initdefault");

  netwib_er(netwib_tcpopt_initdefault(NETWIB_TCPOPTTYPE_END, &tcpopt));
  netwib_er(netwibtest_tcpopt_content(&tcpopt, "00"));

  netwib_er(netwib_tcpopt_initdefault(NETWIB_TCPOPTTYPE_NOOP, &tcpopt));
  netwib_er(netwibtest_tcpopt_content(&tcpopt, "01"));

  netwib_er(netwib_tcpopt_initdefault(NETWIB_TCPOPTTYPE_MSS, &tcpopt));
  netwib_er(netwibtest_tcpopt_content(&tcpopt, "02040000"));

  netwib_er(netwib_tcpopt_initdefault(NETWIB_TCPOPTTYPE_WINDOWSCALE, &tcpopt));
  netwib_er(netwibtest_tcpopt_content(&tcpopt, "030301"));

  netwib_er(netwib_tcpopt_initdefault(NETWIB_TCPOPTTYPE_SACKPERMITTED,
                                      &tcpopt));
  netwib_er(netwibtest_tcpopt_content(&tcpopt, "0402"));

  netwib_er(netwib_tcpopt_initdefault(NETWIB_TCPOPTTYPE_SACK, &tcpopt));
  netwib_er(netwibtest_tcpopt_content(&tcpopt, "0502"));

  netwib_er(netwib_tcpopt_initdefault(NETWIB_TCPOPTTYPE_ECHOREQUEST, &tcpopt));
  tcpopt.opt.echo.data = 0x12345678;
  netwib_er(netwibtest_tcpopt_content(&tcpopt, "060612345678"));

  netwib_er(netwib_tcpopt_initdefault(NETWIB_TCPOPTTYPE_ECHOREPLY, &tcpopt));
  netwib_er(netwibtest_tcpopt_content(&tcpopt, "070600000000"));

  netwib_er(netwib_tcpopt_initdefault(NETWIB_TCPOPTTYPE_CC, &tcpopt));
  netwib_er(netwibtest_tcpopt_content(&tcpopt, "0B0600000000"));

  netwib_er(netwib_tcpopt_initdefault(NETWIB_TCPOPTTYPE_CCNEW, &tcpopt));
  netwib_er(netwibtest_tcpopt_content(&tcpopt, "0C0600000000"));

  netwib_er(netwib_tcpopt_initdefault(NETWIB_TCPOPTTYPE_CCECHO, &tcpopt));
  netwib_er(netwibtest_tcpopt_content(&tcpopt, "0D0600000000"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_tcpopt_test2(netwib_consttcpopt *ptcpopt,
                                                 netwib_conststring mixed,
                                                 netwib_uint32 wantedskipsize)
{
  netwib_tcpopt tcpopt2;
  netwib_uint32 skipsize;
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_pkt_append_tcpopt(ptcpopt, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, mixed));

  netwib_er(netwib_pkt_decode_tcpopt(&buf, &tcpopt2, &skipsize));
  if (skipsize != wantedskipsize) {
    return(NETWIBTEST_ERR_4);
  }
  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_prepend_tcpopt(&tcpopt2, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, mixed));

  netwib_er(netwib_buf_close(&buf));
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_pkt_append_tcpopt_test(void)
{
  netwib_tcpopt tcpopt;

  puts("++++----\nTest of netwib_pkt_append_tcpopt");

  /**/
  tcpopt.type = NETWIB_TCPOPTTYPE_END;
  netwib_er(netwib_pkt_append_tcpopt_test2(&tcpopt, "00", 1));

  /**/
  tcpopt.type = NETWIB_TCPOPTTYPE_NOOP;
  netwib_er(netwib_pkt_append_tcpopt_test2(&tcpopt, "01", 1));

  /**/
  tcpopt.type = NETWIB_TCPOPTTYPE_MSS;
  tcpopt.opt.mss.maxsegsize = 5;
  netwib_er(netwib_pkt_append_tcpopt_test2(&tcpopt, "02040005", 4));

  /**/
  tcpopt.type = NETWIB_TCPOPTTYPE_WINDOWSCALE;
  tcpopt.opt.windowscale.windowscale = 2;
  netwib_er(netwib_pkt_append_tcpopt_test2(&tcpopt, "030302", 3));

  /**/
  tcpopt.type = NETWIB_TCPOPTTYPE_SACKPERMITTED;
  netwib_er(netwib_pkt_append_tcpopt_test2(&tcpopt, "0402", 2));

  /**/
  tcpopt.type = NETWIB_TCPOPTTYPE_SACK;
  tcpopt.opt.sack.storedvalues = 2;
  tcpopt.opt.sack.leftedge[0] = 1;
  tcpopt.opt.sack.rightedge[0] = 2;
  tcpopt.opt.sack.leftedge[1] = 3;
  tcpopt.opt.sack.rightedge[1] = 4;
  netwib_er(netwib_pkt_append_tcpopt_test2(&tcpopt, "051200000001000000020000000300000004", 18));

  /**/
  tcpopt.type = NETWIB_TCPOPTTYPE_ECHOREQUEST;
  tcpopt.opt.echo.data = 4;
  netwib_er(netwib_pkt_append_tcpopt_test2(&tcpopt, "060600000004", 6));

  /**/
  tcpopt.type = NETWIB_TCPOPTTYPE_ECHOREPLY;
  tcpopt.opt.echo.data = 4;
  netwib_er(netwib_pkt_append_tcpopt_test2(&tcpopt, "070600000004", 6));

  /**/
  tcpopt.type = NETWIB_TCPOPTTYPE_TIMESTAMP;
  tcpopt.opt.timestamp.val = 2;
  tcpopt.opt.timestamp.echoreply = 3;
  netwib_er(netwib_pkt_append_tcpopt_test2(&tcpopt, "080A0000000200000003", 10));

  /**/
  tcpopt.type = NETWIB_TCPOPTTYPE_CC;
  tcpopt.opt.cc.connectioncount = 3;
  netwib_er(netwib_pkt_append_tcpopt_test2(&tcpopt, "0B0600000003", 6));

  /**/
  tcpopt.type = NETWIB_TCPOPTTYPE_CCNEW;
  tcpopt.opt.cc.connectioncount = 3;
  netwib_er(netwib_pkt_append_tcpopt_test2(&tcpopt, "0C0600000003", 6));

  /**/
  tcpopt.type = NETWIB_TCPOPTTYPE_CCECHO;
  tcpopt.opt.cc.connectioncount = 3;
  netwib_er(netwib_pkt_append_tcpopt_test2(&tcpopt, "0D0600000003", 6));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_prepend_tcpopt_test(void)
{
  puts("++++----\nTest of netwib_pkt_prepend_tcpopt");
  puts("Redundant with netwib_pkt_append_tcpopt");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_tcpopt_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_tcpopt");
  puts("Redundant with netwib_pkt_append_tcpopt");
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_pkt_tcpopt(void)
{
  puts("###########################################\nTest of pkt/tcpopt");
  netwib_er(netwib_tcpopt_initdefault_test());
  netwib_er(netwib_pkt_append_tcpopt_test());
  netwib_er(netwib_pkt_prepend_tcpopt_test());
  netwib_er(netwib_pkt_decode_tcpopt_test());

  return(NETWIB_ERR_OK);
}
