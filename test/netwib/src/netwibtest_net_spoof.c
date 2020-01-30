#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_spoof_test(void)
{
  netwib_buf device, buf, buf2;
  netwib_io *pio;
  netwib_err ret;

  puts("++++----\nTest of netwib_io_init_spoof");
  netwib_er(netwib_buf_init_mallocdefault(&device));
  ret = netwibtest_choose_device(&device);
  if (ret == NETWIB_ERR_NOTFOUND) {
    netwib_er(netwib_buf_close(&device));
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  netwib_er(netwib_fmt_display("Link spoof\n"));
  ret = netwib_io_init_spoof_link(&device, &pio);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_init_ext_string("1234567890ab aabbccddeeff 0000", &buf2));
    netwib_er(netwib_buf_init_mallocdefault(&buf));
    netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_HEXA, &buf));
    ret = netwib_io_write(pio, &buf);
    if (ret != NETWIB_ERR_OK) {
      netwib_err_display(ret, NETWIB_ERR_ENCODETYPE_FULL);
    }
    netwib_er(netwib_buf_close(&buf));
    netwib_er(netwib_io_close(&pio));
    netwib_er(netwib_fmt_display("Link spoof done\n"));
  } else if (ret == NETWIB_ERR_LONOTSUPPORTED) {
    netwib_er(netwib_fmt_display("Device %{buf} does not support link spoof\n",
                                 &device));
  } else {
    return(ret);
  }
  netwib_er(netwib_buf_close(&device));

  netwib_er(netwib_fmt_display("IP4 spoof\n"));
  ret = netwib_io_init_spoof_ip4(&pio);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_init_ext_string("4510003C001D40004006B63FC0A80101C0A801FE04010017871F743100000000A0027D78AA890000020405B40402080A000C8A400000000001030300", &buf2));
    netwib_er(netwib_buf_init_mallocdefault(&buf));
    netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_HEXA, &buf));
    ret = netwib_io_write(pio, &buf);
    if (ret != NETWIB_ERR_OK) {
      netwib_err_display(ret, NETWIB_ERR_ENCODETYPE_FULL);
    }
    netwib_er(netwib_buf_close(&buf));
    netwib_er(netwib_io_close(&pio));
    netwib_er(netwib_fmt_display("IP4 spoof done\n"));
  } else if (ret == NETWIB_ERR_LONOTSUPPORTED) {
    netwib_er(netwib_fmt_display("IP4 spoof is not supported\n"));
  } else {
    return(ret);
  }

  netwib_er(netwib_fmt_display("IP6 spoof\n"));
  ret = netwib_io_init_spoof_ip6(&pio);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_init_ext_string("6234567890ab 12121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212", &buf2));
    netwib_er(netwib_buf_init_mallocdefault(&buf));
    netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_HEXA, &buf));
    ret = netwib_io_write(pio, &buf);
    if (ret != NETWIB_ERR_OK) {
      netwib_err_display(ret, NETWIB_ERR_ENCODETYPE_FULL);
    }
    netwib_er(netwib_buf_close(&buf));
    netwib_er(netwib_io_close(&pio));
    netwib_er(netwib_fmt_display("IP6 spoof done\n"));
  } else if (ret == NETWIB_ERR_LONOTSUPPORTED) {
    netwib_er(netwib_fmt_display("IP6 spoof is not supported\n"));
  } else {
    return(ret);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_spoof_ip_test100(netwib_io *pio,
                                                  netwib_conststring pkt)
{
  netwib_buf buf, buf2;
  netwib_uint32 i;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_buf_init_ext_string(pkt, &buf2));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_MIXED, &buf));
  for (i = 0; i < 10 ; i++) {
    ret = netwib_io_write(pio, &buf);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_buf_close(&buf));
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_fmt_display("IP spoof done\n"));
  } else if (ret == NETWIB_ERR_LONOTSUPPORTED) {
    netwib_er(netwib_fmt_display("IP spoof is not supported\n"));
  } else {
    return(ret);
  }

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_io_init_spoof_ip_test2(netwib_io *pio,
                                                netwib_conststring pkt)
{
  netwib_buf buf, buf2;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string(pkt, &buf2));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_MIXED, &buf));
  ret = netwib_io_write(pio, &buf);
  netwib_er(netwib_buf_close(&buf));
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_fmt_display("IP spoof done\n"));
  } else if (ret == NETWIB_ERR_LONOTSUPPORTED) {
    netwib_er(netwib_fmt_display("IP spoof is not supported\n"));
  } else {
    return(ret);
  }

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_io_init_spoof_ip_test(void)
{
  netwib_io *pio;

  puts("++++----\nTest of netwib_io_init_spoof_ip");
  netwib_er(netwib_fmt_display("Default route might be needed because IP addresses in the packets do not belong to the computer.\n"));

  netwib_er(netwib_io_init_spoof_ip(NETWIB_SPOOF_IP_INITTYPE_BEST, &pio));
  netwib_er(netwib_io_init_spoof_ip_test2(pio, "4510003C001D40004006B63FC0A80101C0A801FE04010017871F743100000000A0027D78AA890000020405B40402080A000C8A400000000001030300"));
  netwib_er(netwib_io_init_spoof_ip_test2(pio, "45100067001D40004006B63FC0A80101C0A801FE04010017871F743100000000A0027D78AA890000020405B40402080A000C8A400000000001030300 'hello, this is a message I want to send you'"));
  netwib_er(netwib_io_init_spoof_ip_test2(pio, "45100300001D40004006B63FC0A80101C0A801FE04010017871F743100000000A0027D78AA890000020405B40402080A000C8A400000000001030300 'hello, this is a message I want to send you ./src/netwibtest_sys_kbdpress.o ./src/netwibtest_sys_screen.o ./src/netwibtest_sys_bufenc.o ./src/netwibtest_sys_bufdec.o ./src/netwibtest_sys_disp.o ./src/netwibtest_sys_record.o ./src/netwibtest_sys_beep.o ./src/netwibtest_sys_global.o ./src/netwibtest_net_ethsi.o ./src/netwibtest_net_ports.o ./src/netwibtest_net_'"));
  netwib_er(netwib_io_init_spoof_ip_test2(pio, "45100067001D40004006B63FC0A80102C0A801FE04010017871F743100000000A0027D78AA890000020405B40402080A000C8A400000000001030300 'hello, this is a message I want to send you'"));
  netwib_er(netwib_io_init_spoof_ip_test2(pio, "45100067001D40004006B63FC0A80101C0A801FD04010017871F743100000000A0027D78AA890000020405B40402080A000C8A400000000001030300 'hello, this is a message I want to send you'"));
  netwib_er(netwib_io_init_spoof_ip_test100(pio, "45100067001D40004006B63FC0A80101C0A801FD04010017871F743100000000A0027D78AA890000020405B40402080A000C8A400000000001030300 'hello, this is a message I want to send you'"));

  netwib_er(netwib_io_init_spoof_ip_test2(pio, "681004D200190641 fec00000000000100000000000000002 fec00000000000100000000000000002 0001000211223344000000005A553434C5911212 'hello'"));

  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_spoof_ip_inittype_test(void)
{
  netwib_buf buf;
  puts("++++----\nTest of netwib_buf_append_spoof_ip_inittype");

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_spoof_ip_inittype(NETWIB_SPOOF_IP_INITTYPE_RAW,
                                                &buf));
  netwib_er(netwibtest_buf_content2(&buf, "only try raw IP4/6 spoof"));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_spoof_ip_inittype_init_kbd_test(void)
{
  netwib_buf buf;
  netwib_spoof_ip_inittype type;

  puts("++++----\nTest of netwib_spoof_ip_inittype_init_kbd");
  netwib_er(netwib_buf_init_ext_string("Select...", &buf));
  netwib_er(netwib_spoof_ip_inittype_init_kbd(&buf,
                                              NETWIB_SPOOF_IP_INITTYPE_RAWLINK,
                                              &type));

  netwib_er(netwib_spoof_ip_inittype_init_kbd(NULL,
                                              NETWIB_SPOOF_IP_INITTYPE_INIT_KBD_NODEF,
                                              &type));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_net_spoof(void)
{
  puts("###########################################\nTest of net/spoof");
  if (netwibtest_testadm) {
    netwib_er(netwib_io_init_spoof_test());
    netwib_er(netwib_io_init_spoof_ip_test());
  }
  netwib_er(netwib_buf_append_spoof_ip_inittype_test());
  if (netwibtest_testkbdlevel == 2) {
    netwib_er(netwib_spoof_ip_inittype_init_kbd_test());
  }

  return(NETWIB_ERR_OK);
}
