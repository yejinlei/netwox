#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_port_init_kbd_test(void)
{
  netwib_buf msg;
  netwib_port port;

  puts("++++----\nTest of netwib_port_init_kbd");

  netwib_er(netwib_buf_init_ext_string("Enter number \"123\", then key 'Return'",
                                   &msg));
  netwib_er(netwib_port_init_kbd(&msg, 0, 200, 100, &port));
  if (port != 123) {
    return(NETWIBTEST_ERR_8);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_port_init_buf_test(void)
{
  netwib_buf buf;
  netwib_port port;

  puts("++++----\nTest of netwib_port_init_buf");

  netwib_er(netwib_buf_init_ext_string("123", &buf));
  netwib_er(netwib_port_init_buf(&buf, &port));
  if (port != 123) {
    return(NETWIBTEST_ERR_8);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_port_test(void)
{
  netwib_buf buf;

  puts("++++----\nTest of netwib_buf_append_port");

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_port(123, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "123"));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_net_port(void)
{
  puts("###########################################\nTest of net/port");
  if (netwibtest_testkbdlevel == 2) {
    netwib_er(netwib_port_init_kbd_test());
  }
  netwib_er(netwib_port_init_buf_test());
  netwib_er(netwib_buf_append_port_test());

  return(NETWIB_ERR_OK);
}
