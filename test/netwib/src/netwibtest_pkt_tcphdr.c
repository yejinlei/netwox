#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_tcphdr_initdefault_test(void)
{
  puts("++++----\nTest of netwib_tcphdr_initdefault");
  puts("Redundant with netwib_pkt_append_tcphdr");

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_tcphdr_test2(netwib_tcphdr *ptcpheader,
                                                 netwib_conststring mixed,
                                                 netwib_uint32 wantedskipsize)
{
  netwib_tcphdr tcpheader2;
  netwib_uint32 skipsize;
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_tcphdr(ptcpheader, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, mixed));

  netwib_er(netwib_pkt_decode_tcphdr(&buf, &tcpheader2, &skipsize));
  if (skipsize != wantedskipsize) {
    return(NETWIBTEST_ERR_4);
  }
  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_prepend_tcphdr(&tcpheader2, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, mixed));

  netwib_er(netwib_buf_close(&buf));
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_pkt_append_tcphdr_test(void)
{
  netwib_tcphdr tcpheader;

  puts("++++----\nTest of netwib_pkt_append_tcphdr");
  netwib_er(netwibtest_tcphdr_init(&tcpheader));
  netwib_er(netwib_pkt_append_tcphdr_test2(&tcpheader, "0001 0002 11223344 00000000 5a55 3434 0000 1212", NETWIB_TCPHDR_MINLEN));

  netwib_er(netwibtest_tcphdr_init(&tcpheader));
  netwib_er(netwib_buf_init_ext_string("helo", &tcpheader.opts));
  tcpheader.doff += 1;
  netwib_er(netwib_pkt_append_tcphdr_test2(&tcpheader, "0001 0002 11223344 00000000 6a55 3434 0000 1212 'helo'", NETWIB_TCPHDR_MINLEN+4));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_prepend_tcphdr_test(void)
{
  puts("++++----\nTest of netwib_pkt_prepend_tcphdr");
  puts("Redundant with netwib_pkt_append_tcphdr");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_tcphdr_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_tcphdr");
  puts("Redundant with netwib_pkt_append_tcphdr");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_pkt_tcphdr(void)
{
  puts("###########################################\nTest of pkt/tcphdr");
  netwib_er(netwib_tcphdr_initdefault_test());
  netwib_er(netwib_pkt_append_tcphdr_test());
  netwib_er(netwib_pkt_prepend_tcphdr_test());
  netwib_er(netwib_pkt_decode_tcphdr_test());

  return(NETWIB_ERR_OK);
}
