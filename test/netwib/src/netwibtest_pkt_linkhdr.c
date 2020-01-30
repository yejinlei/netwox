#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_linkhdr_initdefault_test(void)
{
  puts("++++----\nTest of netwib_linkhdr_initdefault");
  puts("Redundant with netwib_pkt_append_linkhdr");

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_append_linkhdr_test(void)
{
  netwib_linkhdr linkhdr, linkhdr2;
  netwib_uint32 skipsize;
  netwib_buf buf;

  puts("++++----\nTest of netwib_pkt_append_linkhdr");
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  /**/
  netwib_er(netwib_linkhdr_initdefault(NETWIB_DEVICE_DLTTYPE_EN10MB, &linkhdr));
  netwib_er(netwib_eth_init_fields(0x11,0x22,0x33,0x44,0x55,0x66,
                                 &linkhdr.hdr.ether.src));
  netwib_er(netwib_eth_init_fields(1,2,3,4,5,6, &linkhdr.hdr.ether.dst));
  linkhdr.hdr.ether.type = NETWIB_ETHERHDRTYPE_IP4;

  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_linkhdr(&linkhdr, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, "01 02 03 04  05 06 11 22  33 44 55 66  08 00"));

  netwib_er(netwib_pkt_decode_linkhdr(NETWIB_DEVICE_DLTTYPE_EN10MB, &buf,
                                    &linkhdr2, &skipsize));
  if (skipsize != NETWIB_ETHERHDR_LEN) {
    return(NETWIBTEST_ERR_4);
  }
  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_prepend_linkhdr(&linkhdr2, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, "01 02 03 04  05 06 11 22  33 44 55 66  08 00"));

  /**/
  linkhdr.type = NETWIB_DEVICE_DLTTYPE_NULL;
  linkhdr.hdr.null.type = NETWIB_ETHERHDRTYPE_IP4;

  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_linkhdr(&linkhdr, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, "00000800"));

  netwib_er(netwib_pkt_decode_linkhdr(NETWIB_DEVICE_DLTTYPE_NULL, &buf,
                                    &linkhdr2, &skipsize));
  if (skipsize != NETWIB_NULLHDR_LEN) {
    return(NETWIBTEST_ERR_4);
  }
  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_prepend_linkhdr(&linkhdr2, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, "00000800"));

  /**/
  linkhdr.type = NETWIB_DEVICE_DLTTYPE_LOOP;
  linkhdr.hdr.loop.type = NETWIB_ETHERHDRTYPE_IP4;

  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_linkhdr(&linkhdr, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, "00000800"));

  netwib_er(netwib_pkt_decode_linkhdr(NETWIB_DEVICE_DLTTYPE_LOOP, &buf,
                                    &linkhdr2, &skipsize));
  if (skipsize != NETWIB_LOOPHDR_LEN) {
    return(NETWIBTEST_ERR_4);
  }
  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_prepend_linkhdr(&linkhdr2, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, "00000800"));

  /**/
  linkhdr.type = NETWIB_DEVICE_DLTTYPE_RAW;

  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_linkhdr(&linkhdr, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwib_buf_append_byte(0x40, &buf));
  netwib_er(netwibtest_buf_content3(&buf, "40"));

  netwib_er(netwib_pkt_decode_linkhdr(NETWIB_DEVICE_DLTTYPE_RAW, &buf,
                                      &linkhdr2, &skipsize));
  if (skipsize != NETWIB_RAWHDR_LEN) {
    return(NETWIBTEST_ERR_4);
  }
  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_prepend_linkhdr(&linkhdr2, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, ""));

  /**/
  linkhdr.type = NETWIB_DEVICE_DLTTYPE_RAW;

  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_linkhdr(&linkhdr, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwib_buf_append_byte(0x60, &buf));
  netwib_er(netwibtest_buf_content3(&buf, "60"));

  netwib_er(netwib_pkt_decode_linkhdr(NETWIB_DEVICE_DLTTYPE_RAW, &buf,
                                      &linkhdr2, &skipsize));
  if (skipsize != NETWIB_RAWHDR_LEN) {
    return(NETWIBTEST_ERR_4);
  }
  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_prepend_linkhdr(&linkhdr2, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, ""));

  /**/
  linkhdr.type = NETWIB_DEVICE_DLTTYPE_PPP;
  linkhdr.hdr.ppp.address = 0x12;
  linkhdr.hdr.ppp.control = 0x45;
  linkhdr.hdr.ppp.protocol = NETWIB_PPPHDRPROTO_IP4;

  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_linkhdr(&linkhdr, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, "12450021"));

  netwib_er(netwib_pkt_decode_linkhdr(NETWIB_DEVICE_DLTTYPE_PPP, &buf,
                                    &linkhdr2, &skipsize));
  if (skipsize != NETWIB_PPPHDR_LEN) {
    return(NETWIBTEST_ERR_4);
  }
  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_prepend_linkhdr(&linkhdr2, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, "12450021"));

  /**/
  linkhdr.type = NETWIB_DEVICE_DLTTYPE_LINUX_SLL;
  linkhdr.hdr.linuxsll.pkttype = NETWIB_LINUXSLLHDRPKTTYPE_BROADCAST;
  linkhdr.hdr.linuxsll.hatype = NETWIB_LINUXSLLHDRHATYPE_ETHER;
  linkhdr.hdr.linuxsll.halen = 6;
  netwib_c_memcpy(linkhdr.hdr.linuxsll.srcaddr, "abcdef", 6);
  linkhdr.hdr.linuxsll.protocol = NETWIB_ETHERHDRTYPE_IP4;

  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_linkhdr(&linkhdr, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, "0001 0001  0006 6162636465660000 0800"));

  netwib_er(netwib_pkt_decode_linkhdr(NETWIB_DEVICE_DLTTYPE_LINUX_SLL, &buf,
                                    &linkhdr2, &skipsize));
  if (skipsize != NETWIB_LINUXSLLHDR_LEN) {
    return(NETWIBTEST_ERR_4);
  }
  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_prepend_linkhdr(&linkhdr2, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib_er(netwibtest_buf_content3(&buf, "0001 0001  0006 6162636465660000 0800"));

  /**/
  netwib_er(netwib_buf_close(&buf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_prepend_linkhdr_test(void)
{
  puts("++++----\nTest of netwib_pkt_prepend_linkhdr");
  puts("Redundant with netwib_pkt_append_linkhdr");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_decode_linkhdr_test(void)
{
  puts("++++----\nTest of netwib_pkt_decode_linkhdr");
  puts("Redundant with netwib_pkt_append_linkhdr");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_linkhdr_set_proto_test(void)
{
  netwib_linkhdr linkhdr;
  netwib_linkhdrproto linkhdrproto;

  puts("++++----\nTest of netwib_linkhdr_set_proto");

  linkhdr.type = NETWIB_DEVICE_DLTTYPE_EN10MB;
  linkhdr.hdr.ether.type = NETWIB_ETHERHDRTYPE_IP4;

  netwib_er(netwib_linkhdr_set_proto(&linkhdr, NETWIB_LINKHDRPROTO_IP6));
  netwib_er(netwib_linkhdr_get_proto(&linkhdr, &linkhdrproto));
  if (linkhdrproto != NETWIB_LINKHDRPROTO_IP6) {
    return(NETWIBTEST_ERR_0);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_linkhdr_get_proto_test(void)
{
  puts("++++----\nTest of netwib_linkhdr_get_proto");
  puts("Redundant with netwib_linkhdr_set_proto");
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_pkt_linkhdr(void)
{
  puts("###########################################\nTest of pkt/linkhdr");
  netwib_er(netwib_linkhdr_initdefault_test());

  netwib_er(netwib_pkt_append_linkhdr_test());
  netwib_er(netwib_pkt_prepend_linkhdr_test());
  netwib_er(netwib_pkt_decode_linkhdr_test());

  netwib_er(netwib_linkhdr_set_proto_test());
  netwib_er(netwib_linkhdr_get_proto_test());

  return(NETWIB_ERR_OK);
}
