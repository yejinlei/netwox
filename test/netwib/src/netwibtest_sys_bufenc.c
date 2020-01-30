
#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_encodetype_test(void)
{
  netwib_buf buf;

  puts("++++----\nTest of netwib_buf_append_encodetype");
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_buf_append_encodetype(NETWIB_ENCODETYPE_DATA, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "exact data"));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_encodetype_init_kbd_test(void)
{
  netwib_buf buf;
  netwib_encodetype encodetype;

  puts("++++----\nTest of netwib_encodetype_init_kbd");
  netwib_er(netwib_buf_init_ext_string("Select...", &buf));
  netwib_er(netwib_encodetype_init_kbd(&buf, NETWIB_FALSE,
                                       NETWIB_ENCODETYPE_DATA, &encodetype));

  netwib_er(netwib_encodetype_init_kbd(NULL, NETWIB_FALSE,
                                       NETWIB_ENCODETYPE_ARRAY4, &encodetype));

  netwib_er(netwib_encodetype_init_kbd(NULL, NETWIB_FALSE,
                                       NETWIB_ENCODETYPE_INIT_KBD_NODEF,
                                       &encodetype));

  netwib_er(netwib_encodetype_init_kbd(NULL, NETWIB_TRUE,
                                       NETWIB_ENCODETYPE_ARRAY4, &encodetype));

  netwib_er(netwib_encodetype_init_kbd(NULL, NETWIB_TRUE,
                                       NETWIB_ENCODETYPE_INIT_KBD_NODEF,
                                       &encodetype));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_bufenc(void)
{
  puts("###########################################\nTest of sys/bufenc");
  netwib_er(netwib_buf_append_encodetype_test());
  if (netwibtest_testkbdlevel == 2) {
    netwib_er(netwib_encodetype_init_kbd_test());
  }

  return(NETWIB_ERR_OK);
}
