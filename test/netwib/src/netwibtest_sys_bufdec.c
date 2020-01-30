
#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_decodetype_test(void)
{
  netwib_buf buf;

  puts("++++----\nTest of netwib_buf_append_decodetype");
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_buf_append_decodetype(NETWIB_DECODETYPE_HEXA, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "hexadecimal"));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_decodetype_init_kbd_test(void)
{
  netwib_buf buf;
  netwib_decodetype decodetype;

  puts("++++----\nTest of netwib_decodetype_init_kbd");
  netwib_er(netwib_buf_init_ext_string("Select...", &buf));
  netwib_er(netwib_decodetype_init_kbd(&buf, NETWIB_DECODETYPE_HEXA,
                                       &decodetype));

  netwib_er(netwib_decodetype_init_kbd(NULL, NETWIB_DECODETYPE_MIXED,
                                       &decodetype));

  netwib_er(netwib_decodetype_init_kbd(NULL, NETWIB_DECODETYPE_INIT_KBD_NODEF,
                                       &decodetype));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_bufdec(void)
{
  puts("###########################################\nTest of sys/bufdec");
  netwib_er(netwib_buf_append_decodetype_test());
  if (netwibtest_testkbdlevel == 2) {
    netwib_er(netwib_decodetype_init_kbd_test());
  }

  return(NETWIB_ERR_OK);
}
