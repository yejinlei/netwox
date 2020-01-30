#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_linkhdr_show_test(void)
{
  netwib_linkhdr linkhdr;
  netwib_buf buf;

  puts("++++----\nTest of netwib_linkhdr_show");
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  /**/
  netwib_er(netwib_linkhdr_initdefault(NETWIB_DEVICE_DLTTYPE_EN10MB, &linkhdr));
  netwib_er(netwib_eth_init_fields(0x11,0x22,0x33,0x44,0x55,0x66,
                                 &linkhdr.hdr.ether.src));
  netwib_er(netwib_eth_init_fields(1,2,3,4,5,6, &linkhdr.hdr.ether.dst));
  linkhdr.hdr.ether.type = NETWIB_ETHERHDRTYPE_IP4;

  netwib__buf_reinit(&buf);
  netwib_er(netwib_linkhdr_show(&linkhdr, NETWIB_ENCODETYPE_SYNTH, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_fmt_display("\n"));
  netwib_er(netwibtest_buf_content3(&buf, "'11:22:33:44:55:66->01:02:03:04:05:06'"));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_linkhdr_show(&linkhdr, NETWIB_ENCODETYPE_ARRAY, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwibtest_buf_content3(&buf, "'Ethernet________________________________________________________.' 0A '| 11:22:33:44:55:66->01:02:03:04:05:06 type:0x0800              |' 0A '|_______________________________________________________________|' 0A"));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_linkhdr_show(&linkhdr, NETWIB_ENCODETYPE_HEXA_WRAP,
                                    &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwibtest_buf_content3(&buf, "'01 02 03 04 05 06 11 22 33 44 55 66 08 00' 0A"));

  /**/
  linkhdr.type = NETWIB_DEVICE_DLTTYPE_NULL;
  linkhdr.hdr.null.type = NETWIB_ETHERHDRTYPE_IP4;

  netwib__buf_reinit(&buf);
  netwib_er(netwib_linkhdr_show(&linkhdr, NETWIB_ENCODETYPE_SYNTH, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_fmt_display("\n"));
  netwib_er(netwibtest_buf_content3(&buf, "'null'"));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_linkhdr_show(&linkhdr, NETWIB_ENCODETYPE_ARRAY, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwibtest_buf_content3(&buf, "'Null____________________________________________________________.' 0A '| type:0x00000800                                               |' 0A '|_______________________________________________________________|' 0A"));

  /**/
  linkhdr.type = NETWIB_DEVICE_DLTTYPE_LOOP;
  linkhdr.hdr.loop.type = NETWIB_ETHERHDRTYPE_IP4;

  netwib__buf_reinit(&buf);
  netwib_er(netwib_linkhdr_show(&linkhdr, NETWIB_ENCODETYPE_SYNTH, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_fmt_display("\n"));
  netwib_er(netwibtest_buf_content3(&buf, "'loop'"));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_linkhdr_show(&linkhdr, NETWIB_ENCODETYPE_ARRAY, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwibtest_buf_content3(&buf, "'Loop____________________________________________________________.' 0A '| type:0x00000800                                               |' 0A '|_______________________________________________________________|' 0A"));

  /**/
  linkhdr.type = NETWIB_DEVICE_DLTTYPE_RAW;

  netwib__buf_reinit(&buf);
  netwib_er(netwib_linkhdr_show(&linkhdr, NETWIB_ENCODETYPE_SYNTH, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_fmt_display("\n"));
  netwib_er(netwibtest_buf_content3(&buf, ""));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_linkhdr_show(&linkhdr, NETWIB_ENCODETYPE_ARRAY, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwibtest_buf_content3(&buf, ""));

  /**/
  linkhdr.type = NETWIB_DEVICE_DLTTYPE_PPP;
  linkhdr.hdr.ppp.address = 0x12;
  linkhdr.hdr.ppp.control = 0x45;
  linkhdr.hdr.ppp.protocol = NETWIB_PPPHDRPROTO_IP4;

  netwib__buf_reinit(&buf);
  netwib_er(netwib_linkhdr_show(&linkhdr, NETWIB_ENCODETYPE_SYNTH, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_fmt_display("\n"));
  netwib_er(netwibtest_buf_content3(&buf, "'ppp'"));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_linkhdr_show(&linkhdr, NETWIB_ENCODETYPE_ARRAY, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwibtest_buf_content3(&buf, " 'Ppp_____________________________________________________________.' 0A '| address:0x12 control:0x45 protocol:0x0021                     |' 0A '|_______________________________________________________________|' 0A"));

  /**/
  linkhdr.type = NETWIB_DEVICE_DLTTYPE_LINUX_SLL;
  linkhdr.hdr.linuxsll.pkttype = NETWIB_LINUXSLLHDRPKTTYPE_BROADCAST;
  linkhdr.hdr.linuxsll.hatype = NETWIB_LINUXSLLHDRHATYPE_ETHER;
  linkhdr.hdr.linuxsll.halen = 6;
  netwib_c_memcpy(linkhdr.hdr.linuxsll.srcaddr, "abcdef", 6);
  linkhdr.hdr.linuxsll.protocol = NETWIB_ETHERHDRTYPE_IP4;

  netwib__buf_reinit(&buf);
  netwib_er(netwib_linkhdr_show(&linkhdr, NETWIB_ENCODETYPE_SYNTH, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_fmt_display("\n"));
  netwib_er(netwibtest_buf_content3(&buf, "'linuxsll'"));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_linkhdr_show(&linkhdr, NETWIB_ENCODETYPE_ARRAY, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwibtest_buf_content3(&buf, " 'Linux_SLL_______________________________________________________.' 0A '| pkttype:0x0001 hatype:0x0001 srcaddr:616263646566             |' 0A '| protocol:0x0800                                               |' 0A '|_______________________________________________________________|' 0A"));

  /**/
  netwib_er(netwib_buf_close(&buf));
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_shw_linkhdr(void)
{
  puts("###########################################\nTest of shw/linkhdr");
  netwib_er(netwib_linkhdr_show_test());

  return(NETWIB_ERR_OK);
}
