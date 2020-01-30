#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_pathname_exists_test2(netwib_conststring pathname,
                                               netwib_bool wantedbool)
{
  netwib_buf buf;
  netwib_bool b;

  netwib_er(netwib_buf_init_ext_string(pathname, &buf));
  netwib_er(netwib_pathname_exists(&buf, &b));
  if (b != wantedbool) {
    if (b) {
      netwib_er(netwib_fmt_display("Pathname %s exists\n", pathname));
    } else {
      netwib_er(netwib_fmt_display("Pathname %s does not exist\n", pathname));
    }
    return(NETWIBTEST_ERR_8);
  }

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_pathname_exists_test(void)
{
  puts("++++----\nTest of netwib_pathname_exists");

  netwib_er(netwib_pathname_exists_test2("./files/record", NETWIB_TRUE));
  netwib_er(netwib_pathname_exists_test2("./genemaketest/fake", NETWIB_FALSE));

#ifdef NETWIBTESTDEF_DIR_NAME_1
  netwib_er(netwib_pathname_exists_test2(NETWIBTESTDEF_DIR_NAME_1,
                                         NETWIBTESTDEF_DIR_EXISTS_1));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_2
  netwib_er(netwib_pathname_exists_test2(NETWIBTESTDEF_DIR_NAME_2,
                                         NETWIBTESTDEF_DIR_EXISTS_2));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_3
  netwib_er(netwib_pathname_exists_test2(NETWIBTESTDEF_DIR_NAME_3,
                                         NETWIBTESTDEF_DIR_EXISTS_3));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_4
  netwib_er(netwib_pathname_exists_test2(NETWIBTESTDEF_DIR_NAME_4,
                                         NETWIBTESTDEF_DIR_EXISTS_4));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_5
  netwib_er(netwib_pathname_exists_test2(NETWIBTESTDEF_DIR_NAME_5,
                                         NETWIBTESTDEF_DIR_EXISTS_5));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_6
  netwib_er(netwib_pathname_exists_test2(NETWIBTESTDEF_DIR_NAME_6,
                                         NETWIBTESTDEF_DIR_EXISTS_6));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_7
  netwib_er(netwib_pathname_exists_test2(NETWIBTESTDEF_DIR_NAME_7,
                                         NETWIBTESTDEF_DIR_EXISTS_7));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_8
  netwib_er(netwib_pathname_exists_test2(NETWIBTESTDEF_DIR_NAME_8,
                                         NETWIBTESTDEF_DIR_EXISTS_8));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_9
  netwib_er(netwib_pathname_exists_test2(NETWIBTESTDEF_DIR_NAME_9,
                                         NETWIBTESTDEF_DIR_EXISTS_9));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_10
  netwib_er(netwib_pathname_exists_test2(NETWIBTESTDEF_DIR_NAME_10,
                                         NETWIBTESTDEF_DIR_EXISTS_10));
#endif

#ifdef NETWIBTESTDEF_FILE_NAME_1
  netwib_er(netwib_pathname_exists_test2(NETWIBTESTDEF_FILE_NAME_1,
                                         NETWIBTESTDEF_FILE_EXISTS_1));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_2
  netwib_er(netwib_pathname_exists_test2(NETWIBTESTDEF_FILE_NAME_2,
                                         NETWIBTESTDEF_FILE_EXISTS_2));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_3
  netwib_er(netwib_pathname_exists_test2(NETWIBTESTDEF_FILE_NAME_3,
                                         NETWIBTESTDEF_FILE_EXISTS_3));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_4
  netwib_er(netwib_pathname_exists_test2(NETWIBTESTDEF_FILE_NAME_4,
                                         NETWIBTESTDEF_FILE_EXISTS_4));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_5
  netwib_er(netwib_pathname_exists_test2(NETWIBTESTDEF_FILE_NAME_5,
                                         NETWIBTESTDEF_FILE_EXISTS_5));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_6
  netwib_er(netwib_pathname_exists_test2(NETWIBTESTDEF_FILE_NAME_6,
                                         NETWIBTESTDEF_FILE_EXISTS_6));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_7
  netwib_er(netwib_pathname_exists_test2(NETWIBTESTDEF_FILE_NAME_7,
                                         NETWIBTESTDEF_FILE_EXISTS_7));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_8
  netwib_er(netwib_pathname_exists_test2(NETWIBTESTDEF_FILE_NAME_8,
                                         NETWIBTESTDEF_FILE_EXISTS_8));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_9
  netwib_er(netwib_pathname_exists_test2(NETWIBTESTDEF_FILE_NAME_9,
                                         NETWIBTESTDEF_FILE_EXISTS_9));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_10
  netwib_er(netwib_pathname_exists_test2(NETWIBTESTDEF_FILE_NAME_10,
                                         NETWIBTESTDEF_FILE_EXISTS_10));
#endif

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_pathname(void)
{
  puts("###########################################\nTest of sys/pathname");
  netwib_er(netwib_pathname_exists_test());

  return(NETWIB_ERR_OK);
}
