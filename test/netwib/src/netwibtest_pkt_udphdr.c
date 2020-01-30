#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_udphdr_initdefault_test(void)
{
  puts("++++----\nTest of netwib_udphdr_initdefault");
  puts("Redundant with netwib_pkt_append_udphdr");

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_udphdr_test(void)
{
  netwib_udphdr udpheader, udpheader2;
  netwib_uint32 skipsize;
  netwib_buf buf;

  puts("++++----\nTest of netwib_pkt_append_udphdr");
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwibtest_udphdr_init(&udpheader));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_udphdr(&udpheader, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, "0001 0002 0008 0000"));

  netwib_er(netwib_pkt_decode_udphdr(&buf, &udpheader2, &skipsize));
  if (skipsize != NETWIB_UDPHDR_LEN) {
    return(NETWIBTEST_ERR_4);
  }
  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_prepend_udphdr(&udpheader2, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, "0001 0002 0008 0000"));

  netwib_er(netwib_buf_close(&buf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_prepend_udphdr_test(void)
{
  puts("++++----\nTest of netwib_pkt_prepend_udphdr");
  puts("Redundant with netwib_pkt_append_udphdr");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_udphdr_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_udphdr");
  puts("Redundant with netwib_pkt_append_udphdr");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_pkt_udphdr(void)
{
  puts("###########################################\nTest of pkt/udphdr");
  netwib_er(netwib_udphdr_initdefault_test());
  netwib_er(netwib_pkt_append_udphdr_test());
  netwib_er(netwib_pkt_prepend_udphdr_test());
  netwib_er(netwib_pkt_decode_udphdr_test());

  return(NETWIB_ERR_OK);
}
