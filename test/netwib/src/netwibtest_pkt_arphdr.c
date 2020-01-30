#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_arphdr_initdefault_test(void)
{
  puts("++++----\nTest of netwib_arphdr_initdefault");
  puts("Redundant with netwib_pkt_append_arphdr");

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_arphdr_test(void)
{
  netwib_arphdr arpheader, arpheader2;
  netwib_uint32 skipsize;
  netwib_buf buf;

  puts("++++----\nTest of netwib_pkt_append_arphdr");
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwibtest_arphdr_init(&arpheader));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_arphdr(&arpheader, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, "0001 0800  06 04 0001  010203040506 11223344 000000000000 55667788"));

  netwib_er(netwib_pkt_decode_arphdr(&buf, &arpheader2, &skipsize));
  if (skipsize != NETWIB_ARPHDR_LEN) {
    return(NETWIBTEST_ERR_4);
  }
  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_prepend_arphdr(&arpheader2, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, "0001 0800  06 04 0001  010203040506 11223344 000000000000 55667788"));

  netwib_er(netwib_buf_close(&buf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_prepend_arphdr_test(void)
{
  puts("++++----\nTest of netwib_pkt_prepend_arphdr");
  puts("Redundant with netwib_pkt_append_arphdr");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_arphdr_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_arphdr");
  puts("Redundant with netwib_pkt_append_arphdr");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_pkt_arphdr(void)
{
  puts("###########################################\nTest of pkt/arphdr");
  netwib_er(netwib_arphdr_initdefault_test());
  netwib_er(netwib_pkt_append_arphdr_test());
  netwib_er(netwib_pkt_prepend_arphdr_test());
  netwib_er(netwib_pkt_decode_arphdr_test());

  return(NETWIB_ERR_OK);
}
