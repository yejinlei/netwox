#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_file_test(void)
{
  netwib_buf buf, filedata;
  netwib_io *pio;
  netwib_bool b;

  puts("++++----\nTest of netwib_io_init_file");

  netwib_er(netwib_buf_init_ext_string("./files/record", &buf));
  netwib_er(netwib_io_init_file(&buf, NETWIB_FILE_INITTYPE_READ, NETWIB_FALSE,
                                &pio));
  netwib_er(netwib_io_close(&pio));

  netwib_er(netwib_buf_init_ext_string("./files/record", &buf));
  netwib_er(netwib_io_init_file_read(&buf, &pio));
  netwib_er(netwib_io_close(&pio));

  netwib_er(netwib_buf_init_ext_string("./files/record", &buf));
  netwib_er(netwib_io_init_file_textread(&buf, &pio));
  netwib_er(netwib_io_close(&pio));

  netwib_er(netwib_buf_init_ext_string("./files/recazazeazord", &buf));
  netwib_eir(netwib_io_init_file(&buf, NETWIB_FILE_INITTYPE_READ, NETWIB_FALSE,
                                 &pio), NETWIB_ERR_NOTFOUND);

  netwib_er(netwib_buf_init_ext_string("./files/filew", &buf));
  netwib_er(netwib_filename_exists(&buf, &b));
  if (b) {
    netwib_er(netwib_filename_remove(&buf));
  }
  netwib_er(netwib_io_init_file(&buf, NETWIB_FILE_INITTYPE_WRITE, NETWIB_FALSE,
                                &pio));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_filename_exists(&buf, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_init_file(&buf, NETWIB_FILE_INITTYPE_APPEND, NETWIB_FALSE,
                                &pio));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_filename_remove(&buf));
  netwib_er(netwib_io_init_file(&buf, NETWIB_FILE_INITTYPE_APPEND, NETWIB_FALSE,
                                &pio));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_filename_remove(&buf));

  netwib_er(netwib_buf_init_ext_string("./files/filew", &buf));
  netwib_er(netwib_io_init_file_write(&buf, &pio));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_filename_exists(&buf, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_init_file_append(&buf, &pio));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_filename_remove(&buf));

  netwib_er(netwib_buf_init_ext_string("./files/filew", &buf));
  netwib_er(netwib_io_init_file_textwrite(&buf, &pio));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_filename_exists(&buf, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_init_file_textappend(&buf, &pio));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_filename_remove(&buf));

  /**/
  netwib_er(netwib_buf_init_mallocdefault(&filedata));
  netwib_er(netwib_buf_init_ext_string("./files/filew", &buf));
  netwib_er(netwib_filename_exists(&buf, &b));
  if (b) {
    netwib_er(netwib_filename_remove(&buf));
  }
  netwib_er(netwib_io_init_file_rdwr(&buf, &pio));
  netwib_er(netwib_buf_append_string("abcdef", &filedata));
  netwib_er(netwib_io_write(pio, &filedata));
  netwib_er(netwib_file_ctl_set_seek_begin(pio, 3));
  netwib__buf_reinit(&filedata);
  netwib_er(netwib_io_read(pio, &filedata));
  netwib_er(netwibtest_buf_content3(&filedata, "'def'"));
  netwib_er(netwib_file_ctl_set_seek_begin(pio, 1));
  netwib__buf_reinit(&filedata);
  netwib_er(netwib_buf_append_string("BCD", &filedata));
  netwib_er(netwib_io_write(pio, &filedata));
  netwib__buf_reinit(&filedata);
  netwib_er(netwib_io_read(pio, &filedata));
  netwib_er(netwibtest_buf_content3(&filedata, "'ef'"));
  netwib_er(netwib_file_ctl_set_seek_begin(pio, 0));
  netwib__buf_reinit(&filedata);
  netwib_er(netwib_io_read(pio, &filedata));
  netwib_er(netwibtest_buf_content3(&filedata, "'aBCDef'"));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_filename_remove(&buf));
  netwib_er(netwib_buf_close(&filedata));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_file_waitwrite(void)
{
  netwib_buf buf, bufdata;
  netwib_io *pio;
  netwib_bool b;

  puts("++++----\nTest of wait, write");

  netwib_er(netwib_buf_init_ext_string("./files/filew", &buf));
  netwib_er(netwib_filename_exists(&buf, &b));
  if (b) {
    netwib_er(netwib_filename_remove(&buf));
  }
  netwib_er(netwib_io_init_file_write(&buf, &pio));

  netwib_er(netwib_io_wait_write(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_buf_init_ext_string("hello1", &bufdata));
  netwib_er(netwib_io_write(pio, &bufdata));

  netwib_er(netwib_io_wait_write(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_ctl_set_end_write(pio));
  netwib_eir(netwib_io_wait_write(pio, NETWIB_TIME_INFINITE, &b),
             NETWIB_ERR_LOOBJWRITENOTSUPPORTED);

  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_filename_remove(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_file_waitreadunread(void)
{
  netwib_buf buf, bufdata;
  netwib_io *pio;
  netwib_bool b;

  puts("++++----\nTest of wait, read, unread");

  netwib_er(netwib_buf_init_ext_string("./files/data", &buf));
  netwib_er(netwib_buf_init_mallocdefault(&bufdata));

  netwib_er(netwib_io_init_file_read(&buf, &pio));
  netwib_er(netwib_io_wait_read(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_read(pio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'abcdef' 0D0A 'ghij' 0D0A 'klmnop' 0D0A 'qrstuvw' 0D0A 'xyz' 0D0A"));
  netwib_er(netwib_io_close(&pio));

  netwib_er(netwib_buf_close(&bufdata));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_file_ctl(void)
{
  netwib_buf buf, bufdata;
  netwib_io_restype res;
  netwib_io *pio;
  netwib_int64 pos64;
  netwib_int32 pos;
  netwib_uint32 size;

  puts("++++----\nTest of ctl");

  netwib_er(netwib_buf_init_ext_string("./files/data", &buf));
  netwib_er(netwib_io_init_file_read(&buf, &pio));

  netwib_er(netwib_io_ctl_get_res(pio, NETWIB_IO_WAYTYPE_READ, &res));
  if (res != NETWIB_IO_RESTYPE_FILE) {
    return(NETWIBTEST_ERR_5);
  }

  /**/
  netwib_er(netwib_file_ctl_set_seek_begin(pio, 1));
  netwib_er(netwib_file_ctl_get_tell(pio, &pos));
  if (pos != 1) {
    return(NETWIBTEST_ERR_6);
  }
  netwib_er(netwib_file_ctl_set_seek_current(pio, 1));
  netwib_er(netwib_file_ctl_get_tell(pio, &pos));
  if (pos != 2) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_file_ctl_set_seek_current(pio, -1));
  netwib_er(netwib_file_ctl_get_tell(pio, &pos));
  if (pos != 1) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_file_ctl_set_seek_end(pio, -1));
  netwib_er(netwib_file_ctl_get_tell(pio, &pos));
  if (pos != 35) {
    return(NETWIBTEST_ERR_9);
  }
  netwib_er(netwib_buf_init_mallocdefault(&bufdata));
  netwib_er(netwib_io_read(pio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "0A"));
  netwib_er(netwib_buf_close(&bufdata));

  /**/
  netwib__int64_init_int32(1, pos64);
  netwib_er(netwib_file_ctl_set_seek64_begin(pio, &pos64));
  netwib_er(netwib_file_ctl_get_tell64(pio, &pos64));
  netwib_er(netwibtest_int64_content((netwib_int64)pos64,
                                     "0000000000000001"));
  netwib__int64_init_int32(1, pos64);
  netwib_er(netwib_file_ctl_set_seek64_current(pio, &pos64));
  netwib_er(netwib_file_ctl_get_tell64(pio, &pos64));
  netwib_er(netwibtest_int64_content((netwib_int64)pos64,
                                     "0000000000000002"));
  netwib__int64_init_int32(-1, pos64);
  netwib_er(netwib_file_ctl_set_seek64_current(pio, &pos64));
  netwib_er(netwib_file_ctl_get_tell64(pio, &pos64));
  netwib_er(netwibtest_int64_content((netwib_int64)pos64,
                                     "0000000000000001"));
  netwib__int64_init_int32(-1, pos64);
  netwib_er(netwib_file_ctl_set_seek64_end(pio, &pos64));
  netwib_er(netwib_file_ctl_get_tell64(pio, &pos64));
  netwib_er(netwibtest_int64_content((netwib_int64)pos64,
                                     "0000000000000023"));
  netwib_er(netwib_buf_init_mallocdefault(&bufdata));
  netwib_er(netwib_io_read(pio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "0A"));
  netwib_er(netwib_buf_close(&bufdata));

  netwib_er(netwib_io_close(&pio));

  /**/
  netwib__int64_init_int32(4, pos64);
  netwib_er(netwib_buf_init_ext_string("./files/filew", &buf));
  netwib_er(netwib_io_init_file_write(&buf, &pio));
  netwib_er(netwib_io_write(pio, &buf));
  netwib_er(netwib_file_ctl_set_truncate64(pio, &pos64));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_filename_size(&buf, &size));
  if (size != 4) {
    return(NETWIBTEST_ERR_2);
  }
  netwib_er(netwib_io_init_file_write(&buf, &pio));
  netwib_er(netwib_file_ctl_set_truncate(pio, 3));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_filename_size(&buf, &size));
  if (size != 3) {
    return(NETWIBTEST_ERR_2);
  }
  netwib_er(netwib_filename_remove(&buf));


  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_filetemp_test(void)
{
  netwib_buf buf;
  netwib_io *pio;

  puts("++++----\nTest of netwib_io_init_filetemp");

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_io_init_filetemp(NETWIB_TRUE, &buf, &pio));
  netwib_er(netwib_fmt_display("chosen %{buf}\n", &buf));
  netwib_er(netwib_io_write(pio, &buf));
  netwib_er(netwib_io_close(&pio));

  netwib_er(netwib_filename_remove(&buf));

  netwib_er(netwib_buf_close(&buf));

  /**/
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_string("./tmp", &buf));

  netwib_er(netwib_io_init_filetemp(NETWIB_TRUE, &buf, &pio));
  netwib_er(netwib_fmt_display("chosen %{buf}\n", &buf));
  netwib_er(netwib_io_write(pio, &buf));
  netwib_er(netwib_io_close(&pio));

  netwib_er(netwib_filename_remove(&buf));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_file(void)
{
  puts("###########################################\nTest of sys/file");
  netwib_er(netwib_io_init_file_test());
  netwib_er(test_file_waitwrite());
  netwib_er(test_file_waitreadunread());
  netwib_er(test_file_ctl());
  netwib_er(netwib_io_init_filetemp_test());

  return(NETWIB_ERR_OK);
}
