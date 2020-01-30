#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_dir_init_test(void)
{
  netwib_buf buf;
  netwib_dir *pd;

  puts("++++----\nTest of netwib_dir_init");
  netwib_er(netwib_buf_init_ext_string("./files", &buf));
  netwib_er(netwib_dir_init(&buf, &pd));
  netwib_er(netwib_dir_close(&pd));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_dir_close_test(void)
{
  netwib_buf buf;
  netwib_dir *pd;

  puts("++++----\nTest of netwib_dir_close");
  netwib_er(netwib_buf_init_ext_string("./files", &buf));
  netwib_er(netwib_dir_init(&buf, &pd));
  netwib_er(netwib_dir_close(&pd));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_dir_ctl_set_test(void)
{
  netwib_buf buf;
  netwib_dir *pd;

  puts("++++----\nTest of netwib_dir_ctl_set");
  netwib_er(netwib_buf_init_ext_string("./files", &buf));
  netwib_er(netwib_dir_init(&buf, &pd));
  netwib_er(netwib_dir_ctl_set_rewind(pd));
  netwib_er(netwib_dir_ctl_set_rewind(pd));
  netwib_er(netwib_dir_close(&pd));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_dir_ctl_get_test(void)
{
  puts("++++----\nTest of netwib_dir_ctl_get");

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
static netwib_err netwib_dir_next_test(void)
{
  netwib_buf buf;
  netwib_dir *pd;
  netwib_err ret;
  netwib_uint32 count;

  puts("++++----\nTest of netwib_dir_next");
  netwib_er(netwib_buf_init_ext_string("./files", &buf));
  netwib_er(netwib_dir_init(&buf, &pd));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  count = 0;
  while (NETWIB_TRUE) {
    netwib__buf_reinit(&buf);
    ret = netwib_dir_next(pd, &buf);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
      netwib_er(netwib_fmt_display("%s", "\t"));
    } else if (ret == NETWIB_ERR_DATAEND) {
      break;
    } else {
      return(ret);
    }
    count++;
  }
  if (count != 4) {
    netwib_er(netwib_fmt_display("\nnumber of files in files is now %{uint32}\n", count));
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_fmt_display("%s", "\n"));

  netwib_er(netwib_dir_ctl_set_rewind(pd));
  netwib_er(netwib_dir_next(pd, NULL));
  netwib_er(netwib_dir_ctl_set_rewind(pd));

  count = 0;
  while (NETWIB_TRUE) {
    netwib__buf_reinit(&buf);
    ret = netwib_dir_next(pd, &buf);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
      netwib_er(netwib_fmt_display("%s", "\t"));
    } else if (ret == NETWIB_ERR_DATAEND) {
      break;
    } else {
      return(ret);
    }
    count++;
  }
  if (count != 4) {
    netwib_er(netwib_fmt_display("number of files in files is now %{uint32}\n", count));
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_fmt_display("%s", "\n"));

  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_dir_close(&pd));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_dir(void)
{
  puts("###########################################\nTest of sys/dir");
  netwib_er(netwib_dir_init_test());
  netwib_er(netwib_dir_close_test());
  netwib_er(netwib_dir_ctl_set_test());
  netwib_er(netwib_dir_ctl_get_test());
  netwib_er(netwib_dir_next_test());

  return(NETWIB_ERR_OK);
}
