#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_kbddefault_test(void)
{
  netwib_io *pio;

  puts("++++----\nTest of netwib_io_init_kbddefault");

  netwib_er(netwib_io_init_kbddefault(&pio));
  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_kbd_fd_test(void)
{
  netwib_io *pio;

  puts("++++----\nTest of netwib_io_init_kbd_fd");


#if defined NETWIBDEF_SYSNAME_Unix
  netwib_er(netwib_io_init_kbd_fd(0, &pio));
  netwib_er(netwib_io_close(&pio));
#elif defined NETWIBDEF_SYSNAME_Windows
  netwib_eir(netwib_io_init_kbd_fd(0, &pio), NETWIB_ERR_LONOTIMPLEMENTED);
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_kbd_handle_test(void)
{
  netwib_io *pio;

  puts("++++----\nTest of netwib_io_init_kbd_handle");

#if defined NETWIBDEF_SYSNAME_Unix
  netwib_eir(netwib_io_init_kbd_handle(NULL, &pio), NETWIB_ERR_LONOTIMPLEMENTED);
#elif defined NETWIBDEF_SYSNAME_Windows
  netwib_er(netwib_io_init_kbd_handle(GetStdHandle(STD_INPUT_HANDLE), &pio));
  netwib_er(netwib_io_close(&pio));
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_kbd_waitreadunread(void)
{
  netwib_buf bukbdata;
  netwib_io *pio;
  netwib_bool b;

  puts("++++----\nTest of wait, read, unread");
  netwib_er(netwibtest_something_to_do("Please follow instructions..."));

  netwib_er(netwib_io_init_kbddefault(&pio));
  netwib_er(netwib_io_wait_read(pio, NETWIB_TIME_ZERO, &b));
  if (b) {
    return(NETWIBTEST_ERR_7);
  }

  puts("Enter \"sdf\", then press 'Return'");
  netwib_er(netwib_buf_init_mallocdefault(&bukbdata));
  netwib_er(netwib_io_read(pio, &bukbdata));
  netwib_er(netwibtest_buf_content3(&bukbdata, "'sdf'"));
  netwib_er(netwib_buf_close(&bukbdata));

  puts("Enter \"sdf\", then press 'Return'");
  netwib_er(netwib_buf_init_mallocdefault(&bukbdata));
  netwib_er(netwib_kbd_ctl_set_echo(pio, NETWIB_TRUE));
  netwib_er(netwib_kbd_ctl_set_line(pio, NETWIB_TRUE));
  netwib_er(netwib_io_read(pio, &bukbdata));
  netwib_er(netwibtest_buf_content3(&bukbdata, "'sdf'"));
  netwib_er(netwib_buf_close(&bukbdata));

  puts("Enter \"sdf\", then press 'Return' (nothing is displayed)");
  netwib_er(netwib_buf_init_mallocdefault(&bukbdata));
  netwib_er(netwib_kbd_ctl_set_echo(pio, NETWIB_FALSE));
  netwib_er(netwib_kbd_ctl_set_line(pio, NETWIB_TRUE));
  netwib_er(netwib_io_read(pio, &bukbdata));
  netwib_er(netwibtest_buf_content3(&bukbdata, "'sdf'"));
  netwib_er(netwib_buf_close(&bukbdata));

  puts("Press key 'a'");
  netwib_er(netwib_buf_init_mallocdefault(&bukbdata));
  netwib_er(netwib_kbd_ctl_set_echo(pio, NETWIB_TRUE));
  netwib_er(netwib_kbd_ctl_set_line(pio, NETWIB_FALSE));
  netwib_er(netwib_io_read(pio, &bukbdata));
  netwib_er(netwibtest_buf_content3(&bukbdata, "'a'"));
  netwib_er(netwib_buf_close(&bukbdata));

  puts("Press key 'a' (nothing is displayed)");
  netwib_er(netwib_buf_init_mallocdefault(&bukbdata));
  netwib_er(netwib_kbd_ctl_set_echo(pio, NETWIB_FALSE));
  netwib_er(netwib_kbd_ctl_set_line(pio, NETWIB_FALSE));
  netwib_er(netwib_io_read(pio, &bukbdata));
  netwib_er(netwibtest_buf_content3(&bukbdata, "'a'"));
  netwib_er(netwib_buf_close(&bukbdata));

  puts("You have 3 seconds to press keys 'a' 'a' 'a'");
  netwib_er(netwib_time_sleep_sec(3));
  puts("Now, you must have finished");
  netwib_er(netwib_time_sleep_sec(1));
  netwib_er(netwib_kbd_ctl_set_purge(pio));

  puts("Press key 's'");
  netwib_er(netwib_buf_init_mallocdefault(&bukbdata));
   netwib_er(netwib_kbd_ctl_set_echo(pio, NETWIB_TRUE));
  netwib_er(netwib_kbd_ctl_set_line(pio, NETWIB_FALSE));
  netwib_er(netwib_io_read(pio, &bukbdata));
  netwib_er(netwibtest_buf_content3(&bukbdata, "'s'"));
  netwib_er(netwib_buf_close(&bukbdata));

  puts("Enter \"sdf\", then press 'Return'");
  netwib_er(netwib_buf_init_mallocdefault(&bukbdata));
  netwib_er(netwib_kbd_ctl_set_echo(pio, NETWIB_TRUE));
  netwib_er(netwib_kbd_ctl_set_line(pio, NETWIB_TRUE));
  netwib_er(netwib_io_wait_read(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_read(pio, &bukbdata));
  netwib_er(netwibtest_buf_content3(&bukbdata, "'sdf'"));
  netwib_er(netwib_buf_close(&bukbdata));

  puts("Enter \"sdf\", then press 'Return' (nothing is displayed)");
  netwib_er(netwib_buf_init_mallocdefault(&bukbdata));
  netwib_er(netwib_kbd_ctl_set_echo(pio, NETWIB_FALSE));
  netwib_er(netwib_kbd_ctl_set_line(pio, NETWIB_TRUE));
  netwib_er(netwib_io_wait_read(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_read(pio, &bukbdata));
  netwib_er(netwibtest_buf_content3(&bukbdata, "'sdf'"));
  netwib_er(netwib_buf_close(&bukbdata));

  puts("Press key 'a'");
  netwib_er(netwib_buf_init_mallocdefault(&bukbdata));
  netwib_er(netwib_kbd_ctl_set_echo(pio, NETWIB_TRUE));
  netwib_er(netwib_kbd_ctl_set_line(pio, NETWIB_FALSE));
  netwib_er(netwib_io_wait_read(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_read(pio, &bukbdata));
  netwib_er(netwibtest_buf_content3(&bukbdata, "'a'"));
  netwib_er(netwib_buf_close(&bukbdata));

  puts("Press key 'a' (nothing is displayed)");
  netwib_er(netwib_buf_init_mallocdefault(&bukbdata));
  netwib_er(netwib_kbd_ctl_set_echo(pio, NETWIB_FALSE));
  netwib_er(netwib_kbd_ctl_set_line(pio, NETWIB_FALSE));
  netwib_er(netwib_io_wait_read(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_read(pio, &bukbdata));
  netwib_er(netwibtest_buf_content3(&bukbdata, "'a'"));
  netwib_er(netwib_buf_close(&bukbdata));
  netwib_er(netwib_kbd_ctl_set_echo(pio, NETWIB_TRUE));

  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_kbd_ctl(void)
{
  netwib_io_restype res;
  netwib_io *pio;

  puts("++++----\nTest of ctl");

  netwib_er(netwib_io_init_kbddefault(&pio));

  netwib_er(netwib_io_ctl_get_res(pio, NETWIB_IO_WAYTYPE_READ, &res));
  if (res != NETWIB_IO_RESTYPE_KBD) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_kbd(void)
{
  puts("###########################################\nTest of sys/kbd");

  if (netwibtest_testkbdlevel) {
    netwib_er(netwib_io_init_kbddefault_test());
    netwib_er(netwib_io_init_kbd_fd_test());
    netwib_er(netwib_io_init_kbd_handle_test());
    netwib_er(test_kbd_waitreadunread());
    netwib_er(test_kbd_ctl());
  }
  return(NETWIB_ERR_OK);
}
