#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_ip4opt_initdefault_test(void)
{
  netwib_ip4opt ip4opt;

  puts("++++----\nTest of netwib_ip4opt_initdefault");

  netwib_er(netwib_ip4opt_initdefault(NETWIB_IP4OPTTYPE_RR, &ip4opt));
  netwib_er(netwibtest_ip4opt_content(&ip4opt, "070304"));

  netwib_er(netwib_ip4opt_initdefault(NETWIB_IP4OPTTYPE_TIME, &ip4opt));
  netwib_er(netwibtest_ip4opt_content(&ip4opt, "44040500"));

  netwib_er(netwib_ip4opt_initdefault(NETWIB_IP4OPTTYPE_LSRR, &ip4opt));
  netwib_er(netwibtest_ip4opt_content(&ip4opt, "830304"));

  netwib_er(netwib_ip4opt_initdefault(NETWIB_IP4OPTTYPE_SSRR, &ip4opt));
  netwib_er(netwibtest_ip4opt_content(&ip4opt, "890304"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_ip4opt_test2(netwib_constip4opt *pip4opt,
                                                 netwib_conststring mixed,
                                                 netwib_uint32 wantedskipsize)
{
  netwib_ip4opt ip4opt2;
  netwib_uint32 skipsize;
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_pkt_append_ip4opt(pip4opt, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, mixed));

  netwib_er(netwib_pkt_decode_ip4opt(&buf, &ip4opt2, &skipsize));
  if (skipsize != wantedskipsize) {
    return(NETWIBTEST_ERR_4);
  }
  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_prepend_ip4opt(&ip4opt2, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, mixed));

  netwib_er(netwib_buf_close(&buf));
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_pkt_append_ip4opt_test(void)
{
  netwib_ip4opt ip4opt;

  puts("++++----\nTest of netwib_pkt_append_ip4opt");

  /**/
  ip4opt.type = NETWIB_IP4OPTTYPE_END;
  netwib_er(netwib_pkt_append_ip4opt_test2(&ip4opt, "00", 1));

  /**/
  ip4opt.type = NETWIB_IP4OPTTYPE_NOOP;
  netwib_er(netwib_pkt_append_ip4opt_test2(&ip4opt, "01", 1));

  /**/
  ip4opt.type = NETWIB_IP4OPTTYPE_RR;
  ip4opt.opt.rr.storagesize = 5;
  ip4opt.opt.rr.storedvalues = 3;
  netwib_er(netwib_ip_init_ip4(0x12345678, &ip4opt.opt.rr.ip[0]));
  netwib_er(netwib_ip_init_ip4(0x22345678, &ip4opt.opt.rr.ip[1]));
  netwib_er(netwib_ip_init_ip4(0x32345678, &ip4opt.opt.rr.ip[2]));
  netwib_er(netwib_pkt_append_ip4opt_test2(&ip4opt, "071710 12345678 22345678 32345678 00000000 00000000", 23));

  /**/
  ip4opt.type = NETWIB_IP4OPTTYPE_LSRR;
  ip4opt.opt.lsrr.storagesize = 5;
  ip4opt.opt.lsrr.usedvalues = 3;
  netwib_er(netwib_ip_init_ip4(0x12345678, &ip4opt.opt.lsrr.ip[0]));
  netwib_er(netwib_ip_init_ip4(0x22345678, &ip4opt.opt.lsrr.ip[1]));
  netwib_er(netwib_ip_init_ip4(0x32345678, &ip4opt.opt.lsrr.ip[2]));
  netwib_er(netwib_ip_init_ip4(0x42345678, &ip4opt.opt.lsrr.ip[3]));
  netwib_er(netwib_ip_init_ip4(0x52345678, &ip4opt.opt.lsrr.ip[4]));
  netwib_er(netwib_pkt_append_ip4opt_test2(&ip4opt, "831710 12345678 22345678 32345678 42345678 52345678", 23));

  /**/
  ip4opt.type = NETWIB_IP4OPTTYPE_SSRR;
  ip4opt.opt.ssrr.storagesize = 5;
  ip4opt.opt.ssrr.usedvalues = 3;
  netwib_er(netwib_ip_init_ip4(0x12345678, &ip4opt.opt.ssrr.ip[0]));
  netwib_er(netwib_ip_init_ip4(0x22345678, &ip4opt.opt.ssrr.ip[1]));
  netwib_er(netwib_ip_init_ip4(0x32345678, &ip4opt.opt.ssrr.ip[2]));
  netwib_er(netwib_ip_init_ip4(0x42345678, &ip4opt.opt.ssrr.ip[3]));
  netwib_er(netwib_ip_init_ip4(0x52345678, &ip4opt.opt.ssrr.ip[4]));
  netwib_er(netwib_pkt_append_ip4opt_test2(&ip4opt, "891710 12345678 22345678 32345678 42345678 52345678", 23));

  /**/
  ip4opt.type = NETWIB_IP4OPTTYPE_TIME;
  ip4opt.opt.time.flag = NETWIB_IP4OPT_TIMEFLAG_TS;
  ip4opt.opt.time.overflow = 2;
  ip4opt.opt.time.storagesize = 5;
  ip4opt.opt.time.storedvalues = 3;
  ip4opt.opt.time.timestamp[0] = 0x12233445;
  ip4opt.opt.time.timestamp[1] = 0x22233445;
  ip4opt.opt.time.timestamp[2] = 0x32233445;
  netwib_er(netwib_pkt_append_ip4opt_test2(&ip4opt, "44181120 12233445 22233445 32233445 00000000 00000000", 24));

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
  netwib_er(netwib_pkt_append_ip4opt_test2(&ip4opt, "44241521 1234567812233445 2234567822233445 0000000000000000 0000000000000000", 36));

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
  netwib_er(netwib_pkt_append_ip4opt_test2(&ip4opt, "44241523 1234567812233445 2234567822233445 3234567800000000 4234567800000000", 36));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_prepend_ip4opt_test(void)
{
  puts("++++----\nTest of netwib_pkt_prepend_ip4opt");
  puts("Redundant with netwib_pkt_append_ip4opt");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_ip4opt_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_ip4opt");
  puts("Redundant with netwib_pkt_append_ip4opt");
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_pkt_ip4opt(void)
{
  puts("###########################################\nTest of pkt/ip4opt");
  netwib_er(netwib_ip4opt_initdefault_test());
  netwib_er(netwib_pkt_append_ip4opt_test());
  netwib_er(netwib_pkt_prepend_ip4opt_test());
  netwib_er(netwib_pkt_decode_ip4opt_test());

  return(NETWIB_ERR_OK);
}
