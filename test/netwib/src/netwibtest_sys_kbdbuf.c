#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_kbd_test(void)
{
  netwib_buf msg, def, buf;

  puts("++++----\nTest of netwib_buf_append_kbd");
  netwib_er(netwibtest_something_to_do("Please follow instructions..."));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("Enter text \"sdf\", then key 'Return'",
                                   &msg));
  netwib_er(netwib_buf_init_ext_string("hello", &def));
  netwib_er(netwib_buf_append_kbd(&msg, &def, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "sdf"));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("Enter text \"sdf\", then key 'Return'",
                                   &msg));
  netwib_er(netwib_buf_append_kbd(&msg, NULL, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "sdf"));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("Press key 'Return'", &msg));
  netwib_er(netwib_buf_append_kbd(&msg, NULL, &buf));
  netwib_er(netwibtest_buf_content2(&buf, ""));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("Press key 'Return', then key 'e'",&msg));
  netwib_er(netwib_buf_init_ext_string("hello", &def));
  netwib_er(netwib_buf_append_kbd(&msg, &def, &buf));
  netwib_er(netwibtest_buf_content2(&buf, ""));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("Press key 'Return', then key 'd'",&msg));
  netwib_er(netwib_buf_init_ext_string("hello", &def));
  netwib_er(netwib_buf_append_kbd(&msg, &def, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "hello"));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_password_test(void)
{
  netwib_buf msg, def, buf;

  puts("++++----\nTest of netwib_buf_append_password");
  netwib_er(netwibtest_something_to_do("Please follow instructions..."));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("Enter text \"sdf\", then key 'Return'",
                                     &msg));
  netwib_er(netwib_buf_init_ext_string("hello", &def));
  netwib_er(netwib_buf_append_password(&msg, &def, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "sdf"));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("Enter text \"sdf\", then key 'Return'",
                                     &msg));
  netwib_er(netwib_buf_append_password(&msg, NULL, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "sdf"));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("Press key 'Return'", &msg));
  netwib_er(netwib_buf_append_password(&msg, NULL, &buf));
  netwib_er(netwibtest_buf_content2(&buf, ""));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("Press key 'Return', then key 'e'",&msg));
  netwib_er(netwib_buf_init_ext_string("hello", &def));
  netwib_er(netwib_buf_append_password(&msg, &def, &buf));
  netwib_er(netwibtest_buf_content2(&buf, ""));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("Press key 'Return', then key 'd'",&msg));
  netwib_er(netwib_buf_init_ext_string("hello", &def));
  netwib_er(netwib_buf_append_password(&msg, &def, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "hello"));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_kbdbuf(void)
{
  puts("###########################################\nTest of sys/kbdbuf");
  if (netwibtest_testkbdlevel == 2) {
    netwib_er(netwib_buf_append_kbd_test());
    netwib_er(netwib_buf_append_password_test());
  }
  return(NETWIB_ERR_OK);
}
