#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_filename_exists_test2(netwib_conststring filename,
                                               netwib_bool wantedbool)
{
  netwib_buf buf;
  netwib_bool b;

  netwib_er(netwib_buf_init_ext_string(filename, &buf));
  netwib_er(netwib_filename_exists(&buf, &b));
  if (b != wantedbool) {
    if (b) {
      netwib_er(netwib_fmt_display("Filename %s exists\n", filename));
    } else {
      netwib_er(netwib_fmt_display("Filename %s does not exist\n", filename));
    }
    return(NETWIBTEST_ERR_8);
  }

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_filename_exists_test(void)
{
  puts("++++----\nTest of netwib_filename_exists");

  netwib_er(netwib_filename_exists_test2("./files/record", NETWIB_TRUE));

#ifdef NETWIBTESTDEF_FILE_NAME_1
  netwib_er(netwib_filename_exists_test2(NETWIBTESTDEF_FILE_NAME_1,
                                         NETWIBTESTDEF_FILE_EXISTS_1));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_2
  netwib_er(netwib_filename_exists_test2(NETWIBTESTDEF_FILE_NAME_2,
                                         NETWIBTESTDEF_FILE_EXISTS_2));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_3
  netwib_er(netwib_filename_exists_test2(NETWIBTESTDEF_FILE_NAME_3,
                                         NETWIBTESTDEF_FILE_EXISTS_3));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_4
  netwib_er(netwib_filename_exists_test2(NETWIBTESTDEF_FILE_NAME_4,
                                         NETWIBTESTDEF_FILE_EXISTS_4));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_5
  netwib_er(netwib_filename_exists_test2(NETWIBTESTDEF_FILE_NAME_5,
                                         NETWIBTESTDEF_FILE_EXISTS_5));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_6
  netwib_er(netwib_filename_exists_test2(NETWIBTESTDEF_FILE_NAME_6,
                                         NETWIBTESTDEF_FILE_EXISTS_6));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_7
  netwib_er(netwib_filename_exists_test2(NETWIBTESTDEF_FILE_NAME_7,
                                         NETWIBTESTDEF_FILE_EXISTS_7));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_8
  netwib_er(netwib_filename_exists_test2(NETWIBTESTDEF_FILE_NAME_8,
                                         NETWIBTESTDEF_FILE_EXISTS_8));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_9
  netwib_er(netwib_filename_exists_test2(NETWIBTESTDEF_FILE_NAME_9,
                                         NETWIBTESTDEF_FILE_EXISTS_9));
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_10
  netwib_er(netwib_filename_exists_test2(NETWIBTESTDEF_FILE_NAME_10,
                                         NETWIBTESTDEF_FILE_EXISTS_10));
#endif

#ifdef NETWIBTESTDEF_DIR_NAME_1
  netwib_er(netwib_filename_exists_test2(NETWIBTESTDEF_DIR_NAME_1, NETWIB_FALSE));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_2
  netwib_er(netwib_filename_exists_test2(NETWIBTESTDEF_DIR_NAME_2, NETWIB_FALSE));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_3
  netwib_er(netwib_filename_exists_test2(NETWIBTESTDEF_DIR_NAME_3, NETWIB_FALSE));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_4
  netwib_er(netwib_filename_exists_test2(NETWIBTESTDEF_DIR_NAME_4, NETWIB_FALSE));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_5
  netwib_er(netwib_filename_exists_test2(NETWIBTESTDEF_DIR_NAME_5, NETWIB_FALSE));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_6
  netwib_er(netwib_filename_exists_test2(NETWIBTESTDEF_DIR_NAME_6, NETWIB_FALSE));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_7
  netwib_er(netwib_filename_exists_test2(NETWIBTESTDEF_DIR_NAME_7, NETWIB_FALSE));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_8
  netwib_er(netwib_filename_exists_test2(NETWIBTESTDEF_DIR_NAME_8, NETWIB_FALSE));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_9
  netwib_er(netwib_filename_exists_test2(NETWIBTESTDEF_DIR_NAME_9, NETWIB_FALSE));
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_10
  netwib_er(netwib_filename_exists_test2(NETWIBTESTDEF_DIR_NAME_10, NETWIB_FALSE));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_filename_size_test(void)
{
  netwib_buf buf;
  netwib_uint32 s;

  puts("++++----\nTest of netwib_filename_size");

  netwib_er(netwib_buf_init_ext_string("./files/record", &buf));
  netwib_er(netwib_filename_size(&buf, &s));
  if (s != 1774) {
    netwib_er(netwib_fmt_display("Size of ./files/record is now %{uint32}\n", s));
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_buf_init_ext_string("./files", &buf));
  netwib_eir(netwib_filename_size(&buf, &s), NETWIB_ERR_PAFILENOTREG);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_filename_create_test(void)
{
  netwib_buf buf;
  netwib_io *pio;
  netwib_bool b;
  netwib_uint32 size;

  puts("++++----\nTest of netwib_filename_create");

  netwib_er(netwib_buf_init_ext_string("./files/filew", &buf));
  netwib_er(netwib_filename_exists(&buf, &b));
  if (b) {
    netwib_er(netwib_filename_remove(&buf));
  }
  netwib_er(netwib_filename_create(&buf));
  netwib_er(netwib_filename_exists(&buf, &b));
  if (!b) {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_buf_init_ext_string("./files/filew", &buf));
  netwib_er(netwib_io_init_file_write(&buf, &pio));
  netwib_er(netwib_io_write(pio, &buf));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_filename_size(&buf, &size));
  if (size != 13) {
    return(NETWIBTEST_ERR_2);
  }
  netwib_er(netwib_filename_create(&buf));
  netwib_er(netwib_filename_size(&buf, &size));
  if (size != 0) {
    return(NETWIBTEST_ERR_3);
  }
  netwib_er(netwib_filename_remove(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_filename_remove_test(void)
{
  netwib_buf buf;
  netwib_bool b;

  puts("++++----\nTest of netwib_filename_remove");

  netwib_er(netwib_buf_init_ext_string("./files/filew", &buf));
  netwib_er(netwib_filename_exists(&buf, &b));
  if (b) {
    netwib_er(netwib_filename_remove(&buf));
  }
  netwib_er(netwib_filename_create(&buf));
  netwib_er(netwib_filename_remove(&buf));
  netwib_er(netwib_filename_exists(&buf, &b));
  if (b) {
    return(NETWIBTEST_ERR_8);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_filename_rename_test(void)
{
  netwib_buf buf, buf2;
  netwib_bool b;

  puts("++++----\nTest of netwib_filename_rename");

  netwib_er(netwib_buf_init_ext_string("./files/filew", &buf));
  netwib_er(netwib_filename_exists(&buf, &b));
  if (b) {
    netwib_er(netwib_filename_remove(&buf));
  }
  netwib_er(netwib_buf_init_ext_string("./files/filew2", &buf2));
  buf2.totalsize--;
  netwib_er(netwib_filename_exists(&buf2, &b));
  if (b) {
    netwib_er(netwib_filename_remove(&buf2));
  }
  netwib_er(netwib_filename_create(&buf));
  netwib_er(netwib_filename_rename(&buf, &buf2));
  netwib_er(netwib_filename_exists(&buf, &b));
  if (b) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_filename_exists(&buf2, &b));
  if (!b) {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_filename_create(&buf));
  netwib_er(netwib_filename_rename(&buf, &buf2));
  netwib_er(netwib_filename_exists(&buf, &b));
  if (b) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_filename_exists(&buf2, &b));
  if (!b) {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_filename_remove(&buf2));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_filename_copy_test(void)
{
  netwib_buf buf, buf2;
  netwib_bool b;
  netwib_uint32 s;

  puts("++++----\nTest of netwib_filename_copy");

  netwib_er(netwib_buf_init_ext_string("./files/data", &buf));
  netwib_er(netwib_buf_init_ext_string("./files/filew2", &buf2));
  netwib_er(netwib_filename_exists(&buf2, &b));
  if (b) {
    netwib_er(netwib_filename_remove(&buf2));
  }
  netwib_er(netwib_filename_copy(&buf, &buf2));
  netwib_er(netwib_filename_size(&buf2, &s));
  if (s != 36) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_filename_remove(&buf2));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_filename(void)
{
  puts("###########################################\nTest of sys/filename");
  netwib_er(netwib_filename_exists_test());
  netwib_er(netwib_filename_size_test());
  netwib_er(netwib_filename_create_test());
  netwib_er(netwib_filename_remove_test());
  netwib_er(netwib_filename_rename_test());
  netwib_er(netwib_filename_copy_test());

  return(NETWIB_ERR_OK);
}
