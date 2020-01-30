#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_device_init_kbd_test(void)
{
  netwib_buf device;

  puts("++++----\nTest of netwib_device_init_kbd");
  netwib_er(netwib_buf_init_mallocdefault(&device));
  netwib_er(netwib_device_init_kbd(NULL, NETWIB_DEVICE_INIT_KBD_NODEF, &device));
  netwib_er(netwib_fmt_display("Value : %{buf}\n", &device));
  netwib_er(netwib_buf_close(&device));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_device_hwtype_test(void)
{
  netwib_buf buf;
  puts("++++----\nTest of netwib_buf_append_device_hwtype");

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_device_hwtype(NETWIB_DEVICE_HWTYPE_ETHER, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "ethernet"));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_device_hwtype_init_kbd_test(void)
{
  netwib_buf buf;
  netwib_device_hwtype hwtype;

  puts("++++----\nTest of netwib_device_hwtype_init_kbd");
  netwib_er(netwib_buf_init_ext_string("Select...", &buf));
  netwib_er(netwib_device_hwtype_init_kbd(&buf,
                                          NETWIB_DEVICE_HWTYPE_ETHER,
                                          &hwtype));

  netwib_er(netwib_device_hwtype_init_kbd(NULL,
                                          NETWIB_DEVICE_HWTYPE_INIT_KBD_NODEF,
                                          &hwtype));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_device_dlttype_test(void)
{
  netwib_buf buf;
  puts("++++----\nTest of netwib_buf_append_device_dlttype");

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_device_dlttype(NETWIB_DEVICE_DLTTYPE_ECONET, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "Econet"));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_device_dlttype_init_kbd_test(void)
{
  netwib_buf buf;
  netwib_device_dlttype dlttype;

  puts("++++----\nTest of netwib_device_dlttype_init_kbd");
  netwib_er(netwib_buf_init_ext_string("Select...", &buf));
  netwib_er(netwib_device_dlttype_init_kbd(&buf,
                                           NETWIB_DEVICE_DLTTYPE_ETHER,
                                           &dlttype));

  netwib_er(netwib_device_dlttype_init_kbd(NULL,
                                           NETWIB_DEVICE_DLTTYPE_INIT_KBD_NODEF,
                                           &dlttype));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_net_device(void)
{
  puts("###########################################\nTest of net/device");

  if (netwibtest_testkbdlevel == 2) {
    netwib_er(netwib_device_init_kbd_test());
  }

  netwib_er(netwib_buf_append_device_hwtype_test());
  if (netwibtest_testkbdlevel == 2) {
    netwib_er(netwib_device_hwtype_init_kbd_test());
  }
  netwib_er(netwib_buf_append_device_dlttype_test());
  if (netwibtest_testkbdlevel == 2) {
    netwib_er(netwib_device_dlttype_init_kbd_test());
  }

  return(NETWIB_ERR_OK);
}
