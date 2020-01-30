#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_screen_test(void)
{
  netwib_io *pio;

  puts("++++----\nTest of netwib_io_init_screen");

  netwib_er(netwib_io_init_screen(&pio));
  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_screen_waitwrite(void)
{
  netwib_buf bufdata;
  netwib_io *pio;
  netwib_bool b;

  puts("++++----\nTest of wait, write");

  netwib_er(netwib_io_init_screen(&pio));

  netwib_er(netwib_io_wait_write(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_buf_init_ext_string("hello1", &bufdata));
  netwib_er(netwib_io_write(pio, &bufdata));

  netwib_er(netwib_io_wait_write(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_ctl_set_end_write(pio));
  netwib_eir(netwib_io_wait_write(pio, NETWIB_TIME_INFINITE, &b),
             NETWIB_ERR_LOOBJWRITENOTSUPPORTED);

  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_screen_waitreadunread(void)
{
  netwib_buf bufdata;
  netwib_io *pio;
  netwib_bool b;

  puts("++++----\nTest of wait, read, unread");

  netwib_er(netwib_buf_init_mallocdefault(&bufdata));

  netwib_er(netwib_io_init_screen(&pio));
  netwib_eir(netwib_io_wait_read(pio, NETWIB_TIME_INFINITE, &b),
             NETWIB_ERR_LOOBJREADNOTSUPPORTED);
  netwib_eir(netwib_io_read(pio, &bufdata),
             NETWIB_ERR_LOOBJREADNOTSUPPORTED);
  netwib_er(netwib_io_close(&pio));

  netwib_er(netwib_buf_close(&bufdata));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_screen_ctl(void)
{
  netwib_io_restype res;
  netwib_io *pio;

  puts("++++----\nTest of ctl");

  netwib_er(netwib_io_init_screen(&pio));

  netwib_er(netwib_io_ctl_get_res(pio, NETWIB_IO_WAYTYPE_WRITE, &res));
  if (res != NETWIB_IO_RESTYPE_SCREEN) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_screen(void)
{
  puts("###########################################\nTest of sys/screen");
  netwib_er(netwib_io_init_screen_test());
  netwib_er(test_screen_waitwrite());
  netwib_er(test_screen_waitreadunread());
  netwib_er(test_screen_ctl());

  return(NETWIB_ERR_OK);
}
