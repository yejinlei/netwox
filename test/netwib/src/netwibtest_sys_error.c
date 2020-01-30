#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_err_display_test(void)
{
  puts("++++----\nTest of netwib_err_display");
  netwib_er(netwib_err_display(NETWIB_ERR_NOTFOUND, NETWIB_ERR_ENCODETYPE_TEXT));
  puts("");
  netwib_er(netwib_err_display(NETWIB_ERR_NOTFOUND,NETWIB_ERR_ENCODETYPE_NUMTEXT));
  puts("");
  netwib_er(netwib_err_display(NETWIB_ERR_NOTFOUND, NETWIB_ERR_ENCODETYPE_FULL));

  errno = 10;
  netwib_er(netwib_err_display(NETWIB_ERR_NOTFOUND, NETWIB_ERR_ENCODETYPE_TEXT));
  puts("");
  netwib_er(netwib_err_display(NETWIB_ERR_NOTFOUND,NETWIB_ERR_ENCODETYPE_NUMTEXT));
  puts("");
  netwib_er(netwib_err_display(NETWIB_ERR_NOTFOUND, NETWIB_ERR_ENCODETYPE_FULL));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_err_test(void)
{
  netwib_buf buf;

  puts("++++----\nTest of netwib_buf_append_err");
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_err(NETWIB_ERR_NOTFOUND,
                                  NETWIB_ERR_ENCODETYPE_NUMTEXT,
                                  &buf));
  netwib_er(netwibtest_buf_content2(&buf, "Error 1005 : not found"));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_error(void)
{
  puts("###########################################\nTest of sys/error");
  netwib_er(netwib_err_display_test());
  netwib_er(netwib_buf_append_err_test());

  return(NETWIB_ERR_OK);
}
