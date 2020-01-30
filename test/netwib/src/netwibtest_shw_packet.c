#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_data_show_test(void)
{
  puts("++++----\nTest of netwib_pkt_data_show");
  puts("Redundant with netwib_pkt_data_display");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_data_display_test(void)
{
  netwib_buf pkt;
  netwib_encodetype_context ctx;

  puts("++++----\nTest of netwib_pkt_data_display");

  netwib_er(netwib_buf_init_ext_string("0102030405061122334455660800 45120021FADE000080112E871122334455667788 00010002000DAAAA 'hello'", &pkt));

  netwib_er(netwib_buf_encode_transition(&ctx, NETWIB_ENCODETYPE_TRANSITION_INIT,
                                         NULL));
  netwib_er(netwib_pkt_data_display(&pkt, &ctx, NETWIB_ENCODETYPE_ARRAY));
  netwib_er(netwib_pkt_data_display(&pkt, &ctx, NETWIB_ENCODETYPE_HEXA));
  netwib_er(netwib_pkt_data_display(&pkt, &ctx, NETWIB_ENCODETYPE_SYNTH));
  netwib_er(netwib_pkt_data_display(&pkt, &ctx, NETWIB_ENCODETYPE_SYNTH));
  netwib_er(netwib_pkt_data_display(&pkt, &ctx, NETWIB_ENCODETYPE_ARRAY));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_link_show_test(void)
{
  puts("++++----\nTest of netwib_pkt_link_show");
  puts("Redundant with netwib_pkt_link_display");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_link_display_test(void)
{
  netwib_buf pkt, pktmixed;
  netwib_encodetype_context ctx;

  puts("++++----\nTest of netwib_pkt_link_display");

  netwib_er(netwib_buf_init_ext_string("0102030405061122334455660800 45120021FADE000080112E871122334455667788 00010002000DAAAA 'hello'", &pktmixed));
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_buf_decode(&pktmixed, NETWIB_DECODETYPE_MIXED, &pkt));

  netwib_er(netwib_buf_encode_transition(&ctx, NETWIB_ENCODETYPE_TRANSITION_INIT,
                                         NULL));
  netwib_er(netwib_pkt_link_display(NETWIB_DEVICE_DLTTYPE_ETHER, &pkt,
                                    &ctx, NETWIB_ENCODETYPE_ARRAY,
                                    NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwib_pkt_link_display(NETWIB_DEVICE_DLTTYPE_ETHER, &pkt,
                                    &ctx, NETWIB_ENCODETYPE_HEXA,
                                    NETWIB_ENCODETYPE_HEXA));
  netwib_er(netwib_pkt_link_display(NETWIB_DEVICE_DLTTYPE_ETHER, &pkt,
                                    &ctx, NETWIB_ENCODETYPE_SYNTH,
                                    NETWIB_ENCODETYPE_SYNTH));
  netwib_er(netwib_pkt_link_display(NETWIB_DEVICE_DLTTYPE_ETHER, &pkt,
                                    &ctx, NETWIB_ENCODETYPE_SYNTH,
                                    NETWIB_ENCODETYPE_SYNTH));
  netwib_er(netwib_pkt_link_display(NETWIB_DEVICE_DLTTYPE_NULL, &pkt,
                                    &ctx, NETWIB_ENCODETYPE_ARRAY,
                                    NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwib_buf_close(&pkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_ip_show_test(void)
{
  puts("++++----\nTest of netwib_pkt_ip_show");
  puts("Redundant with netwib_pkt_ip_display");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_ip_display_test(void)
{
  netwib_buf pkt, pktmixed;
  netwib_encodetype_context ctx;

  puts("++++----\nTest of netwib_pkt_ip_display");

  netwib_er(netwib_buf_init_ext_string("4512002DFADE000080062E861122334455667788 0001000211223344000000005A553434C5B41212 'hello'", &pktmixed));
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_buf_decode(&pktmixed, NETWIB_DECODETYPE_MIXED, &pkt));

  netwib_er(netwib_buf_encode_transition(&ctx, NETWIB_ENCODETYPE_TRANSITION_INIT,
                                         NULL));
  netwib_er(netwib_pkt_ip_display(&pkt, &ctx, NETWIB_ENCODETYPE_ARRAY,
                                  NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwib_pkt_ip_display(&pkt, &ctx, NETWIB_ENCODETYPE_HEXA,
                                  NETWIB_ENCODETYPE_HEXA));
  netwib_er(netwib_pkt_ip_display(&pkt, &ctx, NETWIB_ENCODETYPE_SYNTH,
                                  NETWIB_ENCODETYPE_SYNTH));
  netwib_er(netwib_pkt_ip_display(&pkt, &ctx, NETWIB_ENCODETYPE_SYNTH,
                                  NETWIB_ENCODETYPE_SYNTH));
  netwib_er(netwib_pkt_ip_display(&pkt, &ctx, NETWIB_ENCODETYPE_ARRAY,
                                  NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwib_buf_close(&pkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_udp_show_test(void)
{
  puts("++++----\nTest of netwib_pkt_udp_show");
  puts("Redundant with netwib_pkt_udp_display");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_udp_display_test(void)
{
  netwib_buf pkt, pktmixed;
  netwib_encodetype_context ctx;

  puts("++++----\nTest of netwib_pkt_udp_display");

  netwib_er(netwib_buf_init_ext_string("00010002000DAAAA 'hello'", &pktmixed));
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_buf_decode(&pktmixed, NETWIB_DECODETYPE_MIXED, &pkt));

  netwib_er(netwib_buf_encode_transition(&ctx, NETWIB_ENCODETYPE_TRANSITION_INIT,
                                         NULL));
  netwib_er(netwib_pkt_udp_display(&pkt, &ctx, NETWIB_ENCODETYPE_ARRAY,
                                   NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwib_pkt_udp_display(&pkt, &ctx, NETWIB_ENCODETYPE_HEXA,
                                   NETWIB_ENCODETYPE_HEXA));
  netwib_er(netwib_pkt_udp_display(&pkt, &ctx, NETWIB_ENCODETYPE_SYNTH,
                                   NETWIB_ENCODETYPE_SYNTH));
  netwib_er(netwib_pkt_udp_display(&pkt, &ctx, NETWIB_ENCODETYPE_SYNTH,
                                   NETWIB_ENCODETYPE_SYNTH));
  netwib_er(netwib_pkt_udp_display(&pkt, &ctx, NETWIB_ENCODETYPE_ARRAY,
                                   NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwib_buf_close(&pkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_tcp_show_test(void)
{
  puts("++++----\nTest of netwib_pkt_tcp_show");
  puts("Redundant with netwib_pkt_tcp_display");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_tcp_display_test(void)
{
  netwib_buf pkt, pktmixed;
  netwib_encodetype_context ctx;

  puts("++++----\nTest of netwib_pkt_tcp_display");

  netwib_er(netwib_buf_init_ext_string("0001000211223344000000006A553434514A1212 '1234' 'hello'", &pktmixed));
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_buf_decode(&pktmixed, NETWIB_DECODETYPE_MIXED, &pkt));

  netwib_er(netwib_buf_encode_transition(&ctx, NETWIB_ENCODETYPE_TRANSITION_INIT,
                                         NULL));
  netwib_er(netwib_pkt_tcp_display(&pkt, &ctx, NETWIB_ENCODETYPE_ARRAY,
                                   NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwib_pkt_tcp_display(&pkt, &ctx, NETWIB_ENCODETYPE_HEXA,
                                   NETWIB_ENCODETYPE_HEXA));
  netwib_er(netwib_pkt_tcp_display(&pkt, &ctx, NETWIB_ENCODETYPE_SYNTH,
                                   NETWIB_ENCODETYPE_SYNTH));
  netwib_er(netwib_pkt_tcp_display(&pkt, &ctx, NETWIB_ENCODETYPE_SYNTH,
                                   NETWIB_ENCODETYPE_SYNTH));
  netwib_er(netwib_pkt_tcp_display(&pkt, &ctx, NETWIB_ENCODETYPE_ARRAY,
                                   NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwib_buf_close(&pkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_icmp4_show_test(void)
{
  puts("++++----\nTest of netwib_pkt_icmp4_show");
  puts("Redundant with netwib_pkt_icmp4_display");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_icmp4_display_test(void)
{
  netwib_buf pkt, pktmixed;
  netwib_encodetype_context ctx;

  puts("++++----\nTest of netwib_pkt_icmp4_display");

  netwib_er(netwib_buf_init_ext_string("03011212 'hello'", &pktmixed));
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_buf_decode(&pktmixed, NETWIB_DECODETYPE_MIXED, &pkt));

  netwib_er(netwib_buf_encode_transition(&ctx, NETWIB_ENCODETYPE_TRANSITION_INIT,
                                         NULL));
  netwib_er(netwib_pkt_icmp4_display(&pkt, &ctx, NETWIB_ENCODETYPE_ARRAY,
                                     NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwib_pkt_icmp4_display(&pkt, &ctx, NETWIB_ENCODETYPE_HEXA,
                                     NETWIB_ENCODETYPE_HEXA));
  netwib_er(netwib_pkt_icmp4_display(&pkt, &ctx, NETWIB_ENCODETYPE_SYNTH,
                                     NETWIB_ENCODETYPE_SYNTH));
  netwib_er(netwib_pkt_icmp4_display(&pkt, &ctx, NETWIB_ENCODETYPE_SYNTH,
                                     NETWIB_ENCODETYPE_SYNTH));
  netwib_er(netwib_pkt_icmp4_display(&pkt, &ctx, NETWIB_ENCODETYPE_ARRAY,
                                     NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwib_buf_close(&pkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_icmp6_show_test(void)
{
  puts("++++----\nTest of netwib_pkt_icmp6_show");
  puts("Redundant with netwib_pkt_icmp6_display");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_icmp6_display_test(void)
{
  netwib_buf pkt, pktmixed;
  netwib_encodetype_context ctx;

  puts("++++----\nTest of netwib_pkt_icmp6_display");

  netwib_er(netwib_buf_init_ext_string("80006E6500010002", &pktmixed));
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_buf_decode(&pktmixed, NETWIB_DECODETYPE_MIXED, &pkt));

  netwib_er(netwib_buf_encode_transition(&ctx,
                                         NETWIB_ENCODETYPE_TRANSITION_INIT,
                                         NULL));
  netwib_er(netwib_pkt_icmp6_display(&pkt, &ctx, NETWIB_ENCODETYPE_ARRAY,
                                     NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwib_pkt_icmp6_display(&pkt, &ctx, NETWIB_ENCODETYPE_HEXA,
                                     NETWIB_ENCODETYPE_HEXA));
  netwib_er(netwib_pkt_icmp6_display(&pkt, &ctx, NETWIB_ENCODETYPE_SYNTH,
                                     NETWIB_ENCODETYPE_SYNTH));
  netwib_er(netwib_pkt_icmp6_display(&pkt, &ctx, NETWIB_ENCODETYPE_SYNTH,
                                     NETWIB_ENCODETYPE_SYNTH));
  netwib_er(netwib_pkt_icmp6_display(&pkt, &ctx, NETWIB_ENCODETYPE_ARRAY,
                                     NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwib_buf_close(&pkt));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_shw_packet(void)
{
  puts("###########################################\nTest of shw/packet");
  netwib_er(netwib_pkt_data_show_test());
  netwib_er(netwib_pkt_data_display_test());
  netwib_er(netwib_pkt_link_show_test());
  netwib_er(netwib_pkt_link_display_test());
  netwib_er(netwib_pkt_ip_show_test());
  netwib_er(netwib_pkt_ip_display_test());
  netwib_er(netwib_pkt_udp_show_test());
  netwib_er(netwib_pkt_udp_display_test());
  netwib_er(netwib_pkt_tcp_show_test());
  netwib_er(netwib_pkt_tcp_display_test());
  netwib_er(netwib_pkt_icmp4_show_test());
  netwib_er(netwib_pkt_icmp4_display_test());
  netwib_er(netwib_pkt_icmp6_show_test());
  netwib_er(netwib_pkt_icmp6_display_test());

  return(NETWIB_ERR_OK);
}
