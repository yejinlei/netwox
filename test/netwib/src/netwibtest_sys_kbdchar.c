#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_char_init_kbd_test(void)
{
  netwib_buf msg, al;
  netwib_char c;

  puts("++++----\nTest of netwib_char_init_kbd");
  netwib_er(netwibtest_something_to_do("Please follow instructions..."));

  netwib_er(netwib_buf_init_ext_string("abcde", &al));
  netwib_eir(netwib_char_init_kbd(NULL, &al, 'f', &c),
            NETWIB_ERR_PAINVALIDDEFAULT);

  netwib_er(netwib_buf_init_ext_string("Press key 'a'", &msg));
  netwib_er(netwib_buf_init_ext_string("abcde", &al));
  netwib_er(netwib_char_init_kbd(&msg, &al, 'b', &c));
  if (c != 'a') {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_buf_init_ext_string("Press key 'Return'", &msg));
  netwib_er(netwib_buf_init_ext_string("abcde", &al));
  netwib_er(netwib_char_init_kbd(&msg, &al, 'b', &c));
  if (c != 'b') {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_buf_init_ext_string("Press key 'a'", &msg));
  netwib_er(netwib_buf_init_ext_string("abcde", &al));
  netwib_er(netwib_char_init_kbd(&msg, &al, NETWIB_CHAR_INIT_KBD_NODEF, &c));
  if (c != 'a') {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_buf_init_ext_string("Press key 'a'", &msg));
  netwib_er(netwib_char_init_kbd(&msg, NULL, NETWIB_CHAR_INIT_KBD_NODEF, &c));
  if (c != 'a') {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_buf_init_ext_string("Press key 'a'", &msg));
  netwib_er(netwib_char_init_kbd(&msg, NULL, 'b', &c));
  if (c != 'a') {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_buf_init_ext_string("Press key 'z', then key 'a'", &msg));
  netwib_er(netwib_buf_init_ext_string("abcde", &al));
  netwib_er(netwib_char_init_kbd(&msg, &al, 'b', &c));
  if (c != 'a') {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_buf_init_ext_string("Press key 'z', then key 'Return'", &msg));
  netwib_er(netwib_buf_init_ext_string("abcde", &al));
  netwib_er(netwib_char_init_kbd(&msg, &al, 'b', &c));
  if (c != 'b') {
    return(NETWIBTEST_ERR_8);
  }

  puts("Press key 'a'");
  netwib_er(netwib_char_init_kbd(NULL, NULL, NETWIB_CHAR_INIT_KBD_NODEF, &c));
  if (c != 'a') {
    return(NETWIBTEST_ERR_8);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_kbdchar(void)
{
  puts("###########################################\nTest of sys/kbdchar");
  if (netwibtest_testkbdlevel) {
    netwib_er(netwib_char_init_kbd_test());
  }
  return(NETWIB_ERR_OK);
}
