#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_stream_test(void)
{
  netwib_io *pio;

  puts("++++----\nTest of netwib_io_init_stream");

  netwib_er(netwib_io_init_stream(stdin, NETWIB_FALSE, &pio));
  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_stream_waitwrite(void)
{
  netwib_buf bufdata;
  netwib_io *pio;
  netwib_bool b;

  puts("++++----\nTest of wait, write");

  netwib_er(netwib_io_init_stream(stdout, NETWIB_FALSE, &pio));

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
static netwib_err test_stream_waitreadunread(void)
{
  netwib_buf bufdata;
  netwib_io *pio;
  netwib_bool b;
  FILE *pf;

  puts("++++----\nTest of wait, read, unread");

  pf = fopen("./files/data", "rb");
  if (pf == NULL) return(NETWIB_ERR_FUFOPEN);


  netwib_er(netwib_io_init_stream(pf, NETWIB_TRUE, &pio));
  netwib_er(netwib_io_wait_read(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_buf_init_mallocdefault(&bufdata));
  netwib_er(netwib_io_read(pio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'abcdef' 0D0A 'ghij' 0D0A 'klmnop' 0D0A 'qrstuvw' 0D0A 'xyz' 0D0A"));
  netwib_er(netwib_buf_close(&bufdata));

  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_stream_ctl(void)
{
  netwib_io_restype res;
  netwib_io *pio;

  puts("++++----\nTest of ctl");

  netwib_er(netwib_io_init_stream(stdout, NETWIB_FALSE, &pio));

  netwib_er(netwib_io_ctl_get_res(pio, NETWIB_IO_WAYTYPE_READ, &res));
  if (res != NETWIB_IO_RESTYPE_STREAM) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_stream(void)
{
  puts("###########################################\nTest of sys/stream");
  netwib_er(netwib_io_init_stream_test());
  netwib_er(test_stream_waitwrite());
  netwib_er(test_stream_waitreadunread());
  netwib_er(test_stream_ctl());

  return(NETWIB_ERR_OK);
}
