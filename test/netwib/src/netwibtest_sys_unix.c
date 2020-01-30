#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_unix_symlink_test(void)
{

  puts("++++----\nTest of netwib_unix_symlink");

#if defined NETWIBDEF_SYSNAME_Unix
  {
    netwib_buf buf, buf2;
    netwib_bool b;

    netwib_er(netwib_buf_init_ext_string("files/data", &buf));
    buf.totalsize--;
    netwib_er(netwib_buf_init_ext_string("files/data_link", &buf2));
    netwib_er(netwib_filename_exists(&buf2, &b));
    if (b) {
      netwib_er(netwib_filename_remove(&buf2));
    }
    netwib_er(netwib_unix_symlink(&buf, &buf2));
    netwib_er(netwib_filename_exists(&buf2, &b));
    if (!b) {
      return(NETWIBTEST_ERR_1);
    }
    netwib_er(netwib_filename_remove(&buf2));
  }
#elif defined NETWIBDEF_SYSNAME_Windows
  netwib_eir(netwib_unix_symlink(NULL, NULL),
             NETWIB_ERR_LONOTSUPPORTED);
#else
#error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_unix_readlink_test(void)
{

  puts("++++----\nTest of netwib_unix_readlink");

#if defined NETWIBDEF_SYSNAME_Unix
  {
    netwib_buf buf, buf2;
    netwib_bool b;

    netwib_er(netwib_buf_init_ext_string("files/data_link", &buf2));
    netwib_er(netwib_filename_exists(&buf2, &b));
    if (b) {
      netwib_er(netwib_filename_remove(&buf2));
    }
    netwib_er(netwib_buf_init_ext_string("data", &buf));
    netwib_er(netwib_unix_symlink(&buf, &buf2));
    netwib_er(netwib_buf_init_mallocdefault(&buf));
    netwib_er(netwib_unix_readlink(&buf2, &buf));
    netwib_er(netwibtest_buf_content2(&buf, "data"));
    netwib_er(netwib_buf_close(&buf));
    netwib_er(netwib_filename_remove(&buf2));
  }
#elif defined NETWIBDEF_SYSNAME_Windows
  netwib_eir(netwib_unix_readlink(NULL, NULL),
             NETWIB_ERR_LONOTSUPPORTED);
#else
#error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_unix(void)
{
  puts("###########################################\nTest of sys/unix");
  netwib_er(netwib_unix_symlink_test());
  netwib_er(netwib_unix_readlink_test());

  return(NETWIB_ERR_OK);
}
