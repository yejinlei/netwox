#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_ip_frag_f_test(netwib_constbuf *pfrag,
                                        netwib_ptr infos)
{
  netwib_er(netwib_pkt_ip_display(pfrag, NULL, NETWIB_ENCODETYPE_ARRAY,
                                  NETWIB_ENCODETYPE_DUMP));

  infos = infos; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_ip_frag_test2(netwib_constbuf *pippkt,
                                       netwib_uint32 fragdatasize,
                                       netwib_uint32 fragtotalsize)
{
  netwib_er(netwib_fmt_display("Test %{uint32} %{uint32}\n",
                               fragdatasize, fragtotalsize));
  netwib_er(netwib_ip_frag(pippkt, fragdatasize, fragtotalsize,
                           &netwib_ip_frag_f_test, NULL));
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_ip_frag_test(void)
{
  netwib_buf pkt, pktmixed;

  puts("++++----\nTest of netwib_ip_frag");
  netwib_er(netwib_buf_init_mallocdefault(&pkt));

  netwib_er(netwib_buf_init_ext_string("4512002DFADE000080062E861122334455667788 0001000211223344000000005A553434C5B41212 'hello'", &pktmixed));
  netwib_er(netwib_buf_decode(&pktmixed, NETWIB_DECODETYPE_MIXED, &pkt));
  netwib_er(netwib_ip_frag_test2(&pkt, 0, 0));
  netwib_er(netwib_ip_frag_test2(&pkt, 8, 0));
  netwib_er(netwib_ip_frag_test2(&pkt, 16, 0));
  netwib_er(netwib_ip_frag_test2(&pkt, 24, 0));
  netwib_er(netwib_ip_frag_test2(&pkt, 32, 0));
  netwib_er(netwib_ip_frag_test2(&pkt, 0, 32));
  netwib_er(netwib_ip_frag_test2(&pkt, 16, 40));

  netwib__buf_reinit(&pkt);
  netwib_er(netwib_buf_init_ext_string("46120025FADE00008011C91C1122334455667788 '1234' 00010002000DAAAA 'hello'", &pktmixed));
  netwib_er(netwib_buf_decode(&pktmixed, NETWIB_DECODETYPE_MIXED, &pkt));
  netwib_er(netwib_ip_frag_test2(&pkt, 8, 0));
  netwib_er(netwib_ip_frag_test2(&pkt, 16, 0));

  netwib__buf_reinit(&pkt);
  netwib_er(netwib_buf_init_ext_string("681004D2002900411122334400000005000000060000000755667788000000080000000900000000 3C0168656C6C6F00 06016F6C6C656800 0001000211223344000000005A553434C5911212 'hello'", &pktmixed));
  netwib_er(netwib_buf_decode(&pktmixed, NETWIB_DECODETYPE_MIXED, &pkt));
  netwib_er(netwib_ip_frag_test2(&pkt, 16, 0));
  netwib_er(netwib_ip_frag_test2(&pkt, 24, 0));
  netwib_er(netwib_ip_frag_test2(&pkt, 32, 0));
  netwib_er(netwib_ip_frag_test2(&pkt, 0, 80));

  netwib__buf_reinit(&pkt);
  netwib_er(netwib_buf_init_ext_string("681004D2001906411122334400000005000000060000000755667788000000080000000900000000 0001000211223344000000005A553434C5911212 'hello'", &pktmixed));
  netwib_er(netwib_buf_decode(&pktmixed, NETWIB_DECODETYPE_MIXED, &pkt));
  netwib_er(netwib_ip_frag_test2(&pkt, 16, 0));

  netwib_er(netwib_buf_close(&pkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_pkt_ipfrag(void)
{
  puts("###########################################\nTest of pkt/ipfrag");
  netwib_er(netwib_ip_frag_test());

  return(NETWIB_ERR_OK);
}
