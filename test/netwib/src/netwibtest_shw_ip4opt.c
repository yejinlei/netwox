#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_ip4opt_show_test2(netwib_constip4opt *pip4opt,
                                           netwib_encodetype encodetype,
                                           netwib_conststring wantmixed)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_ip4opt_show(pip4opt, encodetype, &buf));
  netwib_er(netwib_fmt_display("%{buf}\n", &buf));
  netwib_er(netwibtest_buf_content3(&buf, wantmixed));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_ip4opt_show_test(void)
{
  netwib_ip4opt ip4opt;

  puts("++++----\nTest of netwib_ip4opt_show");

  /**/
  ip4opt.type = NETWIB_IP4OPTTYPE_END;
  netwib_er(netwib_ip4opt_show_test2(&ip4opt, NETWIB_ENCODETYPE_SYNTH,
                                     "'end'"));
  netwib_er(netwib_ip4opt_show_test2(&ip4opt, NETWIB_ENCODETYPE_ARRAY,
                                     "'| end                                                           |' 0A"));

  /**/
  ip4opt.type = NETWIB_IP4OPTTYPE_NOOP;
  netwib_er(netwib_ip4opt_show_test2(&ip4opt, NETWIB_ENCODETYPE_SYNTH,
                                     "'noop'"));
  netwib_er(netwib_ip4opt_show_test2(&ip4opt, NETWIB_ENCODETYPE_ARRAY,
                                     "'| noop                                                          |' 0A"));

  /**/
  ip4opt.type = NETWIB_IP4OPTTYPE_RR;
  ip4opt.opt.rr.storagesize = 5;
  ip4opt.opt.rr.storedvalues = 3;
  netwib_er(netwib_ip_init_ip4(0x12345678, &ip4opt.opt.rr.ip[0]));
  netwib_er(netwib_ip_init_ip4(0x22345678, &ip4opt.opt.rr.ip[1]));
  netwib_er(netwib_ip_init_ip4(0x32345678, &ip4opt.opt.rr.ip[2]));
  netwib_er(netwib_ip4opt_show_test2(&ip4opt, NETWIB_ENCODETYPE_SYNTH,
                                     "'rr'"));
  netwib_er(netwib_ip4opt_show_test2(&ip4opt, NETWIB_ENCODETYPE_ARRAY,
                                     "'| rr (storagesize=5 storedvalues=3) :                           |' 0A '|   18.52.86.120                                                |' 0A '|   34.52.86.120                                                |' 0A '|   50.52.86.120                                                |' 0A"));

  /**/
  ip4opt.type = NETWIB_IP4OPTTYPE_LSRR;
  ip4opt.opt.lsrr.storagesize = 5;
  ip4opt.opt.lsrr.usedvalues = 3;
  netwib_er(netwib_ip_init_ip4(0x12345678, &ip4opt.opt.lsrr.ip[0]));
  netwib_er(netwib_ip_init_ip4(0x22345678, &ip4opt.opt.lsrr.ip[1]));
  netwib_er(netwib_ip_init_ip4(0x32345678, &ip4opt.opt.lsrr.ip[2]));
  netwib_er(netwib_ip_init_ip4(0x42345678, &ip4opt.opt.lsrr.ip[3]));
  netwib_er(netwib_ip_init_ip4(0x52345678, &ip4opt.opt.lsrr.ip[4]));
  netwib_er(netwib_ip4opt_show_test2(&ip4opt, NETWIB_ENCODETYPE_SYNTH,
                                     "'lsrr'"));
  netwib_er(netwib_ip4opt_show_test2(&ip4opt, NETWIB_ENCODETYPE_ARRAY,
                                     "'| lsrr (storagesize=5 usedvalues=3) :                           |' 0A '|   18.52.86.120                                                |' 0A '|   34.52.86.120                                                |' 0A '|   50.52.86.120                                                |' 0A '|   66.52.86.120                                                |' 0A '|   82.52.86.120                                                |' 0A"));

  /**/
  ip4opt.type = NETWIB_IP4OPTTYPE_SSRR;
  ip4opt.opt.ssrr.storagesize = 5;
  ip4opt.opt.ssrr.usedvalues = 3;
  netwib_er(netwib_ip_init_ip4(0x12345678, &ip4opt.opt.ssrr.ip[0]));
  netwib_er(netwib_ip_init_ip4(0x22345678, &ip4opt.opt.ssrr.ip[1]));
  netwib_er(netwib_ip_init_ip4(0x32345678, &ip4opt.opt.ssrr.ip[2]));
  netwib_er(netwib_ip_init_ip4(0x42345678, &ip4opt.opt.ssrr.ip[3]));
  netwib_er(netwib_ip_init_ip4(0x52345678, &ip4opt.opt.ssrr.ip[4]));
  netwib_er(netwib_ip4opt_show_test2(&ip4opt, NETWIB_ENCODETYPE_SYNTH,
                                     "'ssrr'"));
  netwib_er(netwib_ip4opt_show_test2(&ip4opt, NETWIB_ENCODETYPE_ARRAY,
                                     "'| ssrr (storagesize=5 usedvalues=3) :                           |' 0A '|   18.52.86.120                                                |' 0A '|   34.52.86.120                                                |' 0A '|   50.52.86.120                                                |' 0A '|   66.52.86.120                                                |' 0A '|   82.52.86.120                                                |' 0A"));

  /**/
  ip4opt.type = NETWIB_IP4OPTTYPE_TIME;
  ip4opt.opt.time.flag = NETWIB_IP4OPT_TIMEFLAG_TS;
  ip4opt.opt.time.overflow = 2;
  ip4opt.opt.time.storagesize = 5;
  ip4opt.opt.time.storedvalues = 3;
  ip4opt.opt.time.timestamp[0] = 0x12233445;
  ip4opt.opt.time.timestamp[1] = 0x22233445;
  ip4opt.opt.time.timestamp[2] = 0x32233445;
  netwib_er(netwib_ip4opt_show_test2(&ip4opt, NETWIB_ENCODETYPE_SYNTH,
                                     "'time'"));
  netwib_er(netwib_ip4opt_show_test2(&ip4opt, NETWIB_ENCODETYPE_ARRAY,
                                     "'| time (storagesize=5 storedvalues=3 flag=0 oflw=2) :           |' 0A '|   304297029                                                   |' 0A '|   572732485                                                   |' 0A '|   841167941                                                   |' 0A"));

  /**/
  ip4opt.type = NETWIB_IP4OPTTYPE_TIME;
  ip4opt.opt.time.flag = NETWIB_IP4OPT_TIMEFLAG_IPTS;
  ip4opt.opt.time.overflow = 2;
  ip4opt.opt.time.storagesize = 4;
  ip4opt.opt.time.storedvalues = 2;
  ip4opt.opt.time.timestamp[0] = 0x12233445;
  ip4opt.opt.time.timestamp[1] = 0x22233445;
  netwib_er(netwib_ip_init_ip4(0x12345678, &ip4opt.opt.time.ip[0]));
  netwib_er(netwib_ip_init_ip4(0x22345678, &ip4opt.opt.time.ip[1]));
  netwib_er(netwib_ip4opt_show_test2(&ip4opt, NETWIB_ENCODETYPE_SYNTH,
                                     "'time'"));
  netwib_er(netwib_ip4opt_show_test2(&ip4opt, NETWIB_ENCODETYPE_ARRAY,
                                     "'| time (storagesize=4 storedvalues=2 flag=1 oflw=2) :           |' 0A '|   18.52.86.120 304297029                                      |' 0A '|   34.52.86.120 572732485                                      |' 0A"));

  /**/
  ip4opt.type = NETWIB_IP4OPTTYPE_TIME;
  ip4opt.opt.time.flag = NETWIB_IP4OPT_TIMEFLAG_IPPTS;
  ip4opt.opt.time.overflow = 2;
  ip4opt.opt.time.storagesize = 4;
  ip4opt.opt.time.storedvalues = 2;
  ip4opt.opt.time.timestamp[0] = 0x12233445;
  ip4opt.opt.time.timestamp[1] = 0x22233445;
  netwib_er(netwib_ip_init_ip4(0x12345678, &ip4opt.opt.time.ip[0]));
  netwib_er(netwib_ip_init_ip4(0x22345678, &ip4opt.opt.time.ip[1]));
  netwib_er(netwib_ip_init_ip4(0x32345678, &ip4opt.opt.time.ip[2]));
  netwib_er(netwib_ip_init_ip4(0x42345678, &ip4opt.opt.time.ip[3]));
  netwib_er(netwib_ip4opt_show_test2(&ip4opt, NETWIB_ENCODETYPE_SYNTH,
                                     "'time'"));
  netwib_er(netwib_ip4opt_show_test2(&ip4opt, NETWIB_ENCODETYPE_ARRAY,
                                     "'| time (storagesize=4 storedvalues=2 flag=3 oflw=2) :           |' 0A '|   18.52.86.120 304297029                                      |' 0A '|   34.52.86.120 572732485                                      |' 0A '|   50.52.86.120                                                |' 0A '|   66.52.86.120                                                |' 0A"));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_shw_ip4opt(void)
{
  puts("###########################################\nTest of shw/ip4opt");
  netwib_er(netwib_ip4opt_show_test());

  return(NETWIB_ERR_OK);
}
