#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_ip6exts_show_test2(netwib_ipproto ipproto,
                                            netwib_constbuf *pip6exts,
                                            netwib_encodetype encodetype,
                                            netwib_conststring wantmixed)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_ip6exts_show(ipproto, pip6exts, encodetype, &buf));
  netwib_er(netwib_fmt_display("%{buf}\n", &buf));
  netwib_er(netwibtest_buf_content3(&buf, wantmixed));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_ip6exts_show_test(void)
{
  netwib_buf pkt;
  netwib_ip6ext ip6ext;
  netwib_buf buftext;
  netwib_string text;

  puts("++++----\nTest of netwib_ip6exts_show");

  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_buf_init_mallocdefault(&buftext));

  netwib_er(netwib_ip6ext_initdefault(NETWIB_IPPROTO_HOPOPTS, &ip6ext));
  netwib_er(netwib_buf_init_ext_string("hello", &ip6ext.ext.hopopts.options));
  ip6ext.nextproto = NETWIB_IPPROTO_DSTOPTS;
  netwib_er(netwib_pkt_append_ip6ext(&ip6ext, &pkt));

  netwib_er(netwib_ip6ext_initdefault(NETWIB_IPPROTO_DSTOPTS, &ip6ext));
  netwib_er(netwib_buf_init_ext_string("hello2", &ip6ext.ext.dstopts.options));
  ip6ext.nextproto = NETWIB_IPPROTO_UDP;
  netwib_er(netwib_pkt_append_ip6ext(&ip6ext, &pkt));

  netwib_er(netwib_ip6exts_show_test2(NETWIB_IPPROTO_HOPOPTS, &pkt,
                                      NETWIB_ENCODETYPE_SYNTH, "'ip6exts'"));
  netwib_er(netwib_buf_append_string("'IP6 Hop-by-Hop options__________________________________________.' 0A '|  next header  |  hdr ext len  |                               |' 0A '|____0x3C=60____|_______2_______|                               |' 0A '| options: 68656c6c6f00                                         |' 0A '|___________________________________", &buftext));
  netwib_er(netwib_buf_append_string("____________________________|' 0A 'IP6 Destination options_________________________________________.' 0A '|  next header  |  hdr ext len  |                               |' 0A '|____0x11=17____|_______2_______|                               |' 0A '| options: 68656c6c6f32                                         |' 0A '|_______________________________________________________________|' 0A", &buftext));
  netwib_er(netwib_buf_ref_string(&buftext, &text));
  netwib_er(netwib_ip6exts_show_test2(NETWIB_IPPROTO_HOPOPTS, &pkt,
                                      NETWIB_ENCODETYPE_ARRAY, text));
  netwib_er(netwib_ip6exts_show_test2(NETWIB_IPPROTO_HOPOPTS, &pkt,
                                      NETWIB_ENCODETYPE_DUMP, "'3c 01 68 65  6c 6c 6f 00  11 01 68 65  6c 6c 6f 32  # <.hello...hello2' 0A"));

  netwib_er(netwib_buf_close(&buftext));
  netwib_er(netwib_buf_close(&pkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_shw_ip6exts(void)
{
  puts("###########################################\nTest of shw/ip6exts");
  netwib_er(netwib_ip6exts_show_test());

  return(NETWIB_ERR_OK);
}
