#include "netwibtest.h"


/*-------------------------------------------------------------*/
static netwib_err netwib_record_encodetype_init_kbd_test(void)
{
  netwib_buf buf;
  netwib_record_encodetype encodetype;

  puts("++++----\nTest of netwib_record_encodetype_init_kbd");
  netwib_er(netwib_buf_init_ext_string("Select...", &buf));
  netwib_er(netwib_record_encodetype_init_kbd(&buf, NETWIB_FALSE,
                                              NETWIB_RECORD_ENCODETYPE_HEXA0,
                                              &encodetype));

  netwib_er(netwib_record_encodetype_init_kbd(NULL, NETWIB_FALSE,
                                              NETWIB_RECORD_ENCODETYPE_MIXED1,
                                              &encodetype));

  netwib_er(netwib_record_encodetype_init_kbd(NULL, NETWIB_FALSE,
                                              NETWIB_RECORD_ENCODETYPE_INIT_KBD_NODEF,
                                              &encodetype));

  netwib_er(netwib_record_encodetype_init_kbd(NULL, NETWIB_TRUE,
                                              NETWIB_RECORD_ENCODETYPE_HEXA0,
                                              &encodetype));

  netwib_er(netwib_record_encodetype_init_kbd(NULL, NETWIB_TRUE,
                                              NETWIB_RECORD_ENCODETYPE_INIT_KBD_NODEF,
                                              &encodetype));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_record_encodetype_test(void)
{
  netwib_buf buf;

  puts("++++----\nTest of netwib_buf_append_record_encodetype");
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_buf_append_record_encodetype(NETWIB_RECORD_ENCODETYPE_HEXA0, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "hexadecimal (not spaced)"));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_record_test(void)
{
  netwib_buf buf;
  netwib_io *pio;
  netwib_io_restype res;
  netwib_bool b;
  netwib_err ret;

  puts("++++----\nTest of netwib_io_init_record");

  netwib_er(netwib_buf_init_ext_string("./files/rec1_mixed", &buf));
  netwib_er(netwib_filename_exists(&buf, &b));
  if (b) {
    netwib_er(netwib_filename_remove(&buf));
  }
  netwib_er(netwib_io_init_record_write(&buf, NETWIB_RECORD_ENCODETYPE_HEXA0_WRAP,
                                        &pio));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_filename_exists(&buf, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_init_record_read(&buf, &pio));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_io_init_record_append(&buf, &pio));
  netwib_er(netwib_io_ctl_get_res(pio, NETWIB_IO_WAYTYPE_READ, &res));
  if (res != NETWIB_IO_RESTYPE_RECORD) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_filename_remove(&buf));

  /**/
  netwib_er(netwib_buf_init_ext_string("./files/rec2_bin", &buf));
  netwib_er(netwib_filename_exists(&buf, &b));
  if (b) {
    netwib_er(netwib_filename_remove(&buf));
  }
  netwib_er(netwib_io_init_record_write(&buf, NETWIB_RECORD_ENCODETYPE_BIN,
                                        &pio));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_filename_exists(&buf, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_init_record_read(&buf, &pio));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_io_init_record_append(&buf, &pio));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_filename_remove(&buf));

  /**/
  netwib_er(netwib_buf_init_ext_string("./files/rec3_pcap", &buf));
  netwib_er(netwib_filename_exists(&buf, &b));
  if (b) {
    netwib_er(netwib_filename_remove(&buf));
  }
  if (netwibtest_testadm) {
    ret = netwib_io_init_record_write(&buf, NETWIB_RECORD_ENCODETYPE_PCAP,
                                      &pio);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_io_close(&pio));
      netwib_er(netwib_filename_exists(&buf, &b));
      if (!b) {
        return(NETWIBTEST_ERR_7);
      }
      netwib_er(netwib_io_init_record_read(&buf, &pio));
      netwib_er(netwib_io_close(&pio));
      netwib_eir(netwib_io_init_record_append(&buf, &pio),
                 NETWIB_ERR_LONOTIMPLEMENTED);
      netwib_er(netwib_filename_remove(&buf));
    } else if (ret != NETWIB_ERR_LONOTSUPPORTED) {
      return(ret);
    }
  }

  /**/
  netwib_er(netwib_buf_init_ext_string("./files/qsdfqsdf", &buf));
  netwib_eir(netwib_io_init_record_read(&buf, &pio), NETWIB_ERR_NOTFOUND);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_record(netwib_record_encodetype encodetype)
{
  netwib_buf buf, bufdata;
  netwib_io *pio;
  netwib_bool b;

  puts("++++----\nTest of read, write");

  netwib_er(netwib_buf_init_ext_string("./files/record.tmp", &buf));
  netwib_er(netwib_filename_exists(&buf, &b));
  if (b) {
    netwib_er(netwib_filename_remove(&buf));
  }

  netwib_er(netwib_io_init_record_write(&buf, encodetype, &pio));
  netwib_er(netwib_buf_init_ext_string("hello1", &bufdata));
  netwib_er(netwib_io_write(pio, &bufdata));
  netwib_er(netwib_buf_init_ext_string("hello2", &bufdata));
  netwib_er(netwib_io_write(pio, &bufdata));
  if (encodetype != NETWIB_RECORD_ENCODETYPE_PCAP) {
    netwib_er(netwib_buf_init_ext_string("", &bufdata));
    netwib_er(netwib_io_write(pio, &bufdata));
  }
  netwib_er(netwib_io_close(&pio));

  if (encodetype != NETWIB_RECORD_ENCODETYPE_PCAP) {
    netwib_er(netwib_io_init_record_append(&buf, &pio));
    netwib_er(netwib_buf_init_ext_string("hello3", &bufdata));
    netwib_er(netwib_io_write(pio, &bufdata));
    netwib_er(netwib_buf_init_ext_string("hello4", &bufdata));
    netwib_er(netwib_io_write(pio, &bufdata));
    netwib_er(netwib_io_close(&pio));
  }

  netwib_er(netwib_buf_init_mallocdefault(&bufdata));
  netwib_er(netwib_io_init_record_read(&buf, &pio));
  netwib_er(netwib_io_read(pio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'hello1'"));
  netwib__buf_reinit(&bufdata);
  netwib_er(netwib_io_read(pio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'hello2'"));
  if (encodetype != NETWIB_RECORD_ENCODETYPE_PCAP) {
    netwib__buf_reinit(&bufdata);
    netwib_er(netwib_io_read(pio, &bufdata));
    netwib_er(netwibtest_buf_content3(&bufdata, "''"));
    netwib__buf_reinit(&bufdata);
    netwib_er(netwib_io_read(pio, &bufdata));
    netwib_er(netwibtest_buf_content3(&bufdata, "'hello3'"));
    netwib__buf_reinit(&bufdata);
    netwib_er(netwib_io_read(pio, &bufdata));
    netwib_er(netwibtest_buf_content3(&bufdata, "'hello4'"));
  }
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_buf_close(&bufdata));

  netwib_er(netwib_filename_remove(&buf));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_record(void)
{
  netwib_err ret;

  puts("###########################################\nTest of sys/record");
  if (netwibtest_testkbdlevel == 2) {
    netwib_er(netwib_record_encodetype_init_kbd_test());
  }
  netwib_er(netwib_buf_append_record_encodetype_test());
  netwib_er(netwib_io_init_record_test());

#if NETWIBDEF_SUPPORT_ENCODETYPE_PCAP == 1
  if (netwibtest_testadm) {
    ret = test_record(NETWIB_RECORD_ENCODETYPE_PCAP);
    if (ret != NETWIB_ERR_OK && ret != NETWIB_ERR_LONOTSUPPORTED) {
      return(ret);
    }
  }
#endif
  netwib_er(test_record(NETWIB_RECORD_ENCODETYPE_BIN));
  netwib_er(test_record(NETWIB_RECORD_ENCODETYPE_HEXA0));
  netwib_er(test_record(NETWIB_RECORD_ENCODETYPE_HEXA1));
  netwib_er(test_record(NETWIB_RECORD_ENCODETYPE_HEXA2));
  netwib_er(test_record(NETWIB_RECORD_ENCODETYPE_HEXA4));
  netwib_er(test_record(NETWIB_RECORD_ENCODETYPE_MIXED0));
  netwib_er(test_record(NETWIB_RECORD_ENCODETYPE_MIXED1));
  netwib_er(test_record(NETWIB_RECORD_ENCODETYPE_HEXA0_WRAP));
  netwib_er(test_record(NETWIB_RECORD_ENCODETYPE_HEXA1_WRAP));
  netwib_er(test_record(NETWIB_RECORD_ENCODETYPE_HEXA2_WRAP));
  netwib_er(test_record(NETWIB_RECORD_ENCODETYPE_HEXA4_WRAP));
  netwib_er(test_record(NETWIB_RECORD_ENCODETYPE_MIXED0_WRAP));
  netwib_er(test_record(NETWIB_RECORD_ENCODETYPE_MIXED1_WRAP));
  netwib_er(test_record(NETWIB_RECORD_ENCODETYPE_DUMP));
  netwib_er(test_record(NETWIB_RECORD_ENCODETYPE_MIXED0H_WRAP));
  netwib_er(test_record(NETWIB_RECORD_ENCODETYPE_MIXED1H_WRAP));

  ret = 0; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
