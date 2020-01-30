#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err test_sniff_display(netwib_io *pio)
{
  netwib_buf buf;
  netwib_time t;
  netwib_uint32 i;
  netwib_encodetype_context ctx;
  netwib_device_dlttype dlt;
  netwib_bool event;
  netwib_err ret;

  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_plus_sec(&t, 5));
  netwib_er(netwib_buf_encode_transition(&ctx,
                                         NETWIB_ENCODETYPE_TRANSITION_INIT,
                                         NULL));
  netwib_er(netwib_sniff_ctl_get_dlt(pio, &dlt));

  for (i = 0; i < 10; i++) {
    netwib_er(netwib_io_wait(pio, NETWIB_IO_WAYTYPE_READ, &t, &event));
    if (!event) {
      break;
    }
    netwib_er(netwib_buf_init_mallocdefault(&buf));
    ret = netwib_io_read(pio, &buf);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_fmt_display("%{uint32:02} ", i));
      netwib_er(netwib_pkt_link_display(dlt, &buf,
                                        &ctx, NETWIB_ENCODETYPE_SYNTH,
                                        NETWIB_ENCODETYPE_SYNTH));
    } else if (ret == NETWIB_ERR_DATANOTAVAIL) {
      netwib_er(netwib_fmt_display("Wait indicates event but nothing to read (this might be normal)\n"));
    } else {
      return(ret);
    }
    netwib_er(netwib_buf_close(&buf));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_sniff_test(void)
{
  netwib_buf device, filter;
  netwib_io *pio;
  netwib_err ret;

  puts("++++----\nTest of netwib_io_init_sniff");
  netwib_er(netwib_buf_init_mallocdefault(&device));
  ret = netwibtest_choose_device(&device);
  if (ret == NETWIB_ERR_NOTFOUND) {
    netwib_er(netwib_buf_close(&device));
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  netwib_er(netwib_buf_init_mallocdefault(&filter));
  netwib_er(netwib_buf_append_string("ip or not ip", &filter));
  netwib_er(netwib_fmt_display("We sniff during 5 seconds on %{buf} ...\n",
                               &device));
  ret = netwib_io_init_sniff(&device, &filter, &pio);
  netwib_er(netwib_buf_close(&filter));
  netwib_er(netwib_buf_close(&device));
#if NETWIBDEF_SUPPORT_SNIFF == 1
  netwib_er(ret);
#else
  if (ret == NETWIB_ERR_OK) ret = NETWIB_ERR_LOERROROKKO;
  if (ret == NETWIB_ERR_LONOTSUPPORTED) ret = NETWIB_ERR_OK;
  return(ret);
#endif

  netwib_er(test_sniff_display(pio))

  netwib_er(netwib_io_close(&pio));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_sniff_ipreas_test(void)
{
  netwib_buf device;
  netwib_io *pio, *pioipreas;
  netwib_err ret;

  puts("++++----\nTest of netwib_io_init_sniff_ipreas");
#if NETWIBDEF_SUPPORT_SNIFF == 0
  puts("Not tested");
  return(NETWIB_ERR_OK);
#endif

  netwib_er(netwib_buf_init_mallocdefault(&device));
  ret = netwibtest_choose_device(&device);
  if (ret == NETWIB_ERR_NOTFOUND) {
    netwib_er(netwib_buf_close(&device));
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  netwib_er(netwib_io_init_sniff(&device, NULL, &pio));
  netwib_er(netwib_buf_close(&device));

  netwib_er(netwib_io_init_sniff_ipreas(&pioipreas));
  netwib_er(netwib_io_plug_read(pioipreas, pio));

  netwib_er(test_sniff_display(pioipreas))

  netwib_er(netwib_io_close(&pioipreas));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_sniff_tcpreord_test(void)
{
  netwib_buf device;
  netwib_io *pio, *pioipreas, *piotcpreord;
  netwib_err ret;

  puts("++++----\nTest of netwib_io_init_sniff_tcpreord");
#if NETWIBDEF_SUPPORT_SNIFF == 0
  puts("Not tested");
  return(NETWIB_ERR_OK);
#endif

  netwib_er(netwib_buf_init_mallocdefault(&device));
  ret = netwibtest_choose_device(&device);
  if (ret == NETWIB_ERR_NOTFOUND) {
    netwib_er(netwib_buf_close(&device));
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }


  netwib_er(netwib_io_init_sniff(&device, NULL, &pio));
  netwib_er(netwib_buf_close(&device));

  netwib_er(netwib_io_init_sniff_ipreas(&pioipreas));
  netwib_er(netwib_io_plug_read(pioipreas, pio));

  netwib_er(netwib_io_init_sniff_tcpreord(&piotcpreord));
  netwib_er(netwib_io_plug_read(piotcpreord, pioipreas));

  netwib_er(test_sniff_display(piotcpreord))

  netwib_er(netwib_io_close(&piotcpreord));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_sniff_ip_test(void)
{
  netwib_buf device, filter;
  netwib_io *pio;
  netwib_err ret;

  puts("++++----\nTest of netwib_io_init_sniff_ip");
  netwib_er(netwib_buf_init_mallocdefault(&device));
  ret = netwibtest_choose_device(&device);
  if (ret == NETWIB_ERR_NOTFOUND) {
    netwib_er(netwib_buf_close(&device));
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  netwib_er(netwib_buf_init_mallocdefault(&filter));
  netwib_er(netwib_buf_append_string("ip or not ip", &filter));
  netwib_er(netwib_fmt_display("We sniff during 5 seconds on %{buf} ...\n",
                               &device));
  ret = netwib_io_init_sniff_ip(&device, &filter, NETWIB_TRUE, NETWIB_TRUE, &pio);
  netwib_er(netwib_buf_close(&filter));
  netwib_er(netwib_buf_close(&device));
#if NETWIBDEF_SUPPORT_SNIFF == 1
  netwib_er(ret);
#else
  if (ret == NETWIB_ERR_OK) ret = NETWIB_ERR_LOERROROKKO;
  if (ret == NETWIB_ERR_LONOTSUPPORTED) ret = NETWIB_ERR_OK;
  return(ret);
#endif

  netwib_er(test_sniff_display(pio))

  netwib_er(netwib_io_close(&pio));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwibtest_io_sniff_spoof_supported(void)
{
  netwib_conf_devices conf;
  netwib_conf_devices_index *pconfindex;
  netwib_device_dlttype dlt;
  netwib_buf buf;
  netwib_io *pio;
  netwib_err ret;

  puts("++++----\nTest of supported devices");
  puts("Not done. You can enable it by hand.");
  return(NETWIB_ERR_OK);

  /* spoof IP4 */
  ret = netwib_io_init_spoof_ip4(&pio);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_fmt_display("Spoof ip4 supported\n"));
    netwib_er(netwib_spoof_ctl_get_dlt(pio, &dlt));
    if (dlt != NETWIB_DEVICE_DLTTYPE_RAW) {
      netwib_er(netwib_fmt_display(" dlt=%{uint32} != wanted\n", dlt));
      return(NETWIBTEST_ERR_0);
    }
    netwib_er(netwib_io_close(&pio));
  } else if (ret == NETWIB_ERR_LONOTSUPPORTED) {
    netwib_er(netwib_fmt_display("Spoof ip4 not supported\n"));
  } else {
    return(ret);
  }
  /* spoof IP6 */
  ret = netwib_io_init_spoof_ip6(&pio);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_fmt_display("Spoof ip6 supported\n"));
    netwib_er(netwib_spoof_ctl_get_dlt(pio, &dlt));
    if (dlt != NETWIB_DEVICE_DLTTYPE_RAW) {
      netwib_er(netwib_fmt_display(" dlt=%{uint32} != wanted\n", dlt));
      return(NETWIBTEST_ERR_0);
    }
    netwib_er(netwib_io_close(&pio));
  } else if (ret == NETWIB_ERR_LONOTSUPPORTED) {
    netwib_er(netwib_fmt_display("Spoof ip6 not supported\n"));
  } else {
    return(ret);
  }

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_conf_devices_index_init(&conf, &pconfindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_conf_devices_index_next(pconfindex);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib__buf_reinit(&buf);
    netwib_er(netwib_buf_append_device_hwtype(conf.hwtype, &buf));
    netwib_er(netwib_fmt_display("Dev %{buf} of hwtype=%{uint32}=%{buf} :\n",
                                 &conf.device, conf.hwtype, &buf));
    /* sniff */
    ret = netwib_io_init_sniff(&conf.device, NULL, &pio);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_sniff_ctl_get_dlt(pio, &dlt));
      netwib__buf_reinit(&buf);
      netwib_er(netwib_buf_append_device_dlttype(dlt, &buf));
      netwib_er(netwib_fmt_display(" sniff dlt=%{uint32}=%{buf}\n", dlt, &buf));
      netwib_er(netwib_io_close(&pio));
    } else if (ret == NETWIB_ERR_LONOTSUPPORTED) {
      netwib_er(netwib_fmt_display(" sniff not supported\n"));
    } else {
      break;
    }
    /* spoof */
    ret = netwib_io_init_spoof_link(&conf.device, &pio);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_spoof_ctl_get_dlt(pio, &dlt));
      netwib__buf_reinit(&buf);
      netwib_er(netwib_buf_append_device_dlttype(dlt, &buf));
      netwib_er(netwib_fmt_display(" spoof dlt=%{uint32}=%{buf}\n", dlt, &buf));
      netwib_er(netwib_io_close(&pio));
    } else if (ret == NETWIB_ERR_LONOTSUPPORTED) {
      netwib_er(netwib_fmt_display(" spoof not supported\n"));
    } else {
      break;
    }
  }
  netwib_er(netwib_conf_devices_index_close(&pconfindex));
  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_net_sniff(void)
{
  puts("###########################################\nTest of net/sniff");
  if (netwibtest_testadm) {
    netwib_er(netwib_io_init_sniff_test());
    netwib_er(netwib_io_init_sniff_ipreas_test());
    netwib_er(netwib_io_init_sniff_tcpreord_test());
    netwib_er(netwib_io_init_sniff_ip_test());
    netwib_er(netwibtest_io_sniff_spoof_supported());
  }

  return(NETWIB_ERR_OK);
}
