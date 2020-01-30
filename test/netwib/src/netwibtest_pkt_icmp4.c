#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_icmp4type_test(void)
{
  netwib_buf buf;

  puts("++++----\nTest of netwib_buf_append_icmp4type");

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_icmp4type(NETWIB_ICMP4TYPE_DSTUNREACH, &buf));
  netwib_er(netwibtest_buf_content3(&buf, "'destination unreachable'"));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_icmp4code_test(void)
{
  netwib_buf buf;

  puts("++++----\nTest of netwib_buf_append_icmp4code");

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_icmp4code(NETWIB_ICMP4TYPE_DSTUNREACH,
                                        NETWIB_ICMP4CODE_DSTUNREACH_HOST,
                                        &buf));
  netwib_er(netwibtest_buf_content3(&buf, "'host'"));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_icmp4_initdefault_test(void)
{
  netwib_icmp4 icmp4;

  puts("++++----\nTest of netwib_icmp4_initdefault");

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_ECHOREP, &icmp4));
  netwib_er(netwibtest_icmp4_content(&icmp4, "0000000000000000"));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_DSTUNREACH, &icmp4));
  netwib_er(netwibtest_icmp4_content(&icmp4, "0301000000000000"));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_SRCQUENCH, &icmp4));
  netwib_er(netwibtest_icmp4_content(&icmp4, "0400000000000000"));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_REDIRECT, &icmp4));
  netwib_er(netwibtest_icmp4_content(&icmp4, "0501000000000000"));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_ECHOREQ, &icmp4));
  icmp4.msg.echo.id = 1;
  icmp4.msg.echo.seqnum = 2;
  netwib_er(netwibtest_icmp4_content(&icmp4, "0800000000010002"));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_TIMEEXCEED, &icmp4));
  netwib_er(netwibtest_icmp4_content(&icmp4, "0B00000000000000"));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_PARAPROB, &icmp4));
  netwib_er(netwibtest_icmp4_content(&icmp4, "0C00000000000000"));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_TIMESTAMPREQ, &icmp4));
  icmp4.msg.timestamp.id = 1;
  icmp4.msg.timestamp.seqnum = 2;
  netwib_er(netwibtest_icmp4_content(&icmp4, "0D00000000010002000000000000000000000000"));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_TIMESTAMPREP, &icmp4));
  icmp4.msg.timestamp.id = 1;
  icmp4.msg.timestamp.seqnum = 2;
  netwib_er(netwibtest_icmp4_content(&icmp4, "0E00000000010002000000000000000000000000"));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_INFOREQ, &icmp4));
  icmp4.msg.info.id = 1;
  icmp4.msg.info.seqnum = 2;
  netwib_er(netwibtest_icmp4_content(&icmp4, "0F00000000010002"));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_INFOREP, &icmp4));
  netwib_er(netwibtest_icmp4_content(&icmp4, "1000000000000000"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_icmp4_test2(netwib_icmp4 *picmp4,
                                                netwib_conststring mixed,
                                                netwib_uint32 wantedskipsize)
{
  netwib_icmp4 icmp42;
  netwib_uint32 skipsize;
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_icmp4(picmp4, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, mixed));

  netwib_er(netwib_pkt_decode_icmp4(&buf, &icmp42, &skipsize));
  if (skipsize != wantedskipsize) {
    return(NETWIBTEST_ERR_4);
  }
  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_icmp4(&icmp42, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, mixed));

  netwib_er(netwib_buf_close(&buf));
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_pkt_append_icmp4_test(void)
{
  netwib_buf ippktmixed, ippkt;
  netwib_icmp4 icmp4;

  puts("++++----\nTest of netwib_pkt_append_icmp4");

  netwib_er(netwib_buf_init_ext_string("45120021FADE000080112E871122334455667788 00010002000DAAAA 'hello'", &ippktmixed));
  netwib_er(netwib_buf_init_mallocdefault(&ippkt));
  netwib_er(netwib_buf_decode(&ippktmixed, NETWIB_DECODETYPE_MIXED, &ippkt));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_ECHOREP, &icmp4));
  netwib_er(netwib_buf_init_ext_string("1234", &icmp4.msg.echo.data));
  netwib_er(netwib_pkt_append_icmp4_test2(&icmp4, "0000000000000000 31323334", 12));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_DSTUNREACH, &icmp4));
  icmp4.msg.dstunreach.badippacket = ippkt;
  netwib_er(netwib_pkt_append_icmp4_test2(&icmp4, "0301000000000000 45120021FADE000080112E871122334455667788 00010002000DAAAA", 36));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_SRCQUENCH, &icmp4));
  icmp4.msg.srcquench.badippacket = ippkt;
  netwib_er(netwib_pkt_append_icmp4_test2(&icmp4, "0400000000000000 45120021FADE000080112E871122334455667788 00010002000DAAAA", 36));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_REDIRECT, &icmp4));
  icmp4.msg.redirect.badippacket = ippkt;
  netwib_er(netwib_pkt_append_icmp4_test2(&icmp4, "0501000000000000 45120021FADE000080112E871122334455667788 00010002000DAAAA", 36));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_ECHOREQ, &icmp4));
  icmp4.msg.echo.id = 1;
  icmp4.msg.echo.seqnum = 2;
  netwib_er(netwib_buf_init_ext_string("1234", &icmp4.msg.echo.data));
  netwib_er(netwib_pkt_append_icmp4_test2(&icmp4, "0800000000010002 31323334", 12));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_TIMEEXCEED, &icmp4));
  icmp4.msg.timeexceed.badippacket = ippkt;
  netwib_er(netwib_pkt_append_icmp4_test2(&icmp4, "0B00000000000000 45120021FADE000080112E871122334455667788 00010002000DAAAA", 36));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_PARAPROB, &icmp4));
  icmp4.msg.paraprob.badippacket = ippkt;
  netwib_er(netwib_pkt_append_icmp4_test2(&icmp4, "0C00000000000000 45120021FADE000080112E871122334455667788 00010002000DAAAA", 36));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_TIMESTAMPREQ, &icmp4));
  icmp4.msg.timestamp.id = 1;
  icmp4.msg.timestamp.seqnum = 2;
  netwib_er(netwib_pkt_append_icmp4_test2(&icmp4, "0D00000000010002000000000000000000000000", 20));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_TIMESTAMPREP, &icmp4));
  icmp4.msg.timestamp.id = 1;
  icmp4.msg.timestamp.seqnum = 2;
  netwib_er(netwib_pkt_append_icmp4_test2(&icmp4, "0E00000000010002000000000000000000000000", 20));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_INFOREQ, &icmp4));
  icmp4.msg.info.id = 1;
  icmp4.msg.info.seqnum = 2;
  netwib_er(netwib_buf_init_ext_string("1234", &icmp4.msg.info.data));
  netwib_er(netwib_pkt_append_icmp4_test2(&icmp4, "0F00000000010002 31323334", 12));

  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_INFOREP, &icmp4));
  netwib_er(netwib_buf_init_ext_string("1234", &icmp4.msg.info.data));
  netwib_er(netwib_pkt_append_icmp4_test2(&icmp4, "1000000000000000 31323334", 12));

  netwib_er(netwib_buf_close(&ippkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_icmp4_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_icmp4");
  puts("Redundant with netwib_pkt_append_icmp4");
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_pkt_icmp4(void)
{
  puts("###########################################\nTest of pkt/icmp4");
  netwib_er(netwib_buf_append_icmp4type_test());
  netwib_er(netwib_buf_append_icmp4code_test());

  netwib_er(netwib_icmp4_initdefault_test());
  netwib_er(netwib_pkt_append_icmp4_test());
  netwib_er(netwib_pkt_decode_icmp4_test());

  return(NETWIB_ERR_OK);
}
