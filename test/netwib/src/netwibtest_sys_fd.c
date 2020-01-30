#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_fd_test(void)
{
  netwib_io *pio;

  puts("++++----\nTest of netwib_io_init_fd");

  netwib_er(netwib_io_init_fd(0, NETWIB_FALSE, &pio));
  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_fd_waitwrite(void)
{
  netwib_buf bufdata;
  netwib_io *pio;
  netwib_bool b;

  puts("++++----\nTest of wait, write");

  netwib_er(netwib_io_init_fd(1, NETWIB_FALSE, &pio));

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
static netwib_err test_fd_waitreadunread(void)
{
  netwib_buf bufdata;
  netwib_io *pio;
  netwib_bool b;
  int fd;

  puts("++++----\nTest of wait, read, unread");

#if defined NETWIBDEF_SYSNAME_Unix
  fd = open("./files/data", O_RDONLY);
#elif defined NETWIBDEF_SYSNAME_Windows
  fd = open("./files/data", O_RDONLY|_O_BINARY);
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif
  if (fd == -1) return(NETWIB_ERR_FUOPEN);

  netwib_er(netwib_io_init_fd(fd, NETWIB_TRUE, &pio));
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
static netwib_err test_fd_ctl(void)
{
  netwib_io_restype res;
  netwib_io *pio;

  puts("++++----\nTest of ctl");

  netwib_er(netwib_io_init_fd(1, NETWIB_FALSE, &pio));

  netwib_er(netwib_io_ctl_get_res(pio, NETWIB_IO_WAYTYPE_READ, &res));
  if (res != NETWIB_IO_RESTYPE_FD) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_fd(void)
{
  puts("###########################################\nTest of sys/fd");
  netwib_er(netwib_io_init_fd_test());
  netwib_er(test_fd_waitwrite());
  netwib_er(test_fd_waitreadunread());
  netwib_er(test_fd_ctl());

  return(NETWIB_ERR_OK);
}
