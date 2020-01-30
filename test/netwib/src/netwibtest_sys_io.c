#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err notdone_read(netwib_io *pio,
                               netwib_buf *pbuf)
{
  netwib_er(netwib_fmt_display("notdone_read\n"));
  pio = pio; /* for compiler warning */
  pbuf = pbuf; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}
static netwib_err notdone_write(netwib_io *pio,
                                netwib_constbuf *pbuf)
{
  netwib_er(netwib_fmt_display("notdone_write\n"));
  pio = pio; /* for compiler warning */
  pbuf = pbuf; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}
static netwib_err notdone_wait(netwib_io *pio,
                               netwib_io_waytype way,
                               netwib_consttime *pabstime,
                               netwib_bool *pevent)
{
  netwib_er(netwib_fmt_display("notdone_wait\n"));
  pio = pio; /* for compiler warning */
  way = way; /* for compiler warning */
  pabstime = pabstime; /* for compiler warning */
  pevent = pevent; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}
static netwib_err notdone_unread(netwib_io *pio,
                                 netwib_constbuf *pbuf)
{
  netwib_er(netwib_fmt_display("notdone_unread\n"));
  pio = pio; /* for compiler warning */
  pbuf = pbuf; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}
static netwib_err notdone_ctl_set(netwib_io *pio,
                                  netwib_io_waytype way,
                                  netwib_io_ctltype type,
                                  netwib_ptr p,
                                  netwib_uint32 ui)
{
  netwib_er(netwib_fmt_display("notdone_ctl_set\n"));
  pio = pio; /* for compiler warning */
  way = way; /* for compiler warning */
  type = type; /* for compiler warning */
  p = p; /* for compiler warning */
  ui = ui; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}
static netwib_err notdone_ctl_get(netwib_io *pio,
                                  netwib_io_waytype way,
                                  netwib_io_ctltype type,
                                  netwib_ptr p,
                                  netwib_uint32 *pui)
{
  netwib_er(netwib_fmt_display("notdone_ctl_get\n"));
  pio = pio; /* for compiler warning */
  way = way; /* for compiler warning */
  type = type; /* for compiler warning */
  p = p; /* for compiler warning */
  pui = pui; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}
static netwib_err notdone_close(netwib_io *pio)
{
  netwib_er(netwib_fmt_display("notdone_close\n"));
  pio = pio; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
static netwib_err notdone(netwib_io **ppio)
{
  netwib_er(netwib_io_init(NETWIB_TRUE, NETWIB_TRUE,
                           NULL,
                           notdone_read, notdone_write, notdone_wait,
                           notdone_unread, notdone_ctl_set, notdone_ctl_get,
                           notdone_close,
                           ppio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err usenext_read(netwib_io *pio,
                               netwib_buf *pbuf)
{
  netwib_io *pnextio;
  netwib_err ret;
  netwib_er(netwib_fmt_display("usenext_read\n"));
  ret = netwib_io_next_read(pio, &pnextio);
  if (ret == NETWIB_ERR_DATAEND) {
    return(NETWIB_ERR_PLEASETRYNEXT);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }
  netwib_er(netwib_io_read(pnextio, pbuf));
  return(NETWIB_ERR_OK);
}
static netwib_err usenext_write(netwib_io *pio,
                                netwib_constbuf *pbuf)
{
  netwib_io *pnextio;
  netwib_err ret;
  netwib_er(netwib_fmt_display("usenext_write\n"));
  ret = netwib_io_next_write(pio, &pnextio);
  if (ret == NETWIB_ERR_DATAEND) {
    return(NETWIB_ERR_PLEASETRYNEXT);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }
  netwib_er(netwib_io_write(pnextio, pbuf));
  return(NETWIB_ERR_OK);
}
static netwib_err usenext_wait(netwib_io *pio,
                               netwib_io_waytype way,
                               netwib_consttime *pabstime,
                               netwib_bool *pevent)
{
  netwib_io *pnextio;
  netwib_err ret;
  netwib_er(netwib_fmt_display("usenext_wait\n"));
  ret = netwib_io_next(pio, way, &pnextio);
  if (ret == NETWIB_ERR_DATAEND) {
    return(NETWIB_ERR_PLEASETRYNEXT);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }
  netwib_er(netwib_io_wait(pnextio, way, pabstime, pevent));
  return(NETWIB_ERR_OK);
}
static netwib_err usenext_unread(netwib_io *pio,
                                 netwib_constbuf *pbuf)
{
  netwib_io *pnextio;
  netwib_err ret;
  netwib_er(netwib_fmt_display("usenext_unread\n"));
  ret = netwib_io_next_read(pio, &pnextio);
  if (ret == NETWIB_ERR_DATAEND) {
    return(NETWIB_ERR_PLEASETRYNEXT);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }
  netwib_er(netwib_io_unread(pnextio, pbuf));
  return(NETWIB_ERR_OK);
}
static netwib_err usenext_ctl_set(netwib_io *pio,
                                  netwib_io_waytype way,
                                  netwib_io_ctltype type,
                                  netwib_ptr p,
                                  netwib_uint32 ui)
{
  netwib_io *pnextio;
  netwib_err ret;
  netwib_er(netwib_fmt_display("usenext_ctl_set\n"));
  ret = netwib_io_next(pio, way, &pnextio);
  if (ret == NETWIB_ERR_DATAEND) {
    return(NETWIB_ERR_PLEASETRYNEXT);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }
  netwib_er(netwib_io_ctl_set(pnextio, way, type, p, ui));
  return(NETWIB_ERR_OK);
}
static netwib_err usenext_ctl_get(netwib_io *pio,
                                  netwib_io_waytype way,
                                  netwib_io_ctltype type,
                                  netwib_ptr p,
                                  netwib_uint32 *pui)
{
  netwib_io *pnextio;
  netwib_err ret;
  netwib_er(netwib_fmt_display("usenext_ctl_get\n"));
  ret = netwib_io_next(pio, way, &pnextio);
  if (ret == NETWIB_ERR_DATAEND) {
    return(NETWIB_ERR_PLEASETRYNEXT);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }
  netwib_er(netwib_io_ctl_get(pnextio, way, type, p, pui));
  return(NETWIB_ERR_OK);
}
static netwib_err usenext_close(netwib_io *pio)
{
  netwib_er(netwib_fmt_display("usenext_close\n"));
  pio = pio; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
static netwib_err usenext(netwib_io **ppio)
{
  netwib_er(netwib_io_init(NETWIB_TRUE, NETWIB_TRUE,
                           NULL,
                           usenext_read, usenext_write, usenext_wait,
                           usenext_unread, usenext_ctl_set, usenext_ctl_get,
                           usenext_close,
                           ppio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err cstmsg_read(netwib_io *pio,
                              netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_string("cstmsg text\n", pbuf));
  pio = pio; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
static netwib_err cstmsg_wait(netwib_io *pio,
                              netwib_io_waytype way,
                              netwib_consttime *pabstime,
                              netwib_bool *pevent)
{
  *pevent = NETWIB_TRUE;
  pio = pio; /* for compiler warning */
  way = way; /* for compiler warning */
  pabstime = pabstime; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
static netwib_err cstmsg_unread(netwib_io *pio,
                                netwib_constbuf *pbuf)
{
  netwib_er(netwib_fmt_display("cstmsg_unread of : "));
  netwib_er(netwib_buf_display(pbuf, NETWIB_ENCODETYPE_DATA));
  pio = pio; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
static netwib_err cstmsg(netwib_io **ppio)
{
  netwib_er(netwib_io_init(NETWIB_TRUE, NETWIB_FALSE,
                           NULL,
                           cstmsg_read, NULL, cstmsg_wait,
                           cstmsg_unread, NULL, NULL, NULL,
                           ppio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_plug_test(void)
{
  netwib_io *pscreenio, *pmsgio, *pusenextio, *pnotdoneio, *pnotdoneio2;

  puts("++++----\nTest of netwib_io_plug");
  netwib_er(cstmsg(&pmsgio));
  netwib_er(usenext(&pusenextio));
  netwib_er(notdone(&pnotdoneio));
  netwib_er(netwib_io_plug_read(pusenextio, pmsgio));
  netwib_er(netwib_io_plug_read(pnotdoneio, pusenextio));
  netwib_er(netwib_io_close(&pnotdoneio));

  netwib_er(netwib_io_init_screen(&pscreenio));
  netwib_er(usenext(&pusenextio));
  netwib_er(notdone(&pnotdoneio));
  netwib_er(netwib_io_plug_write(pusenextio, pscreenio));
  netwib_er(netwib_io_plug_write(pnotdoneio, pusenextio));
  netwib_er(netwib_io_close(&pnotdoneio));

  netwib_er(netwib_io_init_screen(&pscreenio));
  netwib_er(usenext(&pusenextio));
  netwib_er(notdone(&pnotdoneio));
  netwib_er(notdone(&pnotdoneio2));
  netwib_er(netwib_io_plug_write(pusenextio, pscreenio));
  netwib_er(netwib_io_plug_rdwr(pnotdoneio, pusenextio));
  netwib_er(netwib_io_plug_supported(pnotdoneio2, pusenextio));
  netwib_er(netwib_io_close(&pnotdoneio));
  netwib_er(netwib_io_close(&pnotdoneio2));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_unplug_next_test(void)
{
  netwib_io *pscreenio, *pusenextio, *pnotdoneio, *pio;

  puts("++++----\nTest of netwib_io_unplug_next");
  netwib_er(netwib_io_init_screen(&pscreenio));
  netwib_er(usenext(&pusenextio));
  netwib_er(notdone(&pnotdoneio));
  netwib_er(netwib_io_plug_write(pusenextio, pscreenio));
  netwib_er(netwib_io_plug_write(pnotdoneio, pusenextio));
  netwib_er(netwib_io_unplug_next_write(pnotdoneio, &pio));
  if (pio != pusenextio) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_io_close(&pnotdoneio));

  netwib_er(netwib_io_init_screen(&pscreenio));
  netwib_er(usenext(&pusenextio));
  netwib_er(notdone(&pnotdoneio));
  netwib_er(netwib_io_plug_write(pusenextio, pscreenio));
  netwib_er(netwib_io_plug_write(pnotdoneio, pusenextio));
  netwib_er(netwib_io_unplug_next_write(pnotdoneio, &pio));
  if (pio != pusenextio) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_io_close(&pnotdoneio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_unplug_before_test(void)
{
  netwib_io *pscreenio, *pusenextio, *pnotdoneio;

  puts("++++----\nTest of netwib_io_unplug_before");
  netwib_er(netwib_io_init_screen(&pscreenio));
  netwib_er(usenext(&pusenextio));
  netwib_er(notdone(&pnotdoneio));
  netwib_er(netwib_io_plug_write(pusenextio, pscreenio));
  netwib_er(netwib_io_plug_write(pnotdoneio, pusenextio));
  netwib_er(netwib_io_unplug_before_write(pnotdoneio, pusenextio));
  netwib_er(netwib_io_close(&pusenextio));
  netwib_er(netwib_io_close(&pnotdoneio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_unplug_after_test(void)
{
  netwib_io *pscreenio, *pusenextio, *pnotdoneio, *pio;

  puts("++++----\nTest of netwib_io_unplug_after");
  netwib_er(netwib_io_init_screen(&pscreenio));
  netwib_er(usenext(&pusenextio));
  netwib_er(notdone(&pnotdoneio));
  netwib_er(netwib_io_plug_write(pusenextio, pscreenio));
  netwib_er(netwib_io_plug_write(pnotdoneio, pusenextio));
  netwib_er(netwib_io_unplug_after_write(pnotdoneio, pusenextio, &pio));
  if (pio != pscreenio) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_io_close(&pnotdoneio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_next_test(void)
{
  netwib_io *pscreenio, *pusenextio, *pnotdoneio, *pio;

  puts("++++----\nTest of netwib_io_next");
  netwib_er(netwib_io_init_screen(&pscreenio));
  netwib_er(usenext(&pusenextio));
  netwib_er(notdone(&pnotdoneio));
  netwib_er(netwib_io_plug_write(pusenextio, pscreenio));
  netwib_er(netwib_io_plug_rdwr(pnotdoneio, pusenextio));

  netwib_er(netwib_io_next_read(pnotdoneio, &pio));
  if (pio != pusenextio) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_eir(netwib_io_next_read(pusenextio, &pio), NETWIB_ERR_DATAEND);

  netwib_er(netwib_io_next_write(pnotdoneio, &pio));
  if (pio != pusenextio) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_io_next_write(pusenextio, &pio));
  if (pio != pscreenio) {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_io_next_rdwr(pnotdoneio, &pio));
  if (pio != pusenextio) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_eir(netwib_io_next_rdwr(pusenextio, &pio),
             NETWIB_ERR_LOOBJRDWRCONFLICT);

  netwib_er(netwib_io_close(&pnotdoneio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_write_test(void)
{
  netwib_io *pscreenio, *pcstmsgio, *pusenextio, *pnotdoneio;
  netwib_buf buf;

  puts("++++----\nTest of netwib_io_write");
  netwib_er(netwib_buf_init_ext_string("hello\n", &buf));

  netwib_er(netwib_io_init_screen(&pscreenio));
  netwib_er(usenext(&pusenextio));
  netwib_er(notdone(&pnotdoneio));
  netwib_er(netwib_io_plug_write(pusenextio, pscreenio));
  netwib_er(netwib_io_plug_write(pnotdoneio, pusenextio));
  netwib_er(netwib_io_write(pnotdoneio, &buf));
  netwib_er(netwib_io_close(&pnotdoneio));

  netwib_er(cstmsg(&pcstmsgio));
  netwib_er(usenext(&pusenextio));
  netwib_er(notdone(&pnotdoneio));
  netwib_er(netwib_io_plug_read(pusenextio, pcstmsgio));
  netwib_er(netwib_io_plug_rdwr(pnotdoneio, pusenextio));
  netwib_eir(netwib_io_write(pnotdoneio, &buf), NETWIB_ERR_PLEASETRYNEXT);
  netwib_er(netwib_io_close(&pnotdoneio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_read_test(void)
{
  netwib_io *pcstmsgio, *pusenextio, *pnotdoneio;
  netwib_buf buf;

  puts("++++----\nTest of netwib_io_read");
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(cstmsg(&pcstmsgio));
  netwib_er(usenext(&pusenextio));
  netwib_er(notdone(&pnotdoneio));
  netwib_er(netwib_io_plug_read(pusenextio, pcstmsgio));
  netwib_er(netwib_io_plug_rdwr(pnotdoneio, pusenextio));
  netwib_er(netwib_io_read(pnotdoneio, &buf));
  netwib_er(netwibtest_buf_content3(&buf, "'cstmsg text' 0A"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_io_close(&pnotdoneio));

  netwib_er(notdone(&pnotdoneio));
  netwib_eir(netwib_io_read(pnotdoneio, &buf), NETWIB_ERR_PLEASETRYNEXT);
  netwib_er(netwib_io_close(&pnotdoneio));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_unread_test(void)
{
  netwib_io *pcstmsgio, *pusenextio, *pnotdoneio;
  netwib_buf buf;

  puts("++++----\nTest of netwib_io_unread");
  netwib_er(netwib_buf_init_ext_string("hello\n", &buf));

  netwib_er(cstmsg(&pcstmsgio));
  netwib_er(usenext(&pusenextio));
  netwib_er(notdone(&pnotdoneio));
  netwib_er(netwib_io_plug_read(pusenextio, pcstmsgio));
  netwib_er(netwib_io_plug_rdwr(pnotdoneio, pusenextio));
  netwib_er(netwib_io_unread(pnotdoneio, &buf));
  netwib_er(netwib_io_close(&pnotdoneio));

  netwib_er(notdone(&pnotdoneio));
  netwib_eir(netwib_io_unread(pnotdoneio, &buf), NETWIB_ERR_PLEASETRYNEXT);
  netwib_er(netwib_io_close(&pnotdoneio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_wait_test(void)
{
  netwib_io *pcstmsgio, *pscreenio, *pusenextio, *pnotdoneio;
  netwib_time t;
  netwib_bool b;

  puts("++++----\nTest of netwib_io_wait");

  netwib_er(cstmsg(&pcstmsgio));
  netwib_er(netwib_io_init_screen(&pscreenio));
  netwib_er(usenext(&pusenextio));
  netwib_er(notdone(&pnotdoneio));
  netwib_er(netwib_io_plug_read(pusenextio, pcstmsgio));
  netwib_er(netwib_io_plug_write(pusenextio, pscreenio));
  netwib_er(netwib_io_plug_rdwr(pnotdoneio, pusenextio));
  netwib_er(netwib_io_wait_read(pnotdoneio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_io_wait_write(pnotdoneio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_plus_msec(&t, 100));
  netwib_er(netwib_io_wait_read(pnotdoneio, &t, &b));
  if (!b) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_io_wait_write(pnotdoneio, &t, &b));
  if (!b) {
    return(NETWIBTEST_ERR_8);
  }

  netwib_eir(netwib_io_wait_rdwr(pnotdoneio, NETWIB_TIME_INFINITE, &b),
             NETWIB_ERR_LOOBJRDWRCONFLICT);

  netwib_eir(netwib_io_wait_read(pscreenio, NETWIB_TIME_INFINITE, &b),
             NETWIB_ERR_LOOBJREADNOTSUPPORTED);

  netwib_eir(netwib_io_wait_write(pcstmsgio, NETWIB_TIME_INFINITE, &b),
             NETWIB_ERR_LOOBJWRITENOTSUPPORTED);

  netwib_er(netwib_io_close(&pnotdoneio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_ctl_set_test(void)
{
  netwib_io *pcstmsgio, *pscreenio, *pusenextio, *pnotdoneio;
  netwib_io *pio;
  netwib_bool b;
  netwib_uint32 ui;

  puts("++++----\nTest of netwib_io_ctl_set");

  netwib_er(netwib_io_init(NETWIB_TRUE, NETWIB_TRUE, (netwib_ptr)0x12345678,
                           NULL, NULL, NULL, NULL, NULL, NULL, NULL, &pio));
  if (pio->pcommon != (netwib_ptr)0x12345678) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_io_close(&pio));

  /**/

  netwib_er(cstmsg(&pcstmsgio));
  netwib_er(netwib_io_init_screen(&pscreenio));
  netwib_er(usenext(&pusenextio));
  netwib_er(notdone(&pnotdoneio));
  netwib_er(netwib_io_plug_read(pusenextio, pcstmsgio));
  netwib_er(netwib_io_plug_write(pusenextio, pscreenio));
  netwib_er(netwib_io_plug_rdwr(pnotdoneio, pusenextio));

  netwib_er(netwib_io_wait_read(pnotdoneio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_io_ctl_set_support(pnotdoneio, NETWIB_IO_WAYTYPE_READ,
                                      NETWIB_FALSE));
  netwib_er(netwib_io_ctl_get_support(pnotdoneio, NETWIB_IO_WAYTYPE_READ, &b));
  if (b) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_eir(netwib_io_wait_read(pnotdoneio, NETWIB_TIME_INFINITE, &b),
             NETWIB_ERR_LOOBJREADNOTSUPPORTED);
  netwib_er(netwib_io_ctl_set_support(pnotdoneio,  NETWIB_IO_WAYTYPE_READ,
                                      NETWIB_TRUE));
  netwib_er(netwib_io_wait_read(pnotdoneio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_io_wait_write(pnotdoneio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_io_ctl_set_support(pnotdoneio, NETWIB_IO_WAYTYPE_WRITE,
                                      NETWIB_FALSE));
  netwib_er(netwib_io_ctl_get_support(pnotdoneio, NETWIB_IO_WAYTYPE_WRITE,
                                      &b));
  if (b) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_eir(netwib_io_wait_write(pnotdoneio, NETWIB_TIME_INFINITE, &b),
             NETWIB_ERR_LOOBJWRITENOTSUPPORTED);
  netwib_er(netwib_io_ctl_set_support(pnotdoneio, NETWIB_IO_WAYTYPE_WRITE,
                                      NETWIB_TRUE));
  netwib_er(netwib_io_wait_write(pnotdoneio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_io_close(&pnotdoneio));

  /**/

  netwib_er(cstmsg(&pcstmsgio));
  netwib_er(netwib_io_ctl_get_numusers(pcstmsgio, NETWIB_IO_WAYTYPE_READ, &ui));
  if (ui != 0) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_io_ctl_set_numusers(pcstmsgio, NETWIB_IO_WAYTYPE_READ, 3));
  netwib_er(netwib_io_ctl_get_numusers(pcstmsgio, NETWIB_IO_WAYTYPE_READ, &ui));
  if (ui != 3) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_io_ctl_set_numusers_inc(pcstmsgio, NETWIB_IO_WAYTYPE_READ));
  netwib_er(netwib_io_ctl_get_numusers(pcstmsgio, NETWIB_IO_WAYTYPE_READ, &ui));
  if (ui != 4) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_io_ctl_set_numusers_dec(pcstmsgio, NETWIB_IO_WAYTYPE_READ));
  netwib_er(netwib_io_ctl_get_numusers(pcstmsgio, NETWIB_IO_WAYTYPE_READ, &ui));
  if (ui != 3) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_io_ctl_get_numusers(pcstmsgio, NETWIB_IO_WAYTYPE_WRITE, &ui));
  if (ui != 0) {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_io_close(&pcstmsgio));
  netwib_er(netwib_io_ctl_set_numusers(pcstmsgio, NETWIB_IO_WAYTYPE_READ, 0));
  netwib_er(netwib_io_close(&pcstmsgio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_ctl_get_test(void)
{
  puts("++++----\nTest of netwib_io_ctl_get");
  puts("Redundant with netwib_io_ctl_set");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_close_test(void)
{
  puts("++++----\nTest of netwib_io_close");
  puts("Redundant with netwib_io_plug");
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_io(void)
{
  puts("###########################################\nTest of sys/io");
  netwib_er(netwib_io_plug_test());
  netwib_er(netwib_io_unplug_next_test());
  netwib_er(netwib_io_unplug_before_test());
  netwib_er(netwib_io_unplug_after_test());
  netwib_er(netwib_io_next_test());
  netwib_er(netwib_io_write_test());
  netwib_er(netwib_io_read_test());
  netwib_er(netwib_io_unread_test());
  netwib_er(netwib_io_wait_test());
  netwib_er(netwib_io_ctl_set_test());
  netwib_er(netwib_io_ctl_get_test());
  netwib_er(netwib_io_close_test());

  return(NETWIB_ERR_OK);
}
