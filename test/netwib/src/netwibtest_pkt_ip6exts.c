#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_ip6exts_test(void)
{
  netwib_buf pkt;
  netwib_ip6ext ip6ext;
  netwib_tcphdr tcpheader;
  netwib_ipproto lastproto;
  netwib_uint32 lastprotooffset, skipsize;

  puts("++++----\nTest of netwib_pkt_decode_ip6exts");
  netwib_er(netwib_buf_init_mallocdefault(&pkt));

  netwib_er(netwib_ip6ext_initdefault(NETWIB_IPPROTO_HOPOPTS, &ip6ext));
  netwib_er(netwib_buf_init_ext_string("hello", &ip6ext.ext.hopopts.options));
  ip6ext.nextproto = NETWIB_IPPROTO_DSTOPTS;
  netwib_er(netwib_pkt_append_ip6ext(&ip6ext, &pkt));
  netwib_er(netwib_ip6ext_initdefault(NETWIB_IPPROTO_DSTOPTS, &ip6ext));
  netwib_er(netwib_buf_init_ext_string("olleh", &ip6ext.ext.dstopts.options));
  ip6ext.nextproto = NETWIB_IPPROTO_TCP;
  netwib_er(netwib_pkt_append_ip6ext(&ip6ext, &pkt));
  netwib_er(netwib_tcphdr_initdefault(&tcpheader));
  tcpheader.seqnum = 3;
  netwib_er(netwib_pkt_append_tcphdr(&tcpheader, &pkt));
  netwib_er(netwibtest_buf_content3(&pkt, "3C0168656C6C6F00 06016F6C6C656800 000000000000000300000000500005DC00000000"));

  netwib_er(netwib_pkt_decode_ip6exts(NETWIB_IPPROTO_HOPOPTS, &pkt,
                                      &lastproto, &lastprotooffset,
                                      &skipsize));
  if (lastproto != NETWIB_IPPROTO_TCP) {
    return(NETWIBTEST_ERR_2);
  }
  if (lastprotooffset != 8) {
    return(NETWIBTEST_ERR_3);
  }
  if (skipsize != 16) {
    return(NETWIBTEST_ERR_4);
  }

  pkt.beginoffset += 8;
  netwib_er(netwib_pkt_decode_ip6exts(NETWIB_IPPROTO_DSTOPTS, &pkt,
                                      &lastproto, &lastprotooffset,
                                      &skipsize));
  if (lastproto != NETWIB_IPPROTO_TCP) {
    return(NETWIBTEST_ERR_2);
  }
  if (lastprotooffset != 0) {
    return(NETWIBTEST_ERR_3);
  }
  if (skipsize != 8) {
    return(NETWIBTEST_ERR_4);
  }

  pkt.beginoffset += 8;
  netwib_er(netwib_pkt_decode_ip6exts(NETWIB_IPPROTO_TCP, &pkt,
                                      &lastproto, &lastprotooffset,
                                      &skipsize));
  if (lastproto != NETWIB_IPPROTO_TCP) {
    return(NETWIBTEST_ERR_2);
  }
  if (lastprotooffset != (netwib_uint32)-1) {
    return(NETWIBTEST_ERR_3);
  }
  if (skipsize != 0) {
    return(NETWIBTEST_ERR_4);
  }

  netwib_er(netwib_buf_close(&pkt));

  /**/
  netwib_er(netwib_buf_init_mallocdefault(&pkt));

  netwib_er(netwib_ip6ext_initdefault(NETWIB_IPPROTO_HOPOPTS, &ip6ext));
  netwib_er(netwib_buf_init_ext_string("hello", &ip6ext.ext.hopopts.options));
  ip6ext.nextproto = NETWIB_IPPROTO_DSTOPTS;
  netwib_er(netwib_pkt_append_ip6ext(&ip6ext, &pkt));
  netwib_er(netwib_ip6ext_initdefault(NETWIB_IPPROTO_DSTOPTS, &ip6ext));
  netwib_er(netwib_buf_init_ext_string("olleh", &ip6ext.ext.dstopts.options));
  ip6ext.nextproto = NETWIB_IPPROTO_TCP;
  netwib_er(netwib_pkt_append_ip6ext(&ip6ext, &pkt));
  pkt.endoffset -= 3;
  netwib_er(netwibtest_buf_content3(&pkt, "3C0168656C6C6F00 06016F6C6C"));

  netwib_er(netwib_pkt_decode_ip6exts(NETWIB_IPPROTO_HOPOPTS, &pkt,
                                      &lastproto, &lastprotooffset,
                                      &skipsize));
  if (lastproto != NETWIB_IPPROTO_DSTOPTS) {
    return(NETWIBTEST_ERR_2);
  }
  if (lastprotooffset != 0) {
    return(NETWIBTEST_ERR_3);
  }
  if (skipsize != 8) {
    return(NETWIBTEST_ERR_4);
  }

  netwib_er(netwib_buf_close(&pkt));

  /**/
  netwib_er(netwib_buf_init_mallocdefault(&pkt));

  netwib_er(netwib_ip6ext_initdefault(NETWIB_IPPROTO_HOPOPTS, &ip6ext));
  netwib_er(netwib_buf_init_ext_string("hello", &ip6ext.ext.hopopts.options));
  ip6ext.nextproto = NETWIB_IPPROTO_FRAGMENT;
  netwib_er(netwib_pkt_append_ip6ext(&ip6ext, &pkt));
  netwib_er(netwib_ip6ext_initdefault(NETWIB_IPPROTO_FRAGMENT, &ip6ext));
  ip6ext.ext.fragment.fragmentoffset = 0;
  ip6ext.ext.fragment.id = 2;
  ip6ext.nextproto = NETWIB_IPPROTO_DSTOPTS;
  netwib_er(netwib_pkt_append_ip6ext(&ip6ext, &pkt));
  netwib_er(netwib_ip6ext_initdefault(NETWIB_IPPROTO_DSTOPTS, &ip6ext));
  netwib_er(netwib_buf_init_ext_string("olleh", &ip6ext.ext.dstopts.options));
  ip6ext.nextproto = NETWIB_IPPROTO_TCP;
  netwib_er(netwib_pkt_append_ip6ext(&ip6ext, &pkt));
  netwib_er(netwibtest_buf_content3(&pkt, "2C0168656C6C6F00 3C00000000000002 06016F6C6C656800"));

  netwib_er(netwib_pkt_decode_ip6exts(NETWIB_IPPROTO_HOPOPTS, &pkt,
                                      &lastproto, &lastprotooffset,
                                      &skipsize));
  if (lastproto != NETWIB_IPPROTO_TCP) {
    return(NETWIBTEST_ERR_2);
  }
  if (lastprotooffset != 16) {
    return(NETWIBTEST_ERR_3);
  }
  if (skipsize != 24) {
    return(NETWIBTEST_ERR_4);
  }

  netwib_er(netwib_buf_close(&pkt));

  /**/
  netwib_er(netwib_buf_init_mallocdefault(&pkt));

  netwib_er(netwib_ip6ext_initdefault(NETWIB_IPPROTO_HOPOPTS, &ip6ext));
  netwib_er(netwib_buf_init_ext_string("hello", &ip6ext.ext.hopopts.options));
  ip6ext.nextproto = NETWIB_IPPROTO_FRAGMENT;
  netwib_er(netwib_pkt_append_ip6ext(&ip6ext, &pkt));
  netwib_er(netwib_ip6ext_initdefault(NETWIB_IPPROTO_FRAGMENT, &ip6ext));
  ip6ext.ext.fragment.fragmentoffset = 1;
  ip6ext.ext.fragment.id = 2;
  ip6ext.nextproto = NETWIB_IPPROTO_DSTOPTS;
  netwib_er(netwib_pkt_append_ip6ext(&ip6ext, &pkt));
  netwib_er(netwib_ip6ext_initdefault(NETWIB_IPPROTO_DSTOPTS, &ip6ext));
  netwib_er(netwib_buf_init_ext_string("olleh", &ip6ext.ext.dstopts.options));
  ip6ext.nextproto = NETWIB_IPPROTO_TCP;
  netwib_er(netwib_pkt_append_ip6ext(&ip6ext, &pkt));
  netwib_er(netwibtest_buf_content3(&pkt, "2C0168656C6C6F00 3C00000800000002 06016F6C6C656800"));

  netwib_er(netwib_pkt_decode_ip6exts(NETWIB_IPPROTO_HOPOPTS, &pkt,
                                      &lastproto, &lastprotooffset,
                                      &skipsize));
  if (lastproto != NETWIB_IPPROTO_DSTOPTS) {
    return(NETWIBTEST_ERR_2);
  }
  if (lastprotooffset != 8) {
    return(NETWIBTEST_ERR_3);
  }
  if (skipsize != 16) {
    return(NETWIBTEST_ERR_4);
  }

  netwib_er(netwib_buf_close(&pkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_pkt_ip6exts(void)
{
  puts("###########################################\nTest of pkt/ip6exts");
  netwib_er(netwib_pkt_decode_ip6exts_test());

  return(NETWIB_ERR_OK);
}
