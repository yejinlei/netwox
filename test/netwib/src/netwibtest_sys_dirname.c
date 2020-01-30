#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_dirname_secure_test2(netwib_conststring dirnamestr,
                                              netwib_bool wantedbool)
{
  netwib_buf buf;
  netwib_bool b;

  netwib_er(netwib_buf_init_ext_string(dirnamestr, &buf));
  netwib_er(netwib_dirname_secure(&buf, &b));
  if (b != wantedbool) {
    netwib_er(netwib_fmt_display("Dir %s is %{bool}\n", dirnamestr, b));
    return(NETWIBTEST_ERR_8);
  }

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_dirname_secure_test(void)
{

  puts("++++----\nTest of netwib_dirname_secure");

  puts("Note : please ignore fd leak created by getpwnam under Solaris 2.5.1");

#ifdef NETWIBTESTDEF_DIRSECURE_NAME_1
  netwib_er(netwib_dirname_secure_test2(NETWIBTESTDEF_DIRSECURE_NAME_1,
                                        NETWIBTESTDEF_DIRSECURE_SECURE_1));
#endif
#ifdef NETWIBTESTDEF_DIRSECURE_NAME_2
  netwib_er(netwib_dirname_secure_test2(NETWIBTESTDEF_DIRSECURE_NAME_2,
                                        NETWIBTESTDEF_DIRSECURE_SECURE_2));
#endif
#ifdef NETWIBTESTDEF_DIRSECURE_NAME_3
  netwib_er(netwib_dirname_secure_test2(NETWIBTESTDEF_DIRSECURE_NAME_3,
                                        NETWIBTESTDEF_DIRSECURE_SECURE_3));
#endif
#ifdef NETWIBTESTDEF_DIRSECURE_NAME_4
  netwib_er(netwib_dirname_secure_test2(NETWIBTESTDEF_DIRSECURE_NAME_4,
                                        NETWIBTESTDEF_DIRSECURE_SECURE_4));
#endif
#ifdef NETWIBTESTDEF_DIRSECURE_NAME_5
  netwib_er(netwib_dirname_secure_test2(NETWIBTESTDEF_DIRSECURE_NAME_5,
                                        NETWIBTESTDEF_DIRSECURE_SECURE_5));
#endif
#ifdef NETWIBTESTDEF_DIRSECURE_NAME_6
  netwib_er(netwib_dirname_secure_test2(NETWIBTESTDEF_DIRSECURE_NAME_6,
                                        NETWIBTESTDEF_DIRSECURE_SECURE_6));
#endif
#ifdef NETWIBTESTDEF_DIRSECURE_NAME_7
  netwib_er(netwib_dirname_secure_test2(NETWIBTESTDEF_DIRSECURE_NAME_7,
                                        NETWIBTESTDEF_DIRSECURE_SECURE_7));
#endif
#ifdef NETWIBTESTDEF_DIRSECURE_NAME_8
  netwib_er(netwib_dirname_secure_test2(NETWIBTESTDEF_DIRSECURE_NAME_8,
                                        NETWIBTESTDEF_DIRSECURE_SECURE_8));
#endif
#ifdef NETWIBTESTDEF_DIRSECURE_NAME_9
  netwib_er(netwib_dirname_secure_test2(NETWIBTESTDEF_DIRSECURE_NAME_9,
                                        NETWIBTESTDEF_DIRSECURE_SECURE_9));
#endif
#ifdef NETWIBTESTDEF_DIRSECURE_NAME_10
  netwib_er(netwib_dirname_secure_test2(NETWIBTESTDEF_DIRSECURE_NAME_10,
                                        NETWIBTESTDEF_DIRSECURE_SECURE_10));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_dirname_cwd_test(void)
{
  netwib_buf buf;

  puts("++++----\nTest of netwib_dirname_cwd");

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_dirname_cwd(&buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_fmt_display("\n"));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_dirname_exists_test2(netwib_conststring dirnamestr,
                                              netwib_bool wantedbool)
{
  netwib_buf buf;
  netwib_bool b;

  netwib_er(netwib_buf_init_ext_string(dirnamestr, &buf));
  netwib_er(netwib_dirname_exists(&buf, &b));
  if (b != wantedbool) {
    if (b) {
      netwib_er(netwib_fmt_display("Dirname %s exists\n", dirnamestr));
    } else {
      netwib_er(netwib_fmt_display("Dirname %s does not exist\n", dirnamestr));
    }
    return(NETWIBTEST_ERR_8);
  }

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_dirname_exists_test(void)
{
  puts("++++----\nTest of netwib_dirname_exists");

#ifdef NETWIBTESTDEF_DIR_NAME_1
  netwib_er(netwib_dirname_exists_test2(NETWIBTESTDEF_DIR_NAME_1,
                                        NETWIBTESTDEF_DIR_EXISTS_1));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_2
  netwib_er(netwib_dirname_exists_test2(NETWIBTESTDEF_DIR_NAME_2,
                                        NETWIBTESTDEF_DIR_EXISTS_2));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_3
  netwib_er(netwib_dirname_exists_test2(NETWIBTESTDEF_DIR_NAME_3,
                                        NETWIBTESTDEF_DIR_EXISTS_3));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_4
  netwib_er(netwib_dirname_exists_test2(NETWIBTESTDEF_DIR_NAME_4,
                                        NETWIBTESTDEF_DIR_EXISTS_4));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_5
  netwib_er(netwib_dirname_exists_test2(NETWIBTESTDEF_DIR_NAME_5,
                                        NETWIBTESTDEF_DIR_EXISTS_5));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_6
  netwib_er(netwib_dirname_exists_test2(NETWIBTESTDEF_DIR_NAME_6,
                                        NETWIBTESTDEF_DIR_EXISTS_6));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_7
  netwib_er(netwib_dirname_exists_test2(NETWIBTESTDEF_DIR_NAME_7,
                                        NETWIBTESTDEF_DIR_EXISTS_7));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_8
  netwib_er(netwib_dirname_exists_test2(NETWIBTESTDEF_DIR_NAME_8,
                                        NETWIBTESTDEF_DIR_EXISTS_8));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_9
  netwib_er(netwib_dirname_exists_test2(NETWIBTESTDEF_DIR_NAME_9,
                                        NETWIBTESTDEF_DIR_EXISTS_9));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_10
  netwib_er(netwib_dirname_exists_test2(NETWIBTESTDEF_DIR_NAME_10,
                                        NETWIBTESTDEF_DIR_EXISTS_10));
#endif

#ifdef NETWIBTESTDEF_FILE_NAME_1
  netwib_er(netwib_dirname_exists_test2(NETWIBTESTDEF_FILE_NAME_1, NETWIB_FALSE));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_2
  netwib_er(netwib_dirname_exists_test2(NETWIBTESTDEF_FILE_NAME_2, NETWIB_FALSE));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_3
  netwib_er(netwib_dirname_exists_test2(NETWIBTESTDEF_FILE_NAME_3, NETWIB_FALSE));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_4
  netwib_er(netwib_dirname_exists_test2(NETWIBTESTDEF_FILE_NAME_4, NETWIB_FALSE));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_5
  netwib_er(netwib_dirname_exists_test2(NETWIBTESTDEF_FILE_NAME_5, NETWIB_FALSE));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_6
  netwib_er(netwib_dirname_exists_test2(NETWIBTESTDEF_FILE_NAME_6, NETWIB_FALSE));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_7
  netwib_er(netwib_dirname_exists_test2(NETWIBTESTDEF_FILE_NAME_7, NETWIB_FALSE));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_8
  netwib_er(netwib_dirname_exists_test2(NETWIBTESTDEF_FILE_NAME_8, NETWIB_FALSE));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_9
  netwib_er(netwib_dirname_exists_test2(NETWIBTESTDEF_FILE_NAME_9, NETWIB_FALSE));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_10
  netwib_er(netwib_dirname_exists_test2(NETWIBTESTDEF_FILE_NAME_10, NETWIB_FALSE));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_dirname_create_test(void)
{
  netwib_buf buf;
  netwib_bool b;

  puts("++++----\nTest of netwib_dirname_create");

  netwib_er(netwib_buf_init_ext_string("./tmp_dir", &buf));
  netwib_er(netwib_dirname_exists(&buf, &b));
  if (b) {
    netwib_er(netwib_dirname_remove(&buf));
  }
  netwib_er(netwib_dirname_create(&buf));
  netwib_er(netwib_dirname_exists(&buf, &b));
  if (!b) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_dirname_remove(&buf));

  netwib_er(netwib_buf_init_ext_string("./tmp_dir/subdir", &buf));
  netwib_er(netwib_dirname_create(&buf));
  netwib_er(netwib_dirname_exists(&buf, &b));
  netwib_er(netwib_dirname_remove(&buf));
  netwib_er(netwib_buf_init_ext_string("./tmp_dir", &buf));
  netwib_er(netwib_dirname_remove(&buf));
  if (!b) {
    return(NETWIBTEST_ERR_8);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_dirname_remove_test(void)
{
  netwib_buf buf;
  netwib_bool b;

  puts("++++----\nTest of netwib_dirname_remove");

  netwib_er(netwib_buf_init_ext_string("./tmp_dir", &buf));
  netwib_er(netwib_dirname_create(&buf));
  netwib_er(netwib_dirname_remove(&buf));
  netwib_er(netwib_dirname_exists(&buf, &b));
  if (b) {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_buf_init_ext_string("./tmp_dir", &buf));
  netwib_eir(netwib_dirname_remove(&buf), NETWIB_ERR_NOTFOUND);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_dirname_rename_test(void)
{
  netwib_buf buf1, buf2;
  netwib_bool b;

  puts("++++----\nTest of netwib_dirname_rename");

  netwib_er(netwib_buf_init_ext_string("./tmp_dir1", &buf1));
  netwib_er(netwib_buf_init_ext_string("./tmp_dir2", &buf2));
  netwib_er(netwib_dirname_exists(&buf1, &b));
  if (b) {
    netwib_er(netwib_dirname_remove(&buf1));
  }
  netwib_er(netwib_dirname_exists(&buf2, &b));
  if (b) {
    netwib_er(netwib_dirname_remove(&buf2));
  }
  netwib_er(netwib_dirname_create(&buf1));
  netwib_er(netwib_dirname_exists(&buf1, &b));
  if (!b) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_dirname_rename(&buf1, &buf2));
  netwib_er(netwib_dirname_exists(&buf1, &b));
  if (b) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_dirname_exists(&buf2, &b));
  if (!b) {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_dirname_remove(&buf2));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_dirname(void)
{
  puts("###########################################\nTest of sys/dirname");
  netwib_er(netwib_dirname_secure_test());
  netwib_er(netwib_dirname_cwd_test());
  netwib_er(netwib_dirname_exists_test());
  netwib_er(netwib_dirname_create_test());
  netwib_er(netwib_dirname_remove_test());
  netwib_er(netwib_dirname_rename_test());

  return(NETWIB_ERR_OK);
}
