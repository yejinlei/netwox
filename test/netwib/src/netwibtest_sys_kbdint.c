#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_uint32_init_kbd_test(void)
{
  netwib_buf msg;
  netwib_uint32 number;

  puts("++++----\nTest of netwib_uint32_init_kbd");
  netwib_er(netwibtest_something_to_do("Please follow instructions..."));

  netwib_eir(netwib_uint32_init_kbd(NULL, 1, 2, 3, &number),
             NETWIB_ERR_PAINVALIDDEFAULT);
  netwib_eir(netwib_uint32_init_kbd(NULL, 10, 2, 3, &number),
             NETWIB_ERR_PAINVALIDRANGE);

  netwib_er(netwib_buf_init_ext_string("Enter number \"123\", then key 'Return'",
                                     &msg));
  netwib_er(netwib_uint32_init_kbd(&msg, 0, 200, 100, &number));
  if (number != 123) {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_buf_init_ext_string("Enter number \"123\", then key 'Return'",
                                     &msg));
  netwib_er(netwib_uint32_init_kbd(&msg, NETWIB_UINT32_INIT_KBD_NOMIN,
                                   200, 100, &number));
  if (number != 123) {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_buf_init_ext_string("Enter number \"123\", then key 'Return'",
                                     &msg));
  netwib_er(netwib_uint32_init_kbd(&msg, 10,
                                   NETWIB_UINT32_INIT_KBD_NOMAX, 100, &number));
  if (number != 123) {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_buf_init_ext_string("Enter number \"123\", then key 'Return'",
                                     &msg));
  netwib_er(netwib_uint32_init_kbd(&msg, 10, 200,
                                   NETWIB_UINT32_INIT_KBD_NODEF, &number));
  if (number != 123) {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_buf_init_ext_string("Press key 'Return'", &msg));
  netwib_er(netwib_uint32_init_kbd(&msg, 10, 200, 140, &number));
  if (number != 140) {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_buf_init_ext_string("Press \"12a\"Return, then \"123\"Return",
                                     &msg));
  netwib_er(netwib_uint32_init_kbd(&msg, 10, 200,
                                   NETWIB_UINT32_INIT_KBD_NODEF, &number));
  if (number != 123) {
    return(NETWIBTEST_ERR_8);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_uint64_init_kbd_test(void)
{
  netwib_buf msg;
  netwib_uint64 number, def;

  puts("++++----\nTest of netwib_uint64_init_kbd");
  netwib_er(netwibtest_something_to_do("Please follow instructions..."));

  netwib__uint64_init_uint32(12, def);
  netwib_er(netwib_buf_init_ext_string("Enter number \"123\", then key 'Return'",
                                     &msg));
  netwib_er(netwib_uint64_init_kbd(&msg, def, &number));
  netwib_er(netwibtest_uint64_content(number, "000000000000007B"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_int32_init_kbd_test(void)
{
  netwib_buf msg;
  netwib_int32 number;

  puts("++++----\nTest of netwib_int32_init_kbd");
  netwib_er(netwibtest_something_to_do("Please follow instructions..."));

  netwib_eir(netwib_int32_init_kbd(NULL, 1, 2, 3, &number),
             NETWIB_ERR_PAINVALIDDEFAULT);
  netwib_eir(netwib_int32_init_kbd(NULL, 10, 2, 3, &number),
             NETWIB_ERR_PAINVALIDRANGE);

  netwib_er(netwib_buf_init_ext_string("Enter number \"123\", then key 'Return'",
                                     &msg));
  netwib_er(netwib_int32_init_kbd(&msg, 0, 200, 100, &number));
  if (number != 123) {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_buf_init_ext_string("Enter number \"123\", then key 'Return'",
                                     &msg));
  netwib_er(netwib_int32_init_kbd(&msg, NETWIB_INT32_INIT_KBD_NOMIN,
                                  200, 100, &number));
  if (number != 123) {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_buf_init_ext_string("Enter number \"123\", then key 'Return'",
                                     &msg));
  netwib_er(netwib_int32_init_kbd(&msg, 10,
                                  NETWIB_INT32_INIT_KBD_NOMAX, 100, &number));
  if (number != 123) {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_buf_init_ext_string("Enter number \"123\", then key 'Return'",
                                     &msg));
  netwib_er(netwib_int32_init_kbd(&msg, 10, 200,
                                  NETWIB_INT32_INIT_KBD_NODEF, &number));
  if (number != 123) {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_buf_init_ext_string("Enter key 'Return'", &msg));
  netwib_er(netwib_int32_init_kbd(&msg, 10, 200, 140, &number));
  if (number != 140) {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_buf_init_ext_string("Press \"12a\"Return, then \"123\"Return",
                                     &msg));
  netwib_er(netwib_int32_init_kbd(&msg, 10, 200,
                                  NETWIB_INT32_INIT_KBD_NODEF, &number));
  if (number != 123) {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_buf_init_ext_string("Enter number \"-123\", then key 'Return'",
                                     &msg));
  netwib_er(netwib_int32_init_kbd(&msg, -1000, 200,
                                  NETWIB_INT32_INIT_KBD_NODEF, &number));
  if (number != -123) {
    return(NETWIBTEST_ERR_8);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_int64_init_kbd_test(void)
{
  netwib_buf msg;
  netwib_int64 number, def;

  puts("++++----\nTest of netwib_int64_init_kbd");
  netwib_er(netwibtest_something_to_do("Please follow instructions..."));

  netwib__int64_init_int32(12, def);
  netwib_er(netwib_buf_init_ext_string("Enter number \"123\", then key 'Return'",
                                     &msg));
  netwib_er(netwib_int64_init_kbd(&msg, def, &number));
  netwib_er(netwibtest_int64_content(number, "000000000000007B"));

  netwib_er(netwib_buf_init_ext_string("Enter number \"-123\", then key 'Return'",
                                     &msg));
  netwib_er(netwib_int64_init_kbd(&msg, def, &number));
  netwib_er(netwibtest_int64_content(number, "FFFFFFFFFFFFFF85"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_kbdint(void)
{
  puts("###########################################\nTest of sys/kbdint");

  if (netwibtest_testkbdlevel == 2) {
    netwib_er(netwib_uint32_init_kbd_test());
    netwib_er(netwib_uint64_init_kbd_test());
    netwib_er(netwib_int32_init_kbd_test());
    netwib_er(netwib_int64_init_kbd_test());
  }

  return(NETWIB_ERR_OK);
}
