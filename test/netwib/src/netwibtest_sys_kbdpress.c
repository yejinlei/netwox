#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_kbd_press_test(void)
{
  netwib_buf buf;
  netwib_char c;

  puts("++++----\nTest of netwib_kbd_press");
  netwib_er(netwibtest_something_to_do("Press key 'Space'"));
  netwib_er(netwib_kbd_press(NULL, &c));
  if (c != ' ') {
    return(NETWIBTEST_ERR_2);
  }

  netwib_er(netwib_buf_init_ext_string("Press key 'Return'", &buf));
  netwib_er(netwib_kbd_press(&buf, NULL));

  netwib_er(netwib_buf_init_ext_string("Press key 'a'", &buf));
  netwib_er(netwib_kbd_press(&buf, &c));
  if (c != 'a') {
    return(NETWIBTEST_ERR_2);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_kbd_purge_test(void)
{
  netwib_buf buf;
  netwib_char c;

  puts("++++----\nTest of netwib_kbd_purge");
  netwib_er(netwibtest_something_to_do("You have 3 seconds to enter \"sdf\""));
  netwib_er(netwib_time_sleep_sec(3));
  puts("Now, you must have finished");
  netwib_er(netwib_time_sleep_sec(1));
  netwib_er(netwib_kbd_purge());
  netwib_er(netwib_kbd_purge());

  netwib_er(netwib_buf_init_ext_string("Press key 'a'", &buf));
  netwib_er(netwib_kbd_press(&buf, &c));
  if (c != 'a') {
    return(NETWIBTEST_ERR_2);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_kbdpress(void)
{
  puts("###########################################\nTest of sys/kbdpress");
  if (netwibtest_testkbdlevel) {
    netwib_er(netwib_kbd_press_test());
    netwib_er(netwib_kbd_purge_test());
  }

  return(NETWIB_ERR_OK);
}
